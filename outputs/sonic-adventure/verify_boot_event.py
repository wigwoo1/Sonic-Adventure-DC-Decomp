"""Original event registry/callback code, explicit VBR and mask storage."""
import collections,csv,ctypes,hashlib,itertools,json,random,sys,types
from pathlib import Path
import verify_boot_input as previous
import verify_draw_clipping_complete as cpu
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent;MANIFEST=PROJECT/'analysis/boot_event_translation.json';manifest=json.loads(MANIFEST.read_text())
REPLACE,REGISTER,REMOVE,PRIMARY,SECONDARY,ENABLE_A,ENABLE_B,INSTALL,CLEAR=0x8c605234,0x8c64594c,0x8c645c54,0x8c64135a,0x8c64137c,0x8c658c20,0x8c658c24,0x8c6412c2,0x8c6411b0
TABLE,HEADS,HANDLE,DRIVER=0x8c892840,0x8c893840,0x8c8900fc,0x8ce40000
MASKS=(0xa05f6920,0xa05f6924);NEW={int(r['address'],16) for r in manifest['routines']};ROWS=dict(previous.ROWS);OWN={};PER_BODY={}
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);OWN.update(body);PER_BODY[int(routine['address'],16)]=body
assert previous.namespace['reference'] is cpu.cpu_ns['reference']
marker='        elif word == 9: pass';assert cpu.cpu_code.count(marker)==1
cpu_code=cpu.cpu_code.replace(marker,"        elif word & 0xF0FF == 0x0022:regs[n]=special['vbr']\n"+marker)
cpu_ns=dict(cpu.cpu_ns);exec(compile(cpu_code,'event_original_vbr_executor','exec'),cpu_ns)
code=previous.previous.code
marker="'bank0_live_in':[]}";assert code.count(marker)==1;code=code.replace(marker,"'bank0_live_in':[],'vbr':c.get('vbr',0x8ce50000)}")
marker='hashlib.sha256(queue_reference.vram).digest(),queue_reference.poll_cursor';assert code.count(marker)==1;code=code.replace(marker,marker+",tuple(queue_reference.masks),special['vbr']")
marker='  access=dynamic_access(text,r,f.fpscr,special)';assert code.count(marker)==1;code=code.replace(marker,"  if w&0xf0ff==0x0022 and not sr&0x40000000:stop(pc,r,sr,-4)\n"+marker)
marker='   a,n=access';assert code.count(marker)==1;code=code.replace(marker,marker+"\n   if a&0x1fffffff in (0x005f6920,0x005f6924) and c.get('null_bus'):stop(pc,r,sr,-2)")
namespace=dict(previous.namespace,ROWS=ROWS,reference=cpu_ns['reference'],RETURNS=previous.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=previous.namespace['LEGACY']-NEW)
exec(compile(code,'event_original_model','exec'),namespace)
effects=types.SimpleNamespace(**previous.effects.__dict__);effects.CONNECTED=previous.effects.CONNECTED|NEW;effects.namespace=namespace;effects.reference_run=namespace['reference_run']
effects.DLL=PROJECT.parents[1]/'work/sonic-adventure/build/boot_event.dll';effects.LIB=ctypes.CDLL(str(effects.DLL));effects.fn=effects.LIB.sa_boot_event_connected;effects.fn.argtypes=previous.effects.fn.argtypes+[ctypes.POINTER(ctypes.c_uint32)];effects.fn.restype=ctypes.c_int
class EventMemory(previous.previous.InterruptMemory):_fields_=[('interrupt_bus',effects.Bus),('masks',ctypes.c_uint32*2)]
effects.LIB.sa_event_bus_init.argtypes=[ctypes.POINTER(EventMemory),ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(effects.Bus)]
def mask_index(a):return ((a&0x1fffffff)-0x005f6920)//4 if a&0x1fffffff in (0x005f6920,0x005f6924) else None
class EventReference(previous.previous.InterruptReference):
 def __init__(self,ticks,capacity,c):super().__init__(ticks,capacity,c);self.masks=list(c.get('masks',[0x1234567,0x89abcdef]))
 def __contains__(self,a):return mask_index(a) is not None or super().__contains__(a)
 def __getitem__(self,a):
  i=mask_index(a)
  if i is None:return super().__getitem__(a)
  value=self.masks[i];self.events.append((17,a,value,4));return value
 def __setitem__(self,a,value):
  i=mask_index(a)
  if i is None:return super().__setitem__(a,value)
  self.masks[i]=value&0xffffffff;self.events.append((16,a,value&0xffffffff,4))
compare_code=previous.previous.compare_code.replace('InterruptMemory','EventMemory').replace('InterruptReference','EventReference').replace('sa_interrupt_bus_init','sa_event_bus_init')
marker="memory.normal_status=c.get('irq_status',8);";assert compare_code.count(marker)==1;compare_code=compare_code.replace(marker,marker+"memory.masks[:]=c.get('masks',[0x1234567,0x89abcdef]);")
marker='ml=ctypes.c_uint32(0x12345678)';assert compare_code.count(marker)==1;compare_code=compare_code.replace(marker,"vb=ctypes.c_uint32(c.get('vbr',0x8ce50000));"+marker)
marker='len(poll_reads)))';assert compare_code.count(marker)==1;compare_code=compare_code.replace(marker,'len(poll_reads),tuple(memory.masks),vb.value))')
marker='ctypes.byref(extra))';assert compare_code.count(marker)==1;compare_code=compare_code.replace(marker,'ctypes.byref(extra),ctypes.byref(vb))')
marker=' return dict(irq_status=';assert compare_code.count(marker)==1;compare_code=compare_code.replace(marker," assert list(memory.masks)==ref.masks and vb.value==c.get('vbr',0x8ce50000)\n return dict(vbr=vb.value,masks=list(memory.masks),mask_writes=[list(e) for e in ref.events if e[0]==16],irq_status=")
exec(compile(compare_code,'event_comparison','exec'),dict(previous.compare.__globals__,effects=effects,namespace=namespace,EventMemory=EventMemory,EventReference=EventReference),local:={});compare=local['compare']
def node(index):return TABLE+index*16
def event_head(event):return (HEADS+((ctypes.c_int16(event&65535).value>>4)*4))&0xffffffff

def prepare(ram,c):
 m,s,f=previous.prepare(ram,c);s.pc=c['entry'];s.sr=c.get('sr',s.sr)
 m[0x892840:0x893b40]=bytes(0x1300);m[0xe40000:0xe40400]=bytes(0x400);store(m,0x8c67c004,DRIVER);store(m,HANDLE,c.get('existing',0))
 event=c.get('event',0x900);chain=c.get('chain',[])
 for index,priority in enumerate(chain,1):
  store(m,node(index),0x8c651bc0);store(m,node(index)+4,node(index+1) if index<len(chain) else 0);store(m,node(index)+8,index*0x101);store(m,node(index)+12,priority|0x80000000)
 if chain:store(m,event_head(event),node(1))
 if c.get('pool_full'):
  for index in range(1,256):store(m,node(index)+12,0x80000000)
 callback=c.get('callback',0x8c651bc0);argument=c.get('argument',0x12345678)
 if c['entry']==REPLACE:s.r[4]=callback
 elif c['entry']==REGISTER:s.r[4]=event&0xffffffff;s.r[5]=callback;s.r[6]=c.get('priority',0x58);s.r[7]=argument
 elif c['entry']==REMOVE:s.r[4]=c.get('handle',1)
 elif c['entry'] in (PRIMARY,SECONDARY):s.r[4]=callback;s.r[5]=argument
 elif c['entry'] in (INSTALL,CLEAR):s.r[4]=DRIVER;s.r[5]=c.get('index',1);s.r[6]=callback;s.r[7]=argument
 else:s.r[4]=c.get('value',1)
 if c.get('bad_stack') is not None:s.r[15]=c['bad_stack']
 return m,s,f

def semantic(m,a,b,fp,s,f,record,c):
 if record['status']!=0:return 0
 assert previous.previous.retained(b,s) and bytes(fp)==bytes(f)
 entry=c['entry'];event=c.get('event',0x900);index=c.get('index',1);callback=c.get('callback',0x8c651bc0);argument=c.get('argument',0x12345678)
 if entry in (ENABLE_A,ENABLE_B):assert s.r[0]==1 and a==m;return 3
 if entry in (PRIMARY,SECONDARY,INSTALL,CLEAR):
  if entry==PRIMARY:index=1
  elif entry==SECONDARY:index=2
  slot=DRIVER+0x230+index*8
  defaults=(0x8c641698,0x8c64169c,0x8c6416a0,0x8c6416a4,0x8c6416e0,0x8c6416e4,0x8c6416e8,0x8c6416ec,0x8c6416f0)
  if entry==CLEAR or not callback:
   if 0<=index<9:assert load(a,slot)==defaults[index]
   else:assert a[DRIVER&0xffffff:(DRIVER&0xffffff)+0x400]==m[DRIVER&0xffffff:(DRIVER&0xffffff)+0x400]
  else:assert load(a,slot)==callback and load(a,slot+4)==argument
  assert s.sr&0xf0==b.sr&0xf0;return 3
 if entry==REGISTER:
  if c.get('pool_full'):assert s.r[0]==0 and a[0x892840:0x893b40]==m[0x892840:0x893b40];return 3
  chosen=len(c.get('chain',[]))+1;priority=c.get('priority',0x58)|0x80000000
  assert s.r[0]==chosen and load(a,node(chosen))==callback and load(a,node(chosen)+8)==argument and load(a,node(chosen)+12)==priority
  order=list(range(1,chosen));position=next((i for i,p in enumerate(c.get('chain',[])) if (p|0x80000000)>=priority),len(order));order.insert(position,chosen)
  current=load(a,event_head(event));observed=[]
  while current:observed.append((current-TABLE)//16);current=load(a,current+4);assert len(observed)<=chosen
  assert observed==order and s.sr&0xf0==b.sr&0xf0
  if not c.get('chain'):
   group=ctypes.c_int16(event&65535).value>>8;bit=(event&255)>>5;expected=list(c.get('masks',[0x1234567,0x89abcdef]))
   if group==10:expected[0]|=1<<((bit+12)&31)
   if group==11:expected[1]|=1<<(bit&31)
   assert record['masks']==expected
  return 5
 if entry==REMOVE:
  chain=c.get('chain',[]);target=c.get('handle',1);expected=[i for i in range(1,len(chain)+1) if i!=target];observed=[];current=load(a,event_head(event))
  while current:observed.append((current-TABLE)//16);current=load(a,current+4);assert len(observed)<=len(chain)
  assert observed==expected and s.r[0]==s.sr and s.sr&0xf0==b.sr&0xf0
  if target<=len(chain):assert load(a,node(target)+12)==0
  return 4
 if entry==REPLACE:
  expected=0 if not callback or c.get('pool_full') and not c.get('existing') else 1
  assert load(a,HANDLE)==expected and s.sr&0xf0==b.sr&0xf0
  if expected:assert load(a,node(1))==callback and load(a,node(1)+12)==0x80000058 and load(a,event_head(0x900))==node(1)
  return 3
 return 1

def cases():
 for existing,callback,full,mode in itertools.product((0,1),(0,0x8c651bc0),(0,1),(0,4)):
  yield dict(entry=REPLACE,mode=mode,existing=existing,callback=callback,pool_full=full,chain=[0x58] if existing else [])
 for event,chain,priority,mode in itertools.product((0x200,0x900,0x920,0x940,0xa00,0xae0,0xb00,0xbe0,0xfff0),([], [10,30]),(0,20,50),(0,4)):
  yield dict(entry=REGISTER,mode=mode,event=event,chain=chain,priority=priority)
 for event in (0x200,0x900,0x920,0xa00,0xb00):yield dict(entry=REGISTER,mode=0,event=event,pool_full=1)
 for event,chain,handle,mode in itertools.product((0x200,0x900,0x920,0x940,0xa00,0xae0,0xb00,0xbe0),([], [10],[10,20,30]),(1,2,3,255),(0,4)):
  yield dict(entry=REMOVE,mode=mode,event=event,chain=chain,handle=handle)
 for entry,index,callback,mode in itertools.product((INSTALL,CLEAR),(0,1,2,3,4,5,6,7,8,9,12),(0,0x8c651bc0),(0,4)):
  yield dict(entry=entry,mode=mode,index=index,callback=callback)
 for entry,callback,mode in itertools.product((PRIMARY,SECONDARY),(0,0x8c651bc0),range(8)):yield dict(entry=entry,mode=mode,callback=callback)
 for entry,value,mode in itertools.product((ENABLE_A,ENABLE_B),(0,1,0xffffffff),range(8)):yield dict(entry=entry,mode=mode,value=value)
 for imask,t,entry in itertools.product((0,1,7,15),(0,1),(REGISTER,REMOVE,INSTALL)):
  yield dict(entry=entry,mode=0,sr=0x60000302|(imask<<4)|t,event=0x200,chain=[10] if entry==REMOVE else [])
 for alias in (0x8c000000,0x0c000000,0xac000000):yield dict(entry=REGISTER,mode=0,event=0x200,vbr=alias|0xe50000)
 yield dict(entry=REGISTER,mode=0,event=0x200,vbr=0)
 for entry in (REPLACE,REGISTER,REMOVE,INSTALL,PRIMARY,SECONDARY):
  for stack in (0,0x8d000004):yield dict(entry=entry,mode=0,bad_stack=stack)
 for entry in (REGISTER,REMOVE,INSTALL):yield dict(entry=entry,mode=0,sr=0x200003f3)
 for entry,event in itertools.product((REGISTER,REMOVE),(0xa00,0xb00)):
  yield dict(entry=entry,mode=0,event=event,chain=[10] if entry==REMOVE else [],reject_bus=MASKS[event==0xb00],bus_status=-3)
  yield dict(entry=entry,mode=0,event=event,chain=[10] if entry==REMOVE else [],null_bus=True)

def main():
 proofs=[MANIFEST,PROJECT/'analysis/boot_event_connection.json',PROJECT/'analysis/boot_input_qualification.json']
 for p in proofs:
  for path,h in json.loads(p.read_text())['sha256'].items():assert hashlib.sha256(Path(path).read_bytes()).hexdigest()==h,path
 ram=previous.previous.previous.previous.seed_ram();records=[];coverage=set();edges=set();selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(entry=e,mode=0,chain=[10] if e==REMOVE else []) for e in (REPLACE,REGISTER,REMOVE,PRIMARY,SECONDARY,ENABLE_A,ENABLE_B,INSTALL,CLEAR)]
 for i,c in enumerate(selected):
  m,s,f=prepare(ram,c);before=effects.State.from_buffer_copy(s);fp=effects.Fpu.from_buffer_copy(f);record,path,a,s,f=compare(m,s,f,c)
  try:record['independent_checks']=semantic(m,a,before,fp,s,f,record,c)
  except AssertionError as error:raise AssertionError((c,'independent event semantics',str(error))) from error
  records.append(record);coverage.update(path)
  if '--smoke' in sys.argv:print(hex(c['entry']),record['status'],record['stop_pc'],len(path),flush=True)
  for j,pc in enumerate(path):
   if pc not in OWN:continue
   w=int.from_bytes(ram[pc&0xffffff:(pc&0xffffff)+2],'little');distance=2 if w>>8 in (0x8d,0x8f) else 1
   if w>>8 in (0x89,0x8b,0x8d,0x8f) and j+distance<len(path):edges.add((pc,path[j+distance]))
  if (i+1)%128==0:print(i+1,'event comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Event smoke passed:',len(records));return
 rows=[]
 for routine in manifest['routines']:
  a=int(routine['address'],16);body=PER_BODY[a];wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
  rows.append(dict(address=hex(a),static_rows=len(body),covered_rows=len(set(body)&coverage),missing_rows=[hex(x) for x in sorted(set(body)-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(x),hex(y)] for x,y in sorted(wanted-edges)]))
 files=[Path(__file__),PROJECT/'verify_draw_clipping_complete.py',PROJECT/'verify_boot_input.py']+proofs
 for p in proofs:files.extend(Path(path) for path in json.loads(p.read_text())['sha256'])
 report=dict(source=manifest['source'],source_sha256=hashlib.sha256((PROJECT/manifest['source']).read_bytes()).hexdigest(),routines=manifest['routines'],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=rows,independent_checks=sum(r['independent_checks'] for r in records),mask_register_writes=sum(len(r['mask_writes']) for r in records),external_game_service_fixtures=0,newly_registered_entries=0,dll_sha256=hashlib.sha256(effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Original event replacement and existing registry/callback bodies. Complete machine/RAM/FPU/banks/MACH/MACL/GBR/VBR, reached boundaries, two event mask words and inherited device state match. No handler delivery or game-service result is supplied.',limitations=['Event pools/chains, driver callback tables, VBR and software masks are explicit initial-state fixtures. Natural initialization, IRQ acceptance/vectoring, exception delivery, controller hardware, DMA/GPU pixels and physical timing remain unproved.','Sorted acyclic event lists and immutable code/literals, distinct valid RAM/stack/data, qualified privileged CPU/MMU-off and inherited numeric/device domains apply.','VBR is an explicit additional readonly register input in the separate pipeline ABI. Existing source/APIs/proofs remain unchanged.','Whole-game recovery, gameplay, ports/demakes and requested display/texture features remain unfinished.'],cases=records)
 (PROJECT/'analysis/boot_event_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','independent_checks','mask_register_writes','coverage_by_routine')},indent=2))

if __name__=='__main__':main()
