"""Ordinary actor/motion/world/collision inputs for the complete072b44 entry."""
import write_watch_player_movement_path as fast
import movement_special_tail_fixtures as previous
v=fast.old
P=v.P
A,B,C,Q,D=previous.A,previous.B,previous.C,previous.Q,previous.D
put,get,bits,number,vector=(getattr(previous,n) for n in ('put','get','bits','number','vector'))
ENTRY=0x8c072b44
COLLISION,INFO,SHAPES,PROVIDER,OTHER_TASK,OTHER_ACTOR=0x8ca90000,0x8caa0000,0x8cab0000,0x8cad0000,0x8cae0000,0x8caf0000

def collision_inputs(m,c,actor=A):
    for address in (COLLISION,INFO,SHAPES,PROVIDER,OTHER_TASK,OTHER_ACTOR):
        m[address&0xffffff:(address&0xffffff)+0x200]=bytes(0x200)
    put(m,actor+0x38,COLLISION)
    put(m,COLLISION+2,c.get('collision_flags',0),2)
    put(m,COLLISION+0xc,PROVIDER);put(m,COLLISION+0x16,c.get('collision_index',0),2);put(m,COLLISION+0x18,INFO)
    put(m,INFO+4,4,2);put(m,INFO+6,0,2);put(m,INFO+0xc,SHAPES);put(m,INFO+0x10,OTHER_TASK)
    put(m,OTHER_TASK+0x20,OTHER_ACTOR)
    put(m,OTHER_ACTOR+2,c.get('other_kind',12),1);put(m,OTHER_ACTOR+4,1,2)
    for j,x in enumerate(c.get('other_angles',(0,0,0))):put(m,OTHER_ACTOR+0x14+4*j,x)
    vector(m,OTHER_ACTOR+0x20,c.get('other_position',(0,0,0)))
    put(m,PROVIDER+4,c.get('provider_flags',0),1);put(m,PROVIDER+0x14,bits(c.get('provider_height',1)))
    for j in range(4):
        shape=SHAPES+0x34*j
        put(m,shape,c.get('shape_type',1));put(m,shape+4,c.get('shape_flags',0),1);put(m,shape+5,c.get('shape_enabled',1),1)
        put(m,shape+0x14,bits(c.get('shape_height',2)));put(m,shape+0x1c,bits(c.get('shape_radius',1)))
        vector(m,shape+8,c.get('shape_position',(0,0,0)))

def prepare(ram,c):
    m,s,f,cfg=previous.prepare(ram,c)
    cfg['entry']=ENTRY;s.pc=ENTRY;s.pr=0x8cfed000
    collision_inputs(m,c)
    put(m,C+0x5c,c.get('previous_surface_flags',0))
    # R7 is a real fourth integer argument;the original entry preserves it inR11.
    s.r[7]=c.get('fourth_argument',0)
    return m,s,f,cfg
