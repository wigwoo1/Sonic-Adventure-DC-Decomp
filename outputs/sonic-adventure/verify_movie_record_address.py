"""Exact arithmetic, all GPR/SR/PR/FPU/RAM and MACL checks for two canonical entries."""
import hashlib,itertools,json
from pathlib import Path
import write_watch_record_address as fast
v=fast.old;P=v.P
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 ram=v.seed_ram();base=v.load(ram,0x8c601f70);indices=set(range(32))|{127,128,255,256,0x7fffffff,0x80000000,0xfffffffe,0xffffffff,0xffffffff//52,(0xffffffff//52)+1,(0x100000000-base)//52,((0x100000000-base)//52)+1}
 configs=((0,0),(1,0x400000f3),(4,0x600003a1),(7,0x40008000));records=[];seen=set();checks=0
 for entry,index,(mode,sr) in itertools.product((v.ADDRESS_THUNK,v.ADDRESS_HELPER),sorted(indices),configs):
  c=v.Input(dict(entry=entry,mode=mode,mach=index^0x13579bdf,macl=index^0x2468ace0,gbr=0x8ce01234,address_arguments=[index,0x12345678,0x89abcdef,0x13579bdf]));m,s,f=v.prepare(ram,c);s.sr=sr
  for bank in range(2):
   for reg in range(16):f.banks[bank][reg]=((bank+1)*0x11111111^(reg+1)*0x01020304^index)&0xffffffff
  before=v.effects.State.from_buffer_copy(s);fp=bytes(f);product=(index*52)&0xffffffff;result=(base+product)&0xffffffff
  r,path,a,s,f=fast.compare(m,s,f,c);seen.update(path)
  assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match'];n=3
  assert s.r[0]==result and s.r[3]==base and all(s.r[j]==before.r[j] for j in range(16) if j not in (0,3));n+=3
  assert s.sr==before.sr and s.pc==s.pr==before.pr and bytes(f)==fp;n+=4
  assert r['final_macl']==product and r['final_mach']==c['mach'] and r['final_gbr']==c['gbr'];n+=3
  assert a==m and not any(r['sound_state']) and not r['aperture_events'] and r['pvr_dma_state'][6]==0;n+=4
  calls=[b['target'] for b in r['boundaries']];assert calls==([hex(v.ADDRESS_THUNK),hex(v.ADDRESS_HELPER)] if entry==v.ADDRESS_THUNK else [hex(v.ADDRESS_HELPER)]);n+=1
  checks+=n;records.append(dict(entry=hex(entry),index=index,mode=mode,sr=sr,result=result,macl=product,steps=len(path),independent_checks=n))
 coverage=[]
 for body in v.manifest['routines']:
  missing=set(body['reachable_addresses'])-{hex(a) for a in seen};assert not missing
  coverage.append(dict(address=body['address'],executed_rows=body['instruction_rows'],missing_rows=[],executed_outcomes=0))
 files=[Path(__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_record_address_connection.json',v.effects.DLL]
 report=dict(cases_passed=len(records),independent_checks=checks,indices_tested=len(indices),configurations=len(configs),stride=52,base=hex(base),cases=records,coverage_by_routine=coverage,executed_rows=9,executed_outcomes=0,new_routine_credit=0,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Two existing canonical address helpers, all9 instruction rows execute. Complete original/native machine and ordered boundaries plus independent modulo32 arithmetic/MACL/MACH/GBR/SR/GPR/FPU/fullRAM expectations. All general registers exceptR0/R3 preserved; RAM unchanged including stack. PR/SZ/FR/SR bank/user/supervisor/FD configurations and pointer/product overflow boundaries. Zero duplicate recovery credit; full cold startup not freshly rerun.')
 (P/'analysis/movie_record_address_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('ADDRESS DIRECT',len(records),checks,'indices',len(indices))
if __name__=='__main__':main()
