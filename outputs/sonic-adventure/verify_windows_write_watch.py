"""Independent OS tracking, lifetime, exact-delta and corruption controls."""
import ctypes,gc,hashlib,json,random,threading
from pathlib import Path
from windows_write_watch import WatchedMemory,Window,BoundaryJournal,PAGE
P=Path(__file__).resolve().parent;checks=0

def check(value):
 global checks
 assert value;checks+=1

def rejects(kind,fn):
 global checks
 try:fn()
 except kind:checks+=1
 else:raise AssertionError('Expected '+kind.__name__)

def tracking():
 for size in (1,PAGE-3,PAGE,PAGE+1,PAGE*9+37):
  a=WatchedMemory(size);check(a.pages()==[]);v=a.view;check(len(v)==size and not any(v))
  # First-touch demand-zero reads may conservatively report pages on Windows.
  check(set(a.pages())<=set(range((size+PAGE-1)//PAGE)))
  check(not any(v) and a.pages()==[])
  v[0]=17;check(a.pages()==[0]);check(a.pages()==[])
  v[size-1]=23;check(a.pages()==[(size-1)//PAGE])
  ctypes.memmove(a.address+max(0,size-4),b'\xa7'*min(4,size),min(4,size))
  check(a.pages()==list(range(max(0,size-4)//PAGE,(size-1)//PAGE+1)))
  v[:]=bytes(size);check(a.pages()==list(range((size+PAGE-1)//PAGE)))
  ctypes.memmove(a.address,b'\0',1);check(a.pages()==[0])
  v[0]=1;v[0]=0;check(a.pages()==[0])
  rejects(BufferError,a.close);alias=(ctypes.c_uint8*size).from_buffer(v);v.release();del v
  rejects(BufferError,a.close);del alias;gc.collect();a.close()
  rejects(ValueError,lambda:a.view);rejects(ValueError,a.pages);rejects(ValueError,lambda:Window(a))
 a=WatchedMemory(PAGE*3);v=a.view;ctypes.memmove(a.address+PAGE-2,b'1234',4);check(a.pages()==[0,1]);v.release();del v;a.close()
 for size in (0,-1,True,1.5,1<<31):rejects(ValueError,lambda:WatchedMemory(size))
 a=WatchedMemory(PAGE);errors=[]
 t=threading.Thread(target=lambda:thread_rejection(a,errors));t.start();t.join();check(errors==[True])
 rejects(ValueError,lambda:a.pages(-1,1));rejects(ValueError,lambda:Window(a,0,0));a.close()

def thread_rejection(a,errors):
 try:a.pages()
 except RuntimeError:errors.append(True)

def journal_cases():
 randomizer=random.Random(0x65a8c8);metrics=[]
 for legacy in (False,True):
  for ro,no,size in ((0,0,PAGE*3),(137,PAGE-11,PAGE*3+59)):
   a=WatchedMemory(ro+size+PAGE);b=WatchedMemory(no+size+PAGE);x=Window(a,ro,size);y=Window(b,no,size)
   initial=bytes(randomizer.getrandbits(8) for _ in range(size));ctypes.memmove(a.address+ro,initial,size);ctypes.memmove(b.address+no,initial,size)
   j=BoundaryJournal(legacy);j.reference_region('memory',x);j.native_region('memory',y);ops=[];tokens=[];full=[]
   rejects(ValueError,lambda:j.reference_region('other',x));rejects(ValueError,lambda:j.native_region('memory',y))
   for step in range(97):
    changes=[]
    for _ in range(step%5):
     count=randomizer.randrange(1,80);offset=randomizer.randrange(size-count+1);data=bytes(randomizer.getrandbits(8) for _ in range(count))
     ctypes.memmove(a.address+ro+offset,data,count);changes.append((offset,data))
    ops.append(changes);full.append(x.read());tokens.append(j.reference_token('memory'))
   for i,changes in enumerate(ops):
    for offset,data in changes:ctypes.memmove(b.address+no+offset,data,len(data))
    # A native write outside the window can mark a shared edge page without
    # changing the guest bytes. Conservative logical-page expansion must work.
    if no:ctypes.memmove(b.address,b'\x9a',1)
    check(j.native_token('memory')==tokens[i]);check(y.read()==full[i]);j.check();check(not j.errors)
   metrics.append(j.finish());check(x.read()==y.read());a.close();b.close()
 for corruption in (0,PAGE-1,PAGE,PAGE*2-1):
  a=WatchedMemory(PAGE*2);b=WatchedMemory(PAGE*2);j=BoundaryJournal();j.reference_region('x',Window(a));j.native_region('x',Window(b))
  j.reference_token('x');ctypes.memmove(b.address+corruption,b'\x7f',1);j.native_token('x');rejects(AssertionError,j.check);a.close();b.close()
 a=WatchedMemory(PAGE);b=WatchedMemory(PAGE);j=BoundaryJournal();j.reference_region('x',Window(a));j.native_region('x',Window(b))
 j.reference_token('x');j.reference_token('x');j.native_token('x');rejects(AssertionError,j.finish)
 j.native_token('x');j.native_token('x');rejects(AssertionError,j.check);a.close();b.close()
 a=WatchedMemory(PAGE);b=WatchedMemory(PAGE);ctypes.memmove(b.address,b'x',1);j=BoundaryJournal();j.reference_region('x',Window(a));rejects(AssertionError,lambda:j.native_region('x',Window(b)));a.close();b.close()
 return metrics

def main():
 tracking();metrics=journal_cases();files=[Path(__file__),P/'windows_write_watch.py']
 report=dict(independent_checks=checks,page_bytes=PAGE,randomized_sequences=4,epochs_per_sequence=97,full_bytes_compared_at_every_randomized_epoch=True,expected_corruption_and_boundary_errors_rejected=True,metrics=metrics,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Verification-only Windows write tracking and exact memory journal. Python/native writes, equal/restored writes, crossing pages, partial/unaligned windows, export lifetime and wrong-thread/invalid inputs are tested. Every randomized epoch also compares complete bytes independently. Game execution integration is a separate required gate; no game routine credit.')
 (P/'analysis/windows_write_watch_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Windows write-watch checks passed:',checks)
if __name__=='__main__':main()
