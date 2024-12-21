#include "../include/screen.h"

Screen::Screen() : screen(nullptr)
{
    song_container = new SongContainer();
    media_container = new MediaContainer();
}

Screen::~Screen()
{
    // LVGL automatically manages memory for most objects.
}

/**
 * @brief Creates the main screen and initializes all UI components.
 *
 * This function sets up the screen and adds the song container, media container,
 * and other UI elements to it. The screen is not loaded onto the display by
 * this function, it just returns the created screen object.
 *
 * @return lv_obj_t* The created screen object.
 */
lv_obj_t* Screen::create()
{
    screen = lv_obj_create(nullptr); // No parent, as this is the top-level object

    // Object Initializations
    song_container->create(screen);
    media_container->create(screen);
    // styles for containers will be applied in their classes

    return screen;
}

/**
 * @brief Loads the screen onto the display.
 *
 * This function loads the screen onto the display, making it the active screen.
 */
void Screen::load()
{
    if (screen != nullptr)
        lv_scr_load(screen);
}

void Screen::update_song(lv_obj_t* cover, lv_obj_t* title, lv_obj_t* artist)
{
    song_container->update_song(cover, title, artist);
}

// TODO: REWRITE update_song function to take in a string/text/json objects instead of lv_obj_t