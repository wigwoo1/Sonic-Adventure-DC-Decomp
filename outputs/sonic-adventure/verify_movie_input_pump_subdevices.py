"""Subdevice presence transitions, retry boundaries and actual metadata effects."""
import hashlib,itertools,json,sys
from pathlib import Path
import movie_input_pump_fixtures as fx
v=fx.v;P=v.P;put=fx.put;REC=0x8cc68000;INFO=0x8cc69000;PORT=0x8cc6a000;RAW=0x8cc6b000

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 ram=v.seed_ram();records=[];seen=set();edges=set();checks=0;smoke='--smoke' in sys.argv
 transitions=((1,0,0),(1,1,0),(0,0,1),(0,0,0))
 for order,(present,new,removed),command,status,retries in itertools.product((0,1),transitions,(0,1,4),(0,5,8,255),(0,3)):
  c,m,s,f=fx.setup(ram,0x8c66005c,[1,PORT,INFO,REC],4 if command%2 else 0,0x400000a1);fx.configure(m);put(m,fx.CTX+0x10c4,order);put(m,fx.CTX+0x10c0,2)
  m[REC&0xffffff:(REC&0xffffff)+44]=bytes(44);put(m,REC,1,1);put(m,REC+4,9,1);put(m,REC+7,command,1);put(m,REC+0x24,retries);put(m,REC+0x1c,0x12345678);put(m,REC+0x20,0x87654321)
  m[INFO&0xffffff:(INFO&0xffffff)+120]=bytes([0xa5])*120
  for off,value in ((0,present),(2,new),(3,removed)):put(m,PORT+off,value,1)
  header=bytes([status,0x20,1,30]);m[RAW&0xffffff:(RAW&0xffffff)+4]=header[::-1] if order else header;m[(RAW+4)&0xffffff:(RAW&0xffffff)+260]=bytes(range(256));put(m,s.r[15],RAW)
  before=type(s).from_buffer_copy(s);fp=bytes(f);r,path,a,s,f=fx.fast.compare(m,s,f,c);n=3
  assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match'],(order,present,new,removed,command,status,retries,r['status'],r['stop_pc'])
  assert s.r[0]==0 and s.r[15]==before.r[15] and s.pr==s.pc==before.pr and list(s.r)[8:15]==list(before.r)[8:15];n+=4
  assert v.sample(a,REC+8,4)==header and v.load(a,REC+0x1c)==0x12345678 and v.load(a,REC+0x20)==0x87654321;n+=3
  assert v.sample(a,PORT,4)==v.sample(m,PORT,4) and v.sample(a,REC+44,16)==v.sample(m,REC+44,16) and v.sample(a,INFO+120,16)==v.sample(m,INFO+120,16);n+=3
  if not present:assert v.sample(a,REC,1)==b'\0';n+=1
  assert bytes(f)==fp and r['final_mach']==c['mach'] and r['final_macl']==c['macl'] and r['final_gbr']==c['gbr'];n+=4
  checks+=n;seen.update(path)
  for j,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((hex(pc),hex(path[j+distance])))
  records.append(dict(order=order,present=present,new=new,removed=removed,command=command,response_status=status,retries=retries,status=r['status'],stop_pc=r['stop_pc'],steps=len(path),independent_checks=n))
  if smoke:break
 if smoke:print('INPUT SUBDEVICE SMOKE');return
 coverage=[]
 for body in v.manifest['routines']:
  missing=set(body['reachable_addresses'])-{hex(x) for x in seen};missing_edges=set(map(tuple,body['conditional_edges']))-edges
  coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(missing_edges),missing_rows=sorted(missing),missing_outcomes=sorted(missing_edges)))
 files=[Path(__file__),Path(fx.__file__),Path(v.__file__),Path(fx.fast.__file__),v.MP,P/'analysis/movie_input_pump_connection.json',v.effects.DLL]
 report=dict(original_native_comparisons=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Actualsubdevice/header/swap/metadata/infoinitialize code. Declaredparentpresence/insert/remove/rawresponse/retry inputs. Independentheader/token-preservation/absent-presence/neighbor/stack/special-state expectations plusentiremachine/boundaries. No callstub,completedtokenorparsedresult substitution. Not physicalcontroller acquisition.')
 (P/'analysis/movie_input_pump_subdevices_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('INPUT PUMP SUBDEVICES PASS',len(records),checks)
if __name__=='__main__':main()
