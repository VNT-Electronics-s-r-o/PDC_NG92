#include "grafika.h"
#include <Arduino.h>
#include "lvgl.h"

// #include <SD.h>
// #include <SPI.h>

// #if LV_USE_TFT_ESPI
// #include <TFT_eSPI.h>
// #endif

#define V2_1
#define TFT_LED 33
#define SD_CS_PIN 4
#define TFT_ROTATION  LV_DISPLAY_ROTATION_180
#define NUM_OBJECTS 20

// /*Change to your screen resolution*/
static const uint16_t HOR_RES		= 320;
static const uint16_t VER_RES		= 480;

#define DRAW_BUF_SIZE (HOR_RES * VER_RES / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];
// TFT_eSPI tft = TFT_eSPI(HOR_RES, VER_RES);

uint32_t BarGraph_Color[5] = { 0x41A535, 0xB0BD2F, 0xFEED00, 0xE5610E, 0xCE1719};

lv_obj_t * Template_Objects[NUM_OBJECTS];  // Pole pro uložení referencí na 20 objektů


char item_strings[5][9] = {
    "OK",
    "",
    "",
    "!CHECK!",
    "!CHECK!"
};

char item_file[5][15] = {
    "B:/cloud.png",
    "B:/bt.png",
    "B:/wifi.png",
    "B:/lte.png",
    "B:/gps.png"   
};

// //static void dummy_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);

// //uint8_t * last_flushed_buf;

// static uint32_t my_tick(void)
// {
//     return millis();
// }

// #if LV_USE_LOG != 0
// void my_print( lv_log_level_t level, const char * buf )
// {
//     LV_UNUSED(level);
//     Serial.println(buf);
//     Serial.flush();
// }
// #endif

lv_obj_t *  SubMainScreen_Status();
void        G_Create_Template();

/*************************
    F U N C T I O N S
*************************/
static uint32_t my_tick(void)
{
    return millis();
}

void G_Grafika_Init()
{
	String LVGL_PDC_NG = "PDC_NG:";
	LVGL_PDC_NG += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

	Serial.println(LVGL_PDC_NG);

    pinMode(TFT_LED, OUTPUT);
	Serial.println("LED pin set to 1");
	digitalWrite(TFT_LED, 1);

    lv_init();

    lv_tick_set_cb(my_tick);

    lv_display_t * disp;	
    
    disp = lv_tft_espi_create(HOR_RES, VER_RES, draw_buf, sizeof(draw_buf));
    lv_display_set_rotation(disp, TFT_ROTATION);

//#if LV_USE_LOG != 0
    //lv_log_register_print_cb( my_print );
//#endif

    G_MainBackground();
}

void G_MainBackground()
{
   //Obarvit na bílo
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0xFFFFFF), LV_PART_MAIN);    
    G_Create_Template();
}

void G_Create_Template()
{
    for (int i = 0; i < NUM_OBJECTS; i++)
    {
        // Vytvoření objektů, např. tlačítek
        Template_Objects[i] = lv_obj_create(lv_scr_act());                                      // Vytvoření tlačítka na hlavní obrazovce
    }
    Template_Objects[0] = lv_obj_create(lv_screen_active());
    //Status lista
	//lv_obj_set_style_bg_color(Template_Objects[0], lv_color_hex(0xCCCCCC), LV_PART_MAIN);
    lv_obj_set_size(Template_Objects[0], 320, 60);
    lv_obj_align(Template_Objects[0], LV_ALIGN_TOP_MID, 0, -15);

    lv_obj_set_style_border_width(Template_Objects[0], 1, LV_PART_MAIN);                        // Nastavení šířky rámečku (2 pixely)
    lv_obj_set_style_border_color(Template_Objects[0], lv_color_black(), LV_PART_MAIN);         // Nastavení barvy rámečku (černá)       
    lv_obj_set_style_radius(Template_Objects[0], 10, LV_PART_MAIN);                             //Nastveni zaobleni
    lv_obj_set_style_pad_all(Template_Objects[0], 0, LV_PART_MAIN);
    lv_obj_set_style_pad_column(Template_Objects[0], 0, LV_PART_MAIN);                          // Zmenší mezeru na 5 pixelů

    Template_Objects[1] = lv_obj_create(Template_Objects[0]);     
    lv_obj_set_size(Template_Objects[1], 300, 40);
    lv_obj_align_to(Template_Objects[1], Template_Objects[0], LV_ALIGN_BOTTOM_MID, 0, -3);
    lv_obj_set_style_bg_opa(Template_Objects[1], LV_OPA_TRANSP, LV_PART_MAIN);                                      //Nastaveni pruhlednosti
    lv_obj_set_style_border_width(Template_Objects[1], 1, LV_PART_MAIN);                       
    lv_obj_set_style_border_color(Template_Objects[1], lv_color_black(), LV_PART_MAIN);        
    lv_obj_set_style_radius(Template_Objects[1], 0, LV_PART_MAIN);
    lv_obj_set_flex_flow(Template_Objects[1], LV_FLEX_FLOW_ROW);    
    lv_obj_set_flex_align(Template_Objects[1], LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_all(Template_Objects[1], 0, LV_PART_MAIN);
    lv_obj_set_style_pad_column(Template_Objects[1], 24, LV_PART_MAIN);                                             // Zmenší mezeru na 5 pixelů

    for (int i = 0; i<5; i++)
    {   
        Template_Objects[2+i] = lv_obj_create(Template_Objects[1]);                  
        lv_obj_set_size(Template_Objects[2+i], 40, 40);
        lv_obj_set_style_border_width(Template_Objects[2+i], 1, LV_PART_MAIN);                       
        lv_obj_set_style_border_color(Template_Objects[2+i], lv_color_hex(0x5E5E5C), LV_PART_MAIN); 
        //lv_obj_set_style_bg_color(Template_Objects[2+i], lv_color_hex(0x5E5E5C), LV_PART_MAIN);//Pozadi podle tabulky       
        lv_obj_set_style_bg_opa(Template_Objects[2+i], LV_OPA_COVER, LV_PART_MAIN); //Nastaveni pruhlednosti
        lv_obj_set_style_radius(Template_Objects[2+i], 10, LV_PART_MAIN);
        lv_obj_set_style_pad_all(Template_Objects[2+i], 0, LV_PART_MAIN);
    }
}