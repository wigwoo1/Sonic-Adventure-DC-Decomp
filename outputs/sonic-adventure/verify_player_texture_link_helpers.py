"""Full original/native proofs for public filename,cursor and handle-error helpers."""
import csv,hashlib,itertools,json
from pathlib import Path
import player_texture_link_fixtures as q
P=q.P;R=P.parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 target=P/'analysis/player_texture_link_helpers_verification.json';assert not target.exists()
 ram=q.v.seed_ram();records=[];seen=set();edges=set();checks=oracles=0
 def require(ok,label):
  nonlocal checks
  assert ok,label;checks+=1
 def exact(ok,label):
  nonlocal oracles
  require(ok,label);oracles+=1
 def phase(base,entry,args,label):
  m,s,f,c=base;sp=s.r[15];s.pc=c['entry']=entry;s.pr=0x8cfed000
  for i,value in enumerate(args):s.r[4+i]=value&0xffffffff
  result,path,m,s,f=q.fast.compare(m,s,f,c)
  require(result['status']==0,(label,result['status'],result['stop_pc']))
  require(result['full_state_match'] and result['ordered_boundaries_match'],'complete state and ordered calls')
  require(s.r[15]==sp and s.pc==s.pr==0x8cfed000,'balanced original return')
  seen.update(path)
  for i,a in enumerate(path):
   text=q.v.ROWS.get(a,'');skip=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+skip<len(path):edges.add((hex(a),hex(path[i+skip])))
  records.append(dict(label=label,entry=hex(entry),steps=len(path),status=0,return_value=hex(s.r[0]),boundaries=result['boundaries'],full_state_match=True,ordered_boundaries_match=True));return m,s
 strings=(b'',b'a',b'abcdefghijklmnopqrstuvwxyz',b'Eff_Regular.Pvr',b'UPPER_case-0123',bytes(range(1,256)))
 cursor=q.get(ram,0x8c64e5d8);extent=q.get(ram,0x8c64e5dc)
 for mode,rm in itertools.product((0,4),(0,1)):
  for text,alignment in itertools.product(strings,range(4)):
   base=q.prepare(ram,dict(mode=mode,rm=rm));m=base[0];pointer=0x8cb40010+alignment;start=(pointer&0xffffff)-16;size=16+len(text)+1+16;m[start:start+size]=bytes([0xa5])*size;m[start+16:start+16+len(text)+1]=text+b'\0';expected=bytearray(m[start:start+size]);expected[16:16+len(text)]=bytes(x-32 if 97<=x<=122 else x for x in text)
   m,s=phase(base,0x8c64c320,[pointer],dict(api='in-place uppercase filename',length=len(text),alignment=alignment,mode=mode,rm=rm))
   exact(m[start:start+size]==expected,'independent complete uppercase bytes,terminator,and guards')
  for origin,amount,(position,length) in itertools.product((-1,0,1,2,3),(-1,0,17),((0,0x100),(0xfffffff0,0x1234))):
   base=q.prepare(ram,dict(mode=mode,rm=rm));m=base[0];start=min(cursor,extent)&0xffffff;start-=16;size=40;m[start:start+size]=bytes([0xa5])*size;q.put(m,cursor,position);q.put(m,extent,length);expected=bytearray(m[start:start+size]);value=amount if origin==0 else position+amount if origin==1 else length-amount if origin==2 else position;at=(cursor&0xffffff)-start;expected[at:at+4]=(value&0xffffffff).to_bytes(4,'little')
   m,s=phase(base,0x8c64e58e,[amount,origin],dict(api='SDK file cursor',origin=origin,amount=amount,position=position,length=length,mode=mode,rm=rm))
   exact(m[start:start+size]==expected,'independent absolute/relative/end cursor,32-bit wrap,invalid-origin no-op,and neighboring bytes')
  for active,error in itertools.product((0,1,0xffff),(0,1,0x7fff,0x8000,0xffff)):
   base=q.prepare(ram,dict(mode=mode,rm=rm));pointer=0x8cb50000;q.put(base[0],pointer+0x4c,active,2);q.put(base[0],pointer+0x52,error,2);before=hashlib.sha256(base[0]).hexdigest()
   m,s=phase(base,0x8c601500,[pointer],dict(api='file-handle error',active=active,error=error,mode=mode,rm=rm));value=error if error<0x8000 else error-0x10000
   exact(s.r[0]==((value if active else 0)&0xffffffff),'independent active-handle signed16 error')
   exact(hashlib.sha256(m).hexdigest()==before,'independent entire RAM unchanged by getter')
  base=q.prepare(ram,dict(mode=mode,rm=rm));before=hashlib.sha256(base[0]).hexdigest();m,s=phase(base,0x8c601500,[0],dict(api='null file handle',mode=mode,rm=rm));exact(s.r[0]==0 and hashlib.sha256(m).hexdigest()==before,'null handle returns zero without RAM writes')
  print('HELPERS',mode,rm,'PASS',len(records),flush=True)
 t=json.loads((P/'analysis/player_texture_link_translation.json').read_text());coverage=[];wanted={'0x8c64c320','0x8c64e58e','0x8c601500'}
 for b in t['routines']:
  addresses=set(b['reachable_addresses']);outcomes={tuple(x) for x in b['conditional_edges']};row=dict(address=b['address'],static_rows=len(addresses),executed_rows=len(addresses&set(map(hex,seen))),missing_rows=sorted(addresses-set(map(hex,seen))),static_outcomes=len(outcomes),executed_outcomes=len(outcomes&edges),unresolved_outcomes=sorted(outcomes-edges));coverage.append(row)
  if b['address'] in wanted:require(not row['missing_rows'] and not row['unresolved_outcomes'],('complete helper rows/outcomes',row))
 files=[Path(__file__),P/'player_texture_link_fixtures.py',P/'player_texture_link_reference.py',P/'write_watch_player_texture_link.py',P/'analysis/player_texture_link_translation.json',R/'work/sonic-adventure/build/player_texture_link.dll']
 report=dict(status='three_complete_public_texture_helpers_match_independent_oracles_and_original_full_state',original_native_comparisons=len(records),checks=checks,independent_exact_oracles=oracles,phases=records,coverage_by_routine=coverage,qualified_candidates=sorted(wanted),supplied_game_callee_outputs=0,supplied_private_execution_frames=0,new_original_routine_credit=0,visible_textured_rendering_qualified=False,overall_completion_percent=10,scope='Public uppercase conversion on empty,mixed,all nonzero byte strings at four alignments;SDK absolute/relative/end/invalid cursor modes with wrapped arithmetic;null/inactive/active signed16 handle errors. Both FPU banks and nearest/zero rounding. No private frame or callee result supplied. Main linker remains unqualified.',sha256={str(p):sha(p) for p in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('HELPER PROOF',len(records),checks,oracles,flush=True)
if __name__=='__main__':main()
