/*Using LVGL with Arduino requires some extra steps:
 *Be sure to read the docs here: https://docs.lvgl.io/master/integration/framework/arduino.html  */

#include <lvgl.h>
// #include <lvgl/lvgl.h>

#include "../.pio/libdeps/4d_systems_esp32s3_gen4_r8n16/lvgl/src/font/lv_font.h"

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

// Variables to store current track information
int current_track_duration = 0;
int current_track_progress = 0;
bool is_playing = false;
int http_error_count = 0;
const int max_error_count = 3;
String current_track_img_url = "";

// LVGL objects for the display
lv_obj_t *track_cover;                    // Image widget for the album cover
lv_obj_t *label_title;                    // Label widget for the song title
lv_obj_t *label_artist;                   // Label widget for the artist name
lv_obj_t *playback_bar;                   // Slider widget for the playback bar
lv_obj_t *btn_play, *btn_next, *btn_prev; // Media control buttons

void create_ui()
{
  // Create the main screen
  lv_obj_t *screen = lv_scr_act();
  lv_obj_set_style_bg_color(screen, lv_color_hex(0x202020), 0); // Dark gray background
  lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);

  // Create a container for the song attributes section
  lv_obj_t *song_container = lv_obj_create(screen);
  lv_obj_set_size(song_container, LV_PCT(100), LV_PCT(60)); // Top half of the screen
  lv_obj_set_flex_flow(song_container, LV_FLEX_FLOW_ROW);
  lv_obj_set_style_radius(song_container, 0, 0); // Remove rounded edges
  lv_obj_set_style_bg_color(song_container, lv_color_hex(0x202020), 0);
  lv_obj_set_style_border_width(song_container, 0, 0); // Remove border
  lv_obj_align(song_container, LV_ALIGN_TOP_MID, 0, 0);

  // Add the album cover (placeholder for now)
  track_cover = lv_img_create(song_container);
  lv_img_set_src(track_cover, "");        // Replace with the album image path
  lv_obj_set_size(track_cover, 128, 128); // Example size

  // Create a container for the song title and artist section
  lv_obj_t *song_subcontainer = lv_obj_create(song_container);
  lv_obj_set_size(song_subcontainer, LV_PCT(70), LV_PCT(90)); // Remaining space for text
  lv_obj_set_flex_flow(song_subcontainer, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_style_radius(song_subcontainer, 0, 0);
  lv_obj_set_style_bg_opa(song_subcontainer, LV_OPA_TRANSP, 0); // Transparent background
  lv_obj_align(song_subcontainer, LV_ALIGN_RIGHT_MID, 0, 0);
  lv_obj_set_scrollbar_mode(song_subcontainer, LV_SCROLLBAR_MODE_OFF); // No scrollbars
  lv_obj_clear_flag(song_subcontainer, LV_OBJ_FLAG_SCROLLABLE);

  // Add song title
  // label_title = lv_label_create(song_subcontainer);
  // lv_label_set_text(label_title, "Song Title with a Long Name Example");
  // lv_obj_set_style_text_color(label_title, lv_color_white(), 0); // White text
  // lv_obj_set_style_text_font(label_title, &lv_font_montserrat_22, 0);
  // lv_obj_set_style_text_align(label_title, LV_TEXT_ALIGN_LEFT, 0);
  // lv_label_set_long_mode(label_title, LV_LABEL_LONG_WRAP); // Enable text wrapping

  // Add song title
  label_title = lv_label_create(song_subcontainer);
  lv_label_set_text(label_title, "Song Title with a Long Name Example");
  lv_obj_set_style_text_color(label_title, lv_color_white(), 0); // White text
  lv_obj_set_style_text_font(label_title, &lv_font_montserrat_22, 0);
  lv_obj_set_style_text_align(label_title, LV_TEXT_ALIGN_LEFT, 0);

  // Enable text wrapping and allow the label to expand vertically
  lv_label_set_long_mode(label_title, LV_LABEL_LONG_WRAP); // Enable text wrapping

  // Ensure the label container is flexible and allows wrapping
  lv_obj_set_size(label_title, LV_PCT(100), LV_SIZE_CONTENT); // Let the label expand vertically

  // Add artist name
  label_artist = lv_label_create(song_subcontainer);
  lv_label_set_text(label_artist, "Artist Name");
  lv_obj_set_style_text_color(label_artist, lv_color_white(), 0);
  // lv_obj_set_style_text_font(label_artist, &lv_font_montserrat_18, 0);
  lv_obj_set_style_text_align(label_artist, LV_TEXT_ALIGN_LEFT, 0);
  lv_label_set_long_mode(label_artist, LV_LABEL_LONG_WRAP); // Enable text wrapping

  // Create a container for media controls
  lv_obj_t *media_container = lv_obj_create(screen);
  lv_obj_set_size(media_container, LV_PCT(100), LV_PCT(40)); // Bottom half of the screen
  lv_obj_set_flex_flow(media_container, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_style_radius(media_container, 0, 0); // Remove rounded edges
  lv_obj_set_style_bg_color(media_container, lv_color_hex(0x202020), 0);
  lv_obj_set_style_border_width(media_container, 0, 0); // Remove border
  lv_obj_align(media_container, LV_ALIGN_BOTTOM_MID, 0, 0);
  lv_obj_set_scrollbar_mode(media_container, LV_SCROLLBAR_MODE_OFF); // No scrollbars

  // Add playback bar
  playback_bar = lv_slider_create(media_container);
  lv_obj_set_width(playback_bar, LV_PCT(100));
  lv_slider_set_range(playback_bar, 0, 100); // 0 to 100% playback
  lv_obj_set_style_bg_color(playback_bar, lv_color_hex(0x404040), 0);
  lv_obj_set_style_bg_grad_color(playback_bar, lv_color_hex(0x404040), 0);

  // Add media control buttons in a horizontal layout
  lv_obj_t *btn_container = lv_obj_create(media_container);
  lv_obj_set_flex_flow(btn_container, LV_FLEX_FLOW_ROW);
  lv_obj_set_style_pad_gap(btn_container, 10, 0);
  lv_obj_set_style_radius(btn_container, 0, 0); // Remove rounded edges
  lv_obj_set_size(btn_container, LV_PCT(100), LV_PCT(85));
  lv_obj_set_style_bg_opa(btn_container, LV_OPA_TRANSP, 0);

  // Add "Previous" button
  btn_prev = lv_btn_create(btn_container);
  lv_obj_set_size(btn_prev, 50, 50);
  lv_obj_t *label_prev = lv_label_create(btn_prev);
  lv_label_set_text(label_prev, "⏮"); // Previous icon

  // Add "Play/Pause" button
  btn_play = lv_btn_create(btn_container);
  lv_obj_set_size(btn_play, 50, 50);
  lv_obj_t *label_play = lv_label_create(btn_play);
  lv_label_set_text(label_play, "▶"); // Play icon

  // Add "Next" button
  btn_next = lv_btn_create(btn_container);
  lv_obj_set_size(btn_next, 50, 50);
  lv_obj_t *label_next = lv_label_create(btn_next);
  lv_label_set_text(label_next, "⏭"); // Next icon
}

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
  // lv_example_bar_1();
  create_ui();

  Serial.println("Setup done");
}

void loop()
{
  lv_timer_handler(); /* let the GUI do its work */
  delay(5);           /* let this time pass */
}