"""Execute the original switch dispatch, stopping at its selected state body."""
import hashlib,json,struct
from verify_startup import ROOT,State,reference

def main():
 b=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(b)]=b
 targets=[0x8c07950e+struct.unpack_from('<h',b,0x69510+i*2)[0] for i in range(15)];cases=0
 for selector in range(15):
  for sr0 in [0,0x700083f3]:
   hit=[]
   def hook(a):
    def callback(memory,regs,status):hit.append(a);return status
    return callback
   state=State();state.r[0]=selector;state.r[15]=0x8cf00000;state.pc=0x8c079502;state.pr=0x8c000100;state.sr=sr0
   reference(ram,state,{target:hook(target) for target in targets})
   assert hit==[targets[selector]] and state.r[1]==selector*2
   assert state.r[0]==struct.unpack_from('<h',b,0x69510+selector*2)[0]&0xffffffff
   assert state.sr==sr0&~1;cases+=1
 out=ROOT/'outputs/sonic-adventure';report=dict(cases_passed=cases,retail_dispatch_sha256=hashlib.sha256(b[0x69502:0x6952e]).hexdigest(),verifier_sha256=hashlib.sha256((out/'verify_startup.py').read_bytes()).hexdigest(),scope='Retail SHLL/MOVA/signed MOV.W/BRAF dispatch with delay-slot NOP for all 15 selectors; branch bodies hooked only to observe target and end. This verifies dispatch, not state-body behavior.')
 (out/'analysis/owner_helper_switch_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(f'{cases} retail helper switch checks passed')
if __name__=='__main__':main()
