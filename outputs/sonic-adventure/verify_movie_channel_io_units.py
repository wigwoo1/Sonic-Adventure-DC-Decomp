"""Independent readiness/status/metadata expectations with actual canonical code."""
import hashlib,itertools,json,sys
from pathlib import Path
import movie_channel_io_fixtures as fixtures
v=fixtures.v;P=v.P;fast=fixtures.fast;put=fixtures.put
READY=0x8c660dec;META=0x8c646bbc;OUT=0x8cc61000;METADATA=0x8cc60000
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
CONFIGS=((0,0),(1,0x400000a1),(4,0x600000a1),(7,0x400080a1))
def setup(ram,entry,args,mode,sr):
 c=v.Input(dict(entry=entry,mode=mode,sr=sr,mach=0x89abcdef,macl=0x76543210,gbr=0x8ce01234,channel_io_arguments=list(args)+[0]*(4-len(args))));m,s,f=v.prepare(ram,c);s.sr=sr;f.fpscr=0x40001|(mode<<19)
 for bank in range(2):
  for reg in range(16):f.banks[bank][reg]=(0x7fa12345^((bank+1)*0x11111111)^((reg+1)*0x01020304))&0xffffffff
 return c,m,s,f
def main():
 ram=v.seed_ram();records=[];seen=set();edges=set();checks=0;smoke='--smoke' in sys.argv
 def note(kind,data,r,path,n):
  nonlocal checks
  checks+=n;seen.update(path)
  for j,pc in enumerate(path[:-1]):
   if v.ROWS.get(pc,'').startswith(('bt ','bf ')):edges.add((hex(pc),hex(path[j+1])))
  records.append(dict(kind=kind,input=data,status=r['status'],stop_pc=r['stop_pc'],steps=len(path),independent_checks=n))
 # Full original readiness tree, including signed header bytes and unsigned busy.
 for index,(active,busy,flags),(mode,sr) in itertools.product((0,7),((0,0,2),(1,1,2),(1,0,0),(1,0,2),(0x80,0xff,2),(0xff,0,0xffffffff)),CONFIGS):
  c,m,s,f=setup(ram,READY,[index],mode,sr);d=fixtures.configure(m,index,active,busy,flags);before=type(s).from_buffer_copy(s);fp=bytes(f);wanted=bytearray(m);regs=list(s.r);sp=s.r[15];mapped=d['mapped'];good=bool(active and not busy and flags&2)
  for delta,value in ((4,s.r[14]),(8,s.pr),(12,d['header']),(16,d['info']),(20,0x8c660e18 if active else 0x8c660e08)):put(wanted,sp-delta,value)
  regs[0]=int(good);regs[1]=int.from_bytes(v.sample(m,0x8c65f3ca,2),'little',signed=True)&0xffffffff
  regs[2]=44 if active else (active-(256 if active&128 else 0))&0xffffffff;regs[3]=d['input_pointer'] if active else d['header'];regs[4]=d['status'] if active else (mapped*120)&0xffffffff
  t=int(not active) if not active else int(not busy) if busy else int(not flags&2)
  r,path,a,s,f=fast.compare(m,s,f,c);n=3;assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match']
  assert a==wanted and list(s.r)==regs and s.sr==(before.sr&~1)|t and s.pc==s.pr==before.pr;n+=4
  assert bytes(f)==fp and [r[k] for k in ('final_mach','final_macl','final_gbr')]==[c['mach'],mapped*(44 if active else 120),c['gbr']];n+=2
  assert sum(b['target']=='0x8c646b36' for b in r['boundaries'])==int(bool(active));n+=1
  note('readiness',dict(index=index,active=active,busy=busy,flags=flags,mode=mode,sr=hex(sr)),r,path,n)
  if smoke:break
 print('IO READINESS',sum(x['kind']=='readiness' for x in records),flush=True)
 for entry,index,(mode,sr) in itertools.product((0x8c65f390,0x8c65f3f4,0x8c65f40e,0x8c65f41e,0x8c646884,0x8c646b36,0x8c646b3c),(0,7,20,31),CONFIGS):
  token=index^0xa5a5a5a5;c,m,s,f=setup(ram,entry,[index,token],mode,sr);ptr=v.load(m,0x8c65f3d8);put(m,ptr,fixtures.INPUT_OBJECT);record=fixtures.INPUT_OBJECT+0x28+44*index;put(m,record+0x20,token if index%2 else token^1);put(m,record+0xc,index^0x80,1);put(m,record+0xd,0xa5,1)
  before=type(s).from_buffer_copy(s);fp=bytes(f);wanted=bytearray(m);regs=list(s.r);regs[2]=44;regs[3]=ptr;regs[4]=record;sr_expected=s.sr
  root={0x8c646884:0x8c65f3f4,0x8c646b36:0x8c65f40e,0x8c646b3c:0x8c65f41e}.get(entry,entry)
  if root==0x8c65f390:regs[0]=record;regs[4]=44*index
  else:
   put(wanted,s.r[15]-4,s.pr)
   if root==0x8c65f3f4:
    put(wanted,s.r[15]-8,token);regs[2]=token;regs[3]=v.load(m,record+0x20);regs[0]=int(token==regs[3]);sr_expected=(s.sr&~1)|regs[0]
   elif root==0x8c65f40e:regs[0]=index^0x80
   else:regs[0]=1;put(wanted,record+0xd,1,1)
  r,path,a,s,f=fast.compare(m,s,f,c);n=3;assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match']
  assert a==wanted and list(s.r)==regs and s.sr==sr_expected and s.pc==s.pr==before.pr;n+=4
  assert bytes(f)==fp and [r[k] for k in ('final_mach','final_macl','final_gbr')]==[c['mach'],index*44,c['gbr']];n+=2
  assert r['boundaries'][0]['target']==hex(entry);n+=1
  note('canonical_status',dict(entry=hex(entry),index=index,mode=mode,sr=hex(sr)),r,path,n)
  if smoke:break
 print('IO STATUS',sum(x['kind']=='canonical_status' for x in records),flush=True)
 for mask,values,(mode,sr) in itertools.product((0,1,2,3,0x80000002,0xffffffff),((0,0,0,0),(255,255,0xab,0x80)),CONFIGS):
  c,m,s,f=setup(ram,META,[OUT,METADATA],mode,sr);m[METADATA&0xffffff:(METADATA&0xffffff)+256]=bytes([0x31])*256;put(m,METADATA,mask);m[OUT&0xffffff:(OUT&0xffffff)+16]=bytes([0xa5])*16;entry=METADATA+4+4*((mask&0xfffffffc).bit_count());m[entry&0xffffff:(entry&0xffffff)+4]=bytes(values)
  before=type(s).from_buffer_copy(s);fp=bytes(f);wanted=bytearray(m)
  for delta,value in ((4,s.r[14]),(8,s.pr),(12,METADATA),(16,s.r[12])):put(wanted,s.r[15]-delta,value)
  if mask&2:
   for off,value in enumerate((values[0]+1,(values[1]+1)<<5,values[2]>>4,values[2]&15,values[3]>>7)):put(wanted,OUT+off*2,value,2)
  r,path,a,s,f=fast.compare(m,s,f,c);n=3;assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match']
  assert a==wanted and s.r[0]==(0 if mask&2 else 0xffffffff) and s.r[14]==before.r[14] and s.r[15]==before.r[15] and s.pc==s.pr==before.pr;n+=5
  assert bytes(f)==fp and r['final_mach']==c['mach'] and r['final_macl']==c['macl'] and r['final_gbr']==c['gbr'];n+=4
  assert sum(b['target']=='0x8c646b42' for b in r['boundaries'])==1;n+=1
  note('canonical_metadata',dict(mask=hex(mask),values=values,mode=mode,sr=hex(sr)),r,path,n)
  if smoke:break
 print('IO METADATA',sum(x['kind']=='canonical_metadata' for x in records),flush=True)
 if smoke:return
 coverage=[]
 complete={READY,0x8c646884,0x8c646b36,0x8c646b3c,0x8c646bbc,0x8c65f3f4,0x8c65f40e,0x8c65f41e,0x8c65f390}
 for body in v.manifest['routines']:
  missing=set(body['reachable_addresses'])-{hex(x) for x in seen};missing_edges=set(map(tuple,body['conditional_edges']))-edges
  if int(body['address'],16) in complete:assert not missing and not missing_edges,(body['address'],missing,missing_edges)
  coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(missing_edges),missing_rows=sorted(missing),missing_outcomes=sorted(missing_edges)))
 files=[Path(__file__),Path(fixtures.__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_channel_io_connection.json',v.effects.DLL]
 report=dict(original_native_comparisons=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Fullnewreadinessbody38rows/sixoutcomes and8canonicalhelperbodies96rows execute. IndependententireRAM/stack/GPR/SR/MACL/FPUoracles forreadiness/status;metadatamask/sparsefield/output+stackfootprints andpreservedstate independent. Explicitcoherentglobalobject/header/statusfixtures,retailmappingindices0/7 anddirectrecordindices0/7/20/31,headerbytes0/1/80/ff,busy0/1/ff/flagsprofiles anduser/FD/FR/PR/SZ modes. Canonicalmetadatamissing/present/sparse/densefield masks;actualoldlocator executes. No hardware/resourceacquisition orparser/callbackresultsubstitution. Canonicalhelperszero duplicatecredit;othernewIO/requestwrappers remainpartial.')
 (P/'analysis/movie_channel_io_units_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('CHANNEL IO UNITS PASS',len(records),checks)
if __name__=='__main__':main()
