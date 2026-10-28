"""Original/native collision inverse-trigonometry call paths and finite sanity checks."""
import collections,csv,hashlib,itertools,json,math,struct,sys
from pathlib import Path
import write_watch_collision_angle as fast
v=fast.old;P=v.P;ASIN,ATAN2,QUADRANT=0x8c10d058,0x8c10d0f8,0x8c10e6a0
POLYNOMIAL,REDUCE,OUTPUT=0x8c10fb94,0x8c10fbb8,0x8cc00000

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def bits(x):return struct.unpack('<I',struct.pack('<f',x))[0]
def number(x):return struct.unpack('<f',struct.pack('<I',x))[0]
def configurations():
 values=[0,0x80000000]+[bits(x) for x in (-2,-1,-.75,-.5,-.125,.125,.5,.75,1,2)]+[0x7f800000,0xff800000,0x7fbfffff,0xffbfffff,0x7fc00001,1,0x80000001,0x7f7fffff]
 pairs=[(bits(y),bits(x)) for y,x in itertools.product((-3,-1,1,3),(-3,-1,1,3))]+[(0,bits(1)),(0x80000000,bits(1)),(0,bits(-1)),(0x80000000,bits(-1)),(bits(1),0),(bits(-1),0),(0,0),(0x80000000,0x80000000),(0x7f800000,bits(1)),(bits(1),0x7f800000),(0x7fbfffff,bits(1)),(bits(1),0x7fbfffff),(0x7fc00001,bits(-1)),(1,bits(1)),(bits(1),1)]
 for bank,rm,dn in itertools.product(range(2),range(2),range(2)):
  for x in values:yield dict(kind='asin',entry=ASIN,fpscr=(bank<<21)|(dn<<18)|rm,y=x,x=bits(3))
  for entry,(y,x) in itertools.product((ATAN2,QUADRANT),pairs):yield dict(kind='atan2',entry=entry,fpscr=(bank<<21)|(dn<<18)|rm,y=y,x=x)
 for entry,bank,sz,pr,dn,rm in itertools.product((ASIN,ATAN2,QUADRANT),range(2),range(2),range(2),range(2),range(2)):
  yield dict(kind='mode',entry=entry,fpscr=(bank<<21)|(sz<<20)|(pr<<19)|(dn<<18)|rm|0x3f07c,y=bits(.5),x=bits(-1))
 for entry,bank,en,y,x in itertools.product((ASIN,ATAN2,QUADRANT),range(2),(0x80,0x800,0xf80), (bits(2),0x7fc00001), (0,bits(1))):
  yield dict(kind='enabled_exception',entry=entry,fpscr=(bank<<21)|0x40000|en,y=y,x=x)
 for entry,bank,guard in itertools.product((ASIN,ATAN2,QUADRANT),range(2),('bad_stack','edge_stack')):
  yield dict(kind='guard',entry=entry,fpscr=(bank<<21)|0x40000,y=bits(.5),x=bits(1),guard=guard)
 for entry,bank,rm,view in itertools.product((ASIN,ATAN2,QUADRANT),range(2),range(2),('p0','p1','p2')):
  yield dict(kind='view',entry=entry,fpscr=(bank<<21)|0x40000|rm,y=bits(.5),x=bits(-1),view=view)
 for entry,rm in itertools.product((ASIN,ATAN2,QUADRANT),(2,3)):
  yield dict(kind='reserved_rm',entry=entry,fpscr=0x40000|rm,y=bits(.5),x=bits(1))
 for bank,rm,y in itertools.product(range(2),range(2),(-.125,-.0625,0,.0625,.125)):
  yield dict(kind='polynomial',entry=POLYNOMIAL,fpscr=(bank<<21)|0x40000|rm,y=bits(y),x=bits(1))
 reduce_pairs=[(bits(y),bits(x)) for y,x in itertools.product((-4,-1.5,-.5,0,.5,1.5,4,2147483648),(-2,-1,1,2))]+[(0x7f800000,bits(1)),(bits(1),0x7f800000),(0x7fbfffff,bits(1)),(bits(1),0x7fbfffff),(bits(1),0)]
 for bank,rm,(y,x) in itertools.product(range(2),range(2),reduce_pairs):
  yield dict(kind='range_reduce',entry=REDUCE,fpscr=(bank<<21)|0x40000|rm,y=y,x=x)

def main():
 configs=[dict(c,case_id=i) for i,c in enumerate(configurations())];shard=int(sys.argv[sys.argv.index('--shard')+1]) if '--shard' in sys.argv else None
 if shard is not None:assert shard in range(3);configs=[c for c in configs if c['case_id']%3==shard]
 case_id=int(sys.argv[sys.argv.index('--case-id')+1]) if '--case-id' in sys.argv else None
 if case_id is not None:configs=[c for c in configs if c['case_id']==case_id];assert len(configs)==1
 smoke='--smoke' in sys.argv
 if smoke:
  configs=[next(c for c in configs if c['entry']==e and c['fpscr']==0x40000 and c['y']==bits(.5) and (e==ASIN or c['x']==bits(-1))) for e in (ASIN,ATAN2,QUADRANT)]
 ram=v.seed_ram();records=[];seen=set();edges=set();checks=oracles=0;status=collections.Counter();targets=collections.Counter()
 # Original wrapper error-pointer literals; these remain unmodified original input.
 errno={v.load(ram,0x8c10d0a4),v.load(ram,0x8c10d144)}
 for c in configs:
  count=0
  def require(ok,label):
   nonlocal checks,count
   assert ok,(c,label);checks+=1;count+=1
  entry=c['entry'];fpscr=c['fpscr'];cfg=v.Input(dict(entry=entry,mode=(fpscr>>19)&7,probe=True,sr=0x400000a0|(c['case_id']&1),mach=0x89abcdef,macl=0x76543210,gbr=0x8ce01234))
  m,s,f=v.prepare(ram,cfg)
  for j in range(16):s.r[j]=(0x12345678^((j+1)*0x01030507))&0xffffffff
  s.r[15]=0x8cf00000;s.pr=0x8cfed000;s.pc=entry;s.sr=cfg['sr'];bank=(fpscr>>21)&1
  for b in range(2):
   for j in range(16):f.banks[b][j]=bits(.5+(b*16+j)/16)
  f.banks[bank][4]=c['y'];f.banks[bank][5]=c['x'];f.fpscr=fpscr;f.fpul=0xc0ffee42;cfg['fpul']=f.fpul
  s.r[4]=OUTPUT
  for a in errno:m[a&0xffffff:(a&0xffffff)+4]=(0xa5a5a5a5).to_bytes(4,'little')
  view={'p0':0,'p1':0x80000000,'p2':0xa0000000}[c.get('view','p1')];s.r[15]=(s.r[15]&0x1fffffff)|view
  if c.get('guard'):s.r[15]=0x8b000100 if c['guard']=='bad_stack' else 0x8c000004
  initial_s=type(s).from_buffer_copy(s);initial_f=type(f).from_buffer_copy(f);before=bytes(m)
  r,path,a,s,f=fast.compare(m,s,f,cfg)
  if r['status'] not in (0,1,-1,-3):print('UNEXPECTED ORIGINAL/NATIVE STOP',c,{k:r[k] for k in ('status','stop_pc','full_state_match','boundaries')},flush=True)
  require(r['full_state_match'],'entire original/native CPU/FPU/RAM/device state');require(r['ordered_boundaries_match'],'ordered original/native boundary state');require(r['status'] in (0,1,-1,-3),'no unconnected/substituted game service')
  require(all(int(b['target'],16) in v.effects.CONNECTED for b in r['boundaries']),'all observed calls use connected original implementations')
  require((r['final_mach'],r['final_gbr'])==(cfg['mach'],cfg['gbr']),'untouched MACH/GBR')
  targets.update(b['target'] for b in r['boundaries']);wanted=bytearray(before)
  sp=initial_s.r[15]&0xffffff
  if not c.get('guard'):
   wanted[sp-0x200:sp]=a[sp-0x200:sp]
   for err in errno:wanted[err&0xffffff:(err&0xffffff)+4]=a[err&0xffffff:(err&0xffffff)+4]
   if entry==REDUCE:wanted[OUTPUT&0xffffff:(OUTPUT&0xffffff)+4]=a[OUTPUT&0xffffff:(OUTPUT&0xffffff)+4]
   require(a==wanted,'only original stack/errno write footprints')
  if r['status']==0:
   require(s.r[15]==initial_s.r[15] and s.pr==initial_s.pr,'original SP/PR restored');require(list(s.r[8:15])==list(initial_s.r[8:15]),'callee-saved integer registers')
   if not fpscr&0x100000:require(list(f.banks[1-bank])==list(initial_f.banks[1-bank]),'inactive physical FP bank preserved')
   y,x=number(c['y']),number(c['x']);normal=lambda u:(u&0x7f800000)!=0 and (u&0x7f800000)!=0x7f800000
   if not fpscr&0x180000 and math.isfinite(y) and (entry==ASIN and abs(y)<=1 or entry in (ATAN2,QUADRANT) and normal(c['y']) and normal(c['x']) or entry==POLYNOMIAL and abs(y)<=.125):
    expected=math.asin(y) if entry==ASIN else math.atan(y) if entry==POLYNOMIAL else math.atan2(y,x);actual=number(f.banks[bank][0])
    require(math.isfinite(actual) and abs(actual-expected)<=2e-6,'independent finite host transcendental sanity,absolute tolerance2e-6');oracles+=1
  seen.update(path)
  for i,pc in enumerate(path):
   w=int.from_bytes(ram[pc&0xffffff:(pc&0xffffff)+2],'little')
   if w>>8 in (0x89,0x8b,0x8d,0x8f):
    j=i+(2 if w>>8 in (0x8d,0x8f) else 1)
    if j<len(path):edges.add((pc,path[j]))
  status[r['status']]+=1;records.append(dict(configuration=c,status=r['status'],stop_pc=r['stop_pc'],steps=len(path),independent_checks=count,full_state_match=r['full_state_match'],ordered_boundaries_match=r['ordered_boundaries_match']))
  if len(records)%24==0:print('COLLISION ANGLE',shard,len(records),dict(status),flush=True)
 coverage=[]
 for body in v.ANALYSIS_ROUTINES:
  missing=set(body['reachable_addresses'])-{hex(pc) for pc in seen};wanted={tuple(int(x,16) for x in e) for e in body['conditional_edges']};miss_edges=wanted-edges
  coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=body['instruction_rows']-len(missing),executed_outcomes=len(wanted&edges),missing_rows=sorted(missing),missing_outcomes=[[hex(x),hex(y)] for x,y in sorted(miss_edges)]))
 files=[Path(__file__),P/'collision_angle_reference.py',P/'write_watch_collision_angle.py',v.MP,P/'analysis/collision_angle_connection.json',v.effects.DLL,P/'source/sa_draw_frame_dependencies.c',P/'source/sh4_compare_bits.h']
 report=dict(existing_numeric_probe_mode=True,original_native_comparisons=len(records),independent_checks=checks,finite_sanity_cases=oracles,status_counts=dict(status),target_invocations=dict(targets),cases=records,coverage_by_routine=coverage,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Five original inverse-trigonometry entry bodies:three wrappers/quadrant path,a polynomial,and canonical range-reduction adapter(no duplicatecredit),with actual connected original callees;fullCPU/FPU/RAM/ordered boundaries,finite independent host math sanity tolerance2e-6,stack/errno/output footprint. Existing numeric stop/trap/guard policies retained. SR.FD clear ABI;physical hardware/FPUexception delivery and whole collision caller/gameplay unqualified. No substituted game services or upstream implementations.')
 suffix='_'+str(shard) if shard is not None else '';dest=P/'analysis'/('collision_angle_direct'+suffix+'_verification.json')
 if smoke:dest=P.parents[1]/'work/sonic-adventure/collision_angle_direct_smoke.json'
 if case_id is not None:dest=P.parents[1]/'work/sonic-adventure'/('collision_angle_case_'+str(case_id)+'.json')
 dest.write_text(json.dumps(report,indent=2)+'\n');print('COLLISION ANGLE DIRECT PASS',len(records),checks,dict(status),coverage,flush=True)
if __name__=='__main__':main()
