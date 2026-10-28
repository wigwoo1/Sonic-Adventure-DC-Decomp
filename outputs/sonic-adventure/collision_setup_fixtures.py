"""External INPUT state for original collision-world/debug initializers."""
import collision_render_fixtures as render
import write_watch_collision_setup as fast
v=fast.old
P=v.P
WORLD_INIT,DEBUG_INIT=0x8c028e60,0x8c04e3de
POOL_A,COUNT_A,POOL_B,COUNT_B=0x8c6bb27c,192,0x8c6be27c,8192
DIRECT_POOL=0x8cc90000


def put(m,a,x,n=4):
    m[a&0xffffff:(a&0xffffff)+n]=(x&((1<<(8*n))-1)).to_bytes(n,'little')


def get(m,a,n=4):
    return int.from_bytes(m[a&0xffffff:(a&0xffffff)+n],'little')


def prepare(ram,c):
    m,s,f,base=render.prepare(ram,dict(case_id=c.get('case_id',0),mode=c.get('mode',0)))
    cfg=v.Input(dict(base)|dict(entry=c['entry'],probe=True))
    s.pc=c['entry'];s.pr=0x8cfed000;s.r[15]=0x8cf00000
    for i,value in enumerate(c.get('arguments',())):s.r[4+i]=value&0xffffffff
    for address,size in ((POOL_A,COUNT_A*64),(POOL_B,COUNT_B*64),(DIRECT_POOL,0x1000)):
        m[address&0xffffff:(address&0xffffff)+size]=bytes([0xa5])*size
    for address,value in ((0x8c1bfe20,c.get('mode_input',0)),(0x8c19a8e0,c.get('variant_input',0)),
                          (0x8c73e400,0x12345678),(0x8c02d548,0x87654321),
                          (0x8c19a8dc,0x12345678),(0x8c19a8d8,0x87654321)):
        put(m,address,value)
    put(m,0x8c752bdc,0,1)
    if c.get('bad')=='stack':s.r[15]=0x8b000000
    return m,s,f,cfg
