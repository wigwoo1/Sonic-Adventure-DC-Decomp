"""Declared scene inputs with original collision/particle allocation producers."""
from player_collision_attach_fixtures import *
import player_collision_attach_fixtures as previous
import write_watch_player_child_particles as fast
v=fast.old
def prepare(ram,c):return previous.prepare(ram,c)
