"""Original frame finalization/submission with IRQ, mask and damaged-vector controls."""
import dataclasses,hashlib,json
from pathlib import Path
import renderer_filesystem_irq_fixtures as q
P=q.P;R=P.parents[1];VBR=0x8ce50000


def carry(c,r):
 c.update(frame_events_seed=r['frame_events_state'],graphics_in=r['graphics_out'],other=r['integer_other'],mach=r['final_mach'],macl=r['final_macl'],gbr=r['final_gbr'],vbr=r['vbr'])
 if r['runtime_state'] is not None:c['runtime_seed']=r['runtime_state']
 for key,value in zip(q.v.EXCEPTION_KEYS,r['exception_state']):c[key]=value


def main():
 target=P/'analysis/renderer_filesystem_irq_controls_verification.json';assert not target.exists()
 m,s,f,c=q.prepare(q.v.seed_ram(),dict(kind=0,real_tables=True,mode=0,rm=0))
 s.r[15]=0x8cfff000;s.sr&=~0xf0;c['vbr']=VBR;q.v.bind_vectors(VBR);setup=[]
 for entry,args,label,stack in [(0x8c645de0,[0,0x8ce40000],'original vector initialization',None),(0x8c602128,[0x8cb00000,0x100000],'original system heap',None),(0x8c604702,[1,0,1,15],'original display/registration',None),(0x8c604dce,[-1,0,0x20000,0],'original allocated context',0),(0x8c642440,[],'original frame command finalization',None)]:
  sp=s.r[15];s.pc=c['entry']=entry;s.pr=0x8cfed000
  for i,x in enumerate(args):s.r[4+i]=x&0xffffffff
  if stack is not None:q.put(m,sp,stack)
  r,path,m,s,f=q.fast.compare(m,s,f,c)
  assert r['status']==0 and s.pc==s.pr==0x8cfed000 and s.r[15]==sp
  assert r['full_state_match'] and r['ordered_boundaries_match'];carry(c,r)
  setup.append(dict(label=label,entry=hex(entry),steps=len(path),frame_events_state=r['frame_events_state']))
  print(label,'returned',flush=True)
 base=(bytes(m),bytes(s),bytes(f),dict(c));records=[];checks=0
 for kind in ('valid','no-vblank','mask6','mask10','blocked','disabled','device-mask','bad-vector','bad-handler'):
  m=bytearray(base[0]);s=q.v.effects.State.from_buffer_copy(base[1]);f=q.v.effects.Fpu.from_buffer_copy(base[2]);c=dict(base[3])
  s.pc=c['entry']=0x8c641f00;s.pr=0x8cfed000;sp=s.r[15]
  # The preserved interrupt-family wait permits64 polls. Include its handler
  # reads as well as each platform poll without changing that original guard.
  c.update(runtime_attached=True,irq_samples=[0 if kind=='no-vblank' else 8]*256)
  device=q.v.runtime_model.Device(enabled=kind!='disabled');c['runtime_seed']=device.snapshot()
  if kind.startswith('mask'):s.sr=(s.sr&~0xf0)|(int(kind[4:])<<4)
  if kind=='blocked':s.sr|=0x10000000
  if kind=='device-mask':
   arrays=list(c['graphics_in'].arrays);registers=list(arrays[4]);registers[(0x6930-0x6800)//4]=0;arrays[4]=tuple(registers)
   c['graphics_in']=dataclasses.replace(c['graphics_in'],arrays=tuple(arrays))
  if kind=='bad-vector':q.put(m,VBR+0x630,0x8cfef000)
  if kind=='bad-handler':q.put(m,VBR+0x224,0x8cfef000)
  r,path,a,after,fp=q.fast.compare(m,s,f,c)
  assert r['full_state_match'] and r['ordered_boundaries_match'];checks+=2
  watchdog=sum(b['target']=='0x8c650b60' and b['pr'] in ('0x8c651d92','0x8c651eae') for b in r['boundaries'])
  assert watchdog==0;checks+=1
  if kind=='valid':
   assert r['status']==0 and after.pc==after.pr==0x8cfed000 and after.r[15]==sp;checks+=3
   assert q.get(a,0x8c8a30c0)==0 and 0x8c651f1e in path and 0x8c651f06 in path;checks+=3
   assert r['runtime_state'][8]>0 and r['runtime_state'][9:11]==(0,0);checks+=2
  elif kind in ('bad-vector','bad-handler'):
   assert r['status']==-2 and r['stop_pc']=='0x8cfef000' and not r['runtime_state'][8];checks+=3
  else:
   assert r['status']==-4 and r['stop_pc']=='0x8c64207e' and q.get(a,0x8c8a30c0)==1;checks+=3
   if kind!='no-vblank':assert 0x8c652210 not in path;checks+=1
  records.append(dict(kind=kind,status=r['status'],stop_pc=r['stop_pc'],steps=len(path),frame_events_state=r['frame_events_state'],runtime_state=r['runtime_state'],exception_state=r['exception_state'],watchdog_calls=watchdog,vector_calls=path.count(VBR+0x600),graphics_handler_calls=path.count(0x8c652210),VBlank_calls=path.count(0x8c651bc0),list_callback_calls=path.count(0x8c651f1e),busy_flag=q.get(a,0x8c8a30c0),boundaries=r['boundaries'],command_packets=r['command_packets']))
  print(kind,r['status'],r['stop_pc'],'deliveries',r['runtime_state'][8],flush=True)
 files=[Path(__file__),P/'write_watch_renderer_filesystem_irq.py',P/'frame_events_eol_reference.py',P/'source/frame_events_bus_eol.c',P/'frame_signal_reference_cpu.py',R/'work/sonic-adventure/build/renderer_filesystem_callback.dll']
 report=dict(status='original_frame_submission_and_retained_IRQ_controls_verified',original_native_comparisons=len(setup)+len(records),normal_setup_calls=len(setup),control_cases=len(records),checks=checks,setup=setup,cases=records,whole_6050b0_frame_qualified=False,physical_GPU_pixels_qualified=False,automatic_VBlank=False,new_original_routine_credit=0,overall_completion_percent=10,scope='Original vector/display/heap/context/finalizer normal outputs initialize every case. Original641f00 runs uninterrupted with actual accepted-packet TA list events anddeclared externalVBlank observations. Original registeredIRQ/context/RTE clears work;no SDKwatchdog onvalid path. Masks,BL,disabled service/noVBlank retain waits;damaged copiedvector/registration retain faults. No stopped frame reused. Full native/reference machine/RAM/VRAM/device/event/boundary equality remains mandatory.',sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('Frame submission controls verified',report['original_native_comparisons'],checks,flush=True)


if __name__=='__main__':main()
