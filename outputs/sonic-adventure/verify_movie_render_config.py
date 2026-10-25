"""Compare original renderer options, configuration and packing with native C."""
import collections,csv,ctypes,hashlib,itertools,json,random,sys,types
from pathlib import Path
import verify_movie_context as previous
from verify_overlay_selector_initializer import load,store
P=Path(__file__).resolve().parent;ROOT=P.parents[1];MP=P/'analysis/movie_render_config_translation.json';manifest=json.loads(MP.read_text())
CONFIG,FLUSH,IDLE,LAYOUT,TYPE0,TYPE1,TYPE2,OPTION,COMPARE_WRAPPER,COMPARE,PACK,VIDEO,THUNK=(0x8c6526bc,0x8c6525c0,0x8c65796e,0x8c657e00,0x8c6585c0,0x8c658804,0x8c658ae0,0x8c665a80,0x8c10d3c8,0x8c10db1c,0x8c657c60,0x8c6582e0,0x8c6513ee)
NEW={int(r['address'],16) for r in manifest['routines']};ROWS=dict(previous.ROWS);PER_BODY={}
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);PER_BODY[int(routine['address'],16)]=body
namespace=dict(previous.namespace,ROWS=ROWS,RETURNS=previous.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=previous.namespace['LEGACY']-NEW)
code=previous.code;exec(compile(code,'renderer_configuration_original_model','exec'),namespace)
reference_run=types.FunctionType(previous.reference_run.__code__,dict(previous.reference_run.__globals__,raw_reference=namespace['reference_run']),'reference_run')
effects=types.SimpleNamespace(**previous.effects.__dict__);effects.CONNECTED=previous.effects.CONNECTED|NEW;effects.namespace=namespace;effects.reference_run=reference_run;effects.DLL=ROOT/'work/sonic-adventure/build/movie_render_config.dll';effects.LIB=ctypes.CDLL(str(effects.DLL));native_fn=effects.LIB.sa_movie_render_config_connected;native_fn.argtypes=previous.native_fn.argtypes;native_fn.restype=ctypes.c_int
initialized_fn=effects.LIB.sa_movie_render_config_initialized_connected;initialized_fn.argtypes=native_fn.argtypes;initialized_fn.restype=ctypes.c_int;effects.sequence=False
def invoke(*args):return (initialized_fn if effects.sequence else native_fn)(*args)
effects.fn=invoke
for name in ('sa_transfer_context_bus_init','sa_hardware_transfer_set_samples','sa_display_timing_set_samples','sa_disc_queue_init'):
 getattr(effects.LIB,name).argtypes=getattr(previous.effects.LIB,name).argtypes
effects.LIB.sa_hardware_transfer_set_samples.restype=ctypes.c_int;effects.LIB.sa_display_timing_set_samples.restype=ctypes.c_int;effects.LIB.sa_disc_queue_size.restype=ctypes.c_uint32
compare_code=previous.compare_code;exec(compile(compare_code,'renderer_configuration_comparison','exec'),dict(previous._compare.__globals__,effects=effects,namespace=namespace),local:={});_compare=local['compare']
def compare(m,s,f,c):effects.sequence=c.get('initialize_context',False);return _compare(m,s,f,c)
def text(m,address,value):data=value.encode('latin1')+b'\0';m[address&0xffffff:(address&0xffffff)+len(data)]=data
def prepare(ram,c):
 m,s,f=previous.prepare(ram,c)
 if c['entry'] not in NEW:return m,s,f
 if c['entry'] in (COMPARE,COMPARE_WRAPPER,OPTION):
  a=0x8cd80000+c.get('left_alignment',0);b=0x8cd81000+c.get('right_alignment',0)
  m[0xd80000:0xd80200]=bytes(512);m[0xd81000:0xd81200]=bytes(512);text(m,a,c.get('left_text','KAMUI'));text(m,b,c.get('right_text','DisplayMode'));s.r[4]=a;s.r[5]=a if c.get('same_pointer') else b;s.r[6]=c.get('default',0x12345678)
 elif c['entry']==CONFIG:s.r[4]=c.get('mode_bits',0);s.r[5]=c.get('pixel_format',1);s.r[6]=c.get('dither',1);s.r[7]=c.get('antialias',0)
 elif c['entry']==LAYOUT:
  s.r[4]=c.get('screen_width',640);s.r[5]=c.get('screen_height',480);s.r[6]=c.get('pixel_format',1);s.r[7]=c.get('interlace',0);store(m,s.r[15],c.get('dither',1));store(m,s.r[15]+4,c.get('antialias',0))
 elif c['entry'] in (PACK,VIDEO):
  for i,value in enumerate(c['stack_words']):store(m,s.r[15]+i*4,value)
 elif c['entry']==THUNK:s.r[4]=c.get('offset',0x40);s.r[5]=c.get('value',0x12345678)
 if c.get('unknown_callback'):store(m,0x8c8a2fbc,0x8c00f100)
 if c.get('bad_stack'):s.r[15]=0x8c000000
 return m,s,f
def cases():
 for entry,mode,left_align,right_align,length in itertools.product((COMPARE,COMPARE_WRAPPER),(0,4),(0,1),(0,1),(0,1,2,3,4,5,7,8,9,12,16)):
  value='Q'*length;yield dict(entry=entry,mode=mode,left_text=value,right_text=value,left_alignment=left_align,right_alignment=right_align)
 for position,mode in itertools.product(range(16),(0,4)):
  value='Q'*16;other=value[:position]+'R'+value[position+1:]
  for a,b in ((value,other),(other,value)):
   yield dict(entry=COMPARE,mode=mode,left_text=a,right_text=b);yield dict(entry=COMPARE,mode=mode,left_text=a,right_text=b,left_alignment=1,right_alignment=2)
 for left,right,default in itertools.product(('KAMUI','HARDWARE','hardware'),('DisplayMode','EnableTexture16MB','Dither'),(0,1,0x80000000)):yield dict(entry=OPTION,mode=0,left_text=left,right_text=right,default=default)
 for renderer,mode_bits,pixel_format in itertools.product((0,1,2,3),(0,1,4,0x10,0x20,0x2c),(0,1,3,4,5,2)):
  yield dict(entry=CONFIG,mode=(renderer&1)*4,renderer=renderer,mode_bits=mode_bits,pixel_format=pixel_format,dither=mode_bits&1,antialias=(mode_bits>>2)&1,scan_samples=[1,0]*64)
 for entry,mode,flags in itertools.product((TYPE0,TYPE1,TYPE2),(0,4),(0,1,0x10,0x20,0x2c)):yield dict(entry=entry,mode=mode,flags=flags,scan_samples=[1,0]*64)
 for mode,flags,pixel_format,interlace in itertools.product((0,4),(0,1,0x20,0x2c),(0,1,3,4,5),(0,1)):yield dict(entry=LAYOUT,mode=mode,flags=flags,pixel_format=pixel_format,interlace=interlace,screen_height=480,dither=flags&1,antialias=(flags>>2)&1)
 rng=random.Random(PACK)
 for entry,mode,flags in itertools.product((PACK,VIDEO),(0,4),(0,1,2,0x2c)):
  for pattern in (0,0xffffffff,None):yield dict(entry=entry,mode=mode,flags=flags,scan_samples=[1,0]*64,stack_words=[rng.getrandbits(32) if pattern is None else pattern for _ in range(40)])
 for mode,renderer,busy,transfer,irq in itertools.product((0,4),(0,1),(0,1),(0,1),(0,8)):
  yield dict(entry=IDLE,mode=mode,renderer=renderer,render_busy=busy,transfer_busy=transfer,irq_status=irq,ticks=list(range(100,1100)))
 for entry,mode in itertools.product((FLUSH,THUNK),(0,4)):yield dict(entry=entry,mode=mode)
 yield dict(entry=IDLE,mode=0,render_busy=1,unknown_callback=True)
 for entry in (CONFIG,OPTION,COMPARE_WRAPPER):yield dict(entry=entry,mode=0,bad_stack=True,left_text='KAMUI',right_text='DisplayMode')
def semantics(m,a,before,fp,s,f,record,c):
 assert bytes(fp)==bytes(f)
 if record['status']:return 1
 assert s.r[15]==before.r[15] and s.pr==before.pr;checks=3
 if c['entry'] in (COMPARE,COMPARE_WRAPPER):
  left=c.get('left_text','KAMUI').encode('latin1')+b'\0';right=(c.get('left_text','KAMUI') if c.get('same_pointer') else c.get('right_text','DisplayMode')).encode('latin1')+b'\0';i=next((i for i,(x,y) in enumerate(zip(left,right)) if x!=y),None);expected=0 if i is None else left[i]-right[i]
  assert s.r[0]==expected&0xffffffff
  if c['entry']==COMPARE:assert a==m
  checks+=2
 elif c['entry']==OPTION:
  if c['left_text']!='HARDWARE':assert s.r[0]==c['default'];checks+=1
  elif c['right_text']=='EnableTexture16MB':assert s.r[0]==0;checks+=1
 elif c['entry']==CONFIG and c['pixel_format'] not in (0,1,3,4,5):assert s.r[0]==1;checks+=1
 elif c['entry']==IDLE:assert load(a,0x8c8a2f5c)==0 and load(a,0x8c8a2f60)==0;checks+=1
 elif c['entry']==THUNK:assert record['register_writes']==[[7,0xa05f8000+c.get('offset',0x40),c.get('value',0x12345678),4]];checks+=1
 return checks
def coverage_rows(coverage,edges):
 rows=[]
 for routine in manifest['routines']:
  body=PER_BODY[int(routine['address'],16)];wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']};rows.append(dict(address=routine['address'],static_rows=len(body),covered_rows=len(set(body)&coverage),missing_rows=[hex(x) for x in sorted(set(body)-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)]))
 return rows
def main():
 proofs=[MP,P/'analysis/movie_render_config_connection.json',P/'analysis/movie_context_qualification.json']
 for p in proofs:
  for path,h in json.loads(p.read_text())['sha256'].items():assert hashlib.sha256(Path(path).read_bytes()).hexdigest()==h,path
 ram=previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.seed_ram();records=[];coverage=set();edges=set();selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(entry=COMPARE,mode=0,left_text='ABC',right_text='ABD'),dict(entry=OPTION,mode=4,left_text='KAMUI',right_text='DisplayMode',default=1),dict(entry=CONFIG,mode=0,renderer=1,pixel_format=1,mode_bits=0,scan_samples=[1,0]*64)]
 for c in selected:
  m,s,f=prepare(ram,c);before=effects.State.from_buffer_copy(s);fp=effects.Fpu.from_buffer_copy(f);record,path,a,s,f=compare(m,s,f,c);record['independent_checks']=semantics(m,a,before,fp,s,f,record,c);records.append(record);coverage.update(path)
  for j,pc in enumerate(path):
   if pc not in ROWS:continue
   text=ROWS[pc];distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((pc,path[j+distance]))
 rows=coverage_rows(coverage,edges)
 if '--smoke' in sys.argv:print('Renderer configuration smoke passed',len(records),[(r['status'],r['stop_pc']) for r in records]);return
 files=[Path(__file__),Path(previous.__file__)]+proofs
 for p in proofs:files.extend(Path(path) for path in json.loads(p.read_text())['sha256'])
 report=dict(source=manifest['source'],routines=manifest['routines'],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),independent_checks=sum(r['independent_checks'] for r in records),coverage_by_routine=rows,newly_registered_entries=0,dll_sha256=hashlib.sha256(effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Original configuration, option selection/comparison, mode-table construction and register packing. Complete machine/RAM/FPU/banked/special state, call boundaries and software peripheral/input state match independent instructions. Existing native register-read adapter remains unchanged. No service return is fabricated.',limitations=['Direct renderer/global/stack/string fixtures and raw status samples are explicit inputs. Full coverage is evaluated per body; generated proposals are not automatically recovered credit.','Software register writes are not a physical display, native presentation backend, rendered pixels, or verified requested FPS/resolutions/settings. Natural reset/startup, whole game and other ports remain unfinished.'],cases=records)
 (P/'analysis/movie_render_config_verification.json').write_text(json.dumps(report,indent=2)+'\n');print({k:report[k] for k in ('cases_passed','status_counts','independent_checks','coverage_by_routine')})
if __name__=='__main__':main()
