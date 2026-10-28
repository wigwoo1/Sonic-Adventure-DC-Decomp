"""Ordinary entry arguments and authored geometry; no supplied outputs/frames."""
import movement_vectors_fixtures as previous
import write_watch_movement_surface_resolution as fast
v=fast.old;P=v.P
A,B,C,Q,D,V,W=0x8cb00000,0x8cb10000,0x8cb20000,0x8cb30000,0x8cb50000,0x8cb60000,0x8cb70000
OWNER,REDIRECT,CLEAR=0x8c0747d2,0x8c0763a0,0x8c07256a
TABLE,ROTATION=0x8c78c628,0x8c1bdfe0
put,get,bits,number,vector=(getattr(previous,n) for n in ('put','get','bits','number','vector'))
def prepare(ram,c):
    m,s,f,base=previous.prepare(ram,dict(c,entry=0x8c078eb0))
    cfg=v.Input(dict(base,entry=c['entry'],probe=True));s.pc=c['entry'];s.pr=0x8cfed000
    f.fpscr=(c.get('mode',0)<<19)|c.get('fpscr',0x40000)|c.get('rm',0)
    s.sr=(s.sr&~1)|c.get('t',0)
    if c.get('fd'):s.sr|=0x8000
    view=c.get('view',0x8c000000);ptr=lambda a:view|(a&0xffffff)
    for a in (A,B,C,Q,D,V,W):m[a&0xffffff:(a&0xffffff)+0x200]=bytes(0x200)
    for j,x in enumerate(c.get('angles',(0,0,0))):put(m,A+0x14+4*j,x)
    put(m,A+4,c.get('grounded',1),2);put(m,A+8,c.get('index',0),1);put(m,A+9,c.get('character_kind',0),1)
    vector(m,A+0x20,c.get('position',(1,2,3)))
    vector(m,B+4,c.get('aux_velocity',(-1,0,0)))
    put(m,C+0x10,c.get('status',1),2);put(m,C+0x60,c.get('state_flags',0))
    vector(m,C+0x38,c.get('state_velocity',(1,0,0)))
    put(m,C+0xc0,bits(c.get('low',0)));put(m,C+0xcc,bits(c.get('cap',20)))
    vector(m,Q+4,c.get('point',(0,0,0)));vector(m,Q+0x10,c.get('motion',(3,0,0)))
    vector(m,Q+0x1c,c.get('secondary_normal',(0,0,1)))
    vector(m,Q+0x48,c.get('normal',(0,1,0)));vector(m,Q+0x54,c.get('plane_point',(0,0,0)))
    vector(m,Q+0x60,c.get('anchor',(10,20,30)));vector(m,Q+0x6c,c.get('delta',(2,0,0)))
    vector(m,D+4,c.get('character_velocity',(7,0,0)));put(m,D+0x20,c.get('character_heading',0))
    put(m,TABLE+4*c.get('index',0),ptr(D));put(m,ROTATION,c.get('rotate_global',0))
    direction=c.get('direction',(1,0,0));origin=c.get('position',(1,2,3))
    vector(m,V,tuple(a+b for a,b in zip(origin,direction)) if c['entry']==OWNER else direction)
    vector(m,W,c.get('offset',(.5,1,2)))
    for field,address in (('normal_words',Q+0x48),('direction_words',V),('motion_words',Q+0x10),('state_words',C+0x38)):
        for j,word in enumerate(c.get(field,())):put(m,address+4*j,word)
    entry=c['entry']
    if entry==OWNER:
        s.r[4],s.r[5],s.r[6],s.r[7]=ptr(A),ptr(B),ptr(C),ptr(Q)
        for j,x in enumerate((ptr(V),ptr(W),c.get('prior_return',9),c.get('flags',0))):put(m,s.r[15]+4*j,x)
    elif entry==REDIRECT:s.r[4],s.r[5],s.r[6],s.r[7]=ptr(A),ptr(C),ptr(Q),ptr(V)
    else:
        assert entry==CLEAR
        s.r[4],s.r[5]=ptr(Q) if c.get('query_present',True) else 0,ptr(C) if c.get('state_present',True) else 0
    bad=c.get('bad')
    if bad=='stack':s.r[15]=0x8b000000
    elif bad=='actor':s.r[4]=0x8b000000
    elif bad=='state':s.r[6 if entry==OWNER else 5]=0x8b000000
    elif bad=='query':s.r[7 if entry==OWNER else 6 if entry==REDIRECT else 4]=0x8b000000
    elif bad=='direction':s.r[7]=0x8b000000
    elif bad=='target':put(m,s.r[15],0x8b000000)
    elif bad=='offset':put(m,s.r[15]+4,0x8b000000)
    elif bad=='character':put(m,TABLE+4*c.get('index',0),0x8b000000)
    return m,s,f,cfg
