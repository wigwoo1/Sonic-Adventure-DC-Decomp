"""Fresh retained 222-source controls without overwriting historical evidence."""
import hashlib,json,sys
from pathlib import Path
P=Path(__file__).resolve().parent

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 records=[];files=[Path(__file__)]
 for suite in ('units','nested','worker','irq','retained'):
  parent=P/f'verify_movie_channel_io_{suite}.py';source=parent.read_text()
  source=source.replace('movie_channel_io_fixtures','movie_channel_request_task_fixtures').replace('write_watch_channel_io','write_watch_channel_request')
  # All generated proof names and source-stage labels change. No historical
  # source, report, or build is overwritten by this replay.
  source=source.replace('movie_channel_io_','movie_channel_request_retained_')
  # Change stage labels only; hexadecimal instruction addresses are immutable.
  for before,after in (('in221','in222'),('fresh221','fresh222'),('new221','new222'),('as221_source','as222_source')):source=source.replace(before,after)
  # Connection/guard inputs are the current stage rather than generated names.
  source=source.replace('movie_channel_request_retained_connection.json','movie_channel_request_connection.json').replace('movie_channel_request_retained_returns.json','movie_channel_request_returns.json')
  report=P/'analysis'/f'movie_channel_request_retained_{suite}_{"regression" if suite=="worker" else "verification"}.json'
  if '--remaining-after-worker' not in sys.argv or suite not in ('units','nested','worker'):
   ns=dict(__name__='request_retained_replay',__file__=str(parent));exec(compile(source,str(parent),'exec'),ns);ns['main']()
  # The explicit recovery flag finishes the remaining suites after the reviewed
  # worker proof-filename correction. Every existing proof is still hash checked.
  r=json.loads(report.read_text())
  count=r.get('original_native_comparisons',r.get('total_original_native_comparisons'));assert isinstance(count,int)
  import movie_channel_request_reference as current
  assert r['dll_sha256']==digest(current.effects.DLL)
  for pin,h in r['sha256'].items():
   if pin!=str(Path(__file__)):assert digest(pin)==h,(suite,pin)
  r['sha256'][str(parent)]=digest(parent);r['sha256'][str(Path(__file__))]=digest(Path(__file__));report.write_text(json.dumps(r,indent=2)+'\n')
  records.append(dict(suite=suite,original_native_comparisons=count,independent_checks=r['independent_checks'],proof=report.relative_to(P).as_posix()));files.extend((parent,report));print('REQUEST RETAINED',suite,count,flush=True)
  if '--smoke' in sys.argv:break
 report=dict(original_native_comparisons=sum(r['original_native_comparisons'] for r in records),independent_checks=sum(r['independent_checks'] for r in records),suites=records,sha256={str(p):digest(p) for p in files},scope='Fresh222DLL retainedreadiness/nestedfailure/worker/softwareIRQ/contextSRFD controls. Identicaloldtestsemantics withnewnative/referencebindings andseparateproofpaths. Oldcold/warm evidence retainedhistorical,notnew222startup. No routinecredit forcontrols.')
 (P/'analysis/movie_channel_request_retained_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('REQUEST RETAINED PASS',report['original_native_comparisons'],report['independent_checks'])
if __name__=='__main__':main()
