// *****************************************************************************
//  Adapted from "TMX Map Sample" by werton playskin
//  Made for battleofthebits.com's Halloween sgen compo, as a cover art submission
//  Sept 2026
// *****************************************************************************

#include <genesis.h>
#include "maps.h"

#define SCREEN_WIDTH            320
#define SCREEN_HEIGHT           224

s16 bg_xpos;

// Declare foreground and background map's pointers
Map *fgMap;
Map *bgMap;

// Forward declarations
_Noreturn void MainLoop();
void Init();
void CameraUpdate();

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
    VDP_setScreenWidth320();

    // Set the maximum number of tiles for the sprite engine.
    SPR_initEx(40);

    // Set the initial VRAM tile index from which we will load the first tileset
    u16 nextToFillTileIndex = TILE_USER_INDEX;
    
    // Load tileset and create foreground map
    VDP_loadTileSet(&fg_tileset, nextToFillTileIndex, DMA);
    fgMap = MAP_create(&fg_map_def, BG_A, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, nextToFillTileIndex));
    
    // Update tile index for next tileset
    nextToFillTileIndex += fg_tileset.numTile;
    
    // Load background tileset and create background map
    VDP_loadTileSet(&bg_tileset, nextToFillTileIndex, DMA);
    bgMap = MAP_create(&bg_map_def, BG_B, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, nextToFillTileIndex));
    
    // Initial camera positioning
    CameraUpdate();

    //reset FG scroll. IDK if this is required, but I'm putting it in.
    MAP_scrollTo(fgMap, 0, 0);
    
    // Set palettes for foreground, background
    PAL_setPalette(PAL0, fg_palette.data, DMA);
    PAL_setPalette(PAL1, bg_palette.data, DMA);

    SYS_doVBlankProcess();
}

_Noreturn void MainLoop()
{
    while (TRUE)
    {
        CameraUpdate();         // Update camera position
        SYS_doVBlankProcess();      // Wait for VBlank processes
    }
}


void CameraUpdate()
{
    // Calculate camera position
    bg_xpos += 1;
    
    // Scroll BG
    MAP_scrollTo(bgMap, bg_xpos, 0);
}
