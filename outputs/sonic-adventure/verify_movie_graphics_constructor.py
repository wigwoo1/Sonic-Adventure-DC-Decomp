"""Original pool constructor, actual cleanup/providers and full backing-state oracle."""
import hashlib,itertools,json
from pathlib import Path
import verify_movie_graphics_allocations as allocation
fast=allocation.fast;v=allocation.v;P=v.P;ROOT=v.ROOT
ENTRY=0x8c663140;MODE=0x8c67c2c8;META=allocation.META;POOL=allocation.POOL
BIG=0x8c8c03e4;BIG_BYTES=0x13000;CACHE_HEAD=0x8c67c308
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def prepare(ram,c):
 m,s,f=v.prepare(ram,c);x=c['constructor_fixture'];v.store(m,MODE,x['mode'])
 allocation.patch(m,POOL,(x['flags'].to_bytes(2,'little')+bytes([0xa5])*22)*(allocation.POOL_BYTES//24))
 allocation.patch(m,BIG,(x['flags'].to_bytes(2,'little')+bytes([0x5a])*74)*(BIG_BYTES//76))
 allocation.patch(m,META,bytes(40));v.store(m,CACHE_HEAD,0)
 if x['linked']:
  for group,offset in enumerate((8,0,0x1c,0x14)):
   first=POOL+group*48;v.store(m,META+offset,first)
   for i in range(2):v.store(m,first+i*24+8,first+24 if i==0 else 0)
  v.store(m,CACHE_HEAD,BIG)
  for i in range(2):v.store(m,BIG+i*76+4,BIG+76 if i==0 else 0)
 return m,s,f
def cases():
 for mode,linked,flags in itertools.product((0,1,2,0xffffffff),(False,True),(0,1,0xffff)):
  yield dict(entry=ENTRY,mode=0,constructor_fixture=dict(mode=mode,linked=linked,flags=flags))
 for mode,fpu in itertools.product((0,1),(1,4)):
  yield dict(entry=ENTRY,mode=fpu,constructor_fixture=dict(mode=mode,linked=True,flags=0xffff))
def oracle(m,c):
 x=c['constructor_fixture'];expected=bytearray(m)
 for node in range(POOL,POOL+allocation.POOL_BYTES,24):allocation.flags(expected,node,0)
 if x['linked']:
  for node in (BIG,BIG+76):allocation.flags(expected,node,x['flags']&0xfffe)
 v.store(expected,CACHE_HEAD,0);allocation.patch(expected,META,bytes(40))
 single=x['mode']==1;size=0x800000 if single else 0x400000
 for a,value in ((0x8c8a2d6c,0x800000),(0x8c67c2cc,size),(0x8c67c2d0,0),(0x8c67c2d4,size),(0x8c67c2d8,size),(0x8c67c2dc,0 if single else size)):v.store(expected,a,value)
 for bank in range(1 if single else 2):
  node=POOL+24*bank;base=META+20*bank;allocation.flags(expected,node,1)
  for offset,value in ((4,0),(8,0),(12,bank*size),(16,size)):v.store(expected,node+offset,value)
  for offset,value in ((8,node),(12,node),(16,size)):v.store(expected,base+offset,value)
 return expected
def main():
 ram=v.seed_ram();seen=set();edges=set();records=[];checks=0
 manifest_path=P/'analysis/movie_driver_translation.json';manifest=json.loads(manifest_path.read_text());body=next(r for r in manifest['routines'] if r['address']==hex(ENTRY))
 for i,data in enumerate(cases(),1):
  c=v.Input(data);m,s,f=prepare(ram,c);before=v.effects.State.from_buffer_copy(s);fp=bytes(f);expected=oracle(m,c);r,path,a,s,f=fast.compare(m,s,f,c)
  assert r['full_state_match'] and r['ordered_boundaries_match'] and r['status']==0 and s.r[0]==0;checks+=4
  assert s.pc==s.pr==before.pr and list(s.r[8:])==list(before.r[8:]) and bytes(f)==fp;checks+=3
  for address,size in ((POOL,allocation.POOL_BYTES),(BIG,BIG_BYTES),(META,40),(0x8c67c2cc,20),(CACHE_HEAD,4),(0x8c8a2d6c,4)):
   assert v.sample(a,address,size)==v.sample(expected,address,size),(i,hex(address));checks+=1
  writes=[e for e in r['register_events'] if e[0]==7] if 'register_events' in r else None
  target=[b for b in r['boundaries'] if b['target']=='0x8c653444'];assert len(target)==1 and target[0]['arguments'][:2]==[0xe4,0];checks+=2
  option=[b for b in r['boundaries'] if b['target']=='0x8c665a80'];assert len(option)==1 and option[0]['arguments'][:3]==[0x8c672514,0x8c672520,0];checks+=2
  assert sum(b['target']=='0x8c664ac0' for b in r['boundaries'])==(1 if c['constructor_fixture']['mode']==1 else 2);checks+=1
  assert not r['vram_writes'] and not any(r['sound_state']) and r['runtime_state'] is None;checks+=3
  seen.update(path)
  for j,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((pc,path[j+distance]))
  records.append(dict(input=dict(c),status=r['status'],r0=s.r[0],steps=len(path),boundaries=len(r['boundaries'])));print(i,c['constructor_fixture'],flush=True)
 wanted=set(body['reachable_addresses']);actual={hex(x) for x in seen};wanted_edges=set(map(tuple,body['conditional_edges']));actual_edges={(hex(a),hex(b)) for a,b in edges}
 coverage=dict(address=hex(ENTRY),static_rows=len(wanted),covered_rows=len(wanted&actual),missing_rows=sorted(wanted-actual),conditional_outcomes=len(wanted_edges),covered_outcomes=len(wanted_edges&actual_edges),missing_outcomes=[list(e) for e in sorted(wanted_edges-actual_edges)])
 files=[Path(__file__),Path(allocation.__file__),Path(v.__file__),Path(fast.__file__),manifest_path,Path(body['listing']),P/'analysis/movie_graphics_init_connection.json',P/'source/sa_movie_driver.c',v.effects.DLL]
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,coverage=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Full original constructor calls with explicit coherent finite small/big lists,allfree/allbusy/arbitrary lowflags,mode1/other andthreeFPUprofiles. Independent entire98304byte smallpool/77824byte cachepool/40byte bank metadata/global-byte-capacity and exact provider/MMIO arguments supplement fullmachine/memory/ordered boundaries. Actual cleanup clearsflags andreal descriptors allocate;no helper returns injected. Missing outcomes require separatecheckedproof;no new recoverycredit here. Not full startup/rendering/physical hardware/gameplay.')
 (P/'analysis/movie_graphics_constructor_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Constructor',len(records),checks,coverage)
if __name__=='__main__':main()
