"""Original context instructions versus native code and independent saved-state contracts."""
import collections,ctypes,gc,hashlib,itertools,json
from pathlib import Path
import write_watch_interrupt_context as fast
v=fast.old;P=v.P;ROOT=v.ROOT
def bank(sr):return 1 if sr&0x40000000 and sr&0x20000000 else 0

def physical_pc(pc,c):
 vb=c.get('vbr',0x8ce50000)
 for offset,source in ((0x100,v.VECTORS[0]),(0x400,v.VECTORS[0]),(0x600,v.VECTORS[1])):
  if vb+offset<=pc<vb+offset+40:return source+pc-vb-offset
 return pc

def semantics(m,a,before,before_f,s,f,r,path,c):
 checks=4;assert r['full_state_match'] and r['ordered_boundaries_match'];assert not any(r['sound_state']);assert not r['cache_writeback_sequence'];assert tuple(r['exception_state'])==tuple(getattr(v.native_exception,k) for k in v.EXCEPTION_KEYS)
 if r['status']:
  if c.get('handler')==v.EXIT:
   assert r['status']==-2 and r['stop_pc']=='0xac000010' and not r['exception_state'][-1];checks+=3
  return checks
 assert r['stop_pc']==hex(c['spc']) and s.pc==c['spc'] and s.sr==c['ssr'];checks+=3
 assert bytes(f)==before_f and s.pr==before.pr;checks+=2
 direct=c['entry'] in (v.NORMAL,v.NESTED)
 visible=list(before.r[:8]);hidden=list(c['other'])
 if direct:visible[1]=c.get('saved_r1',before.r[1])
 if bank(before.sr)!=bank(c['ssr']):visible,hidden=hidden,visible
 expected=visible+list(before.r[8:]);expected[15]+=4 if direct else 0
 assert list(s.r)==expected and r['integer_other']==hidden;checks+=2
 assert r['final_gbr']==c['gbr'] and r['final_mach']==c['mach'] and r['macl']==c.get('macl',0x12345678) and r['vbr']==c.get('vbr',0x8ce50000);checks+=4
 assert r['exception_state']==tuple(c.get(k,0) for k in v.EXCEPTION_KEYS[:-1])+(1,);checks+=1
 callback=c.get('handler',v.COUNTER)
 if callback==v.COUNTER:
  address=v.load(m,0x8c666e4c);assert v.load(a,address)==(v.load(m,address)+1)&0xffffffff;checks+=1
 if not direct:
  origin=c['entry'];assert v.load(a,origin+44)==before.r[0];checks+=1
 return checks

def cases():
 base=dict(mode=0,initial_sr=0x70000003,ssr=0x40000003,spc=0x8cfed000)
 for entry in (v.NORMAL,v.NESTED):
  for callback,nesting,saved,mode in itertools.product((0,v.COUNTER),(0,) if entry==v.NORMAL else (0,1,2),(0x40000003,0x60000063),(0,1,4)):
   yield v.Input(base|dict(entry=entry,handler=callback,nesting=nesting,ssr=saved,mode=mode))
 for entry,context,mode in itertools.product((v.VECTORS[0],v.VECTORS[1],0x8ce50100,0x8ce50400,0x8ce50600),(v.NORMAL,v.NESTED),(0,1,4)):
  yield v.Input(base|dict(entry=entry,context_handler=context,mode=mode))
 for nesting,stack,mode in itertools.product((0,1,2),(0x8ce40000,),(0,1,4)):
  yield v.Input(base|dict(entry=0x8ce50600,context_handler=v.NESTED,handler=v.EXIT,nesting=nesting,interrupt_stack=stack,mode=mode))
 yield v.Input(base|dict(entry=0x8ce50600,context_handler=v.NESTED,handler=v.EXIT,nesting=1,interrupt_stack=0))
 for entry,extra in itertools.product((v.NORMAL,v.NESTED,0x8ce50600),(dict(handler=0x8cfef000),dict(initial_sr=0),dict(initial_sr=0x70008003),dict(stack=0x8c000000),dict(vbr=0))):
  if entry==0x8ce50600 and extra.get('vbr')==0:continue # Relocated entry must agree with its initialized VBR.
  yield v.Input(base|dict(entry=entry)|extra)
 for saved in (0,0x20000003,0x50000003,0xffffffff,0xdeadbeef):
  yield v.Input(base|dict(entry=0x8ce50600,ssr=saved))

def main():
 records=[];coverage=set();edges=set();checks=0;ram=v.seed_ram()
 for c in cases():
  m,s,f=v.prepare(ram,c);before=v.effects.State.from_buffer_copy(s);before_f=bytes(f);r,path,a,s,f=fast.compare(m,s,f,c);count=semantics(m,a,before,before_f,s,f,r,path,c);checks+=count
  mapped=[physical_pc(pc,c) for pc in path];coverage.update(mapped)
  for i,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+distance<len(path):edges.add((physical_pc(pc,c),physical_pc(path[i+distance],c)))
  keys=('input','status','stop_pc','steps','boundary_comparisons','full_state_match','ordered_boundaries_match','exception_state','integer_other','final_gbr','final_mach','final_fpul','write_watch')
  records.append({k:r[k] for k in keys}|dict(independent_checks=count));print(len(records),hex(c['entry']),r['status'],r['stop_pc'],flush=True);del r,path,a;gc.collect()
 rows=[]
 for definition in v.manifest['routines']:
  body=set(v.PER_BODY[int(definition['address'],16)]);wanted={tuple(int(x,16) for x in e) for e in definition['conditional_edges']}
  rows.append(dict(address=definition['address'],kind=definition['kind'],static_rows=len(body),covered_rows=len(body&coverage),missing_rows=[hex(a) for a in sorted(body-coverage)],conditional_outcomes=len(wanted),covered_outcomes=len(wanted&edges),missing_outcomes=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)]))
 files=[Path(__file__),Path(v.__file__),Path(fast.__file__),P/'interrupt_context_reference_cpu.py',v.MP,P/'analysis/movie_interrupt_context_connection.json',P/'analysis/sh4_exception_state_verification.json']
 report=dict(cases_passed=len(records),independent_checks=checks,status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=rows,cases=records,dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},partial_return_frontier='0xac000010',scope='Original normal/nested save/restore and relocated copied vectors compare full CPU/FPU/RAM/device/exception state. Independent canonical-bank, complete saved-register/FPU/stack/PR/SPC/SR restoration contracts checked. Original non-returning helper executes until its genuine unresolved bootstrap tail ac000010; no RTE or helper return is inserted there. Templates are fragments and aliases receive zero duplicate credit. Interrupt entry is explicit software input, not physical acceptance/timing or automatic pumping.')
 (P/'analysis/movie_interrupt_context_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Context comparisons',len(records),'checks',checks);print(rows)
if __name__=='__main__':main()
