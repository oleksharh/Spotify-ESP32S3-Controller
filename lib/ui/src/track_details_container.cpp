// Template for track_details.cpp
#include "../include/track_details_container.h"

TrackDetailsContainer::TrackDetailsContainer() : track_details_container(nullptr)
{
    track_title = new TrackTitle();
    track_artist = new TrackArtist();
}

TrackDetailsContainer::~TrackDetailsContainer()
{
    // LVGL automatically manages memory for most objects.
}

/**
 * @brief Creates the track details container and initializes all UI components.
 *
 * This function sets up the track details container and adds the track title and track artist
 * to it. The track details container is not loaded onto the display by this function,
 * it just returns the created track details container object.
 *
 * @param parent The parent object to which this track details container will be added.
 * @return lv_obj_t* The created track details container object.
 */
lv_obj_t* TrackDetailsContainer::create(lv_obj_t* parent)
{
    track_details_container = lv_obj_create(parent);

    // Object Initializations
    track_title->create(track_details_container);
    track_artist->create(track_details_container);

    // Apply styles to TrackDetailsContainer object
    lv_obj_set_size(track_details_container, LV_PCT(65), LV_PCT(60)); // 65% of parent's width and height is 60%
    lv_obj_align(track_details_container, LV_ALIGN_RIGHT_MID, 0, 0); // Align to left-middle of parent
    lv_obj_set_flex_flow(track_details_container, LV_FLEX_FLOW_COLUMN); // Arrange children in a column

    // Align children to center of parent container
    lv_obj_set_flex_align(track_details_container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);


    return track_details_container;
}

void TrackDetailsContainer::update_details(lv_obj_t* title, lv_obj_t* artist)
{
    track_title->update_title(title);
    track_artist->update_artist(artist);
}