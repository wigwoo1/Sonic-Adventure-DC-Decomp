"""Boot setup, native task disposal, real register effects and honest caller stops."""
import collections,csv,ctypes,hashlib,itertools,json,sys,types
from pathlib import Path
import verify_draw_frame_context as frame
import verify_draw_view_registers as registers
import verify_draw_geometry_stream as geometry
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent
MANIFEST=PROJECT/'analysis/boot_setup_translation.json';manifest=json.loads(MANIFEST.read_text())
RESET,SET,WRAPPER,DISPOSE,BOOT=0x8c107aaa,0x8c605c10,0x8c652f56,0x8c09865c,0x8c10b840
TASK,AFTER,BEFORE,PARENT,FREE_HEAD,ATTACH=0x8cc10000,0x8cc20000,0x8cc20100,0x8cc30000,0x8cc40000,0x8cc50000
FREE,LEVELS,FLAG=0x8c79f048,0x8c79f04c,0x8c5fbabc
ROWS=dict(frame.ROWS);OWN={};PER_BODY={};cleanup_listings=[]
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')}
 ROWS.update(body);OWN.update(body);PER_BODY[int(routine['address'],16)]=body
for address in (DISPOSE,0x8c098998,0x8c098e60,0x8c098e98,0x8c098b42):
 listing=PROJECT/f'analysis/main-dispatch-task-disposal/{address:08x}_instructions.tsv';cleanup_listings.append(listing)
 ROWS.update({int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(listing.open(),delimiter='\t')})
namespace=dict(frame.namespace,ROWS=ROWS,RETURNS=frame.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))})
exec(compile(registers.code,'boot_setup_original_model','exec'),namespace)
effects=types.SimpleNamespace(**frame.effects.__dict__)
effects.CONNECTED=frame.effects.CONNECTED|{RESET,SET,WRAPPER};effects.namespace=namespace;effects.reference_run=namespace['reference_run']
effects.DLL=PROJECT.parents[1]/'work/sonic-adventure/build/boot_setup.dll';effects.LIB=ctypes.CDLL(str(effects.DLL))
native_fn=effects.LIB.sa_boot_setup_connected;native_fn.argtypes=frame.effects.fn.argtypes;native_fn.restype=ctypes.c_int
effects.LIB.sa_render_register_bus_init.argtypes=frame.effects.LIB.sa_render_register_bus_init.argtypes
boundaries=[]
def invoke(*args):
 original_observer=args[7]
 @effects.OBSERVE
 def capture(ctx,target,m,f,sr,r,pr):
  boundaries.append(dict(target=hex(target),pr=hex(pr[0]),stack=hex(r[15]),arguments=list(r[4:8])))
  original_observer(ctx,target,m,f,sr,r,pr)
 copied=list(args);copied[7]=capture
 return native_fn(*copied)
effects.fn=invoke
compare_state=types.FunctionType(registers.compare.__code__,dict(registers.__dict__,namespace=namespace,effects=effects),'compare_state')

def digest(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()

def prepare(ram,c):
 m,s,f=frame.previous.prepare(ram,c)
 for node in (TASK,AFTER,BEFORE,PARENT,FREE_HEAD):
  for i in range(12):store(m,node+4*i,0)
 nxt=AFTER if c.get('next') else 0;prev=BEFORE if c.get('previous') else 0;parent=PARENT if c.get('parent') else 0
 for i,value in enumerate((nxt,prev,parent,0,0x12345678,0x87654321,0,ATTACH if c.get('attachment') else 0,0,0,0,0)):store(m,TASK+4*i,value)
 store(m,AFTER+4,TASK);store(m,BEFORE,TASK);store(m,PARENT+12,TASK)
 store(m,FREE,FREE_HEAD if c.get('free_head',True) else 0)
 for i in range(8):store(m,LEVELS+i*4,TASK if i==c.get('level',0) else 0)
 store(m,ATTACH,0xa5ff1234);store(m,FLAG,TASK if c.get('active') else 0)
 s.r[4]=c.get('value',0x12345678)
 if c['entry']==WRAPPER:store(m,s.r[15],c.get('value',0))
 if c.get('bad_stack') is not None:s.r[15]=c['bad_stack']
 return m,s,f

def cleanup_checks(m,actual,c):
 assert load(actual,FLAG)==0,'global task pointer cleared after disposal'
 if not c.get('active'):
  assert actual[TASK&0xffffff:(TASK&0xffffff)+48]==m[TASK&0xffffff:(TASK&0xffffff)+48]
  assert load(actual,FREE)==load(m,FREE)
  return 3
 assert load(actual,FREE)==TASK
 assert [load(actual,TASK+i*4) for i in (0,1,2,3)]==[load(m,FREE),0,0,0]
 assert all(load(actual,TASK+i*4)==0 for i in (4,5,6,7,8,9,10,11))
 if c.get('free_head',True):assert load(actual,FREE_HEAD+4)==TASK
 nxt=AFTER if c.get('next') else 0
 if c.get('previous'):
  assert load(actual,BEFORE)==nxt
  if nxt:assert load(actual,AFTER+4)==BEFORE
 elif c.get('parent'):
  assert load(actual,PARENT+12)==nxt
  if nxt:assert load(actual,AFTER+4)==0
 elif c.get('level',0)<8:
  assert load(actual,LEVELS+c.get('level',0)*4)==nxt
  if nxt:assert load(actual,AFTER+4)==0
 assert load(actual,ATTACH)==(0xa5fe1234 if c.get('attachment') else 0xa5ff1234)
 callback=[b for b in boundaries if b['target']==hex(DISPOSE)]
 assert len(callback)==1 and callback[0]['pr']=='0x8c107aba' and callback[0]['arguments'][0]==TASK
 return 8

def semantic(m,actual,initial,final,record,c):
 if record['status']!=0:return 0
 assert final.r[15]==initial.r[15] and final.pr==initial.pr
 if initial.pc==RESET:
  assert not record['register_writes']
  return cleanup_checks(m,actual,c)+1
 wanted=bytearray(m);sp=initial.r[15];value=c.get('value',0)
 if initial.pc==SET:
  for delta,word in ((4,initial.pr),(8,value),(12,value),(16,0x8c605c20)):store(wanted,sp-delta,word)
 else:store(wanted,sp-4,initial.pr)
 assert actual==wanted,'independent full stack/RAM footprint'
 assert record['register_writes']==[[7,0xa05f8040,value,4]] and record['queue_events']==1
 assert final.r[0]==0 and final.r[4]==0xa05f8040 and final.r[5]==value
 return 4

def cases():
 for bits,level,bank in itertools.product(range(8),(0,7,8),range(2)):
  yield dict(entry=RESET,active=True,next=bool(bits&1),previous=bool(bits&2),parent=bool(bits&4),level=level,mode=bank*4,attachment=bool(bits&1),free_head=bool(bits&2))
 for bank,alias in itertools.product(range(2),(0x8c000000,0x0c000000,0xac000000)):
  yield dict(entry=RESET,active=False,mode=bank*4,alias=alias)
 for mode,value in itertools.product(range(8),(0,1,0xffffffff,0x12345678)):yield dict(entry=SET,mode=mode,value=value)
 for bank,alias in itertools.product(range(2),(0x0c000000,0xac000000)):yield dict(entry=SET,mode=bank*4,alias=alias,value=0xdeadbeef)
 for bank,value in itertools.product(range(2),(0,0xffffffff,0x87654321)):yield dict(entry=WRAPPER,mode=bank*4,value=value)
 for entry,stack in itertools.product((RESET,SET,WRAPPER),(0,0x8d000004)):yield dict(entry=entry,mode=0,bad_stack=stack)
 for bank,failure in itertools.product(range(2),('null_bus','reject_register')):yield dict(entry=SET,mode=bank*4,value=0xdeadbeef,**{failure:True})

def main():
 proofs=[MANIFEST,PROJECT/'analysis/boot_setup_connection.json',PROJECT/'analysis/draw_frame_context_connection.json']
 for p in proofs:
  for file,hash_value in json.loads(p.read_text())['sha256'].items():assert digest(file)==hash_value,file
 ram=bytearray(16<<20)
 for p,offset in ((effects.SEED.state.gate.support.morph.MAIN,0x10000),(effects.SEED.state.gate.support.morph.MODULE,0x900000)):
  raw=p.read_bytes();ram[offset:offset+len(raw)]=raw
 selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(entry=RESET,active=False,mode=0),dict(entry=RESET,active=True,mode=0),dict(entry=SET,mode=0,value=0x12345678),dict(entry=WRAPPER,mode=0,value=1)]
 records=[];coverage=set();edges=set();cleanup_prs=set()
 for i,c in enumerate(selected):
  m,s,f=prepare(ram,c);initial=effects.State.from_buffer_copy(s);boundaries.clear()
  record,path,actual,final,*_=compare_state(m,s,f,c)
  record['independent_checks']=semantic(m,actual,initial,final,record,c);records.append(record);coverage.update(path)
  cleanup_prs.update(b['pr'] for b in boundaries if b['target']==hex(DISPOSE))
  for j,a in enumerate(path[:-1]):
   if a not in OWN:continue
   word=int.from_bytes(ram[a&0xffffff:(a&0xffffff)+2],'little')
   if word>>8 in (0x89,0x8b):edges.add((a,path[j+1]))
  if (i+1)%32==0:print(i+1,'boot setup comparisons passed',flush=True)
 prefix=[];parent_coverage=set()
 for bank,active,alias in itertools.product(range(2),(False,True),(0x8c000000,0x0c000000,0xac000000)):
  c=dict(entry=BOOT,mode=bank*4,active=active,alias=alias,value=0xabcdef01,level=7,attachment=True)
  m,s,f=prepare(ram,c);boundaries.clear();r,path,actual,*_=compare_state(m,s,f,c)
  assert r['status']==-2 and r['stop_pc']=='0x8c6050b0',(c,r['status'],r['stop_pc'])
  r['independent_checks']=cleanup_checks(m,actual,c)
  assert r['register_writes']==[[7,0xa05f8040,0,4]],r['register_writes']
  r['independent_checks']+=1;prefix.append(r);parent_coverage.update(set(path)&set(geometry.PER_BODY[BOOT]))
  print('Boot prefix',bank,active,hex(alias),r['status'],r['stop_pc'],flush=True)
 if '--smoke' in sys.argv:print('Boot setup smoke passed:',len(records),'direct +',len(prefix),'prefix');return
 coverage_rows=[]
 for a,body in PER_BODY.items():
  required=next(r for r in manifest['routines'] if int(r['address'],16)==a)
  wanted={tuple(int(x,16) for x in edge) for edge in required['conditional_edges']}
  coverage_rows.append(dict(address=hex(a),static_rows=len(body),covered_rows=len(set(body)&coverage),missing_rows=[hex(x) for x in sorted(set(body)-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(x),hex(y)] for x,y in sorted(wanted-edges)]))
 assert all(not r['missing_rows'] and not r['missing_edges'] for r in coverage_rows)
 assert cleanup_prs=={'0x8c107aba'}
 assert len(records)==106 and collections.Counter(r['status'] for r in records)=={0:96,-3:8,-2:2}
 assert len(prefix)==12 and len(parent_coverage)==16
 files=[Path(__file__),PROJECT/'verify_draw_frame_context.py',PROJECT/'verify_draw_view_registers.py',PROJECT/'verify_startup.py',PROJECT/'analysis/main_dispatch_task_disposal_verification.json']+cleanup_listings+proofs
 for p in proofs:files.extend(Path(file) for file in json.loads(p.read_text())['sha256'])
 report=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=coverage_rows,instruction_rows_covered=len(set(OWN)&coverage),independent_checks=sum(r['independent_checks'] for r in records),callback_prs=sorted(cleanup_prs),boundary_comparisons=sum(r['boundary_comparisons'] for r in records),prefix_cases=len(prefix),prefix_status_counts=dict(collections.Counter(r['status'] for r in prefix)),prefix_stops=dict(collections.Counter(r['stop_pc'] for r in prefix)),parent_rows_covered=len(parent_coverage),parent_static_rows=len(geometry.PER_BODY[BOOT]),parent_recovery_credit=0,external_game_service_fixtures=0,dll_sha256=digest(effects.DLL),sha256={str(p):digest(p) for p in files},scope='Three original setup bodies, native task deletion for valid resource-free task records, exact register address/value and complete stack footprints. All reached/final RAM/CPU/FPU/banks/MACH/MACL/GBR, register map and ordered native boundaries match the original instruction model. Boot caller remains partial.',limitations=['Synthetic valid task/list fixtures cover native task deletion with no destructor, child, work or heap payload. Arbitrary task graphs/resources are outside these direct cases.','Cold boot prefixes start from loaded MAIN/AL_MAIN in zero-initialized RAM. They do not prove natural boot prerequisites or startup/scheduler order.','Register effects are bounded host storage, not GPU command consumption, timing or pixels. Typed invalid-memory and missing/rejected bus stops do not implement full hardware exception delivery.','Qualified MMU-off RAM and processor/numerical domains inherited from the frame pipeline; whole gameplay, settings and ports remain unfinished.'],cases=records,prefix=prefix)
 (PROJECT/'analysis/boot_setup_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','instruction_rows_covered','callback_prs','prefix_cases','prefix_stops','parent_rows_covered','parent_static_rows')},indent=2))

if __name__=='__main__':main()
