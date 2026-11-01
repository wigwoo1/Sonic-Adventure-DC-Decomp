"""Audit preserved published bytes before preparing the next public snapshot."""
import hashlib,json,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parents[1];W=R/'work/sonic-adventure';D=R/'publication/sonic-adventure'
BASE='e8d9501799ab5f718d99bb1c132e96ec55d0542c';G=r'C:\Program Files\Git\cmd\git.exe'


def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def main():
 target=P/'analysis/renderer_list_callbacks_preservation.json';assert not target.exists()
 def prior(name):return subprocess.check_output([G,'-C',str(D),'show',BASE+':'+name])
 m=W/'renderer_frame_signal_prior_source_manifest.json';manifest=json.loads(m.read_text());assert len(manifest['files'])==17292
 mutable={'outputs/sonic-adventure/'+n for n in ('README.md','SOURCE_HANDOFF.txt','project_status.json','analysis/recovered_routines.json')}
 for row in manifest['files']:
  if row['path'] not in mutable:assert (R/row['path']).stat().st_size==row['bytes'] and sha(R/row['path'])==row['sha256'],row['path']
 for name in ('README.md','SOURCE_HANDOFF.txt'):assert (P/name).read_bytes().endswith(prior('outputs/sonic-adventure/'+name))
 old=json.loads(prior('outputs/sonic-adventure/analysis/recovered_routines.json'));new=json.loads((P/'analysis/recovered_routines.json').read_text())
 assert new['routines'][:1652]==old['routines'] and len(new['routines'])==1655
 new['routines']=new['routines'][:1652];assert new==old
 new=json.loads((P/'project_status.json').read_text());new['verified_progress'].pop('renderer_list_callbacks_and_frame_events');assert new==json.loads(prior('outputs/sonic-adventure/project_status.json'))
 b=W/'renderer_frame_signal_prior_build_manifest.json';builds=json.loads(b.read_text());assert len(builds['files'])==2803
 before={row['path']:row for row in builds['files']}
 for name,row in before.items():assert (R/name).stat().st_size==row['bytes'] and sha(R/name)==row['sha256'],name
 current={p.relative_to(R).as_posix() for folder in (W/'build',P/'work/sonic-adventure/build') for p in folder.glob('*') if p.is_file() and p.suffix.lower() in ('.dll','.lib','.exe')}
 expected={f'work/sonic-adventure/build/{name}.{suffix}' for name in ('renderer_frame_irq','renderer_frame_signal','renderer_frame_events','renderer_frame_events_safe','renderer_frame_events_packed','renderer_frame_events_eol','renderer_list_completion','renderer_list_callbacks') for suffix in ('dll','lib')}
 assert current-set(before)==expected and set(before)<=current and len(current)==2819
 files=[Path(__file__),m,b,P/'analysis/renderer_list_callbacks_qualification.json']
 result=dict(status='all_prior_published_source_proof_build_bytes_preserved',baseline_commit=BASE,prior_source_files=17292,preserved_immutable_source_files=17288,append_preserving_mutable_paths=sorted(mutable),prior_registry_entries_preserved=1652,current_registry_entries=1655,prior_build_files_preserved=2803,current_build_files=2819,added_research_build_files=sorted(expected),retail_inputs_or_assets_added=False,overall_completion_percent=10,sha256={str(p):sha(p) for p in files})
 target.write_text(json.dumps(result,indent=2)+'\n');print('Preserved17288immutable published sources,1652registry records and2803priorbuilds',flush=True)


if __name__=='__main__':main()
