# -------------------------- Foreground tileset loading -----------------------------------------
#          name          file                        compression  opt
TILESET    fg_tileset    "tileset_fg.tsx"     FAST		  ALL

# -------------------------- Foreground map loading ---------------------------------------------
#          name          tmx_file                    layer_id     ts_compression  map_compression
MAP        fg_map_def    "map.tmx"            layer_a	  FAST            FAST
# note: layer_id - layer name from Tiled

# -------------------------- Foreground palette loading -----------------------------------------
#          name          file
PALETTE    fg_palette    "tileset_fg.png"

# -------------------------- Background map loading ---------------------------------------------
#          name          tmx_file                    layer_id     ts_compression  map_compression
MAP        bg_map_def    "map.tmx"            layer_b	  FAST 	          FAST

# note: see bin/rescomp.txt for a detailed description of commands, arguments,
# and available additional optional arguments