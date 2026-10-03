# -------------------------- Foreground tileset loading -----------------------------------------
#          name          file                 compression   opt
TILESET    fg_tileset    "tileset_fg.tsx"     AUTO		    ALL

# -------------------------- Foreground map loading ---------------------------------------------
#          name          tmx_file             layer_id    ts_compression  map_compression
MAP        fg_map_def    "map.tmx"            layer_a	  AUTO            AUTO
# note: layer_id - layer name from Tiled

# -------------------------- Foreground palette loading -----------------------------------------
#          name          file
PALETTE    fg_palette    "tileset_fg.png"

# -------------------------- Background map loading ---------------------------------------------
#          name          tmx_file             layer_id    ts_compression  map_compression
TILEMAP    bg_tilemap    "map.tmx"            layer_b	  AUTO 	          AUTO

# note: see bin/rescomp.txt for a detailed description of commands, arguments,
# and available additional optional arguments