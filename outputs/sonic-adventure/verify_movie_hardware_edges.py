"""Exercise reachable counter/stack overlap in both original polling bodies."""
import hashlib,itertools,json
from pathlib import Path
import verify_movie_hardware as v
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 ram=v.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.seed_ram();records=[];coverage=set();edges=set()
 for entry,mode in itertools.product((v.WAIT,v.FIFO_WAIT),(0,4)):
  c=dict(entry=entry,mode=mode,fifo_samples=[0,0,0],counter_aliases_poll_output=True)
  m,s,f=v.prepare(ram,c);counter=0x10c if entry==v.WAIT else 0x110;local=s.r[15]-16;root=local-counter;v.store(m,0x8c5fbb14,root)
  record,path,a,s,f=v.compare(m,s,f,c);assert record['status']==-4 and record['fifo_cursor']==3 and record['fifo_samples_unchanged']
  assert record['stop_pc']==('0x8c10fb4a' if entry==v.WAIT else '0x8c10f9aa') and v.load(a,local)==2
  record['context_root']=hex(root);record['aliased_counter_and_poll_output']=hex(local);record['independent_checks']=5;records.append(record);coverage.update(path)
  for j,pc in enumerate(path):
   if pc not in v.ROWS:continue
   text=v.ROWS[pc];distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((pc,path[j+distance]))
 files=[Path(__file__),Path(v.__file__),v.MP,v.P/'analysis/movie_hardware_connection.json']
 report=dict(cases_passed=4,independent_checks=20,status_counts={'-4':4},covered_addresses=[hex(x) for x in sorted(coverage)],covered_conditional_edges=[[hex(a),hex(b)] for a,b in sorted(edges)],dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='The caller context counter intentionally aliases the original stack-local readiness result in valid RAM. Incrementing it from one to two reaches the otherwise missed second loop edge; each real query rewrites one before the next increment. Finite raw FIFO observations stop without inventing completion. Complete original/native state matches; this is an alias-boundary test, not claimed normal driver startup.',cases=records)
 (v.P/'analysis/movie_hardware_edges_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Four original/native counter-alias cases passed; 20 independent checks')
if __name__=='__main__':main()
