"""Retain actual queue RAM/CPU/FPU between nine explicit ABI calls."""
import hashlib,json
from pathlib import Path
import verify_movie_queue_family as direct
fast=direct.fast;v=direct.v;P=v.P

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 ram=v.seed_ram();records=[];checks=0
 operations=((v.ENQUEUE,1),(v.ENQUEUE,2),(v.ENQUEUE,3),(v.ENQUEUE,4),(v.DEQUEUE,1),(v.ENQUEUE,4),(v.DEQUEUE,2),(v.DEQUEUE,3),(v.DEQUEUE,4))
 for mode in (0,1,4):
  initial=v.Input(dict(entry=v.ENQUEUE,mode=mode,capacity=3,count=0));m,s,f=v.prepare(ram,initial)
  for index,(entry,item) in enumerate(operations):
   c=v.Input(initial|dict(entry=entry));s.pc=entry
   if entry==v.ENQUEUE:s.r[4]=item;s.r[5]=0x10000000+item
   else:s.r[4]=v.OUT1;s.r[5]=v.OUT2
   before=v.effects.State.from_buffer_copy(s);r,path,a,s,f=fast.compare(m,s,f,c);count=direct.semantics(m,a,before,s,r,c);checks+=count
   assert r['status']==0 and tuple(r['sound_state'])==(0,0,0,0,0,0),(index,r['status'],r['stop_pc'],r['sound_state']);checks+=2
   if index==3:assert s.r[0]==v.load(m,0x8c667144);checks+=1
   else:assert s.r[0]==0;checks+=1
   if entry==v.DEQUEUE:assert v.load(a,v.OUT1)==item and v.load(a,v.OUT2)==0x10000000+item;checks+=2
   records.append(dict(mode=mode,operation=index,entry=hex(entry),item=item,status=r['status'],stop_pc=r['stop_pc'],full_state_match=r['full_state_match'],ordered_boundaries_match=r['ordered_boundaries_match'],independent_checks=count));m=a
  assert v.load(m,v.ROOT_CONTEXT+0x1a4)==0 and v.load(m,v.ROOT_CONTEXT+0x19c)==v.load(m,v.ROOT_CONTEXT+0x1a0)==1;checks+=3
  assert bytes(m[v.QUEUE&0xffffff:(v.QUEUE&0xffffff)+24])==bytes(24);checks+=1
 files=[Path(__file__),Path(direct.__file__),Path(v.__file__),Path(fast.__file__),v.MP,P/'analysis/movie_queue_family_connection.json']
 report=dict(comparisons_passed=len(records),independent_checks=checks,sequences=3,calls_per_sequence=9,cases=records,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Three explicit ABI sequences preserve actual queue RAM, general CPU and FPU state between calls. Fill capacity three, reject a fourth item, dequeue one, enqueue a replacement through wraparound, then drain in FIFO order. Every call matches the original model and independent full-RAM queue/stack/error oracle. Device input state is separately initialized per ABI call; no natural interrupt-driven lifecycle or startup claim.')
 (P/'analysis/movie_queue_family_lifecycle_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Queue lifecycle',len(records),'comparisons;',checks,'checks')
if __name__=='__main__':main()
