// Template for track_title.h
#ifndef TRACK_TITLE_H
#define TRACK_TITLE_H

#include "lvgl.h"

class TrackTitle
{
private:
    lv_obj_t* track_title;

public:
    // Constructor and Destructor
    TrackTitle();
    ~TrackTitle();

    lv_obj_t* create(lv_obj_t* parent);
    void update_title(lv_obj_t* title);
};

#endif // TRACK_TITLE_H