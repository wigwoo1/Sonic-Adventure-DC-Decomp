"""Fresh existing root/constructor comparisons in the new 202-source DLL."""
import hashlib,json
from pathlib import Path
import write_watch_graphics_handoff as fast
P=Path(__file__).resolve().parent;ROOT=P.parents[1];v=fast.old
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 reports=[];files=[Path(__file__),Path(fast.__file__),Path(v.__file__),P/'analysis/movie_graphics_handoff_connection.json']
 for name,expected_cases,expected_checks in (('root_arithmetic',66,1334),('constructor',28,588)):
  parent=P/f'verify_movie_graphics_{name}.py';target=P/f'analysis/movie_graphics_handoff_{name}_regression.json';source=parent.read_text().replace(f'movie_graphics_{name}_verification.json',target.name)
  if name=='root_arithmetic':
   marker="body=next(b for b in v.manifest['routines'] if int(b['address'],16)==ENTRY)";assert source.count(marker)==1;source=source.replace(marker,"body=next(b for b in v.old.manifest['routines'] if int(b['address'],16)==ENTRY)")
  namespace=dict(__name__='fresh_accounting_retained_suite',__file__=str(Path(__file__)))
  exec(compile(source,str(parent),'exec'),namespace);namespace.update(fast=fast,v=v);namespace['main']()
  report=json.loads(target.read_text());assert (report['cases_passed'],report['independent_checks'])==(expected_cases,expected_checks) and report['dll_sha256']==digest(v.effects.DLL)
  report['sha256'][str(parent)]=digest(parent);report['scope']='Fresh202-source replay of unchanged '+name+' input/oracle suite. Oldsource/proof/report bytes untouched;existing outcomes/limitations retained. NewDLL actually selected;no duplicate routine credit. '+report['scope'];target.write_text(json.dumps(report,indent=2)+'\n')
  files.extend((parent,target));reports.append(dict(suite=name,comparisons=expected_cases,independent_checks=expected_checks,report=target.relative_to(P).as_posix()))
 result=dict(total_original_native_comparisons=94,independent_checks=1922,suites=reports,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='94fresh original/native root/constructor comparisons in202-source integration,witholdindependentoracles andfailureprefixes. Previouscanonicalproofreports/sourcefilespreserved. Earlierothercomparisonsretainednotrerun;no newfullrootrecoverycredit. Fullconfiguredstartup checkedseparately.')
 (P/'analysis/movie_graphics_handoff_retained_verification.json').write_text(json.dumps(result,indent=2)+'\n');print('94fresh retained comparisons/1922checks passed',flush=True)
if __name__=='__main__':main()
