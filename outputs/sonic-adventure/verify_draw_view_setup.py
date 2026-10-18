"""Original view setup dependencies with complete state and independent footprints."""
import collections,csv,ctypes,itertools,json,random,sys,types
from fractions import Fraction
from pathlib import Path
import verify_draw_clipping_complete as previous
import verify_overlay_draw_clipping as machine
import verify_draw_render_headers as headers
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent
MANIFEST=PROJECT/'analysis/draw_view_setup_translation.json';manifest=json.loads(MANIFEST.read_text())
NEW={int(r['address'],16) for r in manifest['routines']};OWN={};PER_BODY={};ROWS=dict(previous.ROWS)
for r in manifest['routines']:
 body={int(x['address'],16):x['instruction'].lstrip('_') for x in csv.DictReader(Path(r['listing']).open(),delimiter='\t')};PER_BODY[int(r['address'],16)]=body;OWN.update(body);ROWS.update(body)
binary=previous.SEED.state.gate.support.morph.MAIN.read_bytes();offset=0x8c63eeb0-0x8c010000
assert binary[offset:offset+4]==bytes.fromhex('0b000900')
ROWS.update({0x8c63eeb0:'rts',0x8c63eeb2:'nop'})
namespace=dict(previous.namespace,ROWS=ROWS,RETURNS=previous.RETURNS|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=previous.namespace['LEGACY']-NEW)
exec(compile(previous.code,'view_setup_original_reference','exec'),namespace)
effects=types.SimpleNamespace(**previous.__dict__);effects.ROWS=ROWS;effects.OWN=OWN;effects.CONNECTED=previous.CONNECTED|NEW|{0x8c63eeb0};effects.namespace=namespace;effects.reference_run=namespace['reference_run']
effects.DLL=previous.SEED.ROOT/'work/sonic-adventure/build/draw_view_setup.dll';effects.LIB=ctypes.CDLL(str(effects.DLL));effects.fn=effects.LIB.sa_draw_view_setup_connected;effects.fn.argtypes=previous.fn.argtypes;effects.fn.restype=ctypes.c_int;effects.LIB.sa_vertex_bus_init.argtypes=previous.LIB.sa_vertex_bus_init.argtypes
compare_state=types.FunctionType(machine.compare.__code__,dict(machine.__dict__,effects=effects),'compare_state')
RESET,RECORD,PACK,SELECT=0x8c64f548,0x8c605006,0x8c652f66,0x8c604fd0
PARAM=0x8cc50000

def prepare(ram,c):
 m,s,f=headers.prepare(ram,dict(c,entry=headers.HEADER));s.pc=c['entry'];s.sr=0x600003f3;c['gbr']=0;machine.extra(c,c.get('seed',0));param=c.get('param',PARAM)
 s.r[4]=c.get('slot',0) if s.pc==SELECT else param
 if s.pc==SELECT:
  param=(load(m,0x8c6051bc)+c.get('slot',0)*192)&0xffffffff
  # Real shipped/default render records, with explicit valid direct metadata.
  original=bytes(m[PARAM&0xffffff:(PARAM&0xffffff)+0xc0]);m[param&0xffffff:(param&0xffffff)+0xc0]=original
  store(m,param+0x90,headers.control(c.get('index',0),c.get('variant',0)))
 if c.get('colours_raw'):
  for i,x in enumerate(c['colours_raw']):store(m,param+0x70+i*4,x)
 if c.get('alias') and s.pc in (PACK,RECORD):s.r[4]=(param&0xffffff)|c['alias']
 if c.get('bad')=='source':s.r[4]=0x8cfffffc
 if c.get('bad')=='stack':s.r[15]=0
 return m,s,f

def semantic(m,actual,initial,final,c):
 entry=initial.pc;wanted=bytearray(m);param=initial.r[4]
 if entry==RESET:
  # Default matrix/control words and the separate cleared 33-word block.
  base=load(m,0x8c64f5ac);zero=load(m,0x8c64f5a8);clear=load(m,0x8c64f5b8)
  store(wanted,zero,0)
  values=[previous.bits(x) for x in (1,0,0,0,1,previous.value(load(m,0x8c64f5b0)),previous.value(load(m,0x8c64f5b0)),previous.value(load(m,0x8c64f5b0)),1,previous.value(load(m,0x8c64f5b0)),previous.value(load(m,0x8c64f5b0)),previous.value(load(m,0x8c64f5b0)),previous.value(load(m,0x8c64f5b4)))]
  for i,x in enumerate(values):store(wanted,base+i*4,x)
  for i in range(33):store(wanted,clear+i*4,0)
  assert actual==wanted,(c,'independent reset full RAM footprint')
  assert final.r[15]==initial.r[15] and final.pr==initial.pr,(c,'independent reset stack/PR')
  return 2
 if entry==PACK:
  for source,literal in ((0x70,0x8c65300c),(0x80,0x8c653010)):
   packed=0
   for i,shift in enumerate((24,16,8,0)):
    raw=load(wanted,param+source+i*4);x=Fraction(previous.value(raw));scale=Fraction(previous.value(load(m,0x8c653008)))
    product=previous.rounded(x*scale);v=max(-2**31,min(2**31-1,int(product)))&0xffffffff;packed|=(v<<shift)&0xffffffff
   store(wanted,load(m,literal),packed)
  offset=load(m,0x8c653002,2);offset=offset if offset<32768 else offset-65536
  for i,literal in enumerate((0x8c653014,0x8c653018,0x8c65301c)):store(wanted,load(m,literal),load(wanted,param+offset+i*4))
  assert actual==wanted and final.r[0]==0 and final.r[15]==initial.r[15] and final.pr==initial.pr,(c,'independent signed conversion/packed colour/raw tails and RAM footprint')
  return 1
 if entry==RECORD:
  defaults={0:load(m,0x8c6051d0),0xc:1,0x14:6,0x18:1,0x20:1,0x2c:8,0x30:6,0x3c:2,0x5c:4,0x60:3}
  defaults.update({off:previous.bits(1) for off in range(0x70,0x84,4)})
  assert all(load(actual,param+off)==defaults.get(off,0) for off in range(0,0x90,4)),(c,'independent default record fields')
  assert final.r[15]==initial.r[15] and final.pr==initial.pr,(c,'independent default-record tail-call stack/PR')
  return 2
 if entry==SELECT:
  slot=c.get('slot',0);param=(load(m,0x8c6051bc)+slot*192)&0xffffffff
  store(wanted,load(m,0x8c6051c0),param)
  # The wrapper preserves its original slot/PR beneath the real header call.
  store(wanted,initial.r[15]-4,initial.pr);store(wanted,initial.r[15]-8,slot&0xffffffff)
  child=previous.State.from_buffer_copy(initial);child.pc=headers.HEADER;child.r[4]=param;child.r[15]-=8;child.pr=0x8c604ff2
  wanted,*_=headers.header_oracle(wanted,param,child)
  store(wanted,load(m,0x8c6051cc),(load(m,0x8c6051c8)+slot*4)&0xffffffff)
  assert actual==wanted and final.r[15]==initial.r[15] and final.pr==initial.pr,(c,'independent selected record/slot pointers plus full native header footprint')
  return 1
 return 0

def cases():
 for entry,bank,seed in itertools.product(sorted(NEW),range(2),range(2)):yield dict(entry=entry,mode=bank*4,seed=seed)
 for profile,bank in itertools.product(range(3),range(2)):yield dict(entry=PACK,colours=profile,mode=bank*4,seed=3)
 for slot,index,variant,bank in itertools.product((0,1,3), (0,16,32,63),(0,2),range(2)):yield dict(entry=SELECT,slot=slot,index=index,variant=variant,mode=bank*4,seed=4)
 for entry,alias,bank in itertools.product((PACK,RECORD),(0x0c000000,0xac000000),range(2)):yield dict(entry=entry,alias=alias,mode=bank*4,seed=5)
 for entry,mode in itertools.product(sorted(NEW),range(8)):yield dict(entry=entry,mode=mode,probe=True,seed=6)
 for entry,bad in itertools.product((PACK,RECORD),('source','stack')):yield dict(entry=entry,bad=bad,mode=0,probe=True,seed=7)
 values=(-1e30,-8421505,-1,-.5,0,.1,.25,.5,.75,1,1.25,2,8421505,1e30)
 rng=random.Random(0x652f66)
 for i in range(32):
  raw=[previous.bits(rng.choice(values)) for _ in range(8)]
  for bank in range(2):yield dict(entry=PACK,colours_raw=raw,mode=bank*4,seed=100+i)
 binary=previous.SEED.state.gate.support.morph.MAIN.read_bytes()
 for literal,bank in itertools.product((0x8c65300c,0x8c653010),range(2)):
  offset=literal-0x8c010000;dest=int.from_bytes(binary[offset:offset+4],'little')
  yield dict(entry=PACK,param=dest-0x80,colours=1,mode=bank*4,seed=9)

def main():
 proofs=[MANIFEST,PROJECT/'analysis/draw_view_setup_connection.json',PROJECT/'analysis/draw_render_headers_verification.json']
 for p in proofs:
  for f,h in json.loads(p.read_text())['sha256'].items():assert machine.digest(f)==h,f
 ram=bytearray(16<<20)
 for p,offset in ((previous.SEED.state.gate.support.morph.MAIN,0x10000),(previous.SEED.state.gate.support.morph.MODULE,0x900000)):
  raw=p.read_bytes();ram[offset:offset+len(raw)]=raw
 selected=list(cases())
 if '--smoke' in sys.argv:selected=selected[:8]
 records=[];coverage=set();edges=set();targets=collections.Counter()
 for i,c in enumerate(selected):
  m,s,f=prepare(ram,c);initial=previous.State.from_buffer_copy(s);r,path,actual,final,*_=compare_state(m,s,f,c)
  r['independent_checks']=semantic(m,actual,initial,final,c) if r['status']==0 and not c.get('probe') else 0
  records.append(r);coverage.update(path);targets.update(a for a in path if a in effects.CONNECTED)
  for j,a in enumerate(path[:-1]):
   if a not in OWN:continue
   w=int.from_bytes(ram[a&0xffffff:(a&0xffffff)+2],'little')
   if w>>8 in (0x89,0x8b,0x8d,0x8f):
    k=j+(2 if w>>8 in (0x8d,0x8f) else 1)
    if k<len(path):edges.add((a,path[k]))
  print(i+1,'view setup comparison',hex(c['entry']),r['status'],r['stop_pc'],flush=True)
 if '--smoke' in sys.argv:print('View setup smoke passed:',len(records));return
 coverage_rows=[]
 for routine in manifest['routines']:
  a=int(routine['address'],16);body=set(PER_BODY[a]);wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
  coverage_rows.append(dict(address=hex(a),rows_covered=len(body&coverage),static_rows=len(body),missing_rows=[hex(x) for x in sorted(body-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(x),hex(y)] for x,y in sorted(wanted-edges)]))
 files=[Path(__file__),PROJECT/'verify_overlay_draw_clipping.py',PROJECT/'verify_draw_clipping_complete.py',PROJECT/'verify_draw_render_headers.py']+proofs
 for p in proofs:files.extend(Path(f) for f in json.loads(p.read_text())['sha256'])
 report=dict(source=manifest['source'],source_sha256=machine.digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),rows_covered=len(set(OWN)&coverage),conditional_edges_covered=sum(x['conditional_edges_covered'] for x in coverage_rows),coverage_by_routine=coverage_rows,independent_cases=sum(bool(r['independent_checks']) for r in records),independent_checks=sum(r['independent_checks'] for r in records),target_invocations={hex(a):n for a,n in sorted(targets.items())},boundary_comparisons=sum(r['boundary_comparisons'] for r in records),queue_events=sum(r['queue_events'] for r in records),queue_flushes=sum(r['queue_flushes'] for r in records),newly_registered_entries=0,external_game_service_fixtures=0,dll_sha256=machine.digest(effects.DLL),sha256={str(p):machine.digest(p) for p in files},scope='Four complete original view-builder dependencies through real native render-state/texture-header children. Full machine/RAM/banks/MACH/GBR/ordered boundary and queue comparisons. Independent default-record fields, reset whole-RAM footprint, exact staged signed-FTRC packed colours/raw tails with sequential overlap, slot-pointer arithmetic and composed whole native-header footprint.',limitations=['Direct prepared input records and selected slots; full view builder, natural initialization, loading, frames and GPU pixels are not proved.','Privileged integer bank-1 entry and FPU enabled, inherited qualified numerical/MMU-off/coherent software-queue domains. PR/SZ guards and typed memory stops retain reached partial state, not full exception delivery.','The four bodies contain no new GPU/MMIO or queue output. Reused native render-state/header bodies operate on RAM; previously verified geometry/header proofs retain separate scopes.','Reset/packing/slot selection have independent complete RAM-footprint checks; record initialization independently checks all first 36 words and stack/PR while the full original-machine comparison checks remaining child side effects.','Full game, PC FPS/8K and settings, SUPIR textures, Android/stereo-3DS and later demakes remain incomplete.'],cases=records)
 assert len(records)==182 and report['rows_covered']==217 and report['conditional_edges_covered']==2
 assert report['status_counts']=={0:167,-1:12,-3:3} and report['independent_cases']==146 and report['independent_checks']==158
 assert all(not r['missing_rows'] and not r['missing_edges'] for r in coverage_rows)
 assert report['queue_events']==report['queue_flushes']==0
 (PROJECT/'analysis/draw_view_setup_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','rows_covered','conditional_edges_covered','coverage_by_routine','independent_cases','independent_checks')},indent=2))

if __name__=='__main__':main()
