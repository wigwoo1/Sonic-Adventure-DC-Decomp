"""Original loader and initialization chain; no supplied texture descriptors."""
from player_texture_load_fixtures import *
import player_texture_load_fixtures as previous
import write_watch_player_texture_driver_defaults as fast
v=fast.old
TABLE_INIT=0x8c64e5e0
HEAP_INIT=0x8c660e58
def prepare(ram,c):return previous.prepare(ram,c)
