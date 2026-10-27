"""Declared coherent countdown/audio/input fixtures; actual game callees only."""
import movie_countdown_reference as v
ROOT_CONTEXT,WORD_BASE=0x8cd60000,0x8cd70000
def put(m,a,x):
 at=a&0xffffff;m[at:at+4]=(x&0xffffffff).to_bytes(4,'little')
def half(m,a,x):
 at=a&0xffffff;m[at:at+2]=(x&0xffff).to_bytes(2,'little')
def literal_put(m,a,x):put(m,v.load(m,a),x)
def sx16(x):return (x&0x7fff)-(x&0x8000)
def offset(m,a):return int.from_bytes(v.sample(m,a,2),'little',signed=True)
def packed(a,b):return ((sx16(a)<<8)|sx16(b))&0xffffffff
def prepare(ram,c):
 wanted=dict(c);m,s,f=v.prepare(ram,c);c.update(wanted);s.sr=c['sr'];entry=c['entry']
 if entry in (v.CD_VARIANT,v.CD_TICK):
  half(m,v.load(m,0x8c04f81c),c.get('variant_a',0));half(m,v.load(m,0x8c04f820),c.get('variant_b',0))
 if entry in (v.CD_TICK,v.CD_INPUT):
  literal_put(m,0x8c051884,c.get('countdown_value',2));literal_put(m,0x8c0518dc,c.get('game_mode',0));literal_put(m,0x8c602f00,c.get('input_enabled',0))
  literal_put(m,0x8c643ff4,0);literal_put(m,0x8c09cec0,0);literal_put(m,0x8c601818,0)
 if entry==v.CD_AUDIO:
  literal_put(m,0x8c643ff4,ROOT_CONTEXT if c.get('root',True) else 0);literal_put(m,0x8c643ff8,c.get('audio_mode',1));literal_put(m,0x8c10d440,WORD_BASE)
  put(m,ROOT_CONTEXT+offset(m,0x8c643ff0),c.get('pending',0));put(m,ROOT_CONTEXT+offset(m,0x8c643ff2),c.get('word_enabled',0))
  # Actual pump rejects this mode and records its original error; no result is
  # supplied by the fixture, and the caller really ignores that return value.
  put(m,ROOT_CONTEXT+offset(m,0x8c65a43a)+8,1)
  put(m,WORD_BASE+offset(m,0x8c10d43c),c.get('word1',0));put(m,WORD_BASE+offset(m,0x8c10d458),c.get('word2',0))
 if entry in (v.CD_ON,v.CD_OFF):
  index=c['index'];s.r[4]=index;record=(v.load(m,0x8c647d10)+(index<<7))&0xffffffff;put(m,record+4,c['record_status'])
 if entry==v.CD_FPU_WRAPPER:
  s.r[4]=c.get('callback',v.CD_NOP);s.r[5]=c['payload'];f.fpscr|=0x1f07f
  for bank in range(2):
   for reg in range(16):f.banks[bank][reg]=(c['payload']^((bank+1)*0x11111111)^((reg+1)*0x01020304))&0xffffffff
 return m,s,f

def tick_expected(m,before,c):
 wanted=bytearray(m);sp=before.r[15];remaining=(c['countdown_value']-1)&0xffffffff;literal_put(wanted,0x8c051884,remaining);put(wanted,sp-4,before.pr)
 if not remaining:
  value=packed(c.get('variant_a',0),c.get('variant_b',0));half(wanted,sp-8,value&255);half(wanted,sp-12,(value&0xff00)>>8)
  if c.get('game_mode',0) in (4,5,9):
   table=v.load(m,0x8c0518e0);target=v.load(m,table+((value>>8)&255)*32+(value&255)*4)
   if target!=v.CD_NOP:return wanted,-2,target
 if c.get('input_enabled'):
  for j,reg in enumerate(range(14,7,-1),1):put(wanted,sp-12-j*4,before.r[reg])
  put(wanted,sp-44,0x8c051876)
  return wanted,-2,0x8c668e60
 # Original input prologue followed by real service tail with null interface.
 for j,reg in enumerate(range(14,7,-1),1):put(wanted,sp-12-j*4,before.r[reg])
 put(wanted,sp-44,0x8c051876);put(wanted,sp-8,before.r[14]);put(wanted,sp-4,before.r[14])
 return wanted,0,before.pr

def audio_expected(m,before,c):
 wanted=bytearray(m);sp=before.r[15];put(wanted,sp-4,before.pr);calls=[]
 if c.get('root',True) and c.get('audio_mode',1)==1:
  if c.get('pending',0)>0 and c['pending']<0x80000000:
   calls.append(v.CD_PUMP)
   for j,reg in enumerate(range(14,7,-1),1):put(wanted,sp-4-j*4,before.r[reg])
   put(wanted,sp-36,0x8c643fb8)
   literal_put(wanted,0x8c65acb8,v.load(m,0x8c65a45c));literal_put(wanted,0x8c65acbc,v.load(m,0x8c65a450));calls.append(0x8c65acac)
  if c.get('word_enabled',0):
   calls.extend((v.CD_WORD1,v.CD_WORD2));literal_put(wanted,0x8c644000,c.get('word1',0));literal_put(wanted,0x8c644008,c.get('word2',0))
   if c.get('word2',0)<c.get('word1',0):literal_put(wanted,0x8c65acb8,v.load(m,0x8c644014));literal_put(wanted,0x8c65acbc,v.load(m,0x8c644010));calls.append(0x8c65acac)
 return wanted,calls

def channel_expected(m,before,c):
 wanted=bytearray(m);r=list(before.r);sp=r[15];index=c['index'];record=(v.load(m,0x8c647d10)+(index<<7))&0xffffffff
 for off,value in ((4,before.r[14]),(8,before.r[13]),(12,before.pr)):put(wanted,sp-off,value)
 r[3]=v.load(m,0x8c647d10);r[4]=record;sr=(before.sr&~1)|int(c['record_status']==0)
 if c['record_status']:r[0]=0xffffffff
 else:
  sr=(before.sr&~1)|((index>>25)&1);r[0]=0;r[1]=1;r[2]=0;r[5]=index;r[6]=r[7]=0
  for off in (16,20,24):put(wanted,sp-off,0)
  put(wanted,record+8,1 if c['entry']==v.CD_ON else 0x12)
  for off,value in ((0x14,index),(0x18,0),(0x1c,0),(0x20,0),(0x24,0),(0x28,0),(4,1)):put(wanted,record+off,value)
 return wanted,r,sr
