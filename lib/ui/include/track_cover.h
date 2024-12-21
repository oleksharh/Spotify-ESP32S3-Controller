// Template for track_cover.h
#ifndef TRACK_COVER_H
#define TRACK_COVER_H

#include "lvgl.h"

class TrackCover
{
private:
    lv_obj_t* track_cover;

public:
    // Constructor and Destructor
    TrackCover();
    ~TrackCover();

    lv_obj_t* create(lv_obj_t* parent);
    void update_cover(lv_obj_t* cover);
};

#endif // TRACK_COVER_H