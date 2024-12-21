#include "../include/track_title.h"

TrackTitle::TrackTitle() : track_title(nullptr) {}

TrackTitle::~TrackTitle()
{
    // LVGL automatically manages memory for most objects.
}


/**
 * @brief Creates the track title object and initializes it.
 *
 * This function creates the track title object and initializes it with the
 * default values. The track title object is not loaded onto the display by
 * this function, it just returns the created track title object.
 *
 * @param parent The parent object to which this track title will be added.
 * @return lv_obj_t* The created track title object.
 */
lv_obj_t* TrackTitle::create(lv_obj_t* parent)
{
    track_title = lv_label_create(parent);
    
    // Apply styles to TrackTitle object
    lv_obj_set_size(track_title, LV_SIZE_CONTENT, LV_SIZE_CONTENT); // Content depends on text
    lv_obj_align(track_title, LV_ALIGN_TOP_MID, 0, 0); // Align to top-middle of parent
    lv_label_set_long_mode(track_title, LV_LABEL_LONG_WRAP);  // Wrap the long lines

    return track_title;
}

void TrackTitle::update_title(lv_obj_t* title)
{
    lv_label_set_text(track_title, lv_label_get_text(title));
}