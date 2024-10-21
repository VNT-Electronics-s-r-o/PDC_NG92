#ifndef __grafika_h
#define __grafika_h

#include <Arduino.h>

void G_Grafika_Init();
void G_MainBackground();
void G_Loop();
void G_Update_FenceVaule(int value);
void G_UpdateChart(int max, int min, int threshold);


#endif