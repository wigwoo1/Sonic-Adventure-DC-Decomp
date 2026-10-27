"""Original request/lock instructions, independent byte/packet/stack oracles."""
import hashlib,itertools,json,sys
from pathlib import Path
import movie_channel_request_fixtures as fixtures
v=fixtures.v;P=v.P;fast=fixtures.fast;put=fixtures.put
ENTRIES=(0x8c660494,0x8c6604e6)

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 ram=v.seed_ram();records=[];seen=set();edges=set();checks=0;smoke='--smoke' in sys.argv
 def run(kind,entry,unit,args,mode,sr,lock=0,busy=0,serial=1,order=0,padding=0,previous=0,bank=0):
  nonlocal checks
  c,m,s,f=fixtures.setup(ram,entry,args,mode,sr);d=fixtures.configure(m,unit,lock,busy,serial,order,padding,previous,bank)
  stack_arg=0x12345678;put(m,s.r[15],stack_arg);before=type(s).from_buffer_copy(s);fp=bytes(f);expected,result,command,words=fixtures.expected_packet(m,s,entry,stack_arg)
  r,path,a,s,f=fast.compare(m,s,f,c);n=3
  assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match'],(kind,entry,lock,r['status'],r['stop_pc'])
  wanted_lock=lock|0x80 if lock else 0
  assert a[fixtures.LOCK&0xffffff]==wanted_lock;n+=1
  assert v.sample(a,fixtures.LOCK-3,3)==bytes([0xa5])*3 and v.sample(a,fixtures.LOCK+1,4)==bytes([0xa5])*4;n+=2
  assert s.r[0]==(0xfffffffe if lock else result) and s.r[15]==before.r[15] and s.pr==s.pc==before.pr;n+=3
  wanted= m if lock else expected
  assert v.sample(a,fixtures.CTX,0x2000)==v.sample(wanted,fixtures.CTX,0x2000) and v.sample(a,fixtures.FRAME,512)==v.sample(wanted,fixtures.FRAME,512);n+=2
  assert list(s.r)[8:15]==list(before.r)[8:15] and bytes(f)==fp and r['final_mach']==c['mach'] and r['final_gbr']==c['gbr'];n+=4
  assert s.sr&~1==before.sr&~1;n+=1
  assert r['cache_purge_sequence']==[fixtures.LOCK];n+=1
  targets=[b['target'] for b in r['boundaries']]
  assert (hex(0x8c65f348) in targets)==bool(lock) and (hex(0x8c65f810) in targets)==(not lock);n+=2
  if lock:
   assert s.sr&1 and r['final_macl']==c['macl'];n+=2
   # Original TAS observes all 256 byte values; diagnostic condition is zero.
   assert v.sample(a,fixtures.FRAME,512)==v.sample(m,fixtures.FRAME,512);n+=1
  else:
   assert r['final_macl']==44*unit;n+=1
   event=next(b for b in r['boundaries'] if b['target']=='0x8c65f810')
   assert event['arguments'][0:2]==[unit,command] and event['arguments'][3]==2;n+=2
   if not busy:
    assert v.load(a,fixtures.CTX+4064)==(((serial+1)&0x7fffffff) or 1) and a[(fixtures.CTX+0x2e+unit*44)&0xffffff]==command;n+=2
  checks+=n;seen.update(path)
  for j,pc in enumerate(path[:-1]):
   if v.ROWS.get(pc,'').startswith(('bt ','bf ')):edges.add((hex(pc),hex(path[j+1])))
  records.append(dict(kind=kind,entry=hex(entry),unit=unit,lock=lock,busy=busy,serial=serial,order=order,mode=mode,sr=hex(sr),status=r['status'],stop_pc=r['stop_pc'],result=hex(s.r[0]),command_words=words,independent_checks=n))
 for entry,lock in itertools.product(ENTRIES,range(256)):
  mode,sr=fixtures.CONFIGS[lock%4];run('all_mutex_bytes',entry,1,[1,2,0,0],mode,sr,lock=lock)
  if smoke:break
 print('REQUEST LOCK',len(records),flush=True)
 configurations=((0,1,0,0,0,0),(0,0x7fffffff,1,0,0,1),(0,0x12345678,0,2,1,0),(0xff,1,1,0,0,1))
 for entry,unit,(mode,sr),(busy,serial,order,padding,previous,bank) in itertools.product(ENTRIES,(1,20),fixtures.CONFIGS,configurations):
  run('packet_api',entry,unit,[unit,0x89abcdef,0xa5a5a5a5,0x12345678],mode,sr,busy=busy,serial=serial,order=order,padding=padding,previous=previous,bank=bank)
  if smoke:break
 print('REQUEST PACKETS',len(records),flush=True)
 if smoke:return
 coverage=[]
 for body in v.manifest['routines']:
  missing=set(body['reachable_addresses'])-{hex(x) for x in seen};missing_edges=set(map(tuple,body['conditional_edges']))-edges
  if int(body['address'],16) in (0x8c660494,0x8c6604e6,0x8c6604c0,0x8c660518):assert not missing and not missing_edges,(body['address'],missing,missing_edges)
  coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(missing_edges),missing_rows=sorted(missing),missing_outcomes=sorted(missing_edges)))
 files=[Path(__file__),Path(fixtures.__file__),Path(v.__file__),Path(fast.__file__),P/'channel_request_reference_cpu.py',v.MP,P/'analysis/movie_channel_request_connection.json',v.effects.DLL]
 report=dict(original_native_comparisons=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Four request bodies execute all89rows/fourbranchoutcomes. All256mutexbytevalues perentry; independentbyte-neighbors,Tflags,gueststackbalance,callee-preservedGPR/FPU/specialstate,packetcontext/frame/serial/endianness/busy/mapping/bank/padding oracles. Actual canonical packetbuilder executes. Diagnosticconditionzero tested; active diagnostic is separate. CoherentRAM single guestexecutor/TASpurgeevent only, not physicalcontroller,hostthreadatomicity/cache/TLB/fullgame qualification.')
 (P/'analysis/movie_channel_request_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('CHANNEL REQUEST PASS',len(records),checks)
if __name__=='__main__':main()
