"""Independent response-presence/header/metadata expectations on current native code."""
import hashlib,itertools,json,sys
from pathlib import Path
import movie_input_pump_fixtures as fx
v=fx.v;P=v.P;put=fx.put;REC=0x8cc61000;RAW=0x8cc62000;INFO=0x8cc63000

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 ram=v.seed_ram();records=[];seen=set();edges=set();checks=0;smoke='--smoke' in sys.argv
 def compare(kind,data,c,m,s,f,wanted,result=0):
  nonlocal checks
  before=type(s).from_buffer_copy(s);fp=bytes(f);r,path,a,s,f=fx.fast.compare(m,s,f,c);n=3
  assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match']
  assert a==wanted and s.r[0]==result and s.r[15]==before.r[15] and s.pr==s.pc==before.pr;n+=4
  assert bytes(f)==fp and r['final_mach']==c['mach'] and r['final_macl']==c['macl'] and r['final_gbr']==c['gbr'];n+=4
  assert r['cache_purge_sequence']==[];n+=1
  checks+=n;seen.update(path)
  for j,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((hex(pc),hex(path[j+distance])))
  records.append(dict(kind=kind,input=data,status=r['status'],stop_pc=r['stop_pc'],steps=len(path),independent_checks=n))
 for command,status,(mode,sr) in itertools.product((0,1,4),(0,5,8,0xdf,0xe0,0xfe,0xff),fx.CONFIGS):
  c,m,s,f=fx.setup(ram,0x8c65ff12,[REC,RAW,0,0],mode,sr);fx.configure(m);m[REC&0xffffff:(REC&0xffffff)+44]=bytes([0x55])*44;put(m,REC+7,command,1);put(m,REC+8,status,1)
  wanted=bytearray(m);put(wanted,REC,int(status==5) if command==1 else int(status not in (0,255)),1)
  compare('presence',dict(command=command,status=status,mode=mode,sr=hex(sr)),c,m,s,f,wanted)
  if smoke:break
 print('INPUT LEAF PRESENCE',len(records),flush=True)
 for order,status,(mode,sr) in itertools.product((0,1,2),(0,5,8,0xdf,0xe0,0xfe,0xff),fx.CONFIGS):
  c,m,s,f=fx.setup(ram,0x8c65ff5a,[REC,RAW,0,0],mode,sr);fx.configure(m);put(m,fx.CTX+0x10c4,order);m[REC&0xffffff:(REC&0xffffff)+44]=bytes([0x55])*44
  header=bytes([status,0x23,0x45,0x67]);raw=header[::-1] if order==1 else header;m[RAW&0xffffff:(RAW&0xffffff)+4]=raw;put(m,RAW+4,0x89abcdef)
  wanted=bytearray(m);wanted[(REC+8)&0xffffff:(REC&0xffffff)+12]=header;put(wanted,REC+0x14,0xffffffff if status==255 else 0x89abcdef if status>=0xe0 else 0);put(wanted,s.r[15]-4,int.from_bytes(raw,'little'))
  compare('header',dict(order=order,status=status,mode=mode,sr=hex(sr)),c,m,s,f,wanted)
  if smoke:break
 print('INPUT LEAF HEADER',len(records),flush=True)
 for order,flag,(mode,sr) in itertools.product((0,1,2),(0,1),fx.CONFIGS):
  c,m,s,f=fx.setup(ram,0x8c66018e,[INFO,0,RAW,0],mode,sr);fx.configure(m);put(m,fx.CTX+0x10c4,order);m[INFO&0xffffff:(INFO&0xffffff)+128]=bytes([0xa5])*128;put(m,INFO+16,flag,1)
  raw=bytes((17*i+11)&255 for i in range(128));m[RAW&0xffffff:(RAW&0xffffff)+128]=raw
  wanted=bytearray(m);out=bytearray(wanted[INFO&0xffffff:(INFO&0xffffff)+120]);out[0:4]=raw[4:8][::-1];out[4:16]=raw[8:20];out[17]=raw[20]
  if flag:out[18]=raw[21]&3;out[19]=(raw[21]>>2)&3
  else:out[18]=v.sample(m,v.load(m,0x8c660370)+(raw[21]&15),1)[0];out[19]=0
  if order==1:out[20:51]=raw[21:52]
  else:out[20:50]=raw[22:52];out[50]=0
  out[51]=0;out[52:112]=raw[52:112];out[112]=0;out[116:120]=raw[112:116];wanted[INFO&0xffffff:(INFO&0xffffff)+120]=out
  compare('metadata',dict(order=order,flag=flag,mode=mode,sr=hex(sr)),c,m,s,f,wanted)
  if smoke:break
 print('INPUT LEAF METADATA',len(records),flush=True)
 if smoke:return
 coverage=[]
 for body in v.manifest['routines']:
  missing=set(body['reachable_addresses'])-{hex(x) for x in seen};missing_edges=set(map(tuple,body['conditional_edges']))-edges
  if int(body['address'],16) in (0x8c65ff12,0x8c65ff5a,0x8c66018e):assert not missing and not missing_edges,(body['address'],missing,missing_edges)
  coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(missing_edges),missing_rows=sorted(missing),missing_outcomes=sorted(missing_edges)))
 files=[Path(__file__),Path(fx.__file__),Path(v.__file__),Path(fx.fast.__file__),v.MP,P/'analysis/movie_input_pump_connection.json',P/'input_pump_reference_cpu.py',v.effects.DLL]
 report=dict(original_native_comparisons=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='IndependententireRAM/stack/special/FPU/presence/headerendianness/value/metadatafield/stringpadding oracles pluswholeoriginalmachine/boundaries. Allthreeleafbodies227rows/30outcomes execute. Actualrawresponsebytes are explicitinputs;no call orparserresult stubs. Physicaldevice/wholegame notqualified.')
 (P/'analysis/movie_input_pump_leaves_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('INPUT PUMP LEAVES PASS',len(records),checks)
if __name__=='__main__':main()
