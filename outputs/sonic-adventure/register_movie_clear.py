"""Credit the clear body once; retain all canonical older entries."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 qp=P/'analysis/movie_clear_qualification.json';q=json.loads(qp.read_text());assert q['newly_registered_entries']==1 and q['qualified_rows']==234
 for path,h in q['sha256'].items():assert digest(path)==h,path
 rp=P/'analysis/recovered_routines.json';registry=json.loads(rp.read_text())
 routine=q['routines'][0];a,b=(int(routine[k],16) for k in ('address','end_exclusive'));raw=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
 assert hashlib.sha256(raw[a-0x8c010000:b-0x8c010000]).hexdigest()==routine['retail_span_sha256']
 row=dict(module='1ST_READ.BIN',name=routine['name'],address=routine['address'],end_exclusive=routine['end_exclusive'],source=routine['source'],retail_span_sha256=routine['retail_span_sha256'],verification=qp.relative_to(P).as_posix(),status='Qualified original register initialization and complete eight-MiB clear: all 2,097,152 native helper calls execute. 233 instruction rows and 31 branch outcomes execute; one row/outcome is separately proved unreachable. Verification-only interpreter and parents earn no duplicate credit.',limitations=q['limitations'])
 old=next((r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==row['address']),None)
 if old is None:registry['routines'].append(row)
 else:assert old==row
 integration={k:v for k,v in q.items() if k not in ('routines','sha256')};integration.update(verification=qp.relative_to(P).as_posix(),verification_sha256=digest(qp),guide='MOVIE_CLEAR.txt',adapter_source='source/sa_movie_clear_connected.c',adapter_source_sha256=digest(P/'source/sa_movie_clear_connected.c'))
 for address in ('0x8c010180','0x8c65253e'):
  r=next(r for r in registry['routines'] if r.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and r['address']==address);r['movie_clear_integration']=integration
 rp.write_text(json.dumps(registry,indent=2)+'\n')
 sp=P/'project_status.json';status=json.loads(sp.read_text());status.setdefault('verified_progress',{})['movie_clear_integration']=integration;status['verified_progress']['recovered_buildable_routines']=status['recovered_buildable_routines']=len(registry['routines']);sp.write_text(json.dumps(status,indent=2)+'\n');print('Registered full original clear; entries',len(registry['routines']))
if __name__=='__main__':main()
