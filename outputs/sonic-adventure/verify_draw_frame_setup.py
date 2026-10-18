"""Real frame dependency execution and independent matrix-stack initialization."""
import collections,csv,ctypes,itertools,json,sys,types
from pathlib import Path
import verify_draw_view_registers as previous
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent;MANIFEST=PROJECT/'analysis/draw_frame_dependencies_translation.json';manifest=json.loads(MANIFEST.read_text())
NEW={int(r['address'],16) for r in manifest['routines']};ROWS=dict(previous.ROWS);OWN={};PER_BODY={}
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);OWN.update(body);PER_BODY[int(routine['address'],16)]=body
vector_words=set(previous.namespace['VECTOR_WORDS']);binary=previous.previous.previous.SEED.state.gate.support.morph.MAIN.read_bytes()
for a,t in OWN.items():
 if t.startswith(('fsca ','fipr ','ftrv ','fsrra ')):vector_words.add(int.from_bytes(binary[a-0x8c010000:a-0x8c010000+2],'little'))
namespace=dict(previous.namespace,ROWS=ROWS,RETURNS=previous.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=previous.namespace['LEGACY']-NEW,VECTOR_WORDS=vector_words)
exec(compile(previous.code,'frame_dependency_original_model','exec'),namespace)
effects=types.SimpleNamespace(**previous.effects.__dict__);effects.CONNECTED=previous.effects.CONNECTED|NEW;effects.namespace=namespace;effects.reference_run=namespace['reference_run']
effects.DLL=previous.previous.previous.SEED.ROOT/'work/sonic-adventure/build/draw_frame_dependencies.dll';effects.LIB=ctypes.CDLL(str(effects.DLL));effects.fn=effects.LIB.sa_draw_frame_dependencies_connected;effects.fn.argtypes=previous.effects.fn.argtypes;effects.fn.restype=ctypes.c_int;effects.LIB.sa_render_register_bus_init.argtypes=previous.effects.LIB.sa_render_register_bus_init.argtypes
compare_state=types.FunctionType(previous.compare.__code__,dict(previous.__dict__,namespace=namespace,effects=effects),'compare_state')
INIT,SCREEN,CAMERA,FRAME=0x8c638fe4,0x8c640528,0x8c6403b0,0x8c0544e2
BUFFER=0x8cd00000

def prepare(ram,c):
 _,s,f=previous.previous.previous.SEED.prepare(ram,dict(entry=c['entry'],mode=c.get('mode',0)));m=bytearray(ram);s.pc=c['entry'];s.sr=0x600003f3;c['gbr']=0;previous.machine.extra(c,c.get('seed',0))
 s.r[4]=c.get('destination',BUFFER);s.r[5]=c.get('capacity',128);s.r[6]=c.get('flags',0)
 if c['entry'] in (SCREEN,CAMERA):
  for i in range(48):store(m,BUFFER+i*4,0xa5a50000+i)
 if c['entry']==SCREEN:
  for off,x in ((0,320),(4,640),(8,480),(12,c.get('half_width',320)),(16,c.get('half_height',240))):store(m,BUFFER+off,effects.bits(x))
 if c.get('alias'):
  s.r[4]=(s.r[4]&0xffffff)|c['alias'];s.r[15]=(s.r[15]&0xffffff)|c['alias']
 if c.get('bad')=='destination':s.r[4]=0x8cfffffc
 if c.get('bad')=='stack':s.r[15]=0
 return m,s,f

def semantic(m,actual,initial,fp,final,out,c):
 bank=(fp.fpscr>>21)&1;wanted=bytearray(m)
 if initial.pc==INIT:
  d=initial.r[4];source=load(m,0x8c639088)
  for literal,value in ((0x8c639074,1),(0x8c639078,initial.r[6]),(0x8c63907c,initial.r[5]),(0x8c639080,d),(0x8c639084,d)):store(wanted,load(m,literal),value)
  upper=[load(wanted,source+32+i*4) for i in range(8)]
  for i,v in enumerate(upper):store(wanted,d+32+i*4,v)
  lower=[load(wanted,source+i*4) for i in range(8)]
  for i,v in enumerate(lower):store(wanted,d+i*4,v)
  lower[0]^=0x80000000;lower[5]^=0x80000000;store(wanted,d,lower[0]);store(wanted,d+20,lower[5])
  assert actual==wanted,(c,'independent matrix-stack setup whole RAM/sequential template overlap')
  want_f=effects.Fpu.from_buffer_copy(fp);want_f.banks[bank][:8]=lower;want_f.banks[1-bank][:]=[load(wanted,d+i*4) for i in range(16)]
  assert bytes(out)==bytes(want_f),(c,'independent active and opposite floating-bank contents')
  regs=list(initial.r)
  for r,v in ((0,20),(1,source+32),(2,load(m,0x8c639078)),(3,load(m,0x8c639084)),(4,(d+64)&0xffffffff),(7,1)):regs[r]=v
  assert list(final.r)==regs and final.sr==initial.sr and final.pr==initial.pr,(c,'independent full integer/stack/PR setup')
  return 3
 if initial.pc==SCREEN:
  src=initial.r[4];dest=load(m,0x8c64053c)
  for off in (0,12,16):store(wanted,dest+off,load(wanted,src+off))
  assert actual==wanted and final.pr==initial.pr and final.r[15]==initial.r[15],(c,'independent sequential screen-state copy and RAM footprint')
  return 1
 if initial.pc==CAMERA:
  d=initial.r[4];template=load(m,0x8c640414)
  for i in range(14):store(wanted,d+64+i*4,effects.bits(-1 if i in (5,12) else 0))
  high=[load(wanted,template+32+i*4) for i in range(8)]
  first=[load(wanted,template+i*4) for i in range(4)]
  for i,v in enumerate(high):store(wanted,d+32+i*4,v)
  second=[load(wanted,template+16+i*4) for i in range(4)]
  for i,v in enumerate(first+second):store(wanted,d+i*4,v)
  assert actual==wanted,(c,'independent complete camera-record template/default vectors')
  want_f=effects.Fpu.from_buffer_copy(fp);want_f.banks[bank][:12]=high[:4]+second[2:]+second[:2]+first
  assert bytes(out)==bytes(want_f),(c,'independent paired-load floating-register staging')
  regs=list(initial.r);regs[5]=template+32;regs[6]=template+64
  assert list(final.r)==regs and final.sr==initial.sr and final.pr==initial.pr,(c,'independent camera GPR/stack/PR')
  return 3
 return 0

def cases():
 for capacity,flags,bank in itertools.product((0,1,16,128,0xffffffff),(0,1,0x80000000,0xffffffff),range(2)):yield dict(entry=INIT,capacity=capacity,flags=flags,mode=bank*4,seed=capacity&255)
 for entry,bank in itertools.product((SCREEN,CAMERA),range(2)):yield dict(entry=entry,mode=bank*4)
 for entry,alias,bank in itertools.product((INIT,SCREEN,CAMERA),(0x0c000000,0xac000000),range(2)):yield dict(entry=entry,alias=alias,mode=bank*4)
 for delta,bank in itertools.product((-16,0,16),range(2)):
  source=int.from_bytes(binary[0x8c639088-0x8c010000:0x8c639088-0x8c010000+4],'little');destination=source+delta
  assert destination+64<=0x8c638fe4 or destination>=0x8c639072
  yield dict(entry=INIT,destination=destination,mode=bank*4)
 for entry,mode in itertools.product((INIT,SCREEN,CAMERA),range(8)):yield dict(entry=entry,mode=mode,probe=True)
 for entry,bad in itertools.product((INIT,SCREEN,CAMERA),('destination','stack')):yield dict(entry=entry,bad=bad,mode=0,probe=True)

def main():
 proofs=[MANIFEST,PROJECT/'analysis/draw_frame_dependencies_connection.json',PROJECT/'analysis/draw_view_registers_verification.json']
 for p in proofs:
  for f,h in json.loads(p.read_text())['sha256'].items():assert previous.machine.digest(f)==h,f
 ram=bytearray(16<<20)
 for p,offset in ((previous.previous.previous.SEED.state.gate.support.morph.MAIN,0x10000),(previous.previous.previous.SEED.state.gate.support.morph.MODULE,0x900000)):
  raw=p.read_bytes();ram[offset:offset+len(raw)]=raw
 selected=list(cases())
 if '--smoke' in sys.argv:selected=selected[:3]
 records=[];coverage=set();targets=collections.Counter()
 for i,c in enumerate(selected):
  m,s,f=prepare(ram,c);initial=effects.State.from_buffer_copy(s);fp=effects.Fpu.from_buffer_copy(f);r,path,actual,final,out,mem=compare_state(m,s,f,c)
  r['independent_checks']=semantic(m,actual,initial,fp,final,out,c) if r['status']==0 and not c.get('probe') else 0;records.append(r);coverage.update(path);targets.update(a for a in path if a in effects.CONNECTED)
  if (i+1)%32==0:print(i+1,'frame setup comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Matrix/frame setup smoke passed:',len(records));return
 chosen={INIT,SCREEN,CAMERA};routines=[r for r in manifest['routines'] if int(r['address'],16) in chosen]
 coverage_rows=[dict(address=r['address'],rows_covered=len(set(PER_BODY[int(r['address'],16)])&coverage),static_rows=r['instruction_rows'],missing_rows=[hex(a) for a in sorted(set(PER_BODY[int(r['address'],16)])-coverage)]) for r in routines]
 files=[Path(__file__),PROJECT/'verify_draw_view_registers.py']+proofs
 for p in proofs:files.extend(Path(f) for f in json.loads(p.read_text())['sha256'])
 report=dict(source=manifest['source'],source_sha256=previous.machine.digest(PROJECT/manifest['source']),routines=routines,cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),rows_covered=sum(r['rows_covered'] for r in coverage_rows),coverage_by_routine=coverage_rows,independent_cases=sum(bool(r['independent_checks']) for r in records),independent_checks=sum(r['independent_checks'] for r in records),target_invocations={hex(a):n for a,n in sorted(targets.items())},boundary_comparisons=sum(r['boundary_comparisons'] for r in records),queue_events=sum(r['queue_events'] for r in records),register_write_events=sum(len(r['register_writes']) for r in records),newly_registered_entries=0,external_game_service_fixtures=0,dll_sha256=previous.machine.digest(effects.DLL),sha256={str(p):previous.machine.digest(p) for p in files},scope='Three complete original matrix-stack/screen/default-camera helpers: all 131 original rows, full original/native machine/RAM/banks/MACH/GBR/register storage and ordered entry checks. Independent full RAM/FPU/GPR footprints preserve staged template copying, signed default axes, paired moves, float-bank selection and real metadata inputs.',limitations=['Direct prepared destinations/capacity/flags and the shipped original constant templates. Real initializer prefixes are separately checked; natural scheduling/full renderer startup and pixels remain unproved.','Independent formulas cover nominal scalar PR=0/SZ=0 in both floating banks; additional mode and invalid-pointer cases compare exact reached original/native state and typed guards. Normal CPU entry privileged bank 1 with FD=0.','No new GPU or queue writes are introduced by these three helpers. Other compiled dependency proposals remain unregistered until their own complete qualified behavior is proved.','The matrix initializer changes actual stack metadata and floating matrix state; it is not a byte-clear service. Capacity values are stored as originally supplied, without invented validation.','Full decompilation/gameplay, FPS/8K/display/HD texture settings and ports remain unfinished.'],cases=records)
 assert len(records)==92 and report['status_counts']=={0:81,-1:8,-3:3} and report['rows_covered']==131
 assert all(not r['missing_rows'] and r['rows_covered']==r['static_rows'] for r in coverage_rows)
 assert report['independent_cases']==62 and report['independent_checks']==174 and report['queue_events']==report['register_write_events']==0
 (PROJECT/'analysis/draw_frame_setup_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','rows_covered','coverage_by_routine','independent_cases','independent_checks')},indent=2))
if __name__=='__main__':main()
