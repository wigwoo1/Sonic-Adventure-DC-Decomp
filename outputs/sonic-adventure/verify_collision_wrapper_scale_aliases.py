"""Execute the original scaling callee to check the paired-store saved-R14 alias."""
import hashlib,itertools,json,struct
from pathlib import Path
import write_watch_collision_angle as fast
v=fast.old;P=v.P;ENTRY=0x8c10e7b8

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def bits(x):return struct.unpack('<I',struct.pack('<f',x))[0]
def main():
 ram=v.seed_ram();cases=[];checks=0
 for bank,rm,sz,power,pattern in itertools.product(range(2),range(2),range(2),(-2,-1,1),(0x12345678,0x8c10e6b8)):
  cfg=v.Input(dict(entry=ENTRY,mode=(bank<<2)|(sz<<1),probe=True,sr=0x400000a0,mach=0x89abcdef,macl=0x76543210,gbr=0x8ce01234));m,s,f=v.prepare(ram,cfg)
  s.r[4]=power&0xffffffff;s.r[14]=0x89abcdef;s.r[15]=0x8cf00000;s.pr=0x8cfed000;s.pc=ENTRY
  for b in range(2):
   for j in range(16):f.banks[b][j]=bits(.5+(b*16+j)/16)
  f.banks[bank][4]=bits(.5);f.banks[bank][5]=pattern;f.fpscr=(bank<<21)|(sz<<20)|0x40000|rm;f.fpul=0xc0ffee42;cfg['fpul']=f.fpul
  before=bytes(m);r,path,a,s,f=fast.compare(m,s,f,cfg)
  def require(ok,label):
   nonlocal checks
   assert ok,(bank,rm,sz,power,hex(pattern),label);checks+=1
  require(r['status']==0,'scaling original/native complete');require(r['full_state_match'],'entire CPU/FPU/RAM/device state');require(r['ordered_boundaries_match'],'ordered boundaries')
  require(s.r[14]==(pattern if sz else 0x89abcdef),'saved R14 preserved in SZ0 and overwritten by input FR5 in SZ1')
  require(s.r[15]==0x8cf00000 and s.pr==0x8cfed000,'original SP/PR restored')
  require([b['target'] for b in r['boundaries']]==[hex(ENTRY)],'actual original leaf;no supplied services/results/frames')
  want=bytearray(before);want[0xeffff8:0xf00000]=a[0xeffff8:0xf00000];require(a==want,'only original eight-byte stack footprint')
  cases.append(dict(bank=bank,rm=rm,sz=sz,power=power,initial_fr5=hex(pattern),final_r14=hex(s.r[14]),status=r['status'],full_state_match=True,ordered_boundaries_match=True,original_instructions=len(path)))
 files=[Path(__file__),P/'check_collision_wrapper_paired_aliases.py',P/'analysis/collision_wrapper_paired_frame_aliases.json',P/'analysis/collision_angle_qualification.json',v.effects.DLL]
 report=dict(original_native_comparisons=len(cases),independent_checks=checks,cases=cases,dll_sha256=digest(v.effects.DLL),native_code_changed=False,caller_input_registers_only=True,callee_results_or_frames_injected=False,new_routine_credit=0,wrapper_qualification_added=False,overall_completion_percent=10,sha256={str(p):digest(p) for p in files},scope='48fresh original/native scaling-callee calls:both physical banks/RM0/1/SZ0/1,powers-2/-1/+1,two legitimate initial FR5bitpatterns including an original continuation address. Entire state and independent stack/R14/SP/PR/boundary assertions pass. Establishes local saved-R14 alias,not reachability of the complete caller or infinity-result wrapper arms. No original constants or callee outputs/frames modified.')
 (P/'analysis/collision_wrapper_scale_alias_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('SCALE FRAME ALIAS VERIFIED',len(cases),checks)
if __name__=='__main__':main()
