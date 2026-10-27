"""Original preflight/error/retry/notification and port-mask control behavior."""
import hashlib,itertools,json,sys
from pathlib import Path
import movie_input_pump_fixtures as fx
v=fx.v;P=v.P;put=fx.put

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 ram=v.seed_ram();records=[];seen=set();edges=set();checks=0;smoke='--smoke' in sys.argv
 def note(kind,data,r,path,n):
  nonlocal checks
  checks+=n;seen.update(path)
  for j,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((hex(pc),hex(path[j+distance])))
  records.append(dict(kind=kind,input=data,status=r['status'],stop_pc=r['stop_pc'],steps=len(path),independent_checks=n))
 for status,flags,lock,callback,(mode,sr) in itertools.product((0,1),(0,1),(0,1),(0,0x8c05180c,0x8cfef000),fx.CONFIGS):
  c,m,s,f=fx.setup(ram,0x8c646654,[0,0,0,0],mode,sr);fx.configure(m,status=status,device_flags=flags,lock=lock);put(m,0x8c893e48,callback)
  before=type(s).from_buffer_copy(s);fp=bytes(f);r,path,a,s,f=fx.fast.compare(m,s,f,c);result=0xfffffffd if status else 0xffffffff if flags else 0xfffffffe if lock else 0;n=3;unknown=bool(callback==0x8cfef000 and result in (0xfffffffd,0xffffffff))
  assert r['status']==(-2 if unknown else 0) and r['full_state_match'] and r['ordered_boundaries_match']
  assert a[fx.LOCK&0xffffff]==((lock|0x80) if not status and not flags and lock else lock) and r['cache_purge_sequence']==([fx.LOCK] if not status and not flags else []);n+=2
  # Nonacquired lock does not run response/rearm or consume display samples.
  targets=[b['target'] for b in r['boundaries']];assert ('0x8c65f202' in targets)==(result==0);n+=1
  if unknown:
   event=next(b for b in r['boundaries'] if b['target']==hex(callback));assert event['arguments'][:2]==[2,0xfffffff8] and event['arguments'][3]==result and event['pr']=='0x8c6467d6';n+=3
   assert r['stop_pc']==hex(callback) and s.r[15]==before.r[15]-36;n+=2
  else:
   assert s.r[0]==result and s.r[15]==before.r[15] and s.pr==s.pc==before.pr and list(s.r)[8:15]==list(before.r)[8:15];n+=4
  assert v.load(a,fx.CTX+0xfc0)==(1 if result==0 else status);n+=1
  if result==0xfffffffe:assert v.load(a,fx.CTX+0x10e8)==1;n+=1
  assert bytes(f)==fp and r['final_mach']==c['mach'] and r['final_gbr']==c['gbr'];n+=3
  note('preflight',dict(status=status,flags=flags,lock=lock,callback=hex(callback),mode=mode,sr=hex(sr)),r,path,n)
  if smoke:break
 print('PUMP PREFLIGHT',len(records),flush=True)
 for port,order,old,new,kind,retries,(mode,sr) in itertools.product((0,3),(0,1),(0,0x83),(1,0x1f),('valid','ff','ff00'),(0,3,0xffffffff),fx.CONFIGS[:2]):
  c,m,s,f=fx.setup(ram,0x8c65fd4e,[port,0,0,0],mode,sr);fx.configure(m);put(m,fx.CTX+0x10c4,order);put(m,fx.CTX+0x10dc,3)
  record=fx.CTX+4+port*8;m[record&0xffffff:(record&0xffffff)+4]=bytes([old,0x55,0x66,0x77]);put(m,record+4,retries)
  pointer=v.load(m,fx.CTX+0xfe8+96+port*6*4)
  packet=bytes([5,new,0,0]) if order else bytes([5,0,new,0])
  if kind!='valid':packet=(0xffffffff if kind=='ff' else 0xffffff00).to_bytes(4,'little')
  m[pointer&0xffffff:(pointer&0xffffff)+4]=packet
  before=type(s).from_buffer_copy(s);fp=bytes(f);wanted=bytearray(m);put(wanted,s.r[15]-4,int.from_bytes(packet,'little'))
  retry=(retries+1)&0xffffffff
  if kind=='valid':wanted[record&0xffffff:(record&0xffffff)+4]=bytes([new,old,new&~old,old&~new]);put(wanted,record+4,0);result=0
  else:
   put(wanted,record+4,retry);result=0xffffffff if retry>3 else 0
   if retry>3:wanted[record&0xffffff:(record&0xffffff)+4]=bytes([0,old,0,old])
  r,path,a,s,f=fx.fast.compare(m,s,f,c);n=3;assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match']
  assert a==wanted and s.r[0]==result and s.r[15]==before.r[15] and s.pr==s.pc==before.pr;n+=4
  assert bytes(f)==fp and r['final_mach']==c['mach'] and r['final_macl']==c['macl'] and r['final_gbr']==c['gbr'];n+=4
  note('portmask',dict(port=port,order=order,old=old,new=new,kind=kind,retries=retries,mode=mode),r,path,n)
  if smoke:break
 print('PUMP PORTMASK',len(records),flush=True)
 # Completion busy flag clear/set is independent of inflight token transfer.
 for unit,busy,(mode,sr) in itertools.product((0,1),(0,1),fx.CONFIGS):
  c,m,s,f=fx.setup(ram,0x8c65fe14,[unit,0,0,0],mode,sr);fx.configure(m);d=fx.seed_response(m,unit,command=4,token=7);put(m,d['record']+0xd,busy,1);before=type(s).from_buffer_copy(s)
  r,path,a,s,f=fx.fast.compare(m,s,f,c);n=3;assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match']
  assert v.load(a,d['record']+0x20)==7 and v.load(a,d['record']+0x1c)==0 and v.sample(a,d['record']+0xd,1)==b'\0';n+=3
  assert s.r[15]==before.r[15] and s.pr==s.pc==before.pr;n+=2
  note('busyflag',dict(unit=unit,busy=busy,mode=mode,sr=hex(sr)),r,path,n)
  if smoke:break
 if smoke:return
 coverage=[]
 for body in v.manifest['routines']:
  missing=set(body['reachable_addresses'])-{hex(x) for x in seen};missing_edges=set(map(tuple,body['conditional_edges']))-edges
  coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(missing_edges),missing_rows=sorted(missing),missing_outcomes=sorted(missing_edges)))
 files=[Path(__file__),Path(fx.__file__),Path(v.__file__),Path(fx.fast.__file__),v.MP,P/'analysis/movie_input_pump_connection.json',v.effects.DLL]
 report=dict(original_native_comparisons=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Real preflight/negative-status/error callback/nullNOPunknown boundaries,TASpurge/rearm,portmask insertion/removal/retrythreshold/unsignedoverflow/byteorder andcompletionbusyflag controls. IndependententireRAM/stack/field/state oracles;rawpacket/oldinflightinputs explicit,no completedtoken/parserresult injection.')
 (P/'analysis/movie_input_pump_controls_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('INPUT PUMP CONTROLS PASS',len(records),checks)
if __name__=='__main__':main()
