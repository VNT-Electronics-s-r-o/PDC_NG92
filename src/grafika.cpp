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
#define NUM_ICON    5

#define HELP_BORDER_WIDTH 1


enum {
	eSTATUSBAR_AREA,
	eFENCE_INDICATOR_AREA,
	eBARGRAPH_TITLE_AREA,
	eBARGRAPH_AREA,
	// eFENCE_VOLTAGE_AREA = 2,
	// eINFO_LABEL = 3,
	
}MainTemplateObject_e;

enum{
	eSB_ICON_AREA = 0,

}StatusBarTemplateObject_e;

enum {
	eIL_INFO_AREA,
	eIL_BATTERY_AREA,
	eIL_INFO_SPACE_AREA,
	eIL_SIGNAL_AREA,
	eIL_BATERIE_ICO,
	eIL_SIGNAL_ICO,
	eIL_BATTERY_VALUE,
	eIL_SIGNAL_VALUE,
	eIL_ARC_FENCE,
	eIL_FENCE_VALUE_AREA,
	eIL_TYPE_AREA,
	eIL_TYPE_TEXT_LABEL,
	eIL_FENCE_TEXT_LABEL,
	eIL_UNIT_TEXT_LABEL,
	eIL_BATTERY_TEXT_LABEL,
	eIL_SIGNAL_TEXT_LABEL,
}InfoLabelTemplateObject_e;

enum {
	eBGT_TITLE,	
}BarGraphTeplateObjects_e;


// /*Change to your screen resolution*/
static const uint16_t HOR_RES		= 320;
static const uint16_t VER_RES		= 480;

#define DRAW_BUF_SIZE (HOR_RES * VER_RES / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];
// TFT_eSPI tft = TFT_eSPI(HOR_RES, VER_RES);

uint32_t BarGraph_Color[5] = { 0x41A535, 0xB0BD2F, 0xFEED00, 0xE5610E, 0xCE1719};

lv_obj_t * MainTemplate_Objects[NUM_OBJECTS];  // Pole pro uložení referencí na 20 objektů
lv_obj_t * StatusBarTemplate_Objects[NUM_ICON];  // Pole pro uložení referencí na 20 objektů

//lv_obj_t * StatusIconTemplate_Objects[NUM_ICON];  // Pole pro uložení referencí na 20 objektů
lv_obj_t * BarGraphTitle_Teplate_Objects[NUM_OBJECTS];
lv_obj_t * BarGraph_Teplate_Objects[NUM_OBJECTS];
lv_obj_t * InfoLabelTemplate_Objects[NUM_ICON];  // Pole pro uložení referencí na 20 objektů


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
void 		G_Template_StatusBar ();
void 		G_Template_FenceeVoltage();
void		G_Insert_Text();
void		G_Template_BarTitle();
void		G_Template_BarGraph();
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
	G_Template_StatusBar();
	G_Template_FenceeVoltage();
	G_Template_BarTitle();
	G_Template_BarGraph();


	G_Insert_Text();
}

void G_MainBackground()
{
	//Obarvit na bílo
	lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0xFFFFFF), LV_PART_MAIN);   
	//Status lista
	MainTemplate_Objects[eSTATUSBAR_AREA] = lv_obj_create(lv_screen_active());	
	//lv_obj_set_style_bg_color(Template_Objects[0], lv_color_hex(0xCCCCCC), LV_PART_MAIN);
	lv_obj_set_size(MainTemplate_Objects[eSTATUSBAR_AREA], 320, 60);
	lv_obj_align(MainTemplate_Objects[eSTATUSBAR_AREA], LV_ALIGN_TOP_MID, 0, -15);

	lv_obj_set_style_border_width(MainTemplate_Objects[eSTATUSBAR_AREA], HELP_BORDER_WIDTH, LV_PART_MAIN);        // Nastavení šířky rámečku (2 pixely)
	lv_obj_set_style_border_color(MainTemplate_Objects[eSTATUSBAR_AREA], lv_color_black(), LV_PART_MAIN);         // Nastavení barvy rámečku (černá)
	lv_obj_set_style_radius(MainTemplate_Objects[eSTATUSBAR_AREA], 10, LV_PART_MAIN);                             // Nastveni zaobleni
	lv_obj_set_style_pad_all(MainTemplate_Objects[eSTATUSBAR_AREA], 0, LV_PART_MAIN);
	lv_obj_set_style_pad_column(MainTemplate_Objects[eSTATUSBAR_AREA], 0, LV_PART_MAIN);                          // Zmenší mezeru na 5 pixelů

	//FenceVoltage area	
	MainTemplate_Objects[eFENCE_INDICATOR_AREA]= lv_obj_create(lv_screen_active());    
	lv_obj_set_size(MainTemplate_Objects[eFENCE_INDICATOR_AREA], 320, 230);
	lv_obj_align_to(MainTemplate_Objects[eFENCE_INDICATOR_AREA], MainTemplate_Objects[eSTATUSBAR_AREA], LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
	lv_obj_set_style_pad_all(MainTemplate_Objects[eFENCE_INDICATOR_AREA], 0, LV_PART_MAIN);
	lv_obj_set_style_border_width(MainTemplate_Objects[eFENCE_INDICATOR_AREA], HELP_BORDER_WIDTH, LV_PART_MAIN);							// Nastavení šířky rámečku (2 pixely)
	lv_obj_set_style_border_color(MainTemplate_Objects[eFENCE_INDICATOR_AREA], lv_color_black(), LV_PART_MAIN);							// Nastavení barvy rámečku (černá)

	//BarGraph Title
	MainTemplate_Objects[eBARGRAPH_TITLE_AREA] = lv_obj_create(lv_screen_active());
	//lv_obj_set_style_bg_color(MainTemplate_Objects[eBARGRAPH_TITLE], lv_color_hex(0xCCCCCC), LV_PART_MAIN);        
	lv_obj_set_style_pad_all(MainTemplate_Objects[eBARGRAPH_TITLE_AREA], 2, LV_PART_MAIN);
	lv_obj_set_size(MainTemplate_Objects[eBARGRAPH_TITLE_AREA], 320, 30);
	lv_obj_set_style_border_width(MainTemplate_Objects[eBARGRAPH_TITLE_AREA], HELP_BORDER_WIDTH, LV_PART_MAIN);							// Nastavení šířky rámečku (2 pixely)
	lv_obj_set_style_border_color(MainTemplate_Objects[eBARGRAPH_TITLE_AREA], lv_color_black(), LV_PART_MAIN);         // Nastavení barvy rámečku (černá)
	lv_obj_align_to(MainTemplate_Objects[eBARGRAPH_TITLE_AREA], MainTemplate_Objects[eFENCE_INDICATOR_AREA], LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_style_radius(MainTemplate_Objects[eBARGRAPH_TITLE_AREA], 0, LV_PART_MAIN);                           //Nastveni zaobleni
    lv_obj_set_flex_flow(MainTemplate_Objects[eBARGRAPH_TITLE_AREA], LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(MainTemplate_Objects[eBARGRAPH_TITLE_AREA], LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);

	//BarGraph	
	MainTemplate_Objects[eBARGRAPH_AREA] = lv_obj_create(lv_screen_active());
	lv_obj_set_size(MainTemplate_Objects[eBARGRAPH_AREA], 315, 180);
	lv_obj_align_to(MainTemplate_Objects[eBARGRAPH_AREA], MainTemplate_Objects[eBARGRAPH_TITLE_AREA], LV_ALIGN_OUT_BOTTOM_MID, 0, -5);
	lv_obj_set_flex_flow(MainTemplate_Objects[eBARGRAPH_AREA], LV_FLEX_FLOW_COLUMN);
	//lv_obj_set_flex_align(cont_col_bot, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_flex_align(MainTemplate_Objects[eBARGRAPH_AREA], LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	//lv_obj_set_style_pad_top(cont_col_bot, 2, LV_PART_MAIN); // Zmenšení odsazení na 5 px
	lv_obj_set_style_pad_all(MainTemplate_Objects[eBARGRAPH_AREA], 5, LV_PART_MAIN);
	lv_obj_set_style_radius(MainTemplate_Objects[eBARGRAPH_AREA], 0, LV_PART_MAIN);
	lv_obj_set_style_pad_row(MainTemplate_Objects[eBARGRAPH_AREA], 2, LV_PART_MAIN);  // Zmenší mezeru na 5 pixelů
	lv_obj_set_style_border_width(MainTemplate_Objects[eBARGRAPH_AREA], 5, LV_PART_MAIN);  // Nastavení šířky rámečku (2 pixely)
	//lv_obj_set_style_border_color(MainTemplate_Objects[eBARGRAPH_AREA], lv_color_white(), LV_PART_MAIN);  // Nastavení barvy rámečku (černá)       
	lv_obj_set_style_border_color(MainTemplate_Objects[eBARGRAPH_AREA], lv_color_black(), LV_PART_MAIN);  // Nastavení barvy rámečku (černá)       
	lv_obj_set_style_radius(MainTemplate_Objects[eBARGRAPH_AREA], 10, LV_PART_MAIN); //Nastveni zaobleni    
}

void G_Create_Template()
{
	
	
	//G_Insert_Text();
	//G_Template_BarTitle();
}

void G_Template_StatusBar ()
{
	StatusBarTemplate_Objects[eSB_ICON_AREA] = lv_obj_create(MainTemplate_Objects[eSTATUSBAR_AREA]);     
	lv_obj_set_size(StatusBarTemplate_Objects[eSB_ICON_AREA], 300, 42);
	lv_obj_align_to(StatusBarTemplate_Objects[eSB_ICON_AREA], MainTemplate_Objects[eSTATUSBAR_AREA], LV_ALIGN_BOTTOM_MID, 0, -3);
	lv_obj_set_style_bg_opa(StatusBarTemplate_Objects[eSB_ICON_AREA], LV_OPA_TRANSP, LV_PART_MAIN);                                      //Nastaveni pruhlednosti
	lv_obj_set_style_border_width(StatusBarTemplate_Objects[eSB_ICON_AREA], HELP_BORDER_WIDTH, LV_PART_MAIN);                       
	lv_obj_set_style_border_color(StatusBarTemplate_Objects[eSB_ICON_AREA], lv_color_black(), LV_PART_MAIN);        
	lv_obj_set_style_radius(StatusBarTemplate_Objects[eSB_ICON_AREA], 0, LV_PART_MAIN);
	lv_obj_set_flex_flow(StatusBarTemplate_Objects[eSB_ICON_AREA], LV_FLEX_FLOW_ROW);    
	lv_obj_set_flex_align(StatusBarTemplate_Objects[eSB_ICON_AREA], LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_all(StatusBarTemplate_Objects[eSB_ICON_AREA], 0, LV_PART_MAIN);
	lv_obj_set_style_pad_column(StatusBarTemplate_Objects[eSB_ICON_AREA], 24, LV_PART_MAIN);                                             // Zmenší mezeru na 5 pixelů

	for (int i = 0; i<5; i++)
	{
		StatusBarTemplate_Objects[1+i] = lv_obj_create(StatusBarTemplate_Objects[eSB_ICON_AREA]);                  
		lv_obj_set_size(StatusBarTemplate_Objects[1+i], 40, 40);
		lv_obj_set_style_border_width(StatusBarTemplate_Objects[1+i], HELP_BORDER_WIDTH, LV_PART_MAIN);                       
		lv_obj_set_style_border_color(StatusBarTemplate_Objects[1+i], lv_color_hex(0x5E5E5C), LV_PART_MAIN); 
		//lv_obj_set_style_bg_color(Template_Objects[2+i], lv_color_hex(0x5E5E5C), LV_PART_MAIN);//Pozadi podle tabulky       
		lv_obj_set_style_bg_opa(StatusBarTemplate_Objects[1+i], LV_OPA_COVER, LV_PART_MAIN); //Nastaveni pruhlednosti
		lv_obj_set_style_radius(StatusBarTemplate_Objects[1+i], 10, LV_PART_MAIN);
		lv_obj_set_style_pad_all(StatusBarTemplate_Objects[1+i], 0, LV_PART_MAIN);
	}
}

void G_Template_FenceeVoltage()
{
	//Info Label
	InfoLabelTemplate_Objects[eIL_INFO_AREA] = lv_obj_create(MainTemplate_Objects[eFENCE_INDICATOR_AREA]);
	lv_obj_set_size(InfoLabelTemplate_Objects[eIL_INFO_AREA], 310, 80); 
	lv_obj_align_to(InfoLabelTemplate_Objects[eIL_INFO_AREA], MainTemplate_Objects[eFENCE_INDICATOR_AREA], LV_ALIGN_BOTTOM_MID, 0, 0);
	lv_obj_set_style_border_width(InfoLabelTemplate_Objects[eIL_INFO_AREA], HELP_BORDER_WIDTH, LV_PART_MAIN);
	lv_obj_set_style_border_color(InfoLabelTemplate_Objects[eIL_INFO_AREA], lv_color_black(), LV_PART_MAIN);
	lv_obj_set_style_pad_all(InfoLabelTemplate_Objects[eIL_INFO_AREA], 0, LV_PART_MAIN);
	lv_obj_set_style_radius(InfoLabelTemplate_Objects[eIL_INFO_AREA], 0, LV_PART_MAIN);
	lv_obj_set_flex_flow(InfoLabelTemplate_Objects[eIL_INFO_AREA], LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align(InfoLabelTemplate_Objects[eIL_INFO_AREA], LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);    
	lv_obj_set_style_pad_column(InfoLabelTemplate_Objects[eIL_INFO_AREA], 2, LV_PART_MAIN);													// Zmenší mezeru na 5 pixelů


	//InfoBattery area
	InfoLabelTemplate_Objects[eIL_BATTERY_AREA] = lv_obj_create(InfoLabelTemplate_Objects[eIL_INFO_AREA]);
	lv_obj_set_size(InfoLabelTemplate_Objects[eIL_BATTERY_AREA], 62, 78); 
	lv_obj_align_to(InfoLabelTemplate_Objects[eIL_BATTERY_AREA], InfoLabelTemplate_Objects[eIL_INFO_AREA], LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_border_width(InfoLabelTemplate_Objects[eIL_BATTERY_AREA], HELP_BORDER_WIDTH, LV_PART_MAIN);
	lv_obj_set_style_border_color(InfoLabelTemplate_Objects[eIL_BATTERY_AREA], lv_color_black(), LV_PART_MAIN);
	lv_obj_set_style_pad_all(InfoLabelTemplate_Objects[eIL_BATTERY_AREA], 0, LV_PART_MAIN);
	lv_obj_set_style_radius(InfoLabelTemplate_Objects[eIL_BATTERY_AREA], 10, LV_PART_MAIN);
	lv_obj_set_flex_flow(InfoLabelTemplate_Objects[eIL_BATTERY_AREA], LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(InfoLabelTemplate_Objects[eIL_BATTERY_AREA], LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);    
	lv_obj_set_style_pad_row(InfoLabelTemplate_Objects[eIL_BATTERY_AREA], 0, LV_PART_MAIN);  // Zmenší mezeru na 5 pixelů
	//Info space area	
	InfoLabelTemplate_Objects[eIL_INFO_SPACE_AREA] = lv_obj_create(InfoLabelTemplate_Objects[eIL_INFO_AREA]);
	lv_obj_set_size(InfoLabelTemplate_Objects[eIL_INFO_SPACE_AREA], 176, 70); 
	lv_obj_align_to(InfoLabelTemplate_Objects[eIL_INFO_SPACE_AREA], InfoLabelTemplate_Objects[eIL_INFO_AREA], LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_border_width(InfoLabelTemplate_Objects[eIL_INFO_SPACE_AREA], HELP_BORDER_WIDTH, LV_PART_MAIN);                       
	lv_obj_set_style_border_color(InfoLabelTemplate_Objects[eIL_INFO_SPACE_AREA], lv_color_black(), LV_PART_MAIN);        
	lv_obj_set_style_pad_all(InfoLabelTemplate_Objects[eIL_INFO_SPACE_AREA], 0, LV_PART_MAIN);
	lv_obj_set_style_radius(InfoLabelTemplate_Objects[eIL_INFO_SPACE_AREA], 10, LV_PART_MAIN);
	//Inof Signal areaeIL_INFO_AREA
	InfoLabelTemplate_Objects[eIL_SIGNAL_AREA]  = lv_obj_create(InfoLabelTemplate_Objects[eIL_INFO_AREA]);
	lv_obj_set_size(InfoLabelTemplate_Objects[eIL_SIGNAL_AREA], 62, 78); 
	lv_obj_align_to(InfoLabelTemplate_Objects[eIL_SIGNAL_AREA], InfoLabelTemplate_Objects[eIL_INFO_AREA], LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_border_width(InfoLabelTemplate_Objects[eIL_SIGNAL_AREA], HELP_BORDER_WIDTH, LV_PART_MAIN);
	lv_obj_set_style_border_color(InfoLabelTemplate_Objects[eIL_SIGNAL_AREA], lv_color_black(), LV_PART_MAIN);
	lv_obj_set_style_pad_all(InfoLabelTemplate_Objects[eIL_SIGNAL_AREA], 0, LV_PART_MAIN);
	lv_obj_set_style_radius(InfoLabelTemplate_Objects[eIL_SIGNAL_AREA], 10, LV_PART_MAIN);
	lv_obj_set_flex_flow(InfoLabelTemplate_Objects[eIL_SIGNAL_AREA], LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align(InfoLabelTemplate_Objects[eIL_SIGNAL_AREA], LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);    
	lv_obj_set_style_pad_row(InfoLabelTemplate_Objects[eIL_SIGNAL_AREA], 0, LV_PART_MAIN);  // Zmenší mezeru na 5 pixelů

    //Baterie icon
    InfoLabelTemplate_Objects[eIL_BATERIE_ICO] = lv_obj_create(InfoLabelTemplate_Objects[eIL_BATTERY_AREA]);
    lv_obj_set_size(InfoLabelTemplate_Objects[eIL_BATERIE_ICO], 40, 40); 
    lv_obj_align_to(InfoLabelTemplate_Objects[eIL_BATERIE_ICO], InfoLabelTemplate_Objects[eIL_BATTERY_AREA], LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_border_width(InfoLabelTemplate_Objects[eIL_BATERIE_ICO], HELP_BORDER_WIDTH, LV_PART_MAIN);                       
    lv_obj_set_style_border_color(InfoLabelTemplate_Objects[eIL_BATERIE_ICO], lv_color_hex(0x5E5E5C), LV_PART_MAIN); 
    //lv_obj_set_style_bg_color(InfoLabelTemplate_Objects[eIL_BATERIE_ICO], lv_color_hex(0x5E5E5C), LV_PART_MAIN);						//Pozadi podle tabulky       
    lv_obj_set_style_pad_all(InfoLabelTemplate_Objects[eIL_BATERIE_ICO], 0, LV_PART_MAIN);
    lv_obj_set_style_radius(InfoLabelTemplate_Objects[eIL_BATERIE_ICO], 20, LV_PART_MAIN);
	
    //Signal icon
    InfoLabelTemplate_Objects[eIL_SIGNAL_ICO] = lv_obj_create(InfoLabelTemplate_Objects[eIL_SIGNAL_AREA]);
    lv_obj_set_size(InfoLabelTemplate_Objects[eIL_SIGNAL_ICO], 40, 40); 
    lv_obj_align_to(InfoLabelTemplate_Objects[eIL_SIGNAL_ICO], InfoLabelTemplate_Objects[eIL_SIGNAL_AREA], LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_border_width(InfoLabelTemplate_Objects[eIL_SIGNAL_ICO], HELP_BORDER_WIDTH, LV_PART_MAIN);                       
    lv_obj_set_style_border_color(InfoLabelTemplate_Objects[eIL_SIGNAL_ICO], lv_color_hex(0x5E5E5C), LV_PART_MAIN);
    //lv_obj_set_style_bg_color(InfoLabelTemplate_Objects[eIL_SIGNAL_ICO], lv_color_hex(0x5E5E5C), LV_PART_MAIN);						//Pozadi podle tabulky
    lv_obj_set_style_pad_all(InfoLabelTemplate_Objects[eIL_SIGNAL_ICO], 0, LV_PART_MAIN);
    lv_obj_set_style_radius(InfoLabelTemplate_Objects[eIL_SIGNAL_ICO], 20, LV_PART_MAIN);

	//Baterie Value
    InfoLabelTemplate_Objects[eIL_BATTERY_VALUE] = lv_obj_create(InfoLabelTemplate_Objects[eIL_BATTERY_AREA]);
    lv_obj_set_size(InfoLabelTemplate_Objects[eIL_BATTERY_VALUE], 60, 30); 
    lv_obj_align_to(InfoLabelTemplate_Objects[eIL_BATTERY_VALUE], InfoLabelTemplate_Objects[eIL_BATTERY_AREA], LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_border_width(InfoLabelTemplate_Objects[eIL_BATTERY_VALUE], HELP_BORDER_WIDTH, LV_PART_MAIN);                       
    lv_obj_set_style_border_color(InfoLabelTemplate_Objects[eIL_BATTERY_VALUE], lv_color_black(), LV_PART_MAIN);        
    lv_obj_set_style_pad_all(InfoLabelTemplate_Objects[eIL_BATTERY_VALUE], 0, LV_PART_MAIN);
    lv_obj_set_style_radius(InfoLabelTemplate_Objects[eIL_BATTERY_VALUE], 10, LV_PART_MAIN);

	//Signal Value
    InfoLabelTemplate_Objects[eIL_SIGNAL_VALUE] = lv_obj_create(InfoLabelTemplate_Objects[eIL_SIGNAL_AREA]);
    lv_obj_set_size(InfoLabelTemplate_Objects[eIL_SIGNAL_VALUE], 60, 30); 
    lv_obj_align_to(InfoLabelTemplate_Objects[eIL_SIGNAL_VALUE], InfoLabelTemplate_Objects[eIL_SIGNAL_AREA], LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_border_width(InfoLabelTemplate_Objects[eIL_SIGNAL_VALUE], HELP_BORDER_WIDTH, LV_PART_MAIN);                       
    lv_obj_set_style_border_color(InfoLabelTemplate_Objects[eIL_SIGNAL_VALUE], lv_color_black(), LV_PART_MAIN);        
    lv_obj_set_style_pad_all(InfoLabelTemplate_Objects[eIL_SIGNAL_VALUE], 0, LV_PART_MAIN);
    lv_obj_set_style_radius(InfoLabelTemplate_Objects[eIL_SIGNAL_VALUE], 10, LV_PART_MAIN);


	//ARC Fencee
	InfoLabelTemplate_Objects[eIL_ARC_FENCE] = lv_arc_create(MainTemplate_Objects[eFENCE_INDICATOR_AREA]);
 	lv_obj_set_style_arc_color(InfoLabelTemplate_Objects[eIL_ARC_FENCE], lv_color_hex(0x4040FF), LV_PART_INDICATOR | LV_STATE_DEFAULT );
    lv_obj_set_size(InfoLabelTemplate_Objects[eIL_ARC_FENCE], 200, 200);                             // Nastaví velikost oblouku na 150x150 px
//     //lv_obj_set_style_arc_width(arc, 15, LV_PART_MAIN);          // Nastaví šířku čáry na 10 px
//     //lv_obj_set_style_arc_width(arc, 25, LV_PART_INDICATOR);  // Šířka indikátoru na 15 px
    lv_obj_set_style_arc_color(InfoLabelTemplate_Objects[eIL_ARC_FENCE], lv_color_hex(0xB2B92D), LV_PART_INDICATOR);  // Nastavení barvy indikátoru (zelená)
    lv_arc_set_rotation(InfoLabelTemplate_Objects[eIL_ARC_FENCE], 245);
    lv_arc_set_bg_angles(InfoLabelTemplate_Objects[eIL_ARC_FENCE], 0, 360);
    lv_arc_set_start_angle(InfoLabelTemplate_Objects[eIL_ARC_FENCE], 0);
    lv_arc_set_value(InfoLabelTemplate_Objects[eIL_ARC_FENCE], 50);  // Nastaví hodnotu na 50
//     // lv_obj_remove_style(arc, NULL, LV_PART_KNOB);   /*Be sure the knob is not displayed*/
//     // lv_obj_remove_flag(arc, LV_OBJ_FLAG_CLICKABLE);  /*To not allow adjusting by click*/
    lv_obj_center(InfoLabelTemplate_Objects[eIL_ARC_FENCE]);


	//FenceValue  BIG
    InfoLabelTemplate_Objects[eIL_FENCE_VALUE_AREA] = lv_obj_create(MainTemplate_Objects[eFENCE_INDICATOR_AREA]);
    lv_obj_set_size(InfoLabelTemplate_Objects[eIL_FENCE_VALUE_AREA], 160, 60);
    lv_obj_align_to(InfoLabelTemplate_Objects[eIL_FENCE_VALUE_AREA], MainTemplate_Objects[eFENCE_INDICATOR_AREA], LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_border_width(InfoLabelTemplate_Objects[eIL_FENCE_VALUE_AREA], HELP_BORDER_WIDTH, LV_PART_MAIN);                       
    lv_obj_set_style_border_color(InfoLabelTemplate_Objects[eIL_FENCE_VALUE_AREA], lv_color_black(), LV_PART_MAIN);        
    lv_obj_set_style_radius(InfoLabelTemplate_Objects[eIL_FENCE_VALUE_AREA], 0, LV_PART_MAIN);
     lv_obj_set_flex_flow(InfoLabelTemplate_Objects[eIL_FENCE_VALUE_AREA], LV_FLEX_FLOW_ROW);
    //lv_obj_set_flex_align(cont_col_bot, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_flex_align(InfoLabelTemplate_Objects[eIL_FENCE_VALUE_AREA], LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_all(InfoLabelTemplate_Objects[eIL_FENCE_VALUE_AREA], 2, LV_PART_MAIN);
    lv_obj_set_style_pad_column(InfoLabelTemplate_Objects[eIL_FENCE_VALUE_AREA], 2, LV_PART_MAIN);  // Zmenší mezeru na 5 pixelů

    InfoLabelTemplate_Objects[eIL_TYPE_AREA] = lv_obj_create(MainTemplate_Objects[eFENCE_INDICATOR_AREA]);
    lv_obj_set_size(InfoLabelTemplate_Objects[eIL_TYPE_AREA], 150, 25);
    lv_obj_align_to(InfoLabelTemplate_Objects[eIL_TYPE_AREA], InfoLabelTemplate_Objects[eIL_FENCE_VALUE_AREA], LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_opa(InfoLabelTemplate_Objects[eIL_TYPE_AREA], LV_OPA_TRANSP, LV_PART_MAIN); //Nastaveni pruhlednosti
    lv_obj_set_style_border_width(InfoLabelTemplate_Objects[eIL_TYPE_AREA], HELP_BORDER_WIDTH, LV_PART_MAIN);
    lv_obj_set_style_border_color(InfoLabelTemplate_Objects[eIL_TYPE_AREA], lv_color_black(), LV_PART_MAIN);       
    lv_obj_set_style_pad_all(InfoLabelTemplate_Objects[eIL_TYPE_AREA], 0, LV_PART_MAIN);


	//Text type
	InfoLabelTemplate_Objects[eIL_TYPE_TEXT_LABEL] = lv_label_create(InfoLabelTemplate_Objects[eIL_TYPE_AREA]);
    lv_obj_set_style_text_font(InfoLabelTemplate_Objects[eIL_TYPE_TEXT_LABEL], &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_align(InfoLabelTemplate_Objects[eIL_TYPE_TEXT_LABEL],  LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_border_width(InfoLabelTemplate_Objects[eIL_TYPE_TEXT_LABEL], HELP_BORDER_WIDTH, LV_PART_MAIN);
    lv_obj_set_style_pad_all(InfoLabelTemplate_Objects[eIL_TYPE_TEXT_LABEL], 0, LV_PART_MAIN);    
    lv_obj_set_style_text_color(InfoLabelTemplate_Objects[eIL_TYPE_TEXT_LABEL], lv_color_hex(0x5E5E5C), LV_PART_MAIN);  // Barva písma bílá
    lv_obj_set_style_text_align(InfoLabelTemplate_Objects[eIL_TYPE_TEXT_LABEL], LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

	//Text Fence
    InfoLabelTemplate_Objects[eIL_FENCE_TEXT_LABEL] = lv_label_create(InfoLabelTemplate_Objects[eIL_FENCE_VALUE_AREA]);
	lv_obj_set_style_text_font(InfoLabelTemplate_Objects[eIL_FENCE_TEXT_LABEL], &lv_font_montserrat_48, LV_PART_MAIN);
	lv_obj_align(InfoLabelTemplate_Objects[eIL_FENCE_TEXT_LABEL],  LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(InfoLabelTemplate_Objects[eIL_FENCE_TEXT_LABEL], 0, LV_PART_MAIN);
	lv_obj_set_style_border_width(InfoLabelTemplate_Objects[eIL_FENCE_TEXT_LABEL], HELP_BORDER_WIDTH, LV_PART_MAIN);
	lv_obj_set_style_text_color(InfoLabelTemplate_Objects[eIL_FENCE_TEXT_LABEL], lv_color_black(), LV_PART_MAIN);  // Barva písma bílá
	lv_obj_set_style_text_align(InfoLabelTemplate_Objects[eIL_FENCE_TEXT_LABEL], LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

	//Text Units
    InfoLabelTemplate_Objects[eIL_UNIT_TEXT_LABEL] = lv_label_create(InfoLabelTemplate_Objects[eIL_FENCE_VALUE_AREA]);    
    lv_obj_set_style_text_font(InfoLabelTemplate_Objects[eIL_UNIT_TEXT_LABEL], &lv_font_montserrat_24, LV_PART_MAIN);
    lv_obj_align(InfoLabelTemplate_Objects[eIL_UNIT_TEXT_LABEL],  LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_border_width(InfoLabelTemplate_Objects[eIL_UNIT_TEXT_LABEL], HELP_BORDER_WIDTH, LV_PART_MAIN);
    lv_obj_set_style_pad_all(InfoLabelTemplate_Objects[eIL_UNIT_TEXT_LABEL], 0, LV_PART_MAIN);
    lv_obj_set_style_text_color(InfoLabelTemplate_Objects[eIL_UNIT_TEXT_LABEL], lv_color_hex(0x5E5E5C), LV_PART_MAIN);  // Barva písma bílá
    lv_obj_set_style_text_align(InfoLabelTemplate_Objects[eIL_UNIT_TEXT_LABEL], LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

	InfoLabelTemplate_Objects[eIL_BATTERY_TEXT_LABEL] = lv_label_create(InfoLabelTemplate_Objects[eIL_BATTERY_VALUE]);
    lv_obj_set_style_text_color(InfoLabelTemplate_Objects[eIL_BATTERY_TEXT_LABEL], lv_color_hex(0x5E5E5C), LV_PART_MAIN);  // Barva písma bílá
	lv_obj_set_style_border_width(InfoLabelTemplate_Objects[eIL_BATTERY_TEXT_LABEL], HELP_BORDER_WIDTH, LV_PART_MAIN);
	lv_obj_set_style_pad_all(InfoLabelTemplate_Objects[eIL_BATTERY_TEXT_LABEL], 0, LV_PART_MAIN);
	lv_obj_align(InfoLabelTemplate_Objects[eIL_BATTERY_TEXT_LABEL],  LV_ALIGN_CENTER, 0, 0);

	InfoLabelTemplate_Objects[eIL_SIGNAL_TEXT_LABEL] = lv_label_create(InfoLabelTemplate_Objects[eIL_SIGNAL_VALUE]);
    lv_obj_set_style_text_color(InfoLabelTemplate_Objects[eIL_SIGNAL_TEXT_LABEL], lv_color_hex(0x5E5E5C), LV_PART_MAIN);  // Barva písma bílá
	lv_obj_set_style_border_width(InfoLabelTemplate_Objects[eIL_SIGNAL_TEXT_LABEL], HELP_BORDER_WIDTH, LV_PART_MAIN);
	lv_obj_set_style_pad_all(InfoLabelTemplate_Objects[eIL_SIGNAL_TEXT_LABEL], 0, LV_PART_MAIN);
	lv_obj_align(InfoLabelTemplate_Objects[eIL_SIGNAL_TEXT_LABEL],  LV_ALIGN_CENTER, 0, 0);
}

void G_Insert_Text()
{
	lv_label_set_text(InfoLabelTemplate_Objects[eIL_TYPE_TEXT_LABEL],		" ");
	lv_label_set_text(InfoLabelTemplate_Objects[eIL_FENCE_TEXT_LABEL],		" ");
	lv_label_set_text(InfoLabelTemplate_Objects[eIL_UNIT_TEXT_LABEL],		" ");
	lv_label_set_text(InfoLabelTemplate_Objects[eIL_BATTERY_TEXT_LABEL],	" ");
	lv_label_set_text(InfoLabelTemplate_Objects[eIL_SIGNAL_TEXT_LABEL], 	" ");
	lv_label_set_text(BarGraphTitle_Teplate_Objects[eBGT_TITLE], 				" ");
}


void G_Template_BarTitle()
{
	BarGraphTitle_Teplate_Objects[eBGT_TITLE] = lv_label_create(MainTemplate_Objects[eBARGRAPH_TITLE_AREA]);	
	lv_obj_set_style_text_color(BarGraphTitle_Teplate_Objects[eBGT_TITLE], lv_color_hex(0x5E5E5C), LV_PART_MAIN);  // Barva písma bílá
	lv_obj_set_style_border_width(BarGraphTitle_Teplate_Objects[eBGT_TITLE], HELP_BORDER_WIDTH, LV_PART_MAIN);
	lv_obj_set_style_text_align(BarGraphTitle_Teplate_Objects[eBGT_TITLE], LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
}

void G_Template_BarGraph()
{
	uint32_t i;
	for(i = 0; i < 5; i++)
	{		
		BarGraph_Teplate_Objects[i] = lv_label_create(MainTemplate_Objects[eBARGRAPH_AREA]);
		lv_label_set_text_fmt(BarGraph_Teplate_Objects[i], "%s", item_strings[i]); //Vlozeni fotmatovaneho textu
		//lv_obj_set_style_bg_color(label, lv_color_hex(BarGraph_Color[i]), LV_PART_MAIN);//Pozadi podle tabulky
		lv_obj_set_style_bg_opa(BarGraph_Teplate_Objects[i], LV_OPA_COVER, LV_PART_MAIN); //Nastaveni pruhlednosti
		lv_obj_set_style_text_color(BarGraph_Teplate_Objects[i], lv_color_hex(0xFFFFFF), LV_PART_MAIN);  // Barva písma bílá
		lv_obj_align(BarGraph_Teplate_Objects[i],  LV_ALIGN_CENTER, 0, 0);
		lv_obj_set_style_pad_all(BarGraph_Teplate_Objects[i], 7, LV_PART_MAIN);
		lv_obj_set_style_border_width(BarGraph_Teplate_Objects[i], HELP_BORDER_WIDTH, LV_PART_MAIN);
    	lv_obj_set_style_border_color(BarGraph_Teplate_Objects[i], lv_color_black(), LV_PART_MAIN);       
		lv_obj_set_style_text_align(BarGraph_Teplate_Objects[i], LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);        
		lv_obj_set_style_radius(BarGraph_Teplate_Objects[i], 5, LV_PART_MAIN); //Nastveni zaobleni
		lv_obj_set_size(BarGraph_Teplate_Objects[i], LV_PCT(100), 30);
	}	
}



//Zbytek FENCEE AREY    
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
						//     lv_obj_t * img_sig = lv_img_create(info_signal);
						//     lv_img_set_src(img_sig, "B:/sig_gr.png");  // 'S' je označení souborového systému SD v LVGL
						//     lv_obj_t * value_sig_text = lv_label_create(value_sig);
						//     lv_label_set_text(value_sig_text, "84%");
						//     lv_obj_set_style_text_color(value_sig_text, lv_color_hex(0x5E5E5C), LV_PART_MAIN);  // Barva písma bílá
						//     lv_obj_set_style_pad_all(value_sig_text, 0, LV_PART_MAIN);
						//     lv_obj_align(value_sig_text,  LV_ALIGN_CENTER, 0, 0);
						//     //Kolecko
						//     lv_obj_set_style_border_width(target, 0, LV_PART_MAIN);                       
						//     lv_obj_set_style_border_color(target, lv_color_black(), LV_PART_MAIN);        
						//     lv_obj_set_style_radius(target, 0, LV_PART_MAIN);
						//     return target;
						// }