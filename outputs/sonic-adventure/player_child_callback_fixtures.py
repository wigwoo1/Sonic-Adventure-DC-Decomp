"""Declared child-task and character-query input records; no supplied results/frames."""
from player_collision_response_fixtures import *
import player_collision_response_fixtures as previous
import write_watch_player_child_callback_dependencies as fast
v=fast.old
ENTRY=0x8c0b3a2e;GETTER=0x8c07ce40
TASK,CHILD_ACTOR,PRIMARY_ACTOR=0x8cb00000,0x8cb10000,0x8cb20000
PLAYER_TABLE=0x8c78c608;FALLBACK_KIND=0x8c7493c8;CHARACTER_TABLE=0x8c1c63a4
def prepare(ram,c):
    m,s,f,cfg=previous.prepare(ram,dict(c,entry=c.get('entry',ENTRY)))
    for a in (TASK,CHILD_ACTOR,PRIMARY_ACTOR):m[a&0xffffff:(a&0xffffff)+0x40]=bytes([0x5a])*0x40
    put(m,TASK+0x20,CHILD_ACTOR)
    put(m,CHILD_ACTOR+0x34,bits(c.get('child_index',0)))
    index=c.get('query_index',0)&255
    put(m,PLAYER_TABLE+4*index,PRIMARY_ACTOR if c.get('present',True) else 0)
    put(m,PRIMARY_ACTOR+9,c.get('kind',0),1)
    put(m,FALLBACK_KIND,c.get('fallback_kind',c.get('kind',0)),2)
    s.r[4]=c.get('query_index',0)&0xffffffff if cfg['entry']==GETTER else TASK
    return m,s,f,cfg
