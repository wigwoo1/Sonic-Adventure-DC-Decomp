"""Real candidate rejection and finite-underflow direction-flip witnesses."""
import hashlib,itertools,json
from pathlib import Path
P=Path(__file__).resolve().parent

def configurations():
    for entry,mode,rm in itertools.product((0x8c075686,0x8c075f8c),(0,4),(0,1)):
        yield dict(kind='candidate_rejection_edge',entry=entry,query_normal=(0,1,0) if entry==0x8c075686 else (0,-1,0),mode=mode,rm=rm)
    for exponent,mode,rm in itertools.product((70,80),(0,4),(0,1)):
        yield dict(kind='underflow_direction_flip',entry=0x8c075f8c,delta=(2,0,-2.0**-exponent),mode=mode,rm=rm)

def main():
    parent=P/'verify_movement_contact_candidates_direct.py';source=parent.read_text()
    marker='movement_contact_candidates_direct_verification.json';assert source.count(marker)==1
    source=source.replace(marker,'movement_contact_candidates_edges_verification.json')
    marker='        seen.update(path)';assert source.count(marker)==1
    added="        if c['kind']=='candidate_rejection_edge':\n            expected=0x8c07573c if c['entry']==q.CLAMP else 0x8c076016\n            require(result['status']==0 and s.r[0]==0 and expected in path,'actual candidate-rejection branch andzero return')\n        else:\n            require(result['status']==0 and 0x8c07631a in path,'actual finite-underflow direction-flip branch')\n            require(vector(m,q.B+4)==(0,0,-1),'independent flipped intersection direction atunit auxiliary speed')\n            require(words(m,q.Q+0x6c)==words(m,q.B+4),'flipped rawvelocity copied intoresponse delta')\n"
    source=source.replace(marker,added+marker)
    ns=dict(__name__='candidate_edges_replay',__file__=str(parent));exec(compile(source,str(parent),'exec'),ns);ns['configurations']=configurations;ns['main']()
    path=P/'analysis/movement_contact_candidates_edges_verification.json';r=json.loads(path.read_text());assert r['original_native_comparisons']==16
    r['scope']='16ordinary-entry witnesses,allactualoriginalmath:clampglobal-reference rejection,responseheight rejection,andfinite delta(2,0,-2^-70/-80). The nonzero cross squared length rounds/flushes tozero,so originalquerynormal fallback is used;negative tiny dot thenexecutes actualdirection reversal. Independentlycheck velocity(0,0,-1),rawdelta copy,rejectionreturns,bank/rounding modes. No suppliedresults/frames or alteredguards.'
    r['sha256'][str(Path(__file__))]=hashlib.sha256(Path(__file__).read_bytes()).hexdigest();path.write_text(json.dumps(r,indent=2)+'\n');print('CANDIDATE EDGE WITNESSES PASS',r['original_native_comparisons'],r['independent_checks'],flush=True)
if __name__=='__main__':main()
