/*Using LVGL with Arduino requires some extra steps:
 *Be sure to read the docs here: https://docs.lvgl.io/master/integration/framework/arduino.html  */

#include <lvgl.h>

#if LV_USE_TFT_ESPI
#include <TFT_eSPI.h>
#endif

/*To use the built-in examples and demos of LVGL uncomment the includes below respectively.
 *You also need to copy `lvgl/examples` to `lvgl/src/examples`. Similarly for the demos `lvgl/demos` to `lvgl/src/demos`.
 *Note that the `lv_examples` library is for LVGL v7 and you shouldn't install it for this version (since LVGL v8)
 *as the examples and demos are now part of the main LVGL library. */

// #include <examples/lv_examples.h>
// #include <demos/lv_demos.h>

/*Set to your screen resolution and rotation*/
#define TFT_HOR_RES 320
#define TFT_VER_RES 480
#define TFT_ROTATION LV_DISPLAY_ROTATION_90

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];

#if LV_USE_LOG != 0
void my_print(lv_log_level_t level, const char *buf)
{
  LV_UNUSED(level);
  Serial.println(buf);
  Serial.flush();
}
#endif

/* LVGL calls it when a rendered image needs to copied to the display*/
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
  /*Copy `px map` to the `area`*/

  /*For example ("my_..." functions needs to be implemented by you)
  uint32_t w = lv_area_get_width(area);
  uint32_t h = lv_area_get_height(area);

  my_set_window(area->x1, area->y1, w, h);
  my_draw_bitmaps(px_map, w * h);
   */

  /*Call it to tell LVGL you are ready*/
  lv_display_flush_ready(disp);
}

/*Read the touchpad*/
void my_touchpad_read(lv_indev_t *indev, lv_indev_data_t *data)
{
  /*For example  ("my_..." functions needs to be implemented by you)
  int32_t x, y;
  bool touched = my_get_touch( &x, &y );

  if(!touched) {
      data->state = LV_INDEV_STATE_RELEASED;
  } else {
      data->state = LV_INDEV_STATE_PRESSED;

      data->point.x = x;
      data->point.y = y;
  }
   */
}

/*use Arduinos millis() as tick source*/
static uint32_t my_tick(void)
{
  return millis();
}

static void event_handler(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);

  if (code == LV_EVENT_CLICKED)
  {
    LV_LOG_USER("Clicked");
  }
  else if (code == LV_EVENT_VALUE_CHANGED)
  {
    LV_LOG_USER("Toggled");
  }
}

void lv_example_button_1(void)
{
  lv_obj_t *label;

  lv_obj_t *btn1 = lv_button_create(lv_screen_active());
  lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, NULL);
  lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -40);
  lv_obj_remove_flag(btn1, LV_OBJ_FLAG_PRESS_LOCK);

  label = lv_label_create(btn1);
  lv_label_set_text(label, "Button");
  lv_obj_center(label);

  lv_obj_t *btn2 = lv_button_create(lv_screen_active());
  lv_obj_add_event_cb(btn2, event_handler, LV_EVENT_ALL, NULL);
  lv_obj_align(btn2, LV_ALIGN_CENTER, 0, 40);
  lv_obj_add_flag(btn2, LV_OBJ_FLAG_CHECKABLE);
  lv_obj_set_height(btn2, LV_SIZE_CONTENT);

  label = lv_label_create(btn2);
  lv_label_set_text(label, "Toggle");
  lv_obj_center(label);
}

// Declare a global variable to hold the bar object and the input value
lv_obj_t *bar1;
int input_value = 0; // Simulated input value (can be replaced with Spotify API data)

// Function to map the input and update the bar
void update_bar_cb(lv_timer_t *timer)
{
  // Increment the simulated input value
  input_value = (input_value + 1) % 200;

  // Map the input range (0-200) to the bar range (0-100)
  int mapped_value = lv_map(input_value, 0, 200, 0, 100);

  // Update the bar's value
  lv_bar_set_value(bar1, mapped_value, LV_ANIM_ON);
}

void lv_example_bar_1(void)
{
  bar1 = lv_bar_create(lv_screen_active());
  lv_obj_set_size(bar1, 460, 10);
  lv_obj_align(bar1, LV_ALIGN_BOTTOM_LEFT, 10, -10);
  lv_bar_set_value(bar1, 0, LV_ANIM_OFF);

  static lv_style_t style;
  lv_style_init(&style);

  // Set the color of the bar (green in this case)
  lv_style_set_bg_color(&style, lv_color_hex(0x00FF00)); // Green color in hex

  // Apply the style to the bar
  lv_obj_add_style(bar1, &style, LV_PART_INDICATOR);

  lv_timer_create(update_bar_cb, 1000, NULL);
}

void setup()
{
  String LVGL_Arduino = "Hello Arduino! ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

  Serial.begin(115200);
  Serial.println(LVGL_Arduino);

  lv_init();

  /*Set a tick source so that LVGL will know how much time elapsed. */
  lv_tick_set_cb(my_tick);

  /* register print function for debugging */
#if LV_USE_LOG != 0
  lv_log_register_print_cb(my_print);
#endif

  lv_display_t *disp;
#if LV_USE_TFT_ESPI
  /*TFT_eSPI can be enabled lv_conf.h to initialize the display in a simple way*/
  disp = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, draw_buf, sizeof(draw_buf));
  lv_display_set_rotation(disp, TFT_ROTATION);

#else
  /*Else create a display yourself*/
  disp = lv_display_create(TFT_HOR_RES, TFT_VER_RES);
  lv_display_set_flush_cb(disp, my_disp_flush);
  lv_display_set_buffers(disp, draw_buf, NULL, sizeof(draw_buf), LV_DISPLAY_RENDER_MODE_PARTIAL);
#endif

  /*Initialize the (dummy) input device driver*/
  lv_indev_t *indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER); /*Touchpad should have POINTER type*/
  lv_indev_set_read_cb(indev, my_touchpad_read);

  /* Create a simple label
   * ---------------------
   lv_obj_t *label = lv_label_create( lv_screen_active() );
   lv_label_set_text( label, "Hello Arduino, I'm LVGL!" );
   lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );

   * Try an example. See all the examples
   *  - Online: https://docs.lvgl.io/master/examples.html
   *  - Source codes: https://github.com/lvgl/lvgl/tree/master/examples
   * ----------------------------------------------------------------

   lv_example_btn_1();

   * Or try out a demo. Don't forget to enable the demos in lv_conf.h. E.g. LV_USE_DEMO_WIDGETS
   * -------------------------------------------------------------------------------------------

   lv_demo_widgets();
   */

  lv_obj_t *label = lv_label_create(lv_screen_active());
  lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x003a57), LV_PART_MAIN);
  lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0xffffff), LV_PART_MAIN);
  lv_label_set_text(label, "Hello Arduino, I'm LVGL!");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
  // lv_example_button_1();
  lv_example_bar_1();

  Serial.println("Setup done");
}

void loop()
{
  lv_timer_handler(); /* let the GUI do its work */
  delay(5);           /* let this time pass */
}