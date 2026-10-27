"""Independent descriptor validation/cleanup and canonical callback oracles."""
import hashlib,itertools,json,sys
from pathlib import Path
import write_watch_channel_decoder as fast
v=fast.old;P=v.P;CLEAN=0x8c64850a;VALIDATE=0x8c64b4e4;CALLBACK=0x8c647bde
DESCRIPTOR=0x8cd60000;CONTEXT=0x8cd60400;OBJECT=0x8cd68000
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def put(m,a,value,n=4):m[a&0xffffff:(a&0xffffff)+n]=(value&((1<<(8*n))-1)).to_bytes(n,'little')
def signed(vv,bits=32):return vv-(1<<bits) if vv&(1<<(bits-1)) else vv
def configs():return ((0,0),(1,0x400000a1),(4,0x600000a1),(7,0x400080a1))
def setup(ram,entry,arg,mode,sr):
 c=v.Input(dict(entry=entry,mode=mode,sr=sr,mach=0x89abcdef,macl=0x76543210,gbr=0x8ce01234,channel_decoder_arguments=[arg,0x12345678,0x89abcdef,0x13579bdf]));m,s,f=v.prepare(ram,c);s.sr=sr;f.fpscr=0x40001|(mode<<19)
 for bank in range(2):
  for reg in range(16):f.banks[bank][reg]=(0x7fa12345^((bank+1)*0x11111111)^((reg+1)*0x01020304))&0xffffffff
 return c,m,s,f
def validation_cases():
 for kind in (255,511,1023,2047):
  yield dict(name='accepted',kind=kind)
  yield dict(name='accepted-upper-bounds',kind=kind,c=kind-1,d=kind-1,e=kind,f=kind,need1=32,need2=32)
  yield dict(name='mode-skips-capacity',kind=kind,global_mode=1,cap1=-1,cap2=-1,need1=0x7fffffff,need2=0x7fffffff)
 for name,update in [('kind-zero',dict(kind=0)),('kind-negative',dict(kind=-1)),('kind-too-large',dict(kind=2048)),('mismatch',dict(match=511)),('negative-c',dict(c=-1)),('c-at-end',dict(c=255)),('negative-d',dict(d=-1)),('d-at-end',dict(d=255)),('negative-e',dict(e=-1)),('e-past-end',dict(e=256)),('negative-f',dict(f=-1)),('f-past-end',dict(f=256)),('capacity1-too-small',dict(cap1=31,need1=32)),('capacity2-too-small',dict(cap2=31,need2=32)),('signed-capacity1',dict(cap1=-1,need1=0)),('signed-capacity2',dict(cap2=-1,need2=0)),('unknown-kind',dict(kind=42))]:yield dict(name=name,**update)
def main():
 ram=v.seed_ram();records=[];checks=0;seen=set();edges=set();smoke='--smoke' in sys.argv
 def note(kind,data,r,path,n):
  nonlocal checks
  seen.update(path);checks+=n
  for j,pc in enumerate(path[:-1]):
   if v.ROWS.get(pc,'').startswith(('bt ','bf ')):edges.add((hex(pc),hex(path[j+1])))
  records.append(dict(kind=kind,input=data,status=r['status'],stop_pc=r['stop_pc'],steps=len(path),independent_checks=n))
 for index,flag,(mode,sr) in itertools.product((0,1,7,0x80000000,0xffffffff),(0,1,0x7f,0x80,0xff),configs()):
  c,m,s,f=setup(ram,CLEAN,index,mode,sr);base=v.load(m,0x8c6485c8);stride=int.from_bytes(v.sample(m,0x8c6485ae,2),'little',signed=True);product=(index*stride)&0xffffffff;address=(base+product)&0xffffffff
  m[address&0xffffff:(address&0xffffff)+0x148]=bytes([0xa5])*0x148;put(m,address+2,flag,1)
  before=type(s).from_buffer_copy(s);fp=bytes(f);wanted=bytearray(m);regs=list(s.r);regs[1]=address;regs[3]=base
  if flag:
   regs[0]=regs[4]=0
   for off in (8,9,10,11,12):put(wanted,address+off,0,1)
   put(wanted,address+4,0)
  else:regs[0]=int.from_bytes(v.sample(m,0x8c6485b0,2),'little',signed=True)&0xffffffff
  r,path,a,s,f=fast.compare(m,s,f,c);n=3;assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match']
  assert a==wanted and list(s.r)==regs and s.sr==(before.sr&~1)|int(not flag) and s.pc==s.pr==before.pr;n+=4
  assert bytes(f)==fp and [r[k] for k in ('final_mach','final_macl','final_gbr')]==[c['mach'],product,c['gbr']];n+=2
  assert [b['target'] for b in r['boundaries']]==[hex(CLEAN)];n+=1
  note('cleanup',dict(index=index,flag=flag,mode=mode,sr=hex(sr)),r,path,n)
  if smoke:break
 print('DECODER CLEANUP',sum(x['kind']=='cleanup' for x in records),flush=True)
 for data,(mode,sr) in itertools.product(list(validation_cases()),configs()):
  c,m,s,f=setup(ram,VALIDATE,DESCRIPTOR,mode,sr);m[DESCRIPTOR&0xffffff:(DESCRIPTOR&0xffffff)+0x148]=bytes([0xa5])*0x148;m[CONTEXT&0xffffff:(CONTEXT&0xffffff)+0x4c]=bytes(0x4c)
  k=data.get('kind',255);values={0:k,8:data.get('match',k),0xc:data.get('c',0),0x14:data.get('d',0),0x34:data.get('e',0),0x38:data.get('f',0),0x10:data.get('need1',0),0x18:data.get('need2',0)}
  for off,value in values.items():put(m,CONTEXT+off,value)
  cap1=data.get('cap1',32);cap2=data.get('cap2',32);put(m,DESCRIPTOR+0x10,cap1,2);put(m,DESCRIPTOR+0x12,cap2,2);put(m,DESCRIPTOR+0x1c,CONTEXT)
  global_mode=data.get('global_mode',0);put(m,v.load(m,0x8c64b5cc),OBJECT);put(m,OBJECT+8,global_mode)
  good=(1<=k<=2047 and values[8]==k and 0<=values[0xc]<k and 0<=values[0x14]<k and 0<=values[0x34]<=k and 0<=values[0x38]<=k and (global_mode!=0 or (cap1>=values[0x10] and cap2>=values[0x18])) and k in (255,511,1023,2047))
  wanted=bytearray(m)
  if good:put(wanted,DESCRIPTOR+4,(255,511,1023,2047).index(k))
  before=type(s).from_buffer_copy(s);fp=bytes(f);r,path,a,s,f=fast.compare(m,s,f,c);n=3;assert r['status']==0 and r['full_state_match'] and r['ordered_boundaries_match']
  assert a==wanted and s.r[0]==(0 if good else 0xffffffff);n+=2
  assert all(s.r[j]==before.r[j] for j in range(16) if j not in (0,1,2,3,5,6,7)) and s.pc==s.pr==before.pr;n+=2
  assert bytes(f)==fp and [r[k] for k in ('final_mach','final_macl','final_gbr')]==[c['mach'],c['macl'],c['gbr']];n+=2
  assert [b['target'] for b in r['boundaries']]==[hex(VALIDATE)];n+=1
  note('validation',data|dict(mode=mode,sr=hex(sr),accepted=good),r,path,n)
  if smoke:break
 print('DECODER VALIDATION',sum(x['kind']=='validation' for x in records),flush=True)
 for index,target,(mode,sr) in itertools.product((0,7),(0,0x8c05180c,0x8cfef000),configs()):
  c,m,s,f=setup(ram,CALLBACK,index,mode,sr);base=v.load(m,0x8c647c4c);record=(base+index*0x80)&0xffffffff;put(m,record+0x78,target)
  before=type(s).from_buffer_copy(s);fp=bytes(f);wanted=bytearray(m);put(wanted,s.r[15]-4,s.r[14]);regs=list(s.r);regs[0]=0x78;regs[2]=target;regs[3]=base
  r,path,a,s,f=fast.compare(m,s,f,c);status=-2 if target==0x8cfef000 else 0;n=3;assert r['status']==status and r['full_state_match'] and r['ordered_boundaries_match']
  assert a==wanted and list(s.r)==regs and s.sr==(before.sr&~1)|int(not target) and s.pr==before.pr;n+=4
  assert s.pc==(target if status else before.pr) and bytes(f)==fp and [r[k] for k in ('final_mach','final_macl','final_gbr')]==[c['mach'],c['macl'],c['gbr']];n+=3
  assert [b['target'] for b in r['boundaries']]==[hex(CALLBACK)]+([hex(target)] if target else []);n+=1
  note('canonical_callback',dict(index=index,target=hex(target),mode=mode,sr=hex(sr)),r,path,n)
  if smoke:break
 print('DECODER CALLBACK',sum(x['kind']=='canonical_callback' for x in records),flush=True)
 if smoke:return
 coverage=[]
 for body in v.manifest['routines']:
  missing=set(body['reachable_addresses'])-{hex(x) for x in seen};miss_edges=set(map(tuple,body['conditional_edges']))-edges
  if int(body['address'],16) in (CLEAN,VALIDATE,CALLBACK):assert not missing and not miss_edges,(body['address'],missing,miss_edges)
  coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(body['conditional_edges'])-len(miss_edges),missing_rows=sorted(missing),missing_outcomes=sorted(miss_edges)))
 files=[Path(__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_channel_decoder_connection.json',v.effects.DLL]
 report=dict(original_native_comparisons=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Direct cleanup/descriptorvalidator pluscanonical647bde callbackdomains:coherentRAM/stack/indexfixtures,allnewleafinstructions/outcomes executed. Independent entireRAM footprint/cleanupfullGPR/SR/MACL/originalreturnoracles;validationkind/count/capacity/signedbounds/errorclassification andpreservedstate checks. Null/original05180cNOP/actualunknowncallbacks;no callbackresult supplied. Cleanupwrapindices testmodulo32pointerarithmetic,notrealresourceadmission. FD/SR/FR/PR/SZ profilesretainallFPUstate. Otherdecoder entries remainpartial;canonicalcallbackzero duplicatecredit.')
 (P/'analysis/movie_channel_decoder_leaves_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('CHANNEL DECODER LEAVES PASS',len(records),checks)
if __name__=='__main__':main()
