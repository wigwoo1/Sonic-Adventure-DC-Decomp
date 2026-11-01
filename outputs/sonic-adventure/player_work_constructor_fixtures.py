"""Declared coherent heap/list inputs; all task/work allocations execute originals."""
import write_watch_player_work_dependencies as fast
v=fast.old
P=v.P
INIT,AUXILIARY,RECORDS,FLAG=0x8c06c350,0x8c04b440,0x8c07d67c,0x8c09068e
CREATE=0x8c09852e
HEAP,POOL=0x8ce00000,0x8ce20000
RETURN,STACK=0x8cfed000,0x8cf00000

def put(m,a,value,n=4):
    m[a&0xffffff:(a&0xffffff)+n]=(value&((1<<(8*n))-1)).to_bytes(n,'little')

def get(m,a,n=4):
    return int.from_bytes(m[a&0xffffff:(a&0xffffff)+n],'little')

def prepare(ram,c):
    cfg=v.Input(dict(entry=INIT,case_id=c.get('case_id',0),mode=c.get('mode',0),
                     probe=True,player_work_initialize_arguments=[0,0,0,0]))
    m,s,f=v.prepare(ram,cfg)
    s.pc=CREATE;s.pr=RETURN;s.r[15]=STACK
    s.r[4],s.r[5],s.r[6],s.r[7]=3,2,0,0
    cfg['entry']=CREATE
    f.fpscr=0x40000|(c.get('mode',0)<<19)|c.get('rm',0)
    for bank in range(2):
        for reg in range(16):f.banks[bank][reg]=0x3f800000+bank*0x10000+reg*0x100
    size=c.get('heap_size',0x10000)
    m[HEAP&0xffffff:(HEAP&0xffffff)+size+16]=bytes([0xa5])*(size+16)
    free=get(m,0x8c098b60)
    put(m,free,HEAP if size else 0)
    if size:
        for j,value in enumerate((HEAP+size,0,HEAP+size,0)):put(m,HEAP+4*j,value)
        for j,value in enumerate((0,HEAP,0,HEAP)):put(m,HEAP+size+4*j,value)
    levels=get(m,0x8c0986c8)
    for j in range(8):put(m,levels+4*j,0)
    for j in range(8):
        task=POOL+0x30*j
        m[task&0xffffff:(task&0xffffff)+0x30]=bytes(0x30)
        put(m,task,task+0x30 if j<7 else 0)
    put(m,get(m,0x8c0986c0),POOL)
    m[0x799920:0x799b20]=bytes(0x200)
    for flag in c.get('flags',()):put(m,0x8c799920+flag,c.get('flag_byte',1),1)
    return m,s,f,cfg

def invoke(m,s,f,cfg,entry,args):
    cfg['entry']=entry;s.pc=entry;s.pr=RETURN
    for j,value in enumerate(args):s.r[4+j]=value&0xffffffff
    r,path,m,s,f=fast.compare(m,s,f,cfg)
    cfg.update(other=r['integer_other'],mach=r['final_mach'],macl=r['final_macl'],
               gbr=r['final_gbr'],vbr=r['vbr'])
    return r,path,m,s,f
