"""Actual movement/model inputs; no supplied callee outputs or execution frames."""
import struct
import write_watch_movement_model_response as fast
import collision_contact_fixtures as previous
v=fast.old;P=v.P
OWNER,WALK,WRAP=0x8c0742d4,0x8c036bc0,0x8c036f12
ACTOR,A,B,EXTRA=0x8cb00000,0x8cb10000,0x8cb20000,0x8cb30000
NODES,MESHES=0x8cb50000,0x8cb60000
QUERY,STACK=previous.QUERY,0x8cf00000
put,get=previous.put,previous.get
def bits(x):return struct.unpack('<I',struct.pack('<f',x))[0]
def number(x):return struct.unpack('<f',int(x).to_bytes(4,'little'))[0]
def vector(m,a,values):
    for j,x in enumerate(values):put(m,a+4*j,bits(x))

def graph(c):
    flags=c.get('type',0)|c.get('flags',0)
    if c.get('tree'):
        return [(flags,1,3),(2|16,None,2),(5|8|16,None,None),(7,4,None),(4|16,None,None)]
    return [(flags,1,None),(7|8|16,None,None)]

def prepare(ram,c):
    m,s,f,base=previous.prepare(ram,dict(mode=c.get('mode',0),case_id=c.get('case_id',0),point=c.get('point',(.5,.25,.5)),motion=c.get('motion',(0,-2,0)),no_world=c.get('no_world',False),face_flags=c.get('face_flags',0)))
    cfg=v.Input(dict(base,entry=c.get('entry',OWNER),probe=True));entry=cfg['entry'];s.pc=entry;s.pr=0x8cfed000;s.r[15]=STACK
    f.fpscr=(c.get('mode',0)<<19)|c.get('fpscr',0x40000)|c.get('rm',0)
    view=c.get('view',0x8c000000);ptr=lambda a:view|(a&0xffffff)
    for a in (ACTOR,A,B,EXTRA):m[a&0xffffff:(a&0xffffff)+0x240]=bytes(0x240)
    for i,(flags,child,sibling) in enumerate(graph(c)):
        node=NODES+i*0x100;mesh=MESHES+i*0x100
        m[node&0xffffff:(node&0xffffff)+0x40]=bytes(0x40);m[mesh&0xffffff:(mesh&0xffffff)+0x40]=bytes(0x40)
        put(m,node,flags);put(m,node+4,ptr(mesh));vector(m,node+8,c.get('translation',(.5,-.25,.75)))
        for j,a in enumerate(c.get('angles',(0,0,0))):put(m,node+0x14+j*4,a&0xffffffff)
        vector(m,node+0x20,c.get('scale',(1,.5,2)))
        put(m,node+0x2c,ptr(NODES+child*0x100) if child is not None else 0);put(m,node+0x30,ptr(NODES+sibling*0x100) if sibling is not None else 0)
        vector(m,mesh+0x18,c.get('mesh_center',(0,0,-100)));put(m,mesh+0x24,bits(c.get('mesh_radius',1)))
    put(m,0x8c754ec8,c.get('mesh_gate',1))
    put(m,0x8c890070,c.get('normalize',0));put(m,0x8c752bdc,c.get('debug',0x80),1)
    if entry in (WALK,WRAP):
        s.r[4]=ptr(NODES)
        if c.get('bad')=='node':s.r[4]=0x8b000000
    else:
        assert entry==OWNER
        index=c.get('character',0);put(m,ACTOR+8,index,1);put(m,ACTOR+4,c.get('actor_flags',0),2)
        for j,a in enumerate(c.get('actor_angles',(0,0,0))):put(m,ACTOR+0x14+j*4,a&0xffffffff)
        vector(m,ACTOR+0x20,c.get('actor_position',c.get('point',(.5,.25,.5))))
        put(m,0x8c78c628+index*4,ptr(A));put(m,0x8c78c668+index*4,ptr(B))
        vector(m,A+4,c.get('velocity',(0,0,0)))
        put(m,B+0x60,c.get('state_flags',0));put(m,B+0x10,c.get('status_flags',0),2)
        for off,name,default in ((0xa0,'acceleration',1),(0x38,'speed',1),(0x3c,'height',0),(0xc0,'low',0),(0xc4,'threshold',1),(0xd0,'cap',2)):
            put(m,B+off,bits(c.get(name,default)))
        put(m,EXTRA,c.get('external_flags',0));put(m,EXTRA+4,previous.OBJECT_ID)
        # Full original132-byte query input. Collision executes against the real
        # authored world set up by the existing fixture, including current ID.
        m[QUERY&0xffffff:(QUERY&0xffffff)+0x84]=bytes(0x84)
        vector(m,QUERY,(c.get('radius',1),*c.get('point',(.5,.25,.5)),*c.get('motion',(0,-2,0)),*c.get('normal',(0,1,0))))
        vector(m,QUERY+0x48,c.get('orientation',(0,1,0)));vector(m,QUERY+0x54,c.get('anchor',(0,0,0)))
        vector(m,QUERY+0x60,c.get('base_position',c.get('point',(.5,.25,.5))));vector(m,QUERY+0x6c,c.get('delta',(0,0,0)))
        put(m,0x8c1be84c,c.get('global_mode',0));put(m,0x8c1be850,bits(c.get('character_height',1)));put(m,0x8c1be854,bits(c.get('character_radius',1)))
        vector(m,0x8c754bdc,c.get('up',(0,1,0)))
        s.r[4],s.r[5],s.r[6]=ptr(ACTOR),ptr(QUERY),ptr(EXTRA)
        if c.get('bad')=='actor':s.r[4]=0x8b000000
        if c.get('bad')=='query':s.r[5]=0x8b000000
        if c.get('bad')=='external':s.r[6]=0x8b000000
    if c.get('bad')=='stack':s.r[15]=0x8b000000
    return m,s,f,cfg
