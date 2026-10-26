"""Retain prior full-startup oracles and check the newly reached graphics frontier."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 parent=P/'verify_movie_midi_commands_startup.py';code=parent.read_text().replace('movie_midi_commands','movie_graphics_init')
 old="assert r['packet_native_calls']=={'0x8c643b9c':32,'0x8c608c14':1};checks+=1"
 new="assert r['packet_native_calls']['0x8c656940']==1;checks+=1\n  assert sum(b['target']=='0x8c643b9c' for b in r['boundaries'])==32 and sum(b['target']=='0x8c608c14' for b in r['boundaries'])==1;checks+=2"
 assert code.count(old)==1;code=code.replace(old,new)
 old="assert r['clear_native']['writes']==r['clear_reference']['writes']==0x200000 and r['full_vram_bytes_compared']==0x800000 and r['vram_sha256']==hashlib.sha256(bytes(0x800000)).hexdigest();checks+=4"
 new="""assert r['clear_native']['writes']==r['clear_reference']['writes']==0x200000 and r['full_vram_bytes_compared']==0x800000;checks+=3
  assert r['clear_reference']['steps']==0x200000*16 and not r['clear_native']['code_errors'] and not r['clear_native']['fpu_errors'];checks+=3
  # The original layout builder writes tile descriptors after the complete clear.
  # Events retains only the first eight writes of the checked sequential clear;
  # all ordinary subsequent VRAM writes remain present (verify_movie_clear.Events).
  assert r['vram_writes'][:8]==[[14,0xa5000000+i*4,0,4] for i in range(8)];checks+=1
  expected_vram=bytearray(0x800000)
  for event,address,value,width in r['vram_writes']:
   assert event==14 and width==4 and address&0xff800000==0xa5000000 and not address&3
   offset=address&0x7fffff;expected_vram[offset:offset+4]=value.to_bytes(4,'little')
  assert len(r['vram_writes'])==3608 and hashlib.sha256(expected_vram).hexdigest()==r['vram_sha256'];checks+=2
  r['post_clear_descriptor_writes']=len(r['vram_writes'])-8
  r['final_graphics_vram_sha256']=r['vram_sha256']"""
 assert code.count(old)==1;code=code.replace(old,new)
 code=code.replace("r['stop_pc']=='0x8c656940'","r['stop_pc']=='0x8c608d88'").replace("actual_next_frontier='0x8c656940'","actual_next_frontier='0x8c608d88'").replace("r['boundary_comparisons']==1870","r['boundary_comparisons']==4353")
 marker='files=[Path(__file__)];';assert code.count(marker)==1;code=code.replace(marker,'files=[Path(__file__),parent_script];')
 scope=dict(__name__='graphics_startup_retained_oracles',__file__=str(Path(__file__)),parent_script=parent)
 exec(compile(code,str(parent),'exec'),scope);scope['main']()
 path=P/'analysis/movie_graphics_init_startup_verification.json';r=json.loads(path.read_text());files=[Path(__file__),parent,path];prefixes=[]
 for profile in ('warm','cold'):
  previous=ROOT/f'work/sonic-adventure/movie_midi_commands_startup_{profile}_capture.json';current=ROOT/f'work/sonic-adventure/movie_graphics_init_startup_{profile}_capture.json';files.extend((previous,current))
  a=json.loads(previous.read_text())['record']['boundaries'];b=json.loads(current.read_text())['record']['boundaries']
  assert len(b)>len(a) and a[:-1]==b[:len(a)-1]
  assert all(b[len(a)-1][key]==value for key,value in a[-1].items()),profile
  assert b[len(a)-1]['target']=='0x8c656940' and len(b[len(a)-1]['graphics_input_record'])==128
  current_record=json.loads(current.read_text())['record'];summary=next(case for case in r['cases'] if case['profile']==profile)
  summary.update(post_clear_descriptor_writes=len(current_record['vram_writes'])-8,final_graphics_vram_sha256=current_record['vram_sha256'])
  prefixes.append(dict(profile=profile,previous_boundaries=len(a),current_boundaries=len(b),all_prior_boundary_fields_retained=True))
 r['prior_200_source_prefix_checks']=prefixes;r['independent_checks']+=8
 r['scope']='Two fresh uninterrupted configured 0546c0 startup comparisons retain the earlier independent full-clear, Sound-fill, real transfer/IRQ/RTE/request, flash/CRC, all 39 queue records and 32 MIDI message oracles. All prior 200-source boundary fields match exactly through entry 656940; native graphics initialization now returns and startup stops honestly at the next missing routine 608d88. The complete clear still executes33554432original instructions;3600subsequent ordinary VRAM tile-descriptor writes legitimately replace the previous all-zero final image. Replaying the retained ordinary writes over the independently checked zero clear reproduces the final full8MiB hash;this consistency check is separate from independent original/native fullmemory comparisons and does not qualify broad layout semantics. All CPU/FPU/RAM/VRAM/Sound/device/runtime/flash state and ordered boundaries compare. Explicit raw configuration is retained; no console priming, helper return or guest checkpoint is supplied. Broad graphics initializer branch/ABI domains remain unqualified and receive zero recovery credit; only separately qualified leaves count. No pixels, audible output, physical timing, reset boot or gameplay claim.'
 r['sha256'].update({str(p):digest(p) for p in files if p!=path})
 path.write_text(json.dumps(r,indent=2)+'\n');print('Verified new graphics frontier and exact old boundary prefixes',prefixes)
if __name__=='__main__':main()
