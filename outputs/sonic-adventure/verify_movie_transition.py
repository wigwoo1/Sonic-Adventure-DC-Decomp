"""Match the original display wait with raw, caller-owned status observations."""
import collections,csv,ctypes,hashlib,itertools,json,random,sys,types
from pathlib import Path
import verify_movie_status as previous
from verify_overlay_selector_initializer import load,store
P=Path(__file__).resolve().parent;ROOT=P.parents[1];MP=P/'analysis/movie_transition_translation.json';manifest=json.loads(MP.read_text());WAIT,SCAN,MASK=0x8c604594,0xa05f810c,0x1ff;NEW={WAIT};ROWS=dict(previous.ROWS);PER_BODY={}
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);PER_BODY[int(routine['address'],16)]=body
def scan_address(a):return a&0x1fffffff==0x005f810c
namespace=dict(previous.namespace,ROWS=ROWS,LEGACY=previous.namespace['LEGACY']-NEW,SCAN=SCAN,scan_address=scan_address)
code=previous.code;marker='queue_reference.port_data';assert code.count(marker)==1;code=code.replace(marker,marker+',queue_reference.scan_cursor,len(queue_reference.scan_samples)')
marker='   a,n=access';assert code.count(marker)==1
code=code.replace(marker,marker+"\n   if scan_address(a):\n    if c.get('null_bus'):stop(pc,r,sr,-2)\n    if n!=4:stop(pc,r,sr,-3)\n    if queue_reference.scan_cursor>=len(queue_reference.scan_samples):stop(pc,r,sr,-4)")
exec(compile(code,'display_wait_original_model','exec'),namespace)
effects=types.SimpleNamespace(**previous.effects.__dict__);effects.CONNECTED=previous.effects.CONNECTED|NEW;effects.namespace=namespace;effects.reference_run=namespace['reference_run'];effects.DLL=ROOT/'work/sonic-adventure/build/movie_transition.dll';effects.LIB=ctypes.CDLL(str(effects.DLL));effects.fn=effects.LIB.sa_movie_transition_connected;effects.fn.argtypes=previous.effects.fn.argtypes;effects.fn.restype=ctypes.c_int
class TimingMemory(previous.VideoMemory):_fields_=[('video_bus',effects.Bus),('samples',ctypes.POINTER(ctypes.c_uint32)),('sample_count',ctypes.c_uint32),('sample_cursor',ctypes.c_uint32)]
effects.LIB.sa_display_timing_bus_init.argtypes=[ctypes.POINTER(TimingMemory),ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(effects.Bus),ctypes.c_uint32]
effects.LIB.sa_display_timing_set_samples.argtypes=[ctypes.POINTER(TimingMemory),ctypes.POINTER(ctypes.c_uint32),ctypes.c_uint32];effects.LIB.sa_display_timing_set_samples.restype=ctypes.c_int
effects.LIB.sa_disc_queue_size.restype=ctypes.c_uint32;effects.LIB.sa_disc_queue_init.argtypes=previous.effects.LIB.sa_disc_queue_init.argtypes
class TimingReference(previous.VideoReference):
 def __init__(self,ticks,capacity,c):super().__init__(ticks,capacity,c);self.scan_samples=[v&0xffffffff for v in c.get('scan_samples',[])];self.scan_cursor=0
 def __getitem__(self,a):
  if scan_address(a):
   value=self.scan_samples[self.scan_cursor];self.scan_cursor+=1;self.registers[a&0x1fffffff]=value
  return super().__getitem__(a)
 def __setitem__(self,a,value):
  assert not scan_address(a),'Reached SPG_STATUS host interface is read-only'
  return super().__setitem__(a,value)
compare_code=previous.compare_code.replace('VideoMemory','TimingMemory').replace('VideoReference(','TimingReference(').replace('sa_video_signal_bus_init','sa_display_timing_bus_init')
marker="ctypes.byref(memory),view,ctypes.byref(bus),c.get('port_data',0))";assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker,marker+"\n scan_values=[v&0xffffffff for v in c.get('scan_samples',[])];scan_storage=(ctypes.c_uint32*len(scan_values))(*scan_values);assert effects.LIB.sa_display_timing_set_samples(ctypes.byref(memory),scan_storage,len(scan_values))==0")
marker='memory.port_data))';assert compare_code.count(marker)==1;compare_code=compare_code.replace(marker,'memory.port_data,memory.sample_cursor,memory.sample_count))')
marker=' return dict(port_data=';assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker," assert memory.sample_cursor==ref.scan_cursor and memory.sample_count==len(ref.scan_samples) and list(scan_storage)==scan_values\n return dict(scan_cursor=memory.sample_cursor,scan_samples_unchanged=True,scan_reads=[list(e) for e in ref.events if e[0]==8 and scan_address(e[1])],port_data=")
exec(compile(compare_code,'display_wait_comparison','exec'),dict(previous.compare.__globals__,effects=effects,namespace=namespace,TimingMemory=TimingMemory,TimingReference=TimingReference,scan_address=scan_address),local:={});compare=local['compare']
def prepare(ram,c):return previous.prepare(ram,c)
def cases():
 rng=random.Random(WAIT)
 for mode,leading,trailing in itertools.product((0,4),range(4),range(4)):
  clear=rng.getrandbits(32)&~MASK;busy=rng.getrandbits(32)|1
  yield dict(entry=WAIT,mode=mode,scan_samples=[busy]*leading+[clear]*(trailing+1)+[busy,0x12345678])
 for bit,mode in itertools.product(range(9,32),(0,4)):yield dict(entry=WAIT,mode=mode,scan_samples=[1<<bit,1,0,1])
 for imask,t in itertools.product((0,1,7,15),(0,1)):yield dict(entry=WAIT,mode=0,sr=0x60000302|(imask<<4)|t,scan_samples=[0xffffffff,0x400,0x80000000,0x1ff])
 for mode,samples in itertools.product((0,4),([], [1], [0], [1,1], [0,0], [0x400], [0x80000000,0])):yield dict(entry=WAIT,mode=mode,scan_samples=samples)
 yield dict(entry=WAIT,mode=0,scan_samples=[0,1],null_bus=True)
 for status in (-2,-3):yield dict(entry=WAIT,mode=0,scan_samples=[0,1],reject_bus=SCAN,bus_status=status)
def semantics(m,a,before,fp,s,f,record,c):
 assert a==m and bytes(f)==bytes(fp) and s.pr==before.pr
 assert s.r[4]==SCAN and s.r[5]==MASK and all(s.r[i]==before.r[i] for i in range(16) if i not in (2,3,4,5))
 assert not record['disc_read_attempts'] and not record['clock_reads'] and record['scan_samples_unchanged']
 if record['status']:
  if record['status']==-4:assert record['scan_cursor']==len(c['scan_samples'])
  return 5
 samples=[v&0xffffffff for v in c['scan_samples']];i=0
 while samples[i]&MASK:i+=1
 assert s.r[2]==samples[i];i+=1
 while not samples[i]&MASK:i+=1
 assert s.r[3]==samples[i] and record['scan_cursor']==i+1 and s.sr==before.sr&~1
 assert record['scan_reads']==[[8,SCAN,value,4] for value in samples[:i+1]];return 10
def main():
 proofs=[MP,P/'analysis/movie_transition_connection.json',P/'analysis/movie_status_qualification.json']
 for p in proofs:
  for path,h in json.loads(p.read_text())['sha256'].items():assert hashlib.sha256(Path(path).read_bytes()).hexdigest()==h,path
 ram=previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.seed_ram();records=[];coverage=set();edges=set();selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(entry=WAIT,mode=0,scan_samples=[0x400,1,0,1]),dict(entry=WAIT,mode=4,scan_samples=[1,0,0,2]),dict(entry=WAIT,mode=0,scan_samples=[])]
 for c in selected:
  m,s,f=prepare(ram,c);before=effects.State.from_buffer_copy(s);fp=effects.Fpu.from_buffer_copy(f);record,path,a,s,f=compare(m,s,f,c);record['independent_checks']=semantics(m,a,before,fp,s,f,record,c);records.append(record);coverage.update(path)
  for j,pc in enumerate(path[:-1]):
   if pc in (0x8c60459c,0x8c6045a2):edges.add((pc,path[j+1]))
 if '--smoke' in sys.argv:print('Display wait smoke passed',len(records));return
 rows=[]
 for routine in manifest['routines']:
  body=PER_BODY[int(routine['address'],16)];wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']};rows.append(dict(address=routine['address'],static_rows=len(body),covered_rows=len(set(body)&coverage),missing_rows=[hex(x) for x in sorted(set(body)-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)]))
 assert all(not row['missing_rows'] and not row['missing_edges'] for row in rows)
 files=[Path(__file__),Path(previous.__file__)]+proofs
 for p in proofs:files.extend(Path(path) for path in json.loads(p.read_text())['sha256'])
 report=dict(source=manifest['source'],routines=manifest['routines'],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),independent_checks=sum(r['independent_checks'] for r in records),coverage_by_routine=rows,newly_registered_entries=0,dll_sha256=hashlib.sha256(effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Ten original instructions, four branch outcomes and the actual 01ff literal. Complete machine, RAM, FPU, stack, boundaries and software platform state match independent instruction execution. Borrowed raw SPG_STATUS samples retain all 32 bits; code itself tests the nine-bit field. Exhaustion/rejection preserve reached partial state.',limitations=['Explicit caller-owned status observations are input fixtures, not physical scan generation, display synchronization, timing accuracy, IRQs or pixels.','Privileged MMU-off, immutable code/literals and prior software platform fixtures remain preconditions. Full startup and game/ports/settings remain incomplete.','The preserved legacy packet_mmio.c helper uses an overbroad ff0f01ff mask; its old selected test sequences did not distinguish it from the actual 01ff literal. The new native path uses original bytes and does not call that helper.'],cases=records)
 (P/'analysis/movie_transition_verification.json').write_text(json.dumps(report,indent=2)+'\n');print({k:report[k] for k in ('cases_passed','status_counts','independent_checks','coverage_by_routine')})
if __name__=='__main__':main()
