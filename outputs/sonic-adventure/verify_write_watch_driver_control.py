"""Cross-check the opt-in checker against frozen direct result evidence."""
import gc,hashlib,json,sys,time
from pathlib import Path
import write_watch_driver_control as fast
v=fast.old;P=v.P

def main():
 ram=v.seed_ram();saved_path=P/'analysis/movie_driver_control_verification.json';saved=json.loads(saved_path.read_text());inputs=list(v.cases());assert len(inputs)==len(saved['cases'])
 checks=0;records=[];started=time.perf_counter()
 for index,(c,baseline) in enumerate(zip(inputs,saved['cases'])):
  if '--smoke' in sys.argv and index not in (0,1,96,108,119):continue
  c['write_watch_legacy_hashes']=True;m,s,f=v.prepare(ram,c);before=v.effects.State.from_buffer_copy(s)
  r,path,a,s,f=fast.compare(m,s,f,c);count=v.semantics(m,a,before,s,r,path,c)
  r['independent_checks']=count;r['guest_r0_at_stop']=hex(s.r[0]);r['guest_sr_at_stop']=hex(s.sr)
  for key,value in baseline.items():
   if key=='input':assert {k:x for k,x in r[key].items() if k!='write_watch_legacy_hashes'}==value
   else:assert json.loads(json.dumps(r[key]))==value,(index,key)
   checks+=1
  assert all(x['snapshots']==r['boundary_comparisons'] for x in r['write_watch']['regions'].values());checks+=2
  records.append(dict(input=dict(c),status=r['status'],stop_pc=r['stop_pc'],write_watch=r['write_watch']));gc.collect()
  print(index,r['status'],r['stop_pc'],flush=True)
 if '--smoke' in sys.argv:print('Write-watch integration smoke passed',len(records));return
 files=[Path(__file__),Path(fast.__file__),P/'windows_write_watch.py',saved_path,P/'analysis/windows_write_watch_verification.json',P/'analysis/movie_driver_control_qualification.json']
 report=dict(cases_passed=len(records),saved_record_checks=checks,legacy_full_hashes_compared_simultaneously=True,seconds=time.perf_counter()-started,game_dll_changed=False,dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),cases=records,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='All prior 121 driver-control inputs replay in the unchanged native DLL. Every recorded service boundary compares both the old complete RAM/VRAM hashes and exact changed-page union, with full initial/final bytes. Original decoder, all CPU/FPU/special/banked/device/input checks and complete video-clear scope are retained. Larger cold/warm startup cross-checks are a separate required gate. No routine recovery credit.')
 (P/'analysis/write_watch_driver_control_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Write-watch driver controls passed',len(records),checks)
if __name__=='__main__':main()
