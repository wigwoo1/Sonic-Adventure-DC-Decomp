"""Actual active-header/request frontiers with independent CPU/RAM footprints."""
import hashlib,itertools,json,sys
from pathlib import Path
import verify_movie_channel_io_units as unit
v=unit.v;P=v.P;fast=unit.fast;fixtures=unit.fixtures;put=fixtures.put
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 ram=v.seed_ram();records=[];seen=set();edges=set();checks=0;smoke='--smoke' in sys.argv
 def note(data,r,path,n):
  nonlocal checks
  checks+=n;seen.update(path)
  for j,pc in enumerate(path[:-1]):
   if v.ROWS.get(pc,'').startswith(('bt ','bf ')):edges.add((hex(pc),hex(path[j+1])))
  records.append(dict(input=data,status=r['status'],stop_pc=r['stop_pc'],steps=len(path),independent_checks=n))
 for index,busy,(mode,sr) in itertools.product((0,7),(0,0xff),unit.CONFIGS):
  c,m,s,f=unit.setup(ram,0x8c6608a4,[index,unit.OUT,0,0],mode,sr);d=fixtures.configure(m,index,1,busy,2);m[unit.OUT&0xffffff:(unit.OUT&0xffffff)+0x4c]=bytes([0xa5])*0x4c
  before=type(s).from_buffer_copy(s);fp=bytes(f);wanted=bytearray(m);sp=s.r[15];regs=list(s.r)
  for j,reg in enumerate((14,13,12,11,10,9,8)):put(wanted,sp-4*(j+1),s.r[reg])
  put(wanted,sp-32,s.pr);put(wanted,sp-48,d['header']);put(wanted,sp-52,0x8c6608fc);put(wanted,d['status']+0xd,1,1)
  regs[0]=d['header'];regs[1]=int.from_bytes(v.sample(m,0x8c65f3ca,2),'little',signed=True)&0xffffffff;regs[2]=v.load(m,0x8c6468b0);regs[3]=0x8c660494;regs[4]=d['mapped'];regs[5]=2;regs[6]=0
  regs[10]=0x8c646856;regs[11]=0x8c66086a;regs[12]=0x8c646982;regs[13]=d['mapped'];regs[14]=unit.OUT;regs[15]=sp-56
  r,path,a,s,f=fast.compare(m,s,f,c);n=3;assert r['status']==-2 and r['full_state_match'] and r['ordered_boundaries_match']
  assert r['stop_pc']=='0x8c660494' and s.pr==0x8c64698c and s.pc==0x8c660494;n+=3
  assert a==wanted and list(s.r)==regs and s.sr==before.sr&~1;n+=3
  assert bytes(f)==fp and [r[k] for k in ('final_mach','final_macl','final_gbr')]==[c['mach'],44*d['mapped'],c['gbr']];n+=2
  assert [b['target'] for b in r['boundaries']][-2:]==['0x8c646982','0x8c660494'] and v.sample(a,unit.OUT,0x4c)==bytes([0xa5])*0x4c;n+=2
  note(dict(entry='0x8c6608a4',index=index,busy=busy,mode=mode,sr=hex(sr)),r,path,n)
  if smoke:break
 print('IO HEADER PREFIX',sum(r['input']['entry']=='0x8c6608a4' for r in records),flush=True)
 for entry,index,(mode,sr) in itertools.product((0x8c646982,0x8c6469c8),(1,20),unit.CONFIGS):
  c,m,s,f=unit.setup(ram,entry,[index,2,0,0x12345678],mode,sr);put(m,s.r[15],0x13579bdf);before=type(s).from_buffer_copy(s);fp=bytes(f);wanted=bytearray(m);regs=list(s.r);target=0x8c660494 if entry==0x8c646982 else 0x8c6604e6;continuation=0x8c64698c if entry==0x8c646982 else 0x8c6469d6
  put(wanted,s.r[15]-4,s.pr);regs[3]=target;regs[15]-=8
  if entry==0x8c6469c8:put(wanted,s.r[15]-12,0x13579bdf);regs[15]-=4
  r,path,a,s,f=fast.compare(m,s,f,c);n=3;assert r['status']==-2 and r['full_state_match'] and r['ordered_boundaries_match']
  assert r['stop_pc']==hex(target) and s.pc==target and s.pr==continuation;n+=3
  assert a==wanted and list(s.r)==regs and s.sr==before.sr;n+=3
  assert bytes(f)==fp and [r[k] for k in ('final_mach','final_macl','final_gbr')]==[c['mach'],c['macl'],c['gbr']];n+=2
  assert [b['target'] for b in r['boundaries']]==[hex(entry),hex(target)];n+=1
  note(dict(entry=hex(entry),index=index,mode=mode,sr=hex(sr)),r,path,n)
  if smoke:break
 print('IO REQUEST PREFIX',sum(r['input']['entry']!='0x8c6608a4' for r in records),flush=True)
 if smoke:return
 coverage=[]
 for body in v.manifest['routines']:
  missing=set(body['reachable_addresses'])-{hex(x) for x in seen};missing_edges=set(map(tuple,body['conditional_edges']))-edges
  coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(missing_edges),missing_rows=sorted(missing),missing_outcomes=sorted(missing_edges)))
 files=[Path(__file__),Path(unit.__file__),Path(fixtures.__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_channel_io_connection.json',v.effects.DLL]
 report=dict(original_native_comparisons=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Activeheader1/flags2 originalIOpath callsactualrequestmarker646b3c/65f41e andrequestwrapper646982,thenactualmissing660494. Directrequest6469c8reachesmissing6604e6 preservingitsstackargument. IndependententireRAM/stack/fullGPR/SR/PR/MACL/FPU/stop/boundary oracles;inputindices0/7useunmodifiedretailmap andcoherentinputobject;busybyte0/ffpreserved. No requestid/header-parser results supplied. TAS/requestbackend andsuccessfulheaderparsing remainunqualified;exportsare researchonly.')
 (P/'analysis/movie_channel_io_prefixes_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('CHANNEL IO PREFIX PASS',len(records),checks)
if __name__=='__main__':main()
