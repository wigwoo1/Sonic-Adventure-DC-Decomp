"""Original inactive returns and explicit active decoder dependency boundaries."""
import hashlib,itertools,json,sys
from pathlib import Path
import verify_movie_channel_decoder_leaves as leaf
v=leaf.v;P=v.P;fast=leaf.fast;put=leaf.put
ROOT1,ROOT18,MODE1,READ,PROCESS,FILL=0x8c6482a0,0x8c64839a,0x8c648470,0x8c64753c,0x8c64b1f0,0x8c660a7e
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def prepare(ram,entry,index,mode,global_mode=0,subtype=0,callback=0,active=1,flag_a=0):
 c,m,s,f=leaf.setup(ram,entry,index,mode,0x400000a1);base=v.load(m,0x8c648368);descriptor=base+index*0x148;record=v.load(m,0x8c647c4c)+index*0x80
 m[descriptor&0xffffff:(descriptor&0xffffff)+0x148]=bytes([0x55])*0x148
 put(m,descriptor+2,active,1);put(m,descriptor+4,subtype);put(m,descriptor+0x1c,leaf.CONTEXT);put(m,descriptor+0xa,flag_a,1)
 put(m,record+0x78,callback);put(m,v.load(m,0x8c648370),leaf.OBJECT);put(m,leaf.OBJECT+8,global_mode)
 m[leaf.CONTEXT&0xffffff:(leaf.CONTEXT&0xffffff)+0x4c]=bytes(0x4c)
 return c,m,s,f,descriptor,record
def main():
 ram=v.seed_ram();records=[];seen=set();edges=set();checks=0;smoke='--smoke' in sys.argv
 def note(kind,case,r,path,n):
  nonlocal checks
  checks+=n;seen.update(path)
  for j,pc in enumerate(path[:-1]):
   if v.ROWS.get(pc,'').startswith(('bt ','bf ')):edges.add((hex(pc),hex(path[j+1])))
  records.append(dict(kind=kind,input=case,status=r['status'],stop_pc=r['stop_pc'],steps=len(path),independent_checks=n))
 inactive=list(itertools.product((ROOT1,MODE1),(0,7),(0,4),(0,1)))
 for entry,index,mode,global_mode in inactive:
  c,m,s,f,descriptor,record=prepare(ram,entry,index,mode,global_mode,active=0);before=type(s).from_buffer_copy(s);fp=bytes(f);wanted=bytearray(m);sp=s.r[15];regs=list(s.r);base=v.load(m,0x8c648368)
  if entry==ROOT1:
   for offset,value in ((4,s.r[14]),(8,s.r[13]),(12,s.r[12]),(16,s.r[11]),(20,s.pr)):put(wanted,sp-offset,value)
   regs[2]=v.load(m,0x8c648370)
  if entry==MODE1 or global_mode==1:
   for offset,value in ((4,s.r[14]),(8,s.r[13]),(12,s.pr)):put(wanted,sp-offset,value)
  regs[0]=0xffffff01;regs[3]=base
  r,path,a,s,f=fast.compare(m,s,f,c);n=3;assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match']
  assert a==wanted and list(s.r)==regs and s.sr==before.sr|1 and s.pc==s.pr==before.pr;n+=4
  assert bytes(f)==fp and [r[k] for k in ('final_mach','final_macl','final_gbr')]==[c['mach'],index*0x148,c['gbr']];n+=2
  assert [b['target'] for b in r['boundaries']]==[hex(entry)]+([hex(MODE1)] if entry==ROOT1 and global_mode==1 else []);n+=1
  note('inactive_return',dict(entry=hex(entry),index=index,mode=mode,global_mode=global_mode),r,path,n)
  if smoke:break
 print('DECODER INACTIVE',sum(r['kind']=='inactive_return' for r in records),flush=True)
 configs=[]
 for entry,index,mode,subtype,callback in itertools.product((ROOT1,ROOT18,MODE1),(0,7),(0,4),(0,3),(0,0x8c05180c,0x8cfef000)):
  for global_mode in ((0,1) if entry==ROOT1 else (0,)):configs.append(dict(entry=entry,index=index,mode=mode,subtype=subtype,callback=callback,global_mode=global_mode))
 for entry,index,mode in itertools.product((READ,FILL),(0,7),(0,4)):configs.append(dict(entry=entry,index=index,mode=mode))
 for index,mode,flag_a in itertools.product((0,7),(0,4),(0,1)):configs.append(dict(entry=PROCESS,index=index,mode=mode,flag_a=flag_a))
 for data in configs:
  entry=data['entry'];index=data['index'];c,m,s,f,descriptor,record=prepare(ram,**data);before=type(s).from_buffer_copy(s)
  callback=data.get('callback',0);target=0x8cfef000 if callback==0x8cfef000 and entry in (ROOT1,ROOT18,MODE1) else 0x8c660dec if entry==FILL else 0x8c64a4b2 if entry==PROCESS and data.get('flag_a') else 0x8c64ae32 if entry in (MODE1,PROCESS) or entry==ROOT1 and data.get('global_mode')==1 else 0x8c6608a4
  r,path,a,s,f=fast.compare(m,s,f,c);n=3;assert r['status']==-2 and r['full_state_match'] and r['ordered_boundaries_match']
  assert r['stop_pc']==hex(target) and r['boundaries'][-1]['target']==hex(target);n+=2
  if entry in (ROOT1,ROOT18,MODE1):
   parameter_literal=0x8c64849c if entry==ROOT18 else 0x8c648374
   assert v.load(a,descriptor+0x30)==2 and v.load(a,descriptor+0x34)==v.load(m,v.load(m,parameter_literal)+data['subtype']*80);n+=2
   assert v.sample(a,descriptor+2,1)==b'\x01';n+=1
   if entry==ROOT18:assert v.load(a,descriptor+0x1c)==v.load(m,0x8c648498)+index*0x4c;n+=1
   assert sum(b['target']=='0x8c647bde' for b in r['boundaries'])==1;n+=1
   if callback:assert any(b['target']==hex(callback) for b in r['boundaries']);n+=1
  if entry==PROCESS or entry in (MODE1,ROOT1) and target in (0x8c64ae32,0x8c64a4b2):
   assert v.load(a,v.load(m,0x8c64b2e8)+index*4)==0;n+=1
  assert r['boundaries'][0]['target']==hex(entry) and s.pr!=before.pr;n+=2
  note('active_prefix',data,r,path,n)
  if smoke:break
 print('DECODER PREFIX',sum(r['kind']=='active_prefix' for r in records),flush=True)
 if smoke:return
 coverage=[]
 for body in v.manifest['routines']:
  missing=set(body['reachable_addresses'])-{hex(x) for x in seen};miss_edges=set(map(tuple,body['conditional_edges']))-edges
  coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(miss_edges),missing_rows=sorted(missing),missing_outcomes=sorted(miss_edges)))
 files=[Path(__file__),Path(leaf.__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_channel_decoder_connection.json',v.effects.DLL]
 report=dict(original_native_comparisons=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Direct original inactive ROOT1/mode1path returns usezeroactiveflag andcoherentglobalcontext. Independent entireRAM/stack/fullGPR/SR/MACL/FPU/return expectations. Activeprefixes useexplicitindexed descriptors/subtypes andnull/realNOP/actualunknowncallbacks;realcanonical647bde executes. Newstate/callback/loop/pump bodies stopatactualmissing6608a4,660dec,64ae32,64a4b2;no callee results replaced. Prefixstate writes checkedindependently;fullnewdecoder behavior remainspartial. No original taskadmission/controller/resource acquisition claim for directfixtures.')
 (P/'analysis/movie_channel_decoder_paths_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('CHANNEL DECODER PATHS PASS',len(records),checks)
if __name__=='__main__':main()
