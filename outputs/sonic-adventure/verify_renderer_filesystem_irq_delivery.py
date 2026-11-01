"""Actual filesystem construction, gate controls and real shared-worker execution."""
import hashlib,itertools,json,shlex
from pathlib import Path
import renderer_filesystem_irq_fixtures as q
P=q.P;R=P.parents[1];ENTRY=0x8c5ffcf0;WORKER=0x8c6005c2


def carry(c,r):
 c.update(frame_events_seed=r['frame_events_state'],graphics_in=r['graphics_out'],other=r['integer_other'],mach=r['final_mach'],macl=r['final_macl'],gbr=r['final_gbr'],vbr=r['vbr'])
 if r['runtime_state'] is not None:c['runtime_seed']=r['runtime_state']
 for name,value in zip(q.v.EXCEPTION_KEYS,r['exception_state']):c[name]=value


def main():
 target=P/'analysis/renderer_filesystem_irq_delivery_verification.json';assert not target.exists()
 m,s,f,c=q.prepare(q.v.seed_ram(),dict(kind=0,real_tables=True,mode=0,rm=0));s.r[15]=0x8cfff000;s.sr&=~0xf0;c['vbr']=0x8ce50000;q.v.bind_vectors(0x8ce50000);setup=[]
 def phase(entry,args,label,stack=None):
  nonlocal m,s,f,c
  sp=s.r[15];s.pc=c['entry']=entry;s.pr=0x8cfed000
  for i,value in enumerate(args):s.r[4+i]=value&0xffffffff
  if stack is not None:q.put(m,sp,stack)
  r,path,m,s,f=q.fast.compare(m,s,f,c)
  print(label,r['status'],r['stop_pc'],flush=True)
  assert r['status']==0 and s.pc==s.pr==0x8cfed000 and s.r[15]==sp
  assert r['full_state_match'] and r['ordered_boundaries_match'];carry(c,r)
  setup.append(dict(label=label,entry=hex(entry),steps=len(path),disc_read_attempts=r['disc_read_attempts'],return_value=hex(s.r[0])))
  print(label,'returned',hex(s.r[0]),flush=True);return r
 phase(0x8c645de0,[0,0x8ce40000],'original vector initialization')
 phase(0x8c602128,[0x8cb00000,0x100000],'original system heap initialization')
 phase(0x8c604702,[1,0,1,15],'original display and library initialization')
 phase(0x8c604dce,[-1,0,0x20000,0],'original allocated context initialization',stack=0)
 phase(0x8c098a10,[],'original game heap initialization')
 phase(0x8c098b0c,[1,0x4020],'actual filesystem-context allocation');context=(s.r[0]+31)&~31
 phase(0x8c098b0c,[1,0x30020],'actual directory allocation');directory=(s.r[0]+31)&~31
 q.put(m,q.get(m,0x8c6000e4),0);q.put(m,0x8c673420,0)
 inventory=json.loads((P/'disc_inventory.json').read_text());track=next(x for x in reversed([shlex.split(line) for line in inventory['gdi'].splitlines()[1:] if line.strip()]) if x[2]=='4');q.put(m,0x8c66c530,int(track[1])+166)
 r=phase(0x8c601618,[4,context,4096,directory],'original real-disc filesystem mount')
 assert s.r[0]==0 and r['disc_read_attempts']==21
 records=[]
 c.update(runtime_attached=True,irq_samples=[8]*256)
 runtime=q.v.runtime_model.Device();c['runtime_seed']=runtime.snapshot()
 for entry,args,label in [(0x8c642440,[],'original empty frame finalization'),(0x8c641f00,[],'original submission withfilesystem callbacks insideIRQ'),(0x8c6005c2,[],'ordinary original filesystem worker afterRTE')]:
  sp=s.r[15];s.pc=c['entry']=entry;s.pr=0x8cfed000
  for i,value in enumerate(args):s.r[4+i]=value&0xffffffff
  r,path,m,s,f=q.fast.compare(m,s,f,c)
  assert r['status']==0 and s.pc==s.pr==0x8cfed000 and s.r[15]==sp
  assert r['full_state_match'] and r['ordered_boundaries_match']
  firmware=[b for b in r['boundaries'] if b['target']=='0x8c00f100']
  for b in firmware:assert b['pr']=='0x8c6452de'
  if entry==0x8c641f00:
   assert path.count(0x8c5ffcf0)==2 and path.count(0x8c6005c2)==2 and len(firmware)==2
   assert r['runtime_state'][8]==2 and r['exception_state'][-1]==1 and q.get(m,0x8c8a30c0)==0
   assert not any(b['target']=='0x8c650b60' and b['pr'] in ('0x8c651d92','0x8c651eae') for b in r['boundaries'])
  if entry==0x8c6005c2:assert len(firmware)==1
  records.append(dict(label=label,entry=hex(entry),steps=len(path),firmware_boundaries=firmware,runtime_state=r['runtime_state'],exception_state=r['exception_state'],frame_events_state=r['frame_events_state'],boundaries=r['boundaries'],full_state_match=True,ordered_boundaries_match=True))
  carry(c,r);print(label,'verified',len(firmware),'firmware calls',flush=True)
 assert q.v.ROWS[0x8c6452da].startswith('jsr ')
 files=[Path(__file__),P/'verify_renderer_filesystem_irq.py',P/'renderer_filesystem_irq_reference.py',P/'write_watch_renderer_filesystem_irq.py',P/'analysis/renderer_filesystem_irq_observer_connection.json',R/'work/sonic-adventure/build/renderer_filesystem_callback.dll']
 report=dict(status='original_filesystem_IRQ_and_following_ordinary_firmware_observation_verified',original_native_comparisons=len(setup)+len(records),setup=setup,cases=records,complete_IRQs=2,IRQ_firmware_calls=2,following_ordinary_firmware_calls=1,firmware_call_instruction='0x8c6452da',actual_architectural_return='0x8c6452de',guest_execution_changed=False,comparisons_removed=False,host_observer_uses_live_CPU=True,scope='Original startup/allocators/realmount/finalizer produceinputs. Originalsubmission executes twofullcopiedvector/context/SDK/filesystemcallback/worker/firmware/RTE paths withoutSDKwatchdog. A followingordinaryworkerentry provesobserverCPUselectionrestoresafterRTE. PRcomesfromliveoriginalCPU andmatchesactualjsr6452da+4,notnativeexpected-value substitution. FullRAM/CPU/FPU/device/event/boundarycomparisons retained. ExternalVBlankobservations remain declared;pixelsandfull6050b0frame unfinished.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('IRQ firmware delivery verified',report['original_native_comparisons'],flush=True)


if __name__=='__main__':main()
