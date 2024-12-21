// Template for song_container.cpp
#include "../include/song_container.h"

SongContainer::SongContainer() : song_container(nullptr)
{
    track_cover = new TrackCover();
    track_details_container = new TrackDetailsContainer();
}

SongContainer::~SongContainer()
{
    // LVGL automatically manages memory for most objects.
}

/**
 * @brief Creates the song container and initializes all UI components.
 *
 * This function sets up the song container and adds the track cover and track details
 * container to it. The song container is not loaded onto the display by this function,
 * it just returns the created song container object.
 *
 * @param parent The parent object to which this song container will be added.
 * @return lv_obj_t* The created song container object.
 */
lv_obj_t* SongContainer::create(lv_obj_t* parent)
{
    song_container = lv_obj_create(parent);

    // Object Initializations
    track_cover->create(song_container);
    track_details_container->create(song_container);

    // Apply styles to SongContainer object
    // lv_object_remove_flag(song_container, LV_OBJ_FLAG_SCROLLABLE); // Disable scroll on focus 
    // TODO: CHECK IF FUNCTION ABOVE WORKS

    lv_obj_set_size(song_container, LV_PCT(100), LV_PCT(70)); // 70% of parent's height and width is 100%
    lv_obj_align(song_container, LV_ALIGN_TOP_MID, 0, 0); // Align to top-middle of parent
    lv_obj_set_flex_flow(song_container, LV_FLEX_FLOW_ROW); // Arrange children in a row

    // Align children to center of parent container
    lv_obj_set_flex_align(song_container, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER); 


    return song_container;
    
}

/**
 * @brief Updates the song details in the song container.
 *
 * This function updates the song details in the song container with the new details
 * provided as arguments.
 *
 * @param cover The new cover image object.
 * @param title The new title object.
 * @param artist The new artist object.
 */
void SongContainer::update_song(lv_obj_t* cover, lv_obj_t* title, lv_obj_t* artist)
{
    track_cover->update_cover(cover);
    track_details_container->update_details(title, artist);
}