"""Preserve the matching checkout and build pinned upstream GCC in a worktree.

Run with Linux Python inside the existing sa-dc-source-review WSL distribution.
All retail data, fetched toolchains and complete game binaries remain private.
"""
import argparse,hashlib,json,os,shutil,subprocess,time
from datetime import datetime,timezone
from pathlib import Path

PIN='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
def sha(p):
 h=hashlib.sha256()
 with p.open('rb') as f:
  for b in iter(lambda:f.read(4*1024*1024),b''):h.update(b)
 return h.hexdigest()
def git(root,*args):return subprocess.check_output(['git','-C',str(root),*args],text=True).strip()
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--workspace',type=Path,required=True);ap.add_argument('--source',type=Path,required=True);ap.add_argument('--destination',type=Path,required=True);ap.add_argument('--jobs',type=int,default=8);a=ap.parse_args()
 source=a.source.resolve();dest=a.destination.resolve();w=a.workspace.resolve()/'work/sonic-adventure'
 assert 1<=a.jobs<=16 and source!=dest and source not in dest.parents and dest not in source.parents
 assert dest.parent==source.parent==Path('/root') and not dest.exists(),'Use a fresh sibling worktree; never overwrite prior work'
 assert git(source,'rev-parse','HEAD')==PIN and not git(source,'status','--porcelain')
 for command in ['git','make','gcc','g++','xz','curl','tar','sha256sum','timeout']:assert shutil.which(command),command
 prior_path=w/'japanese_v1007_full_matching_verification.json'
 assert sha(prior_path)=='3a6c7e28720e6b99c03572fae6e441e471dfe53bb8b8d7570a362ec1027634'
 prior=json.loads(prior_path.read_text());assert len(prior['images'])==247
 for row in prior['images']:
  for area in ['orig','build']:assert sha(source/area/row['disc_path'])==row['sha256'],(area,row['disc_path'])
 tracked=subprocess.check_output(['git','-C',str(source),'ls-files','-z']).decode().rstrip('\0').split('\0')
 source_hashes={n:sha(source/n) for n in tracked}
 print('Pinned source and all 247 original/matching image hashes verified.',flush=True)
 subprocess.run(['git','-C',str(source),'worktree','add','--detach',str(dest),PIN],check=True)
 assert git(dest,'rev-parse','HEAD')==PIN
 assert all(sha(dest/n)==h for n,h in source_hashes.items())
 shutil.copytree(source/'orig',dest/'orig',symlinks=True,dirs_exist_ok=True)
 for name in ['wibo-i686','shc-v5.0r10','shc-v5.0r26','shc-v5.0r32']:
  p=source/'tools'/name;target=dest/'tools'/name;assert p.exists() and not target.exists()
  if p.is_dir():shutil.copytree(p,target,symlinks=True)
  else:shutil.copy2(p,target)
 assert not git(source,'status','--porcelain') and not git(dest,'status','--porcelain')
 for row in prior['images']:assert sha(dest/'orig'/row['disc_path'])==row['sha256']
 prepare=dict(status='prepared_pristine_pinned_GCC_worktree',upstream_commit=PIN,source=str(source),destination=str(dest),source_tracked_files=len(tracked),source_sha256=source_hashes,original_and_matching_image_pairs_checked=247,retail_files_copied_privately=True,original_checkout_unmodified=True,game_source_publicly_imported=False,native_PC_build=False,overall_completion_percent=10,goal_paused=False,checked_at_UTC=datetime.now(timezone.utc).isoformat())
 p=w/'jp_gcc_worktree_preparation.json';assert not p.exists();p.write_text(json.dumps(prepare,indent=2)+'\n')
 log=w/'jp_gcc_baseline_build.log';terminal=w/'jp_gcc_baseline_build_terminal.json';state=w/'jp_gcc_baseline_build_process.json'
 assert not log.exists() and not terminal.exists() and not state.exists()
 cmd=['make',f'-j{a.jobs}','gcc'];started=time.time()
 with log.open('wb') as out:
  proc=subprocess.Popen(cmd,cwd=dest,stdout=out,stderr=subprocess.STDOUT,env=dict(os.environ,PYTHONDONTWRITEBYTECODE='1'))
  state.write_text(json.dumps(dict(status='running',PID=proc.pid,command=cmd,checkout=str(dest),upstream_commit=PIN,log=str(log),started_at_UTC=datetime.now(timezone.utc).isoformat()),indent=2)+'\n')
  print('Started isolated make gcc; PID',proc.pid,'log',str(log),flush=True)
  code=proc.wait()
 source_unchanged=git(source,'rev-parse','HEAD')==PIN and not git(source,'status','--porcelain') and all(sha(source/n)==h for n,h in source_hashes.items())
 old_images_unchanged=all(sha(source/area/row['disc_path'])==row['sha256'] for row in prior['images'] for area in ['orig','build'])
 result=dict(status='upstream_GCC_command_completed' if code==0 else 'upstream_GCC_command_failed',exit_code=code,command=cmd,elapsed_seconds=round(time.time()-started,3),PID=proc.pid,checkout=str(dest),upstream_commit=PIN,checkout_clean=not git(dest,'status','--porcelain'),original_source_unchanged=bool(source_unchanged),original_247_matching_image_pairs_unchanged=bool(old_images_unchanged),log_sha256=sha(log),log_bytes=log.stat().st_size,native_PC_build=False,complete_output_manifest_verification_pending=code==0,overall_completion_percent=10,goal_paused=False)
 terminal.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2),flush=True)
 assert source_unchanged and old_images_unchanged,'Original baseline preservation failed'
 raise SystemExit(code)
if __name__=='__main__':main()
