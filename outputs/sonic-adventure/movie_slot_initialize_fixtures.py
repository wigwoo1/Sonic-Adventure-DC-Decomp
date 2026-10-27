"""Declared initial RAM records for original driver getters; no helper stubs."""
DRIVER=0x8cba0000
SCRATCH=0x8cd60000

def read(m,a,n=4):return int.from_bytes(m[a&0xffffff:(a&0xffffff)+n],'little')
def write(m,a,value,n=4):m[a&0xffffff:(a&0xffffff)+n]=(value&((1<<(n*8))-1)).to_bytes(n,'little')
def info(m,index):
 offset=int.from_bytes(m[0x65f3ca:0x65f3cc],'little',signed=True)
 return DRIVER+offset+index*120

def configure(m,c):
 write(m,read(m,0x8c65f3d8),DRIVER)
 for index in range(24):
  at=info(m,index);m[at&0xffffff:(at&0xffffff)+120]=bytes(120)
  record=c.get('slot_records',{}).get(str(index),c.get('slot_default_record',{}))
  flags=record.get('flags',0);write(m,at,flags)
  # Bit-locator scans from bit31 down. Each preceding present bit owns 4B.
  if flags&4:
   preceding=(flags&0xfffffff8).bit_count();offset=4+preceding*4
   data=bytes(record.get('metadata',[0,5,0x10,0]))
   assert len(data)==4;m[(at+offset)&0xffffff:((at+offset)&0xffffff)+4]=data
  write(m,at+0x12,record.get('status12',0),1)
  write(m,at+0x13,record.get('status13',0),1)
 base=read(m,0x8c109de8)
 m[base&0xffffff:(base&0xffffff)+0x1900]=bytes([c.get('slot_fill',0xa5)])*0x1900
 if c['entry']==0x8c108808:
  for j,index in enumerate((1,7,13,19)):write(m,base+16+j*1596+0x630,index)
 m[SCRATCH&0xffffff:(SCRATCH&0xffffff)+0x640]=bytes([0x5a])*0x640
 write(m,SCRATCH+0x630,c.get('slot_index',1))

def table_value(m,index):
 mapping=read(m,0x8c6029f8);peer=read(m,mapping+index,1)
 source=info(m,peer)+0x12+(index in (2,8,14,20))
 row=read(m,source,1);col=read(m,info(m,index)+0x12,1)
 return read(m,read(m,0x8c602a00)+row*4+col,1)

def predicate(c,index):
 record=c.get('slot_records',{}).get(str(index),c.get('slot_default_record',{}))
 b=record.get('metadata',[0,5,0x10,0]);flags=record.get('flags',0)
 return int(flags==14 and b[0]+1==1 and (b[1]+1)*32==192 and b[2]>>4==1 and b[3]>>6==0 and (b[3]>>5)&1==0)
