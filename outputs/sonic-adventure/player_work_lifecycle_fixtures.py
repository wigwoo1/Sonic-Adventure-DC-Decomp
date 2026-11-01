"""Original heap initialization -> task pool -> parent task -> player constructor."""
import ctypes
import write_watch_player_work_allocation as fast
v=fast.old
P=v.P
INIT,AUXILIARY,RECORDS,FLAG=0x8c06c350,0x8c04b440,0x8c07d67c,0x8c09068e
HEAP_INIT,POOL_INIT,CREATE=0x8c098a10,0x8c09844a,0x8c09852e
RETURN,STACK=0x8cfed000,0x8cf00000

def put(m,a,value,n=4):
    m[a&0xffffff:(a&0xffffff)+n]=(value&((1<<(8*n))-1)).to_bytes(n,'little')

def get(m,a,n=4):
    return int.from_bytes(m[a&0xffffff:(a&0xffffff)+n],'little')

def fresh(ram,mode=0,rm=0):
    cfg=v.Input(dict(entry=INIT,case_id=0,mode=mode,probe=True,
                     player_work_initialize_arguments=[0,0,0,0]))
    m,s,f=v.prepare(ram,cfg)
    s.pc=HEAP_INIT;s.pr=RETURN;s.r[15]=STACK
    f.fpscr=0x40000|(mode<<19)|rm
    for bank in range(2):
        for reg in range(16):f.banks[bank][reg]=0x3f800000+bank*0x10000+reg*0x100
    return m,s,f,cfg

def clone(base):
    m,s,f,c=base
    return bytearray(m),type(s).from_buffer_copy(s),type(f).from_buffer_copy(f),v.Input(dict(c))

def invoke(m,s,f,cfg,entry,args=()):
    cfg['entry']=entry;s.pc=entry;s.pr=RETURN
    for j,value in enumerate(args):s.r[4+j]=value&0xffffffff
    r,path,m,s,f=fast.compare(m,s,f,cfg)
    cfg.update(other=r['integer_other'],mach=r['final_mach'],macl=r['final_macl'],
               gbr=r['final_gbr'],vbr=r['vbr'])
    return r,path,m,s,f
