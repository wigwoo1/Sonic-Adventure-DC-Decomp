"""Record partial caller evidence without altering the recovered-routine registry."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def main():
 path=P/'analysis/collision_contact_qualification.json';q=json.loads(path.read_text());assert q['new_original_routine_credit']==q['newly_registered_entries']==0 and q['executed_rows']==1121 and q['executed_conditional_outcomes']==72 and len(q['unaccounted_outcomes'])==2
 for pin,h in q['sha256'].items():assert hashlib.sha256(Path(pin).read_bytes()).hexdigest()==h,pin
 r=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];assert len(r)==1589
 sp=P/'project_status.json';s=json.loads(sp.read_text());data={k:v for k,v in q.items() if k not in ('sha256','observed_angle_wrapper_inputs')};data.update(verification=path.relative_to(P).as_posix(),verification_sha256=hashlib.sha256(path.read_bytes()).hexdigest(),guide='COLLISION_CONTACT.txt');s['verified_progress']['collision_contact_integration']=data;sp.write_text(json.dumps(s,indent=2)+'\n');print('PARTIAL collisioncaller recorded;registrybytesunchanged1586unique/631linked;10%milestones')
if __name__=='__main__':main()
