"""Original nested cable query with explicit read-only sampled PDTRA input."""
import collections,csv,ctypes,hashlib,itertools,json,random,sys,types
from pathlib import Path
import verify_movie_entry as previous
from verify_overlay_selector_initializer import load,store
P=Path(__file__).resolve().parent;ROOT=P.parents[1];MP=P/'analysis/movie_status_translation.json';manifest=json.loads(MP.read_text());QUERY,READ,PORT=0x8c10d914,0x8c10d926,0xff800030;NEW={QUERY,READ};ROWS=dict(previous.ROWS);PER_BODY={}
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);PER_BODY[int(routine['address'],16)]=body
namespace=dict(previous.namespace,ROWS=ROWS,RETURNS=previous.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=previous.namespace['LEGACY']-NEW,PORT=PORT)
code=previous.code;marker='queue_reference.ccr_state()';assert code.count(marker)==1;code=code.replace(marker,marker+',queue_reference.port_data')
marker='   a,n=access';assert code.count(marker)==1;code=code.replace(marker,marker+"\n   if a==PORT and c.get('null_bus'):stop(pc,r,sr,-2)")
exec(compile(code,'movie_status_original_model','exec'),namespace)
effects=types.SimpleNamespace(**previous.effects.__dict__);effects.CONNECTED=previous.effects.CONNECTED|NEW;effects.namespace=namespace;effects.reference_run=namespace['reference_run'];effects.DLL=ROOT/'work/sonic-adventure/build/movie_status.dll';effects.LIB=ctypes.CDLL(str(effects.DLL));effects.fn=effects.LIB.sa_movie_status_connected;effects.fn.argtypes=previous.effects.fn.argtypes;effects.fn.restype=ctypes.c_int
CacheMemory=previous.compare.__globals__['CacheMemory'];CacheReference=previous.compare.__globals__['CacheReference']
class VideoMemory(CacheMemory):_fields_=[('cache_bus',effects.Bus),('port_data',ctypes.c_uint32)]
effects.LIB.sa_video_signal_bus_init.argtypes=[ctypes.POINTER(VideoMemory),ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(effects.Bus),ctypes.c_uint32]
effects.LIB.sa_disc_queue_size.restype=ctypes.c_uint32;effects.LIB.sa_disc_queue_init.argtypes=previous.effects.LIB.sa_disc_queue_init.argtypes
class VideoReference(CacheReference):
 def __init__(self,ticks,capacity,c):super().__init__(ticks,capacity,c);self.port_data=c.get('port_data',0)&0xffff
 def __contains__(self,a):return a==PORT or super().__contains__(a)
 def __getitem__(self,a):
  if a!=PORT:return super().__getitem__(a)
  self.events.append((21,a,self.port_data,2));return self.port_data
 def __setitem__(self,a,value):
  assert a!=PORT,'PDTRA is read-only in this reached host interface'
  return super().__setitem__(a,value)
compare_code=previous.compare_code.replace('CacheMemory','VideoMemory').replace('CacheReference(','VideoReference(').replace('sa_cache_register_bus_init','sa_video_signal_bus_init')
marker='ctypes.byref(memory),view,ctypes.byref(bus))';assert compare_code.count(marker)==1;compare_code=compare_code.replace(marker,"ctypes.byref(memory),view,ctypes.byref(bus),c.get('port_data',0))")
marker='(memory.ccr,memory.ic_requests,memory.oc_requests)))';assert compare_code.count(marker)==1;compare_code=compare_code.replace(marker,'(memory.ccr,memory.ic_requests,memory.oc_requests),memory.port_data))')
marker=' return dict(ccr_state=';assert compare_code.count(marker)==1;compare_code=compare_code.replace(marker," assert memory.port_data==ref.port_data\n return dict(port_data=memory.port_data,video_reads=[list(e) for e in ref.events if e[0]==21],ccr_state=")
exec(compile(compare_code,'movie_status_comparison','exec'),dict(previous.compare.__globals__,effects=effects,namespace=namespace,VideoMemory=VideoMemory,VideoReference=VideoReference),local:={});compare=local['compare']
def prepare(ram,c):return previous.prepare(ram,c)
def cases():
 rng=random.Random(0xff800030);values=[0,1,0x100,0x200,0x300,0x7fff,0x8000,0xffff]+[rng.getrandbits(16) for _ in range(16)]
 for entry,word,mode in itertools.product((QUERY,READ),values,(0,4)):yield dict(entry=entry,port_data=word,mode=mode)
 for entry,imask,t in itertools.product((QUERY,READ),(0,1,7,15),(0,1)):yield dict(entry=entry,mode=0,port_data=0xa35a,sr=0x60000302|(imask<<4)|t)
 for entry in (QUERY,READ):
  yield dict(entry=entry,mode=0,null_bus=True)
  for status in (-2,-3):yield dict(entry=entry,mode=0,reject_bus=PORT,bus_status=status)
def semantics(m,a,before,fp,s,f,r,c):
 if r['status']:return 0
 raw=c.get('port_data',0)&0xffff;word=raw&0x300;expected=bytearray(m)
 if c['entry']==QUERY:store(expected,before.r[15]-4,before.pr);assert s.r[0]==s.r[4]==word>>8
 else:assert s.r[0]==word and s.r[4]==ctypes.c_uint32(ctypes.c_int16(raw).value).value
 assert s.r[2]==0x300 and s.r[3]==PORT and s.r[15]==before.r[15] and s.sr==before.sr and s.pr==before.pr
 assert a==expected and bytes(f)==bytes(fp) and r['port_data']==raw and r['video_reads']==[[21,PORT,raw,2]]
 assert all(s.r[i]==before.r[i] for i in range(16) if i not in (0,2,3,4)) and not r['disc_read_attempts'] and not r['clock_reads'];return 14
def main():
 proofs=[MP,P/'analysis/movie_status_connection.json',P/'analysis/movie_entry_qualification.json']
 for p in proofs:
  for path,h in json.loads(p.read_text())['sha256'].items():assert hashlib.sha256(Path(path).read_bytes()).hexdigest()==h,path
 ram=previous.previous.previous.previous.previous.previous.previous.previous.previous.seed_ram();records=[];coverage=set();selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(entry=e,mode=0,port_data=0xa35a) for e in (QUERY,READ)]
 for c in selected:
  m,s,f=prepare(ram,c);before=effects.State.from_buffer_copy(s);fp=effects.Fpu.from_buffer_copy(f);record,path,a,s,f=compare(m,s,f,c);record['independent_checks']=semantics(m,a,before,fp,s,f,record,c);records.append(record);coverage.update(path)
 if '--smoke' in sys.argv:print('Status smoke passed',len(records));return
 rows=[dict(address=hex(entry),static_rows=len(body),covered_rows=len(set(body)&coverage),missing_rows=[hex(a) for a in sorted(set(body)-coverage)]) for entry,body in PER_BODY.items()];assert all(not r['missing_rows'] for r in rows)
 files=[Path(__file__),Path(previous.__file__)]+proofs
 for p in proofs:files.extend(Path(f) for f in json.loads(p.read_text())['sha256'])
 report=dict(source=manifest['source'],source_sha256=hashlib.sha256((P/manifest['source']).read_bytes()).hexdigest(),routines=manifest['routines'],cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),independent_checks=sum(r['independent_checks'] for r in records),coverage_by_routine=rows,newly_registered_entries=0,dll_sha256=hashlib.sha256(effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},reference_document='https://github.com/KallistiOS/KallistiOS/blob/master/kernel/arch/dreamcast/hardware/video.c',reference_scope='vid_check_cable reads PDTRA bits 8/9 and names the four cable states. Matching the original query port/bits supports this interpretation; no KOS implementation is copied.',scope='Original nine-row wrapper and nested six-row query with sampled PDTRA word input. Complete machine/RAM/FPU/stack, boundaries, disc/cache/peripheral state and every word read match independent instruction/platform execution. No game query return is supplied.',limitations=['Caller-supplied sampled port state and privileged MMU-off execution are explicit preconditions. GPIO direction/electrical inputs, physical cable detection, system timing and exceptions are not modeled.','This models the reached word-only read interface; PCTRA and writes/byte/longword/Area-7 access are not implemented here.','MOVIE startup, natural boot, pixels, playback and the full game/ports/settings remain unfinished.'],cases=records)
 (P/'analysis/movie_status_verification.json').write_text(json.dumps(report,indent=2)+'\n');print({k:report[k] for k in ('cases_passed','status_counts','independent_checks','coverage_by_routine')})
if __name__=='__main__':main()
