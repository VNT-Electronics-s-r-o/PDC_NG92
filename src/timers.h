#ifndef __timers_h
#define __timers_h

#include <Arduino.h>

// Definice časových intervalů
#define TIMER_BASE 10  // základní interval 10ms

// Struktura pro timer
typedef struct
{
	unsigned long Timer_lastMillis;
	bool RepeatTimer;
} Timer;

// Deklarace funkcí
void InitTimers();      // Inicializace časovačů
void EvaluateTimers();  // Vyhodnocení časovačů

#endif