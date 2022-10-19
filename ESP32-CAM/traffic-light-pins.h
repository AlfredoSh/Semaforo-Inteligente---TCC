#define RED1_PIN  14 // select digital pins for 1st light LEDs
// const int YELLOW1_PIN = 2;
#define GREEN1_PIN  2

#define RED2_PIN  15 // select digital pins for 2nd light LEDs
#define YELLOW2_PIN  12
#define GREEN2_PIN  13
#define LED_CAM 4

#define ON  HIGH // Remapping High/Low to On/Off to better match LED state
#define OFF  LOW
int traffic_light_state  = 0;
