"""Retained scene inputs with additive original texture-loading code."""
from player_child_particles_fixtures import *
import player_child_particles_fixtures as previous
import write_watch_player_texture_load as fast
v=fast.old
TEXTURE_LOAD=0x8c099750
FILE_DECODE=0x8c0997bc
DESCRIPTOR_INIT=0x8c607264
TEXTURE_DECODE=0x8c608134
TEXTURE_LIST=0x8c1c3068
RESOURCE_NAME=0x8c15a16c
def prepare(ram,c):return previous.prepare(ram,c)
