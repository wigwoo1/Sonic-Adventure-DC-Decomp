"""Additive ordinary collision/state/heading inputs for original072946."""
from player_movement_owner_fixtures import *
import player_movement_owner_fixtures as previous
import write_watch_player_response_draw as fast
v=fast.old
ENTRY=0x8c072946
def prepare(ram,c):
    m,s,f,cfg=previous.prepare(ram,c)
    s.pc=cfg['entry']=c.get('entry',ENTRY)
    put(m,B+0x20,c.get('previous_heading',0))
    put(m,OTHER_ACTOR+4,c.get('other_flags',1),2)
    put(m,INFO+4,c.get('collision_count',4),2)
    if c.get('null_collision_info'):put(m,COLLISION+0x18,0)
    if c.get('state_alias_aux'):
        # A declared R6/R5 input alias,not a supplied callee result or frame.
        # Original stores to state+44/+48 overlap velocityY/Z;state+4c is
        # just beyond the three velocity components. No private stack writes.
        s.r[6]=B-0x3c
        vector(m,s.r[6]+0x50,c.get('old_normal',(0,1,0)))
    return m,s,f,cfg
