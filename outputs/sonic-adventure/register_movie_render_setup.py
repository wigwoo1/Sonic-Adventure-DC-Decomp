"""Attach renderer integration evidence without duplicate routine credit."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 qp=P/'analysis/movie_render_setup_qualification.json';q=json.loads(qp.read_text());assert q['newly_registered_entries']==0 and q['integrated_instruction_rows']==154
 for path,h in q['sha256'].items():assert digest(path)==h,path
 assert digest(ROOT/'work/sonic-adventure/build/movie_render_setup.dll')==q['dll_sha256']
 integration={k:v for k,v in q.items() if k not in ('routines','sha256')}
 integration.update(verification=qp.relative_to(P).as_posix(),verification_sha256=digest(qp),adapter_source='source/sa_movie_render_setup_connected.c',adapter_source_sha256=digest(P/'source/sa_movie_render_setup_connected.c'),guide='MOVIE_RENDER_SETUP.txt')
 rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text());before=len(registry['routines'])
 for address in [r['address'] for r in q['routines']]+q['reused_connected_bodies']:
  rows=[r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==address];assert len(rows)==1,address
  rows[0]['movie_render_setup_integration']=integration
 assert len(registry['routines'])==before
 rp.write_text(json.dumps(registry,indent=2)+'\n')
 sp=P/'project_status.json';status=json.loads(sp.read_text());status.setdefault('verified_progress',{})['movie_render_setup_integration']=integration
 sp.write_text(json.dumps(status,indent=2)+'\n');print('Attached renderer integration; no new credit; entries',before)
if __name__=='__main__':main()
