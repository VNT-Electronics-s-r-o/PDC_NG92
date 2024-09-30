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


static void hal_init(void);
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

void lvCheckGreen(void)
{
	lv_obj_t * obj;
	obj = lv_obj_create(lv_screen_active());
	lv_obj_set_style_bg_color(obj, lv_color_hex(0x41A535), LV_PART_MAIN);	
	lv_obj_set_size(obj, 310, 50);
	lv_obj_t * label = lv_label_create(obj);
	lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_PART_MAIN);
	lv_label_set_text(label, "!CHECK!");
	lv_obj_center(label);	
	lv_obj_set_pos(obj, 5, 190);
}

void lvCheckGreenLight(void)
{
	lv_obj_t * obj;
	obj = lv_obj_create(lv_screen_active());
	lv_obj_set_style_bg_color(obj, lv_color_hex(0xB0BD2F), LV_PART_MAIN);	
	lv_obj_set_size(obj, 310, 50);
	lv_obj_t * label = lv_label_create(obj);
	lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_PART_MAIN);
	lv_label_set_text(label, "!CHECK!");
	lv_obj_center(label);	
	lv_obj_set_pos(obj, 5, 250);
}

void lvCheckYellow(void)
{
	lv_obj_t * obj;
	obj = lv_obj_create(lv_screen_active());
	lv_obj_set_style_bg_color(obj, lv_color_hex(0xFEED00), LV_PART_MAIN);	
	lv_obj_set_size(obj, 310, 50);
	lv_obj_t * label = lv_label_create(obj);
	lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_PART_MAIN);
	lv_label_set_text(label, "!CHECK!");
	lv_obj_center(label);	
	lv_obj_set_pos(obj, 5, 310);
}

void lvCheckOrange(void)
{
	lv_obj_t * obj;
	obj = lv_obj_create(lv_screen_active());
	lv_obj_set_style_bg_color(obj, lv_color_hex(0xE5610E), LV_PART_MAIN);	
	lv_obj_set_size(obj, 310, 50);
	lv_obj_t * label = lv_label_create(obj);
	lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_PART_MAIN);
	lv_label_set_text(label, "!CHECK!");
	lv_obj_center(label);	
	lv_obj_set_pos(obj, 5, 370);
}

void lvCheckRed(void)
{
	lv_obj_t * obj;
	obj = lv_obj_create(lv_screen_active());
	lv_obj_set_style_bg_color(obj, lv_color_hex(0xCE1719), LV_PART_MAIN);	
	lv_obj_set_size(obj, 310, 50);
	lv_obj_t * label = lv_label_create(obj);
	lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_PART_MAIN);
	lv_label_set_text(label, "!CHECK!");
	lv_obj_center(label);	
	lv_obj_set_pos(obj, 5, 430);
}


void setup()
{
	Serial.begin(115200);
	while(!Serial){};
	
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

	lvCheckGreen();
	lvCheckGreenLight();
	lvCheckYellow();
	lvCheckOrange();
	lvCheckRed();
	Serial.println( "Setup done" );

}

void loop()
{
	lv_timer_handler(); /* let the GUI do its work */
	//delay(5); /* let this time pass */
}
