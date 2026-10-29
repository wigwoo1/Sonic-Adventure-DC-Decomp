"""Finite ordinary-entry witnesses for the original unset minimum-index path."""
import hashlib,itertools,json
from pathlib import Path
P=Path(__file__).resolve().parent

def configurations():
    for exponent,mode,rm in itertools.product((70,80),(0,4),(0,1)):
        yield dict(kind='unset_minimum_zero_commit',entry=0x8c075686,count=1,motion=(-2.0**-exponent,0,0),contact_point=(1024,0,0),padding_word=0xbf800000,mode=mode,rm=rm)

def main():
    parent=P/'verify_movement_contact_candidates_direct.py';source=parent.read_text()
    marker='movement_contact_candidates_direct_verification.json';assert source.count(marker)==1
    source=source.replace(marker,'movement_contact_candidates_minimum_verification.json')
    marker="                    if c['kind']=='candidate_selection':";assert source.count(marker)==1
    added="                    if c['kind']=='unset_minimum_zero_commit':\n                        require(s.r[0]==1 and words(m,q.Q+0x10)==(0,0,0),'independent original zero-motion commit and return')\n                        require(0x8c0757dc in path and 0x8c0757a4 not in path,'genuine zero-length branch without a selected minimum')\n"
    source=source.replace(marker,added+marker)
    ns=dict(__name__='candidate_minimum_replay',__file__=str(parent));exec(compile(source,str(parent),'exec'),ns);ns['configurations']=configurations;ns['main']()
    path=P/'analysis/movement_contact_candidates_minimum_verification.json';r=json.loads(path.read_text());assert r['original_native_comparisons']==8
    owner=next(x for x in r['coverage_by_routine'] if x['address']=='0x8c075686');assert ['0x8c0757da','0x8c0757dc'] not in owner['missing_outcomes']
    for case in r['cases']:assert case['status']==0 and case['return_value']==1
    r['scope']='Eight finite ordinary-entry witnesses:negative2^-70/2^-80 motion has zero rounded squared length but negative actual dot;candidate distance1024 never beats original1000 minimum initializer,leaving index-1. Declared pre-entry caller-memory paddingword-1 is read by original distance[-1]. It permits zero-motion commit. Bothbanks/rounding modes. No original return/math output or mid-execution frame supplied;no guards/reference/native change.'
    r['sha256'][str(Path(__file__))]=hashlib.sha256(Path(__file__).read_bytes()).hexdigest();path.write_text(json.dumps(r,indent=2)+'\n');print('UNSET MINIMUM WITNESSES PASS',r['original_native_comparisons'],r['independent_checks'],flush=True)
if __name__=='__main__':main()
