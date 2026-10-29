"""Authored task/entity/moving-object input records;actual producers stay unchanged."""
from movement_special_tail_fixtures import *
import movement_special_tail_fixtures as previous
TASK,ENTITY,DATA=0x8ca50000,0x8ca60000,0x8ca70000

def prepare(ram,c):
    m,s,f,cfg=previous.prepare(ram,c)
    view=c.get('view',0x8c000000);ptr=lambda a:view|(a&0xffffff)
    for address in (TASK,ENTITY,DATA):m[address&0xffffff:(address&0xffffff)+0x200]=bytes(0x200)
    put(m,TASK+0x20,ptr(ENTITY));put(m,ENTITY+4,c.get('entity_flags',0x100),2)
    put(m,TASK+0x28,ptr(DATA) if c.get('moving_data',False) else 0)
    for offset in (0,0x1c):
        put(m,DATA+offset,0)
        for j,x in enumerate(c.get('moving_angles',(0,0,0))):put(m,DATA+offset+4+4*j,x)
        vector(m,DATA+offset+0x10,c.get('moving_offset',(0,0,0)))
    return m,s,f,cfg
