"""Explicit font/header/string inputs; no supplied game-callee outputs or frames."""
import struct
from fractions import Fraction
import write_watch_collision_text as fast
v=fast.old
GLYPH,TEXT,STRLEN=0x8c6407b0,0x8c640922,0x8c10d3f0
CURSOR,STRING,STATE_B=0x8cca0000,0x8cc80000,0x8cc31000
FONT_COLOUR,FONT_SIZE,CURSOR_SLOT=0x8c89055c,0x8c890560,0x8c89059c
def bits(x):return struct.unpack('<I',struct.pack('<f',x))[0]
def get(m,a,n=4):return int.from_bytes(m[a&0xffffff:(a&0xffffff)+n],'little')
def put(m,a,x,n=4):m[a&0xffffff:(a&0xffffff)+n]=(x&((1<<(n*8))-1)).to_bytes(n,'little')
def signed(n):return (n&65535)-65536 if n&32768 else n&65535
def input_bytes(c):return b'A'*c['length'] if 'length' in c else bytes(c.get('string',(65,66,67)))
def prepare(ram,c):
 entry=c.get('entry',GLYPH);cfg=v.Input(dict(entry=entry,mode=c.get('mode',0),probe=True,sr=0x400000a0|(c.get('case_id',0)&1),mach=0x89abcdef,macl=0x76543210,gbr=0x8ce01234,vertex_qacr=(12,12) if entry==GLYPH and c.get('device',True) else (0,0)))
 m,s,f=v.prepare(ram,cfg)
 for j in range(15):s.r[j]=(0x12345678^((j+1)*0x01030507))&0xffffffff
 alias=c.get('alias',0x8c000000);ptr=lambda a:alias|(a&0xffffff);cursor=ptr(CURSOR+c.get('offset',0));size=Fraction(c.get('size',8))
 s.r[15]=0x8cf00000;s.pc=entry;s.pr=0x8cfed000;s.sr=cfg['sr'];s.r[4]=c.get('x',10)&0xffffffff;s.r[5]=c.get('y',20)&0xffffffff;s.r[6]=c.get('character',65)&0xffffffff
 f.fpscr=(c.get('mode',0)<<19)|c.get('fpscr',0x40000)|c.get('rm',0);f.fpul=0xc0ffee42;cfg['fpul']=f.fpul
 for bank in range(2):
  for j in range(16):f.banks[bank][j]=bits(.5+(bank*16+j)/16)
 put(m,FONT_COLOUR,c.get('colour',0x80a0c0e0));put(m,FONT_SIZE,c.get('size_word',bits(size)));put(m,FONT_SIZE+4,0x3f000000)
 put(m,CURSOR_SLOT,(0xe0000000|(cursor&0x3ffffff)) if c.get('device',True) else cursor)
 put(m,0x8c88fcd4,ptr(STATE_B))
 for i,x in enumerate((0x31415926,0x27182818,cursor,0xa5a5a5a5,0x01020304)):put(m,STATE_B+4*i,x)
 for a,x in zip((0x8c89058c,0x8c890590,0x8c890594,0x8c890598),(0x12345678,0x87654321,0x89abcdef,0x13579bdf)):put(m,a,x)
 m[CURSOR&0xffffff:(CURSOR&0xffffff)+0x2000]=bytes([0x55])*0x2000
 data=input_bytes(c);raw=data+b'\0';m[STRING&0xffffff:(STRING&0xffffff)+len(raw)]=raw
 if entry==TEXT:
  s.r[4]=((c.get('x',2)&65535)<<16)|(c.get('y',3)&65535);s.r[5]=ptr(STRING)
 if c.get('bad')=='stack':s.r[15]=0x8c000010
 if c.get('bad')=='cursor':put(m,CURSOR_SLOT,0x8b000000)
 if c.get('bad')=='edge_cursor':put(m,CURSOR_SLOT,0x8cffffc0)
 if c.get('bad')=='string':s.r[5]=0x8b000000
 if c.get('bad')=='edge_string':
  s.r[5]=0x8cfffffc;m[-4:]=b'ABCD'
 if c.get('bad')=='state':put(m,0x8c88fcd4,0x8cfffffc)
 return m,s,f,cfg
