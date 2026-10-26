"""Measured original/native configuration coverage; proposals remain unqualified."""
import hashlib,itertools,json
from pathlib import Path
import write_watch_graphics_init as fast
v=fast.old;P=v.P;ROOT=v.ROOT
def cases():
 for bits,fmt in itertools.product(range(64),(32,33,34,35,36)):
  flags=sum(bit for i,bit in enumerate((1,2,0x10,0x20,0x10000,0x100000)) if bits>>i&1)
  yield dict(entry=v.GRAPHICS_INIT_ENTRY,kind='initializer_flags',mode=0,gfx_observed_fpscr=True,gfx_config_words=[(4,flags),(0x5c,fmt)])
 for mode in (1,4):yield dict(entry=v.GRAPHICS_INIT_ENTRY,kind='initializer_fpu',mode=mode)
 for size in (0,1,7,8,9,0x11bfff,0x11c001,0xffffffff):yield dict(entry=v.GRAPHICS_INIT_ENTRY,kind='initializer_size',mode=0,gfx_observed_fpscr=True,gfx_config_words=[(0x1c,size)])
def main():
 ram=v.seed_ram();seen=set();edges=set();records=[];checks=0
 for i,case in enumerate(cases(),1):
  c=v.Input(case);m,s,f=v.prepare(ram,c);before=v.effects.State.from_buffer_copy(s)
  r,path,a,s,f=fast.compare(m,s,f,c)
  assert r['full_state_match'] and r['ordered_boundaries_match'];checks+=2
  if not r['status']:
   assert s.pc==s.pr==before.pr and list(s.r[8:])==list(before.r[8:]);checks+=2
   if s.r[0]==0:
    ctx=v.load(m,0x8c6569d8);flags=v.load(m,before.r[4]+4);size=v.load(m,before.r[4]+0x1c)
    for offset,value in ((6,int(bool(flags&2))),(8,v.load(m,before.r[4]+16)),(9,v.load(m,before.r[4]+20)),(10,int(bool(flags&1))),(12,v.load(m,before.r[4]+16)),(13,v.load(m,before.r[4]+20)),(14,int(bool(flags&1))),(17,int(not flags&0x20)),(23,(size+7)&0xfffffff8)):
     assert v.load(a,ctx+offset*4)==value,(i,offset,hex(v.load(a,ctx+offset*4)),hex(value));checks+=1
  seen.update(path)
  for j,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((pc,path[j+distance]))
  records.append(dict(input=dict(c),status=r['status'],stop_pc=r['stop_pc'],r0=hex(s.r[0]),steps=len(path),boundaries=r['boundaries']))
  print(i,c['kind'],c.get('gfx_config_words'),r['status'],r['stop_pc'],hex(s.r[0]),flush=True)
  # Every completed case is preserved if a later input exposes an issue.
  coverage=[]
  for body in v.manifest['routines']:
   wanted=set(body['reachable_addresses']);actual={hex(pc) for pc in seen};wanted_edges=set(map(tuple,body['conditional_edges']));actual_edges={(hex(a),hex(b)) for a,b in edges}
   coverage.append(dict(address=body['address'],rows=len(wanted),covered_rows=len(wanted&actual),missing_rows=sorted(wanted-actual),outcomes=len(wanted_edges),covered_outcomes=len(wanted_edges&actual_edges),missing_outcomes=[list(e) for e in sorted(wanted_edges-actual_edges)]))
  files=[Path(__file__),Path(v.__file__),Path(fast.__file__),v.MP,v.GRAPHICS_CONFIG_FIXTURE_PATH,P/'analysis/movie_graphics_init_connection.json']
  report=dict(cases_completed=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Exploratory original/native cases with explicit observed configuration data and declared field variations,not a guest checkpoint. Callee-saved/return/context-bit/size oracles supplement full state and boundaries. Coverage is measured per original body;incomplete bodies remain unqualified and receive no recovery credit. No helper results/configuration outputs/ARMack are injected;no hardware rendering or playable proof.')
  (ROOT/'work/sonic-adventure/movie_graphics_init_exploration.json').write_text(json.dumps(report,indent=2)+'\n')
 print('Completed exploration',len(records),'checks',checks)
if __name__=='__main__':main()
