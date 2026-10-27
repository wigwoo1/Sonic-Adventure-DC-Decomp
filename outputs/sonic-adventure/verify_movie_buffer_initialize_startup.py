"""Retain independent startup/audio/clear oracles after the real32768B upload."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 parent=P/'verify_movie_midi_commands_startup.py';code=parent.read_text().replace('movie_midi_commands','movie_buffer_initialize')
 marker="assert r['packet_native_calls']=={'0x8c643b9c':32,'0x8c608c14':1};checks+=1";assert code.count(marker)==1
 code=code.replace(marker,"assert sum(b['target']=='0x8c643b9c' for b in r['boundaries'])==32 and sum(b['target']=='0x8c608c14' for b in r['boundaries'])==1;checks+=2")
 marker="assert r['clear_native']['writes']==r['clear_reference']['writes']==0x200000 and r['full_vram_bytes_compared']==0x800000 and r['vram_sha256']==hashlib.sha256(bytes(0x800000)).hexdigest();checks+=4";assert code.count(marker)==1
 code=code.replace(marker,"""assert r['clear_native']['writes']==r['clear_reference']['writes']==0x200000 and r['full_vram_bytes_compared']==0x800000;checks+=3
  assert r['clear_reference']['steps']==0x200000*16 and not r['clear_native']['code_errors'] and not r['clear_native']['fpu_errors'];checks+=3
  assert r['vram_writes'][:8]==[[14,0xa5000000+i*4,0,4] for i in range(8)];checks+=1
  expected_vram=bytearray(0x800000)
  for event,address,value,width in r['vram_writes']:
   assert event==14 and width==4 and address&0xff800000==0xa5000000 and not address&3
   offset=address&0x7fffff;expected_vram[offset:offset+4]=value.to_bytes(4,'little')
  assert len(r['vram_writes'])==11800 and hashlib.sha256(expected_vram).hexdigest()==r['vram_sha256'];checks+=2
  assert r['aperture_state'][0:3]==[8192,0,8192] and r['pvr_dma_state'][6]==0;checks+=2"""
 )
 code=code.replace("r['stop_pc']=='0x8c656940'","r['stop_pc']=='0x8c60edb0'").replace("actual_next_frontier='0x8c656940'","actual_next_frontier='0x8c60edb0'").replace("r['boundary_comparisons']==1870","r['boundary_comparisons']==4373")
 ns=dict(__name__='buffer_startup_retained_oracles',__file__=str(Path(__file__)));exec(compile(code,str(parent),'exec'),ns);ns['main']()
 target=P/'analysis/movie_buffer_initialize_startup_verification.json';report=json.loads(target.read_text());files=[Path(__file__),parent,P/'capture_movie_buffer_initialize_startup.py'];prefixes=[]
 for profile,count in (('warm',4373),('cold',585671)):
  old_path=ROOT/f'work/sonic-adventure/movie_render_buffer_startup_{profile}_capture.json';new_path=ROOT/f'work/sonic-adventure/movie_buffer_initialize_startup_{profile}_capture.json';old=json.loads(old_path.read_text());new=json.loads(new_path.read_text());a=old['record']['boundaries'];b=new['record']['boundaries'];r=new['record'];files.extend((old_path,new_path))
  assert len(b)==count==len(a)+7 and b[:len(a)]==a;report['independent_checks']+=3
  assert [x['target'] for x in b[len(a):]]==['0x8c6534e0','0x8c65253e','0x8c654e60','0x8c654b60','0x8c654b60','0x8c041e60','0x8c60edb0'];report['independent_checks']+=1
  assert b[len(a)]['arguments'][:3]==[0x52c000,0x8c673d94,32768] and b[len(a)+1]['arguments'][:3]==[0xa452c000,0x8c673d94,32768];report['independent_checks']+=2
  assert not r['input'].get('prime_console') and new['full_original_native_comparison'];report['independent_checks']+=2
  prefixes.append(dict(profile=profile,previous_boundaries=len(a),current_boundaries=len(b),exact_published203_boundary_prefix=True,actual_cpu_upload_bytes=32768,aperture_word_operations=8192))
 report.update(prior_203_source_prefix_checks=prefixes,actual_next_frontier='0x8c60edb0',scope='Twofresh configured0546c0original/native comparisons;fullclear/twoSoundfills/two33376Btransfers/G2IRQ/RTE/REQUEST/1065282Soundevents0drops/all39queueentries/32MIDIsends retained. Exact203boundaryprefix through65393a;actual32768byteCPUupload throughA40064bitaperture executes andstartupreaches60edb0. CorrectedISTERRW1Cpolicy explicitlynew;oldproofbytes retained. ReplayingordinaryVRAMwrites checksfinalimage consistency,notbroadgraphicssemantics. AllCPU/FPU/RAM/VRAM/Sound/platform/runtime/flash andorderedboundaries compare. No helperreturn/checkpoint/priming/shortenedclear/historiccoldsubstitution;notresetboot/hardwaretiming/gameplay/playableport. Graphicsdispatcher652210partial;root656940stillpartialtwooutcomes.')
 report['sha256'].update({str(p):digest(p) for p in files});target.write_text(json.dumps(report,indent=2)+'\n');print('STARTUP',report['independent_checks'],prefixes)
if __name__=='__main__':main()
