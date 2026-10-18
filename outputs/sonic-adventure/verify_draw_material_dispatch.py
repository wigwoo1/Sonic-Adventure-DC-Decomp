"""Full original material dispatcher through actual native primitive children."""
import collections,itertools,json,sys
from pathlib import Path
import verify_overlay_draw_clipping as machine
import verify_draw_primitive_stream as fixtures
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent;effects=machine.effects
ENTRY=0x8c0376d0;MODEL=fixtures.MODEL;MATERIAL=fixtures.MATERIAL;DESC=fixtures.DESC
manifest=json.loads((PROJECT/'analysis/draw_geometry_stream_translation.json').read_text())
routine=next(r for r in manifest['routines'] if int(r['address'],16)==ENTRY)
ROWS=fixtures.geometry.PER_BODY[ENTRY]

def prepare(ram,c):
 # The original descriptor's top two bits select [3,4,0,0]: fixed strips have
 # no count word; the two zero entries read strip lengths from the stream.
 code=c.get('strip_code',0);length=4 if code==1 else 3
 m,s,f=fixtures.prepare(ram,dict(c,override=length if code<2 else 0,lengths=(length,)));s.pc=ENTRY;s.sr=0x600003f3;s.r[4]=MODEL;s.r[13]=int(c.get('clipping',False));c['gbr']=load(m,effects.ORIGINAL_GBR_LITERAL);machine.extra(c,c.get('seed',0))
 store(m,MODEL+12,DESC);store(m,MODEL+16,MATERIAL);store(m,MODEL+20,c.get('blocks',1),2)
 store(m,DESC,code<<14,2)
 descriptor=bytes(m[DESC&0xffffff:(DESC&0xffffff)+24])
 for i in range(1,c.get('blocks',1)):m[(DESC&0xffffff)+i*24:(DESC&0xffffff)+(i+1)*24]=descriptor
 for off,v in ((0,0x7fffffff),(4,fixtures.POSITIONS),(8,0x10203040),(12,0),(16,c['flags'])):store(m,MATERIAL+off,v)
 for i,v in enumerate((0x80840000,0x12345678,0xa5a5a5a5,0xcafebabe)):store(m,0x8c890104+i*4,v)
 for a,v in ((0x8c890114,fixtures.OUTPUT),(0x8c890118,fixtures.OUTPUT),(0x8c890128,0x0c000000),(0x8c89012c,0x8c890114),(0x8c673480,0x8cd70000),(0x8cd70000,0x8cd71000),(0x8cd71008,0x8cd72000),(0x8cd72000,0x22220000),(0x8cd72008,0xcafebe80),(0x8cd7200c,0xdef00000),(0x8c67348c,0),(0x8c88f668,0)) :store(m,a,v)
 store(m,c['gbr']+0x40,fixtures.SPEC);store(m,c['gbr']+0x28,0)
 for i,v in enumerate(c.get('material_override',(0,0xffffffff,0))):store(m,0x8c8ffe1c+i*4,v)
 if c.get('alias'):
  s.r[4]=(MODEL&0xffffff)|c['alias'];s.r[15]=(s.r[15]&0xffffff)|c['alias'];c['gbr']=(c['gbr']&0xffffff)|c['alias']
 if c.get('bad')=='source':s.r[4]=0x8cfffffc
 if c.get('bad')=='stack':s.r[15]=0
 if c.get('bad')=='descriptor':store(m,MODEL+12,0x8cfffffe)
 if c.get('bad')=='materials':store(m,MODEL+16,0x8cfffffc)
 return m,s,f

def semantic(m,s,actual,final,c,path):
 word=c['flags'];enable,mask,forced=c.get('material_override',(0,0xffffffff,0));effective=(word&mask)|forced if enable else word
 assert load(actual,c['gbr']+0x2c)==effective,(c,'independent effective material word')
 texture=bool(effective&0x200000);alpha=bool(effective&0x100000)
 header=0x8c0375b0 if texture and alpha else 0x8c037538 if texture else 0x8c0374d4 if alpha else 0x8c037460
 # Header selection uses the overridden word; primitive selection reloads the
 # original material word. Keep these two decisions distinct.
 primitive_texture=bool(word&0x200000)
 parent=(fixtures.CLIPPED_UV if primitive_texture else fixtures.CLIPPED_SOLID) if c.get('clipping') else (fixtures.UV if primitive_texture else fixtures.SOLID)
 expected=[0x8c037618]+([0x8c037638] if texture else [])+([0x8c037872] if alpha else [])+[0x8c03767c,header,parent]
 if primitive_texture and word&0x400000 and c.get('clipping'):expected.append(fixtures.ENV_CLIP)
 expected.append(0x8c0376b0)
 relevant={0x8c037618,0x8c037638,0x8c037872,0x8c03767c,0x8c037460,0x8c0374d4,0x8c037538,0x8c0375b0,fixtures.UV,fixtures.SOLID,fixtures.CLIPPED_UV,fixtures.CLIPPED_SOLID,fixtures.ENV_CLIP,0x8c0376b0}
 expected*=c.get('blocks',1)
 assert [a for a in path if a in relevant]==expected,(c,'independent material/header/primitive routing',[hex(a) for a in path if a in relevant],[hex(a) for a in expected])
 assert final.r[15]==s.r[15] and final.pr==s.pr and final.r[7]==0 and final.r[2]==DESC+c.get('blocks',1)*24,(c,'independent dispatcher stack/PR/loop count/descriptor stride')
 return 3

def cases():
 for flag_bits,near,bank in itertools.product(range(16),(False,True),range(2)):
  flags=sum(mask for i,mask in enumerate((0x200000,0x100000,0x2000000,0x400000)) if flag_bits>>i&1)
  yield dict(entry=ENTRY,flags=flags,clipping=near,mode=bank*4,depths=(.5,2,.5),seed=flag_bits)
 for flags,override,near,bank in itertools.product((0,0x300000),( (1,0xffcfffff,0x300000),(1,0xffcfffff,0)),(False,True),range(2)):
  yield dict(entry=ENTRY,flags=flags,material_override=override,clipping=near,mode=bank*4,depths=(.5,2,.5),seed=25)
 for flags,near,bank in itertools.product((0,0x100000,0x200000,0x300000),(False,True),range(2)):
  yield dict(entry=ENTRY,flags=flags,clipping=near,mode=bank*4,depths=(.5,2,.5),blocks=2,seed=31)
 for code,near,bank in itertools.product((1,2,3),(False,True),range(2)):
  yield dict(entry=ENTRY,flags=0x300000,clipping=near,strip_code=code,mode=bank*4,depths=(.5,2,.5),seed=32)
 for alias,bank in itertools.product((0x0c000000,0xac000000),range(2)):
  yield dict(entry=ENTRY,flags=0x300000,clipping=True,alias=alias,mode=bank*4,depths=(.5,2,.5),seed=33)
 for bad in ('source','stack','descriptor','materials'):yield dict(entry=ENTRY,flags=0x300000,bad=bad,mode=0,seed=34)

def main():
 ram=bytearray(16<<20)
 for p,offset in ((effects.SEED.state.gate.support.morph.MAIN,0x10000),(effects.SEED.state.gate.support.morph.MODULE,0x900000)):
  raw=p.read_bytes();ram[offset:offset+len(raw)]=raw
 ram=machine.previous.constructor.prepare(ram,dict(draw_flags=0x10,semantic=True))[0]
 selected=list(cases())
 if '--smoke' in sys.argv:selected=selected[:8]
 records=[];coverage=set();edges=set();targets=collections.Counter()
 for i,c in enumerate(selected):
  m,s,f=prepare(ram,c);initial=effects.State.from_buffer_copy(s)
  r,path,actual,final,*_=machine.compare(m,s,f,c)
  r['independent_checks']=semantic(m,initial,actual,final,c,path) if r['status']==0 else 0
  records.append(r);coverage.update(path);targets.update(a for a in path if a in effects.CONNECTED)
  for j,a in enumerate(path[:-1]):
   if a not in ROWS:continue
   w=int.from_bytes(ram[a&0xffffff:(a&0xffffff)+2],'little')
   if w>>8 in (0x89,0x8b,0x8d,0x8f):
    k=j+(2 if w>>8 in (0x8d,0x8f) else 1)
    if k<len(path):edges.add((a,path[k]))
  print(i+1,'material dispatcher comparison',r['status'],r['stop_pc'],flush=True)
 if '--smoke' in sys.argv:print('Dispatcher smoke passed:',len(records));return
 wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
 files=[Path(__file__),PROJECT/'verify_overlay_draw_clipping.py',PROJECT/'analysis/draw_geometry_stream_translation.json',PROJECT/'analysis/draw_clipping_complete_verification.json']
 for p in files[-2:]:files.extend(Path(f) for f in json.loads(p.read_text())['sha256'])
 report=dict(source=manifest['source'],source_sha256=machine.digest(PROJECT/manifest['source']),routines=[routine],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),rows_covered=len(set(ROWS)&coverage),rows_static=len(ROWS),conditional_edges_covered=len(wanted&edges),missing_rows=[hex(a) for a in sorted(set(ROWS)-coverage)],missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)],independent_checks=sum(r['independent_checks'] for r in records),target_invocations={hex(a):n for a,n in sorted(targets.items())},boundary_comparisons=sum(r['boundary_comparisons'] for r in records),queue_flushes=sum(r['queue_flushes'] for r in records),queue_events=sum(r['queue_events'] for r in records),stored_queue_event_prefix=sum(min(4096,r['queue_events']) for r in records),unstored_queue_event_count=sum(max(0,r['queue_events']-4096) for r in records),newly_registered_entries=0,external_game_service_fixtures=0,dll_sha256=machine.digest(effects.DLL),sha256={str(p):machine.digest(p) for p in files},scope='Complete original material-block dispatcher with actual native header/texture/colour/primitive/clipping children on the preserved 122-source DLL. Full reached/final RAM/machine/banks/MACH/GBR, ordered entry and queue comparisons. Independent overridden header versus original primitive flag routing, loop/descriptor stride and stack checks.',limitations=['Prepared model/material/descriptors, projected caches, normals/UV/texture metadata and runtime globals. The real dispatcher/children execute; natural startup and complete object/camera projection are not proved.','Privileged bank-1 CPU entry with FD=0, inherited numerical/MMU-off/coherent software-queue limits. Memory guards are typed stops, not full exception-vector execution.','First 4096 queue events per invocation are individually compared, later counts and full final RAM/queue/QACR/counters checked. Parent geometry assertions are retained in the separate seven-body clipping proof; this dispatcher adds routing/loop assertions.','Full gameplay, physical GPU, FPS/8K, texture/display settings and playable ports remain incomplete.'],cases=records)
 assert len(records)==116 and report['rows_covered']==149 and report['conditional_edges_covered']==20 and not report['missing_rows'] and not report['missing_edges']
 assert report['status_counts']=={0:112,-3:4} and report['independent_checks']==336
 (PROJECT/'analysis/draw_material_dispatch_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:v for k,v in report.items() if k not in ('routines','cases','sha256','target_invocations')},indent=2))

if __name__=='__main__':main()
