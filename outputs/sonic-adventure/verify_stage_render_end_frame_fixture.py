"""Original world/frame handoff under an isolated bank-1 CPU-entry fixture."""
import hashlib,json,pickle
from pathlib import Path
from stage_render_end_verified_state import normal,sha,P,W

def read(s,a,n=4):return int.from_bytes(s.m[a&0xffffff:(a&0xffffff)+n],'little')
def main():
 target=P/'analysis/stage_render_end_frame_fixture_verification.json';assert not target.exists()
 scene,q=normal();original_sr=scene.s.sr;scene.s.sr|=0x20000000
 inherited_jobs=scene.c['pixel_job_seed'].summary()['processed'];cases=[];saved=[];stopped=None
 original=(W/'extracted/1ST_READ.BIN').read_bytes()
 def save(name):
  assert scene.usable and scene.s.pc==scene.s.pr==0x8cfed000
  folder=W/name;folder.mkdir(exist_ok=False);raw=pickle.dumps((bytes(scene.m),bytes(scene.s),bytes(scene.f),scene.c),protocol=5)
  (folder/'normal_fixture_return.pkl').write_bytes(raw)
  meta=dict(status='normal_return_from_explicit_bank1_API_fixture_NOT_CANONICAL_GAME',CPU_entry_SR_is_explicit_fixture=True,original_SR=hex(original_sr),fixture_SR=hex(original_sr|0x20000000),original_bank_mode_producer_verified=False,bank1_fixture_canonical=False,checkpoint_sha256=hashlib.sha256(raw).hexdigest(),prefix_provenance_path=str(scene.prefix),prefix_provenance_sha256=sha(scene.prefix),DLL_path=str(q.v.effects.DLL),DLL_sha256=sha(q.v.effects.DLL),faulted_state_saved=False,inherited_completed_jobs=inherited_jobs,completed_native_jobs=scene.c['pixel_job_seed'].summary()['processed'],retired_packets=scene.c['ta_list_seed']['last_count'],active_packets=len(scene.c['graphics_in'].packets),render_pending=scene.c['render_completion_seed']['pending'],drawn_land_count=read(scene,0x8c75a308,2),phases=scene.records)
  (folder/'provenance.json').write_text(json.dumps(meta,indent=2)+'\n')
  (folder/'NOT_CANONICAL.txt').write_text('Isolated bank-1 CPU API fixture. Original boot/handoff unverified. Never use as a canonical game continuation.\n')
  saved.append(dict(folder=str(folder),checkpoint_sha256=meta['checkpoint_sha256'],provenance_sha256=sha(folder/'provenance.json')));return folder
 def call(name,entry,args=()):
  nonlocal stopped
  r,path=scene.call(entry,args,compare=q.fast.compare)
  assert not any(0x900000<=(a&0xffffff)<0xa00000 for a in path)
  for a in set(path):
   off=a&0xffffff
   if 0x10000<=off<=0x10000+len(original)-2:assert bytes(scene.m[off:off+2])==original[off-0x10000:off-0x10000+2],hex(a)
  row=dict(name=name,entry=hex(entry),arguments=list(args),status=r['status'],stop_pc=r['stop_pc'],steps=len(path),full_state_match=r['full_state_match'],ordered_boundaries_match=r['ordered_boundaries_match'],boundaries=r['boundaries'],command_packets=len(r['command_packets']))
  cases.append(row);print(name,r['status'],r['stop_pc'],len(path),'packets',row['command_packets'],flush=True)
  if r['status']!=0:assert not scene.usable;stopped=row;return False
  assert scene.usable;return True
 if call('isolated_bank1_world_level1',0x8c09878c,(1,)):
  save('stage-render-end-bank1-fixture-world')
  if call('original_particle_level5_under_fixture',0x8c09878c,(5,)):
   save('stage-render-end-bank1-fixture-particles')
   if call('original_frame_under_fixture',0x8c6050b0) and call('original_query_under_fixture',0x8c04fe40):
    folder=save('stage-render-end-bank1-fixture-frame')
    from PIL import Image
    Image.frombytes('RGBA',(640,480),scene.c['pixel_job_seed'].pixels,'raw','BGRA').save(folder/'native_fixture_stage_frame.png')
 files=[Path(__file__),P/'stage_render_end_verified_state.py',P/'stage_render_end_reference.py',P/'analysis/stage_render_end_verification.json',q.v.effects.DLL]
 report=dict(status='matched_original_frame_handoff_under_explicit_CPU_bank_fixture_not_real_game_boot',original_native_comparisons=len(cases),normal_returns=sum(c['status']==0 for c in cases),host_stops=sum(c['status']!=0 for c in cases),cases=cases,stopped_at=stopped,normal_fixture_checkpoints=saved,CPU_entry_SR_is_explicit_fixture=True,original_SR=hex(original_sr),fixture_SR=hex(original_sr|0x20000000),original_bank_mode_producer_verified=False,bank1_fixture_canonical=False,inherited_completed_native_jobs=inherited_jobs,fixture_completed_native_jobs=scene.c['pixel_job_seed'].summary()['processed'],faulted_state_resumed_or_saved=False,geometry_globals_or_callee_results_patched=False,all_task_levels_or_player_qualified=False,real_gameplay_or_complete_game_frame_qualified=False,shared_bank_code_PCs_reached=False,new_registry_credit=0,overall_completion_percent=10,goal_paused=False,playable=False,sha256={str(f):sha(f) for f in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('ISOLATED FRAME HANDOFF SEALED:',len(cases),'comparisons.',flush=True)
if __name__=='__main__':main()
