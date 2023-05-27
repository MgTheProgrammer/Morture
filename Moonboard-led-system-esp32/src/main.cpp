#include "bleServer.h"
#include "wifiServer.h"
#include <Arduino.h>

TaskHandle_t bleTask;
TaskHandle_t wifiTask;

void setup()
{
  xTaskCreatePinnedToCore(
                    bleServerMain,   /* Task function. */
                    "bleServer",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &bleTask,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  

  wifiServerMain(); // by default the main loop is on core 1
}
void loop()
{

}