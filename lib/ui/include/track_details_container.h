// Template for track_details.h
#ifndef TRACK_DETAILS_CONTAINER_H
#define TRACK_DETAILS_CONTAINER_H

#include "lvgl.h"
#include "track_title.h"
#include "track_artist.h"

class TrackDetailsContainer
{
private:
    lv_obj_t* track_details_container;
    TrackTitle* track_title;
    TrackArtist* track_artist;

public:
    // Constructor and Destructor
    TrackDetailsContainer();
    ~TrackDetailsContainer();

    lv_obj_t* create(lv_obj_t* parent);
    void update_details(lv_obj_t* title, lv_obj_t* artist);
};

#endif // TRACK_DETAILS_H