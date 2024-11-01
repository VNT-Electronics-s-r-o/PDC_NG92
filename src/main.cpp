#include <Arduino.h>
#include "lvgl.h"
#include <SD.h>
#include <SPI.h>
#include "grafika.h"
#include "timers.h"
#include "InputReader.h"

#define LVGL_TICK_PERIOD 5
#define TFT_LED 33


void lv_tick_task(void *arg);

void setup()
{
	Serial.begin(115200);
	while(!Serial){};

    Serial.print("Aktuální frekvence: ");
    Serial.print(getCpuFrequencyMhz());
    Serial.println(" MHz");
    randomSeed(analogRead(0));  // Pro lepší náhodnost, čte se hodnota z nepřipojeného analogového pinu

    // if(!SPIFFS.begin(true)){
    //     Serial.println("SPIFFS mount failed");
    //     return;
    // }
    // Serial.println("SPIFFS mount successful");

    pinMode(TFT_LED, OUTPUT);
    
	Serial.println("LED pin set to 1");
	digitalWrite(TFT_LED, 1);

    G_Grafika_Init();
    InitTimers();			// Inicializace časovačů 
	InitInputReader();		// Inicializace pro čtení vstupu
	Serial.println( "Setup done" );

}

void loop()
{
	lv_timer_handler();     /* let the GUI do its work */
    EvaluateTimers();		// Vyhodnocení časovačů v každém cyklu smyčky	
    EvaluateInput();		//
	delay(10);              /* let this time pass */
}
