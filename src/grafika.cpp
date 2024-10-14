#include "lvgl.h"
#include <SPI.h>

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
    
   
	//lv_example_flex_1();
    //GreyTitle();    
	Serial.println( "Setup done" );
}

lv_obj_t * SubMainScreen_Status()
{
    lv_obj_t * target = lv_obj_create(lv_screen_active());
    //Status lista
	lv_obj_set_style_bg_color(target, lv_color_hex(0xCCCCCC), LV_PART_MAIN);
    lv_obj_set_size(target, 320, 60);
    lv_obj_align(target, LV_ALIGN_TOP_MID, 0, -15);

    lv_obj_set_style_border_width(target, 2, LV_PART_MAIN);  // Nastavení šířky rámečku (2 pixely)
    lv_obj_set_style_border_color(target, lv_color_black(), LV_PART_MAIN);  // Nastavení barvy rámečku (černá)       
    lv_obj_set_style_radius(target, 10, LV_PART_MAIN); //Nastveni zaobleni
    lv_obj_set_style_pad_all(target, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_column(target, 0, LV_PART_MAIN);  // Zmenší mezeru na 5 pixelů

    lv_obj_t * icons_bar = lv_obj_create(target);     
    lv_obj_set_size(icons_bar, 300, 40);
    lv_obj_align_to(icons_bar, target, LV_ALIGN_BOTTOM_MID, 0, -3);
    lv_obj_set_style_bg_opa(icons_bar, LV_OPA_TRANSP, LV_PART_MAIN); //Nastaveni pruhlednosti
    lv_obj_set_style_border_width(icons_bar, 2, LV_PART_MAIN);                       
    lv_obj_set_style_border_color(icons_bar, lv_color_black(), LV_PART_MAIN);        
    lv_obj_set_style_radius(icons_bar, 0, LV_PART_MAIN);
     lv_obj_set_flex_flow(icons_bar, LV_FLEX_FLOW_ROW);
    //lv_obj_set_flex_align(cont_col_bot, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_flex_align(icons_bar, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_all(icons_bar, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_column(icons_bar, 24, LV_PART_MAIN);  // Zmenší mezeru na 5 pixelů

    // for (int i = 0; i<5; i++)
    // {
    //     //lv_obj_t * img = lv_img_create(icons_bar);
    //     //lv_img_set_src(img, item_file[i]);  // 'S' je označení souborového systému SD v LVGL

    //     StatusBar[i] = lv_obj_create(icons_bar);                  
    //     lv_obj_set_size(StatusBar[i], 40, 40);
    //     lv_obj_set_style_border_width(StatusBar[i], 2, LV_PART_MAIN);                       
    //     lv_obj_set_style_border_color(StatusBar[i], lv_color_hex(0x5E5E5C), LV_PART_MAIN); 
    //     lv_obj_set_style_bg_color(StatusBar[i], lv_color_hex(0x5E5E5C), LV_PART_MAIN);//Pozadi podle tabulky       
    //     lv_obj_set_style_bg_opa(StatusBar[i], LV_OPA_COVER, LV_PART_MAIN); //Nastaveni pruhlednosti
    //     lv_obj_set_style_radius(StatusBar[i], 10, LV_PART_MAIN);
    //     lv_obj_set_style_pad_all(StatusBar[i], 0, LV_PART_MAIN);
    // }

    return target;
}

void G_MainBackground()
{
    //Obarvit na bílo
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0xFFFFFF), LV_PART_MAIN);


    lv_obj_t * contRowStatus        = SubMainScreen_Status();
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