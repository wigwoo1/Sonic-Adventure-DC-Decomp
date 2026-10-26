"""Original channel start followed by explicit handler ABI entry; no synthetic DMA."""
import hashlib,itertools,json
from pathlib import Path
import write_watch_transfer_interrupt as fast
v=fast.old;P=v.P;ROOT=v.ROOT

def main():
 records=[];checks=0;ram=v.seed_ram()
 for channel,mode in itertools.product(range(4),(0,1,4)):
  c=v.Input(entry=v.old.CLIENT_START,mode=mode,sdk_mode=1,current_record=0,channel=channel)
  m,s,f=v.old.prepare(ram,c);initial=bytearray(m);first,path,a,s,f=fast.compare(m,s,f,c);d=v.DESCRIPTOR
  assert first['status']==0 and s.r[0]==0;checks+=2
  assert v.load(a,d+44)==1 and v.load(a,d+48)==0;checks+=2
  assert v.load(a,d+28)==v.old.old.CALLBACK and v.load(a,d+32)==v.RECORD;checks+=2
  assert v.load(a,v.RECORD+4)==2 and v.load(a,v.RECORD+24)==0;checks+=2
  assert v.load(a,v.ROOT_CONTEXT+0x190)==v.RECORD and not any(first['sound_state']);checks+=2
  started=bytearray(a);saved_pr=s.pr;saved_sp=s.r[15]
  # New public ABI invocation after the previous call returned normally. This
  # does not resume a stopped guest or inject an exception into a running CPU.
  c['entry']=v.HANDLER;s.pc=v.HANDLER;s.r[4]=d
  second,path,a,s,f=fast.compare(a,s,f,c)
  assert second['status']==0 and s.pr==saved_pr and s.r[15]==saved_sp;checks+=3
  assert v.load(a,d+44)==0 and v.load(a,d+48)==1;checks+=2
  assert v.load(a,v.RECORD+4)==1 and v.load(a,v.RECORD+24)==1 and v.load(a,v.ROOT_CONTEXT+0x190)==0;checks+=3
  assert v.load(a,v.ROOT_CONTEXT+0x18c)==(v.load(started,v.ROOT_CONTEXT+0x18c)+2)&0xffffffff;checks+=1
  assert v.load(a,v.ROOT_CONTEXT+0xec+channel*4)==(v.load(started,v.ROOT_CONTEXT+0xec+channel*4)+1)&0xffffffff;checks+=1
  assert second['ack_writes']==[[12,0xa05f6900,1<<(channel+15),4]];checks+=1
  assert not any(second['sound_state']) and not second['sound_trace_events'];checks+=2
  for result in (first,second):assert result['full_state_match'] and result['ordered_boundaries_match'];checks+=2
  records.append(dict(input=dict(c),start_status=first['status'],start_boundaries=first['boundary_comparisons'],handler_status=second['status'],handler_boundaries=second['boundary_comparisons'],ack_writes=second['ack_writes'],completion_written_by_original_callback=True,explicit_second_abi_entry=True))
 files=[Path(__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_transfer_interrupt_connection.json']
 report=dict(lifecycles_passed=len(records),original_native_comparisons=2*len(records),independent_checks=checks,cases=records,dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Twelve initialized channel-start/explicit-handler two-call lifecycles across four channel numbers and three FPU modes. Both calls return normally and compare full state with original instructions. Original handler and original SDK callback set completion flags; fixture does not. Second call is explicit public ABI invocation, not physical IRQ/exception delivery, a resumed guest checkpoint, actual DMA or natural earliest-startup completion. Sound RAM remains untouched and no audio claim is made.')
 (P/'analysis/movie_transfer_interrupt_lifecycle_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Transfer lifecycles',len(records),'comparisons',2*len(records),'checks',checks)
if __name__=='__main__':main()
