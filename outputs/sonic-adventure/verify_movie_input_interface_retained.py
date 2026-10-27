"""Fresh retained task/IRQ/context guards; historical proofs remain immutable."""
import ast,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 files=[Path(__file__)];records=[]
 for suite,name in (('irq','verify_movie_channel_io_irq.py'),('context','verify_movie_channel_io_retained.py')):
  parent=P/name;original=parent.read_text();source=original.replace('movie_channel_request_task_fixtures','movie_input_interface_task_fixtures').replace('write_watch_channel_io','write_watch_input_interface')
  source=source.replace('movie_channel_request_tasks_verification.json','movie_input_interface_retained_tasks_verification.json').replace('movie_channel_io_','movie_input_interface_retained_')
  source=source.replace('movie_channel_request_connection.json','movie_input_interface_connection.json').replace('movie_input_interface_retained_connection.json','movie_input_interface_connection.json')
  for before,after in (('in221','in225'),('fresh221','fresh225'),('new221','new225'),('as221_source','as225_source')):source=source.replace(before,after)
  integers=lambda text:[n.value for n in ast.walk(ast.parse(text)) if isinstance(n,ast.Constant) and type(n.value) is int]
  assert integers(source)==integers(original),'Test instruction addresses/numeric expectations changed'
  ns=dict(__name__='input_interface_retained_replay',__file__=str(parent));exec(compile(source,str(parent),'exec'),ns);ns['main']()
  filename='movie_input_interface_retained_'+('retained' if suite=='context' else suite)+'_verification.json';path=P/'analysis'/filename;r=json.loads(path.read_text())
  import movie_input_interface_reference as v
  assert r['dll_sha256']==digest(v.effects.DLL)
  for pin,h in r['sha256'].items():assert digest(pin)==h,(suite,pin)
  r['sha256'][str(parent)]=digest(parent);r['sha256'][str(Path(__file__))]=digest(Path(__file__));path.write_text(json.dumps(r,indent=2)+'\n')
  count=r.get('original_native_comparisons',r.get('total_original_native_comparisons'));records.append(dict(suite=suite,original_native_comparisons=count,independent_checks=r['independent_checks'],proof=path.relative_to(P).as_posix()));files.extend((parent,path));print('INPUT PUMP RETAINED',suite,count,flush=True)
 report=dict(original_native_comparisons=sum(r['original_native_comparisons'] for r in records),independent_checks=sum(r['independent_checks'] for r in records),suites=records,sha256={str(p):digest(p) for p in files},scope='Fresh225-source softwareIRQandcontextSRFD controls;exactpreviousnumericexpectations verified byAST. Newproofpaths;oldproofs/DLLs unchanged. Oldwarm/cold evidencehistorical,notnew225startup;no newroutinecredit.')
 assert (report['original_native_comparisons'],report['independent_checks'])==(83,759)
 (P/'analysis/movie_input_interface_retained_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('INPUT PUMP RETAINED PASS',report['original_native_comparisons'],report['independent_checks'])
if __name__=='__main__':main()
