#ifndef UI_LIBRARY_H
#define UI_LIBRARY_H

#include "screen.h"
#include "song_container.h"
#include "track_cover.h"
#include "track_details_container.h"
#include "track_title.h"
#include "track_artist.h"
#include "media_container.h"
#include "playback_bar.h"
#include "media_bttns_container.h"
#include "shuffle_bttn.h"
#include "prev_bttn.h"
#include "play_bttn.h"
#include "next_bttn.h"
#include "repeat_bttn.h"

class UiLib {
public:
    static void initialize(lv_obj_t* parent);
};

#endif // UI_LIBRARY_H
