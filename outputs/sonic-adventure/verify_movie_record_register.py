"""Independent registration/queue/context oracles against actual original code."""
import hashlib,itertools,json
from pathlib import Path
import write_watch_record_register as fast
import movie_record_register_fixtures as fixtures
v=fast.old;P=v.P
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def case(entry,**extra):return dict(entry=entry,mode=extra.pop('mode',0),sr=extra.pop('sr',0x400000a1),kind='normal',mach=0x89abcdef,macl=0x76543210,gbr=0x8ce01234)|extra
def cases():
 for shift in (0,1,7,8,31,32,33,0xffffffff,0xffffffe0,0xffffffdf,0x80000000,0x7fffffff):yield case(v.REGISTER_ROOT,shift=shift)
 for mode,sr in ((1,0x40000001),(4,0x600000f3)):yield case(v.REGISTER_ROOT,mode=mode,sr=sr,shift=1)
 masks=sorted({0,255,0x55,0xaa,0x80000000}|{1<<j for j in range(8)})
 for mode,mask in itertools.product((0,4),masks):yield case(v.REGISTER_CHANNELS,mode=mode,register_arguments=[mask,0,0,0],mask=mask)
 for i,(mode,buffer,callback) in enumerate(itertools.product((15,33,65,122),(0,fixtures.BUFFER),(0,0x8c010f22,0x8c660898))):
  mask=(0,0x55,0xff)[i%3];yield case(v.REGISTER_SETUP,register_arguments=[mode,mask,buffer,callback],input_mode=mode,mask=mask,callback=callback,buffer=buffer)
 for buffer in (0,fixtures.BUFFER):yield case(v.REGISTER_SETUP,register_arguments=[0xffffffff,0,buffer,0],input_mode=0xffffffff,mask=0,callback=0,buffer=buffer)
 for mode,pattern in itertools.product((0,4),(0,0x55,0xff)):yield case(v.REGISTER_POOL,mode=mode,pool_pattern=pattern)
 for mode,free,reject,size in itertools.product((0,4),(0,1,7,31),(False,True),(0,4,17)):
  yield case(v.REGISTER_ENQUEUE,mode=mode,free=free,reject=reject,size=size,register_arguments=[0x8c010f22,fixtures.PAYLOAD,size,0])
 for entry,selector,sr in itertools.product((v.REGISTER_CALLBACK,v.REGISTER_DISABLE),(0,7,31,32,0xffffffff),(0x40000000,0x600003f3)):
  yield case(entry,selector=selector,sr=sr)
 for mode,payload in itertools.product(range(8),(0,0xffffffff,0x7fa12345)):
  yield case(v.REGISTER_CONTEXT,mode=mode,payload=payload,register_arguments=[fixtures.CONTEXT,0x13579bdf,0x2468ace0,0x89abcdef])
 for entry,sr,pc in ((v.REGISTER_CONTEXT,0x40008001,0x8c11098a),(v.REGISTER_SETUP,0xa1,0x8c602b6e),(v.REGISTER_DISABLE,0x21,0x8c602bf4)):
  yield case(entry,sr=sr,kind='fault',fault_pc=pc,register_arguments=[fixtures.CONTEXT,0,0,0])

def check_context(before,fp,c,a,s):
 expected={0:before.r[15],4:before.r[14],8:before.pr}
 expected.update({12+j*4:before.r[13-j] for j in range(10)})
 expected.update({0x34+j*4:before.r[3-j] for j in range(4)})
 expected.update({0x44:c['mach'],0x48:c['macl'],0x4c:c['gbr'],0x94:fp.fpscr,0x90:c.get('fpul',getattr(fp,'fpul',0))})
 base=fixtures.CONTEXT;wanted=bytearray([0x5a])*512;origin=base-128
 def put(address,value):at=address-origin;wanted[at:at+4]=(value&0xffffffff).to_bytes(4,'little')
 for offset,value in expected.items():put(base+offset,value)
 cursor=base+0x8c;bank=(fp.fpscr>>21)&1;wide=bool(fp.fpscr&0x100000)
 for reg in range(15,-1,-1):
  if reg!=15:cursor-=8 if wide else 4
  chosen=bank^(reg&1) if wide else bank;first=reg&~1 if wide else reg
  put(cursor,fp.banks[chosen][first])
  if wide:put(cursor+4,fp.banks[chosen][first+1])
 assert fixtures.sample(a,origin,512)==wanted
 assert s.r[0]==0 and s.r[4]==fixtures.read(a,cursor-0x20) and s.sr==before.sr
 return 3

def main():
 ram=v.seed_ram();records=[];seen=set();edges=set();checks=0
 for i,data in enumerate(cases(),1):
  c=v.Input(data);m,s,f=v.prepare(ram,c);fixtures.configure(m,c);s.sr=c['sr']
  if c['entry']==v.REGISTER_POOL:fixtures.fill(m,fixtures.read(m,0x8c668c44),c['pool_pattern'],0x134a0+16)
  if c['entry']==v.REGISTER_CONTEXT:
   for bank in range(2):
    for reg in range(16):f.banks[bank][reg]=(c.get('payload',0)^((bank+1)*0x11111111)^((reg+1)*0x01020304))&0xffffffff
  before=v.effects.State.from_buffer_copy(s);fp=v.effects.Fpu.from_buffer_copy(f) if hasattr(v.effects,'Fpu') else type(f).from_buffer_copy(f)
  r,path,a,s,f=fast.compare(m,s,f,c);seen.update(path);n=0
  for j,pc in enumerate(path):
   instruction=v.ROWS.get(pc,'');delta=2 if instruction.startswith(('bt/s ','bf/s ')) else 1
   if instruction.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+delta<len(path):edges.add((pc,path[j+delta]))
  expected_status=-4 if c['kind']=='fault' else 0
  assert r['status']==expected_status and r['full_state_match'] and r['ordered_boundaries_match'],(i,c,r['status'],r['stop_pc']);n+=3
  assert bytes(f)==bytes(fp);n+=1
  if c['kind']=='fault':
   assert int(r['stop_pc'],16)==c['fault_pc'];n+=1
  else:
   assert s.pc==s.pr==before.pr and list(s.r[8:])==list(before.r[8:]);n+=3
   entry=c['entry']
   if entry in (v.REGISTER_ROOT,v.REGISTER_CHANNELS,v.REGISTER_POOL):
    mask=fixtures.shifted_one(c.get('shift',0)) if entry==v.REGISTER_ROOT else c.get('mask',0)
    if entry==v.REGISTER_POOL:
     pool=fixtures.read(a,0x8c668c44);assert s.r[0]==pool and fixtures.read(a,fixtures.read(a,0x8c660880))==pool and fixtures.read(a,pool)==1;n+=3
    else:n+=fixtures.channels(a,mask,callback_fields=entry==v.REGISTER_ROOT)
   if entry==v.REGISTER_ROOT:
    assert fixtures.sample(a,fixtures.read(a,0x8c010c00),0x260)==bytes(0x260) and s.r[0]==fixtures.read(a,0x8c010c1c);n+=2
    for literal,value in ((0x8c010c18,0),(0x8c010c1c,0),(0x8c010c20,1)):assert fixtures.read(a,fixtures.read(a,literal))==value;n+=1
    call=next(b for b in r['boundaries'] if b['target']=='0x8c602b68');assert call['arguments']==[15,mask,fixtures.read(m,0x8c010c0c),0x8c010f22];n+=1
   if entry==v.REGISTER_SETUP:
    assert s.r[0]==0 and (s.sr&0xf0)==(before.sr&0xf0);n+=2
    n+=fixtures.channels(a,c['mask'],callback_fields=c['callback']==0x8c010f22)
    callbacks=[b for b in r['boundaries'] if b['target']==hex(c['callback']) and b['pr']=='0x8c602bce'] if c['callback'] else []
    assert len(callbacks)==int(bool(c['callback']));n+=1
   if entry in (v.REGISTER_CALLBACK,v.REGISTER_DISABLE):
    expected=(before.sr&0x700083f2)|(1 if entry==v.REGISTER_CALLBACK or c['selector']>=32 else 0)
    assert s.sr==expected and s.r[0]==(0x78 if entry==v.REGISTER_CALLBACK else expected);n+=2
    if entry==v.REGISTER_CALLBACK:n+=fixtures.callbacks(a,fixtures.read(a,0x8c010f58),fixtures.read(a,0x8c010f60))
   if entry==v.REGISTER_ENQUEUE:
    assert s.r[0]==(0xffffffff if c['reject'] else c['free']);n+=1
    if not c['reject']:
     job=fixtures.OWNER+0x210+c['free']*0x124;words=c['size']//4
     assert [fixtures.read(a,job+j*4) for j in range(6)]==[fixtures.PAYLOAD,words,0,1,0,0xabcdef01];n+=1
     assert fixtures.read(a,job+0x18)==(fixtures.PAYLOAD+(words-1)*4)&0xffffffff and fixtures.read(a,job+0x20)==0x8c010f22;n+=2
   if entry==v.REGISTER_CONTEXT:n+=check_context(before,fp,c,a,s)
  checks+=n;records.append(dict(input=dict(c),status=r['status'],stop_pc=r['stop_pc'],r0=s.r[0],steps=len(path),independent_checks=n))
  print(i,hex(c['entry']),c['kind'],r['status'],hex(s.r[0]),n,flush=True)
 coverage=[]
 for body in v.manifest['routines']:
  wanted=set(body['reachable_addresses']);wanted_edges=set(map(tuple,body['conditional_edges']));actual={hex(a) for a in seen};actual_edges={(hex(a),hex(b)) for a,b in edges}
  coverage.append(dict(address=body['address'],static_rows=len(wanted),executed_rows=len(wanted&actual),static_outcomes=len(wanted_edges),executed_outcomes=len(wanted_edges&actual_edges),missing_rows=sorted(wanted-actual),missing_outcomes=sorted(wanted_edges-actual_edges)))
 files=[Path(__file__),Path(fixtures.__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_record_register_connection.json',P/'analysis/record_context_register_primary_evidence.json',v.effects.DLL]
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Original registration/callback/pool/context instructions execute with real helpers, no substituted results. Independent buffer/channel/queue/callback/SR/context expectations, complete machine and ordered-boundary comparisons. Fixed coherent caller domains/private stack; physical exception delivery/timing and full game remain unfinished. Qualification is separate; two impossible own outcomes require checked callee proofs.')
 (P/'analysis/movie_record_register_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('REGISTER DIRECT',len(records),checks,coverage)
if __name__=='__main__':main()
