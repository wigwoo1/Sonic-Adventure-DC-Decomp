"""Retain full startup oracles under the corrected SYSINFO_INIT firmware ABI."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 parent=P/'verify_movie_midi_commands_startup.py';code=parent.read_text().replace('movie_midi_commands','movie_slot_initialize')
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
  assert r['aperture_state'][0:3]==[8192,0,8192] and r['pvr_dma_state'][6]==0;checks+=2""")
 code=code.replace("r['stop_pc']=='0x8c656940'","r['stop_pc']=='0x8c010b00'").replace("actual_next_frontier='0x8c656940'","actual_next_frontier='0x8c010b00'").replace("r['boundary_comparisons']==1870","r['boundary_comparisons']==4648")
 code=code.replace("r['flash_storage_snapshot'][5]==[1,5,0,0]","r['flash_storage_snapshot'][5]==[2,9,0,0]").replace("r['flash_storage_snapshot'][-3:]==[258,0,0]","r['flash_storage_snapshot'][-3:]==[514,0,0]")
 ns=dict(__name__='system_state_startup_oracles',__file__=str(Path(__file__)));exec(compile(code,str(parent),'exec'),ns);ns['main']()
 target=P/'analysis/movie_slot_initialize_startup_verification.json';report=json.loads(target.read_text());files=[Path(__file__),parent,P/'capture_movie_slot_initialize_startup.py'];prefixes=[]
 for profile,count,prior_stage in (('warm',4648,'movie_system_state'),('cold',585946,'movie_system_state')):
  path=ROOT/f'work/sonic-adventure/movie_slot_initialize_startup_{profile}_capture.json';previous=ROOT/f'work/sonic-adventure/{prior_stage}_startup_{profile}_capture.json';new=json.loads(path.read_text());old=json.loads(previous.read_text());r=new['record'];a=old['record']['boundaries'];b=r['boundaries'];files.extend((path,previous))
  assert len(b)==count and b[:len(a)]==a;report['independent_checks']+=2
  # The recorded boundary fields match; this does not claim old/new R0 or
  # scratch RAM were identical under the intentionally different firmware ABI.
  assert r['packet_native_calls']=={'0x8c109cfe':1,'0x8c107ec0':4,'0x8c108808':1,'0x8c1087d2':4,'0x8c602ab4':4,'0x8c602998':4,'0x8c646c8a':4};report['independent_checks']+=1
  raw=[x for x in b if x['target']=='0x8c00f120'];calls=[sum(x['arguments'][3]==op for x in raw) for op in range(4)];read_bytes=sum(x['arguments'][2] for x in raw if x['arguments'][3]==1)
  assert calls==r['flash_storage_snapshot'][5]==[2,9,0,0] and read_bytes==r['flash_storage_snapshot'][-3]==514;report['independent_checks']+=2
  query=next(x for x in b if x['target']=='0x8c5ff39c');packet=next(x for x in b[b.index(query)+1:] if x['target']=='0x8c64bddc');assert packet['arguments'][:2]==[0,0x8c900000];report['independent_checks']+=1
  prefixes.append(dict(profile=profile,prior_source_stage=prior_stage,prior_boundaries=len(a),current_boundaries=len(b),exact_prior_recorded_boundary_fields=True,full_old_machine_state_identity_claimed=False,actual_query_buffer=hex(packet['arguments'][1]),actual_flash_calls=calls,actual_flash_read_bytes=read_bytes))
 report.update(prior_boundary_field_checks=prefixes,compiled_source_count=210,actual_next_frontier='0x8c010b00',scope='Two fresh210-source uninterrupted configured0546c0 warm/cold startups retain all existing independent Sound/flash/VRAM/transfer/IRQ/SDK/MIDI oracles and exact old209 recorded boundary prefixes. Complete original/native machine/ordered-boundary states match. Original four-record initialization, metadata decoding, status predicates and table getters execute without host result substitution. Both reach next real missing010b00. Earlier proofs retained; not physical reset/boot or playable-game proof.')
 report['sha256'].update({str(p):digest(p) for p in files});target.write_text(json.dumps(report,indent=2)+'\n');print('SLOT STARTUP',report['independent_checks'],prefixes)
if __name__=='__main__':main()
