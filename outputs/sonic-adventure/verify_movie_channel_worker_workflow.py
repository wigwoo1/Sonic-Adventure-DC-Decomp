"""Reproduction, pinned evidence and preservation for the partial worker stage."""
import ast,hashlib,json,re,subprocess,sys
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];BASELINE='94af7e7aeb5876da9dcc8b5fd7e2c17a759105f6'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 generated=[P/n for n in ('source/sa_movie_channel_worker.c','analysis/movie_channel_worker_translation.json','source/sa_movie_channel_worker_connected.c','source/movie_channel_worker_probe.c','build_movie_channel_worker.ps1','analysis/movie_channel_worker_connection.json','analysis/movie_channel_worker_contract_audit.json')];before={str(p):digest(p) for p in generated}
 for name in ('recover_movie_channel_worker.py','connect_movie_channel_worker.py','audit_movie_channel_worker.py'):subprocess.run([sys.executable,'-B',str(P/name)],check=True)
 assert all(digest(p)==h for p,h in before.items())
 scripts=[P/n for n in ('recover_movie_channel_worker.py','connect_movie_channel_worker.py','movie_channel_worker_reference.py','write_watch_channel_worker.py','verify_movie_channel_worker.py','verify_movie_channel_worker_pool.py','verify_movie_channel_worker_irq.py','verify_movie_channel_worker_retained.py','audit_movie_channel_worker.py','qualify_movie_channel_worker.py','verify_movie_channel_worker_workflow.py')]
 for p in scripts:ast.parse(p.read_text())
 pins={}
 for name in ('translation','connection','verification','pool_verification','irq_verification','retained_verification','contract_audit','qualification'):
  report=json.loads((P/f'analysis/movie_channel_worker_{name}.json').read_text())
  for p,h in report['sha256'].items():assert digest(p)==h,(name,p);pins[p]=h
 import movie_channel_worker_reference as v
 import write_watch_channel_worker as fast
 assert v.code==v.old.code
 for name in ('REGISTER_ROWS','CONTEXT_ROWS','CD_FPU_ROWS','AI_FPU_ROWS','GUARDS','GUARD_ENTRY'):assert v.namespace[name]==v.old.namespace[name]
 assert all(getattr(v,n)==value for n,value in v.INHERITED_INTEGER_CONSTANTS.items())
 assert fast.address_compare_code==fast.compare_code.replace(' return dict(',' return dict(final_macl=ml.value,')
 checkout=ROOT/'publication/sonic-adventure';git=r'C:\Program Files\Git\cmd\git.exe'
 def historical(path):return subprocess.check_output([git,'-C',str(checkout),'show',BASELINE+':'+path])
 old=json.loads(historical('PUBLICATION_MANIFEST.json'));assert len(old['files'])==13914
 mutable=['outputs/sonic-adventure/README.md','outputs/sonic-adventure/SOURCE_HANDOFF.txt']
 preserved=0
 for row in old['files']:
  if row['path'] in mutable:continue
  assert digest(ROOT/row['path'])==row['sha256'],row['path'];preserved+=1
 assert preserved==13912
 for name in ('outputs/sonic-adventure/analysis/recovered_routines.json','outputs/sonic-adventure/project_status.json'):assert (ROOT/name).read_bytes()==historical(name)
 previous=json.loads((ROOT/'work/sonic-adventure/channel_worker_previous_build_manifest.json').read_text());assert len(previous['files'])==2669
 for row in previous['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
 ps=P/'build_movie_channel_worker.ps1';sources=re.findall(r"'([^']+\.c)'",ps.read_text());assert len(sources)==len(set(sources))==219
 powershell=[ps,P/'ghidra/export_movie_channel_worker.ps1',P/'ghidra/export_movie_channel_decoders.ps1']
 for script in powershell:
  command="$taskTokens=$null;$taskErrors=$null;[void][System.Management.Automation.Language.Parser]::ParseFile('%s',[ref]$taskTokens,[ref]$taskErrors);if($taskErrors.Count){throw ($taskErrors|Out-String)}"%str(script);subprocess.run(['powershell.exe','-NoProfile','-Command',command],check=True)
 strict_path=ROOT/'work/sonic-adventure/channel_worker_unbounded_receipt.json';strict=json.loads(strict_path.read_text());assert strict['source_sha256']=={str(P/'source'/n):digest(P/'source'/n) for n in sources} and strict['exit_code']==0 and strict['flags']==['-fsyntax-only','-Wall','-Wextra','-Werror']
 q=json.loads((P/'analysis/movie_channel_worker_qualification.json').read_text());assert q['newly_registered_entries']==0 and q['worker_fully_qualified'] is False
 files=[Path(__file__),strict_path,P/'analysis/movie_channel_worker_qualification.json']
 report=dict(reproduced_generated_artifacts=7,python_scripts_parsed=len(scripts),powershell_scripts_parsed=len(powershell),pinned_inputs_current=len(pins),inherited_integer_constants_preserved=len(v.INHERITED_INTEGER_CONSTANTS),original_model_execution_code_identical=True,inherited_SR_FD_guards_preserved=True,existing_continuation_routes_preserved=True,observer_only_adds_final_MACL_read=True,compiled_source_count=219,strict_unbounded_sources_checked=219,preserved_baseline_commit=BASELINE,preserved_prior_selected_files=preserved,reviewed_mutable_paths=mutable,registry_and_project_status_byte_identical=True,new_routine_credit=0,preserved_build_files=2669,additional_build_files=['work/sonic-adventure/build/movie_channel_worker.dll','work/sonic-adventure/build/movie_channel_worker.lib'],sha256={str(p):digest(p) for p in files},scope='New7 generated files reproduce byteidentically;219 strictunbounded C sources compile. Prior13912 selected source/proof files bytepreserved;onlyREADME/handoff prefixes updated. Prior2669build bytes preserved;registry/status/requirements unchanged. Exact oldmodel execution code/guards/runtime/ABI plus explicitrows/routes;no fakegame helper/counter/context repair. Fullworkerqualification remainsincomplete;testscover declaredoriginal paths only.')
 (P/'analysis/movie_channel_worker_workflow_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('CHANNEL WORKFLOW PASS',preserved,'preserved source/proof files;',len(pins),'pins')
if __name__=='__main__':main()
