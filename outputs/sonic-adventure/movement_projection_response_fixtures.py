"""Ordinary entry arguments and initial geometry; no supplied game results/frames."""
import movement_response_fixtures as previous
import write_watch_movement_projection_response as fast
v=fast.old;P=v.P
OWNER,PLANES,PARALLEL,DISTANCE,ANGLE=0x8c075a40,0x8c64016c,0x8c650614,0x8c640080,0x8c055d5a
INVERSE=0x8c63a9dc
A,B,C,QUERY,OUT,CONTACTS,UP,STACK=(getattr(previous,n) for n in ('A','B','C','QUERY','OUT','CONTACTS','UP','STACK'))
put,get,bits,number,vector=(getattr(previous,n) for n in ('put','get','bits','number','vector'))

def prepare(ram,c):
    m,s,f,base=previous.prepare(ram,dict(entry=0x8c075416,case_id=c.get('case_id',0),mode=c.get('mode',0),contacts=[]))
    cfg=v.Input(dict(base,entry=c['entry'],probe=True));entry=c['entry'];s.pc=entry;s.pr=c.get('pr',0x8cfed000);s.r[15]=STACK
    f.fpscr=(c.get('mode',0)<<19)|c.get('fpscr',0x40000)|c.get('rm',0)
    s.sr=(s.sr&~1)|c.get('t',0)
    view=c.get('view',0x8c000000);ptr=lambda a:view|(a&0xffffff)
    for a in (A,B,C,QUERY,OUT):m[a&0xffffff:(a&0xffffff)+0x200]=bytes(0x200)
    if entry==ANGLE:
        s.r[4],s.r[5]=c['left']&0xffffffff,c['right']&0xffffffff
    elif entry==INVERSE:
        f.banks[(f.fpscr>>21)&1][4]=c.get('word',bits(c.get('value',1)))
    elif entry in (PLANES,PARALLEL):
        vector(m,A,(*c.get('point_a',(1,2,3)),*c.get('normal_a',(1,0,0))))
        vector(m,B,(*c.get('point_b',(4,5,6)),*c.get('normal_b',(0,1,0))))
        m[OUT&0xffffff:(OUT&0xffffff)+32]=bytes([0x55])*32
        s.r[4],s.r[5],s.r[6]=ptr(A),ptr(B),ptr(OUT) if c.get('output',True) else 0
        if c.get('alias')=='first':s.r[6]=s.r[4]
        if c.get('alias')=='second':s.r[6]=s.r[5]
    elif entry==DISTANCE:
        vector(m,A,c.get('point',(1,2,3)));vector(m,B,(*c.get('plane_point',(4,5,6)),*c.get('normal',(1,0,0))))
        m[OUT&0xffffff:(OUT&0xffffff)+32]=bytes([0x55])*32
        s.r[4],s.r[5],s.r[6]=ptr(A),ptr(B),ptr(OUT) if c.get('output',True) else 0
        if c.get('alias')=='first':s.r[6]=s.r[4]
        if c.get('alias')=='second':s.r[6]=s.r[5]
    else:
        assert entry==OWNER
        vector(m,B+4,c.get('velocity',(3,0,0)));put(m,B+0x20,c.get('prior_angle',0))
        put(m,A+0x18,c.get('angle',0))
        for offset,key,default in ((0x38,'speed',1),(0xc0,'low',0),(0xc4,'threshold',2),(0xcc,'cap',2)):
            put(m,C+offset,bits(c.get(key,default)))
        put(m,C+0x60,c.get('state_flags',0))
        vector(m,QUERY,(c.get('radius',1),*c.get('point',(0,0,0)),*c.get('motion',(-2,0,0)),*c.get('normal',(-1,0,0))))
        vector(m,QUERY+0x60,c.get('anchor',(0,0,0)));vector(m,QUERY+0x6c,c.get('delta',(1,0,0)))
        contacts=c.get('contacts',[dict()]);put(m,QUERY+0x28,len(contacts),2)
        for j,r in enumerate(contacts):
            a=CONTACTS+j*0x58;m[a&0xffffff:(a&0xffffff)+0x58]=bytes(0x58)
            vector(m,a+0x14,r.get('normal',(1,0,0)));vector(m,a+0x20,r.get('plane_point',(.5,0,0)));vector(m,a+0x2c,r.get('point',(.5,0,0)))
        vector(m,UP,c.get('up',(0,1,0)));put(m,STACK,c.get('flags',0))
        s.r[4],s.r[5],s.r[6],s.r[7]=ptr(A),ptr(B),ptr(C),ptr(QUERY)
    if c.get('bad')=='stack':s.r[15]=0x8b000000
    if c.get('bad')=='first':s.r[4]=0x8b000000
    if c.get('bad')=='second':s.r[5]=0x8b000000
    if c.get('bad')=='output':s.r[6]=0x8b000000
    if c.get('bad')=='query':s.r[7]=0x8b000000
    return m,s,f,cfg
