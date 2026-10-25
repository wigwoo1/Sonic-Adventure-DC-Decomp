"""Synchronous original word copy and real callback/counter effects."""
import collections,ctypes,gc,hashlib,itertools,json,sys
from pathlib import Path
import write_watch_cpu_transfer as fast
v=fast.old;P=v.P;ROOT=v.ROOT

def initial_sound(c):return bytearray(bytes(range(256))*(0x200000//256) if c.get('sound_pattern')=='ramp' else bytes([c.get('sound_seed',0xa5)])*0x200000)

def semantics(m,a,before,s,r,path,c):
 checks=2;assert r['full_state_match'] and r['ordered_boundaries_match'];entry=c['entry'];actual=fast.namespace['journal'].regions['sound_ram']['native'].read();wanted=initial_sound(c)
 if entry==v.COUNTER:
  expected=bytearray(m);counter=v.load(m,0x8c666e4c);value=(v.load(m,counter)+1)&0xffffffff;v.store(expected,counter,value)
  regs=list(before.r);regs[4]=counter;regs[3]=value
  assert not r['status'] and a==expected and list(s.r)==regs and s.sr==before.sr and s.pr==before.pr and s.pc==before.pr;checks+=6
  assert actual==wanted and not any(r['sound_state']);return checks+2
 if entry==v.COPY:
  source=before.r[4];length=before.r[6];offset=before.r[5];aligned=not((source|length)&3)
  if not aligned:
   assert r['status']==0 and s.r[0]==v.load(m,0x8c6690f4) and not any(r['sound_state']);checks+=3
   expected=bytearray(m);sp=before.r[15]
   for off,value in ((4,before.r[14]),(8,before.r[13]),(12,before.pr),(16,offset),(20,length)):v.store(expected,sp-off,value)
   v.store(expected,v.load(m,0x8c65acb8),v.load(m,0x8c669104));v.store(expected,v.load(m,0x8c65acbc),v.load(m,0x8c6690f4))
   assert a==expected and actual==wanted;return checks+2
  expected_words=max(0,int(ctypes.c_int32(length).value/4));words=r['sound_state'][1]
  if not r['status'] or r['stop_pc']=='0x8cfef000':assert words==expected_words;checks+=1
  if c.get('partial_end'):assert r['status']==-3 and words==1;checks+=2
  for i in range(words):
   address=(source+i*4)&0xffffffff
   if 0x800000<=address&0x1fffffff<0xa00000:begin=(address&0x1fffffff)-0x800000;word=wanted[begin:begin+4]
   else:word=v.sample(m,address,4)
   target=offset+i*4;assert 0<=target<=0x1ffffc;wanted[target:target+4]=word
  assert actual==wanted and r['sound_trace_dropped']==0;checks+=2
  if r['status']==0:
   assert s.r[0]==0 and s.r[15]==before.r[15] and s.pr==before.pr;checks+=3
   assert v.load(a,v.load(m,0x8c669100))==0;checks+=1
   callback=c.get('callback',0xffffffff)
   if callback==0x8c5ff2cc and c.get('callback_argument',0x2468ace0):assert v.load(a,0x8c67340c)==c.get('callback_argument',0x2468ace0);checks+=1
   if callback==v.COUNTER:assert v.load(a,v.load(m,0x8c666e4c))==(c.get('error_counter',0)+1)&0xffffffff;checks+=1
 if entry in (v.SUBMIT,v.REQUEST) and not r['status']:
  record=before.r[4]
  if c.get('enabled',True) and record and c.get('state',0)!=2 and c.get('sdk_mode',0)==0:
   length=v.load(m,record+12);source=v.load(m,record+8)
   if not (source|length)&3:
    count=max(0,int(ctypes.c_int32(length).value/4));start=c.get('offset',0);wanted[start:start+count*4]=v.sample(m,source,count*4)
    assert s.r[0]==0 and r['sound_state'][1]==count;checks+=2
    assert v.load(a,record+4)==1 and v.load(a,v.ROOT_CONTEXT+0x190)==0;checks+=2
    if c.get('callback',0xffffffff)==0xffffffff:assert v.load(a,record+24)==1;checks+=1
  assert actual==wanted;checks+=1
 if entry==v.CLIENT_START and not r['status'] and c.get('state',0)==0 and c.get('busy',0)&1:
  assert v.load(a,v.load(m,0x8c666e4c))==(c.get('error_counter',0)+1)&0xffffffff and s.r[0]==0;checks+=2
 if entry!=v.COPY:assert r['sound_trace_dropped']==0;checks+=1
 assert a[v.SRC&0xffffff:(v.SRC&0xffffff)+64]==m[v.SRC&0xffffff:(v.SRC&0xffffff)+64];checks+=1
 return checks

def cases(smoke=False):
 base=dict(mode=0,fifo_ready_count=0x40000)
 if smoke:
  for entry in (v.COPY,v.COUNTER,v.SUBMIT,v.REQUEST,v.CLIENT_START):yield v.Input(base|dict(entry=entry))
  return
 for value,r0,mode in itertools.product((0,1,0xffffffff,0x7fffffff,0x80000000),(0,0xffffffff,0x89abcdef),(0,1,4)):
  yield v.Input(base|dict(entry=v.COUNTER,error_counter=value,r0=r0,mode=mode))
 for length,offset,mode in itertools.product((0,4,8,64,256,0xfffffffc,0x80000000),(0,4,256),(0,1,4)):
  yield v.Input(base|dict(entry=v.COPY,length=length,offset=offset,mode=mode))
 for source_low,length_low in itertools.product(range(4),range(4)):
  if source_low or length_low:yield v.Input(base|dict(entry=v.COPY,source=v.SRC+source_low,length=64+length_low))
 for callback,arg,mode in itertools.product((0,0xffffffff,0x8c5ff2cc,v.COUNTER,0x8cfef000),(0,0x2468ace0),(0,1,4)):
  yield v.Input(base|dict(entry=v.COPY,callback=callback,callback_argument=arg,mode=mode))
 for mask,bank in itertools.product(range(16),(0,1)):
  yield v.Input(base|dict(entry=v.SUBMIT,initial_sr=0x40008302|(mask<<4)|(bank<<29)))
 for entry,mode in itertools.product((v.SUBMIT,v.REQUEST,v.CLIENT_START),(0,1,4)):
  for extra in (dict(enabled=False),dict(record=0),dict(state=2),dict(state=0)):
   yield v.Input(base|dict(entry=entry,mode=mode)|extra)
 for mode in (0,1,4):yield v.Input(base|dict(entry=v.CLIENT_START,busy=1,error_counter=0xffffffff,mode=mode))
 for extra in (dict(source=0,length=0),dict(source=0,length=4),dict(source=0x0c900000),dict(source=0xac900000),dict(source=0x8cfffffc,length=8),dict(offset=0x200000,length=4),dict(offset=0x1ffffc,length=8,partial_end=True),dict(offset=1),dict(stack=0x8c000000),dict(fifo_samples=[]),dict(fifo_samples=[0]*64,length=4096),dict(sound_unconfigured=True)):
  yield v.Input(base|dict(entry=v.COPY)|extra)
 for source,offset in ((0xa0800000,4),(0xa0800008,0)):
  yield v.Input(base|dict(entry=v.COPY,source=source,offset=offset,length=16,sound_pattern='ramp'))
 for entry in (v.SUBMIT,v.REQUEST):
  yield v.Input(base|dict(entry=entry,length=33376,source_pattern='masaru'))

def main():
 ram=v.seed_ram();records=[];coverage=set();edges=set();checks=0
 for c in cases('--smoke' in sys.argv):
  m,s,f=v.prepare(ram,c);before=v.effects.State.from_buffer_copy(s);r,path,a,s,f=fast.compare(m,s,f,c);count=semantics(m,a,before,s,r,path,c);checks+=count;coverage.update(path)
  for i,pc in enumerate(path):
   text=v.ROWS.get(pc,'');distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+distance<len(path):edges.add((pc,path[i+distance]))
  keys=('input','status','stop_pc','steps','boundary_comparisons','full_state_match','ordered_boundaries_match','sound_state','sound_ram_sha256','sound_trace_sha256','sound_trace_events','sound_trace_dropped','write_watch')
  records.append({k:r[k] for k in keys}|dict(independent_checks=count,guest_r0=hex(s.r[0]),guest_sr=hex(s.sr)));print(hex(c['entry']),r['status'],r['stop_pc'],r['sound_state'][1],flush=True);del r,path,a;gc.collect()
 if '--smoke' in sys.argv:print('CPU transfer smoke passed',len(records));return
 rows=[]
 for entry in sorted(v.NEW|{v.SUBMIT,v.CLIENT_START,v.REQUEST}):
  body=set(v.PER_BODY[entry]);path=P/'analysis/movie-queue-family'/f'{entry:08x}_instructions.tsv'
  if entry in v.NEW:definition=next(r for r in v.manifest['routines'] if int(r['address'],16)==entry)
  elif entry==v.REQUEST:definition=next(r for r in json.loads((P/'analysis/movie_bulk_family_translation.json').read_text())['routines'] if int(r['address'],16)==entry)
  else:definition=next(r for r in json.loads((P/'analysis/movie_queue_family_translation.json').read_text())['routines'] if int(r['address'],16)==entry)
  wanted={tuple(int(x,16) for x in e) for e in definition['conditional_edges']}
  rows.append(dict(address=hex(entry),static_rows=len(body),covered_rows=len(body&coverage),missing_rows=[hex(x) for x in sorted(body-coverage)],conditional_edges_covered=len(wanted&edges),missing_edges=[[hex(a),hex(b)] for a,b in sorted(wanted-edges)]))
 files=[Path(__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_cpu_transfer_connection.json',P/'analysis/movie_transfer_start_qualification.json',v.DRIVER]
 report=dict(cases_passed=len(records),independent_checks=checks,status_counts=dict(collections.Counter(r['status'] for r in records)),coverage_by_routine=rows,cases=records,dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Original synchronous provider/callback and counter with true existing word-copy calls. Independent entire sound-RAM effects, signed-count/alignment behavior, sequential overlap, complete counter RAM/registers, error RAM, callback effects and closed SDK submission/request results. Driver payload tests use explicit initialized ABI inputs and exact locally supplied original bytes, not a natural reset/interrupt claim. Parent qualification remains separate.')
 (P/'analysis/movie_cpu_transfer_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('CPU comparisons',len(records),checks,report['status_counts']);print(rows)
if __name__=='__main__':main()
