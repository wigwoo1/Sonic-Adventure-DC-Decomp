"""Preserve the prior public source, routine records, proofs and build hashes."""
import hashlib,json,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parents[1];W=R/'work/sonic-adventure';D=R/'publication/sonic-adventure'
G=r'C:\Program Files\Git\cmd\git.exe';BASE='c5289353c2320cad2ebbc60d7b29fb8f21c61435'
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 target=P/'analysis/renderer_input_tick_preservation.json';assert not target.exists()
 def prior(name):return subprocess.check_output([G,'-C',str(D),'show',BASE+':'+name])
 manifest=json.loads(prior('PUBLICATION_MANIFEST.json'));assert len(manifest['files'])==17457
 mutable={'outputs/sonic-adventure/'+n for n in ('README.md','SOURCE_HANDOFF.txt','project_status.json','analysis/recovered_routines.json')}
 for row in manifest['files']:
  if row['path'] not in mutable:assert (R/row['path']).stat().st_size==row['bytes'] and sha(R/row['path'])==row['sha256'],row['path']
 for name in ('README.md','SOURCE_HANDOFF.txt'):assert (P/name).read_bytes().endswith(prior('outputs/sonic-adventure/'+name))
 before=json.loads(prior('outputs/sonic-adventure/analysis/recovered_routines.json'));after=json.loads((P/'analysis/recovered_routines.json').read_text())
 assert len(before['routines'])==1656 and len(after['routines'])==1658 and after['routines'][:1656]==before['routines']
 after['routines']=after['routines'][:1656];assert after==before
 after=json.loads((P/'project_status.json').read_text());after['verified_progress'].pop('renderer_input_context_and_tick_callbacks');assert after==json.loads(prior('outputs/sonic-adventure/project_status.json'))
 build=W/'renderer_filesystem_callback_published_build_manifest.json';rows=json.loads(build.read_text())['files'];assert len(rows)==2821
 old={row['path']:row for row in rows}
 for name,row in old.items():assert (R/name).stat().st_size==row['bytes'] and sha(R/name)==row['sha256'],name
 current={p.relative_to(R).as_posix() for folder in (W/'build',P/'work/sonic-adventure/build') for p in folder.glob('*') if p.is_file() and p.suffix.lower() in ('.dll','.lib','.exe')}
 expected={f'work/sonic-adventure/build/{name}.{suffix}' for name in ('renderer_input_tick','renderer_input_tick_mmio') for suffix in ('dll','lib')}
 assert current-set(old)==expected and set(old)<=current and len(current)==2825
 files=[Path(__file__),build,P/'analysis/renderer_input_tick_qualification.json']
 result=dict(status='all_prior_published_source_proofs_registry_and_builds_preserved',baseline_commit=BASE,prior_source_files=17457,preserved_immutable_source_files=17453,append_preserving_mutable_paths=sorted(mutable),prior_registry_records_preserved=1656,current_registry_entries=1658,prior_builds_preserved=2821,current_builds=2825,added_build_files=sorted(expected),retail_inputs_or_assets_added=False,overall_completion_percent=10,sha256={str(p):sha(p) for p in files})
 target.write_text(json.dumps(result,indent=2)+'\n');print('Preserved 17453 immutable prior files, 1656 old registry records and all 2821 old builds',flush=True)

if __name__=='__main__':main()
