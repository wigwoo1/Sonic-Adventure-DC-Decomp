"""Compare installed interrupt body and original callbacks; no DMA/IRQ is fabricated."""
import collections,ctypes,gc,hashlib,itertools,json,sys
from pathlib import Path
import write_watch_transfer_interrupt as fast
v=fast.old;P=v.P;ROOT=v.ROOT

def shift_mask(channel):
 shift=(channel+15)&0xffffffff
 if not shift&0x80000000:return (1<<(shift&31))&0xffffffff
 count=(-shift)&31
 return 1>>count if count else 0

def semantics(m,a,before,s,r,path,c):
 checks=3
 assert r['full_state_match'] and r['ordered_boundaries_match']
 assert not any(r['sound_state']) and not r['sound_trace_events']
 assert not r['cache_writeback_sequence']
 if r['status'] and r['stop_pc'] not in ('0x8cfef000','0xffffffff'):return checks
 descriptor=before.r[4];channel=v.load(m,descriptor);root=v.load(m,0x8c5fbb14);table=v.load(m,root)
 counter=(root+0xec+channel*4)&0xffffffff;slot=(table+channel*32+0x14)&0xffffffff
 assert v.load(a,descriptor+44)==0 and v.load(a,descriptor+48)==1;checks+=2
 assert v.load(a,counter)==(v.load(m,counter)+1)&0xffffffff;checks+=1
 mask=shift_mask(channel)
 if c.get('ack_pointer') is not None and c['ack_pointer']&0x1f000000==0x0c000000:
  assert v.load(a,c['ack_pointer'])==mask;checks+=1
 else:
  assert r['ack_writes']==[[12,0xa05f6900,mask,4]];checks+=1
 assert v.load(a,slot)==0 if table==v.TABLE else r['transfer_registers'][channel*8+5]==0;checks+=1
 callback=c.get('handler_callback',0)
 if callback==0:
  expected=bytearray(m);v.store(expected,before.r[15]-4,before.r[14])
  if table==v.TABLE:v.store(expected,slot,0)
  v.store(expected,descriptor+44,0);v.store(expected,descriptor+48,1);v.store(expected,counter,(v.load(m,counter)+1)&0xffffffff)
  if c.get('ack_pointer') is not None:v.store(expected,c['ack_pointer'],mask)
  assert a==expected;checks+=1
  regs=list(before.r);regs[0]=root;regs[1]=(channel+15)&0xffffffff;regs[2]=0x8c5fbb0c;regs[3]=0;regs[4]=0x8c5fbb14;regs[5]=mask;regs[6]=0
  assert list(s.r)==regs and s.sr==(before.sr|1) and s.pr==before.pr and s.pc==before.pr;checks+=4
 if callback==v.old.COUNTER:
  address=v.load(m,0x8c666e4c);assert v.load(a,address)==(v.load(m,address)+1)&0xffffffff;checks+=1
 if callback==v.old.old.CALLBACK and r['status']==0:
  record=c.get('handler_argument',v.RECORD)
  assert v.load(a,record+4)==1 and v.load(a,record+24)==1;checks+=2
  assert v.load(a,root+0x190)==0;checks+=1
  assert v.load(a,root+0x18c)==(v.load(m,root+0x18c)+2)&0xffffffff;checks+=1
 if callback in (0x8cfef000,0xffffffff):
  assert r['status']==-2 and s.r[4]==c.get('handler_argument',v.RECORD) and s.r[14]==before.r[14] and s.r[15]==before.r[15] and s.pr==before.pr;checks+=5
 return checks

def cases(smoke=False):
 base=dict(entry=v.HANDLER,mode=0)
 if smoke:
  for callback in (0,v.old.COUNTER,v.old.old.CALLBACK,0x8cfef000):yield v.Input(base|dict(handler_callback=callback,sdk_mode=2))
  return
 for channel,counter,mode in itertools.product((0,1,2,3),(0,0xffffffff),(0,1,4)):
  yield v.Input(base|dict(channel=channel,completed_counter=counter,mode=mode))
 for mask,bank in itertools.product(range(16),(0,1)):
  yield v.Input(base|dict(initial_sr=0x40008302|(mask<<4)|(bank<<29),channel=3))
 for callback,arg,mode in itertools.product((v.old.COUNTER,0x8c5ff2cc,0x8cfef000,0xffffffff),(0,0x2468ace0),(0,1,4)):
  yield v.Input(base|dict(handler_callback=callback,handler_argument=arg,mode=mode,error_counter=0xffffffff))
 for sdk_mode,mode in itertools.product((0,1,2),(0,1,4)):
  yield v.Input(base|dict(handler_callback=v.old.old.CALLBACK,sdk_mode=sdk_mode,mode=mode))
 for channel,table in itertools.product((0,1,2,3),(v.TABLE,0xa05f7800)):
  yield v.Input(base|dict(channel=channel,table=table))
 for channel in (16,17,31,0xfffffff0,0xffffffd1):yield v.Input(base|dict(channel=channel,ack_pointer=0x8cda0080))
 for extra in (dict(descriptor=0),dict(descriptor=0x8cfffffc),dict(root=0),dict(table=0),dict(ack_pointer=0),dict(ack_pointer=0x8cda0080),dict(stack=0x8c000000),dict(null_bus=True)):
  yield v.Input(base|extra)

def main():
 ram=v.seed_ram();records=[];coverage=set();edges=set();checks=0
 for c in cases('--smoke' in sys.argv):
  m,s,f=v.prepare(ram,c);before=v.effects.State.from_buffer_copy(s);r,path,a,s,f=fast.compare(m,s,f,c);count=semantics(m,a,before,s,r,path,c);checks+=count;coverage.update(path)
  for i,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+distance<len(path):edges.add((pc,path[i+distance]))
  keys=('input','status','stop_pc','steps','boundary_comparisons','full_state_match','ordered_boundaries_match','sound_state','ack_writes','irq_status','write_watch')
  records.append({k:r[k] for k in keys}|dict(independent_checks=count,guest_r0=hex(s.r[0]),guest_sr=hex(s.sr)))
  print(len(records),r['status'],r['stop_pc'],flush=True);del r,path,a;gc.collect()
 if '--smoke' in sys.argv:print('Interrupt smoke passed',len(records));return
 body=set(v.PER_BODY[v.HANDLER]);definition=v.manifest['routines'][0];wanted={tuple(int(x,16) for x in e) for e in definition['conditional_edges']}
 assert body<=coverage and wanted<=edges
 files=[Path(__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_transfer_interrupt_connection.json',P/'source/interrupt_bus.c',P/'source/driver_signal_bus.c']
 report=dict(cases_passed=len(records),independent_checks=checks,status_counts=dict(collections.Counter(r['status'] for r in records)),instruction_rows=len(body),covered_rows=len(body&coverage),conditional_outcomes=len(wanted),covered_outcomes=len(wanted&edges),cases=records,dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Complete installed transfer interrupt body in explicit initialized ABI/software-bus domain. Full CPU/FPU/RAM/VRAM/SoundRAM and ordered call-boundary states match independent original instruction execution. Independent descriptor/counter/mask/entire no-callback RAM and registers, actual callback/tail-return and failure effects checked. Handler invocation is explicit; no DMA movement, hardware IRQ delivery, exception entry or natural startup completion is claimed. Existing acknowledge bus policy is retained unchanged.')
 (P/'analysis/movie_transfer_interrupt_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Interrupt comparisons',len(records),checks,report['status_counts'])
if __name__=='__main__':main()
