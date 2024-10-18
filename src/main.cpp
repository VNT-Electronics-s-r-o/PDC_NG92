#include <Arduino.h>
#include "lvgl.h"
#include <SD.h>
#include <SPI.h>
#include "grafika.h"
#include "timers.h"

#define SD_CS_PIN 4


#if LV_USE_LOG != 0
void my_print( lv_log_level_t level, const char * buf )
{
    LV_UNUSED(level);
    Serial.println(buf);
    Serial.flush();
}
#endif

void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
    Serial.printf("Výpis složky: %s\n", dirname);

    File root = fs.open(dirname);
    if (!root) {
        Serial.println("Nezdařilo se otevřít složku");
        return;
    }
    if (!root.isDirectory()) {
        Serial.println("Není to složka");
        return;
    }

    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            Serial.print(" [DIR] ");
            Serial.println(file.name());
            if (levels) {
                listDir(fs, file.name(), levels - 1);  // Rekurzivně výpis podadresářů
            }
        } else {
            Serial.print("Soubor: ");
            Serial.print(file.name());
            Serial.print("  Velikost: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

int SD_Test_Init()
{
    uint8_t cardType;
	uint64_t cardSize;

    if(!SD.begin(SD_CS_PIN))
    {
        Serial.println("Card Mount Failed");
        return 1;
    }
    Serial.println("Card Mount Successful");

    cardType = SD.cardType();

	if (cardType == CARD_NONE)
	{
		return -1;
	}
	cardSize = SD.cardSize() / (1024 * 1024);
	if (cardType == CARD_MMC)
	{
        Serial.println("SD Card Type: MMC, size: " + String(cardSize) + "Mb");
	}
	else if (cardType == CARD_SD)
	{
        Serial.println("SD Card Type: SDSC, size: " + String(cardSize) + "Mb");
	}
	else if (cardType == CARD_SDHC)
	{
        Serial.println("SD Card Type: SDHC, size: " + String(cardSize) + "Mb");
	}
	else
	{
        Serial.println("SD Card Type: UNKNOWN, size: " + String(cardSize) + "Mb");
	}
    listDir(SD, "/", 0);  // 0 znamená, že se nevypisují podadresáře
    return 0;
}

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

    SD_Test_Init();    
    G_Grafika_Init();
    InitTimers();			// Inicializace časovačů
	
    
    
	Serial.println( "Setup done" );

}

void loop()
{
	lv_timer_handler(); /* let the GUI do its work */
    EvaluateTimers();		// Vyhodnocení časovačů v každém cyklu smyčky	
	delay(15); /* let this time pass */
}