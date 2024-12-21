// Template for track_cover.cpp
#include "../include/track_cover.h"

TrackCover::TrackCover() : track_cover(nullptr) {}

TrackCover::~TrackCover()
{
    // LVGL automatically manages memory for most objects.
}

/**
 * @brief Creates the track cover object and initializes it.
 *
 * This function creates the track cover object and initializes it with the
 * default values. The track cover object is not loaded onto the display by
 * this function, it just returns the created track cover object.
 *
 * @param parent The parent object to which this track cover will be added.
 * @return lv_obj_t* The created track cover object.
 */
lv_obj_t* TrackCover::create(lv_obj_t* parent)
{
    track_cover = lv_img_create(parent);

    // Apply styles to TrackCover object
    lv_obj_set_size(track_cover, LV_PCT(30), LV_PCT(60)); // 30% of parent's width and height is 60% for 480*320 screen it's ~128*128
    lv_obj_align(track_cover, LV_ALIGN_LEFT_MID, 0, 0); // Align to left-middle of parent
}

void TrackCover::update_cover(lv_obj_t* cover)
{
    lv_img_set_src(track_cover, cover);
}