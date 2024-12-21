// Template for playback_bar.h
#ifndef PLAYBACK_BAR_H
#define PLAYBACK_BAR_H

#include "lvgl.h"

class PlaybackBar {
public:
    static lv_obj_t* create(lv_obj_t* parent);
};

#endif // PLAYBACK_BAR_H