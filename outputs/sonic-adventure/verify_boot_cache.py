"""Original P1/P2 cache leaf and explicit coherent cache-control state."""
import ctypes,hashlib,itertools,json,random,sys,types
from pathlib import Path
import verify_boot_disc as previous
P=Path(__file__).resolve().parent;ROOT=P.parents[1];MP=P/'analysis/boot_cache_translation.json';manifest=json.loads(MP.read_text());CCR=0xff00001c;CACHED,UNCACHED=0x8c5fc0c2,0xac5fc0c2
ROWS=dict(previous.ROWS);PER_BODY={}
body={int(r['address'],16):r['instruction'].lstrip('_') for r in previous.csv.DictReader(Path(manifest['routines'][0]['listing']).open(),delimiter='\t')};PER_BODY[CACHED]=body
PER_BODY[UNCACHED]={a+0x20000000:t.replace('0x8c5fc','0xac5fc') for a,t in body.items()}
for body in PER_BODY.values():ROWS.update(body)
NEW={CACHED,UNCACHED};namespace=dict(previous.namespace,ROWS=ROWS,LEGACY=previous.namespace['LEGACY']-NEW,CCR=CCR)
code=previous.code;marker='disc_reference.snapshot()';assert code.count(marker)==1;code=code.replace(marker,marker+',queue_reference.ccr_state()')
marker='   a,n=access';assert code.count(marker)==1
code=code.replace(marker,marker+"\n   if a==CCR and c.get('null_bus'):stop(pc,r,sr,-2)\n   if a==CCR and c.get('ccr_write_status') and not text.split(' ',1)[1].split(',')[0].startswith('@'):stop(pc,r,sr,c['ccr_write_status'])")
exec(compile(code,'cache_original_model','exec'),namespace)
effects=types.SimpleNamespace(**previous.effects.__dict__);effects.CONNECTED=previous.effects.CONNECTED|NEW;effects.namespace=namespace;effects.reference_run=namespace['reference_run'];effects.DLL=ROOT/'work/sonic-adventure/build/boot_cache.dll';effects.LIB=ctypes.CDLL(str(effects.DLL));effects.fn=effects.LIB.sa_boot_cache_connected;effects.fn.argtypes=previous.effects.fn.argtypes;effects.fn.restype=ctypes.c_int
EventMemory=previous.compare.__globals__['EventMemory']
class CacheMemory(EventMemory):_fields_=[('event_bus',effects.Bus),('ccr',ctypes.c_uint32),('ic_requests',ctypes.c_uint32),('oc_requests',ctypes.c_uint32)]
effects.LIB.sa_cache_register_bus_init.argtypes=[ctypes.POINTER(CacheMemory),ctypes.POINTER(ctypes.c_uint8),ctypes.POINTER(effects.Bus)]
effects.LIB.sa_disc_queue_size.restype=ctypes.c_uint32;effects.LIB.sa_disc_queue_init.argtypes=previous.effects.LIB.sa_disc_queue_init.argtypes
class CacheReference(previous.DiscEventReference):
 def __init__(self,ticks,capacity,c):
  super().__init__(ticks,capacity,c);self.ccr=c.get('ccr',0)&~0x808;self.ic=c.get('ic_requests',0);self.oc=c.get('oc_requests',0)
 def ccr_state(self):return self.ccr,self.ic,self.oc
 def __contains__(self,a):return a==CCR or super().__contains__(a)
 def __getitem__(self,a):
  if a!=CCR:return super().__getitem__(a)
  value=self.ccr&~0x808;self.events.append((20,a,value,4));return value
 def __setitem__(self,a,value):
  if a!=CCR:return super().__setitem__(a,value)
  self.events.append((19,a,value&0xffffffff,4));self.ccr=value&~0x808&0xffffffff;self.ic=(self.ic+bool(value&0x800))&0xffffffff;self.oc=(self.oc+bool(value&8))&0xffffffff

compare_code=previous.compare_code.replace('EventMemory','CacheMemory').replace('sa_event_bus_init','sa_cache_register_bus_init').replace('DiscEventReference(','CacheReference(')
marker="memory.masks[:]=c.get('masks',[0x1234567,0x89abcdef]);";assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker,marker+"memory.ccr=c.get('ccr',0)&~0x808;memory.ic_requests=c.get('ic_requests',0);memory.oc_requests=c.get('oc_requests',0);")
marker='platform.snapshot(q,view,native_reads)))';assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker,'platform.snapshot(q,view,native_reads),(memory.ccr,memory.ic_requests,memory.oc_requests)))')
marker='  if c.get(\'reject_dma\')==a:return';assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker,"  if a==CCR and c.get('ccr_write_status'):return c['ccr_write_status']\n"+marker)
marker=' return dict(disc_snapshot=';assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker," assert (memory.ccr,memory.ic_requests,memory.oc_requests)==ref.ccr_state()\n return dict(ccr_state=ref.ccr_state(),ccr_events=[list(e) for e in ref.events if e[0] in (19,20)],disc_snapshot=")
exec(compile(compare_code,'cache_comparison','exec'),dict(previous.compare.__globals__,effects=effects,namespace=namespace,CacheReference=CacheReference,CacheMemory=CacheMemory,CCR=CCR),local:={});compare=local['compare']

def prepare(ram,c):
 if c['entry'] in NEW:
  seed_case=dict(c,entry=previous.previous.UPPER);m,s,f=previous.prepare(ram,seed_case);c.update({key:value for key,value in seed_case.items() if key!='entry'});s.pc=c['entry'];return m,s,f
 return previous.prepare(ram,c)

def cases():
 rng=random.Random(0x5fc0c2);values=[0,1,0x105,0x909,0x89abcdef,0xffffffff,0x80000000]+[rng.getrandbits(32) for _ in range(9)]
 for entry,ccr,mode in itertools.product((CACHED,UNCACHED),values,(0,4)):yield dict(entry=entry,ccr=ccr,mode=mode)
 for entry,imask,t in itertools.product((CACHED,UNCACHED),(0,1,7,15),(0,1)):yield dict(entry=entry,mode=0,ccr=0x101,sr=0x60000302|(imask<<4)|t)
 for entry in (CACHED,UNCACHED):
  yield dict(entry=entry,mode=0,ccr=0x101,ic_requests=0xffffffff,oc_requests=0x12345678)
  yield dict(entry=entry,mode=0,null_bus=True)
  yield dict(entry=entry,mode=0,reject_bus=CCR,bus_status=-3)
  yield dict(entry=entry,mode=0,ccr_write_status=-3)

def semantics(m,a,before,fp,s,f,r,c):
 if r['status']:return 0
 original=c.get('ccr',0)&~0x808;assert s.r[0]==original|0x800 and s.r[2]==0x800 and s.r[3]==CCR
 assert r['ccr_state']==(original,(c.get('ic_requests',0)+1)&0xffffffff,c.get('oc_requests',0))
 assert r['ccr_events']==[[20,CCR,original,4],[19,CCR,original|0x800,4]]
 assert a==m and bytes(fp)==bytes(f) and s.sr==before.sr and s.pr==before.pr
 assert all(s.r[i]==before.r[i] for i in range(16) if i not in (0,2,3))
 assert not r['disc_read_attempts'] and not r['clock_reads'];return 12

def main():
 for proof in (MP,P/'analysis/boot_cache_connection.json',P/'analysis/boot_disc_qualification.json'):
  for path,h in json.loads(proof.read_text())['sha256'].items():assert hashlib.sha256(Path(path).read_bytes()).hexdigest()==h,path
 ram=previous.previous.previous.previous.previous.previous.previous.seed_ram();records=[];coverage=set();selected=list(cases())
 if '--smoke' in sys.argv:selected=[dict(entry=e,mode=0) for e in (CACHED,UNCACHED)]
 for c in selected:
  m,s,f=prepare(ram,c);before=effects.State.from_buffer_copy(s);fp=effects.Fpu.from_buffer_copy(f);record,path,a,s,f=compare(m,s,f,c);record['independent_checks']=semantics(m,a,before,fp,s,f,record,c);records.append(record);coverage.update(path)
 if '--smoke' in sys.argv:print('Cache smoke passed',len(records));return
 rows=[dict(address=hex(entry),static_rows=6,covered_rows=len(set(body)&coverage),missing_rows=[hex(a) for a in sorted(set(body)-coverage)]) for entry,body in PER_BODY.items()]
 assert all(r['covered_rows']==6 for r in rows)
 files=[Path(__file__),MP,P/'analysis/boot_cache_connection.json',P/'analysis/boot_disc_qualification.json',Path(previous.__file__)]
 for p in list(files[1:4]):files.extend(Path(f) for f in json.loads(p.read_text())['sha256'])
 report=dict(source=manifest['source'],source_sha256=hashlib.sha256((P/manifest['source']).read_bytes()).hexdigest(),routines=manifest['routines'],cases_passed=len(records),status_counts=dict(__import__('collections').Counter(r['status'] for r in records)),independent_checks=sum(r['independent_checks'] for r in records),coverage_by_routine=rows,unique_physical_rows=6,newly_registered_entries=0,dll_sha256=hashlib.sha256(effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Original six-instruction CCR leaf under both P1 and P2 PCs; full machine/RAM/FPU/boundaries, disc and software peripheral/cache-control state match. Canonical and alias bodies share physical bytes; duplicate recovery credit is zero.',limitations=['CCR uses coherent-RAM storage: invalidation commands read zero, other bits are explicit host storage including reserved fixture inputs. Physical cache contents, timing, MMU translation and exception delivery are not modeled.','Qualified CCR leaf domain is privileged MMU-off execution with immutable code/literals and explicit software peripheral/drive fixtures.','Whole game, natural boot and requested ports/features remain unfinished.'],reference_document='https://www.st.com/resource/en/user_manual/cd00147165.pdf',reference_scope='Table 18, PDF pages 77-79: CCR OCI/ICI commands read zero; other software-storage policies remain explicit.',cases=records)
 (P/'analysis/boot_cache_verification.json').write_text(json.dumps(report,indent=2)+'\n');print({k:report[k] for k in ('cases_passed','status_counts','independent_checks','coverage_by_routine')})
if __name__=='__main__':main()
