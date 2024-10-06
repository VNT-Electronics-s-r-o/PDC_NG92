#ifndef __inputheader_h
#define __inputheader_h

#include <Arduino.h>

// Definice pinu pro digitální vstup a prahové hodnoty
#define DI_PIN 17  // Pin pro digitální vstup
#define STABLE_THRESHOLD 5000  // Prahová hodnota pro stabilní stav (5 sekund)

// Deklarace funkcí
void InitInputReader();  // Inicializace pro čtení vstupu
void EvaluateInput();    // Vyhodnocení stavu vstupu

#endif
