"""Real archive loading and original allocated-particle draw in one RAM/VRAM chain."""
import collections,hashlib,json,shlex
from pathlib import Path
import player_texture_draw_fixtures as q
from decompress_prs import decompress
P=q.P;R=P.parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 target=P/'analysis/player_texture_draw_verification.json';assert not target.exists()
 frontier=P/'analysis/player_texture_draw_frontier.json';assert not frontier.exists()
 base=q.prepare(q.v.seed_ram(),dict(kind=0,real_tables=True,mode=0,rm=0));base[1].r[15]=0x8cfff000;records=[];seen=set();edges=set();checks=oracles=0
 asset=R/'work/sonic-adventure/extracted/files/SONICADV/EFF_REGULAR.PRS';decoded,consumed=decompress(asset.read_bytes());assert sha(asset)=='2c62f8a4035181cb5366a8ea5332a793ae304b0a9546c43faa186e35c6d04096';assert hashlib.sha256(decoded).hexdigest()=='686237260763d1b8a826f0949e1d3d9a2565216ab15b6418b6168e8e3e96aa0b'
 # Independent file-specific PVMH parser: checked flags109,138records with
 # ordinal16/name28/global-index32 layout. The actual game parses separately.
 assert decoded[:4]==b'PVMH' and int.from_bytes(decoded[8:10],'little')==0x109 and int.from_bytes(decoded[10:12],'little')==138
 expected_ids={int.from_bytes(decoded[12+i*34:14+i*34],'little'):int.from_bytes(decoded[42+i*34:46+i*34],'little') for i in range(138)};assert set(expected_ids)==set(range(138));refcounts=collections.Counter(expected_ids.values())
 def require(ok,label):
  nonlocal checks
  assert ok,label;checks+=1
 def exact(ok,label):
  nonlocal oracles
  require(ok,label);oracles+=1
 def proof(status,extra):
  files=[Path(__file__),P/'player_texture_draw_fixtures.py',P/'write_watch_player_texture_draw.py',P/'player_texture_link_reference.py',P/'analysis/player_texture_draw_state_verification.json',P/'disc_inventory.json',asset,R/'work/sonic-adventure/build/player_texture_link.dll']
  return dict(status=status,original_native_comparisons=len(records),checks=checks,independent_exact_oracles=oracles,phases=records,expected_texture_count=138,compressed_sha256=sha(asset),decoded_sha256=hashlib.sha256(decoded).hexdigest(),persistent_graphics_device_state=True,ordinary_entry_stack='0x8cfff000',native_DLL_unchanged=True,CPU_executor_unchanged=True,native_wait_guards_changed=False,game_callee_outputs_supplied=0,allocator_outputs_supplied=0,private_execution_frames_supplied=0,physical_GPU_pixels_qualified=False,full_game_qualified=False,new_original_routine_credit=0,overall_completion_percent=10,scope='Ordinary original heap/filesystem/GPU-memory constructor/resource loading followed by real task/player/collision/child/spawn/view/draw. Match complete original/native state andevery boundary per call;carry actual equal RAM/CPU/FPU andmatched graphics outputs at normal returns,including all8MiBVRAM/registers/storequeues/DMA. External timing/sample services retain their declared scenario inputs. Source retail images remain local. Fault state is discarded,never reentered.',sha256={str(p):sha(p) for p in files},**extra)
 def phase(entry,args,label,count=None):
  nonlocal base
  m,s,f,c=base;sp=s.r[15];s.pc=c['entry']=entry;s.pr=0x8cfed000
  for i,value in enumerate(args):s.r[4+i]=value&0xffffffff
  if count is not None:q.put(m,sp,count)
  if entry==q.TEXTURE_LOAD:c['instruction_budget']=25000000
  result,path,m,s,f=q.fast.compare(m,s,f,c);require(result['full_state_match'] and result['ordered_boundaries_match'],'complete state andall ordered boundaries');seen.update(path)
  for i,a in enumerate(path):
   text=q.v.ROWS.get(a,'');skip=2 if text.startswith(('bt/s ','bf/s ')) else 1
   if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and i+skip<len(path):edges.add((hex(a),hex(path[i+skip])))
  records.append(dict(label=label,entry=hex(entry),status=result['status'],stop_pc=result['stop_pc'],return_value=hex(s.r[0]),steps=len(path),boundaries=result['boundaries'],command_packets=result['command_packets'],disc_read_attempts=result['disc_read_attempts'],graphics_input=result['graphics_input_summary'],graphics_output=result['graphics_output_summary'],full_state_match=True,ordered_boundaries_match=True))
  print(label,result['status'],result['stop_pc'],hex(s.r[0]),len(path),flush=True)
  if result['status']:
   row=proof('original_native_matched_actual_drawing_chain_frontier',dict(frontier_label=label,frontier_status=result['status'],frontier_pc=result['stop_pc'],last_call_targets=[b['target'] for b in result['boundaries'][-12:]],partial_fault_state_discarded=True,normal_visible_draw_qualified=False));frontier.write_text(json.dumps(row,indent=2)+'\n');raise SystemExit(2)
  require(s.r[15]==sp and s.pc==s.pr==0x8cfed000,'balanced normal public return')
  c.update(graphics_in=result['graphics_out'],other=result['integer_other'],mach=result['final_mach'],macl=result['final_macl'],gbr=result['final_gbr'],vbr=result['vbr']);base=m,s,f,c;return result
 phase(0x8c098a10,[],'original game heap initialization')
 phase(0x8c098b0c,[1,0x4020],'original filesystem allocation');ctx=(base[1].r[0]+31)&~31
 phase(0x8c098b0c,[1,0x30020],'original directory allocation');directory=(base[1].r[0]+31)&~31
 m=base[0];q.put(m,q.get(m,0x8c6000e4),0);q.put(m,0x8c673420,0);inventory=json.loads((P/'disc_inventory.json').read_text());track=next(x for x in reversed([shlex.split(line) for line in inventory['gdi'].splitlines()[1:] if line.strip()]) if x[2]=='4');q.put(m,0x8c66c530,int(track[1])+166)
 r=phase(0x8c601618,[4,ctx,4096,directory],'original filesystem mount');exact(base[1].r[0]==0 and r['disc_read_attempts']==21,'actual real-disc mount')
 name=0x8cda0000;text=b'SONICADV\0';base[0][name&0xffffff:(name&0xffffff)+len(text)]=text
 r=phase(0x8c6002a2,[name,0],'original SONICADV selection');exact(base[1].r[0]==0 and r['disc_read_attempts']==51,'actual real directory selection')
 phase(0x8c098b0c,[256,0x44],'original texture table allocation');table=base[1].r[0];require(table!=0,'actual texture table allocation')
 phase(q.TABLE_INIT,[table,256],'original texture table initialization');phase(q.HEAP_INIT,[0x8c900000,0x100000],'original SDK allocator initialization')
 q.put(base[0],0x8c67c2c8,0);phase(0x8c663140,[],'original graphics allocator constructor,declared platform mode0');phase(0x8c641160,[],'original driver default installation')
 r=phase(q.TEXTURE_LOAD,[q.RESOURCE_NAME,q.TEXTURE_LIST],'original real EFF_REGULAR archive loader');exact(r['disc_read_attempts']==61,'actual61sector resource reads')
 m=base[0];compressed=q.get(m,0x8c0998e0)-61*2048;dest=q.get(m,0x8c0998ec)
 exact(hashlib.sha256(m[compressed&0xffffff:(compressed&0xffffff)+124234]).hexdigest()==sha(asset),'complete compressed bytes')
 exact(hashlib.sha256(m[dest&0xffffff:(dest&0xffffff)+len(decoded)]).hexdigest()==hashlib.sha256(decoded).hexdigest(),'complete decoded bytes')
 names=q.get(m,q.TEXTURE_LIST);exact(q.get(m,q.TEXTURE_LIST+4)==138,'actual resource list count');descriptors=[]
 for i in range(138):
  a=q.get(m,names+i*12+8);exact(table<=a<table+256*0x44 and (a-table)%0x44==0,('actual descriptor domain',i));exact(q.get(m,a)==expected_ids[i],('independent raw PVMH global-index match',i));exact(q.get(m,a+0x42,2)==0 and q.get(m,a+0x40,2)==refcounts[expected_ids[i]],('independent zeroerror/reference count',i));descriptors.append(dict(index=i,pointer=hex(a),global_index=hex(expected_ids[i]),references=q.get(m,a+0x40,2),error=0))
 texture_vram=r['graphics_out'].vram;texture_vram_hash=hashlib.sha256(texture_vram).hexdigest();exact(texture_vram!=bytes(range(256))*(8388608//256),'actual resource uploads changed VRAM')
 phase(0x8c09844a,[],'original task-pool initialization');m=base[0];m[0x799920:0x799b20]=bytes(0x200);q.put(m,q.FALLBACK_KIND,0,2);q.vector(m,q.SHAPES+8,(0,1,10))
 phase(0x8c09852e,[3,2,0,0],'original parent allocation');m=base[0];parent=base[1].r[0];primary=q.get(m,parent+0x20);require(parent and primary,'actual parent andactor');q.put(m,primary+8,0,1);q.put(m,primary+9,0,1)
 phase(0x8c06c350,[0,parent],'original player constructor');phase(q.ATTACH,[parent,q.SHAPES,2,0],'original visible-shape collision attachment',count=1)
 m=base[0];child=q.get(m,q.get(m,0x8c0986c8)+8);actor=q.get(m,child+0x20);require(child and actor,'actual child andchild actor');phase(q.get(m,child+0x10),[child],'actual stored child initializer')
 m=base[0];state=q.get(m,q.STATE_TABLE);q.put(m,state+0x1f4,2);q.put(m,state+0x1ec,q.bits(100))
 phase(q.SPAWN,[actor],'original visible particle spawn');node=base[1].r[0];exact(node!=0 and q.get(base[0],actor+0x10)==node,'actual spawned particle list')
 screen=q.get(base[0],0x8c604ce0);q.put(base[0],screen+4,q.bits(640));q.put(base[0],screen+8,q.bits(480));phase(0x8c604ba0,[],'original640x480view builder')
 exact(base[3]['graphics_in'].vram==texture_vram,'all8MiBtexture VRAM persists through original player/child/view producers')
 r=phase(q.get(base[0],child+0x14),[child],'actual stored visible particle draw with loaded textures')
 exact(r['graphics_out'].vram==texture_vram,'drawing preserves existing uploaded texture VRAM')
 phase(q.get(base[0],child+0x18),[child],'actual stored child cleanup');exact(q.get(base[0],actor+0x10)==0,'actual particle cleanup')
 report=proof('original_archive_loaded_and_allocated_particle_draw_returns_with_persistent_texture_VRAM',dict(valid_texture_descriptors=138,descriptors=descriptors,uploaded_texture_vram_sha256=texture_vram_hash,parent=hex(parent),child=hex(child),child_actor=hex(actor),spawned_node=hex(node),normal_visible_draw_qualified=True));target.write_text(json.dumps(report,indent=2)+'\n');print('TEXTURED DRAW PROOF',len(records),checks,oracles,flush=True)
if __name__=='__main__':main()
