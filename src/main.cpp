#include <Arduino.h>
#include "lvgl.h"

#if LV_USE_TFT_ESPI
	#include <TFT_eSPI.h>
#endif


#define V2_1
#define TFT_LED 33
#define TFT_ROTATION  LV_DISPLAY_ROTATION_180


/*Change to your screen resolution*/
static const uint16_t HOR_RES		= 320;
static const uint16_t VER_RES		= 480;

static lv_style_t style_red_btn;
static lv_style_t style_btn;

#define DRAW_BUF_SIZE (HOR_RES * VER_RES / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];
TFT_eSPI tft = TFT_eSPI(HOR_RES, VER_RES);

uint32_t BarGraph_Color[5] = { 0x41A535, 0xB0BD2F, 0xFEED00, 0xE5610E, 0xCE1719};

char item_strings[5][9] = {
    "OK",
    "",
    "",
    "!CHECK!",
    "!CHECK!"
};

//static void dummy_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);

//uint8_t * last_flushed_buf;

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

void lv_example_led_1(void)
{
    /*Create a LED and switch it OFF*/
    lv_obj_t * led1  = lv_led_create(lv_screen_active());
    lv_obj_align(led1, LV_ALIGN_CENTER, -80, 0);
    lv_led_off(led1);

    /*Copy the previous LED and set a brightness*/
    lv_obj_t * led2  = lv_led_create(lv_screen_active());
    lv_obj_align(led2, LV_ALIGN_CENTER, 0, 0);
    lv_led_set_brightness(led2, 150);
    lv_led_set_color(led2, lv_palette_main(LV_PALETTE_RED));

    /*Copy the previous LED and switch it ON*/
    lv_obj_t * led3  = lv_led_create(lv_screen_active());
    lv_obj_align(led3, LV_ALIGN_CENTER, 80, 0);
    lv_led_on(led3);
}

void lv_lable(void)
{
	lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x003a57), LV_PART_MAIN);
    lv_obj_t *label = lv_label_create( lv_screen_active() );
    lv_label_set_text( label, "Hello Arduino, I'm LVGL!" );
	lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
}

void lv_example_spinner_1(void)
{
    /*Create a spinner*/
    lv_obj_t * spinner = lv_spinner_create(lv_screen_active());
    lv_obj_set_size(spinner, 100, 100);
    lv_obj_center(spinner);
    lv_spinner_set_anim_params(spinner, 10000, 200);
}


static void set_angle(lv_obj_t  * obj, int32_t v)
{
    lv_arc_set_value(obj, v);
}

static void style_init(void)
{


}

void lv_example_arc_2(void)
{
	/*Create an Arc*/
    lv_obj_t * arc = lv_arc_create(lv_screen_active());
	lv_obj_set_style_arc_color(arc, lv_color_hex(0x4040FF), LV_PART_INDICATOR | LV_STATE_DEFAULT );
    lv_arc_set_rotation(arc, 270);
    lv_arc_set_bg_angles(arc, 0, 360);
    lv_obj_remove_style(arc, NULL, LV_PART_KNOB);   /*Be sure the knob is not displayed*/
    lv_obj_remove_flag(arc, LV_OBJ_FLAG_CLICKABLE);  /*To not allow adjusting by click*/
    lv_obj_center(arc);
    
    // lv_anim_set_duration(&a, 1000);
    // lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);    /*Just for the demo*/
    // lv_anim_set_repeat_delay(&a, 500);
    // lv_anim_set_values(&a, 0, 100);
    // lv_anim_start(&a);
}

void lv_FenceVoltage(void)
{
	/*Create an Arc*/
    lv_obj_t * arc = lv_arc_create(lv_screen_active());
	lv_obj_set_style_arc_color(arc, lv_color_hex(0x4040FF), LV_PART_INDICATOR | LV_STATE_DEFAULT );
    lv_arc_set_rotation(arc, 270);
    lv_arc_set_bg_angles(arc, 0, 360);
    lv_obj_remove_style(arc, NULL, LV_PART_KNOB);   /*Be sure the knob is not displayed*/
    lv_obj_remove_flag(arc, LV_OBJ_FLAG_CLICKABLE);  /*To not allow adjusting by click*/
    lv_obj_center(arc);
}

void lv_example_flex_1(void)
{
    // Nastaveni obrazovky na bilou
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0xFFFFFF), LV_PART_MAIN);

    //Status lista        
    lv_obj_t * cont_row_up = lv_obj_create(lv_screen_active());
	lv_obj_set_style_bg_color(cont_row_up, lv_color_hex(0xCCCCCC), LV_PART_MAIN);
    lv_obj_set_size(cont_row_up, 320, 60);
    lv_obj_align(cont_row_up, LV_ALIGN_TOP_MID, 0, -15);

    lv_obj_set_style_border_width(cont_row_up, 2, LV_PART_MAIN);                        // Nastavení šířky rámečku (2 pixely)
    lv_obj_set_style_border_color(cont_row_up, lv_color_black(), LV_PART_MAIN);         // Nastavení barvy rámečku (černá)       
    lv_obj_set_style_radius(cont_row_up, 10, LV_PART_MAIN);                             // Nastveni zaobleni
    
    //Napetí
    lv_obj_t * cont_row_mid = lv_obj_create(lv_screen_active());    
    lv_obj_set_size(cont_row_mid, 320, 220);
    lv_obj_align_to(cont_row_mid, cont_row_up, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    lv_obj_set_style_border_width(cont_row_mid, 0, LV_PART_MAIN);                       // Nastavení šířky rámečku (2 pixely)
    lv_obj_set_style_border_color(cont_row_mid, lv_color_black(), LV_PART_MAIN);        // Nastavení barvy rámečku (černá)       
    lv_obj_set_style_radius(cont_row_mid, 0, LV_PART_MAIN);                             // Nastveni zaobleni
    

    //Grey title
    lv_obj_t * cont_row_title = lv_obj_create(lv_screen_active());
    lv_obj_set_style_bg_color(cont_row_title, lv_color_hex(0xCCCCCC), LV_PART_MAIN);        
    lv_obj_set_size(cont_row_title, 320, 30);
    lv_obj_align_to(cont_row_title, cont_row_mid, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    lv_obj_set_style_border_width(cont_row_title, 2, LV_PART_MAIN);                     // Nastavení šířky rámečku (2 pixely)
    lv_obj_set_style_border_color(cont_row_title, lv_color_black(), LV_PART_MAIN);      // Nastavení barvy rámečku (černá)       
    lv_obj_set_style_radius(cont_row_title, 0, LV_PART_MAIN);                           //Nastveni zaobleni

    
    lv_obj_set_flex_flow(cont_row_title, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont_row_title, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);

    lv_obj_t * labelTitle;
    labelTitle = lv_label_create(cont_row_title);
    lv_label_set_text(labelTitle, "OUTPUT POwER");
    lv_obj_set_style_text_color(labelTitle, lv_color_hex(0x5E5E5C), LV_PART_MAIN);  // Barva písma bílá
    lv_obj_set_style_text_align(labelTitle, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);        
    

    ///*Create a container with COLUMN flex direction*/
    lv_obj_t * cont_col_bot = lv_obj_create(lv_screen_active());
    lv_obj_set_size(cont_col_bot, 315, 180);
    lv_obj_align_to(cont_col_bot, cont_row_title, LV_ALIGN_OUT_BOTTOM_MID, 0, -5);
    lv_obj_set_flex_flow(cont_col_bot, LV_FLEX_FLOW_COLUMN);
    //lv_obj_set_flex_align(cont_col_bot, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_flex_align(cont_col_bot, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    //lv_obj_set_style_pad_top(cont_col_bot, 2, LV_PART_MAIN); // Zmenšení odsazení na 5 px
    lv_obj_set_style_pad_all(cont_col_bot, 5, LV_PART_MAIN);
    lv_obj_set_style_radius(cont_col_bot, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_row(cont_col_bot, 2, LV_PART_MAIN);  // Zmenší mezeru na 5 pixelů
    lv_obj_set_style_border_width(cont_col_bot, 5, LV_PART_MAIN);  // Nastavení šířky rámečku (2 pixely)
    lv_obj_set_style_border_color(cont_col_bot, lv_color_white(), LV_PART_MAIN);  // Nastavení barvy rámečku (černá)       
    lv_obj_set_style_radius(cont_col_bot, 10, LV_PART_MAIN); //Nastveni zaobleni
    

    uint32_t i;
    for(i = 0; i < 5; i++)
	{
        lv_obj_t * label;
        label = lv_label_create(cont_col_bot);
        lv_label_set_text_fmt(label, "%s", item_strings[i]); //Vlozeni fotmatovaneho textu
        lv_obj_set_style_bg_color(label, lv_color_hex(BarGraph_Color[i]), LV_PART_MAIN);//Pozadi podle tabulky
        lv_obj_set_style_bg_opa(label, LV_OPA_COVER, LV_PART_MAIN); //Nastaveni pruhlednosti
        lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);  // Barva písma bílá
        lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);        
        lv_obj_set_style_radius(label, 5, LV_PART_MAIN); //Nastveni zaobleni
        //lv_obj_set_style_border_width(label, 2, LV_PART_MAIN);  // Nastavení šířky rámečku (2 pixely)
        //lv_obj_set_style_border_color(label, lv_color_black(), LV_PART_MAIN);  // Nastavení barvy rámečku (černá)        
        
        //lv_obj_set_style_border_side(label, LV_BORDER_SIDE_FULL, LV_PART_MAIN);  // Rámeček kolem celého objektu
        lv_obj_set_size(label, LV_PCT(100), 30);
        

        //lv_obj_set_flex_grow(label, 1);  // Štítek se roztáhne na maximální šířku v rámci flex kontejneru
        // Případně zmenšení odsazení (padding) okolo štítku
        
        
		
        ///*Add items to the column*/
        //obj = lv_button_create(cont_col_bot);
        //lv_obj_set_style_bg_color(obj, lv_color_hex(BarGraph_Color[i]), LV_PART_MAIN);	
        //lv_obj_set_size(obj, 300, 20);
        
        //lv_obj_set_size(label, 300, 20);
        //lv_obj_set_style_bg_color(label, lv_color_hex(BarGraph_Color[i]), LV_PART_MAIN);	
        //lv_obj_set_style_border_width(label, 2, LV_PART_MAIN);  // Nastavení šířky rámečku (2 px)
        //lv_obj_set_style_border_color(label, lv_color_hex(BarGraph_Color[i]), LV_PART_MAIN);  // Nastavení barvy rámečku (černá)
        //lv_obj_set_style_border_side(label, LV_BORDER_SIDE_FULL, LV_PART_MAIN); // Rámeček kolem celého objektu
        

        //lv_obj_center(label);
    }
}

void GreyTitle(void)
{
        //Grey title
    lv_obj_t * cont_row_title = lv_obj_create(lv_screen_active());
    lv_obj_set_style_bg_color(cont_row_title, lv_color_hex(0xCCCCCC), LV_PART_MAIN);        
    lv_obj_set_size(cont_row_title, 320, 30);
    lv_obj_align(cont_row_title, LV_ALIGN_TOP_MID, 0, 0);

    lv_obj_set_style_border_width(cont_row_title, 2, LV_PART_MAIN);                     // Nastavení šířky rámečku (2 pixely)
    lv_obj_set_style_border_color(cont_row_title, lv_color_black(), LV_PART_MAIN);      // Nastavení barvy rámečku (černá)       
    lv_obj_set_style_radius(cont_row_title, 0, LV_PART_MAIN);                           //Nastveni zaobleni

    
    lv_obj_set_flex_flow(cont_row_title, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont_row_title, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);

    lv_obj_t * labelTitle;
    labelTitle = lv_label_create(cont_row_title);
    lv_label_set_text(labelTitle, "OUTPUT POwER");
    lv_obj_set_style_text_color(labelTitle, lv_color_hex(0x5E5E5C), LV_PART_MAIN);  // Barva písma bílá
    lv_obj_set_style_text_align(labelTitle, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);      
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
    return target;
}

 lv_obj_t * SubMainScreen_FenceVoltage(lv_obj_t * dependOn)
{
    lv_obj_t * target = lv_obj_create(lv_screen_active());    
    lv_obj_set_size(target, 320, 230);
    lv_obj_align_to(target, dependOn, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    lv_obj_t * arc = lv_arc_create(target);
	lv_obj_set_style_arc_color(arc, lv_color_hex(0x4040FF), LV_PART_INDICATOR | LV_STATE_DEFAULT );
    lv_obj_set_size(arc, 200, 200);                             // Nastaví velikost oblouku na 150x150 px
    //lv_obj_set_style_arc_width(arc, 15, LV_PART_MAIN);          // Nastaví šířku čáry na 10 px
    //lv_obj_set_style_arc_width(arc, 25, LV_PART_INDICATOR);  // Šířka indikátoru na 15 px
    lv_obj_set_style_arc_color(arc, lv_color_hex(0xB2B92D), LV_PART_INDICATOR);  // Nastavení barvy indikátoru (zelená)
    lv_arc_set_rotation(arc, 245);
    lv_arc_set_bg_angles(arc, 0, 360);
    lv_arc_set_start_angle(arc, 0);
    lv_arc_set_value(arc, 50);  // Nastaví hodnotu na 50
    // lv_obj_remove_style(arc, NULL, LV_PART_KNOB);   /*Be sure the knob is not displayed*/
    // lv_obj_remove_flag(arc, LV_OBJ_FLAG_CLICKABLE);  /*To not allow adjusting by click*/
    lv_obj_center(arc);

    lv_obj_set_style_border_width(target, 2, LV_PART_MAIN);                       
    lv_obj_set_style_border_color(target, lv_color_black(), LV_PART_MAIN);        
    lv_obj_set_style_radius(target, 0, LV_PART_MAIN);

    lv_obj_t * value_area = lv_obj_create(target);
    lv_obj_set_size(value_area, 70, 70);
    lv_obj_align_to(value_area, target, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_border_width(value_area, 2, LV_PART_MAIN);                       
    lv_obj_set_style_border_color(value_area, lv_color_black(), LV_PART_MAIN);        
    lv_obj_set_style_radius(value_area, 0, LV_PART_MAIN);
     lv_obj_set_flex_flow(target, LV_FLEX_FLOW_ROW);
    //lv_obj_set_flex_align(cont_col_bot, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_flex_align(target, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);


    // lv_obj_t * labelTitle;
    // labelTitle = lv_label_create(target);
    // lv_label_set_text(labelTitle, "10.5");
    // lv_obj_set_style_text_font(labelTitle, &lv_font_montserrat_48, LV_PART_MAIN);
    // lv_obj_align(labelTitle,  LV_ALIGN_CENTER, 0, 0);
    // lv_obj_set_style_text_color(labelTitle, lv_color_hex(0x5E5E5C), LV_PART_MAIN);  // Barva písma bílá
    // lv_obj_set_style_text_align(labelTitle, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);







    return target;
}

lv_obj_t *  SubMainScreen_BarGraphTitle(lv_obj_t * dependOn)
{
    //Grey title
    lv_obj_t * target = lv_obj_create(lv_screen_active());
    lv_obj_set_style_bg_color(target, lv_color_hex(0xCCCCCC), LV_PART_MAIN);        
    lv_obj_set_style_pad_all(target, 2, LV_PART_MAIN);
    lv_obj_set_size(target, 320, 30);
    lv_obj_align_to(target, dependOn, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    

    lv_obj_set_style_border_width(target, 0, LV_PART_MAIN);                     // Nastavení šířky rámečku (2 pixely)
    //lv_obj_set_style_border_color(target, lv_color_black(), LV_PART_MAIN);      // Nastavení barvy rámečku (černá)       
    lv_obj_set_style_radius(target, 0, LV_PART_MAIN);                           //Nastveni zaobleni

    
    lv_obj_set_flex_flow(target, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(target, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);

    lv_obj_t * labelTitle;
    labelTitle = lv_label_create(target);
    lv_label_set_text(labelTitle, "OUTPUT POWER");
    lv_obj_set_style_text_color(labelTitle, lv_color_hex(0x5E5E5C), LV_PART_MAIN);  // Barva písma bílá
    lv_obj_set_style_text_align(labelTitle, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    return target;
}

lv_obj_t * SubMainScreen_BarGraph(lv_obj_t * dependOn)
{  
    ///*Create a container with COLUMN flex direction*/
    lv_obj_t * target = lv_obj_create(lv_screen_active());
    lv_obj_set_size(target, 315, 180);
    lv_obj_align_to(target, dependOn, LV_ALIGN_OUT_BOTTOM_MID, 0, -5);
    lv_obj_set_flex_flow(target, LV_FLEX_FLOW_COLUMN);
    //lv_obj_set_flex_align(cont_col_bot, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_flex_align(target, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    //lv_obj_set_style_pad_top(cont_col_bot, 2, LV_PART_MAIN); // Zmenšení odsazení na 5 px
    lv_obj_set_style_pad_all(target, 5, LV_PART_MAIN);
    lv_obj_set_style_radius(target, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_row(target, 2, LV_PART_MAIN);  // Zmenší mezeru na 5 pixelů
    lv_obj_set_style_border_width(target, 5, LV_PART_MAIN);  // Nastavení šířky rámečku (2 pixely)
    lv_obj_set_style_border_color(target, lv_color_white(), LV_PART_MAIN);  // Nastavení barvy rámečku (černá)       
    lv_obj_set_style_radius(target, 10, LV_PART_MAIN); //Nastveni zaobleni

    uint32_t i;
    for(i = 0; i < 5; i++)
	{
        lv_obj_t * label;
        label = lv_label_create(target);
        lv_label_set_text_fmt(label, "%s", item_strings[i]); //Vlozeni fotmatovaneho textu
        lv_obj_set_style_bg_color(label, lv_color_hex(BarGraph_Color[i]), LV_PART_MAIN);//Pozadi podle tabulky
        lv_obj_set_style_bg_opa(label, LV_OPA_COVER, LV_PART_MAIN); //Nastaveni pruhlednosti
        lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);  // Barva písma bílá
        lv_obj_align(label,  LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_style_pad_all(label, 7, LV_PART_MAIN);
        lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);        
        lv_obj_set_style_radius(label, 5, LV_PART_MAIN); //Nastveni zaobleni
        lv_obj_set_size(label, LV_PCT(100), 30);
    }
    return target;
}

void Main_Screen(void)
{
    //lv_obj_t * contRowStatus;
    //lv_obj_t * contRowFenceVoltage;
    //lv_obj_t * contRowTitle;
    //lv_obj_t * contRowBar;
    //Obarvit na bílo
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0xFFFFFF), LV_PART_MAIN);

    lv_obj_t * contRowStatus        = SubMainScreen_Status();
    lv_obj_t * contRowFenceVoltage  = SubMainScreen_FenceVoltage(contRowStatus);
    lv_obj_t * contRowTitle         = SubMainScreen_BarGraphTitle(contRowFenceVoltage);
    lv_obj_t * contRowBar           = SubMainScreen_BarGraph(contRowTitle);


    // lv_obj_t * img = lv_img_create(lv_scr_act());
    // // Cesta k SVG souboru uloženému ve SPIFFS
    // const char *png_path = "/wifi-icon.png";

    // // Ověření, že soubor existuje
    // if (SPIFFS.exists(png_path))
    // {
    //     Serial.println("SVG soubor existuje");
    //     // Načtení SVG obrázku z SPIFFS
    //     lv_img_set_src(img, png_path);        
    //     // Zarovnání obrázku na střed
    //     lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
    // }
    // else
    // {
    //     Serial.println("SVG soubor neexistuje");
    // }
    //SubMainScreen_FenceVoltage(contRowFenceVoltage, contRowStatus);
    //SubMainScreen_BarGraphTitle(contRowTitle, contRowFenceVoltage);

}

// void test (void)
// {
//         /*Create a container with ROW flex direction*/
//     lv_obj_t * cont_row_up = lv_obj_create(lv_screen_active());
// 	lv_obj_set_style_bg_color(cont_row_up, lv_color_hex(0xCCCCCC), LV_PART_MAIN);
//     lv_obj_set_size(cont_row_up, 320, 60);
//     lv_obj_align(cont_row_up, LV_ALIGN_TOP_MID, 0, -15);
//     //lv_obj_set_flex_flow(cont_row, LV_FLEX_FLOW_ROW);

// 	/*Create a container with ROW flex direction*/
//     lv_obj_t * cont_row_mid = lv_obj_create(lv_screen_active());
//     lv_obj_set_size(cont_row_mid, 320, 220);
//     lv_obj_align_to(cont_row_mid, cont_row_up, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

//     /*Create a container with ROW flex direction*/
//     lv_obj_t * cont_row_title = lv_obj_create(lv_screen_active());
//     lv_obj_set_style_bg_color(cont_row_title, lv_color_hex(0xCCCCCC), LV_PART_MAIN);
//     lv_obj_set_size(cont_row_title, 320, 30);
//     lv_obj_align_to(cont_row_title, cont_row_mid, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

//     ///*Create a container with COLUMN flex direction*/
//     lv_obj_t * cont_col_bot = lv_obj_create(lv_screen_active());
//     lv_obj_set_size(cont_col_bot, 320, 180);
//     lv_obj_align_to(cont_col_bot, cont_row_title, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
//     lv_obj_set_flex_flow(cont_col_bot, LV_FLEX_FLOW_COLUMN);
//     //lv_obj_set_flex_align(cont_col_bot, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
//     lv_obj_set_flex_align(cont_col_bot, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
//     //lv_obj_set_style_pad_top(cont_col_bot, 2, LV_PART_MAIN); // Zmenšení odsazení na 5 px
//     lv_obj_set_style_pad_all(cont_col_bot, 2, LV_PART_MAIN);
//     lv_obj_set_style_radius(cont_col_bot, 0, LV_PART_MAIN);
    
// }

void setup()
{
	Serial.begin(115200);
	while(!Serial){};

    if(!SPIFFS.begin(true)){
        Serial.println("SPIFFS mount failed");
        return;
    }
    Serial.println("SPIFFS mount successful");
	
	String LVGL_PDC_NG = "PDC_NG:";
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
    Main_Screen();
	Serial.println( "Setup done" );

}

void loop()
{
	lv_timer_handler(); /* let the GUI do its work */
	//delay(5); /* let this time pass */
}
