#include "grafika.h"
#include <Arduino.h>
#include "lvgl.h"

#include <SD.h>
#include <SPI.h>

#if LV_USE_TFT_ESPI
	#include <TFT_eSPI.h>
#endif

#define V2_1
#define TFT_LED 33
#define SD_CS_PIN 4

#define TFT_ROTATION  LV_DISPLAY_ROTATION_180
#define NUM_OBJECTS 20
#define NUM_ICON    6

#define HELP_BORDER_WIDTH 0

#define	BARGRAPH 1


enum {
	eSTATUSBAR_AREA,
	eFENCE_INDICATOR_AREA,
	eBARGRAPH_TITLE_AREA,
	eBARGRAPH_AREA,
	
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
TFT_eSPI tft = TFT_eSPI(HOR_RES, VER_RES);

uint32_t BarGraph_Color[5] = { 0x41A535, 0xB0BD2F, 0xFEED00, 0xE5610E, 0xCE1719};

lv_obj_t * MainTemplate_Objects[NUM_OBJECTS];  // Pole pro uložení referencí na 20 objektů
lv_obj_t * StatusBarTemplate_Objects[NUM_ICON];  // Pole pro uložení referencí na 20 objektů

//lv_obj_t * StatusIconTemplate_Objects[NUM_ICON];  // Pole pro uložení referencí na 20 objektů
lv_obj_t * BarGraphTitle_Teplate_Objects[NUM_OBJECTS];
lv_obj_t * BarGraph_Teplate_Objects[NUM_OBJECTS];
lv_obj_t * InfoLabelTemplate_Objects[NUM_ICON];  // Pole pro uložení referencí na 20 objektů
lv_anim_t ArcAnim;
lv_chart_series_t * GraphMaxValue;
lv_chart_series_t * GraphMinValue;
lv_chart_series_t * GraphThreshold;
lv_obj_t * GraphFence;


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

#if LV_USE_LOG != 0
void my_print( lv_log_level_t level, const char * buf )
{
    LV_UNUSED(level);
    Serial.println(buf);
    Serial.flush();
}
#endif

lv_obj_t *  SubMainScreen_Status();
void        G_Create_Template();

void		G_Insert_Text();
void		G_Template_BarTitle(lv_obj_t *  parent);
void		G_Template_BarGraph();
void 		G_Update_BarGraph(int led);
void		G_Anime_Arc(int percent, int value);
void		G_Add_Static_Icon();

void		G_Template_StatusBar (lv_obj_t *  parent);
void		G_Template_FenceeVoltage(lv_obj_t *  parent);
void		G_Template_BarGraph(lv_obj_t *  parent);
void		listDir(fs::FS &fs, const char * dirname, uint8_t levels);
int			 SD_Test_Init();
void		G_CreateGraph(lv_obj_t *  parent);
static void hook_division_lines(lv_event_t * e);
static void add_faded_area(lv_event_t * e);
static void draw_event_cb(lv_event_t * e);

/*************************
    F U N C T I O N S
*************************/
static uint32_t my_tick(void)
{
    return millis();
}

static void arc_anim_callback(void * obj, int32_t value)
{
    lv_arc_set_value((lv_obj_t *)obj, value);
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
	tft.begin();		/*TFT Init*/
	tft.setRotation(3);	/*TFT Rotation*/

	SD_Test_Init();

    lv_tick_set_cb(my_tick);

    lv_display_t * disp;	
    
    disp = lv_tft_espi_create(HOR_RES, VER_RES, draw_buf, sizeof(draw_buf));
    lv_display_set_rotation(disp, TFT_ROTATION);

#if LV_USE_LOG != 0
    lv_log_register_print_cb( my_print );
#endif
	
    G_MainBackground();
	// G_Template_StatusBar();
	// G_Template_FenceeVoltage();
	// G_Template_BarTitle();
	// if(BARGRAPH == 1)
	// {
	// 	G_Template_BarGraph();
	// }	
	// G_Insert_Text();
	// G_Add_Static_Icon();
}

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

void G_MainBackground()
{
	lv_obj_t *  Page1;
	lv_obj_t *  tempObject_1;
	lv_obj_t *  tempObject_2;
	lv_obj_t *  tempObject_3;
	lv_obj_t *  tempObject_4;
	//Obarvit na bílo

	//Page1 = lv_obj_create(lv_screen_active());
	//lv_obj_set_style_bg_color(Page1, lv_color_hex(0xFFFFFF), LV_PART_MAIN);

	tempObject_1 = lv_obj_create(lv_screen_active());
	tempObject_2 = lv_obj_create(lv_screen_active());
	tempObject_3 = lv_obj_create(lv_screen_active());
	tempObject_4 = lv_obj_create(lv_screen_active());
	
	//Status lista	
	if(HELP_BORDER_WIDTH == 0)
	{
		lv_obj_set_style_bg_color		(tempObject_1,	lv_color_hex(0xCCCCCC),	LV_PART_MAIN											);
	}
	
	lv_obj_set_size						(tempObject_1,	320,					60														);
	lv_obj_align						(tempObject_1,	LV_ALIGN_TOP_MID,		0,							-15							);
	lv_obj_set_style_border_width		(tempObject_1,	HELP_BORDER_WIDTH,		LV_PART_MAIN											);		// Nastavení šířky rámečku (2 pixely)
	lv_obj_set_style_border_color		(tempObject_1,	lv_color_black(),		LV_PART_MAIN											);		// Nastavení barvy rámečku (černá)
	lv_obj_set_style_radius				(tempObject_1,	10,						LV_PART_MAIN											);		// Nastveni zaobleni
	lv_obj_set_style_pad_all			(tempObject_1,	0,						LV_PART_MAIN											);
	lv_obj_set_style_pad_column			(tempObject_1,	0,						LV_PART_MAIN											);		// Zmenší mezeru na 5 pixelů

	G_Template_StatusBar(tempObject_1);
	lv_timer_handler();

	//FenceVoltage area		
	lv_obj_set_size						(tempObject_2,	320,					230														);
	lv_obj_align_to						(tempObject_2,	tempObject_1,			LV_ALIGN_OUT_BOTTOM_MID,	0,						0	);
	lv_obj_set_style_pad_all			(tempObject_2,	0,						LV_PART_MAIN											);
	lv_obj_set_style_border_width		(tempObject_2,	HELP_BORDER_WIDTH,		LV_PART_MAIN											);		// Nastavení šířky rámečku (2 pixely)
	lv_obj_set_style_border_color		(tempObject_2,	lv_color_black(),		LV_PART_MAIN											);		// Nastavení barvy rámečku (černá)
	G_Template_FenceeVoltage(tempObject_2);
	lv_timer_handler();

	//BarGraph Title
	if(HELP_BORDER_WIDTH == 0)
	{
		lv_obj_set_style_bg_color		(tempObject_3,	lv_color_hex(0xCCCCCC),	LV_PART_MAIN											);        
	}
	lv_obj_set_style_pad_all			(tempObject_3,	2,						LV_PART_MAIN											);
	lv_obj_set_size						(tempObject_3,	320,					30														);
	lv_obj_set_style_border_width		(tempObject_3,	HELP_BORDER_WIDTH,		LV_PART_MAIN											);		// Nastavení šířky rámečku (2 pixely)
	lv_obj_set_style_border_color		(tempObject_3,	lv_color_black(),		LV_PART_MAIN											);		// Nastavení barvy rámečku (černá)
	lv_obj_align_to						(tempObject_3,	tempObject_2,			LV_ALIGN_OUT_BOTTOM_MID,	0,						0	);
    lv_obj_set_style_radius				(tempObject_3,	0,						LV_PART_MAIN											);		//Nastveni zaobleni
    lv_obj_set_flex_flow				(tempObject_3,	LV_FLEX_FLOW_COLUMN																);
    lv_obj_set_flex_align				(tempObject_3,	LV_FLEX_ALIGN_CENTER,	LV_FLEX_ALIGN_START,		LV_FLEX_ALIGN_CENTER		);

	G_Template_BarTitle(tempObject_3);

	// //BarGraph	
	lv_obj_set_size						(tempObject_4,	315,					180														);
	lv_obj_align_to						(tempObject_4,	tempObject_3,			LV_ALIGN_OUT_BOTTOM_MID,	0,						-5	);
	lv_obj_set_flex_flow				(tempObject_4,	LV_FLEX_FLOW_COLUMN																);	
	lv_obj_set_flex_align				(tempObject_4,	LV_FLEX_ALIGN_CENTER,	LV_FLEX_ALIGN_CENTER,		LV_FLEX_ALIGN_CENTER		);	
	lv_obj_set_style_pad_all			(tempObject_4,	5,						LV_PART_MAIN											);
	lv_obj_set_style_radius				(tempObject_4,	0,						LV_PART_MAIN											);
	lv_obj_set_style_pad_row			(tempObject_4,	2,						LV_PART_MAIN											);		// Zmenší mezeru na 5 pixelů
	lv_obj_set_style_border_width		(tempObject_4,	5,						LV_PART_MAIN											);		// Nastavení šířky rámečku (2 pixely)
	if(HELP_BORDER_WIDTH == 0)
	{
		lv_obj_set_style_border_color	(tempObject_4,	lv_color_white(),		LV_PART_MAIN											);		// Nastavení barvy rámečku (černá)       
	}
	else
	{
		lv_obj_set_style_border_color	(tempObject_4,	lv_color_black(),		LV_PART_MAIN											);		// Nastavení barvy rámečku (černá)       
	}	
	lv_obj_set_style_radius				(tempObject_4,	10,						LV_PART_MAIN											);		//Nastveni zaobleni    
	//G_Template_BarGraph(tempObject_4);
	G_CreateGraph(tempObject_4);
	lv_timer_handler();
	
}

void G_Template_StatusBar (lv_obj_t *  parent)
{
	lv_obj_t *  tempObject_1 = lv_obj_create(parent);

	lv_obj_set_size					(tempObject_1, 300,						42													);
	lv_obj_align_to					(tempObject_1, parent,					LV_ALIGN_BOTTOM_MID,	0,						-3	);
	lv_obj_set_style_bg_opa			(tempObject_1, LV_OPA_TRANSP,			LV_PART_MAIN										);				//Nastaveni pruhlednosti
	lv_obj_set_style_border_width	(tempObject_1, HELP_BORDER_WIDTH,		LV_PART_MAIN										);
	lv_obj_set_style_border_color	(tempObject_1, lv_color_black(),		LV_PART_MAIN										);
	lv_obj_set_style_radius			(tempObject_1, 0,						LV_PART_MAIN										);
	lv_obj_set_flex_flow			(tempObject_1, LV_FLEX_FLOW_ROW																);
	lv_obj_set_flex_align			(tempObject_1, LV_FLEX_ALIGN_CENTER,	LV_FLEX_ALIGN_CENTER,	LV_FLEX_ALIGN_CENTER		);
	lv_obj_set_style_pad_all		(tempObject_1, 0, LV_PART_MAIN																);
	lv_obj_set_style_pad_column		(tempObject_1, 24, LV_PART_MAIN																);				// Zmenší mezeru na 5 pixelů

	for (int i = 0; i<5; i++)
	{		
		StatusBarTemplate_Objects[i] = lv_img_create(tempObject_1);
		lv_img_set_src(StatusBarTemplate_Objects[i], item_file[i]);
	}
}

void G_Template_FenceeVoltage(lv_obj_t *  parent)
{
	lv_obj_t *  infoLabel;
	lv_obj_t *  batteryArea;
	lv_obj_t *  spaceArea;
	lv_obj_t *  signalArea;	
	lv_obj_t *  typeArea;
	

	//Info Label	
	infoLabel = lv_obj_create(parent);
	lv_obj_set_size					(infoLabel, 310, 80); 
	lv_obj_align_to					(infoLabel, parent, LV_ALIGN_BOTTOM_MID, 0, 0);
	lv_obj_set_style_border_width	(infoLabel, HELP_BORDER_WIDTH, LV_PART_MAIN);
	lv_obj_set_style_border_color	(infoLabel, lv_color_black(), LV_PART_MAIN);
	lv_obj_set_style_pad_all		(infoLabel, 0, LV_PART_MAIN);
	lv_obj_set_style_radius			(infoLabel, 0, LV_PART_MAIN);
	lv_obj_set_flex_flow			(infoLabel, LV_FLEX_FLOW_ROW);
	lv_obj_set_flex_align			(infoLabel, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);    
	lv_obj_set_style_pad_column		(infoLabel, 2, LV_PART_MAIN);													// Zmenší mezeru na 5 pixelů

	//InfoBattery area
	batteryArea = lv_obj_create(infoLabel);
	lv_obj_set_size					(batteryArea, 52, 78); 
	lv_obj_align_to					(batteryArea, infoLabel, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_border_width	(batteryArea, HELP_BORDER_WIDTH, LV_PART_MAIN);
	lv_obj_set_style_border_color	(batteryArea, lv_color_black(), LV_PART_MAIN);
	lv_obj_set_style_pad_all		(batteryArea, 0, LV_PART_MAIN);
	lv_obj_set_style_radius			(batteryArea, 10, LV_PART_MAIN);
	lv_obj_set_flex_flow			(batteryArea, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align			(batteryArea, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);    
	lv_obj_set_style_pad_row		(batteryArea, 0, LV_PART_MAIN);  // Zmenší mezeru na 5 pixelů

	//Info space area	
	spaceArea = lv_obj_create(infoLabel);
	lv_obj_set_size					(spaceArea, 196, 70); 
	lv_obj_align_to					(spaceArea, infoLabel, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_border_width	(spaceArea, HELP_BORDER_WIDTH, LV_PART_MAIN);                       
	lv_obj_set_style_border_color	(spaceArea, lv_color_black(), LV_PART_MAIN);        
	lv_obj_set_style_pad_all		(spaceArea, 0, LV_PART_MAIN);
	lv_obj_set_style_radius			(spaceArea, 10, LV_PART_MAIN);

	//Inof Signal areaeIL_INFO_AREA
	signalArea = lv_obj_create(infoLabel);
	lv_obj_set_size					(signalArea, 52, 78); 
	lv_obj_align_to					(signalArea, infoLabel, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_border_width	(signalArea, HELP_BORDER_WIDTH, LV_PART_MAIN);
	lv_obj_set_style_border_color	(signalArea, lv_color_black(), LV_PART_MAIN);
	lv_obj_set_style_pad_all		(signalArea, 0, LV_PART_MAIN);
	lv_obj_set_style_radius			(signalArea, 10, LV_PART_MAIN);
	lv_obj_set_flex_flow			(signalArea, LV_FLEX_FLOW_COLUMN);
	lv_obj_set_flex_align			(signalArea, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);    
	lv_obj_set_style_pad_row		(signalArea, 0, LV_PART_MAIN);  // Zmenší mezeru na 5 pixelů

	//Baterie icon
	InfoLabelTemplate_Objects[eIL_BATERIE_ICO] = lv_img_create(batteryArea);
	lv_img_set_src(InfoLabelTemplate_Objects[eIL_BATERIE_ICO] , "B:\\batt_gr.png");

	//Baterie icon
	InfoLabelTemplate_Objects[eIL_SIGNAL_ICO] = lv_img_create(signalArea);
	lv_img_set_src(InfoLabelTemplate_Objects[eIL_SIGNAL_ICO] , "B:\\sig_gr.png");

	//Baterie Value
    InfoLabelTemplate_Objects[eIL_BATTERY_TEXT_LABEL] = lv_label_create(batteryArea);    
    lv_obj_set_style_border_width	(InfoLabelTemplate_Objects[eIL_BATTERY_TEXT_LABEL], HELP_BORDER_WIDTH, LV_PART_MAIN);                       
    lv_obj_set_style_border_color	(InfoLabelTemplate_Objects[eIL_BATTERY_TEXT_LABEL], lv_color_black(), LV_PART_MAIN);        
    lv_obj_set_style_pad_all		(InfoLabelTemplate_Objects[eIL_BATTERY_TEXT_LABEL], HELP_BORDER_WIDTH, LV_PART_MAIN);
    lv_obj_set_style_radius			(InfoLabelTemplate_Objects[eIL_BATTERY_TEXT_LABEL], 10, LV_PART_MAIN);
	lv_obj_set_style_text_color		(InfoLabelTemplate_Objects[eIL_BATTERY_TEXT_LABEL], lv_color_hex(0x5E5E5C), LV_PART_MAIN);  // Barva písma bílá
	lv_label_set_text				(InfoLabelTemplate_Objects[eIL_BATTERY_TEXT_LABEL],	"12,1 V");

	//Signal Value
    InfoLabelTemplate_Objects[eIL_SIGNAL_TEXT_LABEL] = lv_label_create(signalArea);
    lv_obj_set_style_border_width	(InfoLabelTemplate_Objects[eIL_SIGNAL_TEXT_LABEL], HELP_BORDER_WIDTH, LV_PART_MAIN);                       
    lv_obj_set_style_border_color	(InfoLabelTemplate_Objects[eIL_SIGNAL_TEXT_LABEL], lv_color_black(), LV_PART_MAIN);        
    lv_obj_set_style_pad_all		(InfoLabelTemplate_Objects[eIL_SIGNAL_TEXT_LABEL], 0, LV_PART_MAIN);
    lv_obj_set_style_radius			(InfoLabelTemplate_Objects[eIL_SIGNAL_TEXT_LABEL], 10, LV_PART_MAIN);	
	lv_obj_set_style_text_color		(InfoLabelTemplate_Objects[eIL_SIGNAL_TEXT_LABEL], lv_color_hex(0x5E5E5C), LV_PART_MAIN);  // Barva písma bílá
	lv_label_set_text				(InfoLabelTemplate_Objects[eIL_SIGNAL_TEXT_LABEL], 	"85%");


	//ARC Fencee
	InfoLabelTemplate_Objects[eIL_ARC_FENCE]		= lv_arc_create(parent);
 	lv_obj_set_style_arc_color	(InfoLabelTemplate_Objects[eIL_ARC_FENCE], lv_color_hex(0x4040FF), LV_PART_INDICATOR | LV_STATE_DEFAULT );
    lv_obj_set_size				(InfoLabelTemplate_Objects[eIL_ARC_FENCE], 200, 200);                             // Nastaví velikost oblouku na 150x150 px
//     //lv_obj_set_style_arc_width(arc, 15, LV_PART_MAIN);          // Nastaví šířku čáry na 10 px
//     //lv_obj_set_style_arc_width(arc, 25, LV_PART_INDICATOR);  // Šířka indikátoru na 15 px
    lv_obj_set_style_arc_color	(InfoLabelTemplate_Objects[eIL_ARC_FENCE], lv_color_hex(BarGraph_Color[4]), LV_PART_INDICATOR);  // Nastavení barvy indikátoru (zelená)
    lv_arc_set_rotation			(InfoLabelTemplate_Objects[eIL_ARC_FENCE], 245);
    lv_arc_set_bg_angles		(InfoLabelTemplate_Objects[eIL_ARC_FENCE], 0, 360);
    lv_arc_set_start_angle		(InfoLabelTemplate_Objects[eIL_ARC_FENCE], 0);
    lv_arc_set_value			(InfoLabelTemplate_Objects[eIL_ARC_FENCE], 80);  // Nastaví hodnotu na 50
//     // lv_obj_remove_style(arc, NULL, LV_PART_KNOB);   /*Be sure the knob is not displayed*/
//     // lv_obj_remove_flag(arc, LV_OBJ_FLAG_CLICKABLE);  /*To not allow adjusting by click*/
    lv_obj_center				(InfoLabelTemplate_Objects[eIL_ARC_FENCE]);
	
    lv_anim_init				(&ArcAnim);
    lv_anim_set_var				(&ArcAnim, InfoLabelTemplate_Objects[eIL_ARC_FENCE]);                           // Přiřadí objekt k animaci
    lv_anim_set_exec_cb			(&ArcAnim, arc_anim_callback);         // Nastaví callback pro animaci
    lv_anim_set_values			(&ArcAnim, 0, 0);                     // Hodnoty od 0 do 100
    lv_anim_set_time			(&ArcAnim, 500);                         // Doba trvání animace (v ms)
    lv_anim_start				(&ArcAnim);

//FenceValue  BIG
	InfoLabelTemplate_Objects[eIL_FENCE_VALUE_AREA] = lv_obj_create(parent);
	lv_obj_set_size					(InfoLabelTemplate_Objects[eIL_FENCE_VALUE_AREA], 150, 60);
	lv_obj_align_to					(InfoLabelTemplate_Objects[eIL_FENCE_VALUE_AREA], parent, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_border_width	(InfoLabelTemplate_Objects[eIL_FENCE_VALUE_AREA], HELP_BORDER_WIDTH, LV_PART_MAIN);                       
	lv_obj_set_style_border_color	(InfoLabelTemplate_Objects[eIL_FENCE_VALUE_AREA], lv_color_black(), LV_PART_MAIN);        
	lv_obj_set_style_radius			(InfoLabelTemplate_Objects[eIL_FENCE_VALUE_AREA], 0, LV_PART_MAIN);
	lv_obj_set_flex_flow			(InfoLabelTemplate_Objects[eIL_FENCE_VALUE_AREA], LV_FLEX_FLOW_ROW);
	//lv_obj_set_flex_align(cont_col_bot, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_flex_align			(InfoLabelTemplate_Objects[eIL_FENCE_VALUE_AREA], LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_style_pad_all		(InfoLabelTemplate_Objects[eIL_FENCE_VALUE_AREA], 2, LV_PART_MAIN);
	lv_obj_set_style_pad_column		(InfoLabelTemplate_Objects[eIL_FENCE_VALUE_AREA], 2, LV_PART_MAIN);  // Zmenší mezeru na 5 pixelů

    typeArea = lv_obj_create(parent);
    lv_obj_set_size					(typeArea, 130, 25);
    lv_obj_align_to					(typeArea, InfoLabelTemplate_Objects[eIL_FENCE_VALUE_AREA], LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_opa			(typeArea, LV_OPA_TRANSP, LV_PART_MAIN); //Nastaveni pruhlednosti
    lv_obj_set_style_border_width	(typeArea, HELP_BORDER_WIDTH, LV_PART_MAIN);
    lv_obj_set_style_border_color	(typeArea, lv_color_black(), LV_PART_MAIN);       
    lv_obj_set_style_pad_all		(typeArea, 0, LV_PART_MAIN);

	//Text type
	InfoLabelTemplate_Objects[eIL_TYPE_TEXT_LABEL] = lv_label_create(typeArea);
    lv_obj_set_style_text_font		(InfoLabelTemplate_Objects[eIL_TYPE_TEXT_LABEL], &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_align					(InfoLabelTemplate_Objects[eIL_TYPE_TEXT_LABEL],  LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_border_width	(InfoLabelTemplate_Objects[eIL_TYPE_TEXT_LABEL], HELP_BORDER_WIDTH, LV_PART_MAIN);
    lv_obj_set_style_pad_all		(InfoLabelTemplate_Objects[eIL_TYPE_TEXT_LABEL], 0, LV_PART_MAIN);    
    lv_obj_set_style_text_color		(InfoLabelTemplate_Objects[eIL_TYPE_TEXT_LABEL], lv_color_hex(0x5E5E5C), LV_PART_MAIN);  // Barva písma bílá
    lv_obj_set_style_text_align		(InfoLabelTemplate_Objects[eIL_TYPE_TEXT_LABEL], LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_label_set_text				(InfoLabelTemplate_Objects[eIL_TYPE_TEXT_LABEL], "OUTPUT POWER");

	//Text Fence
    InfoLabelTemplate_Objects[eIL_FENCE_TEXT_LABEL] = lv_label_create(InfoLabelTemplate_Objects[eIL_FENCE_VALUE_AREA]);
	lv_obj_set_style_text_font		(InfoLabelTemplate_Objects[eIL_FENCE_TEXT_LABEL], &lv_font_montserrat_48, LV_PART_MAIN);
	lv_obj_align					(InfoLabelTemplate_Objects[eIL_FENCE_TEXT_LABEL],  LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all		(InfoLabelTemplate_Objects[eIL_FENCE_TEXT_LABEL], 0, LV_PART_MAIN);
	lv_obj_set_style_border_width	(InfoLabelTemplate_Objects[eIL_FENCE_TEXT_LABEL], HELP_BORDER_WIDTH, LV_PART_MAIN);
	lv_obj_set_style_text_color		(InfoLabelTemplate_Objects[eIL_FENCE_TEXT_LABEL], lv_color_black(), LV_PART_MAIN);  // Barva písma bílá
	lv_label_set_text				(InfoLabelTemplate_Objects[eIL_FENCE_TEXT_LABEL], 	"0");
	lv_obj_set_style_text_align		(InfoLabelTemplate_Objects[eIL_FENCE_TEXT_LABEL], LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

	//Text Units
    InfoLabelTemplate_Objects[eIL_UNIT_TEXT_LABEL] = lv_label_create(InfoLabelTemplate_Objects[eIL_FENCE_VALUE_AREA]);    
    lv_obj_set_style_text_font		(InfoLabelTemplate_Objects[eIL_UNIT_TEXT_LABEL], &lv_font_montserrat_24, LV_PART_MAIN);
    lv_obj_align					(InfoLabelTemplate_Objects[eIL_UNIT_TEXT_LABEL],  LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_border_width	(InfoLabelTemplate_Objects[eIL_UNIT_TEXT_LABEL], HELP_BORDER_WIDTH, LV_PART_MAIN);
    lv_obj_set_style_pad_all		(InfoLabelTemplate_Objects[eIL_UNIT_TEXT_LABEL], 0, LV_PART_MAIN);
    lv_obj_set_style_text_color		(InfoLabelTemplate_Objects[eIL_UNIT_TEXT_LABEL], lv_color_hex(0x5E5E5C), LV_PART_MAIN);  // Barva písma bílá
	lv_label_set_text				(InfoLabelTemplate_Objects[eIL_UNIT_TEXT_LABEL], 	"kV");
    lv_obj_set_style_text_align		(InfoLabelTemplate_Objects[eIL_UNIT_TEXT_LABEL], LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

// 	InfoLabelTemplate_Objects[eIL_BATTERY_TEXT_LABEL] = lv_label_create(InfoLabelTemplate_Objects[eIL_BATTERY_VALUE]);
//     lv_obj_set_style_text_color(InfoLabelTemplate_Objects[eIL_BATTERY_TEXT_LABEL], lv_color_hex(0x5E5E5C), LV_PART_MAIN);  // Barva písma bílá
// 	lv_obj_set_style_border_width(InfoLabelTemplate_Objects[eIL_BATTERY_TEXT_LABEL], HELP_BORDER_WIDTH, LV_PART_MAIN);
// 	lv_obj_set_style_pad_all(InfoLabelTemplate_Objects[eIL_BATTERY_TEXT_LABEL], 0, LV_PART_MAIN);
// 	lv_obj_align(InfoLabelTemplate_Objects[eIL_BATTERY_TEXT_LABEL],  LV_ALIGN_CENTER, 0, 0);

// 	InfoLabelTemplate_Objects[eIL_SIGNAL_TEXT_LABEL] = lv_label_create(InfoLabelTemplate_Objects[eIL_SIGNAL_VALUE]);
//     lv_obj_set_style_text_color(InfoLabelTemplate_Objects[eIL_SIGNAL_TEXT_LABEL], lv_color_hex(0x5E5E5C), LV_PART_MAIN);  // Barva písma bílá
// 	lv_obj_set_style_border_width(InfoLabelTemplate_Objects[eIL_SIGNAL_TEXT_LABEL], HELP_BORDER_WIDTH, LV_PART_MAIN);
// 	lv_obj_set_style_pad_all(InfoLabelTemplate_Objects[eIL_SIGNAL_TEXT_LABEL], 0, LV_PART_MAIN);
// 	lv_obj_align(InfoLabelTemplate_Objects[eIL_SIGNAL_TEXT_LABEL],  LV_ALIGN_CENTER, 0, 0);
}

void G_Insert_Text()
{
	lv_label_set_text(InfoLabelTemplate_Objects[eIL_TYPE_TEXT_LABEL],		"OUTPUT POWER");	
	lv_label_set_text(InfoLabelTemplate_Objects[eIL_UNIT_TEXT_LABEL],		"kV");
	lv_label_set_text(InfoLabelTemplate_Objects[eIL_BATTERY_TEXT_LABEL],	"12,1");
	lv_label_set_text(InfoLabelTemplate_Objects[eIL_SIGNAL_TEXT_LABEL], 	"85%");
	lv_label_set_text(BarGraphTitle_Teplate_Objects[eBGT_TITLE], 			"OUTPUT POWER");
}

void G_Template_BarTitle(lv_obj_t *  parent)
{
	BarGraphTitle_Teplate_Objects[eBGT_TITLE] = lv_label_create(parent);	
	lv_obj_set_style_text_color(BarGraphTitle_Teplate_Objects[eBGT_TITLE], lv_color_hex(0x5E5E5C), LV_PART_MAIN);  // Barva písma bílá
	lv_obj_set_style_border_width(BarGraphTitle_Teplate_Objects[eBGT_TITLE], HELP_BORDER_WIDTH, LV_PART_MAIN);
	lv_obj_set_style_text_align(BarGraphTitle_Teplate_Objects[eBGT_TITLE], LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
	lv_label_set_text(BarGraphTitle_Teplate_Objects[eBGT_TITLE], 			"HISTOR OF OUTPUT POWER"); 
}

void G_Template_BarGraph(lv_obj_t *  parent)
{
	uint32_t i;
	for(i = 0; i < 5; i++)
	{		
		BarGraph_Teplate_Objects[i] = lv_label_create(parent);
		lv_obj_set_style_bg_opa			(BarGraph_Teplate_Objects[i], LV_OPA_COVER, LV_PART_MAIN); //Nastaveni pruhlednosti
		lv_obj_set_style_text_color		(BarGraph_Teplate_Objects[i], lv_color_hex(0xFFFFFF), LV_PART_MAIN);  // Barva písma bílá
		lv_obj_align					(BarGraph_Teplate_Objects[i],  LV_ALIGN_CENTER, 0, 0);
		lv_obj_set_style_pad_all		(BarGraph_Teplate_Objects[i], 7, LV_PART_MAIN);
		lv_obj_set_style_border_width	(BarGraph_Teplate_Objects[i], HELP_BORDER_WIDTH, LV_PART_MAIN);
    	lv_obj_set_style_border_color	(BarGraph_Teplate_Objects[i], lv_color_black(), LV_PART_MAIN);       
		lv_obj_set_style_text_align		(BarGraph_Teplate_Objects[i], LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);        
		lv_obj_set_style_radius			(BarGraph_Teplate_Objects[i], 5, LV_PART_MAIN); //Nastveni zaobleni
		lv_obj_set_size					(BarGraph_Teplate_Objects[i], LV_PCT(100), 30);
	}	
}


void G_CreateGraph(lv_obj_t *  parent)
{	  
    GraphFence = lv_chart_create(parent);
    lv_chart_set_type(GraphFence, LV_CHART_TYPE_LINE);   /*Show lines and points too*/
	lv_obj_set_style_size(GraphFence, 0, 0, LV_PART_INDICATOR);
    lv_obj_set_size(GraphFence, 290, 160);
    lv_obj_set_style_pad_all(GraphFence, 0, 0);
    lv_obj_set_style_radius(GraphFence, 0, 0);
	lv_chart_set_range(GraphFence, LV_CHART_AXIS_PRIMARY_Y, 0, 110);  // Rozsah hodnot na ose Y
	lv_chart_set_point_count(GraphFence, 20);  // Nastavení počtu bodů (10 bodů na řadu)
    lv_obj_center(GraphFence);
    lv_chart_set_div_line_count(GraphFence, 9, 11);
    // lv_obj_add_event_cb(chart, draw_event_cb, LV_EVENT_DRAW_TASK_ADDED, NULL);
    // lv_obj_add_flag(chart, LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS);

    GraphMaxValue	= lv_chart_add_series(GraphFence, lv_palette_main(LV_PALETTE_BLUE),		LV_CHART_AXIS_PRIMARY_Y);
	GraphMinValue	= lv_chart_add_series(GraphFence, lv_palette_main(LV_PALETTE_ORANGE),	LV_CHART_AXIS_PRIMARY_Y);
	GraphThreshold	= lv_chart_add_series(GraphFence, lv_palette_main(LV_PALETTE_RED),		LV_CHART_AXIS_PRIMARY_Y);

    // uint32_t i;
    // for(i = 0; i < 10; i++) {
    //     lv_chart_set_next_value(chart, ser, lv_rand(10, 80));
    // }
}

void G_Update_FenceVaule(int value)
{
	int helpPercernt = 0;
	int led = 0;
	//Nastaveni LED
	if(value<30)led = 4;	
	else if(value<50)led = 3;
	else if(value<70)led = 2;
	else if(value<90)led = 1;
	else led = 0;	

	helpPercernt = value*100/110;
	Serial.println(helpPercernt);	
	lv_label_set_text_fmt	(InfoLabelTemplate_Objects[eIL_FENCE_TEXT_LABEL],		"%d.%d", value/10, value%10);
	lv_arc_set_value		(InfoLabelTemplate_Objects[eIL_ARC_FENCE],helpPercernt);
	
	G_Anime_Arc(helpPercernt, led);	
}

void G_UpdateChart(int max, int min, int threshold)
{
	if(BARGRAPH == 1)
	{
		//G_Update_BarGraph(led);
		lv_chart_set_next_value(GraphFence, GraphMaxValue, max);  // První bod
		lv_chart_set_next_value(GraphFence, GraphMinValue, min);  // První bod
		lv_chart_set_next_value(GraphFence, GraphThreshold, threshold);  // První bod
	}
}

void G_Anime_Arc(int percent, int value)
{
	Serial.println(value);
	lv_obj_set_style_arc_color(InfoLabelTemplate_Objects[eIL_ARC_FENCE], lv_color_hex(BarGraph_Color[value]), LV_PART_INDICATOR);  // Nastavení barvy indikátoru (zelená)	
	lv_anim_set_time(&ArcAnim, 1500);  	
    lv_anim_set_values(&ArcAnim, 0, percent);                     // Hodnoty od 0 do 100
    lv_anim_start(&ArcAnim);
	lv_anim_set_delay(&ArcAnim, 200);
}

void G_Update_BarGraph(int led)
{	
	for(int i=0;i<5;i++)
	{
		lv_label_set_text(BarGraph_Teplate_Objects[i], ""); //Vlozeni fotmatovaneho textu
		if(i>=led)
		{
			lv_obj_set_style_bg_color(BarGraph_Teplate_Objects[i], lv_color_hex(BarGraph_Color[i]), LV_PART_MAIN);//Pozadi podle tabulky			
		}		
		else
		{			
			lv_obj_set_style_bg_color(BarGraph_Teplate_Objects[i], lv_color_hex(0xFFFFFF), LV_PART_MAIN);//Pozadi podle tabulky
		}		
	}

	switch(led)
	{
		case 4:
			lv_label_set_text_fmt(BarGraph_Teplate_Objects[4], "%s", item_strings[4]); //Vlozeni fotmatovaneho textu
			break;
		case 3:
			lv_label_set_text_fmt(BarGraph_Teplate_Objects[3], "%s", item_strings[3]); //Vlozeni fotmatovaneho textu
			break;
		case 0:
			lv_label_set_text_fmt(BarGraph_Teplate_Objects[0], "%s", item_strings[0]); //Vlozeni fotmatovaneho textu
			break;
		default:
			break;
	}
}