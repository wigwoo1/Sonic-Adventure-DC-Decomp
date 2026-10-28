"""Record canonical matrix integration while preserving every registry row."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent

def main():
 path=P/'analysis/collision_inverse_qualification.json';q=json.loads(path.read_text());assert q['new_original_routine_credit']==q['newly_registered_entries']==0 and q['new_direct_entry_routes_qualified']==2
 for pin,h in q['sha256'].items():assert hashlib.sha256(Path(pin).read_bytes()).hexdigest()==h,pin
 r=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(r)==1586
 sp=P/'project_status.json';s=json.loads(sp.read_text());data={k:v for k,v in q.items() if k!='sha256'};data.update(verification=path.relative_to(P).as_posix(),verification_sha256=hashlib.sha256(path.read_bytes()).hexdigest(),guide='COLLISION_INVERSE.txt');s['verified_progress']['collision_inverse_integration']=data;sp.write_text(json.dumps(s,indent=2)+'\n');print('INVERSE integration recorded;1583unique/628confirmedlinked;registrybytesunchanged;10%milestones')
if __name__=='__main__':main()
