"""Compare original four-byte packet instructions and independent byte oracles."""
import collections,hashlib,itertools,json
from pathlib import Path
import write_watch_midi_packet as fast
v=fast.old;P=v.P;ROOT=v.ROOT
SOUND_HASH=hashlib.sha256(bytes([0xa5])*0x200000).hexdigest()
def cases():
 for message in range(256):yield dict(message=message)
 for enabled,null,mode in itertools.product((False,True),(False,True),(0,1,4)):yield dict(enabled=enabled,null_object=null,mode=mode)
 for low,message,mode in itertools.product((1,2,3),(0x90,0xa0,0xc0,0xf0),(0,1,4)):yield dict(stack_address=0x8cf00000+low,message=message,mode=mode,research_unaligned_stack=True)
 for value,message in itertools.product((0,15,16,127,128,255),(0x80,0xa0,0xc0,0xd0,0xe0,0xf0)):yield dict(object_byte=value,message=message)
 for (a,b),message in itertools.product(((0,0),(127,255),(255,128),(0xffffff80,0x123456ff),(0xdeadbeef,0x87654321)),(0x90,0xa0,0xc0,0xf0)):yield dict(data1=a,data2=b,message=message)
 for message,mode in itertools.product((0x90,0xa0,0xf0),(0,1,4)):yield dict(message=message,trace_enabled=1,mode=mode)
 for dst,message in itertools.product((v.INPUT+1,0x8cf00000,0x8cf00001,0x8cefffe8,0x8cefffe7,0x8cf00004,v.DEST+1,0x0cd67000,0xacd67000),(0x90,0xa0,0xf0)):yield dict(destination=dst,message=message)
 for message in (0x90,0xa0,0xf0):
  yield dict(object_pointer=0xa0800000,message=message)
  yield dict(destination=0xa0800000,message=message)
  yield dict(object_pointer=0xa0800000,destination=0xa0800000,message=message)
 for extra in (dict(bad_stack=True),dict(object_pointer=0x8d000000),dict(object_pointer=0x8cffffff),dict(destination=0x8d000000),dict(destination=0x8cfffffe),dict(destination=0xa09ffffe),dict(stack_address=0x8cfffff8),dict(stack_address=0x8cfffffc)):
  for message in (0x90,0xa0,0xf0):yield dict(message=message)|extra
 for message in (0xffffff90,0x1234a3,0xffffffff):yield dict(message=message,imask=15,ts=3)

def semantics(m,a,before,fp,s,f,r,c):
 assert r['full_state_match'] and r['ordered_boundaries_match'];checks=2
 assert bytes(f)==fp and r['runtime_state'] is None and r['flash_storage_snapshot'] is None;checks+=3
 traces=[b for b in r['boundaries'] if b['target']=='0x8c65a2fc'];errors=[b for b in r['boundaries'] if b['target']==hex(v.LOGGER)]
 sp=before.r[15];dest=before.r[5];obj=before.r[4];expected=bytearray(m);sound=bytearray([0xa5])*0x200000;reads=writes=0
 def ram(addr):return addr&0x1f000000==0x0c000000 and (addr&0xffffff)<0x1000000
 def snd(addr):return 0x800000<=(addr&0x1fffffff)<0xa00000
 def read(addr):
  nonlocal reads
  if ram(addr):return expected[addr&0xffffff]
  if snd(addr):reads+=1;return sound[(addr&0x1fffffff)-0x800000]
  raise MemoryError(hex(addr))
 def write(addr,value):
  nonlocal writes
  if ram(addr):expected[addr&0xffffff]=value&255;return
  if snd(addr):writes+=1;sound[(addr&0x1fffffff)-0x800000]=value&255;return
  raise MemoryError(hex(addr))
 def word(addr):return sum(read(addr+i)<<(i*8) for i in range(4))
 expected_result=0 if c.get('enabled',True) else 0x0d000001
 if not expected_result and not obj:expected_result=0x07000103
 fault=False;expected_traces=0
 if c.get('bad_stack'):
  assert r['status']==-3 and r['stop_pc']=='0x8c643c18' and not traces and not errors;return checks+4
 if not expected_result:
  try:
   source_byte=read(obj+1);write(sp-24,source_byte);status=read(sp)
   if status&0xf0==0xf0:
    for i in range(4):write(dest+i,0)
   else:
    arg=sp+1 if (sp+1)&3==0 else sp+4
    next_arg=arg+4 if (arg+4)&3==0 else arg+8
    ptr=next_arg-4 if (next_arg-4)&3==0 else next_arg-8
    data1=word(ptr)
    if status&0xf0 in (0xa0,0xc0):data2=0
    else:
     end=next_arg+4 if (next_arg+4)&3==0 else next_arg+8
     ptr2=end-4 if (end-4)&3==0 else end-8;data2=word(ptr2)
    expected_traces+=1
    write(dest,data2);write(dest+1,data1)
    write(dest+2,(read(sp-24)<<4)|(read(sp)&15))
    write(dest+3,(read(sp)>>4)&7)
   # The actual second trace reads all packet bytes even if output is disabled.
   for i in (3,2,1,0):read(dest+i)
   expected_traces+=1
  except MemoryError:fault=True
 if expected_result:
  for literal,value in ((0x8c65acb8,v.MIDI),(0x8c65acbc,expected_result)):v.store(expected,v.load(m,literal),value)
 assert len(traces)==expected_traces and len(errors)==int(bool(expected_result));checks+=2
 if fault:assert r['status']==-3;checks+=1
 else:
  assert r['status']==0 and s.r[0]==expected_result and s.pc==s.pr==before.pr;checks+=4
  assert list(s.r[8:])==list(before.r[8:]) and s.sr&0xf0==before.sr&0xf0;checks+=2
 for start,size in ((v.DEST,32),(v.INPUT,64),(v.LIBRARY,0x1000),(v.QUEUE-16,28*256+32)):
  assert a[start&0xffffff:(start&0xffffff)+size]==expected[start&0xffffff:(start&0xffffff)+size];checks+=1
 if ram(dest):
  n=min(4,0x1000000-(dest&0xffffff));assert v.sample(a,dest,n)==v.sample(expected,dest,n);checks+=1
 for literal in (0x8c65acb8,0x8c65acbc):assert v.load(a,v.load(m,literal))==v.load(expected,v.load(m,literal));checks+=1
 assert r['sound_state']==(reads,writes,reads,writes,reads+writes,0);checks+=1
 assert r['sound_ram_sha256']==(hashlib.sha256(sound).hexdigest() if writes else SOUND_HASH);checks+=1
 return checks

def main():
 ram=v.seed_ram();records=[];coverage=set();edges=set();checks=0;aligned_coverage=set();aligned_edges=set()
 for i,case in enumerate(cases(),1):
  c=v.Input(dict(entry=v.MIDI,mode=0)|case);m,s,f=v.prepare(ram,c);before=v.effects.State.from_buffer_copy(s);fp=bytes(f)
  r,path,a,s,f=fast.compare(m,s,f,c);n=semantics(m,a,before,fp,s,f,r,c);checks+=n;coverage.update(path)
  local_edges=set()
  for j,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):local_edges.add((pc,path[j+distance]))
  edges.update(local_edges)
  if not before.r[15]&3:aligned_coverage.update(path);aligned_edges.update(local_edges)
  records.append(dict(input=dict(c),status=r['status'],stop_pc=r['stop_pc'],packet=list(v.sample(a,v.DEST,4)),independent_checks=n,boundaries=r['boundaries']));print(i,hex(c['message'] if 'message' in c else 0x90),r['status'],r['stop_pc'],flush=True)
 definition=v.manifest['routines'][0];body=set(v.PER_BODY[v.MIDI]);wanted={tuple(int(a,16) for a in e) for e in definition['conditional_edges']}
 assert body<=coverage and wanted<=edges,(sorted(body-coverage),sorted(wanted-edges))
 files=[Path(__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_midi_packet_connection.json']
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,coverage_by_routine=[dict(address=hex(v.MIDI),static_rows=len(body),covered_rows=len(body&coverage),conditional_outcomes=len(wanted),covered_outcomes=len(wanted&edges),missing_rows=[],missing_outcomes=[])],aligned_stack_covered_rows=len(body&aligned_coverage),aligned_stack_missing_rows=[hex(a) for a in sorted(body-aligned_coverage)],aligned_stack_missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-aligned_edges)],experimental_unaligned_stack_cases=sum(bool(c['input'].get('research_unaligned_stack')) for c in records),status_counts=dict(collections.Counter(c['status'] for c in records)),dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Original stack-argument four-byte MIDI packet helper uses real trace/error providers,not injected packed bytes or helper returns. All256messagebytes,source byte truncation,A0/C0singledata,F0zero,datafields,aliases,mappedSound,traceenabled and actualpartialfaults compare wholeCPU/RAM/FPU/banked/special/device/runtime/flash/boundaries plus independent byte/ring/error/mask/callee-saved/Sound oracles. Experimental misaligned RAM stack fixtures exercise emitted alignment alternatives under the existing byte-backed RAM model;they are NOT evidence of realSH4unaligned access or address-error exceptions. Aligned ABI coverage is separately reported;hardware alignment exceptions remain unfinished. Producer packing is not ARM/audio/gameplay proof.')
 (P/'analysis/movie_midi_packet_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('MIDI packet cases',len(records),'checks',checks,'aligned',report['aligned_stack_covered_rows'])
if __name__=='__main__':main()
