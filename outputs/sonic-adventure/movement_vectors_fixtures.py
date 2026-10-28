"""Authored movement records/vectors and matrix INPUT state; no callee outputs/frames."""
import struct
import collision_contact_fixtures as previous
import write_watch_movement_vectors as fast
v=fast.old;P=v.P
A,B,C,OUT,MATRIX,STACK=0x8cb00000,0x8cb10000,0x8cb20000,0x8cb30000,0x8cb40000,0x8cd00000
REFERENCE=0x8c754bdc


def bits(x):return struct.unpack('<I',struct.pack('<f',x))[0]
def number(word):return struct.unpack('<f',int(word).to_bytes(4,'little'))[0]
def put(m,a,x,n=4):m[a&0xffffff:(a&0xffffff)+n]=(int(x)&((1<<(8*n))-1)).to_bytes(n,'little')
def get(m,a,n=4):return int.from_bytes(m[a&0xffffff:(a&0xffffff)+n],'little')
def vector(m,a,values):
    for j,x in enumerate(values):put(m,a+4*j,bits(x))


def prepare(ram,c):
    m,s,f,base=previous.prepare(ram,dict(case_id=c.get('case_id',0),mode=c.get('mode',0)))
    cfg=v.Input(dict(base)|dict(entry=c['entry'],probe=True))
    s.pc=c['entry'];s.pr=0x8cfed000;s.r[15]=0x8cf00000
    view=c.get('view',0x8c000000);ptr=lambda address:view|(address&0xffffff)
    for address in (A,B,C,OUT,MATRIX):m[address&0xffffff:(address&0xffffff)+0x200]=bytes([0x55])*0x200
    mat=[(2,3,4,1)[j//5] if j%5==0 else 0 for j in range(16)] if c.get('matrix') else [int(j%5==0) for j in range(16)]
    for j,x in enumerate(mat):put(m,MATRIX+j*4,bits(x))
    f.banks[((f.fpscr>>21)&1)^1][:]=list(map(bits,mat))
    put(m,0x8c88f698,c.get('limit',64));put(m,0x8c88f69c,c.get('depth',1));put(m,0x8c88f5f8,ptr(STACK))
    put(m,0x8c890070,c.get('normalize',0))
    vector(m,REFERENCE,c.get('reference',(0,1,0)))
    entry=c['entry']
    if entry==0x8c079000:
        vector(m,A,c.get('orientation',(1,0,0)));vector(m,B,c.get('vector',(1,2,3)))
        s.r[4],s.r[5],s.r[6]=ptr(A),ptr(B),c.get('roll',0)&0xffffffff
    elif entry==0x8c078eb0:
        for offset,x in zip((0x14,0x18,0x1c),c.get('angles',(0,0,0))):put(m,A+offset,x)
        vector(m,B,c.get('vector',(1,2,3)));s.r[4],s.r[5]=ptr(A),ptr(B)
    elif entry==0x8c639090:
        vector(m,A,c.get('left',(1,2,3)));vector(m,B,c.get('right',(4,5,6)))
        s.r[4],s.r[5]=ptr(A),ptr(B)
    elif entry==0x8c638f28:
        vector(m,B,c.get('vector',(1,2,3)))
        s.r[4],s.r[5],s.r[6]=ptr(MATRIX) if c.get('explicit') else 0,ptr(B),ptr(B if c.get('in_place') else OUT)
    elif entry==0x8c074cf0:
        vector(m,A+4,c.get('velocity',(0,0,0)))
        put(m,B+0x60,c.get('flags',0));put(m,B+0x10,c.get('status_flags',0),2)
        for offset,name,default in ((0xa0,'acceleration',1),(0x3c,'height',0),(0x38,'speed',1),(0xc4,'threshold',1),(0xd0,'cap',2)):
            put(m,B+offset,bits(c.get(name,default)))
        vector(m,C+0x1c,c.get('normal',(0,0,1)));vector(m,OUT,c.get('output',(9,8,7)))
        s.r[4],s.r[5],s.r[6],s.r[7]=ptr(A),ptr(B),ptr(C),ptr(OUT)
    elif entry!=0x8c057b40:raise AssertionError(hex(entry))
    if c.get('bad')=='stack':s.r[15]=0x8b000000
    if c.get('bad')=='first':s.r[4]=0x8b000000
    if c.get('bad')=='second':s.r[5]=0x8b000000
    return m,s,f,cfg
