"""Declared valid RAM fixtures and independent record registration expectations."""
OWNER=0x8ca08000
PAYLOAD=0x8ca50000
CONTEXT=0x8cd60000
BUFFER=0x8cd80000
def read(m,a,n=4):return int.from_bytes(m[a&0xffffff:(a&0xffffff)+n],'little')
def signed16(m,a):return int.from_bytes(m[a&0xffffff:(a&0xffffff)+2],'little',signed=True)
def write(m,a,value,n=4):m[a&0xffffff:(a&0xffffff)+n]=(value&((1<<(8*n))-1)).to_bytes(n,'little')
def sample(m,a,n):return bytes(m[a&0xffffff:(a&0xffffff)+n])
def fill(m,a,value,n):m[a&0xffffff:(a&0xffffff)+n]=bytes([value])*n

def configure(m,c):
 write(m,0x8c161ca8,c.get('shift',0))
 selector=read(m,0x8c602ca0);write(m,selector,c.get('selector',7))
 fill(m,CONTEXT-128,0x5a,512)
 if c['entry']==0x8c660842:
  fill(m,OWNER,0x55,0x2694)
  write(m,OWNER+0x204,0xabcdef01);write(m,OWNER+0x208,1);write(m,OWNER+0x20c,int(c.get('reject',False)))
  for j in range(32):write(m,OWNER+0x210+j*0x124+12,int(j!=c.get('free',0)))
  write(m,read(m,0x8c660880),OWNER)
  m[PAYLOAD&0xffffff:(PAYLOAD&0xffffff)+256]=bytes((j*37+11)&255 for j in range(256))

def shifted_one(value):
 value&=0xffffffff
 if not value&0x80000000:return (1<<(value&31))&0xffffffff
 count=((-value)&0xffffffff)&31
 return 0 if count==0 else 1>>count

def channels(m,mask,callback_fields=False):
 mask&=255;pool=read(m,0x8c668c44);length=signed16(m,0x8c647846)
 assert read(m,read(m,0x8c660880))==pool
 assert read(m,read(m,0x8c668c54))==1
 checks=2
 cleared=read(m,0x8c647850);wanted=bytearray(signed16(m,0x8c647844))
 if callback_fields:
  assert read(m,0x8c647d10)==cleared
  for offset in range(0,len(wanted),signed16(m,0x8c647d0c)):
   wanted[offset+0x74:offset+0x78]=read(m,0x8c010f58).to_bytes(4,'little')
   wanted[offset+0x78:offset+0x7c]=read(m,0x8c010f60).to_bytes(4,'little')
 assert sample(m,cleared,len(wanted))==wanted;checks+=1
 for i in range(8):
  text=read(m,0x8c647854+i*4);assert sample(m,text,length)==bytes([0x30+i])*length;checks+=1
 for j,i in enumerate(i for i in range(8) if mask&(1<<i)):
  job=pool+0x210+j*0x124;data=read(m,0x8c647934+i*8);callback=read(m,0x8c647938+i*8)
  assert [read(m,job+k*4) for k in range(6)]==[data,length//4,0,1,0,0];checks+=1
  assert read(m,job+0x18)==(data+((length//4-1)*4))&0xffffffff and read(m,job+0x20)==callback;checks+=2
 ready=sum(read(m,pool+0x210+j*0x124+12)!=0 for j in range(32));assert ready==mask.bit_count();checks+=1
 return checks

def callbacks(m,argument,callback):
 base=read(m,0x8c647d10);stride=signed16(m,0x8c647d0c);size=signed16(m,0x8c647d0e)
 assert stride>0 and size>0 and size%stride==0
 for at in range(base,base+size,stride):
  assert read(m,at+0x74)==argument and read(m,at+0x78)==callback
 return 1+2*(size//stride)
