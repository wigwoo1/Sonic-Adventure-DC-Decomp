"""Authored geometry/render input state, never game-callee results or frames."""
import struct
from fractions import Fraction
import write_watch_collision_render as fast
v=fast.old
ENTRY=0x8c63d66c
OBJECT,POINTS,COLOURS,UV=0x8cc50000,0x8cc60000,0x8cc70000,0x8cc80000
STATE_A,STATE_B,CURSOR=0x8cc30000,0x8cc31000,0x8cca0000
HEADER=0x8c890104
def bits(x):return struct.unpack('<I',struct.pack('<f',x))[0]
def put(m,a,x,n=4):m[a&0xffffff:(a&0xffffff)+n]=(x&((1<<(n*8))-1)).to_bytes(n,'little')
def get(m,a,n=4):return int.from_bytes(m[a&0xffffff:(a&0xffffff)+n],'little')
def matrix(kind):
 return [Fraction(int(i%5==0)) for i in range(16)] if not kind else list(map(Fraction,(1,0,0,0,0,2,0,0,0,0,1,0,.5,-.25,0,1)))
def geometry(c):
 zs=c.get('zs',(2,2,2));return [(Fraction((i%5)-2,4),Fraction((i*i)%7-3,4),Fraction(zs[i%len(zs)])) for i in range(12)]
def prepare(ram,c):
 cfg=v.Input(dict(entry=ENTRY,mode=c.get('mode',0),probe=True,sr=0x400000a0|(c.get('case_id',0)&1),mach=0x89abcdef,macl=0x76543210,gbr=0x8ce01234))
 m,s,f=v.prepare(ram,cfg)
 for j in range(15):s.r[j]=(0x12345678^((j+1)*0x01030507))&0xffffffff
 alias=c.get('alias',0x8c000000);ptr=lambda a:alias|(a&0xffffff)
 s.r[4]=ptr(OBJECT);s.r[5]=c.get('count',1)&0xffffffff;s.r[6]=c.get('flags',0x20);s.r[15]=0x8cf00000;s.pr=0x8cfed000;s.pc=ENTRY;s.sr=cfg['sr']
 fpscr=(c.get('mode',0)<<19)|c.get('fpscr',0x40000)|c.get('rm',0);f.fpscr=fpscr;f.fpul=0xc0ffee42;cfg['fpul']=f.fpul
 for bank in range(2):
  for j in range(16):f.banks[bank][j]=bits(.5+(bank*16+j)/16)
 f.banks[((fpscr>>21)&1)^1][:]=list(map(bits,matrix(c.get('matrix',0))))
 cursor=ptr(CURSOR+c.get('offset',0));put(m,0x8c88fcb4,ptr(STATE_A));put(m,0x8c88fcd4,ptr(STATE_B));put(m,0x8c88fcd0,ptr(STATE_A+0x80));put(m,STATE_A+0x80,0x55667788);put(m,0x8c88f62c,c.get('env',0)|(0x4000 if c.get('swap') else 0))
 for i,x in enumerate((0x80840012,0x12345678,0x89abcdef,0x13579bdf)):put(m,STATE_A+0x90+i*4,x)
 for i,x in enumerate((cursor,0x31415926,cursor,0x27182818,cursor)):put(m,STATE_B+i*4,x)
 for i,x in enumerate((1,2,-2,Fraction(1,4),Fraction(1,2),1,100)):
  put(m,(0x8c88f5e4,0x8c88f618,0x8c88f61c,0x8c88f5f0,0x8c88f5f4,0x8c88f610,0x8c88f614)[i],bits(x))
 for a,x in ((OBJECT,ptr(POINTS)),(OBJECT+4,ptr(COLOURS)),(OBJECT+8,ptr(UV))):put(m,a,x)
 for i,p in enumerate(geometry(c)):
  for j,x in enumerate(p):put(m,POINTS+i*12+j*4,bits(x))
  put(m,COLOURS+i*4,(0x10203040,0xe0c0a080,0x80ff0010,0xff204080)[i%4])
  put(m,UV+i*4,(-30000+i*512)&65535,2);put(m,UV+i*4+2,(16000-i*768)&65535,2)
 m[CURSOR&0xffffff:(CURSOR&0xffffff)+0x1000]=bytes([0x55])*0x1000
 if c.get('special') is not None:put(m,POINTS+8,c['special'])
 if c.get('bad')=='object':s.r[4]=0x8cfffffc
 if c.get('bad')=='points':put(m,OBJECT,0x8cfffffc)
 if c.get('bad')=='stack':s.r[15]=0x8c000010
 return m,s,f,cfg
