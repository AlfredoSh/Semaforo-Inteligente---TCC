#include "esp_camera.h"
#include <WiFi.h>
#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"
#include "semaforo_inteligente.h"

const char *ssid = "MotoOne";
const char *password = "motorola";

//Pins Digitais para cada LED dos semáforos
#define RED1_PIN  14 
#define GREEN1_PIN  2
#define RED2_PIN  15
#define YELLOW2_PIN  12
#define GREEN2_PIN  13
#define LED_CAM 4

extern int traffic_light_state;
extern int  ON;
extern int OFF;
int option;


const unsigned long period = 4000; 
unsigned long lastMillis;          
unsigned long currentMillis;

void startCameraServer();

void setup()
{

  Serial.begin(115200);
  pinMode(RED1_PIN, OUTPUT);
  pinMode(GREEN1_PIN, OUTPUT);
  pinMode(RED2_PIN, OUTPUT);
  pinMode(YELLOW2_PIN, OUTPUT);
  pinMode(GREEN2_PIN, OUTPUT);
  pinMode(LED_CAM, OUTPUT);

  lastMillis = millis(); 
  Serial.setDebugOutput(true);
  Serial.println();

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;


  if (psramFound())
  {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  }
  else
  {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }


  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK)
  {
    return;
  }

  sensor_t *s = esp_camera_sensor_get();
  if (s->id.PID == OV3660_PID)
  {
    s->set_vflip(s, 1);       
    s->set_brightness(s, 1);  
    s->set_saturation(s, -2); 
  }
  //Tamanho do Frame para reconhecimento de imagen
  s->set_framesize(s, FRAMESIZE_UXGA);


  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  startCameraServer();

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");

  while (true)
  {
    
    currentMillis = millis(); 
    if ((currentMillis - lastMillis >= period) || currentMillis == 100)
    {                             
      UpdateLights(false);       
      lastMillis = currentMillis;
      traffic_light_state++;                    
      if (traffic_light_state > 9)
        traffic_light_state = 0; 
    }
  }
}

void loop()
{
}
