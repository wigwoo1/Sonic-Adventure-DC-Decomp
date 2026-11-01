"""Preserve the published checkpoint except the four authorized status files."""
import hashlib,json,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parents[1];W=R/'work/sonic-adventure';D=R/'publication/sonic-adventure';G=r'C:\Program Files\Git\cmd\git.exe'
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def old(path):return subprocess.check_output([G,'-C',str(D),'show','30002ed530e9acde5ac3e29c99db2621a8d8100e:'+path])
def main():
 target=P/'analysis/player_child_records_and_extent_preservation.json';assert not target.exists()
 mp=W/'player_collision_attach_prior_source_manifest.json';bp=W/'player_collision_attach_prior_build_manifest.json'
 manifest=json.loads(mp.read_text());builds=json.loads(bp.read_text());assert len(manifest['files'])==16885 and len(builds['files'])==2787
 mutable={'outputs/sonic-adventure/'+n for n in ('README.md','SOURCE_HANDOFF.txt','project_status.json','analysis/recovered_routines.json')};changed=[]
 for row in manifest['files']:
  f=R/row['path'];assert f.exists(),row['path']
  if sha(f)!=row['sha256']:
   assert row['path'] in mutable,row['path'];changed.append(row['path'])
 assert set(changed)==mutable
 for name in ('README.md','SOURCE_HANDOFF.txt'):assert (P/name).read_bytes().endswith(old('outputs/sonic-adventure/'+name))
 before=json.loads(old('outputs/sonic-adventure/analysis/recovered_routines.json'))['routines'];after=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(before)==1636 and len(after)==1640 and after[:1636]==before
 before_status=json.loads(old('outputs/sonic-adventure/project_status.json'));after_status=json.loads((P/'project_status.json').read_text());normalized=json.loads(json.dumps(after_status));normalized['recovered_buildable_routines']=before_status['recovered_buildable_routines'];normalized['verified_progress']['recovered_buildable_routines']=before_status['verified_progress']['recovered_buildable_routines'];del normalized['verified_progress']['player_child_records_and_extent'];assert normalized==before_status
 print('Preserved16881 immutable source/proof files and all prior registry/status data',flush=True)
 for row in builds['files']:assert (R/row['path']).stat().st_size==row['bytes'] and sha(R/row['path'])==row['sha256'],row['path']
 qp=P/'analysis/player_child_records_and_extent_qualification.json';q=json.loads(qp.read_text())
 for path,value in q['sha256'].items():assert sha(path)==value,path
 result=dict(status='prior_source_proof_registry_status_and_builds_preserved',baseline_commit='30002ed530e9acde5ac3e29c99db2621a8d8100e',prior_selected_files=16885,preserved_immutable_selected_files=16881,changed_authorized_status_files=sorted(changed),prior_registry_rows_preserved=1636,result_registry_rows=1640,prior_builds_preserved=2787,qualification_sha256=sha(qp),overall_completion_percent=10,sha256={str(p):sha(p) for p in (Path(__file__),mp,bp,qp)})
 target.write_text(json.dumps(result,indent=2)+'\n');print('PRESERVED2787priorbuilds and all qualification hashes',flush=True)
if __name__=='__main__':main()
