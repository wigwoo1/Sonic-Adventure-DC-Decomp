"""Append the newly qualified scalar-profile owner; preserve every historical row."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def main():
    qp=P/'analysis/movement_orientation_fold_qualification.json';q=json.loads(qp.read_text())
    assert q['new_original_routine_credit']==1 and q['newly_qualified_original_rows']==157
    for name,h in q['sha256'].items():assert digest(name)==h,name
    rp=P/'analysis/recovered_routines.json';r=json.loads(rp.read_text());previous=r['routines'][:1612]
    assert len(previous)==1612
    row={k:q['routine'][k] for k in ('module','name','address','end_exclusive','source','retail_span_sha256')}
    row.update(verification=qp.relative_to(P).as_posix(),status='Original074ee4 software-qualified only for the stated fixed-original-math scalar profile;154executed+3range-accounted rows,23executed+1bounded outcomes. Broader precision/mathmutation/physical hardware remain open.',limitations=q['limitations'])
    old=next((x for x in r['routines'] if x.get('module','1ST_READ.BIN')=='1ST_READ.BIN' and x['address']==row['address']),None)
    if old is None:r['routines'].append(row)
    else:assert old==row
    assert len(r['routines'])==1613 and r['routines'][:1612]==previous
    assert len({(x.get('module','1ST_READ.BIN'),x['address']) for x in r['routines']})==1610
    rp.write_text(json.dumps(r,indent=2)+'\n')
    sp=P/'project_status.json';s=json.loads(sp.read_text());data={k:v for k,v in q.items() if k not in ('sha256','routine')}
    data.update(verification=qp.relative_to(P).as_posix(),verification_sha256=digest(qp),guide='MOVEMENT_ORIENTATION_FOLD.txt')
    s['verified_progress']['movement_orientation_fold_integration']=data
    s['verified_progress']['recovered_buildable_routines']=s['recovered_buildable_routines']=1613
    sp.write_text(json.dumps(s,indent=2)+'\n')
    print('REGISTERED1613entries/1610unique/657linked;scalar-profile qualification;goalACTIVE/10%',flush=True)
if __name__=='__main__':main()
