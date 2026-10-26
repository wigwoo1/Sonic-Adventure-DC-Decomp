"""Three actual command controls: packing,byte ranges and partial enqueue state."""
import collections,hashlib,itertools,json
from pathlib import Path
import write_watch_audio_controls as fast
v=fast.old;P=v.P;ROOT=v.ROOT

def signed_byte(x):return (x&255)-256 if x&128 else x&255

def cases():
 for entry,enabled,queued,tail,mode in itertools.product((v.BLOCK,v.TRIPLE,v.PAIR),(False,True),(0,255,256),(0,255),(0,1,4)):
  yield dict(entry=entry,enabled=enabled,queued=queued,tail=tail,mode=mode)
 for a,version,mode in itertools.product((0,15,16,127,128,255,0x100,0xffffff0f),(38,39),(0,1,4)):
  yield dict(entry=v.TRIPLE,arg4=a,arg5=0x80,arg6=0xff,version=version,mode=mode)
 for b,c,version in ((0,0,0),(127,127,38),(128,128,38),(255,255,38),(128,128,39),(255,255,39),(127,127,128),(128,128,255),(0xffffff7f,0xffffff80,127),(0xffffff80,0xffffffff,39)):
  yield dict(entry=v.TRIPLE,arg4=15,arg5=b,arg6=c,version=version,mode=0)
 for a,b in itertools.product((0,127,128,255,0x100,0xffffff7f),repeat=2):
  yield dict(entry=v.PAIR,arg4=a,arg5=b,mode=0)
 for source,mode in itertools.product((v.INPUT,0x0cd66000,0xacd66000,v.BUFFERS[v.BLOCK]+3,v.BUFFERS[v.BLOCK]+4,v.BUFFERS[v.BLOCK]+5,v.QUEUE+20,0xa0800000),(0,1,4)):
  yield dict(entry=v.BLOCK,arg4=source,mode=mode)
 for entry in (v.BLOCK,v.TRIPLE,v.PAIR):
  for extra in (dict(bad_stack=True),dict(unprivileged=True),dict(queue_pointer=0x8d000000),dict(queued=0xffffffff),dict(imask=15,ts=3)):
   yield dict(entry=entry,mode=0)|extra
 for source in (0x8d000000,0x8cfffffc):yield dict(entry=v.BLOCK,arg4=source,mode=0)
 yield dict(entry=v.PAIR,queue_pointer=0x8cffffe4,mode=0)

def semantics(m,a,before,fp,s,f,r,c):
 checks=2;assert r['full_state_match'] and r['ordered_boundaries_match']
 calls=[b for b in r['boundaries'] if b['target']==hex(v.SDK)];copies=[b for b in r['boundaries'] if b['target']==hex(v.COPY)];logs=[b for b in r['boundaries'] if b['target']==hex(v.LOGGER)]
 meta=v.LIBRARY+0x158;count=c.get('queued',0);tail=c.get('tail',0);entry=c['entry'];buffer=v.BUFFERS[entry]
 assert bytes(f)==fp and r['runtime_state'] is None and r['flash_storage_snapshot'] is None;checks+=3
 if r['status']:
  assert r['status'] in (-3,-4) and not logs;checks+=2
  if c.get('queue_pointer')==0x8cffffe4:
   assert len(calls)==2 and len(copies)==1 and v.load(a,meta+20)==1 and v.load(a,meta+24)==2;checks+=4
   first=bytearray(m[buffer&0xffffff:(buffer&0xffffff)+20]);first[:4]=(0x84).to_bytes(4,'little');first[4]=before.r[4]&255
   assert a[0xffffe4:0xfffff8]==first and v.load(a,0x8cfffff8)==1 and s.sr&0xf0==0xf0;checks+=3
  elif 'queue_pointer' in c:
   assert len(calls)==1 and not copies and v.load(a,meta+20)==count and v.load(a,meta+24)==tail+1 and s.sr&0xf0==0xf0;checks+=5
  else:
   assert v.load(a,meta+20)==count and v.load(a,meta+24)==tail and s.sr&0xf0==before.sr&0xf0;checks+=3
  return checks
 assert s.pc==s.pr==before.pr and list(s.r[8:])==list(before.r[8:]) and s.sr&0xf0==before.sr&0xf0;checks+=4
 expected=bytearray(m);result=0;sent=0;attempts=0;payloads=[]
 if not c.get('enabled',True):result=0x0d000001
 elif entry==v.TRIPLE and not 0<=signed_byte(before.r[4])<=15:result=0x03000001
 elif entry==v.PAIR and (signed_byte(before.r[4])<0 or signed_byte(before.r[5])<0):result=0x03000001
 if not result:
  codes=(0x84,0x82) if entry==v.PAIR else ((0x88,) if entry==v.BLOCK else (0x83,))
  for step,code in enumerate(codes):
   v.store(expected,buffer,code)
   if entry==v.BLOCK:
    source=before.r[4]
    for i in range(7):
     value=0xa5 if source&0x1fffffff==0x800000 else expected[(source+i)&0xffffff]
     expected[(buffer+4+i)&0xffffff]=value
   elif entry==v.TRIPLE:
    rate=signed_byte(before.r[6]);revision=signed_byte(c.get('version',39))
    if revision<39:rate>>=3
    expected[(buffer+4)&0xffffff]=before.r[4]&255
    expected[(buffer+5)&0xffffff]=(rate+128)&255
    expected[(buffer+6)&0xffffff]=(signed_byte(before.r[5])+128)&255
   else:expected[(buffer+4)&0xffffff]=before.r[4+step]&255
   attempts+=1;pending=v.load(expected,meta+20)
   if pending==256:result=0x0a000001;break
   index=v.load(expected,meta+24);next_index=(index+1)&0xffffffff
   if next_index==c.get('ring_capacity',256):next_index=0
   v.store(expected,meta+24,next_index);destination=v.QUEUE+index*28
   payload=bytes(expected[buffer&0xffffff:(buffer&0xffffff)+20]);payloads.append(payload)
   expected[destination&0xffffff:(destination&0xffffff)+20]=payload
   v.store(expected,destination+20,1);v.store(expected,meta+20,(pending+1)&0xffffffff);sent+=1
 if result:
  v.store(expected,v.load(m,0x8c65acb8),entry);v.store(expected,v.load(m,0x8c65acbc),result)
 assert s.r[0]==result and len(calls)==attempts and len(copies)==sent;checks+=3
 assert len(logs)==(2 if result==0x0a000001 else 1 if result else 0);checks+=1
 for first,size in ((v.LIBRARY,0x1000),(v.QUEUE-16,28*256+32),(0x8c8905b0,0x50),(v.INPUT,64)):
  assert a[first&0xffffff:(first&0xffffff)+size]==expected[first&0xffffff:(first&0xffffff)+size];checks+=1
 for literal in (0x8c65acb8,0x8c65acbc):assert v.load(a,v.load(m,literal))==v.load(expected,v.load(m,literal));checks+=1
 assert all(b['sr']&0xf0==0xf0 for b in copies) and all(b['sr']&0xf0==before.sr&0xf0 for b in logs);checks+=2
 sound_source=entry==v.BLOCK and before.r[4]&0x1fffffff==0x800000 and c.get('enabled',True)
 assert r['sound_state']==((7,0,7,0,7,0) if sound_source else (0,0,0,0,0,0));checks+=1
 assert r['sound_ram_sha256']==hashlib.sha256(bytes([0xa5])*0x200000).hexdigest();checks+=1
 return checks

def main():
 ram=v.seed_ram();records=[];coverage=set();edges=set();checks=0
 for i,case in enumerate(cases(),1):
  c=v.Input(case);m,s,f=v.prepare(ram,c);before=v.effects.State.from_buffer_copy(s);fp=bytes(f)
  r,path,a,s,f=fast.compare(m,s,f,c);n=semantics(m,a,before,fp,s,f,r,c);checks+=n;coverage.update(path)
  for j,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((pc,path[j+distance]))
  records.append(dict(input=dict(c),status=r['status'],stop_pc=r['stop_pc'],r0=hex(s.r[0]),boundaries=r['boundaries'],independent_checks=n));print(i,hex(c['entry']),r['status'],r['stop_pc'],flush=True)
 cov=[]
 for definition in v.manifest['routines']:
  body=set(v.PER_BODY[int(definition['address'],16)]);wanted=set(map(tuple,definition['conditional_edges']));actual={(hex(a),hex(b)) for a,b in edges}
  cov.append(dict(address=definition['address'],static_rows=len(body),covered_rows=len(body&coverage),missing_rows=[hex(a) for a in sorted(body-coverage)],conditional_outcomes=len(wanted),covered_outcomes=len(wanted&actual),missing_outcomes=[list(x) for x in sorted(wanted-actual)]))
 assert all(not c['missing_rows'] and not c['missing_outcomes'] for c in cov)
 files=[Path(__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_audio_controls_connection.json']
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,coverage_by_routine=cov,status_counts=dict(collections.Counter(r['status'] for r in records)),dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Three original seven-byte/triple-byte/two-value command controls execute all147ownrows/26outcomes against original instructions. Independent whole-command/ring/neighbor/error/mask/callee-saved oracles cover signed low-byte validation,version-dependent arithmetic shifts/biased wrapping,source aliases andmappedSound7reads,two genuine sequential commands,full queue and late rejection preserving first command. Actual invalid source/queue/stack/privilege faults retain partial state;no helper result/checkpoint/ARMack. Producer state does not establish consumed commands/audio/gameplay.')
 (P/'analysis/movie_audio_controls_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Audio controls cases',len(records),'checks',checks)
if __name__=='__main__':main()
