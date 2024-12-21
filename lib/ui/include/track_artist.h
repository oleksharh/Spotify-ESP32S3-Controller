// Template for track_artist.h
#ifndef TRACK_ARTIST_H
#define TRACK_ARTIST_H

#include "lvgl.h"

class TrackArtist
{
private:
    lv_obj_t* track_artist;

public:
    // Constructor and Destructor
    TrackArtist();
    ~TrackArtist();

    lv_obj_t* create(lv_obj_t* parent);
    void update_artist(lv_obj_t* artist);
};

#endif // TRACK_ARTIST_H