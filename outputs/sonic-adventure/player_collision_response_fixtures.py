"""Declared collision structures for complete0344dc; no callee results or frames."""
from player_response_clip_fixtures import *
import player_response_clip_fixtures as previous
ENTRY=0x8c0344dc
OUT=0x8cbf0000
def prepare(ram,c):
    m,s,f,cfg=previous.prepare(ram,c)
    cfg['entry']=s.pc=c.get('entry',ENTRY)
    if c.get('null_provider'):put(m,COLLISION+0xc,0)
    if c.get('null_shapes'):put(m,INFO+0xc,0)
    for j in range(4):put(m,SHAPES+0x34*j+0x24,c.get('shape_heading',0))
    put(m,0x8c752bdc,0 if c.get('debug_enabled') else 0x80,1)
    if cfg['entry']==ENTRY:
        s.r[4]=A;s.r[5]=c.get('output',OUT)
    return m,s,f,cfg
