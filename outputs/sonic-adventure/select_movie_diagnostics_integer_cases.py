"""Select useful integer-format cases by running the original instructions."""
import hashlib,itertools,json
from pathlib import Path
import verify_movie_diagnostics as v
P=v.P

def main():
 base=next(v.old.old.cases())|dict(entry=v.INTEGER);m,s,f=v.prepare(v.seed_ram(),base);initial_s=v.effects.State.from_buffer_copy(s);initial_f=v.effects.Fpu.from_buffer_copy(f)
 routine=next(r for r in v.manifest['routines'] if r['address']==hex(v.INTEGER));wanted_rows=set(v.PER_BODY[v.INTEGER]);wanted_edges={tuple(int(x,16) for x in e) for e in routine['conditional_edges']};seen=set();edges=set();selected=[];attempts=0
 for value,spec,flags,width,precision,pad in itertools.product((0,1,0xffffffff,0x80000000,0x7fffffff),(100,105,117,111,120,88),(0,2,4,8,16,12,20),(0,2,12),(0xffffffff,0,1,6),(32,48)):
  state=v.effects.State.from_buffer_copy(initial_s);fp=v.effects.Fpu.from_buffer_copy(initial_f);state.r[4]=value;state.r[6]=spec
  v.write(m,v.CONTEXT,bytes(48));v.write(m,v.CONTEXT,bytes((flags,pad)));v.store(m,v.CONTEXT+4,width);v.store(m,v.CONTEXT+8,precision);v.store(m,v.CONTEXT+12,32)
  path=[]
  def trace(pc,w,r,sr,pr,f):path.append(pc)
  v.cpu.reference(m,state,fpu=fp,trace=trace,instruction_budget=100000,special=dict(macl=0x12345678,mach=0,other=[0]*8,gbr=0,vbr=0x8ce50000));attempts+=1
  rows=set(path)&wanted_rows;current=set()
  for i,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if i+distance<len(path) and (pc,path[i+distance]) in wanted_edges:current.add((pc,path[i+distance]))
  if rows-seen or current-edges:
   selected.append(dict(value=value,specifier=spec,number_flags=flags,number_width=width,number_precision=precision,number_padding=pad));seen|=rows;edges|=current
  if seen==wanted_rows and edges==wanted_edges:break
 files=[Path(__file__),Path(v.__file__),v.MP,P/'movie_diagnostics_reference_cpu.py',P/'sh4_widen_reference.py']
 report=dict(original_scenarios_searched=attempts,selected_cases=len(selected),covered_rows=len(seen),static_rows=len(wanted_rows),covered_outcomes=len(edges),static_outcomes=len(wanted_edges),missing_rows=[hex(x) for x in sorted(wanted_rows-seen)],missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted_edges-edges)],cases=selected,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Fast original-instruction coverage selection only, not native proof. Each selected case must pass fresh full-state native/original comparison. Selection does not supply any callee return.')
 (P/'analysis/movie_diagnostics_integer_case_selection.json').write_text(json.dumps(report,indent=2)+'\n');print({k:x for k,x in report.items() if k not in ('cases','sha256')})
if __name__=='__main__':main()
