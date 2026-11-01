"""Declared player/effect/camera inputs with the full original effect family."""
from player_child_callback_fixtures import *
import player_child_callback_fixtures as previous
import write_watch_player_child_update as fast
v=fast.old
INIT=0x8c0b3a2e;UPDATE=0x8c0b3b1a;CLEANUP=0x8c0b3afc;DRAW=0x8c0b3f2c
ALLOCATE=0x8c0b33a0;REMOVE=0x8c0b33ce;SPAWN=0x8c0b37f2;GEOMETRY=0x8c0b3480
STATE_TABLE=0x8c78c668;RENDER_FLAG=0x8c754ec8
def prepare(ram,c):
    m,s,f,cfg=previous.prepare(ram,dict(c,entry=c.get('entry',INIT)))
    m[TASK&0xffffff:(TASK&0xffffff)+0x30]=bytes(0x30)
    m[CHILD_ACTOR&0xffffff:(CHILD_ACTOR&0xffffff)+0x40]=bytes(0x40)
    put(m,TASK+0x20,CHILD_ACTOR);put(m,CHILD_ACTOR+0x34,bits(c.get('child_index',0)))
    put(m,RENDER_FLAG,c.get('render_flag',0))
    if c.get('real_tables'):
        for table in (PLAYER_TABLE,STATE_TABLE):
            for j in (0,1):put(m,table+4*j,0)
    else:
        m[PRIMARY_ACTOR&0xffffff:(PRIMARY_ACTOR&0xffffff)+0x40]=m[A&0xffffff:(A&0xffffff)+0x40]
        put(m,PRIMARY_ACTOR+9,c.get('kind',0),1);put(m,PRIMARY_ACTOR+1,c.get('action',0),1)
        put(m,PLAYER_TABLE+4*c.get('child_index',0),PRIMARY_ACTOR)
        put(m,STATE_TABLE+4*c.get('child_index',0),C)
        put(m,C+0x1f4,c.get('environment_flags',0))
        vector(m,C+0x1c4,c.get('cached_vector',(0,0,0)))
    s.r[4]=TASK
    return m,s,f,cfg
