"""Full original clipper execution with independent physical integer banks."""
import collections,csv,ctypes,hashlib,itertools,json,random,struct,sys,types
from pathlib import Path
from fractions import Fraction
import verify_draw_angle_blend as previous
import verify_draw_primitive_stream as fixtures
import verify_draw_gbr_geometry as gbr_cpu
import verify_sh4_integer_state as core
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent;SEED=previous.SEED
MANIFEST=PROJECT/'analysis/draw_clipping_state_translation.json';manifest=json.loads(MANIFEST.read_text())
NEW={int(r['address'],16) for r in manifest['routines']};CONNECTED=previous.CONNECTED|NEW
ROWS=dict(previous.ROWS);OWN={};PER_BODY={}
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};PER_BODY[int(routine['address'],16)]=body;OWN.update(body);ROWS.update(body)
parent_manifest=json.loads((PROJECT/'analysis/draw_primitive_stream_translation.json').read_text())
PARENTS={fixtures.CLIPPED_UV,fixtures.CLIPPED_SOLID,fixtures.ENV_CLIP}
ALL_ROUTINES=[dict(r,source=manifest['source']) for r in manifest['routines']]
for routine in parent_manifest['routines']:
 if int(routine['address'],16) not in PARENTS:continue
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};PER_BODY[int(routine['address'],16)]=body;OWN.update(body);ROWS.update(body)
 ALL_ROUTINES.append(dict(routine,source=parent_manifest['source']))
RETURNS=previous.RETURNS|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))}
class BankView:
 """Canonical physical storage; selecting a bank never swaps oracle arrays."""
 def __init__(self,visible,sr,special):
  self.sr=sr;active=core.bank(sr);self.banks=[None,None];self.banks[active]=list(visible[:8]);self.banks[1-active]=list(special['other']);self.common=list(visible[8:]);self.written=set();self.live=set();self.pc=0
 def __len__(self):return 16
 def __iter__(self):return iter(self.banks[core.bank(self.sr)]+self.common)
 def __getitem__(self,i):
  if isinstance(i,slice):return list(self)[i]
  if not 0<=i<16:raise IndexError(i)
  if i>=8:return self.common[i-8]
  b=core.bank(self.sr)
  if b==0 and i not in self.written:self.live.add((self.pc,i))
  return self.banks[b][i]
 def __setitem__(self,i,v):
  assert 0<=i<16
  if i>=8:self.common[i-8]=v
  else:
   b=core.bank(self.sr);self.banks[b][i]=v
   if b==0:self.written.add(i)
 def inactive(self):return list(self.banks[1-core.bank(self.sr)])
 def publish(self,special):special['other']=self.inactive();special['bank0_live_in']=sorted(self.live)
cpu_code=gbr_cpu.cpu_code
marker='regs = list(state.r); sr = state.sr;';assert cpu_code.count(marker)==1
cpu_code=cpu_code.replace(marker,'regs = BankView(list(state.r),state.sr,special); sr = state.sr;')
marker="if special is not None:special['macl']=macl";assert cpu_code.count(marker)==3
cpu_code=cpu_code.replace(marker,"if special is not None:special['macl']=macl;regs.publish(special)")
marker='        if trace is not None:';assert cpu_code.count(marker)==1
cpu_code=cpu_code.replace(marker,'        regs.pc=pc\n'+marker)
marker='        elif word == 9: pass';assert cpu_code.count(marker)==1
cpu_code=cpu_code.replace(marker,"""        elif word & 0xF00F == 0x300D:
            a,b=regs[n],regs[m];a=a if a<2**31 else a-2**32;b=b if b<2**31 else b-2**32
            product=(a*b)%(2**64);macl=product%(2**32);special['mach']=product//(2**32)
"""+marker)
marker='elif word & 0xF0FF == 0x400E: sr = regs[n]';assert cpu_code.count(marker)==1
cpu_code=cpu_code.replace(marker,'elif word & 0xF0FF == 0x400E: sr = valid_sr(regs[n]);regs.sr=sr')
cpu_ns=dict(gbr_cpu.cpu_ns,BankView=BankView,valid_sr=core.valid_sr);exec(compile(cpu_code,'clipping_original_banked_executor','exec'),cpu_ns)
namespace=dict(previous.namespace,ROWS=ROWS,RETURNS=RETURNS,LEGACY=previous.namespace['LEGACY']-NEW,reference=cpu_ns['reference'])
code=previous.code
marker="'gbr':c.get('gbr',0)}";assert code.count(marker)==1
code=code.replace(marker,"'gbr':c.get('gbr',0),'mach':c.get('mach',0xcafebabe),'other':list(c.get('other',range(8))),'bank0_live_in':[]}")
marker="special['gbr']\n def stop";assert code.count(marker)==1
code=code.replace(marker,"special['gbr'],special['mach'],tuple(special['other'])\n def stop")
marker='  if pc in LEGACY and f.fpscr&0x180000:';assert code.count(marker)==1
code=code.replace(marker,"  if w&0xf0ff in (0x0002,0x400e) and not sr&0x40000000:stop(pc,r,sr,-4)\n"+marker)
marker="return status,path,events,special['macl'],special['gbr']";assert code.count(marker)==1
code=code.replace(marker,marker+",special['mach'],special['other'],special['bank0_live_in']")
exec(compile(code,'clipping_complete_reference','exec'),namespace);reference_run=namespace['reference_run']
DLL=SEED.ROOT/'work/sonic-adventure/build/draw_clipping_complete.dll';LIB=ctypes.CDLL(str(DLL))
Extra=core.Extra;fn=LIB.sa_draw_clipping_state_connected;fn.argtypes=previous.fn.argtypes+[ctypes.POINTER(Extra)];fn.restype=ctypes.c_int
LIB.sa_vertex_bus_init.argtypes=previous.LIB.sa_vertex_bus_init.argtypes
State,Fpu,OBSERVE,Memory,Bus,QueueReference=previous.State,previous.Fpu,previous.OBSERVE,previous.Memory,previous.Bus,previous.QueueReference
RAM_TYPE,ram_digest,bits,value=previous.RAM_TYPE,previous.ram_digest,previous.bits,previous.value
INIT=previous.INIT;ORIGINAL_GBR_LITERAL=previous.ORIGINAL_GBR_LITERAL
UV,SOLID,ENV=0x8c037ee0,0x8c038802,0x8c038516
VERTICES,UVS,NORMALS,OFFSETS=0x8cd30000,0x8cd40000,0x8cd50000,0x8cd60000
OUTPUT=fixtures.OUTPUT
rounded=previous.rounded;round_fraction=previous.round_fraction
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def finish_fixture(m,s,f,c):
 bank=(f.fpscr>>21)&1
 if c.get('matrix'):f.banks[1-bank][:]=[bits(x) for x in c['matrix']]
 if c.get('alias'):
  slots=(2,4,15) if c['entry'] in PARENTS else (8,9,10,11,13,15)+((1,) if c['entry']==ENV else ())
  for r in slots:s.r[r]=(s.r[r]&0xffffff)|c['alias']
  c['gbr']=(c['gbr']&0xffffff)|c['alias']
 return m,s,f
def prepare(ram,c):
 # Existing checked preparation supplies real renderer globals; explicit direct
 # clipper fixtures below remain separate from natural startup/caller proof.
 m,s,f=fixtures.prepare(ram,c if c['entry'] in PARENTS else dict(c,entry=fixtures.CLIPPED_UV));c['gbr']=load(m,ORIGINAL_GBR_LITERAL);bank=(f.fpscr>>21)&1
 viewport=load(m,0x8c037fe8);center=load(m,0x8c037fec)
 for j,x in enumerate(c.get('viewport',(4,8))):store(m,viewport+j*4,bits(x))
 for j,x in enumerate(c.get('center',(1,-2))):store(m,center+j*4,bits(x))
 if c['entry'] in PARENTS:
  s.sr=c.get('sr',0x600003f3)
  rng=random.Random(c.get('seed',0));c['other']=[rng.getrandbits(32) for _ in range(8)];c['mach']=rng.getrandbits(32)
  return finish_fixture(m,s,f,c)
 s.pc=c.get('entry',UV);s.sr=c.get('sr',0x600003f3);s.r[15]=0x8cf00000
 for i,z in enumerate(c.get('depths',(.5,2,2))):
  for j,x in enumerate((10+i*2,20-i*3,z)):store(m,VERTICES+i*16+j*4,bits(x))
  store(m,VERTICES+i*16+12,c.get('colors',(0xff102030,0x80406080,0x20c0a080))[i])
  store(m,c['gbr']+0x48+i*4,c.get('specular',(0x12345678,0x9abcdef0,0xfedcba98))[i])
  u,v=c.get('uvs',((-32,64),(128,-64),(255,96)))[i]
  store(m,UVS+i*4,u&65535,2);store(m,UVS+i*4+2,v&65535,2)
  store(m,OFFSETS+i*2,i*12,2)
  for j,x in enumerate(c.get('normals',((-.5,.5,0),(.5,-.5,0),(0,0,1)))[i]):store(m,NORMALS+i*12+j*4,bits(x))
 s.r[8]=VERTICES;s.r[9]=VERTICES+16;s.r[10]=VERTICES+32;s.r[11]=UVS
 s.r[1]=NORMALS if s.pc==ENV else 0x13572468;s.r[13]=OFFSETS;s.r[14]=(OUTPUT&0x3ffffff)|0xe0000000
 coefficient=load(m,load(m,0x8c0382dc)) if s.pc==ENV else load(m,0x8c038f10)
 f.banks[bank][11]=coefficient
 for i in range(16):f.banks[1-bank][i]=bits(1 if i in (0,5,10,15) else 0)
 rng=random.Random(c.get('seed',0));c['other']=[rng.getrandbits(32) for _ in range(8)];c['mach']=rng.getrandbits(32)
 if c.get('bad')=='stack':s.r[15]=0
 if c.get('bad')=='vertices':s.r[8]=0x8cfffffc
 if c.get('bad')=='uv':s.r[11]=0x8cfffffe
 if c.get('bad')=='normals':s.r[1]=0x8cfffffc
 if c.get('bad')=='destination':s.r[14]=0xe5000000
 return finish_fixture(m,s,f,c)
def as_fraction(raw):return Fraction(value(raw))
def oracle(mem,regs,fp,gbr,entry):
 """Geometric polygon clipping, separate staged arithmetic and fixed colours."""
 solid=entry==SOLID;bank=(fp.fpscr>>21)&1
 w,h=[as_fraction(load(mem,load(mem,0x8c037fe8)+j*4)) for j in range(2)]
 cx,cy=[as_fraction(load(mem,load(mem,0x8c037fec)+j*4)) for j in range(2)]
 iw,ih=rounded(1/w),rounded(1/h);scale=as_fraction(fp.banks[bank][11]);vertices=[]
 for i,r in enumerate((8,9,10)):
  x,y,q=[as_fraction(load(mem,regs[r]+j*4)) for j in range(3)];d=rounded(1/q)
  color=load(mem,regs[r]+12);spec=load(mem,gbr+0x48+i*4)
  u=v=Fraction(0)
  if not solid:
   if entry==ENV:
    offset=load(mem,regs[13]+i*2,2);offset=offset if offset<32768 else offset-65536
    normal=[as_fraction(load(mem,regs[1]+offset+j*4)) for j in range(3)]
    matrix=[as_fraction(fp.banks[1-bank][j]) for j in range(16)]
    uv=[rounded(sum(matrix[col*4+row]*normal[col] for col in range(3))) for row in (0,1)]
    factor=as_fraction(load(mem,load(mem,0x8c038660)))
    u,v=[rounded(rounded(x*factor)+factor) for x in uv]
   else:
    raw=[load(mem,regs[11]+i*4+j*2,2) for j in range(2)];u,v=[Fraction(x if x<32768 else x-65536) for x in raw]
  vertices.append(dict(d=d,x=x,y=y,q=q,camx=rounded(rounded(rounded(x-cx)*d)*iw),camy=rounded(rounded(rounded(y-cy)*d)*ih),color=color,spec=spec,u=u,v=v))
 polygon=[]
 def mix(a,b,t):return rounded(a+rounded(rounded(b-a)*t))
 def packed(a,b,fixed):return sum((((a>>k)&255)+((((b>>k)&255)-((a>>k)&255))*fixed//65536))<<k for k in (0,8,16,24))&0xffffffff
 if any(v['d']>1 for v in vertices):
  for i,current in enumerate(vertices):
   prior=vertices[i-1]
   if (prior['d']<1)!=(current['d']<1):
    t=rounded(rounded(1-prior['d'])/rounded(current['d']-prior['d']));fixed=int(rounded(t*65536))
    polygon.append(dict(q=Fraction(1),x=rounded(rounded(mix(prior['camx'],current['camx'],t)*w)+cx),y=rounded(rounded(mix(prior['camy'],current['camy'],t)*h)+cy),color=packed(prior['color'],current['color'],fixed),spec=packed(prior['spec'],current['spec'],fixed),u=mix(prior['u'],current['u'],t),v=mix(prior['v'],current['v'],t)))
   if current['d']>1:polygon.append(dict(current))
 base=load(mem,0x8c038024);old_records=[[load(mem,base+i*40+12+j*4) for j in range(7)] for i in range(4)]
 return dict(entry=entry,cursor=regs[14],vertices=vertices,polygon=polygon,scale=scale,solid=solid,old_records=old_records,writer=any(v['d']>1 for v in vertices))
def same_float(raw,x):
 wanted=round_fraction(x)
 return raw==wanted or raw&0x7fffffff==wanted&0x7fffffff==0
def check_geometry(m,actual,s,f,initial,initial_f,gbr,c,native,captured):
 checks=0
 for o in captured:
  solid=o['solid'];base=load(m,0x8c038024);scratch=bytearray(m)
  for i,row in enumerate(o['old_records']):
   for j,v in enumerate(row):store(scratch,base+i*40+12+j*4,v)
  # Formula-level expected records preserve the original scratch layout. In
  # equality/degenerate cases the fixed writer can read untouched old records;
  # retain those bytes rather than inventing ideal extra polygon vertices.
  for i,p in enumerate(o['polygon']):
   for j,k in enumerate(('q','y','x','color','spec','u','v')):
    if solid and j>=5:break
    store(scratch,base+i*40+12+j*4,p[k] if k in ('color','spec') else round_fraction(p[k]))
  order=([0,1,2] if len(o['polygon'])==3 else [0,1,3,2]) if o['writer'] else []
  for i,j in enumerate(order):
   fields=[load(scratch,base+j*40+12+k*4) for k in range(7)]
   expected=[0xf0000000 if i==len(order)-1 else 0xe0000000,fields[2],fields[1],fields[0],0 if solid else round_fraction(rounded(as_fraction(fields[5])*o['scale'])),0 if solid else round_fraction(rounded(as_fraction(fields[6])*o['scale'])),fields[3],0 if solid else fields[4]]
   address=o['cursor']+i*32
   got=[load(actual,address+k*4) for k in range(8)]
   assert all(g==v or k in (1,2,3,4,5) and g&0x7fffffff==v&0x7fffffff==0 for k,(g,v) in enumerate(zip(got,expected))),(c,'independent clipped packet',i,got,expected)
   checks+=1
  if initial.pc in (UV,SOLID,ENV):
   wanted=bytearray(m);input_base=load(m,0x8c03801c)
   for i,p in enumerate(o['vertices']):
    for j,k in enumerate(('d','camy','camx','q','y','x','color','spec','u','v')):
     if solid and j>=8:break
     address=input_base+i*40+j*4;raw=load(actual,address)
     assert raw==p[k] if k in ('color','spec') else same_float(raw,p[k]),(c,'independent prepared clip scratch',i,k)
     store(wanted,address,raw);checks+=1
   for i,p in enumerate(o['polygon']):
    for j,k in enumerate(('q','y','x','color','spec','u','v')):
     if solid and j>=5:break
     address=base+i*40+12+j*4;raw=load(actual,address)
     assert raw==p[k] if k in ('color','spec') else same_float(raw,p[k]),(c,'independent clipped scratch',i,k)
     store(wanted,address,raw);checks+=1
   for i in range(len(order)):
    for k in range(8):store(wanted,o['cursor']+i*32+k*4,load(actual,o['cursor']+i*32+k*4))
   sp=initial.r[15];store(wanted,sp-4,initial.r[1] if solid else initial.r[12])
   if o['writer']:
    saved_r11=initial.r[11]+12 if initial.pc==UV else initial.r[11]
    for off,v in ((-8,saved_r11),(-12,initial.r[12] if solid else initial.r[1]),(-16,initial.r[13])):store(wanted,sp+off,v)
   assert actual==wanted,(c,'independent full clipper RAM footprint');checks+=1
   assert s.r[14]==initial.r[14]+len(order)*32 and s.r[15]==initial.r[15] and s.r[12]==initial.r[12] and s.pr==initial.pr and native.flushes==len(order),(c,'independent clip cursor/stack/preserved caller');checks+=1
 return checks
def check_fast_parent(m,actual,initial,initial_f,s,gbr,c,native):
 if initial.pc not in PARENTS or c.get('bad') or c.get('probe') or not all(0<z<1 for z in c.get('depths',(.25,.5,.75))):return 0
 solid=initial.pc==fixtures.CLIPPED_SOLID;environment=initial.pc==fixtures.ENV_CLIP or bool(c.get('environment'));count=0
 factor=as_fraction(load(m,0x8c038f10))
 for strip,n in enumerate(c.get('lengths',(3,))):
  for j in range(n):
   i=(strip*3+j)%16;expected=[0xf0000000 if j==n-1 else 0xe0000000]+[load(m,fixtures.CACHE+i*16+k*4) for k in range(3)]
   if solid:expected.extend((0,0,load(m,fixtures.CACHE+i*16+12),0))
   else:
    if environment:
     bank=(initial_f.fpscr>>21)&1;matrix=[as_fraction(initial_f.banks[1-bank][j]) for j in range(16)];normal=[as_fraction(load(m,load(m,initial.r[4]+4)+i*12+k*4)) for k in range(3)]
     u,v=[rounded(rounded(rounded(sum(matrix[col*4+row]*normal[col] for col in range(3)))*Fraction(1,2))+Fraction(1,2)) for row in (0,1)]
    else:u,v=rounded((i*8-40)*factor),rounded((32-i*4)*factor)
    expected.extend((round_fraction(u),round_fraction(v),load(m,fixtures.CACHE+i*16+12),load(m,fixtures.SPEC+i*4)))
   got=[load(actual,initial.r[14]+count*32+k*4) for k in range(8)]
   assert got==expected,(c,'independent unclipped parent packets',j,got,expected)
   count+=1
 # Environment parent stores the decremented strip counter only on a taken
 # back edge; its last strip deliberately leaves one in the guest global.
 counter=1 if environment else 0
 assert s.r[0]==s.r[14]==initial.r[14]+count*32 and s.r[15]==initial.r[15] and s.pr==initial.pr and all(s.r[r]==initial.r[r] for r in (3,4,7,13)) and native.flushes==count and load(actual,gbr+0x44)==counter,(c,'independent parent count/cursor/counter/preserved state')
 for off,v in ((-4,initial.pr),(-8,initial.r[13]),(-12,initial.r[7]),(-16,initial.r[4]),(-20,initial.r[3])):assert load(actual,initial.r[15]+off)==v,(c,'independent saved parent stack',off)
 return count+6
def compare(m,s,f,c):
 initial=State.from_buffer_copy(s);initial_f=Fpu.from_buffer_copy(f);other=State.from_buffer_copy(s);ef=Fpu.from_buffer_copy(f);actual=bytearray(m);expected=bytearray(m)
 ref=QueueReference();ref.ram=expected;ref.qacr[:]=[12,12];namespace['queue_reference']=ref
 status,path,wanted,ml_expected,gb_expected,mh_expected,hidden,live=reference_run(expected,other,ef,c,tracked=tuple(CONNECTED))
 memory=Memory();bus=Bus();view=RAM_TYPE.from_buffer(actual);LIB.sa_vertex_bus_init(ctypes.byref(memory),view,ctypes.byref(bus));memory.qacr[:]=[12,12]
 ml=ctypes.c_uint32(0x12345678);gb=ctypes.c_uint32(c['gbr']);extra=Extra();extra.mach=c['mach'];extra.other[:]=c['other'];events=[];captured=[];errors=[]
 @OBSERVE
 def observe(ctx,target,mem,fp,sr,r,pr):
  events.append((target,tuple(r[:16]),sr[0],pr[0],bytes(fp.contents),ml.value,ram_digest(mem),gb.value,extra.mach,tuple(extra.other)))
  if target in (UV,SOLID,ENV) and not c.get('bad') and not c.get('probe') and not fp.contents.fpscr&0x180000:
   try:captured.append(oracle(mem,tuple(r[:16]),fp.contents,gb.value,target))
   except Exception as exc:errors.append(repr(exc))
 rc=fn(view,ctypes.byref(s),ctypes.byref(f),ctypes.cast(ctypes.byref(f,Fpu.fpul.offset),core.P),ctypes.byref(ml),c.get('entry',UV),None,observe,ctypes.byref(bus),ctypes.byref(gb),ctypes.byref(extra))
 assert rc==status and events==wanted and actual==expected and bytes(s)==bytes(other) and bytes(f)==bytes(ef) and ml.value==ml_expected and gb.value==gb_expected and extra.mach==mh_expected and list(extra.other)==hidden,(c,'full clipper state',rc,status,hex(s.pc),hex(other.pc),len(events),len(wanted))
 emitted=[(e.kind,e.address,e.value,e.width) for e in memory.trace[:memory.trace_count]]
 assert emitted==ref.events[:4096] and memory.trace_dropped==max(0,len(ref.events)-4096) and list(memory.queue)==ref.queue and list(memory.qacr)==ref.qacr and (memory.flushes,memory.prefetches,memory.invalidates)==(ref.flushes,ref.prefetches,ref.invalidates),(c,'full ordered queue/cache')
 assert not errors,(c,'independent input capture',errors)
 checks=check_geometry(m,actual,s,f,initial,initial_f,gb.value,c,memory,captured)+check_fast_parent(m,actual,initial,initial_f,s,gb.value,c,memory) if rc==0 and not c.get('probe') else 0
 return dict(input=dict(c),status=rc,stop_pc=hex(s.pc),steps=len(path),bank0_live_in=[[hex(a),i] for a,i in live],full_state_match=True,ordered_boundaries_match=True,queue_match=True,independent_checks=checks,independent_clip_invocations=len(captured),queue_flushes=memory.flushes,queue_events=memory.trace_count+memory.trace_dropped),path,actual,s,f,memory
def cases():
 masks=list(itertools.product((.5,2),repeat=3))
 for entry,z,bank,seed in itertools.product((UV,SOLID,ENV),masks,range(2),range(2)):
  yield dict(entry=entry,depths=z,mode=bank*4,seed=seed)
 for entry,z,bank in itertools.product((UV,SOLID,ENV),((1,1,1),(.5,1,1),(1,.5,2),(2,1,.5),(-.5,.5,2),(2,-.5,2)),range(2)):
  yield dict(entry=entry,depths=z,mode=bank*4,seed=42)
 for entry,z,bank,lengths in itertools.product(sorted(PARENTS),masks+[(.25,.5,.75),(-.25,.5,.75),(-1,-1,-1),(1.5,.5,.75)],range(2),((3,),(4,),(6,),(3,4))):
  yield dict(entry=entry,depths=z,mode=bank*4,lengths=lengths,seed=bank)
 for entry,bank,negative,override in itertools.product(sorted(PARENTS),range(2),(False,True),(0,3)):
  for environment in ((False,True) if entry==fixtures.CLIPPED_UV else (False,)):
   yield dict(entry=entry,depths=(.5,2,.5),mode=bank*4,negative=negative,override=override,environment=environment,seed=7)
 for entry,z,bank,n in itertools.product(sorted(PARENTS),((.5,.5,.5,2),(.5,.5,.5,-.5),(.5,2,.5,.5)),range(2),(4,5,6)):
  yield dict(entry=entry,depths=z,mode=bank*4,lengths=(n,),seed=11)
 rng=random.Random(0x37ee0)
 for i in range(48):
  yield dict(entry=(UV,SOLID,ENV)[i%3],depths=tuple(rng.choice((.25,.5,.75,1.25,1.5,2,4)) for _ in range(3)),mode=(i%2)*4,seed=i,colors=tuple(rng.getrandbits(32) for _ in range(3)),specular=tuple(rng.getrandbits(32) for _ in range(3)),uvs=tuple(tuple(rng.randrange(-32768,32768) for _ in range(2)) for _ in range(3)),viewport=(rng.randrange(1,20),rng.randrange(1,20)),center=(rng.randrange(-10,10),rng.randrange(-10,10)))
 for entry,mode in itertools.product((UV,SOLID,ENV,*sorted(PARENTS)),range(8)):
  yield dict(entry=entry,mode=mode,probe=True,seed=3)
 for entry,bad in itertools.product((UV,SOLID,ENV),('vertices','stack','destination')):yield dict(entry=entry,bad=bad,probe=True,seed=9)
 for entry,bad in itertools.product(sorted(PARENTS),('indices','cache','stack','destination')):yield dict(entry=entry,bad=bad,probe=True,seed=9)
 for entry in (UV,ENV):yield dict(entry=entry,bad='uv' if entry==UV else 'normals',probe=True,seed=6)
 for entry,sr in itertools.product((UV,SOLID,ENV),(0x3f3,0x200003f3)):yield dict(entry=entry,sr=sr,probe=True,seed=6)
 matrices=((.5,0,0,0,0,2,0,0,0,0,1,0,0,0,0,1),(0,-1,0,0,1,0,0,0,0,0,1,0,0,0,0,1))
 for entry,matrix,bank,z in itertools.product((ENV,fixtures.ENV_CLIP,fixtures.CLIPPED_UV),matrices,range(2),((.5,.5,2),(.25,.5,.75))):yield dict(entry=entry,matrix=matrix,mode=bank*4,depths=z,environment=entry==fixtures.CLIPPED_UV,lengths=(4,),seed=15)
 for entry,alias,bank in itertools.product((UV,SOLID,ENV,*sorted(PARENTS)),(0x0c000000,0xac000000),range(2)):yield dict(entry=entry,depths=(.5,2,.5),alias=alias,mode=bank*4,seed=16)
 for entry in (UV,SOLID,ENV):yield dict(entry=entry,enable=0xf80,probe=True,seed=5)
def initializer(ram):
 m,s,f=SEED.prepare(ram,dict(entry=INIT,mode=0));s.pc=INIT;s.sr=0x600003f3
 c=dict(entry=INIT,gbr=0,mach=0x87654321,other=[0x13572468+i for i in range(8)],probe=True)
 record,path,actual,*_=compare(m,s,f,c)
 assert record['status']==0 and len(path)==41
 return actual
def main():
 proofs=[MANIFEST,PROJECT/'analysis/draw_clipping_state_connection.json',PROJECT/'analysis/sh4_integer_state_verification.json',PROJECT/'analysis/draw_angle_blend_verification.json',PROJECT/'analysis/draw_primitive_stream_translation.json']
 for p in proofs:
  for f,h in json.loads(p.read_text())['sha256'].items():assert digest(f)==h,f
 assert (PROJECT/'build_draw_clipping_complete.ps1').read_text()==(PROJECT/'build_draw_clipping_state.ps1').read_text().replace('draw_clipping_state.dll','draw_clipping_complete.dll')
 ram=bytearray(16<<20)
 for p,offset in ((SEED.state.gate.support.morph.MAIN,0x10000),(SEED.state.gate.support.morph.MODULE,0x900000)):
  raw=p.read_bytes();ram[offset:offset+len(raw)]=raw
 ram=fixtures.caller.prepare(ram,dict(draw_flags=0x10,semantic=True))[0]
 ram=initializer(ram)
 selected=list(cases())
 if '--smoke' in sys.argv:selected=selected[:3]
 records=[];coverage=set();edges=set();targets=collections.Counter();boundaries=0
 for n,c in enumerate(selected):
  r,path,*_=compare(*prepare(ram,c),c);records.append(r);coverage.update(path)
  targets.update(a for a in path if a in CONNECTED);boundaries+=sum(a in CONNECTED for a in path)
  for i,a in enumerate(path[:-1]):
   if a not in OWN:continue
   w=int.from_bytes(ram[a&0xffffff:(a&0xffffff)+2],'little')
   if w>>8 in (0x89,0x8b,0x8d,0x8f):
    j=i+(2 if w>>8 in (0x8d,0x8f) else 1)
    if j<len(path):edges.add((a,path[j]))
  if (n+1)%32==0:print(n+1,'full clipping comparisons passed',flush=True)
 coverage_rows=[]
 for routine in ALL_ROUTINES:
  a=int(routine['address'],16);body=set(PER_BODY[a]);wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
  coverage_rows.append(dict(address=hex(a),rows_covered=len(body&coverage),static_rows=len(body),missing_rows=[hex(x) for x in sorted(body-coverage)],conditional_edges_covered=len(wanted&edges),static_conditional_edges=len(wanted),missing_edges=[[hex(x),hex(y)] for x,y in sorted(wanted-edges)]))
 if '--smoke' in sys.argv:print('Full clipping smoke passed:',len(records));return
 files=[Path(__file__),PROJECT/'verify_startup.py',PROJECT/'verify_draw_gbr_geometry.py',PROJECT/'verify_draw_primitive_stream.py',PROJECT/'verify_draw_angle_blend.py',PROJECT/'verify_sh4_integer_state.py',PROJECT/'build_draw_clipping_complete.ps1',PROJECT/'analysis/sh4_integer_state_architecture_reference.json']+proofs
 for p in proofs:files.extend(Path(f) for f in json.loads(p.read_text())['sha256'])
 routines=[dict(r,source_sha256=digest(PROJECT/r['source'])) for r in ALL_ROUTINES]
 report=dict(routines=routines,cases_passed=len(records),rows_covered=len(set(OWN)&coverage),rows_static=len(OWN),conditional_edges_covered=sum(x['conditional_edges_covered'] for x in coverage_rows),coverage_by_routine=coverage_rows,status_counts=dict(collections.Counter(r['status'] for r in records)),independent_cases=sum(bool(r['independent_checks']) for r in records),independent_checks=sum(r['independent_checks'] for r in records),independent_clip_invocations=sum(r['independent_clip_invocations'] for r in records),normal_bank1_live_in=sorted({tuple(x) for r in records if r['input'].get('sr',0x600003f3)&0x60000000==0x60000000 for x in r['bank0_live_in']}),boundary_comparisons=boundaries,target_invocations={hex(a):n for a,n in sorted(targets.items())},queue_flushes=sum(r['queue_flushes'] for r in records),queue_events=sum(r['queue_events'] for r in records),stored_queue_event_prefix=sum(min(4096,r['queue_events']) for r in records),unstored_queue_event_count=sum(max(0,r['queue_events']-4096) for r in records),new_dll_original_initializer_stages=1,new_dll_original_initializer_rows=41,external_game_service_fixtures=0,newly_registered_entries=0,dll_sha256=digest(DLL),sha256={str(p):digest(p) for p in files},scope='Full original-model/native clipper and three real primitive-parent bodies, with explicit canonical physical integer banks, MACH/MACL, full RAM/FPU/FPUL/GBR and ordered entries/software queues. Independent polygon clipping, fixed-point packed colours/specular, signed and environment UVs, packet order/cursor/whole direct-helper RAM footprints and parent fast-path state. Actual native parents build helper inputs. Explicit supplied caches/viewport/normal matrices/QACR are not natural engine startup.',limitations=['Privileged bank-1 normal entry, FPU-enabled CPU state, existing qualified PR/SZ/rounding/scalar/vector domains and MMU-off coherent RAM/software queues. Typed guards/traps are checked stops; exception-vector delivery/interrupt/timing/hardware execution remain unproved.','The shared polygon writer 8c037ff4 is verified through original native tail-transfer contexts with the existing outer frame, not an invented standalone ordinary-call ABI.','Boundary equality preserves original classification, fixed packet emission and untouched scratch-record reuse; no idealized clipping replacement. Independent rational numeric checks treat signed zeros as equal; original bit-exact machine comparisons separately retain their signs.','No initial inactive-bank value was read before being written in these tested bank-1 paths. This dynamic observation is not an exhaustive static liveness theorem.','Direct and primitive-parent fixtures use supplied projected caches, UV/normal records, viewport and QACR=12. The current DLL initializer is separately checked once. Full object/camera/startup/frame/GPU, high-FPS/8K and ports remain incomplete.'],cases=records)
 assert len(records)==682 and report['rows_covered']==1739 and report['conditional_edges_covered']==138 and not report['normal_bank1_live_in']
 assert all(not x['missing_rows'] and not x['missing_edges'] for x in coverage_rows)
 (PROJECT/'analysis/draw_clipping_complete_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','rows_covered','conditional_edges_covered','status_counts','independent_cases','independent_checks','normal_bank1_live_in','boundary_comparisons','queue_flushes')},indent=2))
if __name__=='__main__':main()
