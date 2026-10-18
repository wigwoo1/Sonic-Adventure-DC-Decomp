"""Expanded original renderer state/header bodies and independent packing checks."""
import collections,csv,ctypes,hashlib,itertools,json,random,struct,sys
from fractions import Fraction
from pathlib import Path
import verify_draw_geometry_stream as previous
import verify_overlay_draw_geometry_stream as caller
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent;SEED=previous.SEED
SETUP,SELECT,CHANNEL,CHANNEL_SELECT=0x8c03fe1a,0x8c03fe60,0x8c040b90,0x8c040c02
MANIFEST=PROJECT/'analysis/draw_material_setup_translation.json';manifest=json.loads(MANIFEST.read_text());NEW={int(r['address'],16) for r in manifest['routines']};CONNECTED=previous.CONNECTED|NEW
ROWS=dict(previous.ROWS);OWN={};PER_BODY={}
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};PER_BODY[int(routine['address'],16)]=body;OWN.update(body);ROWS.update(body)
RETURNS=previous.RETURNS|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))}
switch_targets=dict(previous.switch_targets)
namespace=dict(previous.namespace,ROWS=ROWS,RETURNS=RETURNS,LEGACY=previous.namespace['LEGACY']-NEW,switch_targets=switch_targets)
code=previous.code
# The coherent-RAM queue backend explicitly rejects non-RAM flush destinations.
# Preserve the PREF event/counter and exact reached partial state before -3.
marker='  access=dynamic_access(text,r,f.fpscr,special)';assert code.count(marker)==1
code=code.replace(marker,"""  if text.startswith('pref '):
   a=r[int(text.split('r')[-1])]
   if a>>26==0x38:
    q=(a>>5)&1;destination=(a&0x03ffffe0)|((queue_reference.qacr[q]&28)<<24)
    if destination&0x1f000000!=0x0c000000:
     queue_reference.events.append((3,a,0,0));queue_reference.prefetches+=1;stop(pc,r,sr,-3)
"""+marker)
exec(compile(code,'draw_material_setup_reference','exec'),namespace);reference_run=namespace['reference_run']
DLL=SEED.ROOT/'work/sonic-adventure/build/draw_material_setup.dll';LIB=ctypes.CDLL(str(DLL));fn=LIB.sa_draw_material_setup_connected;fn.argtypes=previous.fn.argtypes;fn.restype=ctypes.c_int
LIB.sa_vertex_bus_init.argtypes=previous.LIB.sa_vertex_bus_init.argtypes
State,Fpu,OBSERVE,Memory,Bus,QueueReference=previous.State,previous.Fpu,previous.OBSERVE,previous.Memory,previous.Bus,previous.QueueReference
RAM_TYPE,ram_digest,bits,value=previous.RAM_TYPE,previous.ram_digest,previous.bits,previous.value
import types
INIT=previous.INIT;ORIGINAL_GBR_LITERAL=previous.ORIGINAL_GBR_LITERAL
FLAGS,TEXTURE,MASKS,FINISH,HEADER3,HEADER3_RAW,HEADER4,HEADER4_RAW,ALPHA=0x8c037618,0x8c037638,0x8c03767c,0x8c0376b0,0x8c037460,0x8c0374d4,0x8c037538,0x8c0375b0,0x8c037872
HEADERS={HEADER3,HEADER3_RAW,HEADER4,HEADER4_RAW};MODEL=previous.MODEL;CACHE=previous.CACHE
ROOT_HEADER=0x8c890104;CHAIN,TABLE,RECORDS=0x8cd10000,0x8cd11000,0x8cd12000
CURSOR=0x8cf18000
runtime=types.FunctionType(previous.runtime.__code__,globals(),'runtime')

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def prepare(ram,c):
 m,s,f=previous.prepare(ram,c);m,gbr=runtime(m);c['gbr']=gbr;entry=c['entry'];s.r[4]=MODEL;s.r[5]=RECORDS;s.r[0]=c.get('word',0x12345678)
 for i,v in enumerate((0x80840000,0x12345678,0xa5a5a5a5,0xcafebabe)):store(m,ROOT_HEADER+4*i,v)
 cursor=(CURSOR+c.get('offset',0))&0xffffff|c.get('alias',0x8c000000)
 store(m,0x8c890114,cursor);store(m,0x8c890118,cursor)
 store(m,0x8c890128,0x0c000000);store(m,0x8c89012c,0x8c890114);s.r[14]=c.get('r14',0xe0f18100)
 for i in range(32):store(m,CURSOR+i*4,0xface0000+i)
 store(m,0x8c673480,CHAIN);store(m,CHAIN,TABLE)
 for i in range(4):
  store(m,TABLE+i*12+8,RECORDS+i*16)
  for j,v in enumerate((0x22220000+i,0xabab0000,0xcafebe80+i,0xdef00000+i)):store(m,RECORDS+i*16+j*4,v)
 if entry==TEXTURE:
  slot=c.get('slot',0);s.r[0]=slot;store(m,0x8c67348c,0x22220000+slot if c.get('same') else 0)
 if entry==ALPHA:
  store(m,gbr+0x2c,c.get('flags',0));store(m,gbr+0x28,c.get('fade_sign',0)&0xffffffff);store(m,gbr+0x3c,CACHE)
  store(m,MODEL+8,c.get('count',5));store(m,RECORDS,c.get('alpha',127)<<24)
  store(m,0x8c88f668,bits(c.get('fade',0)))
  for i in range(32):store(m,CACHE+i*16+12,c.get('cached',0x55667788))
 if c.get('bad')=='cache':store(m,gbr+0x3c,0x8cfffffc)
 if c.get('bad')=='material':s.r[5]=0x8cfffffe
 if c.get('bad')=='table':store(m,CHAIN,0x8cfffffc)
 if c.get('bad')=='cursor':store(m,0x8c890114,0x0d000000);store(m,0x8c890118,0x0d000000)
 return m,s,f

def semantic(m,actual,initial,initial_f,s,f,gbr,c,native):
 entry=initial.pc;wanted=bytearray(m);word=c.get('word',0x12345678)
 if entry==FLAGS:
  store(wanted,ROOT_HEADER+8,(load(m,ROOT_HEADER+8)&0x03f800ff)|(word&0xfc07ff00))
 elif entry==MASKS:
  store(wanted,ROOT_HEADER,(load(m,ROOT_HEADER)&0xfdffff00)|(word&0x0200003f));store(wanted,ROOT_HEADER+8,(load(m,ROOT_HEADER+8)&0xffe7ff3f)|(word&0x001800c0))
 elif entry==FINISH:store(wanted,0x8c890114,(c.get('r14',0xe0f18100)&0x03ffffff)|0x0c000000)
 elif entry==TEXTURE:
  slot=c.get('slot',0)
  if not c.get('same'):
   store(wanted,0x8c67348c,0x22220000+slot);store(wanted,ROOT_HEADER+8,(load(m,ROOT_HEADER+8)&0xffffffc0)|((0xcafebe80+slot)&0x3f));store(wanted,ROOT_HEADER+12,0xdef00000+slot)
 elif entry in HEADERS:
  ptr=0x8c890114 if entry in (HEADER3,HEADER4) else 0x8c890118;cursor=load(m,ptr);sq=(cursor&0x03ffffff)|0xe0000000
  store(wanted,0x8c89012c,ptr);store(wanted,0x8c890128,cursor&0xfc000000)
  words=[load(m,ROOT_HEADER+i*4) for i in range(4 if entry in (HEADER4,HEADER4_RAW) else 3)]
  if entry in (HEADER3,HEADER4):words[2]=(words[2]&0x03efffff)|0x20080000
  for i,v in enumerate(words+[0]*(8-len(words))):store(wanted,cursor+i*4,v)
  assert s.r[0]==sq+32 and native.flushes==native.prefetches==1 and list(native.qacr)==[(cursor>>24)&0x1c]*2,(c,'independent header cursor/queue')
 elif entry==ALPHA:
  rounded=lambda x:value(bits(x))
  a=rounded(c.get('alpha',127)*value(load(m,0x8c038f10)));sign=c.get('fade_sign',0)
  if sign>0:a=c.get('fade',0)
  elif sign<0:a=max(0,min(1,rounded(a+c.get('fade',0))))
  a=int(rounded(a*value(load(m,0x8c038f1c))))
  packed=((a<<24)|0xffffff)&0xffffffff if c.get('flags',0)&0x2000000 else a&255
  old=load(m,CACHE+12) if c.get('flags',0)&0x2000000 else load(m,CACHE+12)>>24
  # Original byte load is signed; alpha >=128 cannot match positive FPUL.
  same=(packed==old) if c.get('flags',0)&0x2000000 else (a==(old if old<128 else old-256))
  if not same:
   for i in range(max(1,(c.get('count',5)+3)//4)*4):
    if c.get('flags',0)&0x2000000:store(wanted,CACHE+i*16+12,packed)
    else:store(wanted,CACHE+i*16+15,packed,1)
 else:return 0
 assert actual==wanted,(c,'independent material/header/alpha complete RAM footprint');return 1

def cases():
 for entry in sorted(NEW):yield dict(entry=entry,semantic=True)
 for entry,word in itertools.product((FLAGS,MASKS),(0,1,0x3f,0x1800c0,0x200003f,0xfc07ff00,0xffffffff)):yield dict(entry=entry,word=word,semantic=True)
 for slot,same in itertools.product(range(4),(False,True)):yield dict(entry=TEXTURE,slot=slot,same=same,semantic=True)
 for entry,alias,off in itertools.product(sorted(HEADERS),(0x0c000000,0x8c000000,0xac000000),(0,32)):yield dict(entry=entry,alias=alias,offset=off,semantic=True)
 for flags,alpha,sign,fade,count in itertools.product((0,0x2000000),(0,63,127,255),(-1,0,1),(-1,0,.5,1,2),(0,1,4,5,9)):
  if sign==0 and fade!=0:continue
  if sign>0 and fade not in (0,.5,1):continue
  yield dict(entry=ALPHA,flags=flags,alpha=alpha,fade_sign=sign,fade=fade,count=count,semantic=True)
 for flags,alpha,cached in ((0,63,0x3f123456),(0,255,0xff123456),(0x2000000,63,0x3fffffff),(0x2000000,255,0xffffffff)):
  yield dict(entry=ALPHA,flags=flags,alpha=alpha,cached=cached,semantic=True)
 for entry,mode in itertools.product(sorted(NEW),range(8)):yield dict(entry=entry,mode=mode)
 for entry,bad in itertools.product((TEXTURE,ALPHA,HEADER3),('cache','material','table','cursor')):yield dict(entry=entry,bad=bad)

def compare(m,s,f,c):
 initial=State.from_buffer_copy(s);initial_f=Fpu.from_buffer_copy(f);other=State.from_buffer_copy(s);ef=Fpu.from_buffer_copy(f);actual=bytearray(m);expected=bytearray(m)
 ref=QueueReference();ref.ram=expected;namespace['queue_reference']=ref
 status,path,wanted,expected_macl,expected_gbr=reference_run(expected,other,ef,c,tracked=tuple(CONNECTED))
 native=Memory();bus=Bus();view=RAM_TYPE.from_buffer(actual);LIB.sa_vertex_bus_init(ctypes.byref(native),view,ctypes.byref(bus));ml=ctypes.c_uint32(0x12345678);gb=ctypes.c_uint32(c.get('gbr',0));got=[]
 @OBSERVE
 def observe(ctx,target,mem,fp,sr,r,pr):got.append((target,tuple(r[:16]),sr[0],pr[0],bytes(fp.contents),ml.value,ram_digest(mem),gb.value))
 rc=fn(view,ctypes.byref(s),ctypes.byref(f),ctypes.cast(ctypes.byref(f,Fpu.fpul.offset),SEED.state.gate.support.morph.P),ctypes.byref(ml),initial.pc,None,observe,ctypes.byref(bus),ctypes.byref(gb))
 assert rc==status and got==wanted and actual==expected and bytes(s)==bytes(other) and bytes(f)==bytes(ef) and ml.value==expected_macl and gb.value==expected_gbr,(c,'geometry stream full state',rc,status,hex(s.pc),hex(other.pc),len(got),len(wanted))
 events=[(e.kind,e.address,e.value,e.width) for e in native.trace[:native.trace_count]]
 assert events==ref.events[:4096] and native.trace_dropped==max(0,len(ref.events)-4096) and list(native.queue)==ref.queue and list(native.qacr)==ref.qacr and (native.flushes,native.prefetches,native.invalidates)==(ref.flushes,ref.prefetches,ref.invalidates),(c,'geometry stream ordered queue/cache state')
 checks=semantic(m,actual,initial,initial_f,s,f,gb.value,c,native) if c.get('semantic') and rc==0 and not initial_f.fpscr&0x180000 and not c.get('bad') else 0
 return dict(input=dict(c),status=rc,full_state_match=True,ordered_boundaries_match=True,gbr_final_and_ordered_entries_match=True,queue_match=True,independent_checks=checks,queue_flushes=native.flushes,queue_events=native.trace_count+native.trace_dropped,stop_pc=hex(s.pc),steps=len(path)),path,len(got)

def main():
 proofs=[MANIFEST,PROJECT/'analysis/draw_material_setup_connection.json',PROJECT/'analysis/draw_geometry_stream_verification.json']
 for proof in proofs:
  for p,h in json.loads(proof.read_text())['sha256'].items():assert digest(p)==h,p
 ram=bytearray(16<<20)
 for path,offset in ((SEED.state.gate.support.morph.MAIN,0x10000),(SEED.state.gate.support.morph.MODULE,0x900000)):
  data=path.read_bytes();ram[offset:offset+len(data)]=data
 ram=caller.prepare(ram,dict(draw_flags=0x10,semantic=True))[0]
 selected=list(cases());records=[];coverage=set();edges=set();boundaries=0;targets=collections.Counter()
 if '--smoke' in sys.argv:selected=selected[:9]
 for n,c in enumerate(selected):
  record,path,b=compare(*prepare(ram,c),c);records.append(record);coverage.update(path);boundaries+=b;targets.update(a for a in path if a in CONNECTED)
  for i,a in enumerate(path[:-1]):
   if a not in OWN:continue
   w=int.from_bytes(ram[a&0xffffff:(a&0xffffff)+2],'little')
   if w>>8 in (0x89,0x8b,0x8d,0x8f):
    j=i+(2 if w>>8 in (0x8d,0x8f) else 1)
    if j<len(path):edges.add((a,path[j]))
  if (n+1)%128==0:print(n+1,'geometry stream comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Geometry stream smoke passed:',len(records));return
 coverage_rows=[];registerable=[]
 for routine in manifest['routines']:
  a=int(routine['address'],16);body=set(PER_BODY[a]);wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
  row=dict(address=hex(a),rows_covered=len(body&coverage),static_rows=len(body),missing_rows=[hex(x) for x in sorted(body-coverage)],conditional_edges_covered=len(wanted&edges),static_conditional_edges=len(wanted),missing_edges=[[hex(x),hex(y)] for x,y in sorted(wanted-edges)])
  coverage_rows.append(row);associated=[r for r in records if r['input']['entry']==a]
  if not row['missing_rows'] and not row['missing_edges'] and all(r['status']!=-2 for r in associated):registerable.append(hex(a))
 files=[Path(__file__),PROJECT/'verify_draw_geometry_stream.py',PROJECT/'verify_overlay_draw_geometry_stream.py']+proofs
 for proof in proofs:files.extend(Path(p) for p in json.loads(proof.read_text())['sha256'])
 report=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=coverage_rows,rows_covered=len(set(OWN)&coverage),conditional_edges_covered=len(edges),registerable_entries=registerable,boundary_comparisons=boundaries,independent_cases=sum(bool(r['independent_checks']) for r in records),independent_checks=sum(r['independent_checks'] for r in records),queue_flushes=sum(r['queue_flushes'] for r in records),queue_events=sum(r['queue_events'] for r in records),stored_queue_event_prefix=sum(min(4096,r['queue_events']) for r in records),unstored_queue_event_count=sum(max(0,r['queue_events']-4096) for r in records),target_invocations={hex(a):n for a,n in sorted(targets.items())},unconnected_targets=dict(collections.Counter(r['stop_pc'] for r in records if r['status']==-2)),native_original_renderer_initializer_stages=len(records),external_game_service_fixtures=0,newly_registered_entries=0,dll_sha256=digest(DLL),sha256={str(p):digest(p) for p in files},scope='Nine original material/header/colour-cache helpers connected through real native GBR and RAM/store-queue operations. Full original-model reached/final RAM/machine/GBR and ordered queue/entry comparisons. Independent flag masks, texture cache hit/miss, exact header packets and alpha cache footprints.',limitations=['Direct fixtures supply headers, texture records and colour caches. Original renderer initializer is a separately checked preparation stage; natural startup is unproved.','Original alpha loop processes groups of four including count zero and rounds odd tails up; signed byte-load early-out is retained. Headers flush complete software 32-byte queue blocks, not physical GPU pixels.','Inherited qualified numeric/MMU-off RAM/bus domains remain. Three helpers already have registrations; new integration supplies zero duplicate credit. Full rendering/gameplay/ports unfinished.'],cases=records)
 (PROJECT/'analysis/draw_material_setup_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','rows_covered','conditional_edges_covered','registerable_entries','independent_cases','queue_flushes','unconnected_targets')},indent=2))
if __name__=='__main__':main()
