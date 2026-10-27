"""Declared coherent RAM inputs for actual interface callbacks and setup."""
import write_watch_input_interface as fast
v=fast.old
DATA=0x8cd80000
READ_BE,READ_LE,WRITE_BE,WRITE_LE=(0x8c66080c,0x8c660816,0x8c66081e,0x8c660826)
CONFIGS=((0,0),(1,0x400000a1),(4,0x600000a1),(7,0x400080a1))

def put(m,a,value,n=4):m[a&0xffffff:(a&0xffffff)+n]=(value&((1<<(8*n))-1)).to_bytes(n,'little')

def setup(ram,entry,args,mode=0,sr=0x400000a1):
    c=v.Input(dict(entry=entry,mode=mode,sr=sr,mach=0x89abcdef,macl=0x76543210,gbr=0x8ce01234,input_interface_arguments=list(args)))
    m,s,f=v.prepare(ram,c);s.sr=sr;f.fpscr=0x40001|(mode<<19)
    for j,value in enumerate(args):s.r[4+j]=value&0xffffffff
    s.r[0]=0x13579bdf
    for bank in range(2):
        for reg in range(16):f.banks[bank][reg]=(0x7fa12345^((bank+1)*0x11111111)^((reg+1)*0x01020304))&0xffffffff
    return c,m,s,f
