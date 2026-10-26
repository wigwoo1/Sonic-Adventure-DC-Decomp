"""Fresh original/native root, constructor and accounting suites in 203-source DLL."""
import hashlib,json
from pathlib import Path
import write_watch_render_buffer as fast
P=Path(__file__).resolve().parent;ROOT=P.parents[1];v=fast.old

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 reports=[];files=[Path(__file__),Path(fast.__file__),Path(v.__file__),P/'analysis/movie_render_buffer_connection.json']
 for name,script,expected_cases,expected_checks in (('root_arithmetic','verify_movie_graphics_root_arithmetic.py',66,1334),('constructor','verify_movie_graphics_constructor.py',28,588),('accounting','verify_movie_graphics_handoff.py',137,2181)):
  parent=P/script;target=P/f'analysis/movie_render_buffer_{name}_regression.json';source=parent.read_text()
  original='movie_graphics_handoff_verification.json' if name=='accounting' else f'movie_graphics_{name}_verification.json'
  assert source.count(original)==1;source=source.replace(original,target.name)
  if name=='root_arithmetic':
   marker="body=next(b for b in v.manifest['routines'] if int(b['address'],16)==ENTRY)";assert source.count(marker)==1;source=source.replace(marker,"body=next(b for b in v.old.old.manifest['routines'] if int(b['address'],16)==ENTRY)")
  namespace=dict(__name__='fresh_render_retained_suite',__file__=str(Path(__file__)))
  exec(compile(source,str(parent),'exec'),namespace);namespace.update(fast=fast,v=v);namespace['main']()
  report=json.loads(target.read_text());assert (report['cases_passed'],report['independent_checks'])==(expected_cases,expected_checks) and report['dll_sha256']==digest(v.effects.DLL)
  report['sha256'][str(parent)]=digest(parent);report['scope']='Fresh203-source replay of unchanged '+name+' input/oracle suite. Earlier source/proof/report bytes untouched; no duplicate routine credit. '+report['scope'];target.write_text(json.dumps(report,indent=2)+'\n')
  files.extend((parent,target));reports.append(dict(suite=name,comparisons=expected_cases,independent_checks=expected_checks,report=target.relative_to(P).as_posix()))
 result=dict(total_original_native_comparisons=231,independent_checks=4103,suites=reports,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='231fresh original/native root/constructor/accounting comparisons in203-source integration with unchanged independentoracles. Earlier reports/source preserved; no newcredit. Other prior comparisons retained,notallrerun. Fullstartup verifiedseparately.')
 (P/'analysis/movie_render_buffer_retained_verification.json').write_text(json.dumps(result,indent=2)+'\n');print('231fresh retained comparisons/4103checks passed',flush=True)

if __name__=='__main__':main()
