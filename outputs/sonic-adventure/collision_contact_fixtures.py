"""Authored query, linked world, triangle, matrix and render input data only."""
import struct
from fractions import Fraction
import collision_render_fixtures as render
import write_watch_collision_contact as fast
v=fast.old
ENTRY=0x8c029b00;QUERY,NODE,LIST,FACES=0x8cc00000,0x8cc10000,0x8cc20000,0x8cc90000
WORLD,COUNT,CONTACTS=0x8c02d548,0x8c73e400,0x8c73e404
MATRIX=0x8c67c640;STACK=0x8cd00000;OBJECT_ID=0x12345678
def bits(x):return struct.unpack('<I',struct.pack('<f',x))[0]
def put(m,a,x,n=4):m[a&0xffffff:(a&0xffffff)+n]=(x&((1<<(8*n))-1)).to_bytes(n,'little')
def get(m,a,n=4):return int.from_bytes(m[a&0xffffff:(a&0xffffff)+n],'little')
def identity():return [bits(int(i%5==0)) for i in range(16)]
def triangle(c,index=0):
 base=c.get('vertices',((0,0,0),(0,0,2),(2,0,0)))
 offset=c.get('face_offsets',((0,0,0),))[index%len(c.get('face_offsets',((0,0,0),)))]
 return [tuple(Fraction(x)+Fraction(d) for x,d in zip(p,offset)) for p in base]
def prepare(ram,c):
 m,s,f,base_cfg=render.prepare(ram,dict(flags=0x20,mode=0,case_id=c.get('case_id',0)))
 cfg=v.Input(dict(base_cfg)|dict(entry=ENTRY,mode=c.get('mode',0),probe=True,sr=0x400000a0|(c.get('case_id',0)&1),mach=0x89abcdef,macl=0x76543210,gbr=0x8ce01234))
 for j in range(15):s.r[j]=(0x12345678^((j+1)*0x01030507))&0xffffffff
 alias=c.get('alias',0x8c000000);ptr=lambda a:alias|(a&0xffffff)
 s.r[4]=ptr(QUERY);s.r[5]=OBJECT_ID;s.r[15]=0x8cf00000;s.pr=0x8cfed000;s.pc=ENTRY;s.sr=cfg['sr'];f.fpscr=(c.get('mode',0)<<19)|c.get('fpscr',0x40000)|c.get('rm',0);f.fpul=0xc0ffee42;cfg['fpul']=f.fpul
 for bank in range(2):
  for j in range(16):f.banks[bank][j]=bits(.5+(bank*16+j)/16)
 f.banks[((f.fpscr>>21)&1)^1][:]=identity()
 m[QUERY&0xffffff:(QUERY&0xffffff)+0x100]=bytes([0x55])*0x100
 for i,x in enumerate((c.get('radius',1),*c.get('point',(.5,.25,.5)),*c.get('motion',(0,0,0)))):put(m,QUERY+4*i,bits(x))
 if c.get('radius_word') is not None:put(m,QUERY,c['radius_word'])
 m[CONTACTS&0xffffff:(CONTACTS&0xffffff)+0x9c*18]=bytes([0xa5])*(0x9c*18)
 put(m,COUNT,0x12345678);put(m,WORLD,0 if c.get('no_world') else ptr(NODE))
 n=c.get('face_count',1)
 for j in range(c.get('node_count',1)):
  node=NODE+j*0x100;put(m,node,ptr(node+0x100) if j<c.get('node_count',1)-1 else 0);put(m,node+8,OBJECT_ID if j==c.get('match_node',0) else OBJECT_ID+1);put(m,node+0x1c,ptr(LIST));put(m,node+0x24,n)
 for j in range(max(0,n)):
  face=FACES+j*0x100;put(m,LIST+j*4,ptr(face))
  for k in range(6):put(m,face+4*k,0x81000000+j*0x100+k)
  for k,p in enumerate(triangle(c,j)):
   for q,x in enumerate(p):put(m,face+0x18+k*12+4*q,bits(x))
  put(m,face+0x3c,c.get('face_flags',0))
 for j,x in enumerate(identity()):put(m,MATRIX+4*j,x)
 put(m,0x8c88f698,64);put(m,0x8c88f69c,1);put(m,0x8c88f5f8,STACK)
 for j in range(0,0x1000,4):put(m,STACK+j,identity()[(j//4)%16])
 put(m,0x8c752bdc,c.get('debug',0x80),1)
 # Actual debug renderer consumes authored geometry/header/font input state.
 for a,x in ((0x8c196034,render.POINTS),(0x8c196038,render.COLOURS),(0x8c19603c,render.UV)):put(m,a,x)
 for j in range(9):
  for k,x in enumerate((Fraction(j%3,4),Fraction(j//3,4),2)):put(m,render.POINTS+j*12+4*k,bits(x))
  put(m,render.COLOURS+j*4,0xffffffff)
 for a,x in ((0x8c89055c,0xffffffff),(0x8c890560,bits(8)),(0x8c89058c,0x12345678),(0x8c890590,0x87654321),(0x8c890594,0x89abcdef),(0x8c890598,0x13579bdf)):put(m,a,x)
 if c.get('bad')=='query':s.r[4]=0x8b000000
 if c.get('bad')=='stack':s.r[15]=0x8c000010
 if c.get('bad')=='face':put(m,LIST,0x8cfffffc)
 if c.get('bad')=='node':put(m,WORLD,0x8cfffffc)
 return m,s,f,cfg
