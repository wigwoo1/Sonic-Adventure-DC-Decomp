"""Nonzero control-register and all-bank/width context-save vectors."""
import hashlib,itertools,json
from pathlib import Path
import verify_movie_record_register as parent
v=parent.v;P=v.P
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 ram=v.seed_ram();records=[];checks=0
 for mode,pattern in itertools.product(range(8),(1,0x80000000,0xffffffff,0x7fa12345)):
  c=v.Input(parent.case(v.REGISTER_CONTEXT,mode=mode,payload=pattern,mach=pattern^0x13579bdf,macl=pattern^0x2468ace0,gbr=pattern^0x10203040,register_arguments=[parent.fixtures.CONTEXT,0x13579bdf,0x2468ace0,0x89abcdef]))
  m,s,f=v.prepare(ram,c);parent.fixtures.configure(m,c);s.sr=c['sr'];f.fpul=pattern
  f.fpscr|=0x1f07f if pattern&1 else 0
  for bank in range(2):
   for reg in range(16):f.banks[bank][reg]=(pattern^((bank+1)*0x11111111)^((reg+1)*0x01020304))&0xffffffff
  before=v.effects.State.from_buffer_copy(s);fp=type(f).from_buffer_copy(f)
  r,path,a,s,f=parent.fast.compare(m,s,f,c)
  assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match'] and bytes(f)==bytes(fp)
  assert s.pc==s.pr==before.pr and list(s.r[8:])==list(before.r[8:])
  n=7+parent.check_context(before,fp,c,a,s);checks+=n;records.append(dict(mode=mode,fpul=pattern,mach=c['mach'],macl=c['macl'],gbr=c['gbr'],steps=len(path),independent_checks=n));print('CONTEXT',mode,hex(pattern),flush=True)
 files=[Path(__file__),Path(parent.__file__),Path(parent.fixtures.__file__),Path(v.__file__),Path(parent.fast.__file__),v.MP,P/'analysis/movie_record_register_connection.json',v.effects.DLL,P/'analysis/record_context_register_primary_evidence.json']
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='32 fresh original/native context comparisons with nonzero FPUL/MACH/MACL/GBR, sticky FPSCR bits and distinct raw data in both physical FPU banks, all eight PR/SZ/FR modes. Independent complete512-byte context-region oracle includes exact overlapping wide stores. All CPU/FPU/RAM/platform state compares; not physical exception delivery or timing proof. No additional routine credit beyond the51-row original context-save body.')
 (P/'analysis/movie_record_context_vectors_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('CONTEXT VECTORS',len(records),checks)
if __name__=='__main__':main()
