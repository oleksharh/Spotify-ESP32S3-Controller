#include "../include/ui.h"

void UiLib::initialize(lv_obj_t* parent) {
    lv_obj_t* screen = Screen::create();
    SongContainer::create(screen);
    TrackCover::create(screen);
    TrackDetailsContainer::create(screen);
    TrackTitle::create(screen);
    TrackArtist::create(screen);
    MediaContainer::create(screen);
    PlaybackBar::create(screen);
    MediaBttnsContainer::create(screen);
    ShuffleBttn::create(screen);
    PrevBttn::create(screen);
    PlayBttn::create(screen);
    NextBttn::create(screen);
    RepeatBttn::create(screen);
}
