"""Original background context compiler, independent packets and context tags."""
import collections,csv,ctypes,itertools,json,sys,types
from pathlib import Path
import verify_draw_frame_setup as previous
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent;MANIFEST=PROJECT/'analysis/draw_frame_context_translation.json';manifest=json.loads(MANIFEST.read_text())
COMPILE,CLASSIFY,TAG=0x8c661300,0x8c66118a,0x8c661160
ROWS=dict(previous.ROWS);OWN={};PER_BODY={}
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);OWN.update(body);PER_BODY[int(routine['address'],16)]=body
namespace=dict(previous.namespace,ROWS=ROWS,RETURNS=previous.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))})
exec(compile(previous.previous.code,'frame_context_original_model','exec'),namespace)
effects=types.SimpleNamespace(**previous.effects.__dict__);effects.CONNECTED=previous.effects.CONNECTED|{COMPILE,CLASSIFY,TAG};effects.namespace=namespace;effects.reference_run=namespace['reference_run']
effects.DLL=previous.previous.previous.previous.SEED.ROOT/'work/sonic-adventure/build/draw_frame_context.dll';effects.LIB=ctypes.CDLL(str(effects.DLL));effects.fn=effects.LIB.sa_draw_frame_context_connected;effects.fn.argtypes=previous.effects.fn.argtypes;effects.fn.restype=ctypes.c_int;effects.LIB.sa_render_register_bus_init.argtypes=previous.effects.LIB.sa_render_register_bus_init.argtypes
compare_state=types.FunctionType(previous.previous.compare.__code__,dict(previous.previous.__dict__,namespace=namespace,effects=effects),'compare_state')
RECORD,VERTICES=0x8cd10000,0x8cd20000

def classify(word):
 if not word&0x2000000:return 1
 return 2+bool(word&0x1000000)+int(not word&0x400000)

def prepare(ram,c):
 _,s,f=previous.previous.previous.previous.SEED.prepare(ram,dict(entry=c['entry'],mode=c.get('mode',0)));m=bytearray(ram);s.pc=c['entry'];s.sr=0x600003f3;c['gbr']=0;previous.previous.machine.extra(c,c.get('seed',0))
 s.r[4]=c.get('word',0);s.r[5]=c.get('flag5',0);s.r[6]=c.get('flag6',0)
 if c['entry']==COMPILE:
  s.r[4]=RECORD
  for i in range(3):
   pointer=VERTICES+i*64;store(m,RECORD+i*4,pointer)
   for j in range(8):store(m,pointer+j*4,0x10203040+i*0x11110000+j*0x01020304+c.get('seed',0))
  for literal,value in ((0x8c6615e0,c.get('word',0)),(0x8c6615e4,0x12345678),(0x8c6615e8,0x9abcdef0)):store(m,load(m,literal),value)
 if c.get('alias'):
  s.r[15]=(s.r[15]&0xffffff)|c['alias']
  if c['entry']==COMPILE:s.r[4]=(s.r[4]&0xffffff)|c['alias']
 if c.get('bad')=='source':s.r[4]=0x8cfffffc
 if c.get('bad')=='stack':s.r[15]=0
 if c.get('bad')=='vertex':store(m,RECORD,0x8cfffffc)
 return m,s,f

def semantic(m,actual,initial,final,c):
 wanted=bytearray(m);state=load(m,0x8c661280)
 if initial.pc==CLASSIFY:
  count=classify(initial.r[4]);assert actual==m and final.r[0]==final.r[5]==count and final.r[15]==initial.r[15] and final.pr==initial.pr,(c,'independent context classification/footprint')
  return 1
 if initial.pc==TAG:
  encoded=(initial.r[4]&7)<<24
  if initial.r[6]:encoded|=load(m,0x8c661284)
  if initial.r[5]:encoded|=load(m,0x8c661288)
  store(wanted,state,encoded)
  assert actual==wanted and final.r[4]==(initial.r[4]&7)<<24 and final.sr==(initial.sr&~1)|int(initial.r[5]==0) and final.pr==initial.pr and final.r[15]==initial.r[15],(c,'independent context tag/flags/T-bit and full footprint')
  return 1
 pointers=[load(m,initial.r[4]+i*4) for i in range(3)]
 header=[load(m,load(m,literal)) for literal in (0x8c6615e0,0x8c6615e4,0x8c6615e8)]
 for offset,value in ((-4,initial.r[14]),(-8,initial.r[13]),(-12,initial.pr),(-24,pointers[0]),(-20,pointers[1]),(-16,pointers[2]),(-28,header[1])):store(wanted,initial.r[15]+offset,value)
 output=load(m,0x8c6615ec)
 for i,value in enumerate(header):store(wanted,output+i*4,value)
 cursor=3
 for pointer in pointers:
  for off in (4,8,12,24):store(wanted,output+cursor*4,load(wanted,pointer+off));cursor+=1
 store(wanted,load(m,0x8c6615f0),60);store(wanted,state,(classify(header[0])&7)<<24);store(wanted,load(m,0x8c6615f4),load(wanted,pointers[0]+12))
 assert actual==wanted,(c,'independent 15-word compiled packet, size/tag/depth and whole stack/RAM footprint')
 assert final.r[0]==0 and final.r[13]==initial.r[13] and final.r[14]==initial.r[14] and final.r[15]==initial.r[15] and final.pr==initial.pr,(c,'independent compiler preserved state')
 return 2

def cases():
 for word,bank in itertools.product((sum(mask for i,mask in enumerate((0x2000000,0x1000000,0x400000)) if bits>>i&1) for bits in range(8)),range(2)):
  yield dict(entry=CLASSIFY,word=word,mode=bank*4)
  yield dict(entry=COMPILE,word=word,mode=bank*4,seed=bank)
 for word,f5,f6,bank in itertools.product((0,1,7,8,0xffffffff),(0,1),(0,1),range(2)):yield dict(entry=TAG,word=word,flag5=f5,flag6=f6,mode=bank*4)
 for entry,mode in itertools.product((COMPILE,CLASSIFY,TAG),range(8)):yield dict(entry=entry,mode=mode)
 for alias,bank in itertools.product((0x0c000000,0xac000000),range(2)):yield dict(entry=COMPILE,alias=alias,mode=bank*4,word=0x3400000)
 for bad in ('source','stack','vertex'):yield dict(entry=COMPILE,bad=bad,mode=0)

def main():
 proofs=[MANIFEST,PROJECT/'analysis/draw_frame_context_connection.json',PROJECT/'analysis/draw_frame_setup_verification.json']
 for p in proofs:
  for f,h in json.loads(p.read_text())['sha256'].items():assert previous.previous.machine.digest(f)==h,f
 ram=bytearray(16<<20)
 for p,offset in ((previous.previous.previous.previous.SEED.state.gate.support.morph.MAIN,0x10000),(previous.previous.previous.previous.SEED.state.gate.support.morph.MODULE,0x900000)):
  raw=p.read_bytes();ram[offset:offset+len(raw)]=raw
 selected=list(cases())
 if '--smoke' in sys.argv:selected=selected[:4]
 records=[];coverage=set();edges=set();targets=collections.Counter()
 for i,c in enumerate(selected):
  m,s,f=prepare(ram,c);initial=effects.State.from_buffer_copy(s);r,path,actual,final,*_=compare_state(m,s,f,c)
  r['independent_checks']=semantic(m,actual,initial,final,c) if r['status']==0 else 0;records.append(r);coverage.update(path);targets.update(a for a in path if a in effects.CONNECTED)
  for j,a in enumerate(path[:-1]):
   if a not in OWN:continue
   w=int.from_bytes(ram[a&0xffffff:(a&0xffffff)+2],'little')
   if w>>8 in (0x89,0x8b,0x8d,0x8f):
    k=j+(2 if w>>8 in (0x8d,0x8f) else 1)
    if k<len(path):edges.add((a,path[k]))
  if (i+1)%32==0:print(i+1,'compiler/context comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Compiler/context smoke passed:',len(records));return
 coverage_rows=[]
 for routine in manifest['routines']:
  a=int(routine['address'],16);wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']};body=set(PER_BODY[a])
  coverage_rows.append(dict(address=hex(a),rows_covered=len(body&coverage),static_rows=len(body),missing_rows=[hex(x) for x in sorted(body-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(x),hex(y)] for x,y in sorted(wanted-edges)]))
 files=[Path(__file__),PROJECT/'verify_draw_frame_setup.py',PROJECT/'verify_draw_view_registers.py']+proofs
 for p in proofs:files.extend(Path(f) for f in json.loads(p.read_text())['sha256'])
 report=dict(source=manifest['source'],source_sha256=previous.previous.machine.digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),rows_covered=len(set(OWN)&coverage),conditional_edges_covered=sum(r['conditional_edges_covered'] for r in coverage_rows),coverage_by_routine=coverage_rows,independent_cases=sum(bool(r['independent_checks']) for r in records),independent_checks=sum(r['independent_checks'] for r in records),target_invocations={hex(a):n for a,n in sorted(targets.items())},boundary_comparisons=sum(r['boundary_comparisons'] for r in records),register_write_events=sum(len(r['register_writes']) for r in records),queue_events=sum(r['queue_events'] for r in records),newly_registered_entries=0,external_game_service_fixtures=0,dll_sha256=previous.previous.machine.digest(effects.DLL),sha256={str(p):previous.previous.machine.digest(p) for p in files},scope='Complete original compiler-table callback and two context helpers. All 166 rows/ten conditional outcomes, full machine/RAM/banks/MACH/GBR/register storage and ordered boundaries. Independent 15-word packet/size/type/depth and full stack/RAM footprints plus exact list classification and encoded tag/flags/T-bit.',limitations=['Direct valid three-vertex records and raw header/position/colour words. This is original RAM command compilation, not physical GPU consumption/pixels.','Integer-only bodies preserve the original raw floating bit patterns in RAM. Both floating banks and all tested PR/SZ modes retain full original/native state; normal CPU entry privileged bank 1 with FD=0.','Typed invalid source/stack/vertex stops preserve reached partial state; complete hardware exception delivery remains unproved.','Other compiler table entries, whole boot/loading/scheduling, full gameplay/high-FPS/8K and ports remain incomplete.'],cases=records)
 assert len(records)==103 and report['status_counts']=={0:100,-3:3} and report['rows_covered']==166 and report['conditional_edges_covered']==10
 assert all(r['rows_covered']==r['static_rows'] and not r['missing_rows'] and not r['missing_edges'] for r in coverage_rows)
 assert report['independent_cases']==100 and report['independent_checks']==128 and report['register_write_events']==report['queue_events']==0
 (PROJECT/'analysis/draw_frame_context_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','rows_covered','conditional_edges_covered','coverage_by_routine','independent_cases','independent_checks')},indent=2))
if __name__=='__main__':main()
