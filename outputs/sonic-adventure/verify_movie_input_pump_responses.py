"""Actual multi-port response processing, inflight-token and published-byte oracles."""
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
 for unit,order,bank,status,command,token in itertools.product((0,1,6,7,12,17,18,23),(0,1),(0,1),(0,5,8,255),(0,1,4),(0,7)):
  mode=4 if unit%2 else 0;c,m,s,f=fx.setup(ram,0x8c65fe14,[unit,unit//6,0,0],mode,0x400000a1);fx.configure(m)
  d=fx.seed_response(m,unit,status=status,present=1,command=command,token=token,order=order,buffer_bank=bank,port=unit//6);put(m,d['record']+0x20,99)
  # Port state is declared received device presence; submitted token is an
  # existing inflight fixture, never an injected completed result for a task.
  portstate=fx.CTX+4+(unit//6)*8;put(m,portstate,0x1f,1);put(m,portstate+1,0x1f,1)
  put(m,fx.CTX+0x10c0,2)
  header=bytes([status,0x20,0x1f,30]);m[d['response']&0xffffff:(d['response']&0xffffff)+4]=header[::-1] if order else header
  before=type(s).from_buffer_copy(s);fp=bytes(f);r,path,a,s,f=fx.fast.compare(m,s,f,c);n=3
  assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match'],(unit,order,bank,status,command,token,r['status'],r['stop_pc'])
  assert v.load(a,d['record']+0x1c)==0 and v.load(a,d['record']+0x20)==(token or 99);n+=2
  assert v.sample(a,d['record']+8,4)==header;n+=1
  assert s.r[15]==before.r[15] and s.pr==s.pc==before.pr and list(s.r)[8:15]==list(before.r)[8:15];n+=3
  assert bytes(f)==fp and r['final_mach']==c['mach'] and r['final_gbr']==c['gbr'];n+=3
  assert r['cache_purge_sequence']==[];n+=1
  expected_leaf='0x8c65ffb8' if unit%6==0 else '0x8c66005c';assert expected_leaf in [b['target'] for b in r['boundaries']];n+=1
  note('complete',dict(unit=unit,order=order,buffer_bank=bank,status=status,command=command,token=token,mode=mode),r,path,n)
  if smoke:break
 print('INPUT COMPLETE',len(records),flush=True)
 # Entire pump uses the real port-mask walker, response completion, status
 # publisher and rearm. Each selected port has a primary device response.
 for mask,order,bank,(mode,sr) in itertools.product((0,1,5,15),(0,1),(0,1),fx.CONFIGS):
  c,m,s,f=fx.setup(ram,0x8c646654,[0,0,0,0],mode,sr);fx.configure(m);put(m,fx.CTX+0x10c4,order);put(m,fx.CTX+0x10b4,bank)
  affected=[]
  for port in range(4):
   if not mask&(1<<port):continue
   unit=port*6;put(m,fx.CTX+0xf90+unit,1,1);d=fx.seed_response(m,unit,status=5,present=0,command=1,token=7+unit,order=order,buffer_bank=bank,port=port)
   header=bytes([5,0x20,0,30]);m[d['response']&0xffffff:(d['response']&0xffffff)+4]=header[::-1] if order else header;affected.append(d)
  before=type(s).from_buffer_copy(s);fp=bytes(f);r,path,a,s,f=fx.fast.compare(m,s,f,c);n=3
  assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match'],(mask,order,bank,mode,r['status'],r['stop_pc'])
  assert v.load(a,fx.CTX+0xfc0)==1 and a[fx.LOCK&0xffffff]==0 and r['cache_purge_sequence']==[fx.LOCK];n+=3
  assert s.r[0]==0 and s.r[15]==before.r[15] and s.pr==s.pc==before.pr and list(s.r)[8:15]==list(before.r)[8:15];n+=4
  assert bytes(f)==fp and r['final_mach']==c['mach'] and r['final_gbr']==c['gbr'];n+=3
  for d in affected:
   assert v.load(a,d['record']+0x1c)==0 and v.load(a,d['record']+0x20)==d['token'];n+=2
  # Independent 24-record publication projection, including preserved holes.
  output=v.load(m,0x8c6462b0)
  for unit in range(24):
   rec=fx.CTX+0x28+unit*44;dest=output+24*unit
   wanted=bytearray(v.sample(m,dest,24));wanted[0:4]=v.sample(a,rec,4);wanted[4:8]=v.sample(a,rec+8,4);wanted[8:12]=v.sample(a,rec+0x10,4);wanted[12:16]=v.sample(a,rec+0x14,4)
   assert v.sample(a,dest,24)==wanted;n+=1
  targets=[b['target'] for b in r['boundaries']];assert targets.count('0x8c65fe14')==mask.bit_count() and '0x8c6461ae' in targets and '0x8c65f1ec' in targets;n+=3
  note('multiport_pump',dict(mask=mask,order=order,buffer_bank=bank,mode=mode,sr=hex(sr)),r,path,n)
  if smoke:break
 print('INPUT MULTIPORT',len(records),flush=True)
 if smoke:return
 coverage=[]
 for body in v.manifest['routines']:
  missing=set(body['reachable_addresses'])-{hex(x) for x in seen};missing_edges=set(map(tuple,body['conditional_edges']))-edges
  coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(missing_edges),missing_rows=sorted(missing),missing_outcomes=sorted(missing_edges)))
 files=[Path(__file__),Path(fx.__file__),Path(v.__file__),Path(fx.fast.__file__),v.MP,P/'analysis/movie_input_pump_connection.json',P/'input_pump_reference_cpu.py',v.effects.DLL]
 report=dict(original_native_comparisons=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Realresponsecomplete/primary/sub/header/presence/metadata/swap chain onunchangedretailunitmap;explicitrawRXbuffers/inflighttokens andactualreturnedpublication. Wholepump four-portmask/wordorder/bufferbank/SRFR matrix,independenttoken/stack/state/24recordpublishedprojection oracles. No callback/completion/parserresultstubs;syntheticdeviceinputs donotprovephysicalhardware,naturalDMAorsuccessfulactivegameparser.')
 (P/'analysis/movie_input_pump_responses_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('INPUT PUMP RESPONSES PASS',len(records),checks)
if __name__=='__main__':main()
