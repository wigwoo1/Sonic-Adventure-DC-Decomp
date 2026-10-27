"""Independent complete-memory/register expectations for real setup/status code."""
import hashlib,itertools,json
from pathlib import Path
import write_watch_earlier_initialize as fast
v=fast.old;P=v.P

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def put(m,address,value):
 at=address&0xffffff;m[at:at+4]=(value&0xffffffff).to_bytes(4,'little')

def cases():
 configs=((0,0),(1,0x400000f3),(4,0x600003a1),(7,0x40008000))
 for (mode,sr),pointer,size in itertools.product(configs,(0,1,4,0x8cd50000,0xffffffff),(0,1,2048,0x3fffffff,0x40000000,0xffffffff)):
  yield dict(entry=v.MEMORY_SETUP,mode=mode,sr=sr,earlier_arguments=[pointer,size,0x12345678,0x89abcdef])
 for (mode,sr),payload in itertools.product(configs,(0,0x80000000,0xffffffff)):
  yield dict(entry=v.MEMORY_TAIL,mode=mode,sr=sr,earlier_arguments=[payload,payload,0x12345678,0x89abcdef])
 for (mode,sr),pointer in itertools.product(configs,(0,1,4,0x8cd50000,0xffffffff)):
  yield dict(entry=v.MEMORY_POINTER,mode=mode,sr=sr,earlier_arguments=[pointer,0x12345678,0x89abcdef,0x13579bdf])
 for entry,(mode,sr),index,value in itertools.product((v.RECORD_POLL,v.RECORD_WORD),configs,(0,1,7,31,255,0x02000000,0x02000001,0xfe00001f),(0,1,0x80000000,0xffffffff)):
  yield dict(entry=entry,mode=mode,sr=sr,record_value=value,earlier_arguments=[index,0x12345678,0x89abcdef,0x13579bdf])

def expected(m,before,c):
 wanted=bytearray(m);r=list(before.r);sr=before.sr;entry=c['entry'];original_sp=r[15]
 if entry in (v.MEMORY_SETUP,v.MEMORY_TAIL):
  if entry==v.MEMORY_TAIL:r[3]=v.MEMORY_SETUP;r[4]=v.load(m,0x8c644d90);r[5]=int.from_bytes(v.sample(m,0x8c644d8e,2),'little',signed=True)&0xffffffff
  put(wanted,original_sp-4,before.pr)
  if not r[4]:r[0]=0xffffffff;sr|=1
  else:
   r[3]=v.load(m,0x8c5fbd0c);r[2]=v.load(m,0x8c5fbd08);r[5]=(r[4]+r[5]*4)&0xffffffff
   put(wanted,r[3],r[2]);r[2]=v.load(m,0x8c5fbd10);put(wanted,original_sp-8,r[5]);put(wanted,r[2],r[5]);r[4]=r[5]
   sr=(sr&~1)|int(not r[4])
   if r[4]:r[2]=v.load(m,0x8c5fbd18);put(wanted,r[2],r[4])
   r[0]=0
 elif entry==v.MEMORY_POINTER:
  sr=(sr&~1)|int(not r[4]);r[0]=0xffffffff if not r[4] else 0
  if r[4]:r[2]=v.load(m,0x8c5fbd18);put(wanted,r[2],r[4])
 else:
  index=r[4];r[3]=v.load(m,0x8c647d10);r[4]=(r[3]+(index<<7))&0xffffffff;value=c['record_value']
  if entry==v.RECORD_WORD:r[0]=value;sr=(sr&~1)|((index>>25)&1)
  else:
   put(wanted,original_sp-4,before.pr);r[0]=0xffffffff if value else 0;sr=(sr&~1)|int(value==0)
 return wanted,r,sr

def main():
 ram=v.seed_ram();records=[];seen=set();edges=set();checks=0
 for i,data in enumerate(cases(),1):
  c=v.Input(dict(mach=0x89abcdef,macl=0x76543210,gbr=0x8ce01234)|data);m,s,f=v.prepare(ram,c);s.sr=c['sr']
  if c['entry'] in (v.RECORD_WORD,v.RECORD_POLL):
   address=(v.load(m,0x8c647d10)+(s.r[4]<<7)+4)&0xffffffff;assert address&0x1f000000==0x0c000000;put(m,address,c['record_value'])
  for bank in range(2):
   for reg in range(16):f.banks[bank][reg]=((bank+1)*0x11111111^(reg+1)*0x01020304^i)&0xffffffff
  before=v.effects.State.from_buffer_copy(s);fp=bytes(f);wanted,regs,sr=expected(m,before,c)
  r,path,a,s,f=fast.compare(m,s,f,c);seen.update(path)
  for j,pc in enumerate(path):
   text=v.ROWS.get(pc,'');delta=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+delta<len(path):edges.add((pc,path[j+delta]))
  assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match'];n=3
  assert list(s.r)==regs and s.sr==sr and s.pc==s.pr==before.pr;n+=3
  assert bytes(f)==fp and r['final_macl']==c['macl'] and r['final_mach']==c['mach'] and r['final_gbr']==c['gbr'];n+=4
  assert a==wanted and not any(r['sound_state']) and not r['aperture_events'] and r['pvr_dma_state'][6]==0;n+=4
  calls=[int(b['target'],16) for b in r['boundaries']]
  wanted_calls=[c['entry']]
  if c['entry']==v.MEMORY_TAIL:wanted_calls+=[v.MEMORY_SETUP,v.MEMORY_POINTER]
  elif c['entry']==v.MEMORY_SETUP and before.r[4]:wanted_calls+=[v.MEMORY_POINTER]
  elif c['entry']==v.RECORD_POLL:wanted_calls+=[v.RECORD_WORD]
  assert calls==wanted_calls;n+=1
  checks+=n;records.append(dict(entry=hex(c['entry']),arguments=c['earlier_arguments'],record_value=c.get('record_value'),mode=c['mode'],sr=c['sr'],steps=len(path),independent_checks=n))
  if i%50==0:print('EARLIER DIRECT',i,flush=True)
 coverage=[]
 for body in v.manifest['routines']:
  missing=set(body['reachable_addresses'])-{hex(pc) for pc in seen};missing_edges={tuple(e) for e in body['conditional_edges']}-{(hex(a),hex(b)) for a,b in edges}
  if int(body['address'],16)!=v.EARLIER_CALLER:assert not missing and not missing_edges,(body['address'],missing,missing_edges)
  coverage.append(dict(address=body['address'],executed_rows=body['instruction_rows']-len(missing),missing_rows=sorted(missing),executed_outcomes=len(body['conditional_edges'])-len(missing_edges),missing_outcomes=sorted(missing_edges)))
 files=[Path(__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_earlier_initialize_connection.json',v.effects.DLL]
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,qualified_new_entries=['0x8c5fbb60','0x8c6033e4'],canonical_entries=['0x8c644d84','0x8c5fbcb6','0x8c647ca4'],partial_entry='0x8c053ac0',dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Five helper bodies fully execute, including every conditional outcome, with real callees. Full original/native machine/boundary comparison plus independent allGPR/SR/PR/PC/FPU/MACH/MACL/GBR/fullRAM/stack/global-store expectations; setup null/overflow/wrapped-zero and high index/record-status cases. Earlier caller is not credited or qualified. Code/literals immutable, fixed globals disjoint from private mapped stack and test records. No physical boot or gameplay proof.')
 (P/'analysis/movie_earlier_initialize_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('EARLIER DIRECT PASS',len(records),checks,flush=True)
if __name__=='__main__':main()
