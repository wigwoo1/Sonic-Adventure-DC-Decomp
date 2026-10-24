"""Original raw MOVIE startup runs through the existing native boot services."""
import collections,csv,ctypes,hashlib,json,sys,types
from pathlib import Path
import verify_boot_cache as previous
from verify_overlay_selector_initializer import load,store
P=Path(__file__).resolve().parent;ROOT=P.parents[1];MP=P/'analysis/movie_entry_translation.json';manifest=json.loads(MP.read_text());MAIN_MP=P/'analysis/movie_main_translation.json';main_manifest=json.loads(MAIN_MP.read_text());ENTRY,START=0x8ceb0000,0x8ceb0020;MODULE_ENTRIES={ENTRY,START};routines=manifest['routines']+main_manifest['routines'];NEW={int(r['address'],16) for r in routines};ROWS=dict(previous.ROWS);PER_BODY={}
for routine in routines:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);PER_BODY[int(routine['address'],16)]=body
namespace=dict(previous.namespace,ROWS=ROWS,RETURNS=previous.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=previous.namespace['LEGACY']-NEW)
code=previous.code;exec(compile(code,'movie_original_model','exec'),namespace)
effects=types.SimpleNamespace(**previous.effects.__dict__);effects.CONNECTED=previous.effects.CONNECTED|NEW;effects.namespace=namespace;effects.reference_run=namespace['reference_run'];effects.DLL=ROOT/'work/sonic-adventure/build/movie_entry.dll';effects.LIB=ctypes.CDLL(str(effects.DLL));effects.fn=effects.LIB.sa_movie_entry_connected;effects.fn.argtypes=previous.effects.fn.argtypes;effects.fn.restype=ctypes.c_int
effects.LIB.sa_cache_register_bus_init.argtypes=previous.effects.LIB.sa_cache_register_bus_init.argtypes;effects.LIB.sa_disc_queue_size.restype=ctypes.c_uint32;effects.LIB.sa_disc_queue_init.argtypes=previous.effects.LIB.sa_disc_queue_init.argtypes
compare_code=previous.compare_code
marker="  boundaries.append(dict(target=hex(target),pr=hex(pr[0]),stack=hex(r[15]),arguments=list(r[4:8])))";assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker,marker+"\n  if target==0x8ceb0000:\n   boundaries[-1]['loaded_movie_image_sha256']=hashlib.sha256(bytes((ctypes.c_uint8*184592).from_address(ctypes.addressof(mem.contents)+0xeb0000))).hexdigest()\n   boundaries[-1]['movie_entry_live_requests']=sum(j.state!=0 for j in q.jobs)\n   boundaries[-1]['movie_entry_read_attempts']=len(native_reads)")
exec(compile(compare_code,'movie_comparison','exec'),dict(previous.compare.__globals__,effects=effects,namespace=namespace),local:={});compare=local['compare']
def prepare(ram,c):
 m,s,f=previous.prepare(ram,c)
 if c['entry'] in MODULE_ENTRIES:
  payload=(ROOT/manifest['retail_binary']).read_bytes();assert hashlib.sha256(payload).hexdigest()==manifest['retail_binary_sha256'];m[0xeb0000:0xeb0000+len(payload)]=payload;s.r[4]=c.get('argument',1)
 elif c['entry']==0x8c6436d0:
  store(m,load(m,0x8c6436e4),c.get('pointer',0x8cd60000));s.r[4]=c.get('value',1)
 elif c['entry']==0x8c041e60:
  base=load(m,0x8c041ed0)
  for i,value in enumerate(c['words']):store(m,base+0x90+i*4,value)
 elif c['entry']==0x8c10d31c:
  s.r[4]=0x8cd70000;s.r[5]=c['value'];s.r[6]=c['count'];m[0xd70000:0xd70200]=bytes([0xa5])*512
 elif c['entry']==0x8c0509a0:m[load(m,0x8c050a20)&0xffffff]=c['before_byte']
 return m,s,f

def semantics(m,a,before,fp,s,f,r,c):
 if r['status']:return 0
 expected=bytearray(m);entry=c['entry']
 if entry==0x8c6436d0:
  ptr=c.get('pointer',0x8cd60000)
  if ptr:store(expected,ptr+0x1a8,c['value'])
  assert s.r[0]==0 and s.r[3]==load(m,0x8c6436e4) and s.r[4]==before.r[4] and s.r[5]==ptr
  assert s.sr==(before.sr&~1)|int(ptr==0) and bytes(f)==bytes(fp);checks=8
 elif entry==0x8c041e60:
  store(expected,load(m,0x8c041ec4),load(m,0x8c041ec0));store(expected,load(m,0x8c041ecc),load(m,0x8c041ec8))
  for literal,value in zip((0x8c041ed4,0x8c041ed8,0x8c041edc,0x8c041ee0),c['words']):store(expected,load(m,literal),value)
  out=effects.Fpu.from_buffer_copy(fp);out.banks[(out.fpscr>>21)&1][3]=load(m,0x8c041ec8)
  assert bytes(f)==bytes(out) and s.sr==before.sr and s.r[0]==0x9c and s.r[3]==c['words'][3];checks=10
 elif entry==0x8c10d31c:
  n=c['count'];expected[0xd70000:0xd70000+n]=bytes([c['value']&255])*n
  assert s.r[0]==before.r[4] and s.r[7]==n and s.r[3]==0 and s.sr==before.sr|1 and bytes(f)==bytes(fp);checks=8
 elif entry==0x8c0509a0:
  expected[load(m,0x8c050a20)&0xffffff]=1;assert s.sr==before.sr and bytes(f)==bytes(fp);checks=4
 else:return 0
 assert a==expected and s.pr==before.pr and s.r[15]==before.r[15] and not r['disc_read_attempts'] and not r['clock_reads'];return checks+5
def main():
 for proof in (MP,MAIN_MP,P/'analysis/movie_entry_connection.json',P/'analysis/boot_cache_qualification.json'):
  for path,h in json.loads(proof.read_text())['sha256'].items():assert hashlib.sha256(Path(path).read_bytes()).hexdigest()==h,path
 ram=previous.previous.previous.previous.previous.previous.previous.previous.seed_ram();records=[];coverage=set();edges=set()
 selected=[dict(entry=e,mode=mode,argument=argument) for e in (ENTRY,START) for mode in (0,4) for argument in (0,1,7,8)]
 selected += [dict(entry=0x8c6436d0,mode=mode,pointer=ptr,value=value) for mode in (0,4) for ptr in (0,0x8cd60000) for value in (0,1,0xffffffff)]
 selected += [dict(entry=0x8c041e60,mode=mode,words=words) for mode in (0,4) for words in ([0]*4,[0xffffffff]*4,[0x12345678,0x89abcdef,0x3f800000,0x80000000])]
 selected += [dict(entry=0x8c10d31c,mode=mode,count=n,value=value) for mode in (0,4) for n in (0,1,2,3,32,257) for value in (0,0x1234ff)]
 selected += [dict(entry=0x8c0509a0,mode=mode,before_byte=value) for mode in (0,4) for value in (0,255)]
 if '--smoke' in sys.argv:selected=selected[:1]
 for c in selected:
  m,s,f=prepare(ram,c);before=effects.State.from_buffer_copy(s);fp=effects.Fpu.from_buffer_copy(f);r,path,a,s,f=compare(m,s,f,c);r['independent_checks']=semantics(m,a,before,fp,s,f,r,c);records.append(r);coverage.update(path);print(hex(c['entry']),c.get('argument'),r['status'],r['stop_pc'],len(path),flush=True)
  for j,pc in enumerate(path):
   if pc not in ROWS:continue
   w=int.from_bytes(m[pc&0xffffff:(pc&0xffffff)+2],'little');distance=2 if w>>8 in (0x8d,0x8f) else 1
   if w>>8 in (0x89,0x8b,0x8d,0x8f) and j+distance<len(path):edges.add((pc,path[j+distance]))
 if '--smoke' in sys.argv:print('MOVIE smoke passed',len(records));return
 files=[Path(__file__),MP,MAIN_MP,P/'analysis/movie_entry_connection.json',P/'analysis/movie_runtime_inventory.json',P/'analysis/boot_cache_qualification.json']
 for p in list(files[1:]):files.extend(Path(f) for f in json.loads(p.read_text())['sha256'])
 report=dict(source=manifest['source'],source_sha256=hashlib.sha256((P/manifest['source']).read_bytes()).hexdigest(),module='MOVIE.BIN',load_address=manifest['load_address'],retail_binary=manifest['retail_binary'],retail_binary_sha256=manifest['retail_binary_sha256'],routines=manifest['routines'],coverage_by_routine=[dict(address=hex(a),static_rows=len(body),covered_rows=len(set(body)&coverage),missing_rows=[hex(x) for x in sorted(set(body)-coverage)]) for a,body in PER_BODY.items()],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),newly_registered_entries=0,dll_sha256=hashlib.sha256(effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Original raw MOVIE entry/startup proposals, actual loaded code/literals and existing native game services. Complete reached/final machine, RAM/FPU, boundaries, disc, cache and software peripheral state match. Missing dependencies and completion waits stay explicit.',limitations=['Initialized runtime driver/input/event state and original matching MOVIE code/literals are preconditions. No natural cold startup or playable movie/game is proved.','Firmware/disc/CCR/coherent-cache services retain their declared host policies, not physical timing or exception delivery.','The startup parent remains partial until all reachable behavior and dependencies are qualified. Ghidra candidates alone are not recovery credit.'],cases=records)
 report['routines']=routines;report['independent_checks']=sum(r.get('independent_checks',0) for r in records)
 report['sources']={source:hashlib.sha256((P/source).read_bytes()).hexdigest() for source in (manifest['source'],main_manifest['source'])}
 for row in report['coverage_by_routine']:
  routine=next(r for r in routines if r['address']==row['address']);wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
  row['conditional_edges_covered']=len(wanted&edges);row['missing_edges']=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)]
 (P/'analysis/movie_entry_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('MOVIE comparisons passed',len(records),'independent checks',report['independent_checks'])
if __name__=='__main__':main()
