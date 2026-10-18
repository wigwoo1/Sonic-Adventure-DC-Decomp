"""Original hierarchy bodies, real vector backend and independent matrix/cache checks."""
import collections,csv,ctypes,hashlib,itertools,json,sys
from fractions import Fraction
from pathlib import Path
import verify_draw_motion_mesh as previous
import verify_draw_motion_setup as seed
from verify_overlay_selector_initializer import load,store
PROJECT=Path(__file__).resolve().parent;MANIFEST=PROJECT/'analysis/draw_hierarchy_translation.json';manifest=json.loads(MANIFEST.read_text())
PREP,GEN,COPY,GET,SCALE,SAVE,DIFF=0x8c0583a0,0x8c057bc0,0x8c63a770,0x8c058434,0x8c63a5ec,0x8c638f94,0x8c055d4e
STALL=0x8c05809c;NEW={PREP,GEN,COPY,GET,SCALE,SAVE};CONNECTED=previous.CONNECTED|NEW|{DIFF}
ROWS=dict(previous.ROWS);OWN={}
for r in manifest['routines']:
 for row in csv.DictReader(Path(r['listing']).open(),delimiter='\t'):
  a=int(row['address'],16);OWN[a]=row['instruction'].lstrip('_');ROWS[a]=OWN[a]
for row in csv.DictReader((PROJECT/f'analysis/draw-hierarchy/{DIFF:08x}_instructions.tsv').open(),delimiter='\t'):ROWS[int(row['address'],16)]=row['instruction'].lstrip('_')
RETURNS=previous.RETURNS|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))}
namespace=dict(previous.namespace,ROWS=ROWS,RETURNS=RETURNS,LEGACY=previous.namespace['LEGACY']-NEW)
code=previous.code.replace('path=[];events=[];status=0;','path=[];events=[];status=0;stall_visits=0;')
code=code.replace(' def trace(pc,w,r,sr,pr,fp):',' def trace(pc,w,r,sr,pr,fp):\n  nonlocal stall_visits')
marker='  if pc in tracked:events.append(event(pc,r,sr,pr))';assert code.count(marker)==1
code=code.replace(marker,'''  if pc==STALL:
   stall_visits+=1
   if stall_visits==2:stop(pc,r,sr,-4)
'''+marker).replace('w==0xf3fd and','w in (0xf3fd,0xfbfd) and')
namespace['STALL']=STALL;exec(compile(code,'draw_hierarchy_reference','exec'),namespace);reference_run=namespace['reference_run']
DLL=seed.ROOT/'work/sonic-adventure/build/draw_hierarchy.dll';LIB=ctypes.CDLL(str(DLL));fn=LIB.sa_draw_hierarchy_connected;fn.argtypes=previous.fn.argtypes;fn.restype=ctypes.c_int
Bus,Memory,QueueReference=previous.Bus,previous.Memory,previous.QueueReference;State,Fpu,OBSERVE=seed.State,seed.Fpu,seed.OBSERVE
LIB.sa_vertex_bus_init.argtypes=previous.LIB.sa_vertex_bus_init.argtypes
NODE,PAIR,DESC,DATA,KEYS,INPUT,OUTPUT,CTX,VECTOR=0x8cc00000,0x8cc20000,0x8cc20100,0x8cc30000,0x8cc40000,0x8cc50000,0x8cc60000,0x8cc70000,0x8cc80000
CACHE,COUNT,LIMIT=0x8c789448,0x8c78b448,0x8c78b44c
bits,value=previous.bits,previous.value;RAM_TYPE=ctypes.c_uint8*(16<<20)

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def ram_digest(mem):return hashlib.sha256(RAM_TYPE.from_address(ctypes.addressof(mem.contents))).digest()
def graph(c):return [(1,4),(3,2),(None,None),(None,None),(None,None)] if c.get('tree',True) else [(None,None)]
def order(c):return [0,1,3,2] if c.get('tree',True) else [0]
def key_labels(c):return c.get('labels',(0x80000000,0x80000400,0x80000800,0xffffffff) if c.get('high') else (0,1,3,15,100))
def track_values(node,channel,key,c):
 if channel==1:return tuple(((0x7ff0,0x8010,0xfff0,0x10,0x20)[key]+j*0x100)&0xffff for j in range(3)) if c.get('angles') else (0,0,0)
 if channel==2:return (Fraction(key+2,4),Fraction(1),Fraction(key+3,4))
 return (Fraction(node+key+1,4),Fraction(key-2,4),Fraction(key+1,2))

def prepare(ram,c):
 entry=c.get('entry',PREP);m,s,f=seed.prepare(ram,dict(entry=entry,bank=c.get('bank',0),mode=c.get('fpu_mode',c.get('bank',0)*4),rm=c.get('rm',0),enable=c.get('enable',0)))
 s.pc=entry;layout=c.get('layout',3);typ=c.get('type',(1<<layout)-1);request=c.get('request',3 if c.get('tree',True) else 0);s.r[4]=INPUT if c.get('input') else 0;s.r[5]=PAIR;s.r[6]=request;s.r[7]=OUTPUT if c.get('output') else 0
 f.banks[f.fpscr>>21&1][4]=bits(c.get('frame',Fraction(1,2)))
 for i,(child,sibling) in enumerate(graph(c)):
  a=NODE+i*0x100;store(m,a,c.get('flags',0));store(m,a+4,0)
  for j,v in enumerate((Fraction(i+1,4),Fraction(-1,2),Fraction(3,4))):store(m,a+8+j*4,bits(v))
  for j,v in enumerate((0x1000,0xe000,0x800) if c.get('angles') else (0,0,0)):store(m,a+20+j*4,v)
  for j,v in enumerate((Fraction(1,2),1,2)):store(m,a+32+j*4,bits(v))
  store(m,a+44,NODE+child*0x100 if child is not None else 0);store(m,a+48,NODE+sibling*0x100 if sibling is not None else 0)
 for ordinal,node in enumerate(order(c)):
  for channel in range(layout):
   key=KEYS+(ordinal*3+channel)*0x100
   for k,label in enumerate(key_labels(c)):
    store(m,key+k*16,label)
    for j,v in enumerate(track_values(node,channel,k,c)):store(m,key+k*16+4+j*4,int(v)&0xffffffff if channel==1 else bits(v))
   store(m,DATA+ordinal*layout*8+channel*4,key if c.get('mask',0)>>channel&1 else 0);store(m,DATA+ordinal*layout*8+layout*4+channel*4,len(key_labels(c)))
 store(m,PAIR,NODE);store(m,PAIR+4,DESC);store(m,DESC,DATA);store(m,DESC+4,c.get('frame_count',16));store(m,DESC+8,typ,2)
 for i in range(-16,128*16):store(m,CACHE+i*4,0x3e000000+(i&0xffff))
 store(m,COUNT,0);store(m,LIMIT,c.get('limit',4));store(m,CTX,DATA);store(m,CTX+4,CACHE);store(m,CTX+8,typ);store(m,CTX+12,c.get('frame_count',16));store(m,CTX+16,bits(c.get('frame',Fraction(1,2))))
 matrix=[Fraction(1 if i%5==0 else 0) for i in range(16)]
 if c.get('input'):matrix[5]=Fraction(1,2);matrix[10]=2;matrix[12:15]=[Fraction(1,2),Fraction(1,4),Fraction(-1,2)]
 for i,v in enumerate(matrix):store(m,INPUT+i*4,bits(v));store(m,OUTPUT+i*4,0xa5000000+i)
 for j,v in enumerate((Fraction(1,2),1,2)):store(m,VECTOR+j*4,bits(v))
 if entry==GEN:
  s.r[4]=NODE;s.r[5]=CTX
  if c.get('input'):f.banks[(f.fpscr>>21&1)^1][:]=[bits(v) for v in matrix]
 elif entry==COPY:
  s.r[4]=0 if c.get('implicit') else INPUT+c.get('overlap',0);s.r[5]=INPUT
  for i in range(64):store(m,INPUT+i*4,(0x01020304+i*0x01010101)&0xffffffff)
 elif entry==GET:
  store(m,COUNT,c.get('count',4));s.r[4]=c.get('index',0);s.r[5]=0 if c.get('implicit') else OUTPUT
 elif entry==SCALE:s.r[4]=INPUT if c.get('explicit') else 0;s.r[5]=VECTOR
 elif entry==SAVE:s.r[4]=OUTPUT
 if c.get('bad')=='stack':s.r[15]=0
 if c.get('bad')=='source':s.r[5]=0x8cfffffc
 if c.get('bad')=='root':s.r[4]=0 if entry==GEN else s.r[4];store(m,PAIR,0)
 if c.get('bad')=='destination':s.r[4]=0x8cfffffc if entry in (COPY,SCALE,SAVE) else s.r[4]
 return m,s,f

def factor(c):
 frame=Fraction(c.get('frame',Fraction(1,2)));labels=key_labels(c);i=max((i for i,k in enumerate(labels[:-1]) if k<=int(frame)),default=0)
 return i,(frame-labels[i]) if labels[i]==15 else (frame-labels[i])/(labels[i+1]-labels[i])
def sampled(node,channel,c):
 i,t=factor(c);j=0 if key_labels(c)[i]==15 else i+1;a,b=track_values(node,channel,i,c),track_values(node,channel,j,c)
 if channel==1:return [(x+int((((y-x+0x8000)&0xffff)-0x8000)*t))&0xffffffff for x,y in zip(a,b)]
 return [x+(y-x)*t for x,y in zip(a,b)]

def worlds(c):
 layout=c.get('layout',3);mask=c.get('mask',0);flags=c.get('flags',0);result=[]
 scale=[Fraction(1),Fraction(1,2) if c.get('input') else Fraction(1),Fraction(2) if c.get('input') else Fraction(1)];translation=[Fraction(1,2),Fraction(1,4),Fraction(-1,2)] if c.get('input') else [Fraction(0)]*3
 maximum=min(len(order(c)),c.get('limit',4)) if c.get('entry')==GEN else min(len(order(c)),min(128,(c.get('request',3 if c.get('tree',True) else 0)+1)&0xffffffff))
 def visit(i,parent_scale,parent_translation,siblings=True):
  while i is not None and len(result)<maximum:
   pos=sampled(i,0,c) if mask&1 else [Fraction(i+1,4),Fraction(-1,2),Fraction(3,4)] if layout==2 or not flags&1 else [Fraction(0)]*3
   local_scale=sampled(i,2,c) if layout==3 and mask&4 else [Fraction(1,2),Fraction(1),Fraction(2)] if not flags&4 else [Fraction(1)]*3
   ws=[a*b for a,b in zip(parent_scale,local_scale)];wt=[a+b*d for a,b,d in zip(parent_translation,parent_scale,pos)]
   result.append([bits(ws[k//4]) if k<12 and k%4==k//4 else bits(wt[k-12]) if 12<=k<15 else bits(1) if k==15 else bits(0) for k in range(16)])
   child,sibling=graph(c)[i]
   if child is not None:visit(child,ws,wt)
   i=sibling if siblings else None
 visit(0,scale,translation,False);return result

def compare(m,s,f,c):
 entry=s.pc;other=State.from_buffer_copy(s);ef=Fpu.from_buffer_copy(f);actual=bytearray(m);expected=bytearray(m);ref=QueueReference();ref.ram=expected;namespace['queue_reference']=ref
 status,path,wanted,expected_macl=reference_run(expected,other,ef,c,tracked=tuple(CONNECTED));native=Memory();bus=Bus();view=RAM_TYPE.from_buffer(actual);LIB.sa_vertex_bus_init(ctypes.byref(native),view,ctypes.byref(bus));got=[];errors=[];angles=[];ml=ctypes.c_uint32(0x12345678)
 @OBSERVE
 def observe(ctx,target,mem,fp,sr,r,pr):
  try:
   got.append((target,tuple(r[:16]),sr[0],pr[0],bytes(fp.contents),ml.value,ram_digest(mem)))
   if target==0x8c639cf4 and c.get('semantic') and c.get('mask',0)&2 and not c.get('high'):
    i=(r[13]-NODE)//0x100;want=sampled(i,1,c);assert list(r[5:8])==want,(c,'independent shortest-arc angles',list(r[5:8]),want);angles.append(True)
  except Exception as exc:errors.append(repr(exc))
 rc=fn(view,ctypes.byref(s),ctypes.byref(f),ctypes.cast(ctypes.byref(f,Fpu.fpul.offset),seed.state.gate.support.morph.P),ctypes.byref(ml),entry,None,observe,ctypes.byref(bus))
 assert not errors and rc==status and got==wanted and actual==expected and bytes(s)==bytes(other) and bytes(f)==bytes(ef) and ml.value==expected_macl,(c,'full original hierarchy state',rc,status,hex(s.pc),hex(other.pc),errors,len(got),len(wanted))
 assert not native.trace_count and not ref.events,(c,'hierarchy must not fabricate render packets')
 semantic=False;matrix_checks=0
 if c.get('semantic') and not rc and not c.get('fpu_mode',0)&3:
  if entry in (PREP,GEN) and not c.get('angles') and not c.get('high'):
   expected_worlds=worlds(c);matrix_checks=len(expected_worlds)
   assert load(actual,COUNT)==len(expected_worlds),(c,'independent count/limit')
   for i,w in enumerate(expected_worlds):assert [load(actual,CACHE+i*64+j*4) for j in range(16)]==w,(c,'independent generated world matrix',i)
   end=(CACHE&0xffffff)+len(expected_worlds)*64
   assert actual[end:(CACHE&0xffffff)+8192]==m[end:(CACHE&0xffffff)+8192],(c,'independent cache tail preservation')
   assert load(actual,0x8c88f69c)==1 and load(actual,0x8c88f5f8)==seed.STACK,(c,'independent stack restoration')
   identity=[bits(1 if i%5==0 else 0) for i in range(16)]
   assert list(f.banks[(f.fpscr>>21&1)^1])==identity,(c,'independent caller matrix restoration')
   if entry==PREP:
    assert s.r[0]==1
    if c.get('output'):
     index=c.get('request',3 if c.get('tree',True) else 0)
     want=expected_worlds[index] if index<len(expected_worlds) else [load(m,CACHE+index*64+i*4) for i in range(16)]
     assert [load(actual,OUTPUT+i*4) for i in range(16)]==want,(c,'independent selected matrix output')
   else:assert s.r[0]==(NODE+0x400 if expected_worlds and c.get('tree',True) else 0),(c,'independent sibling/limit result')
  elif entry==COPY and not f.fpscr&0x180000:
   oracle=bytearray(m)
   if c.get('implicit'):assert list(f.banks[(f.fpscr>>21&1)^1])==[load(m,INPUT+i*4) for i in range(16)]
   else:
    destination=INPUT+c.get('overlap',0)
    for half in range(2):
     chunk=bytes(oracle[(INPUT&0xffffff)+half*32:(INPUT&0xffffff)+(half+1)*32]);at=(destination&0xffffff)+half*32;oracle[at:at+32]=chunk
    assert actual==oracle,(c,'independent original two-block overlap copy')
  elif entry==SAVE and not f.fpscr&0x180000:assert [load(actual,OUTPUT+i*4) for i in range(16)]==list(ef.banks[(ef.fpscr>>21&1)^1]),(c,'independent bank serialization')
  elif entry==GET and not f.fpscr&0x180000:
   accepted=c.get('index',0)<c.get('count',4);assert s.r[0]==int(accepted)
   if accepted:
    have=list(f.banks[(f.fpscr>>21&1)^1]) if c.get('implicit') else [load(actual,OUTPUT+i*4) for i in range(16)];assert have==[load(m,CACHE+c.get('index',0)*64+i*4) for i in range(16)],(c,'independent indexed retrieval')
  elif entry==SCALE and not f.fpscr&0x180000:
   matrix=[load(m,INPUT+i*4) for i in range(16)] if c.get('explicit') else [bits(1 if i%5==0 else 0) for i in range(16)]
   want=[bits(Fraction(value(x))*((Fraction(1,2),1,2)[i//4] if i<12 else 1)) for i,x in enumerate(matrix)]
   have=[load(actual,INPUT+i*4) for i in range(16)] if c.get('explicit') else list(f.banks[(f.fpscr>>21&1)^1]);assert have==want,(c,'independent scale')
  else:semantic=bool(angles)
  if entry not in (PREP,GEN) or not c.get('angles'):semantic=True
 return dict(input=c,status=rc,full_state_match=True,ordered_boundaries_match=True,independent_checks=semantic,independent_world_matrices=matrix_checks,independent_angle_handoffs=len(angles),stop_pc=hex(s.pc),steps=len(path)),path,len(got)

def cases():
 for layout,mask,bank,flags in itertools.product((2,3),range(8),range(2),(0,1,2,4,7)):
  yield dict(layout=layout,mask=mask&((1<<layout)-1),bank=bank,flags=flags,semantic=True)
 for entry,layout,frame,limit in itertools.product((PREP,GEN),(2,3),(0,.5,1.5,3,6,9,12,15,15.5),(0,1,2,4)):
  yield dict(entry=entry,layout=layout,mask=7,frame=frame,limit=limit,request=(limit-1)&0xffffffff,semantic=True)
 for layout,bank,phase,input_matrix,output in itertools.product((2,3),range(2),(.5,2,15.5),(False,True),(False,True)):
  yield dict(layout=layout,bank=bank,frame=phase,mask=7,input=input_matrix,output=output,semantic=True)
 for layout,bank,frame in itertools.product((2,3),range(2),(.5,1.5,3,15.5)):yield dict(layout=layout,bank=bank,frame=frame,mask=7,angles=True,semantic=True)
 for typ,entry,bank in itertools.product((0,1,2,4,8,16,32,0x8000),(PREP,GEN),range(2)):yield dict(entry=entry,type=typ,bank=bank,mask=0)
 for entry,mode,implicit in itertools.product((COPY,SAVE,SCALE),range(8),(False,True)):yield dict(entry=entry,fpu_mode=mode,implicit=implicit,explicit=not implicit,semantic=True)
 for overlap,bank in itertools.product((-36,-4,0,4,28,32,36,64),range(2)):yield dict(entry=COPY,overlap=overlap,bank=bank,semantic=True)
 for count,index,implicit,bank in itertools.product((0,1,4),(0,1,3,127,0xffffffff),(False,True),range(2)):yield dict(entry=GET,count=count,index=index,implicit=implicit,bank=bank,semantic=True)
 for entry,bad in itertools.product((PREP,GEN,COPY,SCALE,SAVE,GET),('stack','source','root','destination')):yield dict(entry=entry,bad=bad)
 for entry,mode in itertools.product((PREP,GEN),range(8)):yield dict(entry=entry,fpu_mode=mode,mask=7)
 for layout,entry in itertools.product((2,3),(PREP,GEN)):yield dict(entry=entry,layout=layout,mask=7,high=True,frame=2147484672)
 for layout,entry,terminal in itertools.product((2,3),(PREP,GEN),(False,True)):
  yield dict(entry=entry,layout=layout,mask=7,high=True,frame=2147484672 if terminal else 1,frame_count=2147484673 if terminal else 16,labels=(0x80000000,0x80000400,0x80000800,0xffffffff) if terminal else (0,0x80000000,0xfffff000,0xffffffff))
 for request in (127,128,0xfffffffe):yield dict(request=request,mask=7,semantic=True)
 for layout,mask in itertools.product((2,3),(0,7)):yield dict(layout=layout,mask=mask,tree=False,semantic=True)
 for rm,enable in itertools.product(range(4),(0,0x80,0x800)):yield dict(rm=rm,enable=enable,mask=7,frame=1.5,probe=True)

def main():
 proofs=[MANIFEST,PROJECT/'analysis/draw_hierarchy_connection.json',PROJECT/'analysis/draw_motion_mesh_verification.json']
 for proof in proofs:
  for p,h in json.loads(proof.read_text())['sha256'].items():assert digest(p)==h,p
 binary=seed.state.gate.support.morph.MAIN.read_bytes();ram=bytearray(16<<20);ram[0x10000:0x10000+len(binary)]=binary
 selected=list(cases());records=[];coverage=set();edges=set();targets=collections.Counter();boundaries=0
 if '--smoke' in sys.argv:selected=[dict(layout=2,semantic=True),dict(layout=3,mask=7,input=True,output=True,semantic=True),dict(type=1),dict(entry=COPY,overlap=4,semantic=True),dict(entry=GEN,mask=7,angles=True,semantic=True)]
 for n,c in enumerate(selected):
  m,s,f=prepare(ram,c);record,path,b=compare(m,s,f,c);records.append(record);coverage.update(path);boundaries+=b;targets.update(a for a in path if a in CONNECTED)
  for i,a in enumerate(path[:-1]):
   if a not in OWN:continue
   w=int.from_bytes(binary[a-0x8c010000:a-0x8c010000+2],'little')
   if w>>8 in (0x89,0x8b,0x8d,0x8f):
    j=i+(2 if w>>8 in (0x8d,0x8f) else 1)
    if j<len(path):edges.add((a,path[j]))
  if (n+1)%128==0:print(n+1,'original hierarchy/native numeric comparisons passed',flush=True)
 if '--smoke' in sys.argv:print('Hierarchy native numeric smoke passed:',len(records));return
 unreachable={0x8c057c4e,0x8c057c50,0x8c057d50,0x8c057d52,0x8c057e3e,0x8c057e40,0x8c057f7c,0x8c057f7e}
 assert set(OWN)-coverage==unreachable,list(map(hex,set(OWN)-coverage))
 wanted={tuple(int(a,16) for a in e) for r in manifest['routines'] for e in r['conditional_edges']};missing_edges=wanted-edges
 assert len(missing_edges)==4 and all(b in unreachable for a,b in missing_edges),(missing_edges,edges-wanted)
 files=[Path(__file__),PROJECT/'verify_draw_motion_mesh.py',PROJECT/'verify_draw_motion_setup.py']+proofs
 for proof in proofs:files.extend(Path(p) for p in json.loads(proof.read_text())['sha256'])
 files.append(PROJECT/f'analysis/draw-hierarchy/{DIFF:08x}_instructions.tsv')
 report=dict(source=manifest['source'],source_sha256=digest(PROJECT/manifest['source']),routines=manifest['routines'],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),rows_covered=len(set(OWN)&coverage),static_rows=844,unreachable_rows=list(map(hex,sorted(unreachable))),conditional_edges_covered=len(edges),unreachable_conditional_edges=[[hex(a),hex(b)] for a,b in sorted(missing_edges)],boundary_comparisons=boundaries,independent_cases=sum(r['independent_checks'] for r in records),independent_world_matrices=sum(r['independent_world_matrices'] for r in records),independent_angle_handoffs=sum(r['independent_angle_handoffs'] for r in records),target_invocations={hex(a):n for a,n in sorted(targets.items())},newly_registered_entries=5,existing_entry_extensions=1,reused_routine_credit=0,external_game_service_fixtures=0,retail_stall=manifest['retail_stall'],dll_sha256=digest(DLL),sha256={str(p):digest(p) for p in files},scope='Six complete guarded original hierarchy/matrix bodies, real vector backend and all reached original CPU/FPU/FPUL/MACL/RAM/boundary states. Independent dyadic world/cache/stack, shortest-arc angle, two-block overlapping copy, scale, bank serialization and retrieval checks.',limitations=['Eight fallback rows and four branch outcomes are unreachable for valid nonaliasing stack/data: channel counters are bounded to 0..1 or 0..2 and callees preserve R9. Source retains these rows. Unsupported layouts retain original production stall; research DLL bounds it after one BRA/NOP cycle and compares the exact original finite prefix, not a successful return.','Legacy reused transform/numerical/valid-RAM limits remain. Vector reference shares pinned separately qualified numerical support; exact dyadic and integer checks are independent. No injected vector outputs.','No whole frame/gameplay/natural renderer initialization/GPU pixels/high-FPS or playable-port proof.'],cases=records)
 (PROJECT/'analysis/draw_hierarchy_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({k:report[k] for k in ('cases_passed','status_counts','rows_covered','conditional_edges_covered','boundary_comparisons','independent_cases','independent_world_matrices','independent_angle_handoffs')},indent=2))
if __name__=='__main__':main()
