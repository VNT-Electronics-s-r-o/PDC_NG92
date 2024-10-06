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

// Funkce pro inicializaci
void InitInputReader()
{
	pinMode(DI_PIN, INPUT);  // Nastavení pinu jako vstupního
}

// Funkce pro vyhodnocení vstupu (základní smyčka)
void EvaluateInput()
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

			// Vyhodnocení blikání (50% střída)
			if (period >= 900 && period <= 1100)
			{
				Serial.println("50% střída s periodou 1 sekunda");
				isStable = false;
			}
			else if (period >= 3900 && period <= 4100)
			{
				Serial.println("50% střída s periodou 4 sekundy");
				isStable = false;
			}
			else
			{
				isStable = true;  // Pokud není perioda odpovídající blikání, nastavíme stav jako stabilní
			}
		}
		// Kontrola, zda je signál stabilní
		if (isStable && (currentMillis - lastChangeTime > STABLE_THRESHOLD))
		{
			Serial.println("Stabilní signál (logická 1)");
		}
	}
}