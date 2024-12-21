// Template for song_container.h
#ifndef SONG_CONTAINER_H
#define SONG_CONTAINER_H

#include "lvgl.h"
#include "track_cover.h"
#include "track_details_container.h"

class SongContainer
{
private:
    lv_obj_t* song_container;
    TrackCover* track_cover;
    TrackDetailsContainer* track_details_container;

public:
    // Constructor and Destructor
    SongContainer();
    ~SongContainer();

    // Public Methods
    lv_obj_t* create(lv_obj_t* parent);
    void update_song(lv_obj_t* cover, lv_obj_t* title, lv_obj_t* artist);
};

#endif // SONG_CONTAINER_H