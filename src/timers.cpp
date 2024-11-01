#include "Timers.h"
#include "grafika.h"
#include "InputReader.h"

// Vytvoření instancí struktur pro jednotlivé timery
Timer Timer1 = {0, true};  // Timer s intervalem 100ms a opakováním
Timer Timer2 = {0, true};  // Timer s intervalem 1000ms a opakováním
Timer Timer3 = {0, false}; // Timer s intervalem 10000ms a bez opakování (jednorázový)

int rndMax= 110;
int rndMin= 0;
int randomInt;  // Vrací náhodné číslo mezi 101 a 109 (včetně 109)

#define TFT_LED 33
#define RELE 18

#define CNT_CHECKING_LIMIT 10
int Min = 110;
int Max = 0;
int CheckCnt = 0;
int randomSig = 83;

// Callbacky pro jednotlivé časovače
void _timer1Callback()
{
	//Serial.println("Timer1 - 100ms interval");
	
}

void _timer2Callback()
{
	Serial.println("Timer2 - 1000ms interval");
	
	
	if(CheckCnt==CNT_CHECKING_LIMIT)
	{
		G_UpdateChart(Max, Min, 70);

		CheckCnt = 0;
		Min = 110;
		Max = 0;
		rndMax = random(60, 110);
		rndMin = random(10, rndMax-5);
		randomSig = random(70, 95);  
		randomInt = random(100, 108);  // Vrací náhodné číslo mezi 101 a 109 (včetně 109)
		if((temporaryState == 1)||(temporaryState == 0))
		{
			randomInt = 0;
		}
	}
	else
	{
		CheckCnt++;
	}
	if(Min>randomInt)Min = randomInt;
	if(Max<randomInt)Max = randomInt;
	int helpOut = digitalRead(RELE);

	switch(temporaryState)
	{
		case 1:
			digitalWrite(TFT_LED, 1);
			G_Update_FenceVaule(randomInt, 0, randomSig);
			digitalWrite(RELE,0);
			break; 
		case 2:
			digitalWrite(TFT_LED, 1);
			G_Update_FenceVaule(randomInt, 50, randomSig);
			digitalWrite(RELE,1-helpOut);
			break; 
		case 3:
			digitalWrite(TFT_LED, 1);
			G_Update_FenceVaule(randomInt, 100, randomSig);
			digitalWrite(RELE,1-helpOut);
			break;
		default:
			G_Update_FenceVaule(randomInt, 0, randomSig);
			digitalWrite(RELE,0);
			digitalWrite(TFT_LED, 0);
			break;
	} 
    
}

void _timer3Callback()
{
	//Serial.println("Timer3 - 10000ms jednorázový");
}

// Inicializační funkce (pokud by bylo potřeba časem přidávat další konfigurace)
void InitTimers()
{
	pinMode(RELE, OUTPUT);
    Serial.println("InitTimers");
	unsigned long currentMillis = millis();
	//Timer1.Timer_lastMillis = currentMillis;
	Timer2.Timer_lastMillis = currentMillis;
	//Timer3.Timer_lastMillis = currentMillis;
}

// Funkce pro vyhodnocení timerů v hlavní smyčce
void EvaluateTimers()
{
  unsigned long currentMillis = millis();

	// // Timer1 - 5000ms interval
	// if ((currentMillis - Timer1.Timer_lastMillis) >= 5000)
	// {
	// 	Timer1.Timer_lastMillis = currentMillis;
	// 	_timer1Callback();
	// 	if (!Timer1.RepeatTimer)
	// 	{
	// 		Timer1.Timer_lastMillis = ULONG_MAX;  // deaktivace timeru
	// 	}
	// }

	// Timer2 - 1000ms interval
	if ((currentMillis - Timer2.Timer_lastMillis) >= 2000)
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


