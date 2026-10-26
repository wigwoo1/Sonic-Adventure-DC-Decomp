"""Original MIDI sender and startup handoff: real writes,queues and faults."""
import collections,hashlib,itertools,json
from pathlib import Path
import write_watch_midi_commands as fast
v=fast.old;P=v.P;ROOT=v.ROOT
SOUND_HASH=hashlib.sha256(bytes([0xa5])*0x200000).hexdigest()
def cases():
 for enabled,null,count,tail,mode in itertools.product((False,True),(False,True),(0,255,256,0xffffffff),(0,255),(0,1,4)):
  yield dict(entry=v.SEND,enabled=enabled,null_object=null,queued=count,tail=tail,mode=mode)
 for source,mode in itertools.product((v.INPUT,0x0cd66000,0xacd66000,v.SEND_COMMAND,v.SEND_COMMAND+3,v.SEND_COMMAND+5,v.SEND_COMMAND+6,v.SEND_COMMAND+7,v.SEND_COMMAND+9,v.QUEUE+20),(0,1,4)):
  yield dict(entry=v.SEND,packet_source=source,mode=mode)
 for packet,mode in itertools.product(([0]*4,[255]*4,[128,127,1,165],[0,1,2,3]),(0,1,4)):yield dict(entry=v.SEND,packet=packet,mode=mode)
 for obj in (1,0x8d000000,0xffffffff):yield dict(entry=v.SEND,object_pointer=obj,mode=0)
 for imask,ts in itertools.product(range(16),(0,3)):yield dict(entry=v.SEND,imask=imask,ts=ts,mode=0)
 for extra,mode in itertools.product((dict(bad_stack=True),dict(unprivileged=True),dict(packet_source=0x8d000000),dict(packet_source=0x8cfffffe),dict(library_pointer=0x8d000000),dict(queue_pointer=0x8d000000),dict(queue_pointer=0x8bffffec)),(0,1,4)):
  yield dict(entry=v.SEND,mode=mode)|extra
 for source,queue,mode in itertools.product((v.INPUT,0xa0800000),(v.QUEUE,0xa0800000),(0,1,4)):yield dict(entry=v.SEND,packet_source=source,queue_pointer=queue,mode=mode)
 for queue in (v.SEND_COMMAND,v.SEND_COMMAND-4,v.SEND_COMMAND-20):yield dict(entry=v.SEND,queue_pointer=queue,mode=0)
 for capacity,tail in ((0,0),(1,0),(1,1),(2,1)):yield dict(entry=v.SEND,ring_capacity=capacity,tail=tail,mode=0)
 for size,mode in itertools.product((0,1,2047,2048,2049,0x3ffff,0x40000,0x40001,0x7fffffff,0x80000000,0xfffff800,0xffffffff),(0,1,4)):yield dict(entry=v.HANDOFF,buffer_size=size,mode=mode)
 for pointer in (0,1,0x8d000000,0xffffffff):yield dict(entry=v.HANDOFF,buffer_pointer=pointer,buffer_size=0x40001,mode=0)
 for ts in range(4):yield dict(entry=v.HANDOFF,buffer_size=0xffffffff,ts=ts,mode=0)
 yield dict(entry=v.HANDOFF,bad_stack=True,mode=0)

def semantics(m,a,before,fp,s,f,r,c):
 assert r['full_state_match'] and r['ordered_boundaries_match'];checks=2
 assert bytes(f)==fp and r['runtime_state'] is None and r['flash_storage_snapshot'] is None;checks+=3
 if c['entry']==v.HANDOFF:
  expected=bytearray(m);v.store(expected,0x8c88f7c8,before.r[4]);v.store(expected,0x8c88f7cc,before.r[5]&0xfffff800)
  assert r['status']==0 and s.pc==s.pr==before.pr and bytes(f)==fp;checks+=4
  assert hashlib.sha256(a).digest()==hashlib.sha256(expected).digest();checks+=1
  assert s.r[0]==before.r[0] and s.r[4]==before.r[4] and s.r[5]==before.r[5]&0xfffff800;checks+=3
  assert list(s.r[8:])==list(before.r[8:]) and s.sr==before.sr&~1;checks+=2
  assert len(r['boundaries'])==1 and not any(r['sound_state']);checks+=2
  return checks
 assert c['entry']==v.SEND
 calls=[b for b in r['boundaries'] if b['target']==hex(v.SDK)];copies=[b for b in r['boundaries'] if b['target']==hex(v.COPY)];logs=[b for b in r['boundaries'] if b['target']==hex(v.LOGGER)]
 if c.get('bad_stack'):
  assert r['status']==-3 and r['stop_pc']=='0x8c643b9e' and not calls and not copies and not logs;return checks+5
 expected=bytearray(m);sound=bytearray([0xa5])*0x200000;reads=read_bytes=writes=write_bytes=0
 def ram(addr,n=1):return addr&0x1f000000==0x0c000000 and (addr&0xffffff)<=0x1000000-n
 def snd(addr,n=1):return 0x800000<=(addr&0x1fffffff)<=0xa00000-n and not addr&(n-1)
 def read(addr,n=1):
  nonlocal reads,read_bytes
  addr&=0xffffffff
  if ram(addr,n):return int.from_bytes(expected[addr&0xffffff:(addr&0xffffff)+n],'little')
  if snd(addr,n):reads+=1;read_bytes+=n;return int.from_bytes(sound[(addr&0x1fffffff)-0x800000:(addr&0x1fffffff)-0x800000+n],'little')
  raise MemoryError(hex(addr))
 def write(addr,value,n=1):
  nonlocal writes,write_bytes
  addr&=0xffffffff;data=(value&((1<<(8*n))-1)).to_bytes(n,'little')
  if ram(addr,n):expected[addr&0xffffff:(addr&0xffffff)+n]=data;return
  if snd(addr,n):writes+=1;write_bytes+=n;sound[(addr&0x1fffffff)-0x800000:(addr&0x1fffffff)-0x800000+n]=data;return
  raise MemoryError(hex(addr))
 root=read(v.ROOT_WORD,4);result=0 if root else 0x0d000001
 if not result and not before.r[4]:result=0x07000103
 attempts=copy_count=0;host=0;mask=before.sr&0xf0;queue=None;entry=None
 try:
  if not result:
   write(v.SEND_COMMAND,0x86,4)
   for index in range(4):write(v.SEND_COMMAND+6+index,read(before.r[5]+index))
   attempts+=1
   if c.get('unprivileged'):raise PermissionError()
   mask=0xf0;meta=root+0x158;count=read(meta+20,4)
   if count==256:result=0x0a000001;mask=before.sr&0xf0
   else:
    queue=read(meta+12,4);tail=read(meta+24,4);capacity=read(meta+16,4);next_tail=(tail+1)&0xffffffff
    if next_tail==capacity:next_tail=0
    write(meta+24,next_tail,4);entry=(queue+tail*28)&0xffffffff;write(entry+20,1,4);copy_count+=1
    previous=read(v.SEND_COMMAND+16,4)
    for incoming,outgoing in ((12,16),(8,12),(4,8),(0,4)):
     following=read(v.SEND_COMMAND+incoming,4);write(entry+outgoing,previous,4);previous=following
    write(entry,previous,4);write(meta+20,(count+1)&0xffffffff,4);mask=before.sr&0xf0
 except MemoryError:host=-3
 except PermissionError:host=-4
 if result:
  write(v.load(m,0x8c65acb8),v.SEND,4);write(v.load(m,0x8c65acbc),result,4)
 assert r['status']==host and s.sr&0xf0==mask;checks+=2
 assert len(calls)==attempts and len(copies)==copy_count and len(logs)==(2 if result==0x0a000001 else 1 if result else 0);checks+=3
 if not host:assert s.r[0]==result and s.pc==s.pr==before.pr and list(s.r[8:])==list(before.r[8:]);checks+=3
 for start,size in ((v.SEND_COMMAND-32,96),(v.LIBRARY,0x1000),(v.QUEUE-16,28*256+32),(v.INPUT,64)):
  assert v.sample(a,start,size)==v.sample(expected,start,size);checks+=1
 if entry is not None and ram((entry+20)&0xffffffff,4):assert v.load(a,(entry+20)&0xffffffff)==v.load(expected,(entry+20)&0xffffffff);checks+=1
 for literal in (0x8c65acb8,0x8c65acbc):assert v.load(a,v.load(m,literal))==v.load(expected,v.load(m,literal));checks+=1
 assert r['sound_state']==(reads,writes,read_bytes,write_bytes,reads+writes,0);checks+=1
 assert r['sound_ram_sha256']==(hashlib.sha256(sound).hexdigest() if writes else SOUND_HASH);checks+=1
 return checks

def main():
 ram=v.seed_ram();records=[];coverage=set();edges=set();checks=0
 for i,case in enumerate(cases(),1):
  c=v.Input(case);m,s,f=v.prepare(ram,c);before=v.effects.State.from_buffer_copy(s);fp=bytes(f)
  r,path,a,s,f=fast.compare(m,s,f,c);n=semantics(m,a,before,fp,s,f,r,c);checks+=n;coverage.update(path)
  for j,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((pc,path[j+distance]))
  records.append(dict(input=dict(c),status=r['status'],stop_pc=r['stop_pc'],independent_checks=n,boundaries=r['boundaries']));print(i,hex(c['entry']),r['status'],r['stop_pc'],flush=True)
 cov=[]
 for definition in v.manifest['routines']:
  body=set(v.PER_BODY[int(definition['address'],16)]);wanted=set(map(tuple,definition['conditional_edges']));actual={(hex(a),hex(b)) for a,b in edges}
  cov.append(dict(address=definition['address'],static_rows=len(body),covered_rows=len(body&coverage),conditional_outcomes=len(wanted),covered_outcomes=len(wanted&actual),missing_rows=[hex(a) for a in sorted(body-coverage)],missing_outcomes=[list(e) for e in sorted(wanted-actual)]))
 assert all(not row['missing_rows'] and not row['missing_outcomes'] for row in cov)
 files=[Path(__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_midi_commands_connection.json']
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,coverage_by_routine=cov,status_counts=dict(collections.Counter(r['status'] for r in records)),dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Original MIDI sender and startup handoff execute actual native instructions/providers. Command86 stores original four packet bytes at sparse offsets6..9 then realSDK/copy/tag/count/tail/mask/error paths. Aliases,full/overflow/wrap/null/non-dereferenced object values,Sound/source/queue/root/stack/privilege faults retain reached partial state. Independent byte/pipelinedcopy/ring/Sound/error/mask oracles supplement fullCPU/RAM/FPU/banked/special/device/runtime/flash/orderedboundarycomparison. Handoff stores rawpointer and size&fffff800,not allocation;wholeRAM/register/FPU invariants checked. No command bytes/helperresults/ARMack/checkpoint injected;no physicaltiming/audio/gameplayclaim.')
 (P/'analysis/movie_midi_commands_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('MIDI commands cases',len(records),'checks',checks)
if __name__=='__main__':main()
