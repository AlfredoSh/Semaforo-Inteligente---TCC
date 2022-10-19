
#include "HardwareSerial.h"
#include "traffic-light-pins.h"

/**
 * Atualiza o comportamento do semáforo de acordo
 * ao reconhecimento de imagen
 * {bool} Verdadeiro para Cadeirante
 */
void UpdateLights(bool preferenceGreen)
{
    if (preferenceGreen && traffic_light_state >= 5 && traffic_light_state < 9)
    {
        Serial.print("Cadeirante Detectado");
        traffic_light_state = 9;
    }

    if (preferenceGreen && traffic_light_state < 5)
    {
        Serial.print("Cadeirante Detectado");
        traffic_light_state = 0;
    }

    switch (traffic_light_state)
    {

    // Sinal Verde para pedestres
    case 0:
        digitalWrite(GREEN1_PIN, ON);
        digitalWrite(YELLOW2_PIN, OFF);
        digitalWrite(RED2_PIN, ON);
        digitalWrite(RED1_PIN, OFF);
        digitalWrite(GREEN2_PIN, OFF);
        break;
    // Sinal Verde para veículos
    case 5:
        digitalWrite(GREEN2_PIN, ON);
        digitalWrite(RED2_PIN, OFF);
        digitalWrite(GREEN1_PIN, OFF);
        digitalWrite(RED1_PIN, ON);
        break;
    // Sinal Amarelo veículos
    case 9:
        digitalWrite(GREEN2_PIN, OFF);
        digitalWrite(YELLOW2_PIN, ON);
        digitalWrite(RED1_PIN, ON);
        break;
    }
}

/**
 * Controle da iluminação externa
 * {bool} Verdadeiro para ligar iluminação
 */
void iluminationHanlder(bool lightsState)
{

    if (lightsState)
    {
        Serial.print("External Lights ON \n");
        digitalWrite(LED_CAM, ON);
    }
    else
    {
        Serial.print("External Lights OFF \n");
        digitalWrite(LED_CAM, OFF);
    }
}