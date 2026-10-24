"""Actual event replacement lifecycles and extended original boot prefixes."""
import hashlib,itertools,json
from pathlib import Path
import verify_boot_event as v
import verify_boot_wait as wait
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 ram=v.previous.previous.previous.previous.seed_ram();lifecycles=[]
 for mode in (0,4):
  c=dict(entry=v.REPLACE,mode=mode);m,s,f=v.prepare(ram,c);stages=[]
  for callback in (0x8c651bc0,wait.NOP,0):
   c['callback']=callback;s.pc=v.REPLACE;s.r[4]=callback;before=v.effects.State.from_buffer_copy(s);fp=v.effects.Fpu.from_buffer_copy(f)
   record,path,a,s,f=v.compare(m,s,f,c);assert record['status']==0
   expected=int(callback!=0);assert v.load(a,v.HANDLE)==expected and v.load(a,v.event_head(0x900))==(v.node(1) if expected else 0)
   assert v.load(a,v.DRIVER+0x238)==(0x8c645b1e if expected else 0x8c64169c)
   assert v.previous.previous.retained(before,s) and bytes(fp)==bytes(f) and not record['queue_events'] and not record['clock_reads'] and not record['mask_writes']
   if expected:assert v.load(a,v.node(1))==callback and v.load(a,v.node(1)+8)==0 and v.load(a,v.node(1)+12)==0x80000058
   else:assert v.load(a,v.node(1)+12)==0
   record['independent_checks']=5;stages.append(record);m=a
  lifecycles.append(stages)
 prefix=[];boot_rows=set()
 for mode,renderer,existing in itertools.product((0,4),(1,2),(0,1)):
  c=dict(entry=wait.boot.BOOT,mode=mode,renderer=renderer,existing=existing,chain=[0x58] if existing else [],gate=1,pending=1,active=True,command_ticks=8,timeout_b=4,ticks=[0,0,20000000]+list(range(20000001,20000300)))
  original,_,_=v.previous.previous.previous.prepare(ram,dict(c));m,s,f=v.prepare(ram,c);m[0x8a3380:0x8a3400]=original[0x8a3380:0x8a3400];v.store(m,v.load(m,0x8c60521c),v.previous.CHANNEL)
  record,path,a,s,f=v.compare(m,s,f,c);assert record['status']==-2 and record['stop_pc']=='0x8c09da40'
  targets={int(b['target'],16) for b in record['boundaries']};assert {v.REPLACE,v.REGISTER,v.PRIMARY,v.previous.UPDATE}<=targets
  if existing:assert v.REMOVE in targets
  callback=next(b['arguments'][0] for b in record['boundaries'] if int(b['target'],16)==v.REPLACE)
  assert v.load(a,v.HANDLE)==1 and v.load(a,v.event_head(0x900))==v.node(1) and v.load(a,v.node(1))==callback
  assert v.load(a,v.DRIVER+0x238)==0x8c645b1e and v.load(a,v.node(1)+12)==0x80000058
  for i,literal in enumerate((0x8c605220,0x8c605224,0x8c605228,0x8c60522c,0x8c605230)):assert v.load(a,v.load(m,literal))==v.load(a,v.previous.CHANNEL+i*4)
  record['independent_checks']=10;prefix.append(record);boot_rows.update(set(path)&set(wait.boot.geometry.PER_BODY[wait.boot.BOOT]))
 files=[Path(__file__),v.PROJECT/'verify_boot_event.py',v.PROJECT/'analysis/boot_event_translation.json',v.PROJECT/'analysis/boot_event_connection.json']
 report=dict(lifecycles_passed=2,lifecycle_stage_comparisons=6,lifecycle_independent_checks=30,boot_prefix_cases=8,boot_prefix_independent_checks=80,boot_rows_covered=len(boot_rows),boot_static_rows=51,boot_recovery_credit=0,next_dependency='0x8c09da40 -> 0x8c09cc80',external_game_service_fixtures=0,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Two actual create/replace/disable lifecycles carry RAM/CPU/FPU through native event functions. Those entries do not change MACL/GBR/VBR/MACH/inactive bank state or emit device/clock events; resetting the unchanged empty device model therefore retains identical state. Eight uninterrupted outer-boot prefixes run actual frame/wait/submission/IRQ/input and event registration to the next file-loading branch thunk. Native boundary states and all final state match.',limitations=['Runtime driver/event/frame/input tables, selected controller, VBR/masks and finite timer samples are explicit fixtures; natural startup and IRQ/exception delivery remain unproved.','The next branch thunk 8c09da40 is already separately recovered and targets the unrecovered 8c09cc80 file-load body. Neither is connected here; no file operation result is fabricated.','Physical devices, DMA/GPU pixels, timing, playable gameplay, all ports/demakes and display/texture settings remain unfinished.'],lifecycles=lifecycles,boot_prefix=prefix)
 (v.PROJECT/'analysis/boot_event_sequences_verification.json').write_text(json.dumps(report,indent=2)+'\n');print({k:report[k] for k in ('lifecycles_passed','lifecycle_stage_comparisons','boot_prefix_cases','boot_rows_covered','next_dependency')})

if __name__=='__main__':main()
