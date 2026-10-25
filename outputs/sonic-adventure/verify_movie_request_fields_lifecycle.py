"""Actual request field API calls retain producer writes for later consumers."""
import hashlib,json
from pathlib import Path
import verify_movie_request_fields as v
P=v.P

def main():
 records=[];checks=0
 for mode in (0,1,4):
  base=next(v.old.old.old.cases())|dict(mode=mode,entry=v.SET4,value=2);m,s,f=v.prepare(v.seed_ram(),base)
  steps=((v.SET4,2,0,0),(v.GET,v.OUTPUT,0,0),(v.BIND,v.DATA,0x11223344,0x55667788),(v.SET4,0,0,0),(v.BIND,v.DATA,0x11223344,0x55667788),(v.GET,v.OUTPUT,0,0),(v.SET8,0x123,0,0),(v.SETC,0x456,0,0),(v.PAIR,77,88,0),(v.GET,v.OUTPUT,0,0))
  hidden={}
  for index,(entry,arg5,arg6,arg7) in enumerate(steps):
   c=base|dict(entry=entry,api_step=index,value=arg5,second=arg6,third=arg7,fourth=0x99aabbcc)|hidden;s.pc=entry;s.pr=0x8cfed000;s.r[4]=v.RECORD;s.r[5]=arg5;s.r[6]=arg6;s.r[7]=arg7
   if entry==v.BIND:v.store(m,s.r[15],0x99aabbcc)
   before=v.effects.State.from_buffer_copy(s);r,path,a,s,f=v.compare(m,s,f,c);checks+=v.semantics(m,a,before,s,r,c);assert r['status']==0;checks+=1
   if index in (1,2):assert v.load(a,v.RECORD+4)==2;checks+=1
   if index==4:assert [v.load(a,v.RECORD+x) for x in (4,8,12,16,20)]==[0,v.DATA,0x11223344,0x55667788,0x99aabbcc];checks+=1
   if index==9:assert v.load(a,v.OUTPUT)==0 and [v.load(a,v.RECORD+x) for x in (8,12,16,20)]==[0x123,0x456,77,88];checks+=1
   r['guest_r0_at_stop']=hex(s.r[0]);records.append(r);m=a;hidden={k:c[k] for k in ('gbr','mach','other')}
 files=[Path(__file__),Path(v.__file__),v.MP,v.PARENT_MANIFEST,P/'analysis/movie_request_fields_connection.json']
 report=dict(cases_passed=len(records),sequences=3,calls_per_sequence=10,independent_checks=checks,dll_sha256=hashlib.sha256(v.effects.DLL.read_bytes()).hexdigest(),sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},cases=records,scope='Each sequence retains actual original/native RAM, ordinary CPU and FPU producer results across ten public ABI calls. Named special-register inputs stay fixed; MACL remains its unchanged declared input. Device/firmware services are unused by these bodies. Inputs change only explicit call arguments/entry/return link and the caller-supplied fifth stack argument. Busy rejection, then successful bind and field updates are observed without injecting callee results. This is a component API lifecycle, not uninterrupted natural startup.')
 (P/'analysis/movie_request_fields_lifecycle_verification.json').write_text(json.dumps(report,indent=2)+'\n');print(len(records),'API comparisons;',checks,'checks in three actual ten-call sequences')
if __name__=='__main__':main()
