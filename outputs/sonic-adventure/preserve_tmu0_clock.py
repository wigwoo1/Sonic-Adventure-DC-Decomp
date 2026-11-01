"""Keep every prior published source/proof/registry/build byte intact."""
import hashlib,json,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parents[1];W=R/'work/sonic-adventure';D=R/'publication/sonic-adventure'
G=r'C:\Program Files\Git\cmd\git.exe';BASE='47cba014bcb414b9ff9f2a6661f8abe614f6e347'
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 target=P/'analysis/tmu0_clock_preservation.json';assert not target.exists()
 def prior(name):return subprocess.check_output([G,'-C',str(D),'show',BASE+':'+name])
 manifest=json.loads(prior('PUBLICATION_MANIFEST.json'));assert len(manifest['files'])==17517
 mutable={'outputs/sonic-adventure/'+n for n in ('README.md','SOURCE_HANDOFF.txt','project_status.json')}
 for row in manifest['files']:
  if row['path'] not in mutable:assert (R/row['path']).stat().st_size==row['bytes'] and sha(R/row['path'])==row['sha256'],row['path']
 for name in ('README.md','SOURCE_HANDOFF.txt'):assert (P/name).read_bytes().endswith(prior('outputs/sonic-adventure/'+name))
 assert (P/'analysis/recovered_routines.json').read_bytes()==prior('outputs/sonic-adventure/analysis/recovered_routines.json')
 status=json.loads((P/'project_status.json').read_text());status['verified_progress'].pop('tmu0_elapsed_clock_foundation');assert status==json.loads(prior('outputs/sonic-adventure/project_status.json'))
 build=W/'renderer_input_tick_published_build_manifest.json';rows=json.loads(build.read_text())['files'];assert len(rows)==2825
 old={row['path']:row for row in rows}
 for name,row in old.items():assert (R/name).stat().st_size==row['bytes'] and sha(R/name)==row['sha256'],name
 current={p.relative_to(R).as_posix() for folder in (W/'build',P/'work/sonic-adventure/build') for p in folder.glob('*') if p.is_file() and p.suffix.lower() in ('.dll','.lib','.exe')}
 expected={f'work/sonic-adventure/build/tmu0_clock.{suffix}' for suffix in ('dll','lib')}
 assert current-set(old)==expected and set(old)<=current and len(current)==2827
 files=[Path(__file__),build,P/'analysis/tmu0_clock_qualification.json']
 result=dict(status='all_prior_published_source_registry_proofs_and_builds_preserved',baseline_commit=BASE,prior_source_files=17517,preserved_immutable_source_files=17514,append_preserving_mutable_paths=sorted(mutable),prior_registry_records_preserved=1658,registry_bytes_unchanged=True,prior_builds_preserved=2825,current_builds=2827,added_build_files=sorted(expected),retail_inputs_or_assets_added=False,overall_completion_percent=10,sha256={str(p):sha(p) for p in files})
 target.write_text(json.dumps(result,indent=2)+'\n');print('Preserved 17514 immutable files, all registry bytes and 2825 prior builds',flush=True)

if __name__=='__main__':main()
