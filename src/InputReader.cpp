/**
 * @file InputReader.cpp
 * @author Vaclav Silar (vaclav.silar@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-10-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "InputReader.h"

// Proměnné pro uložení časů a stavu vstupu
unsigned long lastChangeTime = 0;  // Poslední čas změny stavu
unsigned long lastCheckTime = 0;   // Poslední čas kontroly
bool lastState = LOW;              // Poslední čtený stav vstupu
bool isStable = true;              // Stav, zda je signál stabilní
uint8_t temporaryState = 0;        // Pomocná proměnná pro uložení stavu

// Funkce pro inicializaci
void InitInputReader()
{
	pinMode(DI_PIN, INPUT);  // Nastavení pinu jako vstupního
}

// Funkce pro vyhodnocení vstupu (základní smyčka)
uint8_t EvaluateInput()
{
	// Periodické čtení vstupu každých 100ms
	unsigned long currentMillis = millis();
	if (currentMillis - lastCheckTime >= 100)
	{
		lastCheckTime = currentMillis;

		bool currentState = digitalRead(DI_PIN);  // Čtení aktuálního stavu

		// Pokud došlo ke změně stavu
		if (currentState != lastState)
		{	
			
			unsigned long period = currentMillis - lastChangeTime;  // Výpočet periody mezi změnami
			lastChangeTime = currentMillis;  // Aktualizace času změny stavu
			lastState = currentState;        // Uložení nového stavu
			if(currentState == 0)
			{
				//Serial.printf("Perioda: %d\n", period);
				if( period < 800)
				{
					temporaryState = 2;
				}
				else if (period < 1200)
				{
					temporaryState = 3;
				}
			}			
		}
		// Kontrola, zda je signál stabilní
		if (isStable && (currentMillis - lastChangeTime > STABLE_THRESHOLD))
		{
			//Serial.printf("Stabilní signál (%d)\n",currentState);
			if(currentState == 1)
			{
				temporaryState = 0;
			}
			else
			{
				temporaryState = 1;
			}
		}
	}
	return temporaryState;
}