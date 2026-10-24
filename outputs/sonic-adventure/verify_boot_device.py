"""Actual frame command finalization and dependencies, with bounded TA capture."""
import collections,csv,ctypes,inspect,itertools,json,sys,types
from pathlib import Path
import verify_boot_wait as previous
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent
MANIFEST=PROJECT/'analysis/boot_device_translation.json';manifest=json.loads(MANIFEST.read_text())
FRAME,TAG,COPY,SELECT,READY,QUERY,TAIL,COPY8=(0x8c642440,0x8c641bc0,0x8c65253e,0x8c6564e0,0x8c65661c,0x8c650f78,0x8c6514e0,0x8c652520)
CURRENT,RECORDS,CHOSEN=0x8ce80000,0x8c8a3398,0x8c8a3384
SOURCE,DEST=0x8ce00000,0x8ce10000
NOP,UNKNOWN=previous.NOP,previous.UNKNOWN
ROWS=dict(previous.ROWS);OWN={};PER_BODY={};NEW={int(r['address'],16) for r in manifest['routines']}
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')}
 ROWS.update(body);OWN.update(body);PER_BODY[int(routine['address'],16)]=body
code=previous.code
marker='if destination&0x1f000000!=0x0c000000:';assert code.count(marker)==1
code=code.replace(marker,"if 0x10000000<=destination<0x10800000 and len(queue_reference.packets)>=queue_reference.capacity:\n     queue_reference.events.append((3,a,0,0));queue_reference.prefetches+=1;stop(pc,r,sr,-4)\n    if destination&0x1f000000!=0x0c000000 and not 0x10000000<=destination<0x10800000:")
marker='queue_reference.clock_cursor';assert code.count(marker)==2
# Only the boundary event tuple gets packet contents; timer guards stay intact.
code=code.replace('queue_reference.clock_cursor\n','queue_reference.clock_cursor,tuple(queue_reference.packets)\n',1)
namespace=dict(previous.namespace,ROWS=ROWS,RETURNS=previous.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=previous.namespace['LEGACY']-NEW)
exec(compile(code,'frame_command_original_model','exec'),namespace)
effects=types.SimpleNamespace(**previous.effects.__dict__);effects.CONNECTED=previous.effects.CONNECTED|NEW;effects.namespace=namespace;effects.reference_run=namespace['reference_run']
effects.DLL=PROJECT.parents[1]/'work/sonic-adventure/build/boot_device.dll';effects.LIB=ctypes.CDLL(str(effects.DLL));effects.fn=effects.LIB.sa_boot_device_connected;effects.fn.argtypes=previous.effects.fn.argtypes;effects.fn.restype=ctypes.c_int
class Packet(ctypes.Structure):_fields_=[('destination',ctypes.c_uint32),('queue',ctypes.c_uint32),('words',ctypes.c_uint32*8)]
class FrameMemory(ctypes.Structure):
 _fields_=[('graphics',previous.registers.Memory),('graphics_bus',effects.Bus),('packet_count',ctypes.c_uint32),('packet_capacity',ctypes.c_uint32),('packets',Packet*64)]
 @property
 def vertex(self):return self.graphics.vertex
 @property
 def registers(self):return self.graphics.registers
effects.LIB.sa_frame_command_bus_init.argtypes=[ctypes.POINTER(FrameMemory),ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(effects.Bus)]
def packet_snapshot(memory):return tuple((p.destination,p.queue,tuple(p.words)) for p in memory.packets[:memory.packet_count])
class FrameReference(previous.ClockReference):
 def __init__(self,ticks,capacity):super().__init__(ticks);self.packets=[];self.capacity=capacity
 def prefetch(self,a):
  if a>>26==0x38:
   q=(a//32)%2;destination=(a&0x3ffffe0)+((self.qacr[q]&28)*0x1000000)
   if 0x10000000<=destination<0x10800000:
    assert len(self.packets)<self.capacity
    self.events.extend(((3,a,0,0),(9,destination,q,32)));self.prefetches+=1;self.flushes+=1
    self.packets.append((destination,q,tuple(self.queue[q*8:q*8+8])));return
  super().prefetch(a)
compare_code=inspect.getsource(previous.compare).replace("ClockReference(c.get('ticks',[0x12345678]))","FrameReference(c.get('ticks',[0x12345678]),c.get('capacity',64))")
compare_code=compare_code.replace('memory=registers.Memory()','memory=FrameMemory()').replace('sa_render_register_bus_init','sa_frame_command_bus_init')
compare_code=compare_code.replace('native_clock=[];original_read=bus.read',"memory.packet_capacity=c.get('capacity',64);native_clock=[];original_read=bus.read")
compare_code=compare_code.replace('tuple(memory.registers),len(native_clock)','tuple(memory.registers),len(native_clock),packet_snapshot(memory)')
compare_code=compare_code.replace(' return dict(input='," assert packet_snapshot(memory)==tuple(ref.packets),(c,'full command-packet stream')\n return dict(command_packets=packet_snapshot(memory),queue_events=len(ref.events),input=")
exec(compile(compare_code,'frame_command_comparison','exec'),dict(previous.__dict__,effects=effects,namespace=namespace,FrameMemory=FrameMemory,FrameReference=FrameReference,packet_snapshot=packet_snapshot),local:={})
compare=local['compare']

def ptr(m,literal):return load(m,literal)
def put(m,literal,value):store(m,ptr(m,literal),value)
def words(m,a,n):return [load(m,a+4*i) for i in range(n)]
def seed_ram():
 ram=bytearray(16<<20)
 for p,offset in ((effects.SEED.state.gate.support.morph.MAIN,0x10000),(effects.SEED.state.gate.support.morph.MODULE,0x900000)):
  raw=p.read_bytes();ram[offset:offset+len(raw)]=raw
 return ram

def prepare(ram,c):
 m,s,f=previous.prepare(ram,c);s.pc=c['entry'];alias=c.get('data_alias',0x8c000000)
 for i in range(64):store(m,SOURCE+i*4,(i*0x01020307)^0x59a5c33c);store(m,DEST+i*4,0xdeadbeef)
 store(m,0x8c8a3388,CURRENT)
 for group in range(3):
  for lane in range(5):
   base=alias|0x700100|group*0x20000|lane*0x2000
   store(m,RECORDS+group*20+lane*4,base)
 for lane in range(5):
  for slot in range(4):
   store(m,ptr(m,0x8c642678+lane*4)+slot*4,0x13130000+lane*0x100+slot*4)
   store(m,ptr(m,0x8c64268c+lane*4)+slot*4,0x91910000+lane*0x100+slot*4)
  store(m,ptr(m,0x8c6426bc)+lane*4,c.get('maximum',48))
  store(m,ptr(m,0x8c6426c0)+lane*4,c.get('limits',[256]*5)[lane])
  delta=c.get('deltas',[0,4,32,64,96])[lane]
  store(m,0x8c8a33f4+lane*4,(delta//4)&0xffffffff)
 put(m,0x8c6426b0,c.get('renderer',0));put(m,0x8c642674,c.get('slot',0));put(m,0x8c6426b4,c.get('mask',31))
 put(m,0x8c642880,c.get('flags',0));put(m,0x8c642884,c.get('selected',0))
 put(m,0x8c642664,0x40 if c.get('pre_gate',0) else 0)
 for i,literal in enumerate((0x8c642668,0x8c64266c,0x8c642670)):put(m,literal,int(c.get('pre_gate',0)>i+1))
 put(m,0x8c651500,NOP if c.get('callback','native')=='native' else UNKNOWN)
 put(m,0x8c642a5c,c.get('index',0));put(m,0x8c642a58,0x13579bdf)
 index=0 if c.get('renderer',0)==1 else c.get('index',0)
 store(m,CHOSEN,RECORDS+20*index)
 for lane in range(5):store(m,CURRENT+lane*4,(load(m,RECORDS+20*index+lane*4)+c.get('deltas',[0,4,32,64,96])[lane])&0xffffffff)
 query=c.get('query',0x31415926);store(m,0x8c8a3174,query)
 table=ptr(m,0x8c650fe4)
 for i in range(16):
  for j in range(8):store(m,table+32*i+4*j,0xabc00000+i*0x100+j)
  store(m,table+32*i,query^0x80000000);store(m,table+32*i+24,0)
 if c.get('match') is not None:
  store(m,table+32*c['match'],query);store(m,table+32*c['match']+24,c.get('ready_flags',1))
 if c['entry'] in (COPY,COPY8):
  s.r[4]=c.get('dest',DEST);s.r[5]=c.get('source',SOURCE);s.r[6]=c.get('length',32)
 elif c['entry']==SELECT:s.r[4]=c.get('index',0)
 elif c['entry']==QUERY:s.r[4]=query
 elif c['entry']==TAIL:s.r[4]=c.get('value',2)
 if c.get('bad_stack') is not None:s.r[15]=c['bad_stack']
 return m,s,f

def helper_checks(m,actual,initial,fp,final,f,record,c):
 entry=initial.pc
 if record['status']!=0:return 0
 if entry in (COPY,COPY8):
  wanted=bytearray(m);src=initial.r[5];dst=initial.r[4];length=initial.r[6]
  paired=entry==COPY8 and (src|dst|length)&7==0
  count=length//(8 if paired else 4);width=(4 if fp.fpscr&0x100000 else 8) if paired else 4;stride=8 if paired else 4
  for _ in range(count):
   raw=bytes(wanted[src&0xffffff:(src&0xffffff)+width]);wanted[dst&0xffffff:(dst&0xffffff)+width]=raw;src+=width;dst+=stride
  assert actual==wanted and final.r[4]==dst and final.r[5]==src and final.r[6]==0
  assert final.pr==initial.pr and final.r[15]==initial.r[15] and f.fpscr==fp.fpscr
  return 3
 if entry==SELECT:
  wanted=bytearray(m);index=initial.r[4];base=RECORDS+(0 if c.get('renderer',0)==1 else index*20)
  store(wanted,CHOSEN,base)
  for i,value in enumerate(words(wanted,base,5)):store(wanted,CURRENT+4*i,value)
  for i,value in enumerate(words(wanted,CURRENT,5)):store(wanted,0x8c8a33d4+4*i,value)
  store(wanted,0x8c8a3380,index&1)
  for i in range(5):store(wanted,CURRENT+4*i,(load(wanted,CURRENT+4*i)+4*load(wanted,0x8c8a33f4+4*i))&0xffffffff)
  assert actual==wanted and final.r[15]==initial.r[15] and final.pr==initial.pr and bytes(f)==bytes(fp)
  return 4
 if entry in (QUERY,READY):
  target=initial.r[4] if entry==QUERY else load(m,0x8c8a3174);table=ptr(m,0x8c650fe4)
  found=any(load(m,table+i*32)==target and load(m,table+i*32+24)&3 for i in range(16))
  assert final.r[0]==((0 if found else 1) if entry==QUERY else (13 if found else 0))
  assert final.pr==initial.pr and final.r[15]==initial.r[15] and bytes(f)==bytes(fp)
  return 2
 if entry==TAIL:
  wanted=bytearray(m);store(wanted,initial.r[15]-4,initial.r[4]);assert actual==wanted
  assert final.pr==initial.pr and final.r[15]==initial.r[15] and bytes(f)==bytes(fp)
  calls=[x for x in record['boundaries'] if x['target']==hex(NOP)];assert len(calls)==1 and int(calls[0]['pr'],16)==initial.pr
  return 3
 if entry==TAG:
  address=load(m,CURRENT);assert actual[address&0xffffff:(address&0xffffff)+128]==m[0x6736fc:0x67377c]
  assert load(actual,CURRENT)==(address+128)&0xffffffff and not record['command_packets']
  assert final.pr==initial.pr and final.r[15]==initial.r[15] and bytes(f)==bytes(fp)
  return 3
 return 0

def frame_checks(m,actual,initial,fp,final,f,record,c):
 if record['status']!=0:return 0
 slot=c.get('slot',0);mask=c.get('mask',31);renderer=c.get('renderer',0);selected=c.get('selected',0);special=bool(renderer and c.get('flags',0)&32)
 bases=words(m,load(m,CHOSEN),5);cursor=words(m,CURRENT,5);maximum=words(m,ptr(m,0x8c6426bc),5);limits=words(m,ptr(m,0x8c6426c0),5)
 lengths=[load(m,ptr(m,0x8c64268c+4*i)+slot*4) for i in range(5)];writes=[];checks=0
 header=bytearray(m[ptr(m,0x8c642660)&0xffffff:(ptr(m,0x8c642660)&0xffffff)+32]);terminator=m[ptr(m,0x8c6426b8)&0xffffff:(ptr(m,0x8c6426b8)&0xffffff)+4]
 if c.get('pre_gate')==4:writes.append((cursor[0],bytes(m[0x6736fc:0x67377c])));cursor[0]+=128
 def length(i):
  diff=(cursor[i]-bases[i])&0xffffffff;signed=diff if diff<0x80000000 else diff-0x100000000
  lengths[i]=(abs(signed)//4*(1 if signed>=0 else -1)*4)&0xffffffff
  writes.append((cursor[i],bytes(terminator)));cursor[i]+=32
 def reset():
  for i in range(5):
   if mask>>i&1 and (not special or i!=selected):
    word=(int.from_bytes(header[:4],'little')&0xf0ffffff)|(i<<24);header[:4]=word.to_bytes(4,'little')
    writes.extend(((bases[i],bytes(header)),(bases[i]+32,bytes(terminator))));lengths[i]=32
   elif renderer:lengths[i]=0
 if not renderer:
  for i in range(5):
   if mask>>i&1:length(i)
   else:lengths[i]=0
  for i in range(5):
   if mask>>i&1:
    maximum[i]=max(maximum[i],lengths[i])
    if lengths[i]>limits[i]:reset();break
 else:
  for i in range(5):
   if special and i==selected:continue
   if not special or mask>>i&1:length(i)
   if mask>>i&1:
    maximum[i]=max(maximum[i],lengths[i])
    if lengths[i]>limits[i]:reset();break
   else:lengths[i]=0
 wanted=bytearray(m)
 for a,raw in writes:wanted[a&0xffffff:(a&0xffffff)+len(raw)]=raw
 for a,raw in writes:assert actual[a&0xffffff:(a&0xffffff)+len(raw)]==wanted[a&0xffffff:(a&0xffffff)+len(raw)];checks+=1
 for i in range(5):
  assert load(actual,ptr(m,0x8c642678+4*i)+slot*4)==bases[i]
  assert load(actual,ptr(m,0x8c64268c+4*i)+slot*4)==lengths[i]
  assert load(actual,ptr(m,0x8c6426bc)+4*i)==maximum[i]
  for other in set(range(4))-{slot}:
   for literal in (0x8c642678+4*i,0x8c64268c+4*i):assert load(actual,ptr(m,literal)+4*other)==load(m,ptr(m,literal)+4*other)
  checks+=9
 index=(c.get('index',0)+1)&1;base=RECORDS+(0 if renderer==1 else index*20)
 assert load(actual,ptr(m,0x8c642a58))==0 and load(actual,ptr(m,0x8c642a5c))==index and load(actual,CHOSEN)==base
 for i in range(5):assert load(actual,CURRENT+4*i)==(load(m,base+4*i)+load(m,0x8c8a33f4+4*i)*4)&0xffffffff
 packets=record['command_packets'];expected_packet=special and cursor[selected]==bases[selected]
 assert len(packets)==int(expected_packet)
 if expected_packet:
  header[:4]=((int.from_bytes(header[:4],'little')&0xf0ffffff)|((selected&7)<<24)).to_bytes(4,'little')
  assert tuple(packets[0][2])==tuple(int.from_bytes(header[i:i+4],'little') for i in range(0,32,4))
  assert packets[0][0]==0x10000000|(cursor[selected]&0x3ffffe0)
 assert final.r[0]==0 and final.pr==initial.pr and final.r[15]==initial.r[15] and list(final.r[8:15])==list(initial.r[8:15]) and bytes(f)==bytes(fp)
 return checks+9

def bus_checks():
 ram=bytearray(16<<20);view=effects.RAM_TYPE.from_buffer(ram);memory=FrameMemory();bus=effects.Bus();checks=0
 effects.LIB.sa_frame_command_bus_init(ctypes.byref(memory),view,ctypes.byref(bus))
 expected=[]
 for number in range(64):
  destination=0x10000000+(number*32 if number<63 else 0x7fffe0);a=0xe0000000|(destination&0x3ffffe0);q=(a>>5)&1
  assert bus.write(bus.context,0xff000038+4*q,16,4)==0
  values=[(number<<24)|(i*0x10203) for i in range(8)]
  for i,value in enumerate(values):assert bus.write(bus.context,a+i*4,value,4)==0
  assert bus.cache(bus.context,a,1)==0
  expected.append((destination,q,tuple(values)));assert packet_snapshot(memory)==tuple(expected);checks+=11
 memory.packet_capacity=65
 assert bus.cache(bus.context,0xe0000000,1)==-4 and packet_snapshot(memory)==tuple(expected)
 assert memory.vertex.prefetches==65 and memory.vertex.flushes==64 and not any(ram);checks+=3
 # Explicit exhausted storage does not drop a packet and pretend submission.
 effects.LIB.sa_frame_command_bus_init(ctypes.byref(memory),view,ctypes.byref(bus));memory.packet_capacity=0
 assert bus.write(bus.context,0xff000038,16,4)==0 and bus.cache(bus.context,0xe0000000,1)==-4
 assert not memory.packet_count and memory.vertex.prefetches==1 and not memory.vertex.flushes;checks+=2
 effects.LIB.sa_frame_command_bus_init(ctypes.byref(memory),view,ctypes.byref(bus));memory.vertex.trace_count=4096
 assert bus.write(bus.context,0xff00003c,16,4)==0
 for i in range(8):assert bus.write(bus.context,0xe0000020+4*i,0xa5a50000+i,4)==0
 assert bus.cache(bus.context,0xe0000020,1)==0
 assert memory.vertex.trace_dropped==11 and packet_snapshot(memory)==((0x10000020,1,tuple(0xa5a50000+i for i in range(8))),);checks+=11
 effects.LIB.sa_frame_command_bus_init(ctypes.byref(memory),view,ctypes.byref(bus))
 for target in (0x10800000,0x14000000,0):
  assert bus.write(bus.context,0xff000038,(target>>24)&28,4)==0
  a=0xe0000000|(target&0x3ffffe0)
  assert bus.cache(bus.context,a,1)==-3 and not memory.packet_count;checks+=2
 assert bus.cache(bus.context,0x8c123440,1)==0 and bus.cache(bus.context,0x8c123440,0)==0
 assert bus.cache(bus.context,0x8c123440,2)==0 and bus.cache(bus.context,0x8c123440,3)==-3
 assert bus.write(bus.context,0xa05f8078,0x12345678,4)==0
 out=ctypes.c_uint32(0xfedcba98)
 assert bus.read(bus.context,0xa05f8078,4,ctypes.byref(out))==0 and out.value==0x12345678
 assert bus.read(bus.context,0xff000034,4,ctypes.byref(out))==-3 and out.value==0x12345678
 assert bus.write(bus.context,0xe0000001,0,4)==-3 and bus.write(bus.context,0xe0000000,0,8)==-3;checks+=9
 effects.LIB.sa_frame_command_bus_init(ctypes.byref(memory),view,ctypes.byref(bus))
 assert memory.packet_capacity==64 and not memory.packet_count and not memory.vertex.trace_count and not any(memory.registers)
 assert all(not any(bytes(packet)) for packet in memory.packets);checks+=2
 return dict(independent_checks=checks,capacity=64,accepted_packets=65,overflow_stops=2,unsupported_destinations=3,trace_capacity_preserves_packets=True)

def query_entry_edge_proof(ram):
 # The first unsigned bound check is executed only once: both operands are
 # fixed literal-derived values, and later block iterations jump to f9c.
 expected={0x8c650f88:'mov.w 0x8c650fe0,r8',0x8c650f8a:'mov.l 0x8c650fe4,r6',0x8c650f8e:'add r6,r8',0x8c650f90:'mov r6,r7',0x8c650f92:'cmp/hs r8,r7',0x8c650f94:'mov r6,r5',0x8c650f96:'mov r6,r12',0x8c650f98:'bt/s 0x8c650fc4',0x8c650fc0:'bf/s 0x8c650f9c'}
 assert all(ROWS[a]==t for a,t in expected.items())
 base=load(ram,0x8c650fe4);extent=load(ram,0x8c650fe0,2);assert base==0x8c8a2b38 and extent==512 and base<base+extent<0x100000000
 return dict(address=hex(QUERY),unreachable_edges=[['0x8c650f98','0x8c650fc4']],base=hex(base),extent=extent,assumptions='Original immutable literal/code bytes and valid distinct stack/data. No self modification or stack/code aliasing.',reason='R7 is fixed base and R8 is fixed base+512 at the only entry comparison; unsigned base>=base+512 is false. Later iterations bypass this comparison.')

def cases():
 for entry,mode,length in itertools.product((COPY,COPY8),range(8),(4,8,32)):
  yield dict(entry=entry,mode=mode,length=length)
 for entry,offset,alias in itertools.product((COPY,COPY8),(4,8),(0x0c000000,0xac000000)):
  yield dict(entry=entry,mode=0,length=32,source=SOURCE,dest=(SOURCE+offset)&0xffffff|alias)
 for entry,length in itertools.product((COPY,COPY8),(0,1,3)):
  yield dict(entry=entry,mode=0,length=length,source=0x8cfffffc)
 for renderer,index,bank in itertools.product((0,1,2),(0,1,2),range(2)):yield dict(entry=SELECT,mode=bank*4,renderer=renderer,index=index,deltas=[0,4,-4,128,0])
 for entry,match,flags,bank in itertools.product((QUERY,READY),(None,0,7,8,15),(0,1,2,3,4),range(2)):
  yield dict(entry=entry,mode=bank*4,match=match,ready_flags=flags)
 for callback,value,mode in itertools.product(('native','unresolved'),(0,2,0xffffffff),range(8)):yield dict(entry=TAIL,mode=mode,callback=callback,value=value)
 for alias,offset,mode in itertools.product((0x8c000000,0x0c000000,0xac000000),(0,32),range(8)):yield dict(entry=TAG,mode=mode,data_alias=alias,deltas=[offset,0,0,0,0])
 for renderer,flags,mask,bank in itertools.product((0,1,2),(0,32),(0,1,21,31),range(2)):
  yield dict(entry=FRAME,mode=bank*4,renderer=renderer,flags=flags,mask=mask,slot=bank*3,selected=4,index=bank)
 for renderer,flags,lane,limit in itertools.product((0,1),(0,32),range(5),(31,32,33)):
  limits=[0xffffffff]*5;limits[lane]=limit
  yield dict(entry=FRAME,mode=0,renderer=renderer,flags=flags,selected=4 if lane!=4 else 0,mask=31,limits=limits,deltas=[32]*5,maximum=128,slot=2)
 for renderer,flags,negative in itertools.product((0,1),(0,32),(-4,-8)):
  yield dict(entry=FRAME,mode=0,renderer=renderer,flags=flags,selected=4,deltas=[negative,0,4,32,0],limits=[0xffffffff]*5)
 for renderer,flags in itertools.product((0,1),(0,32)):yield dict(entry=FRAME,mode=0,renderer=renderer,flags=flags,selected=4,mask=21,limits=[0]*5)
 for gate in range(5):yield dict(entry=FRAME,mode=0,pre_gate=gate,mask=31,limits=[512]*5)
 for selected in (0,1,4):
  for equal in (False,True):
   deltas=[0]*5;deltas[selected]=0 if equal else 32
   yield dict(entry=FRAME,mode=0,renderer=1,flags=32,selected=selected,deltas=deltas,mask=0)
 for entry in (FRAME,READY,TAIL,TAG):
  for stack in (0,0x8d000004):yield dict(entry=entry,mode=0,bad_stack=stack)
 yield dict(entry=FRAME,mode=0,renderer=1,flags=32,selected=0,deltas=[0]*5,capacity=0)
 for renderer in (0,1):yield dict(entry=FRAME,mode=0,renderer=renderer,limits=[0]*5,callback='unresolved')

def coverage_row(routine,coverage,edges):
 a=int(routine['address'],16);body=PER_BODY[a];wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
 return dict(address=hex(a),static_rows=len(body),covered_rows=len(set(body)&coverage),missing_rows=[hex(x) for x in sorted(set(body)-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(x),hex(y)] for x,y in sorted(wanted-edges)])

def main():
 proofs=[MANIFEST,PROJECT/'analysis/boot_device_connection.json',PROJECT/'analysis/boot_wait_verification.json']
 for p in proofs:
  for file,h in json.loads(p.read_text())['sha256'].items():assert previous.boot.digest(file)==h,file
 ram=seed_ram();selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(entry=e,mode=0) for e in (COPY,COPY8,SELECT,QUERY,READY,TAIL,TAG,FRAME)]+[dict(entry=FRAME,mode=0,renderer=1,flags=32,deltas=[0]*5)]
 records=[];coverage=set();edges=set()
 for i,c in enumerate(selected):
  m,s,f=prepare(ram,c);initial=effects.State.from_buffer_copy(s);initial_f=effects.Fpu.from_buffer_copy(f)
  record,path,actual,final,out=compare(m,s,f,c)
  record['independent_checks']=frame_checks(m,actual,initial,initial_f,final,out,record,c) if c['entry']==FRAME else helper_checks(m,actual,initial,initial_f,final,out,record,c)
  records.append(record);coverage.update(path)
  for j,a in enumerate(path):
   if a not in OWN:continue
   w=int.from_bytes(ram[a&0xffffff:(a&0xffffff)+2],'little')
   distance=2 if w>>8 in (0x8d,0x8f) else 1
   if w>>8 in (0x89,0x8b,0x8d,0x8f) and j+distance<len(path):edges.add((a,path[j+distance]))
  if (i+1)%32==0:print(i+1,'frame/dependency comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Frame/dependency smoke passed:',len(records));return
 sequences=[]
 for renderer,flags,index in itertools.product((0,1,2),(0,32),(0,1)):
  c=dict(entry=SELECT,mode=0,renderer=renderer,flags=flags,index=index,selected=0,mask=21,deltas=[0,4,32,64,96])
  m,s,f=prepare(ram,c);sequence=[]
  for entry in (SELECT,FRAME):
   c=dict(c,entry=entry);s.pc=entry;initial=effects.State.from_buffer_copy(s);initial_f=effects.Fpu.from_buffer_copy(f)
   record,path,actual,s,f=compare(m,s,f,c);assert record['status']==0
   record['independent_checks']=helper_checks(m,actual,initial,initial_f,s,f,record,c) if entry==SELECT else frame_checks(m,actual,initial,initial_f,s,f,record,c)
   sequence.append(record);m=actual;coverage.update(path)
  sequences.append(sequence)
 prefixes=[];wait_coverage=set();boot_coverage=set()
 for entry,renderer,bank in itertools.product((previous.WAIT,previous.boot.BOOT),(0,1),range(2)):
  c=dict(entry=entry,mode=bank*4,renderer=renderer,mask=31,gate=1,pending=1,callback='native',ticks=[0,previous.TICK_LIMIT])
  m,s,f=prepare(ram,c);record,path,*_=compare(m,s,f,c)
  assert record['status']==-2 and record['stop_pc']=='0x8c641f00'
  prefixes.append(record);wait_coverage.update(set(path)&set(previous.PER_BODY[previous.WAIT]));boot_coverage.update(set(path)&set(previous.boot.geometry.PER_BODY[previous.boot.BOOT]))
 rows=[coverage_row(r,coverage,edges) for r in manifest['routines']];proof=query_entry_edge_proof(ram)
 for row in rows:
  if int(row['address'],16)==QUERY:assert row['missing_edges']==proof['unreachable_edges']
  else:assert not row['missing_edges'],row
  assert not row['missing_rows'],row
 backend=bus_checks()
 files=[Path(__file__),PROJECT/'verify_boot_wait.py',PROJECT/'verify_draw_view_registers.py']+proofs
 for p in proofs:files.extend(Path(file) for file in json.loads(p.read_text())['sha256'])
 report=dict(source=manifest['source'],source_sha256=previous.boot.digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=rows,independent_checks=sum(r['independent_checks'] for r in records),command_packets=sum(len(r['command_packets']) for r in records),query_entry_edge_proof=proof,backend=backend,native_producer_sequences=len(sequences),sequence_stage_comparisons=sum(len(s) for s in sequences),sequence_independent_checks=sum(r['independent_checks'] for s in sequences for r in s),boot_wait_prefix_cases=len(prefixes),wait_rows_covered=len(wait_coverage),boot_rows_covered=len(boot_coverage),next_dependency='0x8c641f00',external_game_service_fixtures=0,newly_registered_entries=0,dll_sha256=previous.boot.digest(effects.DLL),sha256={str(p):previous.boot.digest(p) for p in files},scope='Full reached/final machine, RAM, FPU/banks, integer banks, MACL/MACH/GBR, register/queue/timer and command-packet comparisons. Independent copy, selection, readiness, callback, packet/history/length/maximum and frame return checks. Twelve actual native selector -> frame sequences preserve RAM/machine state. Registration remains separate.',limitations=['Direct runtime command-buffer fixtures and explicit selector -> frame call boundaries do not establish natural boot scheduling or a playable frame.','The bounded host packet sink captures polygon-window SQ transfers, not GPU consumption, rendering, interrupt/status behavior, backpressure or timing. Capacity exhaustion is an explicit -4 host stop.','Inherited privileged bank-1/FD=0, MMU-off RAM, numerical and typed guard domains apply. Readiness entry edge proof excludes self-modifying code and stack/literal aliasing.'],cases=records,producer_sequences=sequences,boot_wait_prefixes=prefixes)
 (PROJECT/'analysis/boot_device_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','coverage_by_routine','independent_checks','command_packets','backend','native_producer_sequences','wait_rows_covered','boot_rows_covered','next_dependency')},indent=2))
if __name__=='__main__':main()
