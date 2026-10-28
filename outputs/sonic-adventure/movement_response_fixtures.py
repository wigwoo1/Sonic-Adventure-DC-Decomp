"""Authored vector/state/query/contact INPUTS; no callee outputs or execution frames."""
import struct
import collision_contact_fixtures as previous
import write_watch_movement_response as fast
v=fast.old;P=v.P
A,B,C,QUERY,OUT=0x8cb00000,0x8cb10000,0x8cb20000,0x8cb30000,0x8cb40000
CONTACTS,UP=0x8c6bacf0,0x8c754bdc
STACK=0x8cf00000
def bits(x):return struct.unpack('<I',struct.pack('<f',x))[0]
def number(x):return struct.unpack('<f',int(x).to_bytes(4,'little'))[0]
def put(m,a,x,n=4):m[a&0xffffff:(a&0xffffff)+n]=(int(x)&((1<<(8*n))-1)).to_bytes(n,'little')
def get(m,a,n=4):return int.from_bytes(m[a&0xffffff:(a&0xffffff)+n],'little')
def vector(m,a,values):
    for j,x in enumerate(values):put(m,a+j*4,bits(x))

def prepare(ram,c):
    m,s,f,base=previous.prepare(ram,dict(case_id=c.get('case_id',0),mode=c.get('mode',0)))
    cfg=v.Input(dict(base)|dict(entry=c['entry'],probe=True));s.pc=c['entry'];s.pr=0x8cfed000;s.r[15]=STACK
    f.fpscr=(c.get('mode',0)<<19)|c.get('fpscr',0x40000)|c.get('rm',0)
    view=c.get('view',0x8c000000);ptr=lambda a:view|(a&0xffffff)
    for a in (A,B,C,QUERY,OUT):m[a&0xffffff:(a&0xffffff)+0x200]=bytes([0x55])*0x200
    vector(m,UP,c.get('up',(0,1,0)))
    entry=c['entry']
    if entry in (0x8c055dac,0x8c638eac,0x8c639b50):
        vector(m,A,c.get('left',(1,0,0)));vector(m,B,c.get('right',(0,1,0)));vector(m,C,c.get('sign',(0,0,1)))
        s.r[4],s.r[5]=ptr(A),ptr(B)
        if entry==0x8c055dac:s.r[6]=ptr(C) if c.get('signed') else 0
        if entry==0x8c639b50:s.r[6]=ptr(OUT)
        for field,address in (('left_words',A),('right_words',B)):
            for j,word in enumerate(c.get(field,())):put(m,address+j*4,word)
    elif entry==0x8c63a970:s.r[4]=c.get('phase',0)&0xffffffff
    elif entry==0x8c075416:
        vector(m,B+4,c.get('velocity',(3,4,0)));put(m,C+0xc4,bits(c.get('limit',2)))
        vector(m,QUERY,(c.get('radius',1),*c.get('point',(0,0,0)),*c.get('motion',(-2,0,0)),*c.get('normal',(-1,0,0))))
        contacts=c.get('contacts',[dict()]);put(m,QUERY+0x28,len(contacts),2)
        for j,record in enumerate(contacts):
            a=CONTACTS+j*0x58;m[a&0xffffff:(a&0xffffff)+0x58]=bytes([0x66])*0x58
            vector(m,a+0x14,record.get('normal',(1,0,0)))
            vector(m,a+0x2c,record.get('point',(.5,0,0)))
            for field,off in (('normal_words',0x14),('point_words',0x2c)):
                for k,word in enumerate(record.get(field,())):put(m,a+off+k*4,word)
        # Fifth caller argument, not a fabricated callee frame.
        put(m,STACK,c.get('flags',0))
        s.r[4],s.r[5],s.r[6],s.r[7]=ptr(A),ptr(B),ptr(C),ptr(QUERY)
    else:raise AssertionError(hex(entry))
    if c.get('bad')=='stack':s.r[15]=0x8b000000
    if c.get('bad')=='first':s.r[4]=0x8b000000
    if c.get('bad')=='second':s.r[5]=0x8b000000
    if c.get('bad')=='output':s.r[6]=0x8b000000
    if c.get('bad')=='query':s.r[7]=0x8b000000
    return m,s,f,cfg
