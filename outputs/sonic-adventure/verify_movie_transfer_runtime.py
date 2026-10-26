"""Original REQUEST runs continuously through real native transfer/IRQ and returns."""
import collections,gc,hashlib,itertools,json
from pathlib import Path
import write_watch_transfer_runtime as fast
v=fast.old;P=v.P;ROOT=v.ROOT;cpu=v.old.old.old.old
class Input(v.Input):
 sound_image=None
 def get(self,key,default=None):return self.sound_image if key=='sound_image' else super().get(key,default)
def carry(c,r):
 c.update(other=r['integer_other'],mach=r['final_mach'],macl=r['macl'],gbr=r['final_gbr'],vbr=r['vbr'],runtime_seed=r['runtime_state'],masks=r['masks'],transfer_registers=r['transfer_registers'])
 for key,value in zip(v.EXCEPTION_KEYS,r['exception_state']):c[key]=value
def setup(ram,c):
 m,s,f=cpu.prepare(ram,c);v.prepare(ram,v.Input(entry=0x8ce50600));v.effects.CONNECTED.update(v.old.effects.CONNECTED)
 for key in v.EXCEPTION_KEYS:c[key]=0
 s.pc=0x8c645de0;s.r[4]=0x80000000 if c.get('context')==v.NESTED else 0;s.r[5]=0x8ce40000;c['entry']=s.pc
 r,path,a,s,f=fast.compare(m,s,f,c);assert r['status']==0;carry(c,r);first=r
 a[0x892840:0x893c00]=bytes(0x13c0);s.pc=0x8c64594c;s.r[4:8]=[0xa60+c['channel']*32,v.HANDLER,0x60,v.DESCRIPTOR];c['entry']=s.pc
 r,path,a,s,f=fast.compare(a,s,f,c);assert r['status']==0 and s.r[0]==1;carry(c,r)
 return a,s,f,[first,r]
def main():
 records=[];controls=[];failures=[];coverage=set();edges=set();checks=0;ram=v.seed_ram()
 grid=[(ch,0,33376,cx) for ch in range(4) for cx in (v.NORMAL,v.NESTED)]+list(itertools.product((0,3),(1,4),(64,),(v.NORMAL,v.NESTED)))
 for ch,mode,length,context in grid:
  c=Input(entry=cpu.REQUEST,mode=mode,sdk_mode=1,current_record=0,channel=ch,length=length,context=context,source_pattern='masaru' if length==33376 else 'varying',masks=[0,0],irq_status=0,table=0xa05f7800,transfer_registers=[0]*48,runtime_attached=True)
  c.sound_image=bytes([0xa5])*0x200000;a,s,f,stages=setup(ram,c);wanted=bytearray(c.sound_image)
  for sequence,offset in enumerate((0,128),1):
   if sequence==2:
    start=cpu.SRC&0xffffff;a[start:start+64]=bytes(value^0x5a for value in a[start:start+64])
   payload=bytes(a[cpu.SRC&0xffffff:(cpu.SRC&0xffffff)+length]);wanted[offset:offset+length]=payload
   s.pc=cpu.REQUEST;s.r[4]=v.RECORD;s.r[5]=offset;c['entry']=s.pc
   before=v.effects.State.from_buffer_copy(s);fp=bytes(f)
   r,path,a,s,f=fast.compare(a,s,f,c);assert r['status']==0 and s.pc==before.pr and s.r[0]==0;checks+=3
   assert s.r[15]==before.r[15] and s.pr==before.pr and bytes(f)==fp;checks+=3
   assert r['full_state_match'] and r['ordered_boundaries_match'];checks+=2
   state=r['runtime_state'];assert state[4]==0 and state[5]==state[6]==state[8]==sequence and state[7]==sequence*length and state[9]==state[10]==0;checks+=7
   assert state[3][ch]==(ch,0x800000+offset,0xc900000,length|0x80000000,0);checks+=1
   assert r['sound_ram_sha256']==hashlib.sha256(wanted).hexdigest() and r['sound_trace_events']==length//4 and not r['sound_trace_dropped'];checks+=3
   assert bytes(a[cpu.SRC&0xffffff:(cpu.SRC&0xffffff)+length])==payload;checks+=1
   assert r['irq_status']==0 and v.load(a,v.DESCRIPTOR+44)==0 and v.load(a,v.DESCRIPTOR+48)==1;checks+=3
   assert v.HANDLER in path and v.ASIC in path and 0x8ce50600 in path and context in path;checks+=4
   assert path.index(0x8c666e02)<path.index(0x8ce50600)<path.index(v.HANDLER)<path.index(0x8c65a6ee);checks+=1
   coverage.update(path)
   for i,pc in enumerate(path):
    text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
    if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+distance<len(path):edges.add((pc,path[i+distance]))
   stages.append(r);carry(c,r);c.sound_image=bytes(wanted)
  records.append(dict(channel=ch,mode=mode,length=length,context=hex(context),original_native_comparisons=4,continuous_requests=2,source_preserved=True,complete_sound_bytes_verified=True,independent_checks=56,runtime_state=stages[-1]['runtime_state'],sound_ram_sha256=stages[-1]['sound_ram_sha256'],boundaries=[r['boundary_comparisons'] for r in stages]))
  print('Continuous',ch,mode,length,hex(context),'two requests returned',flush=True);del a,stages;gc.collect()
 for kind in ('disabled','masked','blocked','high-mask'):
  c=Input(entry=cpu.REQUEST,mode=0,sdk_mode=1,current_record=0,channel=0,length=64,context=v.NORMAL,masks=[0,0],irq_status=0,table=0xa05f7800,transfer_registers=[0]*48,runtime_attached=True,runtime_enabled=kind!='disabled')
  a,s,f,stages=setup(ram,c)
  if kind=='masked':c['masks']=[0,0]
  if kind=='blocked':s.sr|=0x10000000
  if kind=='high-mask':s.sr=(s.sr&~0xf0)|0x60
  s.pc=cpu.REQUEST;s.r[4]=v.RECORD;s.r[5]=0;c['entry']=s.pc
  r,path,a,s,f=fast.compare(a,s,f,c)
  assert r['status']==-4 and r['stop_pc']=='0x8c65a6ec' and v.load(a,v.RECORD+24)==0 and v.HANDLER not in path;checks+=4
  state=r['runtime_state'];assert state[8]==0
  if kind=='disabled':assert state[6]==state[7]==state[4]==0
  else:assert state[6]==1 and state[7]==64 and state[4]==1<<15
  if kind in ('blocked','high-mask'):assert state[9]==1
  controls.append(dict(kind=kind,status=r['status'],stop_pc=r['stop_pc'],runtime_state=state));print('Control',kind,r['status'],flush=True)
 for kind in ('bad-handler','bad-vector','unconfigured-sound'):
  c=Input(entry=cpu.REQUEST,mode=0,sdk_mode=1,current_record=0,channel=0,length=64,context=v.NORMAL,masks=[0,0],irq_status=0,table=0xa05f7800,transfer_registers=[0]*48,runtime_attached=True)
  a,s,f,stages=setup(ram,c)
  if kind=='bad-handler':v.store(a,0x8c892850,0x8cfef000)
  if kind=='bad-vector':v.store(a,0x8ce50630,0x8cfef000)
  if kind=='unconfigured-sound':c['sound_unconfigured']=True
  s.pc=cpu.REQUEST;s.r[4]=v.RECORD;s.r[5]=0;c['entry']=s.pc
  r,path,a,s,f=fast.compare(a,s,f,c);state=r['runtime_state']
  assert r['status']==-2 and r['stop_pc']==('0x8c666e04' if kind=='unconfigured-sound' else '0x8cfef000');checks+=2
  assert not state[8] and not state[10] and v.load(a,v.RECORD+24)==0 and not r['exception_state'][-1];checks+=4
  if kind=='unconfigured-sound':assert state[4]==state[6]==state[7]==0
  else:assert state[4]==1<<15 and state[6]==1 and state[7]==64
  failures.append(dict(kind=kind,status=r['status'],stop_pc=r['stop_pc'],runtime_state=state));print('Failure retained',kind,r['stop_pc'],flush=True)
 files=[Path(__file__),Path(v.__file__),Path(fast.__file__),P/'transfer_runtime_reference.py',P/'transfer_runtime_reference_cpu.py',P/'analysis/movie_transfer_runtime_connection.json',P/'analysis/transfer_runtime_verification.json',cpu.DRIVER]
 report=dict(lifecycles_passed=len(records),continuous_requests_returned=2*len(records),original_native_comparisons=4*len(records)+3*(len(controls)+len(failures)),retained_wait_controls=len(controls),failure_controls=len(failures),independent_checks=checks,cases=records,controls=controls,failures=failures,request_executed_rows=[hex(a) for a in sorted(coverage&set(v.PER_BODY.get(cpu.REQUEST,{})))],request_executed_branch_outcomes=[[hex(a),hex(b)] for a,b in sorted(edges) if a in v.PER_BODY.get(cpu.REQUEST,{})],dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},newly_registered_entries=0,scope='Sixteen initialized lifecycles run original vector initialization and registration, then two continuous original REQUEST entries with persistent runtime state and real device data. No guest checkpoint, completed flag, event injection between start and return, helper result or skipped instruction is supplied. Authored native service advances at actual idle-helper RTS boundary; original copied vector/context/ASIC/generic/handler/SDK/RTE return before original REQUEST resumes and returns. Full machine/boundary/RAM/sound/platform/runtime state matches independent original decoder and independent host-device oracle. Whole data, changed repeated payload, offset and counters checked. Four disabled/masked/BL/IMASK controls retain actual wait; three missing-handler/vector/sound controls retain real fault without completion. Separate ABI setup and borrowed device-state carry are explicit; uninterrupted earliest startup, physical timing/security/suspend/ARM/audio/gameplay remain unfinished.')
 (P/'analysis/movie_transfer_runtime_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Continuous runtime',report['original_native_comparisons'],'comparisons',report['continuous_requests_returned'],'requests returned')
if __name__=='__main__':main()
