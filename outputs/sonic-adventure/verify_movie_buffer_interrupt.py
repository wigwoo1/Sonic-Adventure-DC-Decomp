"""Original queue retirement and IRQ/RTE/wait lifecycle; no completion hooks."""
import hashlib,itertools,json
from pathlib import Path
import verify_movie_buffer_initialize as direct
fast=direct.fast;v=direct.v;P=v.P;ROOT=v.ROOT

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def carry(c,r):
 c.update(other=r['integer_other'],mach=r['final_mach'],macl=r['macl'],gbr=r['final_gbr'],vbr=r['vbr'],runtime_seed=r['runtime_state'],masks=r['masks'],transfer_registers=r['transfer_registers'],dma_operation=r['dma_operation'],irq_status=r['irq_status'])
 c['startup_overrides']={0x5f6800+i*4:value for i,value in enumerate(r['startup_registers'])}
 for key,value in zip(v.EXCEPTION_KEYS,r['exception_state']):c[key]=value

def main():
 records=[];seen=set();edges=set();checks=0;ram=v.seed_ram();v.bind_vectors(0x8ce50000)
 def observe(c,m,s,f):
  nonlocal checks
  r,path,a,s,f=fast.compare(m,s,f,c);assert r['full_state_match'] and r['ordered_boundaries_match'];checks+=2;seen.update(path)
  for i,pc in enumerate(path):
   ins=v.ROWS.get(pc,'');delta=2 if ins.startswith(('bt/s ','bf/s ')) else 1
   if ins.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+delta<len(path):edges.add((pc,path[i+delta]))
  return r,path,a,s,f
 # Declared queue inputs, including wrap and every control-bit combination.
 for queue,flags,slot in itertools.product((0,1),(1,5,17,21),(0,7)):
  c=v.Input(direct.case(direct.DMA,aligned=True));m,s,f=direct.prepare(ram,c);c['entry']=s.pc=0x8c650e40
  active=direct.TABLE+7*32;v.store(m,active+24,0x82)
  v.store(m,direct.ISSUED+queue*4,slot);node=direct.TABLE+queue*256+slot*32
  # Keep the active record distinct from the queued record.
  if node==active:v.store(m,direct.ACTIVE,6);active=direct.TABLE+6*32;v.store(m,active+24,0x82)
  for field,value in ((4,direct.SOURCE),(12,0x11004000),(20,64),(24,0x80|flags)):v.store(m,node+field,value)
  before=bytes(m);r,path,a,s,f=observe(c,m,s,f)
  assert r['status']==0 and s.r[0]==1 and v.load(a,active+24)==0x80 and v.load(a,node+24)==(0x82|flags&0x14);checks+=4
  assert v.load(a,direct.ACTIVE)==slot and v.load(a,direct.QUEUE)==queue and v.load(a,direct.ISSUED+queue*4)==(slot+1)&7;checks+=3
  assert r['pvr_dma_state'][6]==1 and r['pvr_dma_state'][4]==64 and r['pvr_dma_state'][7]==1<<19;checks+=3
  expected=bytearray(bytes(range(256))*(0x800000//256))
  for offset in range(0,64,4):direct.v.store(expected,direct.aperture.mapping(0xa4004000+offset)-0xa5000000,direct.v.load(before,direct.SOURCE+offset))
  assert r['vram_sha256']==hashlib.sha256(expected).hexdigest();checks+=1
  records.append(dict(kind='queue-next',queue=queue,flags=flags,slot=slot,status=r['status'],steps=len(path)))
 c=v.Input(direct.case());m,s,f=direct.prepare(ram,c);c['entry']=s.pc=0x8c650e40;v.store(m,direct.TABLE+7*32+24,0x82)
 r,path,a,s,f=observe(c,m,s,f);assert r['status']==0 and s.r[0]==0 and v.load(a,direct.TABLE+7*32+24)==0x80 and not r['pvr_dma_state'][6];checks+=4;records.append(dict(kind='queue-empty',status=0))
 # All setup calls execute original code. Only platform state is carried across
 # separate host entries; the submitted job and its completion flags are never
 # replaced. The actual VBR handler acknowledges bit19 and calls650e40.
 for entry,length,control in [(direct.DMA,n,'ready') for n in (32,64,68)]+[(direct.VQ,64,'ready'),(direct.DMA,64,'mask-off'),(direct.DMA,64,'runtime-off')]:
  c=v.Input(direct.case(entry,aligned=True,length=length,runtime_attached=True,masks=[0,0],irq_status=0));m,s,f=direct.prepare(ram,c)
  for key in v.EXCEPTION_KEYS:c[key]=0
  c['entry']=s.pc=0x8c645de0;s.r[4]=0;s.r[5]=0x8ce40000
  r,path,a,s,f=observe(c,m,s,f);assert r['status']==0;carry(c,r);checks+=1
  a[0x892840:0x893c00]=bytes(0x13c0)
  c['entry']=s.pc=0x8c651520;r,path,a,s,f=observe(c,a,s,f);assert r['status']==0;carry(c,r);checks+=1
  assert v.load(a,0x8ce50224)==0x8c652210 and r['startup_registers'][(0x6930-0x6800)//4]==0x2807ec;checks+=2
  v.store(a,0x8c8a317c,1)
  if control=='mask-off':c['startup_overrides'][0x5f6930]=0
  if control=='runtime-off':c['runtime_enabled']=False;c['runtime_seed']=list(c['runtime_seed'][:-1])+[0]
  c['entry']=s.pc=entry;s.r[4:8]=[0x4000,direct.SOURCE,16,0] if entry==direct.VQ else [0x4000,direct.SOURCE,length,0];s.sr&=~0x100000f0
  before=v.effects.State.from_buffer_copy(s);r,path,a,s,f=observe(c,a,s,f)
  if control=='ready':
   jobs=2 if entry==direct.VQ else 1
   assert r['status']==0 and s.r[0]==1 and s.r[15]==before.r[15] and s.pc==before.pr;checks+=4
   assert r['runtime_state'][8]==jobs and r['runtime_state'][4]==r['pvr_dma_state'][7]==0 and r['pvr_dma_state'][6]==jobs;checks+=3
   assert all(pc in path for pc in (0x8ce50600,0x8c652210,0x8c650e40,0x8c651000)) and r['exception_state'][4]==0x320 and r['exception_state'][6]==1;checks+=3
   assert all(v.load(a,direct.TABLE+q*256+i*32+24)&3==0 for q in range(2) for i in range(8));checks+=1
   segments=[(0x4000,direct.SOURCE,length)] if entry==direct.DMA else [(0x4000,direct.SOURCE,128),(0x4800,direct.SOURCE+128,64)]
   expected=bytearray(bytes(range(256))*(0x800000//256))
   for offset,source,count in segments:
    for pos in range(0,count,4):v.store(expected,direct.aperture.mapping(0xa4000000+offset+pos)-0xa5000000,v.load(a,source+pos))
   assert r['vram_sha256']==hashlib.sha256(expected).hexdigest();checks+=1
  else:
   assert r['status']==-4 and r['stop_pc']=='0x8c65100c' and r['runtime_state'][8]==0 and r['pvr_dma_state'][7]==1<<19,(control,r['status'],r['runtime_state'],r['pvr_dma_state']);checks+=4
   assert 0x8c652210 not in path and v.load(a,direct.TABLE+256+24)&3==2;checks+=2
  records.append(dict(kind='irq-lifecycle',entry=hex(entry),length=length,control=control,status=r['status'],device_transfers=r['pvr_dma_state'][6],irq_deliveries=r['runtime_state'][8],steps=len(path),ack_writes=r['ack_writes']))
 coverage=[]
 for body in v.manifest['routines']:
  rows=set(body['reachable_addresses']);outcomes=set(map(tuple,body['conditional_edges']));actual={hex(a) for a in seen};actual_edges={(hex(a),hex(b)) for a,b in edges}
  coverage.append(dict(address=body['address'],static_rows=len(rows),covered_rows=len(rows&actual),conditional_outcomes=len(outcomes),covered_outcomes=len(outcomes&actual_edges),missing_rows=sorted(rows-actual),missing_outcomes=[list(e) for e in sorted(outcomes-actual_edges)]))
 files=[Path(__file__),Path(direct.__file__),Path(v.__file__),Path(fast.__file__),P/'pvr_dma_reference.py',P/'buffer_transfer_reference_cpu.py',v.MP,P/'analysis/movie_buffer_initialize_connection.json',v.effects.DLL]
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Original vector setup645de0 andgraphicsregistration651520/653640,actualCH2texturedata/devicecompletion,IRL9event320priority6,originalVBRcontext/652210ack/650e40retirement/RTE/651000waitreturn. Explicitcoherentqueues andpersistentplatformstate;no gamecompletion/helperreturn injections. Broad652210branches remainpartial/unqualified;untimedevent boundary only,notphysicaltiming/gameplay.')
 (P/'analysis/movie_buffer_interrupt_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('BUFFER INTERRUPT',len(records),checks,coverage,flush=True)

if __name__=='__main__':main()
