#include "lvgl.h"
#if LV_USE_TFT_ESPI
	#include <TFT_eSPI.h>
#endif

lv_obj_t * objects[NUM_OBJECTS];  // Pole pro uložení referencí na 20 objektů


void G_Create_Objects()
{
    for (int i = 0; i < NUM_OBJECTS; i++) {
        // Vytvoření objektů, např. tlačítek
        objects[i] = lv_btn_create(lv_scr_act());  // Vytvoření tlačítka na hlavní obrazovce
        lv_obj_set_size(objects[i], 100, 50);     // Nastavení velikosti
        lv_obj_align(objects[i], LV_ALIGN_CENTER, i * 110, 0);  // Zarovnání a rozložení
    }
}