"""Reproduce native diagnostic integration and retain previous source/proofs/builds."""
import ast,hashlib,json,re,subprocess,sys,zipfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 generated=[P/n for n in ('source/sa_movie_diagnostics.c','analysis/movie_diagnostics_translation.json','source/sa_movie_diagnostics_connected.c','source/movie_diagnostics_probe.c','build_movie_diagnostics.ps1','analysis/movie_diagnostics_connection.json','analysis/movie_diagnostics_frontier.json','analysis/movie_diagnostics_alignment_invariant.json','analysis/movie_diagnostics_integer_case_selection.json')]
 before={str(p):digest(p) for p in generated}
 for name in ('recover_movie_diagnostics.py','connect_movie_diagnostics.py','inventory_movie_diagnostics_frontier.py','prove_movie_diagnostics_alignment.py','select_movie_diagnostics_integer_cases.py'):
  subprocess.run([sys.executable,'-B',str(P/name)],check=True)
 assert all(digest(p)==h for p,h in before.items())
 registry=P/'analysis/recovered_routines.json';status=P/'project_status.json';before_register=(registry.read_bytes(),status.read_bytes())
 subprocess.run([sys.executable,'-B',str(P/'register_movie_diagnostics.py')],check=True);assert before_register==(registry.read_bytes(),status.read_bytes())
 names=('recover_movie_diagnostics.py','connect_movie_diagnostics.py','verify_movie_diagnostics.py','verify_movie_diagnostics_prefix.py','verify_movie_diagnostics_regression.py','inventory_movie_diagnostics_frontier.py','qualify_movie_diagnostics.py','register_movie_diagnostics.py','verify_movie_diagnostics_workflow.py','movie_diagnostics_reference_cpu.py','sh4_widen_reference.py','verify_sh4_widen_compare.py','select_movie_diagnostics_integer_cases.py','verify_movie_diagnostics_semantics.py','verify_movie_diagnostics_default_base.py','prove_movie_diagnostics_alignment.py')
 scripts=[P/n for n in names]
 for path in scripts:ast.parse(path.read_text())
 pins={}
 for name in ('movie_diagnostics_translation','movie_diagnostics_connection','movie_diagnostics_verification','movie_diagnostics_prefix_verification','movie_diagnostics_regression_verification','movie_diagnostics_frontier','movie_diagnostics_alignment_invariant','movie_diagnostics_integer_case_selection','movie_diagnostics_semantics_verification','movie_diagnostics_default_base_verification','sh4_widen_compare_verification','movie_diagnostics_qualification','movie_record_constructors_qualification'):
  for path,h in json.loads((P/f'analysis/{name}.json').read_text())['sha256'].items():assert digest(path)==h,path;pins[path]=h
 baseline='92cbbcf5627cfc1fdee37c9150f4d08fdb406f22';checkout=ROOT/'publication/sonic-adventure';git=r'C:\Program Files\Git\cmd\git.exe'
 def historical(path):return subprocess.check_output([git,'-C',str(checkout),'show',baseline+':'+path])
 old_manifest=json.loads(historical('PUBLICATION_MANIFEST.json'));preserved=0
 for row in old_manifest['files']:
  path=row['path']
  if '/source/' not in path and not ('/analysis/' in path and any(x in Path(path).name for x in ('verification','qualification','translation','connection'))):continue
  if path.endswith('analysis/main_dispatch_case_map_verification.json'):continue
  assert digest(ROOT/path)==row['sha256'],path;preserved+=1
 old=json.loads(historical('outputs/sonic-adventure/analysis/recovered_routines.json'))['routines'];current=json.loads(registry.read_text())['routines'];assert len(old)==1374 and len(current)==1376
 index={(r.get('module','1ST_READ.BIN'),r['address'],r['name']):r for r in current}
 for row in old:
  now=index[(row.get('module','1ST_READ.BIN'),row['address'],row['name'])]
  for key,value in row.items():assert now[key]==value,(row['address'],key)
 with zipfile.ZipFile(ROOT/'publication/release-assets/sonic-adventure-windows-research-builds.zip') as z:
  build=json.loads(z.read('BUILD_MANIFEST.json'))['files']
  for row in build:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).read_bytes()==z.read(row['path'])
 ps=[P/'build_movie_diagnostics.ps1']+[P/'ghidra'/n for n in ('export_movie_record_constructors_frontier.ps1','export_movie_record_diagnostics_lower.ps1','export_movie_record_diagnostics_format.ps1','export_movie_diagnostics_primitives.ps1','export_movie_diagnostics_lower.ps1','export_movie_diagnostics_frontier.ps1')]
 command="$ErrorActionPreference='Stop'\nforeach($taskPath in @(%s)){$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile($taskPath,[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}}"%(','.join("'"+str(p)+"'" for p in ps))
 subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
 sources=re.findall(r"'([^']+\.c)'",(P/'build_movie_diagnostics.ps1').read_text());assert len(sources)==len(set(sources))==177
 subprocess.run([r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe','-fsyntax-only','-Wall','-Wextra','-Werror']+[str(P/'source'/n) for n in sources],check=True)
 report=dict(reproduced_generated_artifacts=len(generated),registration_byte_idempotent=True,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(ps),pinned_inputs_current=len(pins),strict_unbounded_sources_checked=len(sources),preserved_baseline_commit=baseline,preserved_source_and_proof_files=preserved,preserved_original_registry_entries=len(old),preserved_build_files=len(build),newly_registered_entries=2,scope='New 177-source DLL built; 396 direct comparisons including default-radix controls, two caller profiles with a newly executed cold replay, 350 regression comparisons and 52815 CPU backend cases pass. Four logger rows/one outcome have an aligned-stack proof. Larger formatters remain partial. All older source/proof/build bytes and canonical fields retained. The complete accumulated suite is not claimed.',sha256={str(p):digest(p) for p in scripts+ps})
 (P/'analysis/movie_diagnostics_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print({k:v for k,v in report.items() if k!='sha256'})
if __name__=='__main__':main()
