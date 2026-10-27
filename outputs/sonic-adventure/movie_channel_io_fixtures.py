"""Explicit coherent I/O inputs; original task constructors own task state."""
import write_watch_channel_io as fast
v=fast.old
INPUT_OBJECT=0x8cc00000;GLOBAL_OBJECT=0x8cd68000
def put(m,a,value,n=4):m[a&0xffffff:(a&0xffffff)+n]=(value&((1<<(8*n))-1)).to_bytes(n,'little')
def layout(m,index):
 mapped=v.load(m,v.load(m,0x8c660e3c)+index*4)
 info_offset=int.from_bytes(v.sample(m,0x8c65f3ca,2),'little',signed=True)
 return dict(index=index,mapped=mapped,map_base=v.load(m,0x8c660e3c),header=(v.load(m,0x8c6468b0)+mapped*24)&0xffffffff,info=(INPUT_OBJECT+info_offset+mapped*120)&0xffffffff,status=(INPUT_OBJECT+0x28+mapped*44)&0xffffffff,input_pointer=v.load(m,0x8c65f3d8))
def configure(m,index,active=0,busy=0,flags=2):
 d=layout(m,index);assert all((d[k]&0x1f000000)==0x0c000000 for k in ('header','info','status'))
 put(m,d['input_pointer'],INPUT_OBJECT)
 for a,n in ((d['header'],24),(d['info'],120),(d['status'],44)):m[a&0xffffff:(a&0xffffff)+n]=bytes(n)
 put(m,d['header'],active,1);put(m,d['info'],flags);put(m,d['status']+0xc,busy,1)
 return d
def call(m,s,f,c,entry,args=None,stack_args=()):
 c['entry']=entry;s.pc=entry;s.pr=0x8cfed000
 if args is not None:
  for j,value in enumerate(args):s.r[4+j]=value
 for j,value in enumerate(stack_args):put(m,s.r[15]+j*4,value)
 if entry==v.AI_SCHEDULE:s.r[4]=v.load(m,v.load(m,0x8c660880))
 r,path,m,s,f=fast.compare(m,s,f,c)
 c.update(other=r['integer_other'],mach=r['final_mach'],macl=r['final_macl'],gbr=r['final_gbr'],vbr=r['vbr'])
 return r,path,m,s,f
