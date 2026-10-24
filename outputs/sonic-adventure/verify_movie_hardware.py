"""Original control/copy/wait chain with explicit hardware inputs and full state."""
import collections,csv,ctypes,hashlib,itertools,json,random,sys,types
from pathlib import Path
import verify_movie_transition as previous
import verify_boot_event as cpu
from verify_overlay_selector_initializer import load,store
P=Path(__file__).resolve().parent;ROOT=P.parents[1];MP=P/'analysis/movie_hardware_translation.json';manifest=json.loads(MP.read_text())
CONTROL,READ,WRITE,INSTALL,WAIT,RESTORE,FIFO_WAIT,FLAG,CHANNEL_RESTORE,CHANNEL_INSTALL,READY,STATUS=(0x8c6046c0,0x8c10d460,0x8c10d770,0x8c10ee9c,0x8c10eee0,0x8c10ee20,0x8c10ebac,0x8c10f9a8,0x8c10f9bc,0x8c10fa60,0x8c10fb48,0x8c1101f0)
FIFO,PORT=0xa05f688c,0xa0702c00;CTX,CHANNELS,SRC,DST,OUT=0x8cd60000,0x8cd64000,0x8cd70040,0x8cd71040,0x8cd78000
NEW={int(r['address'],16) for r in manifest['routines']};ROWS=dict(previous.ROWS);PER_BODY={}
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);PER_BODY[int(routine['address'],16)]=body
def fifo_address(a):return a&0x1fffffff==0x005f688c
def control_address(a):return 0x00702c00<=a&0x1fffffff<0x00702c04
# Add byte MMIO to a local copy of the existing independent executor. Retain
# its SR/banked-register/vector extensions and every original source byte.
assert previous.namespace['reference'] is cpu.cpu_ns['reference']
cpu_code=cpu.cpu_code;marker='    def read16(addr):';assert cpu_code.count(marker)==1
cpu_code=cpu_code.replace(marker,"""    def read8(addr):
        if mmio is not None and addr in mmio:return mmio[addr]&255
        return ram[index(addr)]
    def write8(addr,value):
        if mmio is not None and addr in mmio:mmio[addr]=value&255
        else:ram[index(addr)]=value&255
"""+marker)
patches={
 'ram[index(regs[m])]':'read8(regs[m])',
 'ram[index((regs[0]+regs[m])&MASK)]':'read8((regs[0]+regs[m])&MASK)',
 'ram[index((regs[0]+regs[n])&MASK)] = regs[m]&255':'write8((regs[0]+regs[n])&MASK,regs[m])',
 'value=ram[index(address)]':'value=read8(address)',
 'ram[index(regs[m] + (word & 15))] = regs[0] & 255':'write8(regs[m]+(word&15),regs[0])',
 'ram[index(regs[m] + (word & 15))]':'read8(regs[m]+(word&15))',
 'ram[index(regs[n])] = regs[m] & 255':'write8(regs[n],regs[m])',
 "struct.unpack_from('<H',ram,index(address))[0]":'read16(address)',
 "struct.unpack_from('<H', ram, index(regs[m]))[0]":'read16(regs[m])',
 "struct.unpack_from('<H', ram, index(regs[m] + (word & 15) * 2))[0]":'read16(regs[m]+(word&15)*2)',
 "struct.pack_into('<H', ram, index(regs[m] + (word & 15) * 2), regs[0] & 65535)":'write16(regs[m]+(word&15)*2,regs[0])',
 "struct.pack_into('<B' if width==1 else '<H',ram,index(regs[n]),value&((1<<(width*8))-1))":'(write8 if width==1 else write16)(regs[n],value)',
 "struct.pack_into('<H', ram, index(regs[n]), regs[m] & 65535)":'write16(regs[n],regs[m])',
}
for old,new in patches.items():assert old in cpu_code,old;cpu_code=cpu_code.replace(old,new)
cpu_ns=dict(cpu.cpu_ns);exec(compile(cpu_code,'hardware_byte_mmio_original_executor','exec'),cpu_ns)
namespace=dict(previous.namespace,ROWS=ROWS,reference=cpu_ns['reference'],RETURNS=previous.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=previous.namespace['LEGACY']-NEW,fifo_address=fifo_address,control_address=control_address)
code=previous.code;marker='queue_reference.scan_cursor,len(queue_reference.scan_samples)';assert code.count(marker)==1;code=code.replace(marker,marker+',queue_reference.fifo_cursor,len(queue_reference.fifo_samples),queue_reference.control_word')
marker='   a,n=access';assert code.count(marker)==1
code=code.replace(marker,marker+"\n   queue_reference.access_width=n\n   if fifo_address(a) or control_address(a):\n    if c.get('null_bus'):stop(pc,r,sr,-2)\n    if fifo_address(a) and n!=4:stop(pc,r,sr,-3)\n    if control_address(a) and (n not in (1,2,4) or a&(n-1) or (a&0x1fffffff)+n>0x00702c04):stop(pc,r,sr,-3)\n    if fifo_address(a) and queue_reference.fifo_cursor>=len(queue_reference.fifo_samples):stop(pc,r,sr,-4)")
exec(compile(code,'hardware_transfer_original_model','exec'),namespace)
effects=types.SimpleNamespace(**previous.effects.__dict__);effects.CONNECTED=previous.effects.CONNECTED|NEW;effects.namespace=namespace;effects.reference_run=namespace['reference_run'];effects.DLL=ROOT/'work/sonic-adventure/build/movie_hardware.dll';effects.LIB=ctypes.CDLL(str(effects.DLL));effects.fn=effects.LIB.sa_movie_hardware_connected;effects.fn.argtypes=previous.effects.fn.argtypes;effects.fn.restype=ctypes.c_int
class HardwareMemory(previous.TimingMemory):_fields_=[('timing_bus',effects.Bus),('fifo_samples',ctypes.POINTER(ctypes.c_uint32)),('fifo_count',ctypes.c_uint32),('fifo_cursor',ctypes.c_uint32),('control_word',ctypes.c_uint32)]
effects.LIB.sa_hardware_transfer_bus_init.argtypes=[ctypes.POINTER(HardwareMemory),ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(effects.Bus),ctypes.c_uint32,ctypes.c_uint32]
effects.LIB.sa_hardware_transfer_set_samples.argtypes=[ctypes.POINTER(HardwareMemory),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32];effects.LIB.sa_hardware_transfer_set_samples.restype=ctypes.c_int
effects.LIB.sa_display_timing_set_samples.argtypes=previous.effects.LIB.sa_display_timing_set_samples.argtypes;effects.LIB.sa_display_timing_set_samples.restype=ctypes.c_int
effects.LIB.sa_disc_queue_size.restype=ctypes.c_uint32;effects.LIB.sa_disc_queue_init.argtypes=previous.effects.LIB.sa_disc_queue_init.argtypes
class HardwareReference(previous.TimingReference):
 def __init__(self,ticks,capacity,c):super().__init__(ticks,capacity,c);self.fifo_samples=[x&0xffffffff for x in c.get('fifo_samples',[0]*256)];self.fifo_cursor=0;self.control_word=c.get('control_word',0)&0xffffffff;self.access_width=4
 def __contains__(self,a):return fifo_address(a) or control_address(a) or super().__contains__(a)
 def __getitem__(self,a):
  if fifo_address(a):
   value=self.fifo_samples[self.fifo_cursor];self.fifo_cursor+=1;self.events.append((22,a,value,4));return value
  if control_address(a):
   n=self.access_width;value=(self.control_word>>((a&3)*8))&((1<<(n*8))-1);self.events.append((23,a,value,n));return value
  return super().__getitem__(a)
 def __setitem__(self,a,value):
  assert not fifo_address(a),'FIFO status input is read-only'
  if control_address(a):
   n=self.access_width;mask=(1<<(n*8))-1;value&=mask;shift=(a&3)*8;self.control_word=(self.control_word&~(mask<<shift))|(value<<shift);self.events.append((24,a,value,n));return
  return super().__setitem__(a,value)
compare_code=previous.compare_code.replace('TimingMemory','HardwareMemory').replace('TimingReference(','HardwareReference(').replace('sa_display_timing_bus_init','sa_hardware_transfer_bus_init')
marker="ctypes.byref(memory),view,ctypes.byref(bus),c.get('port_data',0))";assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker,marker[:-1]+",c.get('control_word',0))\n fifo_values=[x&0xffffffff for x in c.get('fifo_samples',[0]*256)];fifo_storage=(ctypes.c_uint32*len(fifo_values))(*fifo_values);assert effects.LIB.sa_hardware_transfer_set_samples(ctypes.byref(memory),fifo_storage,len(fifo_values))==0")
marker='memory.sample_cursor,memory.sample_count))';assert compare_code.count(marker)==1;compare_code=compare_code.replace(marker,'memory.sample_cursor,memory.sample_count,memory.fifo_cursor,memory.fifo_count,memory.control_word))')
marker=' return dict(scan_cursor=';assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker," assert memory.fifo_cursor==ref.fifo_cursor and memory.fifo_count==len(ref.fifo_samples) and memory.control_word==ref.control_word and list(fifo_storage)==fifo_values\n return dict(fifo_cursor=memory.fifo_cursor,control_word=memory.control_word,fifo_samples_unchanged=True,hardware_events=[list(e) for e in ref.events if e[0] in (22,23,24)],scan_cursor=")
exec(compile(compare_code,'hardware_transfer_comparison','exec'),dict(previous.compare.__globals__,effects=effects,namespace=namespace,HardwareMemory=HardwareMemory,HardwareReference=HardwareReference),local:={});compare=local['compare']
def prepare(ram,c):
 m,s,f=previous.prepare(ram,c)
 if c['entry'] not in NEW:return m,s,f
 store(m,0x8c5fbb14,CTX);m[CTX&0xffffff:(CTX&0xffffff)+0x180]=bytes(0x180);store(m,CTX,CHANNELS);store(m,CTX+0xd4,c.get('mask_interrupts',1));store(m,0x8c7ac8c0,c.get('installed',0));store(m,CTX+0x10c,c.get('read_counter',0));store(m,CTX+0x110,c.get('write_counter',0))
 flags=c.get('channel_flags',[0,1,0,1])
 for i in range(4):store(m,CTX+4+i*52,i);store(m,CTX+4+i*52+0x18,0x12345678);store(m,CHANNELS+i*32+0x18,flags[i]);store(m,CHANNELS+i*32+0x1c,0xa5a50000+i)
 for i in range(256):m[(SRC&0xffffff)-64+i]=(i*53+0x9b)&255;m[(DST&0xffffff)-64+i]=(i*17+0x5a)&255
 store(m,OUT,0xabcdef01)
 if c['entry']==CONTROL:s.r[4]=c.get('value',1)
 elif c['entry'] in (READ,WRITE):
  s.r[4]=c.get('src',SRC);s.r[5]=c.get('dst',DST);s.r[6]=c.get('width',4);s.r[7]=c.get('count',1)&0xffffffff;store(m,s.r[15],c.get('source_stride',1));store(m,s.r[15]+4,c.get('dest_stride',1))
 elif c['entry'] in (CHANNEL_INSTALL,CHANNEL_RESTORE,STATUS):s.r[4]=CTX+4+c.get('channel',1)*52;s.r[5]=OUT
 elif c['entry'] in (READY,FLAG):s.r[4]=OUT
 if c.get('bad_stack'):s.r[15]=0x8cfffffc
 return m,s,f
def cases():
 for mode,word,value in itertools.product((0,4),(0,1,0xffffffff,0x12345678),(0,1,2,0x80000000)):yield dict(entry=CONTROL,mode=mode,control_word=word,value=value)
 for entry,mode,width,count,stride in itertools.product((READ,WRITE),(0,4),(1,2,4,3),(-1,0,1,3,9),(0,1,2)):yield dict(entry=entry,mode=mode,width=width,count=count,source_stride=stride,dest_stride=stride,mask_interrupts=(width+count)&1)
 for entry,mode,width in itertools.product((READ,WRITE),(0,4),(1,2,4)):yield dict(entry=entry,mode=mode,width=width,count=1,source_stride=3,dest_stride=3)
 for entry,mode,flag in itertools.product((INSTALL,WAIT,RESTORE,FIFO_WAIT,FLAG,CHANNEL_RESTORE,CHANNEL_INSTALL,READY,STATUS),(0,4),(0,1,0xffffffff,0x80000000)):yield dict(entry=entry,mode=mode,channel_flags=[flag]*4,fifo_samples=[0x21,0x20,0],read_counter=0xffffffff,write_counter=0xffffffff)
 for entry,width,mode in itertools.product((READ,WRITE),(1,2,4),(0,4)):
  yield dict(entry=entry,mode=mode,width=width,count=1,src=PORT if entry==READ else SRC,dst=DST if entry==READ else PORT,control_word=0xfedcba98)
 for entry,samples in itertools.product((CONTROL,READ,WRITE,WAIT,FIFO_WAIT),([], [0x21], [0x20], [0,0])):yield dict(entry=entry,mode=0,fifo_samples=samples)
 for entry in (FLAG,READY):
  yield dict(entry=entry,mode=0,null_bus=True)
  for status in (-2,-3):yield dict(entry=entry,mode=0,reject_bus=FIFO,bus_status=status)
 for entry in (READ,WRITE):yield dict(entry=entry,mode=0,bad_stack=True)
def semantics(m,a,before,fp,s,f,record,c):
 assert record['fifo_samples_unchanged'] and bytes(fp)==bytes(f)
 if record['status']:return 2
 assert s.r[15]==before.r[15] and s.pr==before.pr and s.r[0]==0
 checks=5
 if c['entry']==CONTROL:
  mask=load(m,0x8c60476c,2);mask=ctypes.c_uint32(ctypes.c_int16(mask).value).value
  assert record['control_word']==((c.get('control_word',0)&mask)|c.get('value',1))&0xffffffff
  assert [e for e in record['hardware_events'] if e[0] in (23,24)]==[[23,PORT,c.get('control_word',0),4],[24,PORT,record['control_word'],4]];checks+=2
 if c['entry'] in (READ,WRITE):
  width=c.get('width',4);count=c.get('count',1);source=c.get('src',SRC);dest=c.get('dst',DST)
  if width in (1,2,4) and source==SRC and dest==DST:
   wanted=bytearray(m);source_step={0:0,1:width,2:-width}.get(c.get('source_stride',1),0);dest_step={0:0,1:width,2:-width}.get(c.get('dest_stride',1),0)
   for i in range(max(0,count)):
    start=(source+i*source_step)&0xffffff;target=(dest+i*dest_step)&0xffffff;wanted[target:target+width]=wanted[start:start+width]
   assert a[(DST&0xffffff)-64:(DST&0xffffff)+192]==wanted[(DST&0xffffff)-64:(DST&0xffffff)+192];checks+=1
  assert s.sr&0xf0==before.sr&0xf0;checks+=1
 return checks
def main():
 proofs=[MP,P/'analysis/movie_hardware_connection.json',P/'analysis/movie_transition_qualification.json']
 for p in proofs:
  for path,h in json.loads(p.read_text())['sha256'].items():assert hashlib.sha256(Path(path).read_bytes()).hexdigest()==h,path
 ram=previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.seed_ram();records=[];coverage=set();edges=set();selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(entry=entry,mode=0) for entry in (CONTROL,READ,WRITE,FLAG,CHANNEL_INSTALL)]+[dict(entry=WAIT,mode=4,fifo_samples=[])]+[dict(entry=entry,mode=0,width=width,count=1,src=PORT if entry==READ else SRC,dst=DST if entry==READ else PORT,control_word=0xfedcba98) for entry,width in itertools.product((READ,WRITE),(1,2,4))]
 for c in selected:
  m,s,f=prepare(ram,c);before=effects.State.from_buffer_copy(s);fp=effects.Fpu.from_buffer_copy(f);record,path,a,s,f=compare(m,s,f,c);record['independent_checks']=semantics(m,a,before,fp,s,f,record,c);records.append(record);coverage.update(path)
  for j,pc in enumerate(path):
   if pc not in ROWS:continue
   text=ROWS[pc];distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((pc,path[j+distance]))
 rows=[]
 for routine in manifest['routines']:
  body=PER_BODY[int(routine['address'],16)];wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']};rows.append(dict(address=routine['address'],static_rows=len(body),covered_rows=len(set(body)&coverage),missing_rows=[hex(x) for x in sorted(set(body)-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)]))
 if '--smoke' in sys.argv:print('Hardware transfer smoke passed',len(records));return
 files=[Path(__file__),Path(previous.__file__),Path(cpu.__file__)]+proofs
 for p in proofs:files.extend(Path(path) for path in json.loads(p.read_text())['sha256'])
 report=dict(source=manifest['source'],routines=manifest['routines'],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),independent_checks=sum(r['independent_checks'] for r in records),coverage_by_routine=rows,newly_registered_entries=0,dll_sha256=hashlib.sha256(effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Twelve original control/copy/query/synchronization bodies with full machine, RAM, FPU, banked/special/interrupt-mask state, boundaries and software platform comparison. FIFO words are borrowed raw input; the 32-bit control register is declared host storage. No game-service result is fabricated.',limitations=['Direct initialized channel tables and software device state are explicit fixtures, not proof of natural driver initialization.','Hardware FIFO progression, physical AICA reset, G2 arbitration, electrical timing, IRQs, audio and pixels are not modeled. Missing/exhausted input stops at the reached original read.','Whole startup, game, ports and requested settings remain unfinished.'],cases=records)
 (P/'analysis/movie_hardware_verification.json').write_text(json.dumps(report,indent=2)+'\n');print({k:report[k] for k in ('cases_passed','status_counts','independent_checks','coverage_by_routine')})
if __name__=='__main__':main()
