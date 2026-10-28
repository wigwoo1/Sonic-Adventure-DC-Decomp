"""Original field/table ABI with explicitly authored or untouched retail table inputs."""
import struct
import write_watch_movement_parameters as fast
import collision_contact_fixtures as previous
v=fast.old;P=v.P
ENTRY=0x8c075214
SELECTOR,OUTPUT,STATE,TABLE,STACK=0x8cb00000,0x8cb10000,0x8cb20000,0x8c1bdfe4,0x8cf00000
def bits(x):return struct.unpack('<I',struct.pack('<f',x))[0]
def number(word):return struct.unpack('<f',int(word).to_bytes(4,'little'))[0]
def put(m,a,x,n=4):m[a&0xffffff:(a&0xffffff)+n]=(int(x)&((1<<(8*n))-1)).to_bytes(n,'little')
def get(m,a,n=4):return int.from_bytes(m[a&0xffffff:(a&0xffffff)+n],'little')

def prepare(ram,c):
    m,s,f,base=previous.prepare(ram,dict(case_id=c.get('case_id',0),mode=c.get('mode',0)))
    cfg=v.Input(dict(base,entry=ENTRY,probe=True));s.pc=ENTRY;s.pr=0x8cfed000;s.r[15]=STACK
    f.fpscr=(c.get('mode',0)<<19)|c.get('fpscr',0x40000)|c.get('rm',0)
    state=c.get('state_address',STATE);selector=c.get('selector_address',SELECTOR);output=c.get('output_address',OUTPUT)
    for a,size in ((SELECTOR,0x100),(OUTPUT,0x100),(state,0x240)):
        m[a&0xffffff:(a&0xffffff)+size]=bytes([0x55])*size
    if c.get('authored_table',True):
        # Declared source data, not execution results. Populate every unsigned
        # selector index so alias-driven selector changes remain valid inputs.
        for index in range(256):
            for offset,value in ((0x78,4+index),(0x58,8+index),(0x3c,16+index)):
                put(m,TABLE+index*0x84+offset,bits(value))
    for item in c.get('table_words',()):put(m,TABLE+item['index']*0x84+item['offset'],item['word'])
    put(m,state+0x5c,c.get('old_flags',0x12340000));put(m,state+0x1e4,c.get('state_flags',0))
    put(m,selector+9,c.get('selector',0),1)
    view=c.get('view',0x8c000000);ptr=lambda a:view|(a&0xffffff)
    s.r[4],s.r[5],s.r[6],s.r[7]=ptr(selector),ptr(output),ptr(state),c.get('flags',0)&0xffffffff
    if c.get('bad')=='stack':s.r[15]=0x8b000000
    if c.get('bad')=='selector':s.r[4]=0x8b000000
    if c.get('bad')=='state':s.r[6]=0x8b000000
    if c.get('bad')=='output':s.r[5]=0x8b000000
    return m,s,f,cfg
