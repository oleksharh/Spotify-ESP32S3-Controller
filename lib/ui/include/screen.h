#ifndef SCREEN_H
#define SCREEN_H

#include <lvgl.h>
#include "song_container.h"
#include "media_container.h"

class Screen
{
private:
    lv_obj_t* screen;
    SongContainer* song_container;
    MediaContainer* media_container;

public:
    // Constructor and Destructor
    Screen();
    ~Screen();

    // Public Methods
    lv_obj_t* create(); // Create and initialize the screen
    void load();                        // Load this screen to make it active
    void update_song(lv_obj_t* cover, lv_obj_t* title, lv_obj_t* artist); // Update the song details

};

#endif // SCREEN_H