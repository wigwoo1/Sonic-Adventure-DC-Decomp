"""Exact initializer proofs plus real-disc read/decode and original linker frontier."""
import csv,hashlib,itertools,json,shlex
from pathlib import Path
import player_texture_resource_fixtures as q
P=q.P;R=P.parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 target=P/'analysis/player_texture_resource_verification.json';assert not target.exists()
 records=[];seen=set();edges=set();checks=oracles=0;ram=q.v.seed_ram()
 def require(ok,label):
  nonlocal checks
  assert ok,label;checks+=1
 def exact(ok,label):
  nonlocal oracles
  require(ok,label);oracles+=1
 def phase(base,entry,args,label,stack=None,status=0):
  m,s,f,c=base;sp=s.r[15];s.pc=c['entry']=entry;s.pr=0x8cfed000
  for j,value in enumerate(args):s.r[4+j]=value&0xffffffff
  if stack is not None:q.put(m,sp,stack)
  if entry==q.TEXTURE_LOAD:c['instruction_budget']=25000000 # Existing host-audit option; no native/runtime guard change.
  result,path,m,s,f=q.fast.compare(m,s,f,c)
  require(result['status']==status,(label,result['status'],result['stop_pc']))
  require(result['full_state_match'] and result['ordered_boundaries_match'],'exact complete state and ordered boundaries')
  if not status:require(s.r[15]==sp and s.pc==s.pr==0x8cfed000,'balanced original return')
  c.update(other=result['integer_other'],mach=result['final_mach'],macl=result['final_macl'],gbr=result['final_gbr'],vbr=result['vbr'])
  seen.update(path)
  for i,a in enumerate(path):
   text=q.v.ROWS.get(a,'');skip=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+skip<len(path):edges.add((hex(a),hex(path[i+skip])))
  records.append(dict(label=label,entry=hex(entry),status=result['status'],stop_pc=result['stop_pc'],steps=len(path),return_value=hex(s.r[0]),boundaries=result['boundaries'],disc_read_attempts=result['disc_read_attempts'],disc_reads=result['disc_reads'],full_state_match=True,ordered_boundaries_match=True))
  return (m,s,f,c),result
 # Direct public initializer inputs. Guarded input bytes are declared workspace,
 # not a fabricated output descriptor or private execution frame.
 array,strings,obj,heap=0x8cb40000,0x8cb50000,0x8cb60000,0x8c900000
 for mode,rm in itertools.product((0,4),(0,1)):
  for count in (0,1,2,8,512,-1):
   base=q.prepare(ram,dict(mode=mode,rm=rm));m=base[0];start=array&0xffffff;size=512*12+32;m[start-16:start-16+size]=bytes([0xa5])*size
   expected=bytearray(m[start-16:start-16+size])
   for i in range(max(0,count)):expected[16+i*12:20+i*12]=(strings+28*i).to_bytes(4,'little')
   base,_=phase(base,q.DESCRIPTOR_INIT,[obj,array,strings,count],dict(api='descriptor initializer',count=count,mode=mode,rm=rm))
   exact(q.get(base[0],obj)==array,'original output descriptor array binding')
   exact(base[0][start-16:start-16+size]==expected,'independent complete guarded descriptor-array bytes')
  for count in (0,1,2,256):
   base=q.prepare(ram,dict(mode=mode,rm=rm));m=base[0];start=array&0xffffff;size=256*0x44+32;m[start-16:start-16+size]=bytes([0x5a])*size;expected=bytearray(m[start-16:start-16+size])
   for i in range(count):
    at=16+i*0x44;expected[at:at+8]=bytes([255])*8;expected[at+0x40:at+0x42]=bytes(2)
   base,_=phase(base,q.TABLE_INIT,[array,count],dict(api='texture table initializer',count=count,mode=mode,rm=rm))
   exact(base[0][start-16:start-16+size]==expected,'independent complete table bytes including untouched upper word/guards')
   exact(q.get(base[0],q.get(base[0],0x8c64e810))==array and q.get(base[0],q.get(base[0],0x8c64e814))==count,'original global table pointer and capacity')
   exact(q.get(base[0],q.get(base[0],0x8c64e818))==q.get(base[0],q.get(base[0],0x8c64e81c))==0,'original cursor/stat reset')
  for size in (0x100,0x1000,0x100000):
   base=q.prepare(ram,dict(mode=mode,rm=rm));m=base[0];sentinel=q.get(m,0x8c660f38);tag=q.get(m,0x8c660f36,2);head=q.get(m,0x8c660f3c)
   base,_=phase(base,q.HEAP_INIT,[heap,size],dict(api='SDK allocator initializer',size=size,mode=mode,rm=rm));m=base[0]
   exact(q.get(m,sentinel,2)==q.get(m,heap,2)==tag and q.get(m,sentinel+2,2)==1 and q.get(m,heap+2,2)==0,'independent allocator tags and free/used status')
   exact(q.get(m,sentinel+4)==0x24 and q.get(m,heap+4)==size,'independent sentinel/block sizes')
   exact(q.get(m,sentinel+8)==q.get(m,sentinel+12)==heap and q.get(m,heap+8)==q.get(m,heap+12)==sentinel,'independent circular allocator links')
   exact(q.get(m,head)==heap,'independent allocator first-block binding')
  print('INITIALIZERS',mode,rm,'PASS',flush=True)
 # Compose original producers from ordinary entries. File workspace allocations
 # and the entry stack are disjoint from the loader's fixed CD0/CEE scratch ranges.
 base=q.prepare(ram,dict(kind=0,real_tables=True));base[1].r[15]=0x8cfff000
 base,_=phase(base,0x8c098a10,[],'original game heap initialization')
 base,_=phase(base,0x8c098b0c,[1,0x4020],'original filesystem workspace allocation');ctx=(base[1].r[0]+31)&~31
 base,_=phase(base,0x8c098b0c,[1,0x30020],'original directory workspace allocation');table=(base[1].r[0]+31)&~31
 require(0x8c7ee8e0<=ctx<ctx+0x4000<=table<table+0x30000<=0x8c87e8e0,'genuine disjoint filesystem allocations')
 m=base[0];q.put(m,q.get(m,0x8c6000e4),0);q.put(m,0x8c673420,0)
 inventory=P/'disc_inventory.json';disc=json.loads(inventory.read_text());track=next(x for x in reversed([shlex.split(s) for s in disc['gdi'].splitlines()[1:] if s.strip()]) if x[2]=='4');q.put(m,0x8c66c530,int(track[1])+166)
 base,result=phase(base,0x8c601618,[4,ctx,4096,table],'original filesystem wrapper initialization')
 exact(base[1].r[0]==0 and result['disc_read_attempts']==21,'original successful real-disc mount and expected reads')
 name=0x8cda0000;data=b'SONICADV\0';base[0][name&0xffffff:(name&0xffffff)+len(data)]=data
 base,result=phase(base,0x8c6002a2,[name,0],'original SONICADV selection');exact(base[1].r[0]==0 and result['disc_read_attempts']==51,'actual directory selection and reads')
 data=b'EFF_REGULAR.PRS\0';base[0][name&0xffffff:(name&0xffffff)+len(data)]=data
 base,_=phase(base,0x8c6016d6,[name,0],'original EFF_REGULAR open');handle=base[1].r[0];exact(handle!=0,'actual file handle produced')
 base,_=phase(base,0x8c600e2c,[handle,q.OUT],'original file size');exact(base[1].r[0]==1 and q.get(base[0],q.OUT)==124234,'independent retail compressed file size')
 base,_=phase(base,0x8c600df4,[handle],'original close')
 base,_=phase(base,0x8c098b0c,[1,256*0x44],'original texture table allocation');texture_table=base[1].r[0];exact(texture_table!=0,'actual texture table allocation')
 base,_=phase(base,q.TABLE_INIT,[texture_table,256],'original texture table initialization')
 base,_=phase(base,q.HEAP_INIT,[heap,0x100000],'original SDK allocator initialization')
 base,_=phase(base,0x8c641160,[],'original driver default initialization')
 exact(q.get(base[0],0x8c8a2f84+3*8)==0x8c6416a4,'original default callback producer installs real RTS/NOP body')
 base,result=phase(base,q.TEXTURE_LOAD,[q.RESOURCE_NAME,q.TEXTURE_LIST],'original real-resource read and decode')
 exact(result['disc_read_attempts']==61,'actual rounded compressed-resource sector count')
 m=base[0];asset=R/'work/sonic-adventure/extracted/files/SONICADV/EFF_REGULAR.PRS';asset_hash=sha(asset);compressed=q.get(m,0x8c0998e0)-61*2048;decoded=q.get(m,0x8c0998ec)
 compressed_hash=hashlib.sha256(m[compressed&0xffffff:(compressed&0xffffff)+124234]).hexdigest();decoded_hash=hashlib.sha256(m[decoded&0xffffff:(decoded&0xffffff)+358304]).hexdigest()
 exact(compressed_hash==asset_hash=='2c62f8a4035181cb5366a8ea5332a793ae304b0a9546c43faa186e35c6d04096','complete compressed input bytes match retail asset')
 exact(decoded_hash=='686237260763d1b8a826f0949e1d3d9a2565216ab15b6418b6168e8e3e96aa0b','complete original decoded output matches independent preserved inventory')
 names=q.get(m,q.TEXTURE_LIST);texture_ids=[q.get(m,names+i*12+8) for i in range(138)];exact(any(texture_ids),'actual decoder staged numeric texture IDs')
 base,result=phase(base,0x8c608c82,[q.TEXTURE_LIST],'original texture-list linking frontier',status=-2)
 exact(result['stop_pc']=='0x8c64c374','actual missing original linker dependency reached')
 require(any(b['target']=='0x8c64c374' for b in result['boundaries']),'missing routine reached through ordinary original call chain')
 coverage=[]
 for prefix in ('player_texture_load','player_texture_table_initialize','player_texture_heap_initialize','player_texture_driver_defaults'):
  t=json.loads((P/f'analysis/{prefix}_translation.json').read_text())
  for b in t['routines']:
   wanted=set(b['reachable_addresses']);outcomes={tuple(x) for x in b['conditional_edges']};row=dict(address=b['address'],static_rows=len(wanted),executed_rows=len(wanted&set(map(hex,seen))),missing_rows=sorted(wanted-set(map(hex,seen))),static_outcomes=len(outcomes),executed_outcomes=len(outcomes&edges),unresolved_outcomes=sorted(outcomes-edges));coverage.append(row)
   if int(b['address'],16) in (q.DESCRIPTOR_INIT,q.TABLE_INIT,q.HEAP_INIT):require(not row['missing_rows'] and not row['unresolved_outcomes'],('complete initializer row/outcome coverage',row))
 files=[Path(__file__),P/'player_texture_resource_fixtures.py',P/'player_texture_driver_defaults_reference.py',P/'write_watch_player_texture_driver_defaults.py',inventory,asset,R/'work/sonic-adventure/build/player_texture_driver_defaults.dll']
 report=dict(status='complete_initializer_domains_match_and_actual_retail_read_decode_reaches_unrecovered_linker',original_native_comparisons=len(records),checks=checks,independent_exact_oracles=oracles,phases=records,coverage_by_routine=coverage,real_disc_mount_reads=21,real_directory_reads=51,real_resource_reads=61,compressed_bytes=124234,compressed_sha256=compressed_hash,decoded_bytes=358304,decoded_sha256=decoded_hash,texture_ids=texture_ids,texture_table=hex(texture_table),filesystem_context=hex(ctx),directory_table=hex(table),ordinary_entry_stack='0x8cfff000',fixed_compressed_buffer=hex(compressed),fixed_decoded_buffer=hex(decoded),sdk_workspace=['0x8c900000','0x8ca00000'],host_verification_instruction_budget=25000000,native_wait_guards_changed=False,next_dependency='0x8c64c374',frontier_status=-2,partial_frontier_execution_discarded=True,visible_textured_rendering_qualified=False,supplied_allocator_results=0,supplied_game_callee_outputs=0,supplied_private_execution_frames=0,new_original_routine_credit=0,overall_completion_percent=10,
  scope='Three complete public initializer bodies;actual original calloc/filesystem mount/directory/open/size/close/read/format/decode chain with exact retail input/output hashes. Ordinary public stack/workspace inputs avoid static scratch aliases. Existing per-case instruction_budget audit option only;CPU code/native waits unchanged. Numeric IDs are not yet linked descriptors. Original linking reaches64c374,which remains unrecovered;fault state is discarded. No asset is published.',sha256={str(p):sha(p) for p in files})
 target.write_text(json.dumps(report,indent=2)+'\n');print('TEXTURE RESOURCE COMPLETE',len(records),checks,oracles,'next64c374',flush=True)
if __name__=='__main__':main()
