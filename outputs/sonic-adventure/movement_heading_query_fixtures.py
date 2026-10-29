"""Initial actor/table/stage words and optional output pointers only."""
import movement_vectors_fixtures as previous
import write_watch_movement_heading_query as fast
v=fast.old;P=v.P
ENTRY,A,B,C,D,E=0x8c06cd22,0x8cb00000,0x8cb10000,0x8cb20000,0x8cb30000,0x8cb40000
TABLE,CHARACTERS,MAJOR,MINOR=0x8c754dcc,0x8c78c668,0x8c7493ba,0x8c7493bc
put,get,bits,number=(getattr(previous,n) for n in ('put','get','bits','number'))
def prepare(ram,c):
    m,s,f,base=previous.prepare(ram,dict(c,entry=0x8c078eb0))
    cfg=v.Input(dict(base,entry=ENTRY,probe=True));s.pc=ENTRY;s.pr=0x8cfed000
    f.fpscr=(c.get('mode',0)<<19)|c.get('fpscr',0x40000)|c.get('rm',0)
    s.sr=(s.sr&~0x8001)|c.get('t',0)|(0x8000 if c.get('fd') else 0)
    view=c.get('view',0x8c000000);ptr=lambda a:view|(a&0xffffff)
    for a in (A,B,C,D,E):m[a&0xffffff:(a&0xffffff)+0x200]=bytes(0x200)
    index=c.get('index',0);put(m,A+4,c.get('flags',0x2000),2);put(m,A+8,index,1);put(m,A+9,c.get('actor_mode',0),1)
    put(m,A+0x18,c.get('actor_heading',0xfedc1234));put(m,A+0x3c,ptr(E));put(m,E+0x24,c.get('override',1),1)
    put(m,D+0xa,c.get('character_word',0),2);put(m,CHARACTERS+4*index,ptr(D))
    put(m,TABLE+8*index,c.get('heading',0x12345678));put(m,TABLE+8*index+4,c.get('word',0x3f800000))
    put(m,MAJOR,c.get('major',9),2);put(m,MINOR,c.get('minor',2),2)
    put(m,B,0xa1b2c3d4);put(m,C,0x55667788)
    heading,output=B,C;alias=c.get('alias')
    if alias=='same_output':output=B
    elif alias=='heading_table_value':heading=TABLE+8*index+4
    elif alias=='heading_actor_index':
        assert index==1
        heading=A+8;put(m,TABLE+8,2);put(m,TABLE+2*8+4,0x40400000)
    elif alias=='value_actor_heading':output=A+0x18
    s.r[4],s.r[5],s.r[6]=ptr(A),ptr(heading) if c.get('heading_output',True) else 0,ptr(output) if c.get('value_output',True) else 0
    bad=c.get('bad')
    if bad=='stack':s.r[15]=0x8b000000
    elif bad=='actor':s.r[4]=0x8b000000
    elif bad=='heading':s.r[5]=0x8b000000
    elif bad=='value':s.r[6]=0x8b000000
    elif bad=='override_object':put(m,A+0x3c,0x8b000000)
    elif bad=='character':put(m,CHARACTERS+4*index,0x8b000000)
    return m,s,f,cfg
