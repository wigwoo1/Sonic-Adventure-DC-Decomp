"""Authored query, scene objects, linked polygon records and external RAM inputs.

All selection arrays/counts are produced by original callees, never supplied.
Sentinels and head positions are explicit scene input, not initialization proof.
"""
import struct
import collision_contact_fixtures as previous
import write_watch_movement_world as fast
v=fast.old;P=v.P
QUERY,NODES,LOW,HIGH,OBJECTS,MODELS,OUTPUT=0x8cb00000,0x8cb10000,0x8cb20000,0x8cb40000,0x8cb60000,0x8cb80000,0x8cba0000
WORLD,COUNT,CONTACTS=0x8c02d548,0x8c73e27c,0x8c73e280
NEAR_COUNT,NEAR,STATIC_COUNT,STATIC=0x8c754ef0,0x8c754ef4,0x8c19e964,0x8c757ef4
DYNAMIC_ENABLE,DYNAMIC_PRESENT,DYNAMIC_COUNT,DYNAMIC=0x8c19e978,0x8c7596f4,0x8c19e968,0x8c7584f4
IDENTITY=0x8c67c640

def bits(x):return struct.unpack('<I',struct.pack('<f',x))[0]
def number(x):return struct.unpack('<f',int(x).to_bytes(4,'little'))[0]
def put(m,a,x,n=4):m[a&0xffffff:(a&0xffffff)+n]=(int(x)&((1<<(8*n))-1)).to_bytes(n,'little')
def get(m,a,n=4):return int.from_bytes(m[a&0xffffff:(a&0xffffff)+n],'little')
def vector(m,a,values):
    for j,x in enumerate(values):put(m,a+4*j,bits(x))

def prepare(ram,c):
    m,s,f,base=previous.prepare(ram,dict(case_id=c.get('case_id',0),mode=c.get('mode',0)))
    cfg=v.Input(dict(base)|dict(entry=c['entry'],probe=True))
    s.pc=c['entry'];s.pr=0x8cfed000;s.r[15]=0x8cf00000
    f.fpscr=(c.get('mode',0)<<19)|c.get('fpscr',0x40000)|c.get('rm',0)
    view=c.get('view',0x8c000000);ptr=lambda a:view|(a&0xffffff)
    for a,size in ((QUERY,0x100),(NODES,0x1000),(LOW,0x18000),(HIGH,0x18000),(OBJECTS,0x2000),(MODELS,0x2000),(OUTPUT-0x40,0x200)):
        m[a&0xffffff:(a&0xffffff)+size]=bytes(size)
    put(m,0x8c88f698,c.get('limit',64));put(m,0x8c88f69c,c.get('depth',1));put(m,0x8c88f5f8,0x8cd00000)
    put(m,COUNT,0x12345678);put(m,NEAR_COUNT,0x5555,2)
    put(m,0x8c752bdc,c.get('debug',0x80),1)
    point=c.get('point',(0,0,0));radius=c.get('radius',1)
    vector(m,QUERY,(radius,*point,*c.get('motion',(0,0,0))))
    if c.get('radius_word') is not None:put(m,QUERY,c['radius_word'])
    static=c.get('static',[dict()]);dynamic=c.get('dynamic',[])
    put(m,STATIC_COUNT,len(static));put(m,DYNAMIC_ENABLE,c.get('dynamic_enable',int(bool(dynamic))))
    put(m,DYNAMIC_PRESENT,c.get('dynamic_present',int(bool(dynamic))));put(m,DYNAMIC_COUNT,c.get('dynamic_count',len(dynamic)))
    for j,record in enumerate(static):
        obj=OBJECTS+(j%16)*0x100;model=MODELS+(j%16)*0x100
        for off,value in ((0,record.get('flags',0)),(4,ptr(obj)),(8,record.get('id',0x100+j))):put(m,STATIC+j*12+off,value)
        put(m,obj+4,ptr(model));vector(m,obj+8,record.get('position',(0,0,0)))
        vector(m,model+0x18,record.get('centre',(0,0,0)));put(m,model+0x24,bits(record.get('radius',2)))
        for off,value in zip((0x14,0x18,0x1c),record.get('angles',(0,0,0))):put(m,obj+off,value)
    # These are input dynamic scene records. The producer's 1024-item early
    # return may intentionally ignore these bytes entirely.
    for j,record in enumerate(dynamic):
        a=DYNAMIC+j*0x24;vector(m,a,record.get('position',(0,0,0)));put(m,a+0xc,bits(record.get('radius',2)))
        put(m,a+0x18,record.get('object',ptr(OBJECTS+(j%16)*0x100)));put(m,a+0x20,record.get('flags',1))
    # This fixed presence word lies inside a sufficiently large authored dynamic
    # table (record128.x). The overlap is real RAM, not a disjointness assumption.
    put(m,DYNAMIC_PRESENT,c.get('dynamic_present',int(bool(dynamic))))
    nodes=c.get('nodes',[]);put(m,WORLD,ptr(NODES) if nodes else 0)
    for j,node in enumerate(nodes):
        a=NODES+j*0x100;put(m,a,ptr(a+0x100) if j+1<len(nodes) else 0)
        put(m,a+8,node.get('object',ptr(OBJECTS+node.get('object_index',0)*0x100)));put(m,a+0x28,bits(node.get('radius',0)))
        put(m,a+0x1c,0x11111111);put(m,a+0x24,0x22222222)
        for field,offset,root in (('low',0x10,LOW),('high',0x14,HIGH)):
            records=node.get(field,[]);root+=j*0x4000
            for k,record in enumerate(records):
                address=root+k*0x40
                put(m,address,ptr(root+(k-1)*0x40) if k else 0)
                put(m,address+4,ptr(root+(k+1)*0x40) if k+1<len(records) else 0)
                vector(m,address+8,record.get('point',(0,0,record.get('key',0))))
                put(m,address+0x10,bits(record.get('key',0)))
                put(m,address+0x14,bits(record.get('extent',0)))
            head=node.get(field+'_head',0)
            put(m,a+offset,ptr(root+head*0x40) if records else 0)
    bank=(f.fpscr>>21)&1
    if c['entry']==0x8c028bfe:s.r[4]=ptr(QUERY)
    elif c['entry']==0x8c0522c0:
        for reg,x in zip((4,5,6,7),(*point,radius)):f.banks[bank][reg]=bits(x)
    elif c['entry']==0x8c63a8e0:
        s.r[4]=0 if not c.get('explicit') else ptr(OUTPUT+c.get('output_offset',0))
    else:raise AssertionError(hex(c['entry']))
    if c.get('bad')=='stack':s.r[15]=0x8b000000
    if c.get('bad')=='query':s.r[4]=0x8b000000
    if c.get('bad')=='output':s.r[4]=0x8b000000
    if c.get('bad')=='object':put(m,STATIC+4,0x8cfffffc)
    return m,s,f,cfg
