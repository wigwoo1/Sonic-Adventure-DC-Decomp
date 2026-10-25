"""Preserve the previous DLL; remove only its clear-loop research bound."""
import copy,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 parent=P/'source/sa_movie_driver.c';text=parent.read_text()
 bound='L8c665998:\n#ifdef SA_BOUND_RETAIL_STALLS\n *stop=0x8c665998u;if(!(*sr&1u) && ++driver_clear_visits>=2u)return -4;\n#endif\n '
 assert text.count(bound)==1;text=text.replace(bound,'L8c665998: ')
 text=text.replace('sa_movie_driver_','sa_movie_clear_');source=P/'source/sa_movie_clear.c';source.write_text(text)
 old=P/'analysis/movie_driver_translation.json';report=copy.deepcopy(json.loads(old.read_text()))
 report['source']=source.relative_to(P).as_posix()
 for r in report['routines']:
  r['name']=r['name'].replace('sa_movie_driver_','sa_movie_clear_')
  if r['address']=='0x8c6656e2':assert r.pop('bounded_research_waits')[0]['pc']=='0x8c665998'
 report['scope']='Same original 23 proposals, with the full finite VRAM clear unbounded. Hardware-failure self-loop remains explicitly bounded only in research builds. Verification support is separate from recovered game code.'
 report['sha256'].update({str(p):digest(p) for p in (Path(__file__),parent,old,source)})
 (P/'analysis/movie_clear_translation.json').write_text(json.dumps(report,indent=2)+'\n')
 print('Generated full original clear variant; all previous source and DLL bytes retained')
if __name__=='__main__':main()
