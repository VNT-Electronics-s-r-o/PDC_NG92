#include "lvgl.h"
#if LV_USE_TFT_ESPI
	#include <TFT_eSPI.h>
#endif

#define NUM_OBJECTS 20
#define V2_1
#define TFT_LED 33

/*Change to your screen resolution*/
static const uint16_t HOR_RES		= 320;
static const uint16_t VER_RES		= 480;


#define DRAW_BUF_SIZE (HOR_RES * VER_RES / 10 * (LV_COLOR_DEPTH / 8))
#define TFT_ROTATION  LV_DISPLAY_ROTATION_180




uint32_t draw_buf[DRAW_BUF_SIZE / 4];
TFT_eSPI tft = TFT_eSPI(HOR_RES, VER_RES);

lv_obj_t * objects[NUM_OBJECTS];  // Pole pro uložení referencí na 20 objektů

static uint32_t my_tick(void)
{
    return millis();
}

#if LV_USE_LOG != 0
void my_print( lv_log_level_t level, const char * buf )
{
    LV_UNUSED(level);
    Serial.println(buf);
    Serial.flush();
}
#endif


void G_Grafika_Init()
{
    String LVGL_PDC_NG = "LVGL:";
	LVGL_PDC_NG += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
    Serial.println(LVGL_PDC_NG);


    pinMode(TFT_LED, OUTPUT);
	Serial.println("LED pin set to 1");
	digitalWrite(TFT_LED, 1);

    
	lv_init();
	tft.begin();		/*TFT Init*/
	tft.setRotation(3);	/*TFT Rotation*/
	lv_tick_set_cb(my_tick);
    #if LV_USE_LOG != 0
        lv_log_register_print_cb( my_print );
    #endif

    lv_display_t * disp;	
    
    disp = lv_tft_espi_create(HOR_RES, VER_RES, draw_buf, sizeof(draw_buf));
    lv_display_set_rotation(disp, TFT_ROTATION);  
}

void G_MainBackground()
{
    //Obarvit na bílo
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0xFFFFFF), LV_PART_MAIN);


}


void G_Create_Objects()
{
    for (int i = 0; i < NUM_OBJECTS; i++) {
        // Vytvoření objektů, např. tlačítek
        objects[i] = lv_btn_create(lv_scr_act());  // Vytvoření tlačítka na hlavní obrazovce
        lv_obj_set_size(objects[i], 100, 50);     // Nastavení velikosti
        lv_obj_align(objects[i], LV_ALIGN_CENTER, i * 110, 0);  // Zarovnání a rozložení
    }
}


void G_Loop()
{
    lv_task_handler();
    delay(5);
}