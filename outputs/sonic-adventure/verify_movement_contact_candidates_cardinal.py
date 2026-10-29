"""Independent cardinal-plane direction and speed contracts for actual response code."""
import hashlib,itertools,json
from pathlib import Path
P=Path(__file__).resolve().parent

def configurations():
    for axis,sign,speed,mode,rm in itertools.product((0,2),(-1,1),(.5,1,2),(0,4),(0,1)):
        normal=(0,0,1) if axis==0 else (1,0,0)
        motion=tuple(-3*x for x in normal);delta=tuple(2*sign if j==axis else 0 for j in range(3))
        yield dict(kind='cardinal_plane_response',entry=0x8c075f8c,normal=normal,motion=motion,delta=delta,aux_velocity=(-speed,0,0),axis=axis,sign=sign,speed=speed,mode=mode,rm=rm)

def main():
    parent=P/'verify_movement_contact_candidates_direct.py';source=parent.read_text()
    marker='movement_contact_candidates_direct_verification.json';assert source.count(marker)==1
    source=source.replace(marker,'movement_contact_candidates_cardinal_verification.json')
    marker="                    require(s.r[0]==(12 if 0x8c076344 in path else 0),'original response returns final offset12 on redirect,zero on earlier exits')";assert source.count(marker)==1
    added="                    if c['kind']=='cardinal_plane_response':\n                        expected=tuple(c['sign']*c['speed'] if j==c['axis'] else 0 for j in range(3))\n                        require(s.r[0]==12 and vector(m,q.B+4)==expected,'independent intersection-axis orientation and preserved cardinal speed')\n                        require(words(m,q.Q+0x6c)==words(m,q.B+4),'response delta copies committed auxiliary velocity exactly')\n                        for offset in (4,0x10,0x60):require(words(m,q.Q+offset)==words(before,q.Q+offset),'response preserves query position,motion andanchor')\n"
    source=source.replace(marker,added+marker)
    ns=dict(__name__='candidate_cardinal_replay',__file__=str(parent));exec(compile(source,str(parent),'exec'),ns);ns['configurations']=configurations;ns['main']()
    path=P/'analysis/movement_contact_candidates_cardinal_verification.json';r=json.loads(path.read_text());assert r['original_native_comparisons']==48
    r['scope']='48ordinary-entry real plane-intersection responses:normalX orZ againstY,delta signedZ orX. Independently check axis,orientation andspeeds0.5/1/2,bothbanks/rounding modes,exact delta copy andpreserved query position/motion/anchor. All original plane/cross/angle/normalization/math callees execute;no supplied outputs orframes.'
    r['sha256'][str(Path(__file__))]=hashlib.sha256(Path(__file__).read_bytes()).hexdigest();path.write_text(json.dumps(r,indent=2)+'\n');print('CARDINAL RESPONSE PASS',r['original_native_comparisons'],r['independent_checks'],flush=True)
if __name__=='__main__':main()
