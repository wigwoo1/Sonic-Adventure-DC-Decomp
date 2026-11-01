"""Preserve the current public source, proof records and research binaries."""
import hashlib,json,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parents[1];W=R/'work/sonic-adventure';D=R/'publication/sonic-adventure'
BASE='bfcd7ed79723f1de305d0685ada32f86ece5faf7';G=r'C:\Program Files\Git\cmd\git.exe'


def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def main():
 target=P/'analysis/renderer_filesystem_callback_preservation.json';assert not target.exists()
 def prior(path):return subprocess.check_output([G,'-C',str(D),'show',BASE+':'+path])
 source=W/'renderer_filesystem_callback_prior_source_manifest.json';manifest=json.loads(source.read_text());assert len(manifest['files'])==17409
 mutable={'outputs/sonic-adventure/'+n for n in ('README.md','SOURCE_HANDOFF.txt','project_status.json','analysis/recovered_routines.json')}
 for row in manifest['files']:
  if row['path'] not in mutable:assert (R/row['path']).stat().st_size==row['bytes'] and sha(R/row['path'])==row['sha256'],row['path']
 for name in ('README.md','SOURCE_HANDOFF.txt'):assert (P/name).read_bytes().endswith(prior('outputs/sonic-adventure/'+name))
 before=json.loads(prior('outputs/sonic-adventure/analysis/recovered_routines.json'));after=json.loads((P/'analysis/recovered_routines.json').read_text())
 assert len(before['routines'])==1655 and after['routines'][:1655]==before['routines'] and len(after['routines'])==1656
 after['routines']=after['routines'][:1655];assert after==before
 after=json.loads((P/'project_status.json').read_text());after['verified_progress'].pop('renderer_filesystem_callback_and_live_IRQ_observer');assert after==json.loads(prior('outputs/sonic-adventure/project_status.json'))
 build=W/'renderer_filesystem_callback_prior_build_manifest.json';rows=json.loads(build.read_text())['files'];assert len(rows)==2819
 old={row['path']:row for row in rows}
 for name,row in old.items():assert (R/name).stat().st_size==row['bytes'] and sha(R/name)==row['sha256'],name
 current={p.relative_to(R).as_posix() for folder in (W/'build',P/'work/sonic-adventure/build') for p in folder.glob('*') if p.is_file() and p.suffix.lower() in ('.dll','.lib','.exe')}
 expected={f'work/sonic-adventure/build/renderer_filesystem_callback.{suffix}' for suffix in ('dll','lib')}
 assert current-set(old)==expected and set(old)<=current and len(current)==2821
 files=[Path(__file__),source,build,P/'analysis/renderer_filesystem_callback_qualification.json']
 result=dict(status='all_prior_published_source_proofs_and_build_bytes_preserved',baseline_commit=BASE,prior_source_files=17409,preserved_immutable_source_files=17405,append_preserving_mutable_paths=sorted(mutable),prior_registry_records_preserved=1655,current_registry_entries=1656,prior_builds_preserved=2819,current_builds=2821,added_build_files=sorted(expected),retail_inputs_or_assets_added=False,overall_completion_percent=10,sha256={str(p):sha(p) for p in files})
 target.write_text(json.dumps(result,indent=2)+'\n');print('Preserved17405immutable source files,1655registry records and2819priorbuilds',flush=True)


if __name__=='__main__':main()
