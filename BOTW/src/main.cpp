#include "stdafx.h"

#ifdef OS_WINDOWS
#include <SDL/SDL_syswm.h>
#endif
#include <Vorb/Vorb.h>
#define VORB_IMPL_UI_SDL
#define VORB_IMPL_SOUND_FMOD
#define VORB_IMPL_FONT_SDL
#include <Vorb/VorbLibs.h>

#include "App.h"

// Entry point
int main(int argc, char** argv) {
    // Initialize Vorb modules
    vorb::init(vorb::InitParam::ALL);

    // Run the game
    App().run();

    // Dispose Vorb modules
    vorb::dispose(vorb::InitParam::ALL);

    // Genesis
    return 0;
}