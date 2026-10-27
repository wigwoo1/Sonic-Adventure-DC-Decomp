"""Actual response-driven callback table and active selector; no callback stubs."""
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
 for slot,callback,matched in itertools.product(range(48),(0x8c05180c,0x8cfef000),(False,True)):
  unit=slot%24;port=unit//6;mode=4 if slot%2 else 0;c,m,s,f=fx.setup(ram,0x8c646654,[0,0,0,0],mode,0x400000a1);fx.configure(m)
  parent=port*6;put(m,fx.CTX+0xf90+parent,1,1);put(m,fx.CTX+0xf90+unit,1,1)
  for target in sorted({parent,unit}):
   d=fx.seed_response(m,target,status=8,present=1,command=4,token=7+unit if target==unit else 0,port=port)
   header=bytes([8,0x20,0x1f,30]);m[d['response']&0xffffff:(d['response']&0xffffff)+4]=header
  table=0x8c893e50+slot*12;token=7+unit;put(m,table,unit);put(m,table+4,token if matched else token+1);put(m,table+8,callback)
  before=type(s).from_buffer_copy(s);fp=bytes(f);r,path,a,s,f=fx.fast.compare(m,s,f,c);n=3;unknown=matched and callback==0x8cfef000
  assert r['status']==(-2 if unknown else 0) and r['full_state_match'] and r['ordered_boundaries_match']
  assert v.load(a,fx.CTX+0x28+unit*44+0x1c)==0 and v.load(a,fx.CTX+0x28+unit*44+0x20)==token;n+=2
  assert v.load(a,table+8)==(0 if matched else callback) and v.load(a,table)==unit and v.load(a,table+4)==(token if matched else token+1);n+=3
  events=[b for b in r['boundaries'] if b['target']==hex(callback) and b['pr']=='0x8c6467a4'];assert len(events)==int(matched);n+=1
  if matched:
   wanted=[unit,token,v.load(m,0x8c6468b0)+unit*24,fx.CTX+0x44c+unit*120];assert events[0]['arguments']==wanted;n+=1
  if unknown:assert r['stop_pc']==hex(callback) and s.pr==0x8c6467a4 and s.r[15]==before.r[15]-36;n+=3
  else:assert s.r[0]==0 and s.r[15]==before.r[15] and s.pr==s.pc==before.pr;n+=3
  assert bytes(f)==fp and r['final_mach']==c['mach'] and r['final_gbr']==c['gbr'] and a[fx.LOCK&0xffffff]==0;n+=4
  note('response_callback',dict(slot=slot,unit=unit,callback=hex(callback),matched=matched,mode=mode),r,path,n)
  if smoke:break
 print('PUMP CALLBACKS',len(records),flush=True)
 for mask,flags,state,kind,bit,oldbit,(mode,sr) in itertools.product((0,1,5,15),(0,0x80),(0,1),(7,8),(0,4),(0,4),fx.CONFIGS[:2]):
  c,m,s,f=fx.setup(ram,0x8c64658a,[0,0,0,0],mode,sr);fx.configure(m)
  offset=int.from_bytes(v.sample(m,0x8c646616,2),'little',signed=True);put(m,fx.CTX+offset,mask,1)
  indices=list(v.sample(m,v.load(m,0x8c646634),4));current=v.load(m,0x8c64662c);previous=v.load(m,0x8c646630)
  m[current&0xffffff:(current&0xffffff)+4]=bytes([oldbit])*4;m[previous&0xffffff:(previous&0xffffff)+4]=bytes([0xa5])*4
  for j,index in enumerate(indices):
   rec=v.load(m,0x8c6468b0)+index*24;info=fx.CTX+0x44c+index*120;raw=0x8cc70000+index*0x100
   put(m,info,flags);put(m,rec+2,state,1);put(m,rec+4,kind,1);put(m,rec+8,raw);put(m,raw+8,bit,1)
  before=type(s).from_buffer_copy(s);fp=bytes(f);wanted=bytearray(m);put(wanted,s.r[15]-32,s.pr)
  eligible=flags&0x80 and state==0 and kind==8;expected=next((j+1 for j in range(4) if mask&(1<<j) and eligible and bit==0 and oldbit==4),0)
  r,path,a,s,f=fx.fast.compare(m,s,f,c);n=3;assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match']
  assert s.r[0]==expected and s.r[15]==before.r[15] and s.pr==s.pc==before.pr and list(s.r)[8:15]==list(before.r)[8:15];n+=4
  last=expected-1 if expected else 3
  expected_current=bytes(bit if eligible and mask&(1<<j) and j<=last else oldbit for j in range(4));expected_previous=bytes(oldbit if mask&(1<<j) and j<=last else 0xa5 for j in range(4))
  assert v.sample(a,current,4)==expected_current and v.sample(a,previous,4)==expected_previous;n+=2
  assert bytes(f)==fp and r['final_mach']==c['mach'] and r['final_gbr']==c['gbr'];n+=3
  note('selector',dict(mask=mask,flags=flags,state=state,kind=kind,bit=bit,oldbit=oldbit,mode=mode,sr=hex(sr)),r,path,n)
  if smoke:break
 print('PUMP SELECTOR',len(records),flush=True)
 if smoke:return
 coverage=[]
 for body in v.manifest['routines']:
  missing=set(body['reachable_addresses'])-{hex(x) for x in seen};missing_edges=set(map(tuple,body['conditional_edges']))-edges
  coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(missing_edges),missing_rows=sorted(missing),missing_outcomes=sorted(missing_edges)))
 files=[Path(__file__),Path(fx.__file__),Path(v.__file__),Path(fx.fast.__file__),v.MP,P/'analysis/movie_input_pump_connection.json',v.effects.DLL]
 report=dict(original_native_comparisons=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='All48callbackslots,actualresponse-publishedtokenmatches/misses,NOP/unknowncallbacktargets andpre-callclear/arguments independentlychecked. RawRX/inflighttokens explicit,no completedtokenorcallbackreturnstub. Active-selectusesunchangedretailtable/actualpointerhelpers andindependentmask/flags/state/type/bittransition/firsteligibleindex oracles. Wholemachine/boundaries compare.')
 (P/'analysis/movie_input_pump_notifications_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('INPUT PUMP NOTIFICATIONS PASS',len(records),checks)
if __name__=='__main__':main()
