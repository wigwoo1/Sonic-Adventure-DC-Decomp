"""Reach the remaining original request rejection branches with real callees."""
import hashlib,itertools,json
from pathlib import Path
import write_watch_transfer_runtime as fast
v=fast.old;P=v.P;ROOT=v.ROOT;cpu=v.old.old.old.old
def main():
 records=[];coverage=set();edges=set();ram=v.seed_ram();rows=set(v.PER_BODY[cpu.REQUEST])
 for mode,kind in itertools.product((0,1,4),('unsupported-mode','unsupported-negative-mode','unaligned-source','unaligned-length')):
  c=v.Input(entry=cpu.REQUEST,mode=mode,sdk_mode=2 if kind=='unsupported-mode' else 0xffffffff if kind=='unsupported-negative-mode' else 0,length=63 if kind=='unaligned-length' else 64,source=cpu.SRC+1 if kind=='unaligned-source' else cpu.SRC)
  m,s,f=cpu.prepare(ram,c);before=v.effects.State.from_buffer_copy(s);fp=bytes(f);r,path,a,s,f=fast.compare(m,s,f,c)
  assert r['status']==0 and s.pc==before.pr and s.pr==before.pr and s.r[15]==before.r[15] and bytes(f)==fp
  assert not any(r['sound_state']) and v.HANDLER not in path and r['runtime_state'] is None
  if kind.startswith('unsupported'):assert 0x8c65a64c in path and s.r[0]==0
  else:assert 0x8c65a65e in path and s.r[0]!=0
  coverage.update(rows&set(path))
  for i,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if pc in rows and text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+distance<len(path):edges.add((pc,path[i+distance]))
  records.append(dict(input=dict(c),status=r['status'],stop_pc=r['stop_pc'],r0=hex(s.r[0]),boundaries=r['boundary_comparisons']));print('Closure',mode,kind,hex(s.r[0]),flush=True)
 files=[Path(__file__),Path(v.__file__),Path(fast.__file__),P/'source/sa_movie_transfer_wait.c',P/'analysis/movie_transfer_runtime_connection.json']
 report=dict(cases_passed=len(records),cases=records,request_executed_rows=[hex(a) for a in sorted(coverage)],request_executed_branch_outcomes=[[hex(a),hex(b)] for a,b in sorted(edges)],dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Original139-row request: explicit SDK mode2/FFFFFFFF or real synchronous-copy alignment errors reach remaining original rejection branches. Complete original/native state, full RAM/devices/FPU and ordered real callees match; no host helper result, transfer,IRQ or completion flag is supplied.')
 (P/'analysis/movie_transfer_request_closure_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Request closure',len(records))
if __name__=='__main__':main()
