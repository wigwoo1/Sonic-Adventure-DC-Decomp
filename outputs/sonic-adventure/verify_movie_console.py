"""Original game queries using explicit native console/flash input spans."""
import collections,ctypes,hashlib,itertools,json,sys,types
from pathlib import Path
import verify_movie_initialize as old
import console_service_reference as console_model
P=old.P;ROOT=old.ROOT;load=old.load;store=old.store;ROWS=old.ROWS;PER_BODY=old.PER_BODY
namespace=dict(old.namespace)
code=old.code;marker='queue_reference.rtc_snapshot()';assert code.count(marker)==1;code=code.replace(marker,marker+',console_reference.snapshot()')
marker=' def trace(pc,w,r,sr,pr,fp):';assert code.count(marker)==1
hooks=''' def console_hook(m,regs,sr,channel,target):
  events.append(event(target,regs,sr,state.pr))
  rc,result=console_reference.service(m,channel,regs[7],regs[4],regs[5],regs[6])
  if rc:stop(target,regs,sr,rc)
  if result is not None:regs[0]=result
  return sr
 hooked[SYSINFO_TARGET]=lambda m,r,sr:console_hook(m,r,sr,0,SYSINFO_TARGET)
 hooked[FLASH_TARGET]=lambda m,r,sr:console_hook(m,r,sr,1,FLASH_TARGET)
'''
code=code.replace(marker,hooks+marker)
namespace.update(SYSINFO_TARGET=console_model.SYSINFO,FLASH_TARGET=console_model.FLASH)
exec(compile(code,'console_original_model','exec'),namespace)
reference_run=types.FunctionType(old.reference_run.__code__,dict(old.reference_run.__globals__,raw_reference=namespace['reference_run']),'reference_run')
effects=types.SimpleNamespace(**old.effects.__dict__);effects.namespace=namespace;effects.reference_run=reference_run
effects.DLL=ROOT/'work/sonic-adventure/build/movie_console.dll';effects.LIB=ctypes.CDLL(str(effects.DLL));console_model.declare(effects.LIB)
observed=effects.LIB.sa_movie_console_observed;observed.argtypes=old.observed.argtypes[:-2]+[ctypes.POINTER(console_model.Console)]+old.observed.argtypes[-2:];observed.restype=ctypes.c_int
for name in ('sa_driver_signal_bus_init','sa_driver_signal_set_samples','sa_hardware_transfer_set_samples','sa_display_timing_set_samples','sa_disc_queue_init','sa_rtc_sample_bus_init','sa_rtc_set_samples'):
 getattr(effects.LIB,name).argtypes=getattr(old.effects.LIB,name).argtypes;getattr(effects.LIB,name).restype=getattr(old.effects.LIB,name).restype
effects.LIB.sa_disc_queue_size.restype=ctypes.c_uint32;native_stats=old.native_stats.__class__()
def invoke(*args):return observed(*args,ctypes.byref(effects.console_state),ctypes.byref(native_stats),0xffffffff)
effects.fn=invoke
compare_code=old.old.old.old.compare_code
marker=' other=effects.State.from_buffer_copy(s);';assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker," console_ref=console_model.Reference(c);namespace['console_reference']=console_ref\n"+marker)
marker=' memory=RtcMemory();';assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker," console_state,console_info,console_flash=console_model.configure(effects.LIB,c);effects.console_state=console_state\n console_inputs=(bytes(console_info),bytes(console_flash))\n"+marker)
marker='(memory.rtc_configured,memory.rtc_count,memory.rtc_cursor)))';assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker,'(memory.rtc_configured,memory.rtc_count,memory.rtc_cursor),console_model.snapshot(console_state)))')
marker=' return dict(rtc_snapshot=';assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker," assert console_model.snapshot(console_state)==console_ref.snapshot() and (bytes(console_info),bytes(console_flash))==console_inputs\n return dict(console_snapshot=console_ref.snapshot(),console_inputs_unchanged=True,rtc_snapshot=")
exec(compile(compare_code,'console_comparison','exec'),dict(old.compare.__globals__,effects=effects,namespace=namespace,native_stats=native_stats,console_model=console_model),local:={});compare=local['compare']

def prepare(ram,c):
 m,s,f=old.prepare(ram,c)
 state,info,flash=console_model.configure(effects.LIB,c)
 if c.get('console_info') or c.get('console_flash'):
  view=effects.RAM_TYPE.from_buffer(m);assert effects.LIB.sa_console_bind_vectors(ctypes.byref(state),view)==0
 return m,s,f

def cases(smoke=False):
 base=dict(entry=old.QUERY,mode=0,console_info=True,console_flash=True,rtc_samples=[0x1234,0xabcd]*32,render_seed={0x5f8000:0x17fd11db,0x5f8004:0x11},startup_overrides={0x5f689c:0xb},irq_samples=[0,8],fifo_samples=[0]*256,scan_samples=[0,1]*64)
 if smoke:
  for a in (old.REGION,old.QUERY,old.SETUP,old.INFO_INIT,old.INFO,old.FLASH):yield base|dict(entry=a)
  return
 for entry,mode,port,region in itertools.product((old.REGION,old.QUERY),(0,1,4),(0,0x100,0x200,0x300),(0,ord('0'),ord('1'),ord('2'),ord('3'),ord('9'),0x80,0xff)):
  yield base|dict(entry=entry,mode=mode,port_data=port,region_byte=region)
 for entry in (old.REGION,old.QUERY,old.SETUP,old.INFO_INIT,old.INFO,old.FLASH):
  for extra in (dict(console_info=False,console_flash=False),dict(console_info=False),dict(console_flash=False),dict(flash_size=0),dict(bad_stack=True)):
   yield base|dict(entry=entry)|extra
 for entry in (old.REGION,old.QUERY,old.SETUP):
  yield base|dict(entry=entry,flash_patches=[[0x1a05e,[0x12,0x34]]])
 for port,region in ((0,ord('1')),(0x100,ord('1')),(0x300,ord('2'))):
  yield base|dict(entry=old.CALLER,port_data=port,region_byte=region,cold_library=True)

def main():
 ram=old.old.driver.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.seed_ram();records=[];coverage=set();edges=set();checks=0
 for c in cases('--smoke' in sys.argv):
  m,s,f=prepare(ram,c);before=effects.State.from_buffer_copy(s);r,path,a,s,f=compare(m,s,f,c)
  assert r['full_state_match'] and r['ordered_boundaries_match'] and r['console_inputs_unchanged'];checks+=3
  entry=c['entry'];raw=c.get('region_byte',ord('1'));region=raw-ord('0') if raw in (ord('1'),ord('2'),ord('3')) else 0
  if entry in (old.REGION,old.QUERY) and c.get('console_info') and c.get('console_flash') and not c.get('bad_stack'):
   assert r['status']==0;checks+=1
  if r['status']==0 and entry in (old.REGION,old.QUERY):
   value=region if entry==old.REGION else 1 if not c.get('port_data',0)&0x300 else 2 if region in (1,3) else 0
   assert s.r[0]==value and s.r[15]==before.r[15] and s.pr==before.pr;checks+=3
   assert bytes(a[0x68:0x80])==console_model.inputs(c)[0] and r['console_snapshot'][4]==1;checks+=2
   assert r['console_snapshot'][5:]==(2,1,0 if c.get('flash_size',0x20000)==0 else 2);checks+=1
  if entry==old.CALLER:
   calls=[b for b in r['boundaries'] if b['target']=='0x8c010180'];r['initializer_call_arguments']=[b['arguments'] for b in calls]
   expected,mode=(1,2) if not c.get('port_data',0)&0x300 else (58,1) if region in (1,3) else (56,0)
   assert len(calls)==1 and calls[0]['arguments']==[expected,0,1,3];checks+=2
   assert load(a,load(m,0x8c0547f8))==mode;checks+=1
   assert r['status']==-2 and r['stop_pc']=='0x8c092fa0';checks+=2
   assert r['clear_native']['writes']==r['clear_reference']['writes']==0x200000 and r['full_vram_bytes_compared']==0x800000;checks+=2
  coverage.update(path)
  for i,pc in enumerate(path):
   text=ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+distance<len(path):edges.add((pc,path[i+distance]))
  r['guest_r0_at_stop']=hex(s.r[0]);records.append(r);print(hex(entry),r['status'],r['stop_pc'],r['console_snapshot'],flush=True)
 if '--smoke' in sys.argv:return
 rows=[]
 for routine in old.manifest['routines']:
  body=set(PER_BODY[int(routine['address'],16)]);wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
  rows.append(dict(address=routine['address'],static_rows=len(body),covered_rows=len(body&coverage),missing_rows=[hex(x) for x in sorted(body-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)]))
 files=[Path(__file__),Path(old.__file__),P/'console_service_reference.py',P/'analysis/movie_console_connection.json',P/'analysis/console_service_backend_verification.json',P/'analysis/movie_initialize_integration.json']
 report=dict(cases_passed=len(records),independent_checks=checks,status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=rows,dll_sha256=hashlib.sha256(effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},cases=records,new_registry_credit=0,scope='Original game execution plus separate explicit native console/flash policy. Borrowed invented bytes are actual storage inputs, not game query results. Flash returns byte counts per primary ABI evidence; unsupported/unconfigured and invalid controls remain stops. No physical BIOS/flash, natural reset or playable port claim.')
 (P/'analysis/movie_console_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Console comparisons',len(records),checks,report['status_counts']);print([(r['address'],r['covered_rows'],r['static_rows'],len(r['missing_edges'])) for r in rows])
if __name__=='__main__':main()
