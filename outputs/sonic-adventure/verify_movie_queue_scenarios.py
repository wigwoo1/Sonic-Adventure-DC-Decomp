"""Original initializer request admission/status/cancellation evidence."""
import ctypes,hashlib,itertools,json,types
from pathlib import Path
import verify_movie_disc_paths as v
import disc_queue_scenario_reference as scenario
P=v.P;ROOT=v.ROOT;INIT=v.old.INIT
platform=types.SimpleNamespace(**v.platform.__dict__);platform.DiscReference=scenario.DiscReference

def cases():
 items=[{},dict(empty_media=True),dict(owned=1),dict(owned=7),dict(owned=8),dict(owned=8,empty_media=True),dict(cancel_pump=1),dict(cancel_pump=2),dict(fill_before_command=8),dict(fill_before_command=7),dict(fill_before_command=7,cancel_pump=1)]
 for mode,item in itertools.product((0,1,4),items):
  yield dict(entry=INIT,mode=mode,rtc_samples=[],fifo_samples=[0]*128,scan_samples=[0,1]*32,slots=2,capacity=8,mount_profile='normal')|item

def main():
 ram=v.driver.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.seed_ram()
 prior_path=P/'analysis/movie_mount_verification.json';prior=json.loads(prior_path.read_text());body=set(v.PER_BODY[INIT]);coverage=body-{int(a,16) for a in prior['missing_rows']}
 translation=P/'analysis/movie_disc_init_translation.json';routine=next(r for r in json.loads(translation.read_text())['routines'] if int(r['address'],16)==INIT)
 wanted={tuple(int(a,16) for a in edge) for edge in routine['conditional_edges']};edges=wanted-{tuple(int(a,16) for a in edge) for edge in prior['missing_edges']}
 records=[];checks=0
 for c in cases():
  effects,actions=scenario.native_scenario(v,c)
  compare=types.FunctionType(v.compare.__code__,dict(v.compare.__globals__,effects=effects,platform=platform),'compare')
  m,s,f=v.old.prepare(ram,c);before=effects.State.from_buffer_copy(s);record,path,a,s,f=compare(m,s,f,c)
  assert record['full_state_match'] and record['ordered_boundaries_match'];checks+=2
  assert actions==v.namespace['disc_reference'].actions;checks+=1
  guest=ctypes.c_int32(s.r[0]).value
  if c.get('owned'):
   assert record['status']==-6 and record['stop_pc']=='0x8c00f100' and not record['disc_reads'] and not actions;checks+=4
   assert sum(j[1]!=0 for j in record['disc_snapshot'][7])==c['owned'];checks+=1
  else:
   expected=-2 if c.get('fill_before_command')==8 else -27 if c.get('cancel_pump')==1 and not c.get('fill_before_command') else -5 if c.get('empty_media') else 0
   assert record['status']==0 and guest==expected and s.r[15]==before.r[15] and s.pr==before.pr;checks+=4
   assert record['disc_read_attempts']==(2 if expected==0 else 0);checks+=1
   root=v.load(a,0x8c673414);assert root==v.old.CTX and int.from_bytes(v.sample(a,root+0x38,2),'little')==int(expected==0);checks+=2
   if c.get('fill_before_command'):
    n=c['fill_before_command'];assert actions[:n]==[('submit',i+1) for i in range(n)];checks+=1
    assert sum(j[1]!=0 for j in record['disc_snapshot'][7])==n;checks+=1
   if c.get('cancel_pump'):
    handle=1 if c['cancel_pump']==1 else 2;assert ('cancel',handle) in actions;checks+=1
  coverage.update(set(path)&body)
  for i,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+distance<len(path):edges.add((pc,path[i+distance]))
  record.update(guest_r0=hex(s.r[0]),external_queue_actions=[list(x) for x in actions]);records.append(record)
  print(c['mode'],{k:c[k] for k in ('empty_media','owned','cancel_pump','fill_before_command') if k in c},record['status'],guest,flush=True)
 files=[Path(__file__),Path(scenario.__file__),P/'disc_mount_platform_reference.py',prior_path,translation,P/'analysis/movie_mount_media.json',P/'analysis/movie_disc_paths_qualification.json',P/'source/native_disc_queue.c',P/'source/native_disc_read_command.c',P/'source/sa_movie_disc_init.c',P/'verify_movie_disc_paths.py']
 report=dict(cases_passed=len(records),independent_checks=checks,initializer_address=hex(INIT),static_rows=len(body),combined_covered_rows=len(coverage),newly_executed_rows=len(coverage)-prior['combined_covered_rows'],combined_executed_outcomes=len(wanted&edges),missing_rows=[hex(a) for a in sorted(body-coverage)],missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)],qualified=False,new_registry_credit=0,dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},cases=records,scope='Explicit legal native queue setup/other-client submission/cancellation and zero-sector media inputs. Every external action uses actual exported native queue APIs after the unchanged service-entry snapshot, before the unchanged original/native service call. Independent model applies the same declared actions. No register, game helper or firmware return is replaced. Host platform policies remain distinct from physical Dreamcast firmware. Original initializer remains partial; no new routine credit or natural-boot claim.')
 (P/'analysis/movie_queue_scenarios_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Initializer',len(coverage),'/',len(body),'rows;',len(wanted&edges),'/',len(wanted),'outcomes;',checks,'checks')
if __name__=='__main__':main()
