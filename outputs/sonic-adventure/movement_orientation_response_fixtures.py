"""Authored actor/state/query/vector inputs, with the unchanged matrix environment."""
import movement_vectors_fixtures as previous
import write_watch_movement_orientation_response as fast
v=fast.old;P=v.P
OWNER,LOCAL,APPROACH=0x8c074ee4,0x8c078e40,0x8c055d20
A,B,C,OUT,MATRIX,STACK=(getattr(previous,n) for n in ('A','B','C','OUT','MATRIX','STACK'))
REFERENCE=previous.REFERENCE
put,get,bits,number,vector=(getattr(previous,n) for n in ('put','get','bits','number','vector'))
def prepare(ram,c):
    m,s,f,base=previous.prepare(ram,dict(c,entry=0x8c078eb0))
    cfg=v.Input(dict(base,entry=c['entry'],probe=True));s.pc=c['entry'];s.pr=c.get('pr',0x8cfed000)
    f.fpscr=(c.get('mode',0)<<19)|c.get('fpscr',0x40000)|c.get('rm',0)
    s.sr=(s.sr&~1)|c.get('t',0)
    if c.get('fd'):s.sr|=0x8000
    view=c.get('view',0x8c000000);ptr=lambda a:view|(a&0xffffff)
    for address in (A,B,C,OUT):m[address&0xffffff:(address&0xffffff)+0x200]=bytes(0x200)
    entry=c['entry']
    if entry==APPROACH:
        s.r[4],s.r[5],s.r[6]=(c[k]&0xffffffff for k in ('left','right','step'))
    else:
        angles=c.get('angles',(0,0,0))
        for j,value in enumerate(angles):put(m,A+0x14+j*4,value)
        put(m,A+4,c.get('actor_flags',0),2)
        if entry==LOCAL:
            vector(m,B,c.get('vector',(1,2,3)));s.r[4],s.r[5]=ptr(A),ptr(B)
        else:
            assert entry==OWNER
            put(m,B+0x10,c.get('status_flags',1),2)
            for offset,key,default in ((0x38,'speed',1),(0xc0,'low',0),(0xc4,'middle',0),(0xc8,'high',0),(0xd0,'cap',2)):
                put(m,B+offset,bits(c.get(key,default)))
            vector(m,C+0x1c,c.get('normal',(0,0,1)));vector(m,C+0x48,c.get('orientation',(1,0,0)))
            put(m,C+0x34,c.get('target_pitch',0x3000));put(m,C+0x38,c.get('target_roll',0xc000))
            vector(m,REFERENCE,c.get('up',(0,1,0)));s.r[4],s.r[5],s.r[6]=ptr(A),ptr(B),ptr(C)
    if c.get('bad')=='stack':s.r[15]=0x8b000000
    if c.get('bad')=='actor':s.r[4]=0x8b000000
    if c.get('bad')=='state':s.r[5]=0x8b000000
    if c.get('bad')=='query':s.r[6]=0x8b000000
    if c.get('bad')=='vector':s.r[5]=0x8b000000
    return m,s,f,cfg
