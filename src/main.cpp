#include <Arduino.h>

#include <SD.h>
#include <SPI.h>
#include "Timers.h"
#include "InputReader.h"
#include "grafika.h"



#define SD_CS_PIN 4




uint8_t GeneratorStatus = 5; // 0 = Zobrazil logo, 1 = Zobrazit hlavni obrazovku radio only, 2 50%, 3 100%
bool NewValue = false;



uint32_t BarGraph_Color[5] = { 0x41A535, 0xB0BD2F, 0xFEED00, 0xE5610E, 0xCE1719};






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

//static void dummy_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);

//uint8_t * last_flushed_buf;






// lv_obj_t * SubMainScreen_Status()
// {
//     lv_obj_t * target = lv_obj_create(lv_screen_active());
//     //Status lista
// 	lv_obj_set_style_bg_color(target, lv_color_hex(0xCCCCCC), LV_PART_MAIN);
//     lv_obj_set_size(target, 320, 60);
//     lv_obj_align(target, LV_ALIGN_TOP_MID, 0, -15);

//     lv_obj_set_style_border_width(target, 2, LV_PART_MAIN);  // Nastavení šířky rámečku (2 pixely)
//     lv_obj_set_style_border_color(target, lv_color_black(), LV_PART_MAIN);  // Nastavení barvy rámečku (černá)       
//     lv_obj_set_style_radius(target, 10, LV_PART_MAIN); //Nastveni zaobleni
//     lv_obj_set_style_pad_all(target, 0, LV_PART_MAIN);
//     lv_obj_set_style_pad_column(target, 0, LV_PART_MAIN);  // Zmenší mezeru na 5 pixelů

//     lv_obj_t * icons_bar = lv_obj_create(target);     
//     lv_obj_set_size(icons_bar, 300, 40);
//     lv_obj_align_to(icons_bar, target, LV_ALIGN_BOTTOM_MID, 0, -3);
//     lv_obj_set_style_bg_opa(icons_bar, LV_OPA_TRANSP, LV_PART_MAIN); //Nastaveni pruhlednosti
//     lv_obj_set_style_border_width(icons_bar, 2, LV_PART_MAIN);                       
//     lv_obj_set_style_border_color(icons_bar, lv_color_black(), LV_PART_MAIN);        
//     lv_obj_set_style_radius(icons_bar, 0, LV_PART_MAIN);
//      lv_obj_set_flex_flow(icons_bar, LV_FLEX_FLOW_ROW);
//     //lv_obj_set_flex_align(cont_col_bot, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
//     lv_obj_set_flex_align(icons_bar, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
//     lv_obj_set_style_pad_all(icons_bar, 0, LV_PART_MAIN);
//     lv_obj_set_style_pad_column(icons_bar, 24, LV_PART_MAIN);  // Zmenší mezeru na 5 pixelů

//     for (int i = 0; i<5; i++)
//     {
//         //lv_obj_t * img = lv_img_create(icons_bar);
//         //lv_img_set_src(img, item_file[i]);  // 'S' je označení souborového systému SD v LVGL

//         StatusBar[i] = lv_obj_create(icons_bar);                  
//         lv_obj_set_size(StatusBar[i], 40, 40);
//         lv_obj_set_style_border_width(StatusBar[i], 2, LV_PART_MAIN);                       
//         lv_obj_set_style_border_color(StatusBar[i], lv_color_hex(0x5E5E5C), LV_PART_MAIN); 
//         lv_obj_set_style_bg_color(StatusBar[i], lv_color_hex(0x5E5E5C), LV_PART_MAIN);//Pozadi podle tabulky       
//         lv_obj_set_style_bg_opa(StatusBar[i], LV_OPA_COVER, LV_PART_MAIN); //Nastaveni pruhlednosti
//         lv_obj_set_style_radius(StatusBar[i], 10, LV_PART_MAIN);
//         lv_obj_set_style_pad_all(StatusBar[i], 0, LV_PART_MAIN);
//     }

//     return target;
// }

//  lv_obj_t * SubMainScreen_FenceVoltage(lv_obj_t * dependOn)
// {
//     lv_obj_t * target = lv_obj_create(lv_screen_active());    
//     lv_obj_set_size(target, 320, 230);
//     lv_obj_align_to(target, dependOn, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
//     lv_obj_set_style_pad_all(target, 0, LV_PART_MAIN);

//     lv_obj_t * info_label = lv_obj_create(target);
//     lv_obj_set_size(info_label, 310, 80); 
//     lv_obj_align_to(info_label, target, LV_ALIGN_BOTTOM_MID, 0, 0);
//     lv_obj_set_style_border_width(info_label, 0, LV_PART_MAIN);                       
//     lv_obj_set_style_border_color(info_label, lv_color_black(), LV_PART_MAIN);        
//     lv_obj_set_style_pad_all(info_label, 0, LV_PART_MAIN);
//     lv_obj_set_style_radius(info_label, 0, LV_PART_MAIN);
//     lv_obj_set_flex_flow(info_label, LV_FLEX_FLOW_ROW);
//     //lv_obj_set_flex_align(cont_col_bot, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
//     lv_obj_set_flex_align(info_label, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);    
//     lv_obj_set_style_pad_column(info_label, 2, LV_PART_MAIN);  // Zmenší mezeru na 5 pixelů

//     lv_obj_t * info_bat = lv_obj_create(info_label);
//     lv_obj_set_size(info_bat, 60, 80); 
//     lv_obj_align_to(info_bat, info_label, LV_ALIGN_CENTER, 0, 0);
//     lv_obj_set_style_border_width(info_bat, 0, LV_PART_MAIN);                       
//     lv_obj_set_style_border_color(info_bat, lv_color_black(), LV_PART_MAIN);        
//     lv_obj_set_style_pad_all(info_bat, 0, LV_PART_MAIN);
//     lv_obj_set_style_radius(info_bat, 10, LV_PART_MAIN);
//     lv_obj_set_flex_flow(info_bat, LV_FLEX_FLOW_COLUMN);
//     //lv_obj_set_flex_align(cont_col_bot, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
//     lv_obj_set_flex_align(info_bat, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);    
//     lv_obj_set_style_pad_row(info_bat, 0, LV_PART_MAIN);  // Zmenší mezeru na 5 pixelů
    
//     //Info Baterie
//     //Baterie icon
//     // lv_obj_t * icon_bat = lv_obj_create(info_bat);
//     // lv_obj_set_size(icon_bat, 40, 40); 
//     // lv_obj_align_to(icon_bat, info_bat, LV_ALIGN_TOP_MID, 0, 0);
//     // lv_obj_set_style_border_width(icon_bat, 2, LV_PART_MAIN);                       
//     // lv_obj_set_style_border_color(icon_bat, lv_color_hex(0x5E5E5C), LV_PART_MAIN); 
//     // lv_obj_set_style_bg_color(icon_bat, lv_color_hex(0x5E5E5C), LV_PART_MAIN);//Pozadi podle tabulky       
//     // lv_obj_set_style_pad_all(icon_bat, 0, LV_PART_MAIN);
//     // lv_obj_set_style_radius(icon_bat, 20, LV_PART_MAIN);
    
//     lv_obj_t * img_bat = lv_img_create(info_bat);
//     lv_img_set_src(img_bat, "B:/batt_gr.png");  // 'S' je označení souborového systému SD v LVGL

//     //Baterie Value
//     lv_obj_t * value_bat = lv_obj_create(info_bat);
//     lv_obj_set_size(value_bat, 60, 30); 
//     lv_obj_align_to(value_bat, info_bat, LV_ALIGN_BOTTOM_MID, 0, 0);
//     lv_obj_set_style_border_width(value_bat, 0, LV_PART_MAIN);                       
//     lv_obj_set_style_border_color(value_bat, lv_color_black(), LV_PART_MAIN);        
//     lv_obj_set_style_pad_all(value_bat, 0, LV_PART_MAIN);
//     lv_obj_set_style_radius(value_bat, 10, LV_PART_MAIN);

//     lv_obj_t * value_bat_text = lv_label_create(value_bat);
//     lv_label_set_text(value_bat_text, "11.5 V");
//     lv_obj_set_style_text_color(value_bat_text, lv_color_hex(0x5E5E5C), LV_PART_MAIN);  // Barva písma bílá
//     lv_obj_set_style_pad_all(value_bat_text, 0, LV_PART_MAIN);
//     lv_obj_align(value_bat_text,  LV_ALIGN_CENTER, 0, 0);


//     lv_obj_t * info_space = lv_obj_create(info_label);
//     lv_obj_set_size(info_space, 180, 70); 
//     lv_obj_align_to(info_space, info_label, LV_ALIGN_CENTER, 0, 0);
//     lv_obj_set_style_border_width(info_space, 0, LV_PART_MAIN);                       
//     lv_obj_set_style_border_color(info_space, lv_color_black(), LV_PART_MAIN);        
//     lv_obj_set_style_pad_all(info_space, 0, LV_PART_MAIN);
//     lv_obj_set_style_radius(info_space, 10, LV_PART_MAIN);



//     lv_obj_t * info_signal = lv_obj_create(info_label);
//     lv_obj_set_size(info_signal, 60, 70); 
//     lv_obj_align_to(info_signal, info_label, LV_ALIGN_CENTER, 0, 0);
//     lv_obj_set_style_border_width(info_signal, 0, LV_PART_MAIN);                       
//     lv_obj_set_style_border_color(info_signal, lv_color_black(), LV_PART_MAIN);        
//     lv_obj_set_style_pad_all(info_signal, 0, LV_PART_MAIN);
//     lv_obj_set_style_radius(info_signal, 10, LV_PART_MAIN);

//     //Info Signal
//     //Signal icon
//     // lv_obj_t * icon_sig = lv_obj_create(info_signal);
//     // lv_obj_set_size(icon_sig, 40, 40); 
//     // lv_obj_align_to(icon_sig, info_signal, LV_ALIGN_TOP_MID, 0, 0);
//     // lv_obj_set_style_border_width(icon_sig, 2, LV_PART_MAIN);                       
//     // lv_obj_set_style_border_color(icon_sig, lv_color_hex(0x5E5E5C), LV_PART_MAIN);
//     // lv_obj_set_style_bg_color(icon_sig, lv_color_hex(0x5E5E5C), LV_PART_MAIN);//Pozadi podle tabulky
//     // lv_obj_set_style_pad_all(icon_sig, 0, LV_PART_MAIN);
//     // lv_obj_set_style_radius(icon_sig, 20, LV_PART_MAIN);

//     lv_obj_t * img_sig = lv_img_create(info_signal);
//     lv_img_set_src(img_sig, "B:/sig_gr.png");  // 'S' je označení souborového systému SD v LVGL
    
//     //Signal Value
//     lv_obj_t * value_sig = lv_obj_create(info_signal);
//     lv_obj_set_size(value_sig, 60, 30); 
//     lv_obj_align_to(value_sig, info_signal, LV_ALIGN_BOTTOM_MID, 0, 0);
//     lv_obj_set_style_border_width(value_sig, 0, LV_PART_MAIN);                       
//     lv_obj_set_style_border_color(value_sig, lv_color_black(), LV_PART_MAIN);        
//     lv_obj_set_style_pad_all(value_sig, 0, LV_PART_MAIN);
//     lv_obj_set_style_radius(value_sig, 10, LV_PART_MAIN);

//     lv_obj_t * value_sig_text = lv_label_create(value_sig);
//     lv_label_set_text(value_sig_text, "84%");
//     lv_obj_set_style_text_color(value_sig_text, lv_color_hex(0x5E5E5C), LV_PART_MAIN);  // Barva písma bílá
//     lv_obj_set_style_pad_all(value_sig_text, 0, LV_PART_MAIN);
//     lv_obj_align(value_sig_text,  LV_ALIGN_CENTER, 0, 0);

//     //Kolecko
//     lv_obj_t * arc = lv_arc_create(target);
// 	lv_obj_set_style_arc_color(arc, lv_color_hex(0x4040FF), LV_PART_INDICATOR | LV_STATE_DEFAULT );
//     lv_obj_set_size(arc, 200, 200);                             // Nastaví velikost oblouku na 150x150 px
//     //lv_obj_set_style_arc_width(arc, 15, LV_PART_MAIN);          // Nastaví šířku čáry na 10 px
//     //lv_obj_set_style_arc_width(arc, 25, LV_PART_INDICATOR);  // Šířka indikátoru na 15 px
//     lv_obj_set_style_arc_color(arc, lv_color_hex(0xB2B92D), LV_PART_INDICATOR);  // Nastavení barvy indikátoru (zelená)
//     lv_arc_set_rotation(arc, 245);
//     lv_arc_set_bg_angles(arc, 0, 360);
//     lv_arc_set_start_angle(arc, 0);
//     lv_arc_set_value(arc, 50);  // Nastaví hodnotu na 50
//     // lv_obj_remove_style(arc, NULL, LV_PART_KNOB);   /*Be sure the knob is not displayed*/
//     // lv_obj_remove_flag(arc, LV_OBJ_FLAG_CLICKABLE);  /*To not allow adjusting by click*/
//     lv_obj_center(arc);

//     lv_obj_set_style_border_width(target, 0, LV_PART_MAIN);                       
//     lv_obj_set_style_border_color(target, lv_color_black(), LV_PART_MAIN);        
//     lv_obj_set_style_radius(target, 0, LV_PART_MAIN);

//     lv_obj_t * value_area = lv_obj_create(target);
//     lv_obj_set_size(value_area, 160, 60);
//     lv_obj_align_to(value_area, target, LV_ALIGN_CENTER, 0, 0);
//     lv_obj_set_style_border_width(value_area, 0, LV_PART_MAIN);                       
//     lv_obj_set_style_border_color(value_area, lv_color_black(), LV_PART_MAIN);        
//     lv_obj_set_style_radius(value_area, 0, LV_PART_MAIN);
//      lv_obj_set_flex_flow(value_area, LV_FLEX_FLOW_ROW);
//     //lv_obj_set_flex_align(cont_col_bot, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
//     lv_obj_set_flex_align(value_area, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
//     lv_obj_set_style_pad_all(value_area, 2, LV_PART_MAIN);
//     lv_obj_set_style_pad_column(value_area, 2, LV_PART_MAIN);  // Zmenší mezeru na 5 pixelů

//     lv_obj_t * type_area = lv_obj_create(target);
//     lv_obj_set_size(type_area, 150, 25);
//     lv_obj_align_to(type_area, value_area, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
//     lv_obj_set_style_bg_opa(type_area, LV_OPA_TRANSP, LV_PART_MAIN); //Nastaveni pruhlednosti
//     lv_obj_set_style_border_width(type_area, 0, LV_PART_MAIN);                       
//     lv_obj_set_style_border_color(type_area, lv_color_black(), LV_PART_MAIN);       
//     lv_obj_set_style_pad_all(type_area, 0, LV_PART_MAIN);
    
//     lv_obj_t * labelType;
//     labelType = lv_label_create(type_area);
//     lv_label_set_text(labelType, "OUTPUT POWER");
//     lv_obj_set_style_text_font(labelType, &lv_font_montserrat_14, LV_PART_MAIN);
//     lv_obj_align(labelType,  LV_ALIGN_CENTER, 0, 0);
//     lv_obj_set_style_pad_all(labelType, 0, LV_PART_MAIN);    
//     lv_obj_set_style_text_color(labelType, lv_color_hex(0x5E5E5C), LV_PART_MAIN);  // Barva písma bílá
//     lv_obj_set_style_text_align(labelType, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);


//     lv_obj_t * labelValue;
//     labelValue = lv_label_create(value_area);
//     lv_label_set_text(labelValue, "10.5");
//     lv_obj_set_style_text_font(labelValue, &lv_font_montserrat_48, LV_PART_MAIN);
//     lv_obj_align(labelValue,  LV_ALIGN_CENTER, 0, 0);
//     lv_obj_set_style_pad_all(labelValue, 0, LV_PART_MAIN);
//     lv_obj_set_style_text_color(labelValue, lv_color_black(), LV_PART_MAIN);  // Barva písma bílá
//     lv_obj_set_style_text_align(labelValue, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

//     lv_obj_t * labelUnits;
//     labelUnits = lv_label_create(value_area);
//     lv_label_set_text(labelUnits, "kV");    
//     lv_obj_set_style_text_font(labelUnits, &lv_font_montserrat_24, LV_PART_MAIN);
//     lv_obj_align(labelUnits,  LV_ALIGN_CENTER, 0, 0);
//     lv_obj_set_style_pad_all(labelUnits, 0, LV_PART_MAIN);
//     lv_obj_set_style_text_color(labelUnits, lv_color_hex(0x5E5E5C), LV_PART_MAIN);  // Barva písma bílá
//     lv_obj_set_style_text_align(labelUnits, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
//     return target;
// }

// lv_obj_t *  SubMainScreen_BarGraphTitle(lv_obj_t * dependOn)
// {
//     //Grey title
//     lv_obj_t * target = lv_obj_create(lv_screen_active());
//     lv_obj_set_style_bg_color(target, lv_color_hex(0xCCCCCC), LV_PART_MAIN);        
//     lv_obj_set_style_pad_all(target, 2, LV_PART_MAIN);
//     lv_obj_set_size(target, 320, 30);
//     lv_obj_align_to(target, dependOn, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    

//     lv_obj_set_style_border_width(target, 0, LV_PART_MAIN);                     // Nastavení šířky rámečku (2 pixely)
//     //lv_obj_set_style_border_color(target, lv_color_black(), LV_PART_MAIN);      // Nastavení barvy rámečku (černá)       
//     lv_obj_set_style_radius(target, 0, LV_PART_MAIN);                           //Nastveni zaobleni

    
//     lv_obj_set_flex_flow(target, LV_FLEX_FLOW_COLUMN);
//     lv_obj_set_flex_align(target, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);

//     lv_obj_t * labelTitle;
//     labelTitle = lv_label_create(target);
//     lv_label_set_text(labelTitle, "OUTPUT POWER");
//     lv_obj_set_style_text_color(labelTitle, lv_color_hex(0x5E5E5C), LV_PART_MAIN);  // Barva písma bílá
//     lv_obj_set_style_text_align(labelTitle, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
//     return target;
// }

// lv_obj_t * SubMainScreen_BarGraph(lv_obj_t * dependOn)
// {  
//     ///*Create a container with COLUMN flex direction*/
//     lv_obj_t * target = lv_obj_create(lv_screen_active());
//     lv_obj_set_size(target, 315, 180);
//     lv_obj_align_to(target, dependOn, LV_ALIGN_OUT_BOTTOM_MID, 0, -5);
//     lv_obj_set_flex_flow(target, LV_FLEX_FLOW_COLUMN);
//     //lv_obj_set_flex_align(cont_col_bot, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
//     lv_obj_set_flex_align(target, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
//     //lv_obj_set_style_pad_top(cont_col_bot, 2, LV_PART_MAIN); // Zmenšení odsazení na 5 px
//     lv_obj_set_style_pad_all(target, 5, LV_PART_MAIN);
//     lv_obj_set_style_radius(target, 0, LV_PART_MAIN);
//     lv_obj_set_style_pad_row(target, 2, LV_PART_MAIN);  // Zmenší mezeru na 5 pixelů
//     lv_obj_set_style_border_width(target, 5, LV_PART_MAIN);  // Nastavení šířky rámečku (2 pixely)
//     lv_obj_set_style_border_color(target, lv_color_white(), LV_PART_MAIN);  // Nastavení barvy rámečku (černá)       
//     lv_obj_set_style_radius(target, 10, LV_PART_MAIN); //Nastveni zaobleni

//     uint32_t i;
//     for(i = 0; i < 5; i++)
// 	{
//         lv_obj_t * label;
//         label = lv_label_create(target);
//         lv_label_set_text_fmt(label, "%s", item_strings[i]); //Vlozeni fotmatovaneho textu
//         lv_obj_set_style_bg_color(label, lv_color_hex(BarGraph_Color[i]), LV_PART_MAIN);//Pozadi podle tabulky
//         lv_obj_set_style_bg_opa(label, LV_OPA_COVER, LV_PART_MAIN); //Nastaveni pruhlednosti
//         lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);  // Barva písma bílá
//         lv_obj_align(label,  LV_ALIGN_CENTER, 0, 0);
//         lv_obj_set_style_pad_all(label, 7, LV_PART_MAIN);
//         lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);        
//         lv_obj_set_style_radius(label, 5, LV_PART_MAIN); //Nastveni zaobleni
//         lv_obj_set_size(label, LV_PCT(100), 30);
//     }
//     return target;
// }

// void Main_Screen(void)
// {
//     //Obarvit na bílo
//     lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0xFFFFFF), LV_PART_MAIN);


    

//     lv_obj_t * contRowStatus        = SubMainScreen_Status();
//     //lv_obj_t * contRowFenceVoltage  = SubMainScreen_FenceVoltage(contRowStatus);
//     //lv_obj_t * contRowTitle         = SubMainScreen_BarGraphTitle(contRowFenceVoltage);
//     //lv_obj_t * contRowBar           = SubMainScreen_BarGraph(contRowTitle);    
// }


void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
    Serial.printf("Výpis složky: %s\n", dirname);

    File root = fs.open(dirname);
    if (!root) {
        Serial.println("Nezdařilo se otevřít složku");
        return;
    }
    if (!root.isDirectory()) {
        Serial.println("Není to složka");
        return;
    }

    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            Serial.print(" [DIR] ");
            Serial.println(file.name());
            if (levels) {
                listDir(fs, file.name(), levels - 1);  // Rekurzivně výpis podadresářů
            }
        } else {
            Serial.print("Soubor: ");
            Serial.print(file.name());
            Serial.print("  Velikost: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

int SD_Test_Init()
{
    uint8_t cardType;
	uint64_t cardSize;

    if(!SD.begin(SD_CS_PIN))
    {
        Serial.println("Card Mount Failed");
        return 1;
    }
    Serial.println("Card Mount Successful");

    cardType = SD.cardType();

	if (cardType == CARD_NONE)
	{
		return -1;
	}
	cardSize = SD.cardSize() / (1024 * 1024);
	if (cardType == CARD_MMC)
	{
        Serial.println("SD Card Type: MMC, size: " + String(cardSize) + "Mb");
	}
	else if (cardType == CARD_SD)
	{
        Serial.println("SD Card Type: SDSC, size: " + String(cardSize) + "Mb");
	}
	else if (cardType == CARD_SDHC)
	{
        Serial.println("SD Card Type: SDHC, size: " + String(cardSize) + "Mb");
	}
	else
	{
        Serial.println("SD Card Type: UNKNOWN, size: " + String(cardSize) + "Mb");
	}
    listDir(SD, "/", 0);  // 0 znamená, že se nevypisují podadresáře
    return 0;
}

void setup()
{
	Serial.begin(115200);
	while(!Serial){};

    // if(!SPIFFS.begin(true)){
    //     Serial.println("SPIFFS mount failed");
    //     return;
    // }
    // Serial.println("SPIFFS mount successful");
	

    
	



    
    SD_Test_Init();

    G_Grafika_Init();
    G_MainBackground();

	
    
	//lv_example_flex_1();
    //GreyTitle();

	Serial.println( "Setup done" );
    // InitTimers();			// Inicializace časovačů
	// InitInputReader();		// Inicializace pro čtení vstupu

}

void loop()
{
    void G_Loop();
	//lv_timer_handler(); /* let the GUI do its work */
	//delay(5); /* let this time pass */
	// EvaluateTimers();		// Vyhodnocení časovačů v každém cyklu smyčky	
    // if(GeneratorStatus != EvaluateInput())
    // {
    //     GeneratorStatus = EvaluateInput();  
    //     NewValue = true;            
    // }
    // else if (NewValue == true)
    // {
    //     switch(GeneratorStatus)
    //     {
    //         case 0:
    //             Serial.println("Generator OFF");
    //             break;
    //         case 1:
    //             Serial.println("Generator Radio Only");
    //             break;
    //         case 2:
    //             Serial.println("Generator 50%");
    //             break;
    //         case 3:
    //             Serial.println("Generator 100%");
    //             break;
    //     }
    //     NewValue = false;
    // }    
    //delay(TIMER_BASE);		// Zachování neblokujícího provozu s časovou základnou 10ms
}
