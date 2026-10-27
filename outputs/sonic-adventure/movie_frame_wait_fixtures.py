"""Declared standalone fixtures, never guest startup checkpoints or mock callees."""
import movie_frame_wait_reference as v
CURRENT,RECORD,LIST,RESOURCE=0x8ce80000,0x8cd80000,0x8cd81000,0x8cd82000
def put(m,address,value):
 at=address&0xffffff;m[at:at+4]=(value&0xffffffff).to_bytes(4,'little')
def half(m,address,value):
 at=address&0xffffff;m[at:at+2]=(value&0xffff).to_bytes(2,'little')
def global_put(m,literal,value):put(m,v.load(m,literal),value)
def prepare(ram,c):
 requested=dict(c);m,s,f=v.prepare(ram,c);c.update(requested);s.sr=c['sr'];entry=c['entry']
 if entry==v.DISPLAY_SYNC:
  global_put(m,0x8c64348c,c['flag']);global_put(m,0x8c643490,c['bank']);global_put(m,0x8c6434c0,c['index'])
  assert v.load(m,0x8c64348c)==v.load(m,0x8c656588)
  global_put(m,0x8c65658c,CURRENT)
  for group in range(3):
   for lane in range(5):put(m,v.load(m,0x8c656584)+20*group+4*lane,0x8c700100+group*0x20000+lane*0x2000+c['pattern'])
  for lane,value in enumerate((0,1,7,31,0xffffffff)):put(m,v.load(m,0x8c656598)+4*lane,value)
  for literal in (0x8c643498,0x8c64349c,0x8c6434a0,0x8c6434a4,0x8c6434a8):
   for bank in (0,1):put(m,v.load(m,literal)+bank*32,(literal^c['pattern']^bank*0x11223344)&0xffffffff)
  global_put(m,0x8c6434b4,0x13579bdf);global_put(m,0x8c6434b8,c['pattern']);global_put(m,0x8c6434bc,0x89abcdef)
 elif entry==v.FRAME_WAIT:global_put(m,0x8c051884,c['counter'])
 elif entry==v.BATCH:
  global_put(m,0x8c055d08,c['count']);global_put(m,0x8c055d0c,RECORD)
  m[RECORD&0xffffff:(RECORD&0xffffff)+0x100]=bytes([0x5a])*0x100
  put(m,RECORD+0x28,LIST if c.get('release') else 0);put(m,RECORD+0x2c,0);put(m,LIST,RESOURCE);put(m,LIST+4,0)
  half(m,RECORD+10,0xfedc);half(m,RECORD+12,0x89ab);put(m,RECORD+0x20,0x8cd83000);put(m,RECORD+0x24,0x12345678)
 elif entry in (v.SUBMIT,v.RESOURCE_DISPATCH,v.RELEASE):
  s.r[4]=RECORD;put(m,RECORD,LIST);put(m,RECORD+4,c.get('count',1));s.r[5]=c.get('index',0)
  for i in range(3):
   put(m,LIST+12*i+4,c.get('flags',0));put(m,LIST+12*i+8,RESOURCE+i*0x80);put(m,RESOURCE+i*0x80+0x10,1)
 elif entry==v.RESOURCE_RELEASE:
  s.r[4]=RESOURCE;put(m,RESOURCE,c.get('identifier',0xffffffff));put(m,RESOURCE+0x10,c['flags']);put(m,RESOURCE+0x18,0)
 else:
  s.r[4]=RECORD;s.r[5]=0x13579bdf;s.r[6]=c.get('payload',0x89abcdef);s.r[7]=c.get('flags',0x40800000);put(m,s.r[15],0x2468ace0)
 return m,s,f

def display_expected(m,before,c):
 wanted=bytearray(m);sp=before.r[15];put(wanted,sp-4,before.r[14]);put(wanted,sp-8,before.r[13]);put(wanted,sp-12,before.pr);writes=[]
 if c['flag']==1:
  bank=c['bank']&1;put(wanted,sp-16,bank);writes=[(8,1),(8,0)]
  for half_literal,pointer_literal in zip(range(0x8c643480,0x8c64348a,2),(0x8c643498,0x8c64349c,0x8c6434a0,0x8c6434a4,0x8c6434a8)):
   offset=int.from_bytes(v.sample(m,half_literal,2),'little',signed=True)&0xffffffff;writes.append((offset,v.load(m,v.load(m,pointer_literal)+bank*32)))
  offset=int.from_bytes(v.sample(m,0x8c64348a,2),'little',signed=True)&0xffffffff;writes.append((offset,v.load(m,0x8c6434ac)))
  global_put(wanted,0x8c6434b4,0);global_put(wanted,0x8c6434bc,v.load(m,v.load(m,0x8c6434b8)))
 index=c['index'];base=v.load(m,0x8c656584)+(0 if c['flag']==1 else index*20);global_put(wanted,0x8c656580,base)
 values=[v.load(wanted,base+i*4) for i in range(5)]
 for i,value in enumerate(values):put(wanted,CURRENT+i*4,value);put(wanted,v.load(m,0x8c656590)+i*4,value)
 global_put(wanted,0x8c656594,index&1)
 for i in range(5):put(wanted,CURRENT+4*i,(v.load(wanted,CURRENT+4*i)+4*v.load(wanted,v.load(m,0x8c656598)+4*i))&0xffffffff)
 return wanted,writes
