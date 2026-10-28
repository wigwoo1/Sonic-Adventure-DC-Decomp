"""Original/native matrix dependencies with independent determinant and inverse products."""
import collections,csv,hashlib,itertools,json,random,struct,sys
from fractions import Fraction
from pathlib import Path
import write_watch_collision_inverse as fast
v=fast.old;P=v.P;INVERSE,DET=0x8c6390b0,0x8c64f3ec;MATRIX=0x8cc00000;SINGULAR_SENTINEL=0x7f7fc99e

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def bits(x):return struct.unpack('<I',struct.pack('<f',x))[0]
def value(w):return Fraction.from_float(struct.unpack('<f',struct.pack('<I',w))[0])
def determinant(a):
 total=0
 for order in itertools.permutations(range(4)):
  term=1
  for i in range(4):term*=a[4*i+order[i]]
  total+=(-term if sum(order[i]>order[j] for i in range(4) for j in range(i+1,4))%2 else term)
 return total

def matrices():
 identity=[int(i%5==0) for i in range(16)]
 yield dict(name='identity',a=identity,exact=True)
 yield dict(name='zero',a=[0]*16,exact=True)
 a=list(identity);a[4:8]=a[:4];yield dict(name='repeated_row',a=a,exact=True)
 yield dict(name='diagonal',a=[(-2,1,2,-1)[i//4] if i%5==0 else 0 for i in range(16)],exact=True)
 yield dict(name='upper',a=[1,1,-1,2,0,-2,1,1,0,0,2,-1,0,0,0,1],exact=True)
 yield dict(name='lower',a=[2,0,0,0,1,-1,0,0,-1,2,1,0,2,-1,1,-2],exact=True)
 yield dict(name='permutation',a=[int(j==(i+1)%4) for i in range(4) for j in range(4)],exact=True)
 yield dict(name='translation',a=[1,0,0,2,0,1,0,-1,0,0,1,3,0,0,0,1],exact=True)
 for seed in range(4):
  rng=random.Random(seed);yield dict(name='dense'+str(seed),a=[rng.randrange(-2,3) for _ in range(16)],exact=False)

def configurations():
 ms=list(matrices())
 for entry,bank,rm,dn,explicit,m in itertools.product((INVERSE,DET),range(2),range(2),range(2),range(2),ms):yield dict(kind='core',entry=entry,fpscr=(bank<<21)|(dn<<18)|rm,explicit=explicit,matrix=m,view='p1')
 for entry,bank,rm,sz,pr,explicit,m in itertools.product((INVERSE,DET),range(2),range(2),range(2),range(2),range(2),ms[:2]):yield dict(kind='mode',entry=entry,fpscr=(bank<<21)|(sz<<20)|(pr<<19)|0x40000|rm|0x3f07c,explicit=explicit,matrix=m,view='p1')
 for entry,bank,explicit,word in itertools.product((INVERSE,DET),range(2),range(2),(0x7f800000,0xff800000,0x7fbfffff,0x7fc00001,1,0x7f7fffff)):
  raw=list(map(bits,ms[0]['a']));raw[0]=word;yield dict(kind='edge',entry=entry,fpscr=(bank<<21)|0x40000,explicit=explicit,matrix=dict(name='edge',words=raw),view='p1')
 for entry,bank,sz,guard in itertools.product((INVERSE,DET),range(2),range(2),('bad_matrix','edge_matrix')):yield dict(kind='guard',entry=entry,fpscr=(bank<<21)|(sz<<20)|0x40000,explicit=1,matrix=ms[0],view='p1',guard=guard)
 for entry,bank,rm,view,m in itertools.product((INVERSE,DET),range(2),range(2),('p0','p1','p2'),(ms[0],ms[4])):yield dict(kind='view',entry=entry,fpscr=(bank<<21)|0x40000|rm,explicit=1,matrix=m,view=view)

def main():
 configs=[dict(c,case_id=i) for i,c in enumerate(configurations())];shard=int(sys.argv[sys.argv.index('--shard')+1]) if '--shard' in sys.argv else None
 if shard is not None:assert shard in range(3);configs=[c for c in configs if c['case_id']%3==shard]
 case_id=int(sys.argv[sys.argv.index('--case-id')+1]) if '--case-id' in sys.argv else None
 if case_id is not None:configs=[c for c in configs if c['case_id']==case_id];assert len(configs)==1
 smoke='--smoke' in sys.argv
 if smoke:configs=[next(c for c in configs if c['entry']==e and c['fpscr']==0x40000 and c['explicit']==x and c['matrix']['name']=='upper') for e,x in itertools.product((INVERSE,DET),range(2))]
 ram=v.seed_ram();assert v.load(ram,0x8c6390e8)==SINGULAR_SENTINEL
 seen=set();edges=set();records=[];counts=collections.Counter();checks=det_oracles=exact_inverse=approx_inverse=singular_oracles=0
 for c in configs:
  cc=0
  def require(ok,label):
   nonlocal checks,cc
   assert ok,(c,label);checks+=1;cc+=1
  entry=c['entry'];fpscr=c['fpscr'];bank=(fpscr>>21)&1;cfg=v.Input(dict(entry=entry,mode=(fpscr>>19)&7,probe=True,sr=0x400000a0|(c['case_id']&1),mach=0x89abcdef,macl=0x76543210,gbr=0x8ce01234))
  m,s,f=v.prepare(ram,cfg)
  for j in range(16):s.r[j]=(0x12345678^((j+1)*0x01030507))&0xffffffff
  top={'p0':0,'p1':0x80000000,'p2':0xa0000000}[c['view']];pointer=(MATRIX&0x1fffffff)|top;s.r[4]=pointer if c['explicit'] else 0;s.r[15]=0x8cf00000;s.pr=0x8cfed000;s.pc=entry;s.sr=cfg['sr']
  for b in range(2):
   for j in range(16):f.banks[b][j]=bits(.5+(b*16+j)/16)
  words=c['matrix'].get('words') or list(map(bits,c['matrix']['a']));f.banks[bank^1][:]=words;f.fpscr=fpscr;f.fpul=0xc0ffee42;cfg['fpul']=f.fpul
  offset=MATRIX&0xffffff;m[offset-32:offset+256]=bytes([0x55])*288;m[offset:offset+64]=b''.join(w.to_bytes(4,'little') for w in words)
  if c.get('guard'):s.r[4]=0x8b000000 if c['guard']=='bad_matrix' else 0x8cfffff0
  initial_s=type(s).from_buffer_copy(s);initial_f=type(f).from_buffer_copy(f);before=bytes(m)
  r,path,a,s,f=fast.compare(m,s,f,cfg)
  if r['status'] not in (0,1,-1,-3):print('UNEXPECTED STOP',c,{k:r[k] for k in ('status','stop_pc','boundaries')},flush=True)
  require(r['status'] in (0,1,-1,-3),'complete/trap/unsupported/mapped-memory status;no missing game service');require(r['full_state_match'],'full CPU/FPU/RAM/device state');require(r['ordered_boundaries_match'],'ordered original/native boundaries')
  wanted_targets=[hex(entry)]+([hex(DET)] if entry==INVERSE else [])
  require([b['target'] for b in r['boundaries']]==wanted_targets,'actual original determinant child executes;no supplied game service/results/frames')
  require((r['final_mach'],r['final_macl'],r['final_gbr'])==(cfg['mach'],cfg['macl'],cfg['gbr']),'integer MACH/MACL/GBR untouched')
  want=bytearray(before)
  if entry==INVERSE:
   want[0xefff00:0xf00000]=a[0xefff00:0xf00000]
   if c['explicit'] and not c.get('guard'):
    # Singular: R4=M+64, sixteen predecrements. SZ0 writes[M,M+64),
    # SZ1 writes[M-64,M+64). Nonsingular final pointer walks fit this union:
    # R5=M+40 then ten predecrements; R6=M+56 then two, plus M+40/44/56/60.
    start=offset-64 if fpscr&0x100000 else offset;want[start:offset+64]=a[start:offset+64]
  require(a==want,'only original inverse matrix/stack write footprints;determinant writes no RAM')
  if r['status']==0:require(s.pr==initial_s.pr and s.r[15]==initial_s.r[15],'completed PR/SP restored');require(list(s.r[8:15])==list(initial_s.r[8:15]),'callee-saved integer registers')
  ordinary=c['kind'] in ('core','view') and not fpscr&0x180000
  if ordinary:
   require(r['status']==0,'bounded finite matrix operation completes');mat=c['matrix']['a'];d=determinant(mat)
   if entry==DET:
    require(value(f.banks[bank][0])==d,'independent 24-term exact integer determinant');require(a==before,'determinant is read-only');det_oracles+=1
   else:
    output=[int.from_bytes(a[offset+4*i:offset+4*i+4],'little') for i in range(16)] if c['explicit'] else list(f.banks[bank^1])
    if d==0:require(output==[SINGULAR_SENTINEL]*16 and s.r[0]==0,'original singular result fills the original finite sentinel and returns zero');singular_oracles+=1
    else:
     inverse=list(map(value,output));products=[sum(Fraction(mat[4*i+k])*inverse[4*k+j] for k in range(4)) for i in range(4) for j in range(4)];want_identity=[int(i%5==0) for i in range(16)]
     if c['matrix']['exact']:require(products==want_identity,'independent exact rational A*inverse(A)=I');exact_inverse+=1
     else:require(all(abs(x-y)<=Fraction(1,10000) for x,y in zip(products,want_identity)),'independent bounded dense inverse product absolute tolerance1e-4');approx_inverse+=1
  if c.get('guard'):require(r['status']==-3,'mapped matrix guard reached without supplied callee outputs')
  seen.update(path)
  for i,pc in enumerate(path):
   w=int.from_bytes(ram[pc&0xffffff:(pc&0xffffff)+2],'little')
   if w>>8 in (0x89,0x8b,0x8d,0x8f):
    j=i+(2 if w>>8 in (0x8d,0x8f) else 1)
    if j<len(path):edges.add((pc,path[j]))
  counts[r['status']]+=1;records.append(dict(configuration=c,status=r['status'],stop_pc=r['stop_pc'],original_rows=len(path),full_state_match=True,ordered_boundaries_match=True,independent_checks=cc))
  if len(records)%24==0:print('INVERSE ROUTE',shard,len(records),dict(counts),flush=True)
 coverage=[]
 for body in v.ANALYSIS_ROUTINES:
  wanted=set(map(lambda x:int(x,16),body['reachable_addresses']));wanted_edges={tuple(int(x,16) for x in e) for e in body['conditional_edges']}
  coverage.append(dict(address=body['address'],static_rows=body['instruction_rows'],executed_rows=len(wanted&seen),executed_outcomes=len(wanted_edges&edges),missing_rows=list(map(hex,sorted(wanted-seen))),missing_outcomes=[[hex(x),hex(y)] for x,y in sorted(wanted_edges-edges)]))
 files=[Path(__file__),P/'collision_inverse_reference.py',P/'write_watch_collision_inverse.py',v.MP,P/'analysis/collision_inverse_connection.json',v.effects.DLL,P/'source/sa_collision_inverse.c']
 report=dict(original_native_comparisons=len(records),independent_checks=checks,exact_determinant_oracle_cases=det_oracles,exact_inverse_identity_cases=exact_inverse,approximate_inverse_identity_cases=approx_inverse,singular_sentinel_oracle_cases=singular_oracles,status_counts=dict(counts),cases=records,coverage_by_routine=coverage,new_original_routine_credit=0,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Canonical inverse/determinant modernABI adapters,original internal call/PR/stop ordering,fullCPU/FPU/RAM/device/ordered boundaries. Independent integer permutation determinant and exact/bounded rational inverse products;original singular sentinel0x7f7fc99e and zero return. Paired explicit stores use the original backward pointer footprint. Existing numeric/guard stop policies retained. SRFDclear ABI,validmapped matrices/stacks;physicalhardware/exception/MMUdelivery/gameplay unqualified. Oldcanonical implementations/proofs retained,no duplicate routinecredit.')
 suffix='_'+str(shard) if shard is not None else '';dest=P/'analysis'/('collision_inverse_direct'+suffix+'_verification.json')
 if smoke:dest=P.parents[1]/'work/sonic-adventure/collision_inverse_smoke.json'
 if case_id is not None:dest=P.parents[1]/'work/sonic-adventure'/('collision_inverse_case_'+str(case_id)+'.json')
 dest.write_text(json.dumps(report,indent=2)+'\n');print('INVERSE DIRECT PASS',len(records),checks,dict(counts),coverage,flush=True)
if __name__=='__main__':main()
