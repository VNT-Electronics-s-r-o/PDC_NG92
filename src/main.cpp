#include <Arduino.h>
#include "lvgl.h"
#include <SD.h>
#include <SPI.h>
#include "grafika.h"
#include "timers.h"

#define LVGL_TICK_PERIOD 5

void lv_tick_task(void *arg);

void setup()
{
	Serial.begin(115200);
	while(!Serial){};
    randomSeed(analogRead(0));  // Pro lepší náhodnost, čte se hodnota z nepřipojeného analogového pinu

    // if(!SPIFFS.begin(true)){
    //     Serial.println("SPIFFS mount failed");
    //     return;
    // }
    // Serial.println("SPIFFS mount successful");

    G_Grafika_Init();
    InitTimers();			// Inicializace časovačů     
	Serial.println( "Setup done" );

}

void loop()
{
	lv_timer_handler();     /* let the GUI do its work */
    EvaluateTimers();		// Vyhodnocení časovačů v každém cyklu smyčky	
	delay(10);              /* let this time pass */
}
