"""Ordinary actor/state and authored scene inputs for the complete special tail.
World/contact arrays are produced by actual callees,never supplied as outputs.
"""
import movement_world_fixtures as world
import write_watch_movement_special_tail as fast
v=fast.old;P=v.P
ENTRY=0x8c0777e0
A,B,C,Q,D=0x8ca00000,0x8ca10000,0x8ca20000,0x8ca30000,0x8ca40000
put,get,bits,number,vector=(getattr(world,n) for n in ('put','get','bits','number','vector'))
def prepare(ram,c):
    m,s,f,base=world.prepare(ram,dict(c,entry=0x8c028bfe,static=c.get('static',[]),nodes=c.get('nodes',[]),dynamic=c.get('dynamic',[])))
    cfg=v.Input(dict(base,entry=ENTRY,probe=True));s.pc=ENTRY;s.pr=0x8cfed000;s.r[15]=0x8cf00000
    view=c.get('view',0x8c000000);ptr=lambda a:view|(a&0xffffff)
    for a in (A,B,C,Q,D):m[a&0xffffff:(a&0xffffff)+0x200]=bytes(0x200)
    put(m,A+4,c.get('flags',1),2);put(m,A+8,c.get('index',0),1);put(m,A+9,c.get('character_kind',0),1)
    for j,x in enumerate(c.get('angles',(0,0,0))):put(m,A+0x14+4*j,x)
    vector(m,A+0x20,c.get('position',(0,0,0)));put(m,A+0x3c,ptr(D))
    vector(m,B+4,c.get('velocity',(1,0,0)))
    put(m,C+0x10,c.get('prior_status',1),2);put(m,C+0xa,c.get('state_word_a',0),2);put(m,C+0x60,c.get('state_flags',0));put(m,C+0x64,ptr(Q))
    for offset,name,default in ((0x110,'height',2),(0x38,'state_speed',0),(0x3c,'lift',0),(0x40,'vertical',0),(0xc0,'low',0),(0xc4,'threshold',1),(0xcc,'cap',20),(0xd0,'speed_cap',2),(0xa0,'acceleration',1),(0x1c,'slope',1)):
        put(m,C+offset,bits(c.get(name,default)))
    vector(m,C+0x50,c.get('old_normal',(0,1,0)))
    put(m,C+0x6c,c.get('previous_owner',0));put(m,C+0x74,c.get('previous_object',0))
    put(m,D+0xa,c.get('character_word',0),2)
    vector(m,D+4,c.get('character_velocity',(1,0,0)));put(m,D+0x20,c.get('character_heading',0));put(m,D+0x24,c.get('override',1),1)
    for table in (0x8c78c628,0x8c78c668):put(m,table+4*c.get('index',0),ptr(D))
    put(m,0x8c754dcc+8*c.get('index',0),c.get('table_heading',0));put(m,0x8c754dd0+8*c.get('index',0),bits(c.get('table_value',1)))
    vector(m,0x8c754bdc,c.get('reference',(0,1,0)))
    put(m,0x8c7493ba,c.get('major',0),2);put(m,0x8c7493bc,c.get('minor',0),2)
    # Authored mesh input behind the world's linked broad-phase records.
    # The world producer itself builds node polygon pointers and nearby lists.
    for j,node in enumerate(c.get('nodes',[])):
        for side,root in (('low',world.LOW),('high',world.HIGH)):
            for k,record in enumerate(node.get(side,[])):
                address=root+j*0x4000+k*0x40
                vertices=record.get('vertices',c.get('vertices',((0,0,0),(0,0,2),(2,0,0))))
                for n,point in enumerate(vertices):vector(m,address+0x18+12*n,point)
                put(m,address+0x3c,record.get('face_flags',c.get('face_flags',0)))
    s.r[4],s.r[5],s.r[6],s.r[7]=ptr(A),ptr(B),ptr(C),0
    if c.get('fd'):s.sr|=0x8000
    for key,reg in (('actor',4),('aux',5),('state',6)):
        if c.get('bad')==key:s.r[reg]=0x8b000000
    if c.get('bad')=='query':put(m,C+0x64,0x8b000000)
    if c.get('bad')=='stack':s.r[15]=0x8b000000
    return m,s,f,cfg
