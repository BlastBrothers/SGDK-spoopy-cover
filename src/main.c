// *****************************************************************************
//  Adapted from "TMX Map Sample" by werton playskin
//  Made for battleofthebits.com's Halloween sgen compo, as a cover art submission
//  Sept 2026
// *****************************************************************************

#include <genesis.h>
#include "maps.h"

#define SCREEN_WIDTH_PIX        320
#define SCREEN_HEIGHT_PIX       224
#define SCREEN_WIDTH_TILES      64
#define SCREEN_HEIGHT_TILES     28
#define CLOUD_SCROLL_NUM_TILES  13
#define TITLE_SCROLL_NUM_TILES  9
#define TITLE_SCROLL_SPEED      3

s16 bg_xpos = 0;
s16 cloudScroll[CLOUD_SCROLL_NUM_TILES]; //holds per-tile hscroll values for BG clouds
//Only push right, so the wavy building on plane A doesn't pop in front of the static building on the window layer
//This would be un-noticeable, but the gap between the title and the building is only 8px, so either part of the title gets cut off, or part of the building scrolls.
const s16 titleScrollInitial[7] = {0, 0, 0, 1, 2, 2, 1}; 
s16 titleScroll[TITLE_SCROLL_NUM_TILES] = {}; //holds per-tile hscroll values for FG title


// Declare foreground and background map's pointers
Map *fgMap;

// Forward declarations
_Noreturn void MainLoop();
void Init();

int main(bool hardReset)
{
    // do hard reset on soft reset to nullify global variables
    if (!hardReset)
        SYS_hardReset();
    
    Init();
    MainLoop();
    return 0; // unreachable, MainLoop is _Noreturn but keep signature
}

void Init()
{
    VDP_setPlaneSize(SCREEN_WIDTH_TILES, SCREEN_HEIGHT_TILES, TRUE);
    VDP_setScreenWidth320();
    VDP_setScrollingMode(HSCROLL_TILE, VSCROLL_PLANE);

    // Set the maximum number of tiles for the sprite engine.
    SPR_initEx(40);

    // Set the initial VRAM tile index from which we will load the first tileset
    u16 nextTileIndex = TILE_USER_INDEX;
    
    // Load tileset and create tilemaps
    VDP_loadTileSet(&fg_tileset, nextTileIndex, DMA);
    fgMap = MAP_create(&fg_map_def, BG_A, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, nextTileIndex));
    VDP_setTileMapEx(BG_B, &bg_tilemap, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, nextTileIndex), 0, 0, 0, 0, 
    SCREEN_WIDTH_TILES, SCREEN_HEIGHT_TILES, DMA_QUEUE_COPY);

    //reset scroll. As a side effect this loads in the initial map data.
    MAP_scrollTo(fgMap, 0, 0);

    //set up window layer as a copy of Plane A, set to fill the right of screen.
    //This lets the title have linescroll while the building remains stationary
    VDP_setWindowAddress(VDP_getPlaneAddress(BG_A, 0, 0));
    VDP_setWindowOnRight(9);

    // Set palette for foreground
    PAL_setPalette(PAL0, fg_palette.data, DMA);

    //prep title linescroll
    titleScroll[0] = titleScrollInitial[1];
    titleScroll[1] = titleScrollInitial[2];
    titleScroll[2] = titleScrollInitial[3];
    titleScroll[3] = titleScrollInitial[4];
    titleScroll[4] = titleScrollInitial[5];
    titleScroll[5] = titleScrollInitial[6];
    titleScroll[6] = titleScrollInitial[7];
    titleScroll[7] = titleScrollInitial[8];
    titleScroll[8] = titleScrollInitial[0];

    SYS_doVBlankProcess();
}

_Noreturn void MainLoop()
{
    s16 scrollCountdown = TITLE_SCROLL_SPEED;
    while (TRUE)
    {
        scrollCountdown -= 1;

        if (scrollCountdown == -1) {
            scrollCountdown = TITLE_SCROLL_SPEED;
        }

        //Calculate cloud linescroll
        bg_xpos -= 1; //starting value
        cloudScroll[0] = bg_xpos << 1;
        cloudScroll[1] = bg_xpos << 1;
        cloudScroll[2] = bg_xpos << 1;
        cloudScroll[3] = bg_xpos << 1;
        cloudScroll[4] = bg_xpos << 1;
        cloudScroll[5] = bg_xpos;
        cloudScroll[6] = bg_xpos;
        cloudScroll[7] = bg_xpos;
        cloudScroll[8] = bg_xpos >> 1;
        cloudScroll[9] = bg_xpos >> 1;
        cloudScroll[10] = bg_xpos >> 1;
        cloudScroll[11] = 0;
        cloudScroll[12] = 0;

        //calculate title linescroll
        if (scrollCountdown == TITLE_SCROLL_SPEED) {
            s16 temp = titleScroll[0];
            titleScroll[0] = titleScroll[1];
            titleScroll[1] = titleScroll[2];
            titleScroll[2] = titleScroll[3];
            titleScroll[3] = titleScroll[4];
            titleScroll[4] = titleScroll[5];
            titleScroll[5] = titleScroll[6];
            titleScroll[6] = titleScroll[7];
            titleScroll[7] = temp;
            titleScroll[8] = titleScroll[0];
        }
        

        // update line scroll
        VDP_setHorizontalScrollTile(BG_B, 0, cloudScroll, CLOUD_SCROLL_NUM_TILES, DMA_QUEUE);
        VDP_setHorizontalScrollTile(BG_A, 2, titleScroll, TITLE_SCROLL_NUM_TILES, DMA_QUEUE);

        SYS_doVBlankProcess();      // Wait for VBlank processes
    }
}