"""Original mode requests and real20-byte command enqueue,including failures."""
import collections,hashlib,itertools,json
from pathlib import Path
import write_watch_audio_mode as fast
v=fast.old;P=v.P;ROOT=v.ROOT

def cases():
 for audio,enabled,queued,tail,mode in itertools.product((v.MONO,v.STEREO),(False,True),(0,256),(0,255),(0,1,4)):
  yield dict(entry=v.MODE,audio_mode=audio,enabled=enabled,queued=queued,tail=tail,mode=mode)
 for audio,mode in itertools.product((0,1,0xffffffff,0x80000000,v.MONO^1),(0,1,4)):
  yield dict(entry=v.MODE,audio_mode=audio,mode=mode)
 for queued,tail,mode in itertools.product((0,255,256,257,0xffffffff),(0,255),(0,1,4)):
  yield dict(entry=v.SDK,queued=queued,tail=tail,mode=mode)
 for imask,ts in itertools.product(range(16),(0,3)):
  yield dict(entry=v.SDK,imask=imask,ts=ts,mode=0)
 for capacity,tail in ((0,0),(1,0),(1,1),(2,1)):
  yield dict(entry=v.SDK,ring_capacity=capacity,tail=tail,mode=0)
 for mode in (0,1,4):
  yield dict(entry=v.SDK,enabled=False,mode=mode)
  for source in (0x0cd66000,0xacd66000,v.QUEUE,v.QUEUE+4,v.QUEUE+20):
   yield dict(entry=v.SDK,source=source,mode=mode)
  for entry in (v.MODE,v.SDK):
   yield dict(entry=entry,bad_stack=True,mode=mode)
   yield dict(entry=entry,unprivileged=True,mode=mode)
  for source in (0x8d000000,0x8cfffff8,0x8bfffff8):
   yield dict(entry=v.SDK,source=source,mode=mode,imask=3)
  yield dict(entry=v.SDK,queue_pointer=0x8d000000,mode=mode,imask=7)
  yield dict(entry=v.SDK,queue_pointer=0xa0800000,mode=mode,imask=7)
  yield dict(entry=v.SDK,library_pointer=0x8d000000,mode=mode,imask=5)

def semantics(m,a,before,fp,s,f,r,path,c):
 assert r['full_state_match'] and r['ordered_boundaries_match'];checks=2
 sound_queue=c.get('queue_pointer')==0xa0800000
 assert (sound_queue or not any(r['sound_state'])) and r['runtime_state'] is None and r['flash_storage_snapshot'] is None;checks+=3
 meta=v.LIBRARY+0x158;count=c.get('queued',0);tail=c.get('tail',0)
 copies=[b for b in r['boundaries'] if b['target']==hex(v.COPY)]
 logs=[b for b in r['boundaries'] if b['target']==hex(v.LOGGER)]
 if r['status']:
  assert r['status'] in (-3,-4);checks+=1
  if c.get('bad_stack') or c.get('unprivileged'):
   assert not copies and not logs and s.sr&0xf0==before.sr&0xf0;checks+=3
   assert a[v.QUEUE&0xffffff:(v.QUEUE&0xffffff)+0x1c*256]==m[v.QUEUE&0xffffff:(v.QUEUE&0xffffff)+0x1c*256];checks+=1
  elif 'source' in c:
   assert len(copies)==1 and not logs and v.load(a,meta+20)==count and v.load(a,meta+24)==tail+1 and v.load(a,v.QUEUE+tail*28+20)==1;checks+=5
   assert s.sr&0xf0==0xf0;checks+=1
  elif 'queue_pointer' in c:
   assert not copies and not logs and v.load(a,meta+20)==count and v.load(a,meta+24)==tail+1 and s.sr&0xf0==0xf0;checks+=5
  return checks
 assert s.pc==s.pr==before.pr and list(s.r[8:])==list(before.r[8:]) and bytes(f)==fp;checks+=4
 assert s.sr&0xf0==before.sr&0xf0;checks+=1
 if sound_queue:
  wanted=bytearray([0xa5])*0x200000;wanted[:20]=m[v.INPUT&0xffffff:(v.INPUT&0xffffff)+20];wanted[20:24]=(1).to_bytes(4,'little')
  assert s.r[0]==0 and v.load(a,meta+20)==count+1 and v.load(a,meta+24)==tail+1;checks+=3
  assert r['sound_ram_sha256']==hashlib.sha256(wanted).hexdigest() and r['sound_state']==(0,6,0,24,6,0);checks+=2
  assert len(copies)==1 and not logs and copies[0]['sr']&0xf0==0xf0;checks+=3
  assert a[v.QUEUE&0xffffff:(v.QUEUE&0xffffff)+28*256]==m[v.QUEUE&0xffffff:(v.QUEUE&0xffffff)+28*256];checks+=1
  return checks
 expected=bytearray(m);enabled=c.get('enabled',True);entry=c['entry'];audio=c.get('audio_mode',v.STEREO);result=0
 source=before.r[4]
 if not enabled:result=0x0d000001
 elif entry==v.MODE:
  v.store(expected,v.COMMAND,0x8a)
  if audio not in (v.MONO,v.STEREO):result=0x03000001
  else:
   expected[(v.COMMAND+4)&0xffffff]=255 if audio==v.MONO else 0
   source=v.COMMAND
 if not result and count==256:result=0x0a000001
 if not result:
  index=(tail+1)&0xffffffff
  if index==c.get('ring_capacity',256):index=0
  v.store(expected,meta+24,index)
  destination=v.QUEUE+tail*28;v.store(expected,destination+20,1)
  # Independent data oracle follows the already recovered pipelined descending
  # copy,including source aliases with the ready word or destination payload.
  previous=v.load(expected,source+16)
  for incoming,outgoing in ((12,16),(8,12),(4,8),(0,4)):
   following=v.load(expected,source+incoming);v.store(expected,destination+outgoing,previous);previous=following
  v.store(expected,destination,previous);v.store(expected,meta+20,(count+1)&0xffffffff)
  assert len(copies)==1 and not logs and copies[0]['sr']&0xf0==0xf0;checks+=3
  assert any(b['target']=='0x8c10cd82' for b in r['boundaries']);checks+=1
 else:
  last=entry;v.store(expected,v.load(m,0x8c65acb8),last);v.store(expected,v.load(m,0x8c65acbc),result)
  expected_logs=2 if entry==v.MODE and enabled and audio in (v.MONO,v.STEREO) and count==256 else 1
  assert not copies and len(logs)==expected_logs;checks+=2
 assert s.r[0]==result;checks+=1
 for first,size in ((v.LIBRARY,0x1000),(v.QUEUE-16,28*256+32),(v.COMMAND,32),(v.INPUT,64)):
  assert a[first&0xffffff:(first&0xffffff)+size]==expected[first&0xffffff:(first&0xffffff)+size];checks+=1
 for literal in (0x8c65acb8,0x8c65acbc):assert v.load(a,v.load(m,literal))==v.load(expected,v.load(m,literal));checks+=1
 assert all(b['sr']&0xf0==before.sr&0xf0 for b in logs);checks+=1
 return checks

def main():
 ram=v.seed_ram();records=[];coverage=set();edges=set();checks=0
 for i,case in enumerate(cases(),1):
  c=v.Input(case);m,s,f=v.prepare(ram,c);before=v.effects.State.from_buffer_copy(s);fp=bytes(f)
  r,path,a,s,f=fast.compare(m,s,f,c);n=semantics(m,a,before,fp,s,f,r,path,c);checks+=n;coverage.update(path)
  for j,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):edges.add((pc,path[j+distance]))
  records.append(dict(input=dict(c),status=r['status'],stop_pc=r['stop_pc'],r0=hex(s.r[0]),sr=hex(s.sr),boundaries=r['boundaries'],independent_checks=n))
  print(i,hex(c['entry']),r['status'],r['stop_pc'],flush=True)
 coverage_rows=[]
 for definition in v.manifest['routines']:
  body=set(v.PER_BODY[int(definition['address'],16)]);wanted=set(map(tuple,definition['conditional_edges']));actual={(hex(a),hex(b)) for a,b in edges}
  coverage_rows.append(dict(address=definition['address'],static_rows=len(body),covered_rows=len(body&coverage),missing_rows=[hex(a) for a in sorted(body-coverage)],conditional_outcomes=len(wanted),covered_outcomes=len(wanted&actual),missing_outcomes=[list(x) for x in sorted(wanted-actual)]))
 assert all(not row['missing_rows'] and not row['missing_outcomes'] for row in coverage_rows)
 files=[Path(__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_audio_mode_connection.json']
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=coverage_rows,new_own_rows=120,new_own_conditional_outcomes=16,shared_copy_fragment_rows=11,shared_copy_additional_credit=0,dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Original MONO/STRO mode setter andSDKproducer execute all120ownrows/16outcomes plus11existing shared copy-tail rows. Actual20-byte command copies,ready words,ring counts/indices,wrapping and signed/overflow count inputs compare. Independent byte/alias/command/error/mask/callee-saved/FPU oracles supplement full original CPU/RAM/FPU/banked/special/bus/runtime/flash/boundary state. Failed copies retain advanced tail/ready/partial bytes and raisedIMASK;successful/full/uninitialized returns restoremaskbeforeactualdiagnostic stores. No helper result,ARMacknowledgement,guestcheckpointorcompletedqueue is supplied. Enqueue alone does not establish ARM/audio/timing/gameplay.')
 (P/'analysis/movie_audio_mode_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print('Audio mode comparisons',len(records),'independentchecks',checks)
if __name__=='__main__':main()
