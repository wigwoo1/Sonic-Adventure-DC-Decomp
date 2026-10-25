"""Replay retained startup and verify that newly added routes preserve old code."""
import hashlib,json
from pathlib import Path
import verify_movie_disc_paths as v
P=v.P;ROOT=v.ROOT

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 translation=json.loads(v.MP.read_text());old_bridge=P/'source/sa_movie_disc_init_connected.c';new_bridge=P/'source/sa_movie_disc_paths_connected.c'
 text=new_bridge.read_text().replace('sa_movie_disc_paths_connected','sa_movie_disc_init_connected').replace('sa_movie_disc_paths_initialized_connected','sa_movie_disc_init_initialized_connected')
 for routine in translation['routines']:
  declaration=f'VDECL({routine["name"]});\n';assert text.count(declaration)==1;text=text.replace(declaration,'')
  route=f' if(target=={routine["address"]}u)return {routine["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
  assert text.count(route)==1;text=text.replace(route,'')
 assert text==old_bridge.read_text()
 prior_path=P/'analysis/movie_disc_init_verification.json';prior=json.loads(prior_path.read_text());warm,cold=[c for c in prior['cases'] if c['input']['entry']==0x8c010180]
 additions={r['address'] for r in translation['routines']};assert not additions&{b['target'] for b in cold['boundaries']}
 ram=v.driver.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.seed_ram();c=next(c for c in v.old.cases() if c['entry']==0x8c010180 and not c.get('cold_filesystem'))
 m,s,f=v.old.old.prepare(ram,c);record,path,a,s,f=v.compare(m,s,f,c)
 assert record['status']==0 and hex(s.r[0])==warm['guest_r0']=='0xfffffff8' and record['stop_pc']==warm['stop_pc']=='0x8cfed000'
 for key in ('steps','boundaries','boundary_comparisons','disc_reads','disc_snapshot','clear_native','clear_reference','vram_sha256','clock_reads'):
  assert json.loads(json.dumps(record[key]))==warm[key],key
 assert record['clear_native']['writes']==0x200000 and record['full_state_match'] and record['ordered_boundaries_match']
 files=[Path(__file__),Path(v.__file__),v.MP,prior_path,old_bridge,new_bridge,P/'analysis/movie_disc_paths_connection.json']
 report=dict(retained_warm_startup_comparisons=1,original_bridge_preserved_except_added_routes=True,added_routes=len(additions),retained_cold_trace_has_no_added_routes=True,retained_cold_reads=cold['disc_read_attempts'],retained_cold_boundaries=cold['boundary_comparisons'],cold_start_reexecuted_in_new_dll=False,warm_result=record,warm_guest_r0=hex(s.r[0]),dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},
  scope='New 171-source build directly replays retained warm earlier startup and every full-clear write. The prior 170-source cold benchmark remains separately retained. Byte-identical original bridge routing plus its cold trace having none of the additions is source-level preservation evidence, not a claim to have rerun that entire cold comparison in the new DLL.')
 (P/'analysis/movie_disc_paths_regression_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Retained warm startup/full clear matched; prior cold trace does not use any new route; original dispatch preserved')
if __name__=='__main__':main()
