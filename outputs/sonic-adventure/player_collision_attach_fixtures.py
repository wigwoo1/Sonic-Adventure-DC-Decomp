"""Retain declared scene inputs; execute original collision allocation/attachment."""
from player_child_update_fixtures import *
import player_child_update_fixtures as previous
import write_watch_player_collision_attach as fast
v=fast.old
ATTACH=0x8c02f4b0
EXTENT=0x8c02f400
COPY=0x8c604500

def prepare(ram,c):
    return previous.prepare(ram,c)
