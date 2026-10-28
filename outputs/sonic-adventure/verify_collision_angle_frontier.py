"""Target remaining original numeric paths; preserve the completed base matrix."""
import ast,hashlib,itertools,json,types
from pathlib import Path
import verify_collision_angle_direct as base
P=base.P

def configurations():
 for bank,rm,y,x in itertools.product(range(2),range(2),(2**32,2**33,2**64,-2**32),(1,2)):
  yield dict(kind='large_range_reduce',entry=base.REDUCE,fpscr=(bank<<21)|0x40000|rm,y=base.bits(y),x=base.bits(x))
 # Arbitrary initial scratch FP bits are legitimate caller inputs. The actual
 # original callees execute; no callee results, frames, or code are substituted.
 for entry,bank,pattern,y,x in itertools.product((base.ASIN,base.ATAN2),range(2),(0,0x7f800000,0xff800000,0x7fbfffff),(0,.5),(0,1)):
  yield dict(kind='sz_scratch_frontier',entry=entry,fpscr=(bank<<21)|0x140000,y=base.bits(y),x=base.bits(x),register_pattern=pattern)

def main():
 source=(P/'verify_collision_angle_direct.py').read_text()
 old='  f.banks[bank][4]=c[\'y\'];f.banks[bank][5]=c[\'x\'];f.fpscr=fpscr;f.fpul=0xc0ffee42;cfg[\'fpul\']=f.fpul'
 assert source.count(old)==1
 source=source.replace(old,"  if 'register_pattern' in c:\n   for b in range(2):\n    for j in range(16):f.banks[b][j]=c['register_pattern']\n"+old)
 marker="configs=[dict(c,case_id=i) for i,c in enumerate(configurations())]";assert source.count(marker)==1
 source=source.replace(marker,"configs=[dict(c,case_id=10000+i) for i,c in enumerate(configurations())]")
 marker="('collision_angle_direct'+suffix+'_verification.json')";assert source.count(marker)==1
 source=source.replace(marker,"('collision_angle_frontier'+suffix+'_verification.json')")
 ns=dict(base.__dict__,__name__='collision_angle_frontier_replay',__file__=__file__,configurations=configurations)
 exec(compile(source,'collision_angle_frontier_replay','exec'),ns);ns['configurations']=configurations;ns['main']()
 path=P/'analysis/collision_angle_frontier_verification.json';report=json.loads(path.read_text())
 report['sha256'][str(P/'verify_collision_angle_direct.py')]=base.digest(P/'verify_collision_angle_direct.py');report['sha256'][str(Path(__file__))]=base.digest(Path(__file__))
 report['scope']+=' Targeted large range-reduction and legitimate initial scratch-register SZ frontier;original code/constants/callee results/frames unchanged. Coverage gaps remain explicit.'
 path.write_text(json.dumps(report,indent=2)+'\n')
if __name__=='__main__':main()
