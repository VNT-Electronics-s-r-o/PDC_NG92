#include "Timers.h"
#include "grafika.h"

// Vytvoření instancí struktur pro jednotlivé timery
Timer Timer1 = {0, true};  // Timer s intervalem 100ms a opakováním
Timer Timer2 = {0, true};  // Timer s intervalem 1000ms a opakováním
Timer Timer3 = {0, false}; // Timer s intervalem 10000ms a bez opakování (jednorázový)

// Callbacky pro jednotlivé časovače
void _timer1Callback()
{
	//Serial.println("Timer1 - 100ms interval");
}

void _timer2Callback()
{
	Serial.println("Timer2 - 1000ms interval");
    int randomInt = random(10, 110);  // Vrací náhodné číslo mezi 101 a 109 (včetně 109)
  
    //G_Update_FenceVaule(randomInt);
    // Převod na rozsah 10.1 - 10.9
    float randomFloat = randomInt / 10.0;
  
    // Výpis na sériový monitor
    Serial.println(randomFloat);
}

void _timer3Callback()
{
	//Serial.println("Timer3 - 10000ms jednorázový");
}

// Inicializační funkce (pokud by bylo potřeba časem přidávat další konfigurace)
void InitTimers()
{
    Serial.println("InitTimers");
	unsigned long currentMillis = millis();
//	Timer1.Timer_lastMillis = currentMillis;
	Timer2.Timer_lastMillis = currentMillis;
	//Timer3.Timer_lastMillis = currentMillis;
}

// Funkce pro vyhodnocení timerů v hlavní smyčce
void EvaluateTimers()
{
  unsigned long currentMillis = millis();

	// // Timer1 - 100ms interval
	// if ((currentMillis - Timer1.Timer_lastMillis) >= 100)
	// {
	// 	Timer1.Timer_lastMillis = currentMillis;
	// 	_timer1Callback();
	// 	if (!Timer1.RepeatTimer)
	// 	{
	// 		Timer1.Timer_lastMillis = ULONG_MAX;  // deaktivace timeru
	// 	}
	// }

	// Timer2 - 1000ms interval
	if ((currentMillis - Timer2.Timer_lastMillis) >= 1800)
	{
		Timer2.Timer_lastMillis = currentMillis;
		_timer2Callback();
		if (!Timer2.RepeatTimer)
		{
			Timer2.Timer_lastMillis = ULONG_MAX;
		}
	}

	// // Timer3 - 10000ms interval (jednorázový)
	// if ((currentMillis - Timer3.Timer_lastMillis) >= 10000 && Timer3.RepeatTimer)
	// {
	// 	Timer3.Timer_lastMillis = currentMillis;
	// 	_timer3Callback();
	// 	Timer3.RepeatTimer = false;  // nastaveno na jednorázové volání
	// }
}


