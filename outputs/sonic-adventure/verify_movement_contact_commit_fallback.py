"""Ordinary zero-displacement fallback witnesses; no supplied callee results."""
import hashlib,itertools,json
from pathlib import Path
P=Path(__file__).resolve().parent

def configurations():
    for sign,flags,mode,rm in itertools.product((1,-1),(0,0x200000),(0,4),(0,1)):
        yield dict(kind='owner_fallback_accept',entry=0x8c074afc,grounded=1,direction=(0,0,0),aux_velocity=(-sign,0,0),normal=(sign,0,0),flags=flags,mode=mode,rm=rm)

def main():
    parent=P/'verify_movement_contact_commit_direct.py';source=parent.read_text()
    marker='movement_contact_commit_direct_verification.json';assert source.count(marker)==1
    source=source.replace(marker,'movement_contact_commit_fallback_verification.json')
    ns=dict(__name__='contact_commit_fallback_replay',__file__=str(parent));exec(compile(source,str(parent),'exec'),ns)
    ns['configurations']=configurations;ns['main']()
    path=P/'analysis/movement_contact_commit_fallback_verification.json';r=json.loads(path.read_text())
    assert r['original_native_comparisons']==16
    edge=['0x8c074bce','0x8c074bd4']
    owner=next(x for x in r['coverage_by_routine'] if x['address']=='0x8c074afc');assert edge not in owner['missing_outcomes']
    for case in r['cases']:
        assert case['status']==0 and case['return_value']==1
        assert any(b['target']=='0x8c076640' and b['pr']=='0x8c074be2' for b in case['boundaries'])
    r['scope']='Sixteen genuine zero-displacement witnesses use the negative auxiliary velocity as fallback. Equal unit-axis normal accepts it and executes original074bce->074bd4 and actual076640 child. Both banks/rounding modes andflags tested;independent commit/status contracts unchanged. No supplied game results,patchedframes oridealized numerical predicates.'
    r['sha256'][str(Path(__file__))]=hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
    path.write_text(json.dumps(r,indent=2)+'\n');print('FALLBACK WITNESSES PASS',r['original_native_comparisons'],r['independent_checks'],flush=True)
if __name__=='__main__':main()
