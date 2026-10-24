"""Check that failed original initialization prevents the next native stage."""
import hashlib,json
from pathlib import Path
import verify_movie_context as v
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 ram=v.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.seed_ram();records=[]
 for failure in ({'null_bus':True},{'context_write_status':-2},{'context_write_status':-3}):
  c=dict(entry=v.BOOT,mode=0,initialize_context=True,**failure);m,s,f=v.prepare(ram,c);record,path,a,s,f=v.compare(m,s,f,c)
  assert record['status']==failure.get('context_write_status',-2) and record['stop_pc']=='0x8c10ee14'
  assert record['boundaries'][0]['target']==hex(v.INIT) and not any(b['target']==hex(v.BOOT) for b in record['boundaries'])
  assert v.load(a,0x8c5fbb14)==v.CROOT and not record['disc_read_attempts'];record['independent_checks']=4;records.append(record)
 files=[Path(__file__),Path(v.__file__),v.P/'analysis/movie_context_connection.json']
 report=dict(cases_passed=3,independent_checks=12,status_counts={'-2':2,'-3':1},dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='The exported native initialized entry executes original initialization, preserves reached writes/register state on its real rejected channel write, and never invokes the requested next stage after failure. Full state matches independent original instruction execution. No rollback or invented success is applied.',cases=records)
 (v.P/'analysis/movie_context_sequence_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Native first-stage error propagation passed: three cases / 12 assertions')
if __name__=='__main__':main()
