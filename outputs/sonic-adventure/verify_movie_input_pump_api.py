"""Original request APIs now execute enabled automatic pump and preserve serials."""
import hashlib,itertools,json,sys
from pathlib import Path
import movie_input_pump_fixtures as fx
import movie_channel_request_fixtures as request
v=fx.v;P=v.P;put=fx.put

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 ram=v.seed_ram();records=[];checks=0;seen=set();edges=set();smoke='--smoke' in sys.argv
 def note(kind,data,r,path,n):
  nonlocal checks
  checks+=n;seen.update(path)
  for j,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((hex(pc),hex(path[j+distance])))
  records.append(dict(kind=kind,input=data,status=r['status'],stop_pc=r['stop_pc'],steps=len(path),independent_checks=n))
 for entry,unit,pump,sdk_status,busy,(mode,sr) in itertools.product((0x8c646982,0x8c6469c8),(1,20),(0,1),(0,1),(0,0xff),fx.CONFIGS):
  c,m,s,f=fx.setup(ram,entry,[unit,0x89abcdef,0xa5a5a5a5,0x12345678],mode,sr);fx.configure(m,status=sdk_status);put(m,fx.CTX+0x2e+unit*44,busy,1)
  put(m,fx.CTX+0x10e8,pump);put(m,0x8c893e18,0);put(m,s.r[15],0x13579bdf)
  before=type(s).from_buffer_copy(s);fp=bytes(f);backend=0x8c660494 if entry==0x8c646982 else 0x8c6604e6;expected,result,opcode,words=request.expected_packet(m,s,backend,0x13579bdf)
  r,path,a,s,f=fx.fast.compare(m,s,f,c);n=3;assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match']
  assert s.r[0]==result and s.r[15]==before.r[15] and s.pr==s.pc==before.pr and list(s.r)[8:15]==list(before.r)[8:15];n+=4
  assert v.sample(a,request.FRAME,512)==v.sample(expected,request.FRAME,512) and v.load(a,fx.CTX+4064)==v.load(expected,fx.CTX+4064);n+=2
  assert a[fx.LOCK&0xffffff]==0 and v.load(a,fx.CTX+0xfc0)==(1 if pump else sdk_status);n+=2
  assert r['cache_purge_sequence']==[fx.LOCK]*(1+int(pump and not sdk_status));n+=1
  targets=[b['target'] for b in r['boundaries']];assert targets.count('0x8c646654')==pump and targets.count('0x8c65f202')==int(pump and not sdk_status);n+=2
  assert bytes(f)==fp and r['final_mach']==c['mach'] and r['final_gbr']==c['gbr'];n+=3
  # Queueing alone does not create a submitted/completed hardware token.
  rec=fx.CTX+0x28+unit*44;assert v.load(a,rec+0x18)==(result if not busy else 0) and v.load(a,rec+0x1c)==0 and v.load(a,rec+0x20)==0;n+=3
  note('automatic_pump_api',dict(entry=hex(entry),unit=unit,pump=pump,sdk_status=sdk_status,busy=busy,mode=mode,sr=hex(sr),words=words),r,path,n)
  if smoke:break
 print('INPUT API',len(records),flush=True)
 for callback,previous_selection,(mode,sr) in itertools.product((0,0x8c05180c,0x8c04fe40,0x8cfef000),(0,2),fx.CONFIGS):
  c,m,s,f=fx.setup(ram,0x8c646654,[0,0,0,0],mode,sr);fx.configure(m,active=1);put(m,0x8c893e4c,callback);selection=v.load(m,0x8c646758);assert selection==0x8c8940a4;put(m,selection,previous_selection)
  put(m,v.load(m,0x8c04fe8c),0,2)
  # The original selector's mask selects a port with no eligible info flags.
  # Getter and selector read the same +fb0 word. NOP preserves getter R0=1;
  # the actual game-mode predicate returns false for declared game-mode zero.
  # No callback return is substituted.
  offset=int.from_bytes(v.sample(m,0x8c646616,2),'little',signed=True);assert offset==int.from_bytes(v.sample(m,0x8c65f52c,2),'little',signed=True)==0xfb0;put(m,fx.CTX+offset,1,1)
  before=type(s).from_buffer_copy(s);fp=bytes(f);r,path,a,s,f=fx.fast.compare(m,s,f,c);n=3;unknown=callback==0x8cfef000
  assert r['status']==(-2 if unknown else 0) and r['full_state_match'] and r['ordered_boundaries_match']
  targets=[b['target'] for b in r['boundaries']];assert ('0x8c64658a' in targets)==(callback in (0,0x8c05180c));n+=1
  expected=0 if callback in (0,0x8c05180c) else previous_selection
  assert v.load(a,selection)==expected and a[fx.LOCK&0xffffff]==0 and v.load(a,fx.CTX+0xfc0)==1;n+=3
  if unknown:assert r['stop_pc']==hex(callback) and s.pr==0x8c6466de;n+=2
  else:assert s.r[0]==0 and s.r[15]==before.r[15] and s.pr==s.pc==before.pr;n+=3
  assert bytes(f)==fp and r['final_mach']==c['mach'] and r['final_gbr']==c['gbr'];n+=3
  note('selection_gate',dict(callback=hex(callback),previous_selection=previous_selection,mode=mode,sr=hex(sr)),r,path,n)
  if smoke:break
 if smoke:return
 # Merge source-level rows for the old outer APIs, which remain byte-identical.
 parent=json.loads((P/'analysis/movie_channel_io_translation.json').read_text());coverage=[]
 for body in list(v.manifest['routines'])+[r for r in parent['routines'] if r['address'] in ('0x8c646982','0x8c6469c8')]:
  missing=set(body['reachable_addresses'])-{hex(x) for x in seen};missing_edges=set(map(tuple,body['conditional_edges']))-edges
  if body['address'] in ('0x8c646982','0x8c6469c8'):assert not missing and not missing_edges,(body['address'],missing,missing_edges)
  coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(missing_edges),missing_rows=sorted(missing),missing_outcomes=sorted(missing_edges)))
 files=[Path(__file__),Path(fx.__file__),Path(request.__file__),Path(v.__file__),Path(fx.fast.__file__),v.MP,P/'analysis/movie_input_pump_connection.json',P/'analysis/movie_channel_io_translation.json',v.effects.DLL]
 report=dict(original_native_comparisons=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Actual646982/6469c8 withdisabled/enabledpump,preflightstatus0/1,busy/freequeuedrequest,extra stackargument andunit/SR/FPU matrix. Originalpacket->actualpump->actualnegative/success return preservesrequestserial,doesnotinventinflight/completedtoken. Independentpacket/serial/stack/purge/queue-state oracles. Realactive-selection null/NOP/unknowncallbackgate andpreviousvalidselectionprofiles;fullmachine/device/boundary match. No physicalsubmit/RX/naturalactiveparsercompletion proof.')
 (P/'analysis/movie_input_pump_api_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('INPUT PUMP API PASS',len(records),checks)
if __name__=='__main__':main()
