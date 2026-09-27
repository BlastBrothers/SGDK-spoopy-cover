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
#define TITLE_SCROLL_NUM_TILES  8
#define TITLE_SCROLL_SPEED      3

s16 bg_xpos = 0;
s16 cloudScroll[CLOUD_SCROLL_NUM_TILES]; //holds per-tile hscroll values for BG clouds
s16 titleScroll[CLOUD_SCROLL_NUM_TILES] = {1, 1, 1, 0, -1, -1, -1, 0}; //holds per-tile hscroll values for FG title

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

    // Set palette for foreground
    PAL_setPalette(PAL0, fg_palette.data, DMA);

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
        cloudScroll[8] = bg_xpos;
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
        }
        

        // update line scroll
        VDP_setHorizontalScrollTile(BG_B, 0, cloudScroll, CLOUD_SCROLL_NUM_TILES, DMA_QUEUE);
        VDP_setHorizontalScrollTile(BG_A, 0, titleScroll, TITLE_SCROLL_NUM_TILES, DMA_QUEUE);

        SYS_doVBlankProcess();      // Wait for VBlank processes
    }
}