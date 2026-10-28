"""Initial actor, movement records and real original callback arguments only."""
import movement_vectors_fixtures as previous
import write_watch_movement_record_adjustment as fast
v=fast.old;P=v.P
A,B,C,OUT=(getattr(previous,n) for n in ('A','B','C','OUT'))
D,RECORD,HOLDER=0x8cb50000,0x8cb60000,0x8cb70000
OWNER,IMPULSE=0x8c07506e,0x8c0751c0
TABLE=0x8c78c648
put,get,bits,number,vector=(getattr(previous,n) for n in ('put','get','bits','number','vector'))

def prepare(ram,c):
    m,s,f,base=previous.prepare(ram,dict(c,entry=0x8c078eb0))
    cfg=v.Input(dict(base,entry=c['entry'],probe=True));s.pc=c['entry'];s.pr=0x8cfed000
    f.fpscr=(c.get('mode',0)<<19)|c.get('fpscr',0x40000)|c.get('rm',0)
    s.sr=(s.sr&~1)|c.get('t',0)
    if c.get('fd'):s.sr|=0x8000
    view=c.get('view',0x8c000000);ptr=lambda a:view|(a&0xffffff)
    for a in (A,B,C,OUT,D,RECORD,HOLDER):m[a&0xffffff:(a&0xffffff)+0x200]=bytes(0x200)
    actor,query,displacement,character=A,C,OUT,D
    alias=c.get('alias')
    if alias=='query_actor':query=A+0x1c
    if alias=='displacement_actor':displacement=A+0x20
    if alias=='character_actor':character=A
    if alias=='query_displacement':query=OUT-4
    index=c.get('index',0);selected=RECORD+(0x1c if index else 0)
    put(m,A+8,index,1)
    vector(m,A+0x20,c.get('position',(10,20,30)))
    for i,value in enumerate(c.get('angles',(0xffff,0x10000,0xffff8000))):put(m,A+0x14+4*i,value)
    put(m,A+4,bits(7));put(m,A+0xc,bits(11))
    vector(m,query+4,c.get('query',(100,200,300)))
    vector(m,displacement,c.get('displacement',(4,5,6)))
    if character!=A:vector(m,character+0x20,(40,50,60))
    vector(m,B+0x38,c.get('velocity',(0,0,0)))
    for a,word in zip((B+0x38,B+0x3c,B+0x40),c.get('velocity_words',())):put(m,a,word)
    put(m,B+0x6c,ptr(HOLDER));put(m,HOLDER+0x28,ptr(RECORD) if c.get('enabled',True) else 0)
    # Both28-byte slots are live input records. No callback return/result is
    # injected.0751c0, when selected, executes all its original instructions.
    for slot in (0,1):
        a=RECORD+slot*0x1c
        put(m,a,c.get('callback',0))
        for j,x in enumerate(((3,-7,0x8001),(-5,9,0xffff))[slot]):put(m,a+4+4*j,x)
        vector(m,a+0x10,((1,-2,.5),(-3,4,-.5))[slot])
    vector(m,selected+0x3c,c.get('impulse',(1,2,3)))
    put(m,TABLE+index*4,ptr(character))
    if c['entry']==IMPULSE:
        vector(m,A+0x20,c.get('position',(10,20,30)))
        vector(m,C+4,c.get('query',(100,200,300)))
        selected=RECORD;vector(m,selected+0x3c,c.get('impulse',(1,2,3)))
        s.r[4],s.r[5],s.r[6]=ptr(A),ptr(C),ptr(selected)
    else:
        assert c['entry']==OWNER
        s.r[4],s.r[5],s.r[6],s.r[7]=ptr(A),ptr(B),ptr(query),ptr(displacement)
    for j,word in enumerate(c.get('impulse_words',())):put(m,selected+0x3c+4*j,word)
    bad=c.get('bad')
    if bad=='stack':s.r[15]=0x8b000000
    elif bad=='actor':s.r[4]=0x8b000000
    elif bad=='state':s.r[5]=0x8b000000
    elif bad=='query':s.r[6 if c['entry']==OWNER else 5]=0x8b000000
    elif bad=='displacement':s.r[7]=0x8b000000
    elif bad=='holder':put(m,B+0x6c,0x8b000000)
    elif bad=='record':put(m,HOLDER+0x28,0x8b000000)
    elif bad=='impulse':s.r[6]=0x8b000000
    elif bad=='character':put(m,TABLE+index*4,0x8b000000)
    return m,s,f,cfg
