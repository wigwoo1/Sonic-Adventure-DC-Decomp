"""Ordinary allocation/release inputs with actual matched graphics-state carry."""
from player_texture_draw_fixtures import *
import player_texture_draw_fixtures as previous
import write_watch_renderer_context_owner_state as fast
v=fast.old.old
def prepare(ram,c):return previous.prepare(ram,c)
