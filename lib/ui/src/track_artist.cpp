#include "../include/track_artist.h"

TrackArtist::TrackArtist() : track_artist(nullptr) {}

TrackArtist::~TrackArtist()
{
    // LVGL automatically manages memory for most objects.
}

/**
 * @brief Creates the track artist object and initializes it.
 *
 * This function creates the track artist object and initializes it with the
 * default values. The track artist object is not loaded onto the display by
 * this function, it just returns the created track artist object.
 *
 * @param parent The parent object to which this track artist will be added.
 * @return lv_obj_t* The created track artist object.
 */
lv_obj_t* TrackArtist::create(lv_obj_t* parent)
{
    track_artist = lv_label_create(parent);
    
    // Apply styles to TrackArtist object
    lv_obj_set_size(track_artist, LV_SIZE_CONTENT, LV_SIZE_CONTENT); // Content depends on text
    lv_obj_align(track_artist, LV_ALIGN_CENTER, 0, 0); // Align to bottom-middle of parent
    lv_label_set_long_mode(track_artist, LV_LABEL_LONG_WRAP);  // Wrap the long lines

    return track_artist;
}

void TrackArtist::update_artist(lv_obj_t* artist)
{
    lv_label_set_text(track_artist, lv_label_get_text(artist));
}