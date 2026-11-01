"""Verify original static-buffer input construction and every status accessor."""
import csv,hashlib,json
from pathlib import Path
import renderer_filesystem_irq_fixtures as q
from verify_renderer_filesystem_irq import carry
P=Path(__file__).resolve().parent;R=P.parents[1]

def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 target=P/'analysis/renderer_input_constructor_verification.json';assert not target.exists()
 binary=R/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=binary.read_bytes()
 listing=P/'analysis/movie-heap-caller/8c010180_instructions.tsv'
 rows=list(csv.DictReader(listing.open(),delimiter='\t'))
 for row in rows:
  address=int(row['address'],16);data=bytes.fromhex(row['bytes'])
  assert raw[address-0x8c010000:address-0x8c010000+len(data)]==data
 m,s,f,c=q.prepare(q.v.seed_ram(),dict(kind=0,real_tables=True,mode=0,rm=0))
 s.r[15]=0x8cfff000;s.sr&=~0xf0;c['vbr']=0x8ce50000;q.v.bind_vectors(c['vbr'])
 records=[];checks=0
 def require(ok,label):
  nonlocal checks
  assert ok,label;checks+=1
 def phase(entry,args,label,stack=None):
  nonlocal m,s,f,c
  sp=s.r[15];s.pc=c['entry']=entry;s.pr=0x8cfed000
  for i,value in enumerate(args):s.r[4+i]=value&0xffffffff
  if stack is not None:q.put(m,sp,stack)
  r,path,m,s,f=q.fast.compare(m,s,f,c)
  require(r['status']==0 and s.r[15]==sp and s.pc==s.pr==0x8cfed000,'balanced original/native normal return')
  require(r['full_state_match'] and r['ordered_boundaries_match'],'complete state and ordered boundary equality')
  records.append(dict(label=label,entry=hex(entry),status=r['status'],stop_pc=r['stop_pc'],steps=len(path),full_state_match=True,ordered_boundaries_match=True,boundaries=r['boundaries'],input_root=hex(q.get(m,0x8c8a83b4)),return_value=hex(s.r[0])))
  carry(c,r);print(label,r['status'],hex(s.r[0]),len(path),flush=True);return r
 require(q.get(m,0x8c8a83b4)==0,'raw image has no constructed input context')
 phase(0x8c645de0,[0,0x8ce40000],'original vector initialization')
 phase(0x8c602128,[0x8cb00000,0x100000],'original system heap')
 phase(0x8c604702,[1,0,1,15],'original display/library initialization')
 phase(0x8c604dce,[-1,0,0x20000,0],'original allocated polygon context',0)
 require(q.get(m,0x8c8a83b4)==0,'renderer initialization does not construct input context')
 phase(0x8c646868,[0],'negative pre-initialization accessor control')
 require(s.r[0]==0x44c,'uninitialized accessor returns invalid low status offset')
 first=q.get(m,0x8c0102cc);second=q.get(m,0x8c0102c8)
 require((first,second)==(0x8c67c680,0x8c6886a0),'original caller literal values')
 r=phase(0x8c601a34,[0,first,second],'original game/Maple initializer with actual caller arguments')
 root=q.get(m,0x8c8a83b4)
 require(root==q.get(m,0x8c65ecb0)==0x8c8a72b8,'constructor selected original static context')
 require(q.get(m,root+0xfc4)==0xac6886a0 and q.get(m,root+0xfc8)==0xac67c680,'constructor wrote actual uncached buffer aliases')
 require(all(any(b['target']==hex(a) for b in r['boundaries']) for a in (0x8c646304,0x8c65ebb0,0x8c65f558)),'actual nested constructors executed')
 formatted=bytearray(120);formatted[20:26]=b'(none)';formatted[26:51]=b' '*25
 for channel in range(24):
  # 646868 saves PR on the stack and reloads it; the saved bytes remain.
  # Assert that exact write as well as preservation of every other RAM byte.
  expected=bytearray(m);q.put(expected,s.r[15]-4,0x8cfed000)
  phase(0x8c646868,[channel],'constructed controller-status accessor '+str(channel))
  status=root+0x44c+channel*0x78
  require(s.r[0]==status,'independent status-address arithmetic')
  require(m==expected,'exact PR-save write and preservation of all other RAM')
  require(bytes(m[status&0xffffff:(status&0xffffff)+120])==formatted,'independent complete absent-device status format')
  record=q.get(m,0x8c601ae8)+channel*0x34
  require(q.get(m,record+0x30)==status,'game input record retains actual SDK status pointer')
 require(len(records)==30,'complete declared comparison inventory')
 files=[Path(__file__),P/'renderer_filesystem_irq_fixtures.py',P/'renderer_filesystem_irq_reference.py',P/'write_watch_renderer_filesystem_irq.py',P/'verify_renderer_filesystem_irq.py',P/'analysis/renderer_filesystem_callback_qualification.json',P/'analysis/movie-revision-lower/8c646868_instructions.tsv',listing,binary,R/'work/sonic-adventure/build/renderer_filesystem_callback.dll']
 report=dict(status='original_input_constructor_and_all_24_status_accessors_verified',original_native_comparisons=len(records),normal_returns=len(records),checks=checks,controller_channels=24,caller_instruction_rows_byte_checked=len(rows),input_context_root=hex(root),caller_first_buffer=hex(first),caller_second_buffer=hex(second),native_DLL_unchanged=True,native_game_C_unchanged=True,new_original_routine_credit=0,allocator_outputs_supplied=0,game_callee_outputs_supplied=0,input_context_pointer_supplied=False,private_execution_frames_supplied=0,full_game_qualified=False,physical_GPU_pixels_qualified=False,overall_completion_percent=10,scope='Original 601a34 executes existing 646304/65ebb0/65f558. Mode zero and fixed buffer arguments match byte-checked original 010180 literal/call sites; the complete 010180 boot wrapper is not executed here. The caller buffers are original static storage, not invented allocator outputs. Raw image starts with null root; only genuine constructors write it. Complete original/native CPU/FPU/RAM/VRAM/device/boundary equality is checked. All 24 status records and their game-level pointer fields are independently checked. Declared external peripheral profile and no connected controller; no device-input packet, reset boot or pixel qualification.',phases=records,sha256={str(p):sha(p) for p in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('VERIFIED',len(records),'comparisons',checks,'checks; zero new routine credit',flush=True)

if __name__=='__main__':main()
