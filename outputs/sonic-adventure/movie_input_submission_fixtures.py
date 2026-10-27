"""Declared SDK/buffer/register inputs; original routines move request tokens."""
import types
import movie_input_pump_fixtures as previous
import write_watch_input_submission as fast
v=fast.old
put=previous.put;CTX=previous.CTX;FRAME=previous.FRAME;LOCK=previous.LOCK
CONFIGS=previous.CONFIGS
SECOND_FRAME=FRAME+0x10000
MMIO=0xa05f6c00
SUBMIT,PREPARE,FEATURES,FOUR,THREE,TWO=(0x8c65efda,0x8c65eeda,0x8c66034e,0x8c6603f4,0x8c660438,0x8c66047c)
_setup=types.FunctionType(previous._setup.__code__,dict(previous._setup.__globals__,v=v,fast=fast),'setup',previous._setup.__defaults__)

def setup(ram,entry=SUBMIT,args=(0,0,0,0),mode=0,sr=0x400000a1):
    c,m,s,f=_setup(ram,entry,list(args),mode,sr)
    c['input_submission_arguments']=list(args)
    return c,m,s,f

def half(m,a):return int.from_bytes(v.sample(m,a,2),'little',signed=True)

def configure(c,m,status=1,busy=0,lock=0,bank=0,buffer_bank=0,rate=0,last_rate=0,padding=0,order=0):
    previous.configure(m,status=status,lock=lock)
    put(m,CTX+0x10f4,MMIO)
    for off,value in ((0x10a8,FRAME),(0x10ac,SECOND_FRAME),(0x10b8,bank),
                      (0x10b4,buffer_bank),(0xfcc,FRAME if bank==0 else SECOND_FRAME),
                      (0xfd8,0),(0xfdc,0x20),(0xfac,rate),(0xfb0,last_rate),
                      (0x10c8,padding),(0x10e4,padding),(0x10c4,order)):
        put(m,CTX+off,value)
    m[SECOND_FRAME&0xffffff:(SECOND_FRAME&0xffffff)+0x10000]=bytes(0x10000)
    physical=MMIO&0x1fffffff
    c['startup_overrides']={physical+0x18:busy,physical+4:0x13579bdf,physical+0x14:0x2468ace0,physical+0x80:0x55aa55aa}
    return dict(context=CTX,mmio=MMIO,lock=LOCK)

def record(m,unit,present=1,command=10,state=0,token=7,old_token=0x89abcdef):
    rec=CTX+0x28+unit*44
    put(m,CTX+0xf90+unit,present,1)
    put(m,rec+4,state,1);put(m,rec+6,command,1);put(m,rec+7,0xa5,1)
    put(m,rec+0x18,token);put(m,rec+0x1c,old_token);put(m,rec+0x20,0x12345678)
    return rec
