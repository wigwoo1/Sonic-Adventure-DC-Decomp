"""Full-state original/native checks of the newly exposed canonical normalization route."""
import collections,csv,hashlib,itertools,json,math,struct,sys
from pathlib import Path
import write_watch_collision_normalize as fast
v=fast.old;P=v.P;ENTRY=0x8c63a94c;VECTOR=0x8cc00000;LISTING=P/'analysis/main-dispatch-next-family/8c63a94c_instructions.tsv'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def bits(x):return struct.unpack('<I',struct.pack('<f',x))[0]
def number(x):return struct.unpack('<f',struct.pack('<I',x))[0]
def configurations():
 vectors=[(3,4,0),(-3,4,0),(1,0,0),(0,-1,0),(0,0,0),(1/1024,2/1024,-2/1024),(1,1,1),(1e10,2e10,-2e10)]
 words=[tuple(map(bits,x)) for x in vectors]+[(0x7f800000,bits(1),0),(0x7fbfffff,bits(1),bits(2)),(0x7fc00001,bits(1),0),(1,bits(1),0),(0x80000000,0,0),(0x7f7fffff,bits(1),bits(2))]
 for bank,rm,dn,sz,pr,xyz in itertools.product(range(2),range(2),range(2),range(2),range(2),words):
  yield dict(kind='matrix',fpscr=(bank<<21)|(sz<<20)|(pr<<19)|(dn<<18)|rm,xyz=xyz,view='p1')
 for bank,en,xyz in itertools.product(range(2),(0x80,0x800,0xf80),((0x7fc00001,bits(1),0),(0,0,0),(1,bits(1),0))):
  yield dict(kind='enabled_exception',fpscr=(bank<<21)|0x40000|en,xyz=xyz,view='p1')
 for bank,rm,view,xyz in itertools.product(range(2),range(2),('p0','p1','p2'),vectors[:4]):
  yield dict(kind='view',fpscr=(bank<<21)|0x40000|rm,xyz=tuple(map(bits,xyz)),view=view)

def main():
 configs=[dict(c,case_id=i) for i,c in enumerate(configurations())];shard=int(sys.argv[sys.argv.index('--shard')+1]) if '--shard' in sys.argv else None
 if shard is not None:assert shard in range(2);configs=[c for c in configs if c['case_id']%2==shard]
 smoke='--smoke' in sys.argv
 if smoke:configs=[next(c for c in configs if c['fpscr']==0x40000 and c['xyz']==tuple(map(bits,(3,4,0))))]
 ram=v.seed_ram();records=[];seen=set();counts=collections.Counter();checks=oracles=0
 for c in configs:
  cc=0
  def require(ok,label):
   nonlocal checks,cc
   assert ok,(c,label);checks+=1;cc+=1
  fpscr=c['fpscr'];bank=(fpscr>>21)&1;cfg=v.Input(dict(entry=ENTRY,mode=(fpscr>>19)&7,probe=True,sr=0x400000a0|(c['case_id']&1),mach=0x89abcdef,macl=0x76543210,gbr=0x8ce01234))
  m,s,f=v.prepare(ram,cfg)
  for j in range(16):s.r[j]=(0x12345678^((j+1)*0x01030507))&0xffffffff
  s.r[15]=0x8cf00000;s.pr=0x8cfed000;s.pc=ENTRY;s.sr=cfg['sr'];top={'p0':0,'p1':0x80000000,'p2':0xa0000000}[c['view']];s.r[4]=(VECTOR&0x1fffffff)|top
  for b in range(2):
   for j in range(16):f.banks[b][j]=bits(.5+(b*16+j)/16)
  f.fpscr=fpscr;f.fpul=0xc0ffee42;cfg['fpul']=f.fpul
  offset=VECTOR&0xffffff;m[offset-16:offset+80]=bytes([0x6d])*96
  data=c['xyz']+(bits(.5),bits(1),bits(2),bits(3),bits(4));m[offset:offset+32]=b''.join(w.to_bytes(4,'little') for w in data)
  before=bytes(m);initial_s=type(s).from_buffer_copy(s);initial_f=type(f).from_buffer_copy(f)
  r,path,a,s,f=fast.compare(m,s,f,cfg)
  require(r['status'] in (0,1,-1),'supported completion or existing numeric trap/unsupported stop')
  require(r['full_state_match'],'full CPU/FPU/RAM/device state');require(r['ordered_boundaries_match'],'ordered original/native entry state')
  require([b['target'] for b in r['boundaries']]==[hex(ENTRY)],'one original leaf entry;no substituted game services')
  require(s.pr==initial_s.pr and s.r[15]==initial_s.r[15] and s.sr==initial_s.sr,'PR/SP/SR unchanged')
  require(all(s.r[j]==initial_s.r[j] for j in range(16) if j!=4),'only vector pointer integer register may change')
  require((r['final_mach'],r['final_macl'],r['final_gbr'],f.fpul)==(cfg['mach'],cfg['macl'],cfg['gbr'],initial_f.fpul),'integer specials/FPUL unaffected')
  want=bytearray(before);size=24 if fpscr&0x100000 else 12;want[offset:offset+size]=a[offset:offset+size];require(a==want,'only original in-place vector footprint')
  if r['status']==0:require(s.r[4]==initial_s.r[4],'completed original pointer restored')
  xyz=list(map(number,c['xyz']));length=math.sqrt(sum(x*x for x in xyz))
  ordinary=not(fpscr&0x180000) and not(fpscr&0xf80) and (fpscr&3)==0
  if ordinary and length and all(math.isfinite(x) and (w&0x7f800000 or not w&0x7fffffff) for w,x in zip(c['xyz'],xyz)) and length<1e15:
   require(r['status']==0,'ordinary finite vector completes');got=[number(v.load(a,initial_s.r[4]+4*j)) for j in range(3)]
   require(all(math.isclose(x,y/length,rel_tol=2e-5,abs_tol=2e-5) for x,y in zip(got,xyz)),'independent finite unit-vector components')
   require(math.isclose(math.sqrt(sum(x*x for x in got)),1,rel_tol=2e-5,abs_tol=2e-5),'independent unit-vector norm')
   require(math.isclose(number(f.banks[bank][0]),length,rel_tol=2e-5,abs_tol=2e-5),'original RTS-delay returns approximate length in FR0')
   require(list(f.banks[1-bank])==list(initial_f.banks[1-bank]),'inactive physical FP bank preserved');oracles+=1
  seen.update(path);counts[r['status']]+=1;records.append(dict(configuration=c,status=r['status'],stop_pc=r['stop_pc'],original_rows=len(path),full_state_match=True,ordered_boundaries_match=True,independent_checks=cc))
  if len(records)%24==0:print('NORMALIZE ROUTE',shard,len(records),dict(counts),flush=True)
 rows=list(csv.DictReader(LISTING.open(),delimiter='\t'));wanted={int(r['address'],16) for r in rows};coverage=dict(address=hex(ENTRY),static_rows=len(rows),executed_rows=len(wanted&seen),missing_rows=list(map(hex,sorted(wanted-seen))),executed_conditional_outcomes=0)
 files=[Path(__file__),P/'collision_normalize_reference.py',P/'write_watch_collision_normalize.py',P/'collision_angle_reference.py',P/'analysis/collision_normalize_connection.json',v.effects.DLL,LISTING,P/'source/sa_motion_reuse_sa_main_dispatch_face_lit_primitives.c',P/'source/sa_draw_motion_mesh_lower_connected.c']
 report=dict(original_native_comparisons=len(records),independent_checks=checks,finite_unit_vector_oracle_cases=oracles,status_counts=dict(counts),cases=records,canonical_routine_coverage=coverage,new_recovered_routines=0,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='New root bridge route to existing canonical normalizer through unchanged lower context/observer/bus. Full original/native CPU/FPU/RAM/device/ordered boundaries;both banks/RM0/1/SZ/PR/DN,selected enables,normal/zero/nonfinite/subnormal/overflow inputs,mappedMMUoffP0/P1/P2 views. Independent finite geometry uses2e-5 tolerance;FIPR/FSRRA share pinned backend,not arbitrary-bit hardware proof. Valid bounded mapped vector buffers only;invalid-pointer/MMU/fault/physical exception delivery and wholecollision/gameplay remain unqualified. Canonicalcode receiveszero newroutinecredit.')
 suffix='_'+str(shard) if shard is not None else '';dest=P/'analysis'/('collision_normalize_direct'+suffix+'_verification.json')
 if smoke:dest=P.parents[1]/'work/sonic-adventure/collision_normalize_smoke.json'
 dest.write_text(json.dumps(report,indent=2)+'\n');print('NORMALIZER DIRECT PASS',len(records),checks,dict(counts),coverage,flush=True)
if __name__=='__main__':main()
