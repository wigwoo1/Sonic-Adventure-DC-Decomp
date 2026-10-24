"""Compare original game instructions with independently modeled host disc I/O."""
import collections,csv,ctypes,hashlib,itertools,json,sys,types
from pathlib import Path
import verify_boot_file as previous
import boot_disc_reference as platform
from verify_overlay_selector_initializer import load,store
P=Path(__file__).resolve().parent;ROOT=P.parents[1];MP=P/'analysis/boot_disc_translation.json';manifest=json.loads(MP.read_text())
BIOS=previous.BIOS;NEW={int(r['address'],16) for r in manifest['routines']};ROWS=dict(previous.ROWS);OWN={};PER_BODY={}
for routine in manifest['routines']:
 body={int(r['address'],16):r['instruction'].lstrip('_') for r in csv.DictReader(Path(routine['listing']).open(),delimiter='\t')};ROWS.update(body);OWN.update(body);PER_BODY[int(routine['address'],16)]=body
namespace=dict(previous.namespace,ROWS=ROWS,RETURNS=previous.namespace['RETURNS']|{a+4 for a,t in ROWS.items() if t.startswith(('jsr ','bsr '))},LEGACY=previous.namespace['LEGACY']-NEW,BIOS=BIOS)
code=previous.code;marker="tuple(queue_reference.masks),special['vbr']";assert code.count(marker)==1;code=code.replace(marker,marker+',disc_reference.snapshot()')
marker=' def trace(pc,w,r,sr,pr,fp):';assert code.count(marker)==1
code=code.replace(marker,""" def firmware_hook(m,regs,sr):
  events.append(event(BIOS,regs,sr,state.pr))
  if regs[6]!=0:stop(BIOS,regs,sr,-8)
  rc,result=disc_reference.service(m,regs[7],regs[4],regs[5])
  if rc:stop(BIOS,regs,sr,rc)
  regs[0]=result;return sr
 hooked={BIOS:firmware_hook}
"""+marker)
marker='cache_invalidate=queue_reference.invalidate,';assert code.count(marker)==1;code=code.replace(marker,marker+'cache_purge=queue_reference.purge,')
exec(compile(code,'disc_original_model','exec'),namespace)
effects=types.SimpleNamespace(**previous.effects.__dict__);effects.CONNECTED=previous.effects.CONNECTED|NEW;effects.namespace=namespace;effects.reference_run=namespace['reference_run'];effects.DLL=ROOT/'work/sonic-adventure/build/boot_disc.dll';effects.LIB=ctypes.CDLL(str(effects.DLL));effects.fn=effects.LIB.sa_boot_disc_connected;effects.fn.argtypes=previous.effects.fn.argtypes+[ctypes.POINTER(platform.Queue),ctypes.c_uint32];effects.fn.restype=ctypes.c_int
effects.LIB.sa_event_bus_init.argtypes=previous.effects.LIB.sa_event_bus_init.argtypes
effects.LIB.sa_disc_queue_size.restype=ctypes.c_uint32;assert effects.LIB.sa_disc_queue_size()==ctypes.sizeof(platform.Queue)
effects.LIB.sa_disc_queue_init.argtypes=[ctypes.POINTER(platform.Queue),platform.Read,ctypes.c_void_p,ctypes.c_uint64,ctypes.c_uint64,ctypes.c_uint32,ctypes.c_uint32]
class DiscEventReference(previous.previous.EventReference):
 def purge(self,a):
  assert a&0xe0000000 in (0,0x80000000,0xa0000000) and a&0x1f000000==0xc000000
  self.events.append((18,a,0,0));self.invalidates+=1

compare_code=previous.previous.compare_code
marker=' other=effects.State.from_buffer_copy(s);';assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker," disc_ref=platform.DiscReference(ROOT,c);namespace['disc_reference']=disc_ref\n"+marker)
marker='EventReference(';compare_code=compare_code.replace(marker,'DiscEventReference(')
marker=' memory=EventMemory();';assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker,""" q=platform.Queue();native_reads=[];callback_errors=[]
 @platform.Read
 def sector_read(user,pos,out,size):
  try:
   error=-17 if len(native_reads)==c.get('fail_after',-1) else 0;native_reads.append((pos,size,error))
   if error:return error
   with disc_ref.path.open('rb') as stream:stream.seek(pos);data=stream.read(size)
   assert len(data)==size;ctypes.memmove(out,data,size);return 0
  except BaseException as exc:callback_errors.append(repr(exc));return -8
 assert effects.LIB.sa_disc_queue_init(ctypes.byref(q),sector_read,None,disc_ref.offset,disc_ref.sectors,disc_ref.base,disc_ref.stride)==0
"""+marker)
marker='tuple(memory.masks),vb.value))';assert compare_code.count(marker)==1;compare_code=compare_code.replace(marker,'tuple(memory.masks),vb.value,platform.snapshot(q,view,native_reads)))')
marker='ctypes.byref(extra),ctypes.byref(vb))';assert compare_code.count(marker)==1;compare_code=compare_code.replace(marker,'ctypes.byref(extra),ctypes.byref(vb),ctypes.byref(q),BIOS)')
marker=' return dict(vbr=';assert compare_code.count(marker)==1
compare_code=compare_code.replace(marker," assert not callback_errors,callback_errors\n assert platform.snapshot(q,view,native_reads)==disc_ref.snapshot(),(c,'complete disc ownership/progress and transport events')\n return dict(disc_snapshot=disc_ref.snapshot(),disc_read_attempts=len(native_reads),disc_reads=[list(r) for r in native_reads],vbr=")
exec(compile(compare_code,'disc_comparison','exec'),dict(previous.compare.__globals__,effects=effects,namespace=namespace,DiscEventReference=DiscEventReference,platform=platform,ROOT=ROOT,BIOS=BIOS),local:={});compare=local['compare']

def prepare(ram,c):
 m,s,f=previous.prepare(ram,c);_,_,_,base,_=platform.track_info(ROOT)
 start=c.get('sector_start',base+16+150);store(m,previous.FD+8,start);store(m,previous.DIRECTORY+16,start-150)
 if c['entry']==previous.GDREAD:s.r[4]=start
 if c['entry'] in (0x8c5fc0d8,0x8c5fc336):s.r[4]=c.get('buffer',previous.DEST);s.r[5]=c.get('bytes',2048)
 return m,s,f

def cases():
 for entry in (previous.GDREAD,previous.LOAD,previous.READ,previous.RAW_READ,previous.WAIT,previous.CANCEL,previous.CLOSE,previous.PUMP):yield dict(entry=entry,mode=0)
 for entry,count,mode in itertools.product((previous.GDREAD,previous.LOAD),(0,1,3),(0,4)):
  yield dict(entry=entry,mode=mode,count=count,sizes=[count*2048,0,0])
 for entry,n,mode in itertools.product((0x8c5fc0d8,0x8c5fc336),(0,1,31,32,33,2048,0x30000),(0,4)):yield dict(entry=entry,mode=mode,bytes=n)
 for size,mode,dma in itertools.product((0,1,2047,2048,2049,184592),(0,4),(0,1)):
  yield dict(entry=previous.LOAD,mode=mode,dma_mode=dma,names=[b'MOVIE.BIN'],request=b'MOVIE.BIN',sizes=[size],sector_start=510488,movie_bytes=size)
 for fail,mode,dma in itertools.product((0,1),(0,4),(0,1)):
  yield dict(entry=previous.LOAD,mode=mode,dma_mode=dma,names=[b'MOVIE.BIN'],request=b'MOVIE.BIN',sizes=[184592],sector_start=510488,movie_bytes=184592,fail_after=fail)
 for mode in (0,4):yield dict(entry=previous.LOAD,mode=mode,names=[b'MOVIE.BIN'],request=b'MISSING.BIN',sizes=[184592],missing=True)

def semantics(m,a,before,s,f,record,c):
 checks=0
 if c.get('movie_bytes') is not None:
  assert record['status']==0 and s.r[0]==0;checks+=2
  size=c['movie_bytes'];expected=(size+2047)//2048;done=sum(rc==0 for _,_,rc in record['disc_reads'])
  assert record['disc_read_attempts']==(min(expected,c['fail_after']+1) if 'fail_after' in c else expected);checks+=1
  movie=(ROOT/'work/sonic-adventure/extracted/files/SONICADV/MOVIE.BIN').read_bytes();assert len(movie)==184592 and hashlib.sha256(movie).hexdigest()=='84489968b607f6f80a7ad04ad402224ddd951a65e3ae8e18429ecfa17525614b';checks+=1
  n=min(done*2048,len(movie));assert a[previous.DEST&0xffffff:(previous.DEST&0xffffff)+n]==movie[:n];checks+=1
  count=(size+2047)//2048;start=previous.DEST&0xffffff
  if 'fail_after' in c:assert a[start+done*2048:start+count*2048]==m[start+done*2048:start+count*2048];checks+=1
  assert previous.word16(a,previous.FD+0x4c)==0 and not any(j[1] for j in record['disc_snapshot'][7]);checks+=2
 if c.get('missing'):assert record['status']==0 and s.r[0]==0xffffffff and not record['disc_read_attempts'];checks+=3
 if c['entry'] in (0x8c5fc0d8,0x8c5fc336) and c.get('bytes',2048)<0x30000:
  assert record['status']==0
  stack=before.r[15]&0xffffff;assert a[:stack-20]==m[:stack-20] and a[stack:]==m[stack:]
  n=c.get('bytes',2048);assert record['queue_events']==(((before.r[4]&31)+n+31)//32 if n else 0)
  assert s.r[12]==before.r[12] and s.r[13]==before.r[13] and s.r[14]==before.r[14] and s.r[15]==before.r[15];checks+=7
 return checks

def main():
 for proof in (MP,P/'analysis/boot_disc_connection.json',P/'analysis/boot_file_qualification.json'):
  for path,h in json.loads(proof.read_text())['sha256'].items():assert hashlib.sha256(Path(path).read_bytes()).hexdigest()==h,path
 ram=previous.previous.previous.previous.previous.previous.seed_ram();records=[];coverage=set();edges=set();selected=list(cases())
 if '--smoke' in sys.argv:selected=selected[:8]
 for c in selected:
  m,s,f=prepare(ram,c);before=effects.State.from_buffer_copy(s);record,path,a,s,f=compare(m,s,f,c);record['independent_checks']=semantics(m,a,before,s,f,record,c);records.append(record);coverage.update(path);print(hex(c['entry']),record['status'],record['stop_pc'],record['disc_read_attempts'],len(path),flush=True)
  for j,pc in enumerate(path):
   if pc not in ROWS:continue
   w=int.from_bytes(ram[pc&0xffffff:(pc&0xffffff)+2],'little');distance=2 if w>>8 in (0x8d,0x8f) else 1
   if w>>8 in (0x89,0x8b,0x8d,0x8f) and j+distance<len(path):edges.add((pc,path[j+distance]))
 if '--smoke' in sys.argv:print('Disc smoke comparisons passed',len(records));return
 coverage_rows=[]
 for routine in previous.manifest['routines']+manifest['routines']:
  address=int(routine['address'],16);body=PER_BODY.get(address,previous.PER_BODY.get(address));wanted={tuple(int(x,16) for x in e) for e in routine['conditional_edges']}
  coverage_rows.append(dict(address=routine['address'],static_rows=len(body),covered_rows=len(set(body)&coverage),missing_rows=[hex(a) for a in sorted(set(body)-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)]))
 files=[Path(__file__),P/'boot_disc_reference.py',MP,P/'analysis/boot_disc_connection.json',P/'verify_boot_file.py',P/'analysis/boot_file_qualification.json',P/'analysis/native_disc_service_verification.json',P/'disc_inventory.json',ROOT/'work/sonic-adventure/extracted/disc_inventory.json']
 for p in list(files[2:4]):files.extend(Path(f) for f in json.loads(p.read_text())['sha256'])
 files.extend(Path(f) for f in json.loads((P/'analysis/boot_file_qualification.json').read_text())['sha256'])
 report=dict(cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),independent_checks=sum(r['independent_checks'] for r in records),coverage_by_routine=coverage_rows,newly_registered_entries=0,dll_sha256=hashlib.sha256(effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Original game code plus explicitly modeled host disc policies; no recovered-firmware claim. Full machine/RAM/boundaries, software peripheral effects, disc ownership/progress and every host transport attempt match.',limitations=['Runtime filesystem/directory/driver tables remain explicit fixtures; natural mount/boot ordering is not yet proved.','Native sector-per-pump, status, cancellation, drive identity and coherent cache policies are host platform choices, not decompiled BIOS or cycle-accurate devices.','Large cache paths stop at their missing uncached aliases; asynchronous hardware, gameplay and all ports/settings remain unfinished.'],cases=records)
 def encode(value):
  if isinstance(value,(bytes,bytearray)):return {'bytes_hex':value.hex()}
  raise TypeError(type(value).__name__)
 (P/'analysis/boot_disc_verification.json').write_text(json.dumps(report,indent=2,default=encode)+'\n');print('Disc comparisons passed',len(records),'independent checks',report['independent_checks'])

if __name__=='__main__':main()
