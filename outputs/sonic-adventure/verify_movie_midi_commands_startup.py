"""Qualify completed configured startup captures; never count a running capture."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 files=[Path(__file__)];records=[];checks=0
 dll=digest(ROOT/'work/sonic-adventure/build/movie_midi_commands.dll')
 for profile in ('warm','cold'):
  path=ROOT/f'work/sonic-adventure/movie_midi_commands_startup_{profile}_capture.json'
  assert path.exists(), 'Both completed startup captures are required'
  capture=json.loads(path.read_text());files.append(path)
  for pinned,h in capture['sha256'].items():assert digest(pinned)==h,pinned;files.append(Path(pinned))
  r=capture['record'];c=r['input']
  assert capture['dll_sha256']==dll and capture['full_original_native_comparison'] and r['full_original_native_comparison'];checks+=3
  assert r['status']==-2 and r['stop_pc']=='0x8c656940' and r['uninterrupted_declared_entry']=='0x8c0546c0';checks+=3
  assert r['full_state_match'] and r['ordered_boundaries_match'] and not c.get('prime_console');checks+=3
  assert c['full_flash_storage'] and c['flash_writable'] and c['info_bytes']==[0]*12+[49]+[0]*8+[1,0,0];checks+=3
  assert r['flash_storage_snapshot'][5][0]>=1 and r['flash_storage_snapshot'][5][1]>=5 and r['flash_storage_snapshot'][-3]>=258;checks+=3
  assert r['flash_storage_snapshot'][5][2:]==[0,0] and r['flash_storage_snapshot'][-2:]==[0,0];checks+=2
  import console_service_reference
  assert r['flash_storage_sha256']==hashlib.sha256(console_service_reference.inputs(c)[1]).hexdigest();checks+=1
  assert r['audio_library_root']!='0x0' and r['audio_queue_metadata']['capacity']==256 and r['audio_queue_metadata']['pending']==r['audio_queue_metadata']['tail']==39;checks+=4
  assert r['audio_command_bytes']==[0x8a]+[0]*19 and r['audio_queue_first_record']==[0x8a]+[0]*19+[1]+[0]*7;checks+=2
  assert r['packet_native_calls']=={'0x8c643b9c':32,'0x8c608c14':1};checks+=1
  traces=[b for b in r['boundaries'] if b['target']=='0x8c65a2fc']
  midi_traces=[b for b in traces if b['pr'] in ('0x8c643ce6','0x8c643d5a')]
  assert [b['pr'] for b in midi_traces]==['0x8c643ce6','0x8c643d5a']*32;checks+=1
  prior_path=ROOT/f'work/sonic-adventure/movie_midi_packet_startup_{profile}_capture.json';files.append(prior_path)
  prior=json.loads(prior_path.read_text())['record']
  assert r['boundaries'][:len(prior['boundaries'])]==prior['boundaries'];checks+=1
  assert len(traces)==sum(b['target']=='0x8c65a2fc' for b in prior['boundaries'])+62;checks+=1
  r['midi_trace_return_sites']=[b['pr'] for b in midi_traces];r['all_trace_calls']=len(traces)
  midis=[b for b in r['boundaries'] if b['target']=='0x8c643c18'];assert len(midis)==32 and all(int(b['stack'],16)&3==0 for b in midis);checks+=2
  sender=[b for b in r['boundaries'] if b['target']=='0x8c65a464'];assert len(sender)==39;checks+=1
  pairs=[b['arguments'][:2] for b in r['boundaries'] if b['target']=='0x8c6437f0'];assert pairs==[[0,0]];checks+=1
  triples=[b['arguments'][:3] for b in r['boundaries'] if b['target']=='0x8c643754'];assert triples==[[0,0,0xffffffd0],[1,0,0xffffffd0],[2,0,0],[3,0,0]];checks+=1
  observed_edges={tuple(edge) for edge in capture['executed_edges']}
  legacy_shift=('0x8c643786','0x8c64378a') in observed_edges
  modern_shift=('0x8c643786','0x8c643790') in observed_edges
  assert legacy_shift!=modern_shift;checks+=1
  assert legacy_shift==(profile=='warm');checks+=1
  r['audio_protocol_legacy_shift']=legacy_shift
  assert sum(b['target']=='0x8c10cd82' for b in r['boundaries'])==39;checks+=1

  assert sum(b['pr']=='0x8c643bdc' for b in sender)==32;checks+=1
  buffer_call=next(b for b in r['boundaries'] if b['target']=='0x8c608c14')
  assert r['handoff_buffer_state']==dict(pointer=buffer_call['arguments'][0],size=buffer_call['arguments'][1]&0xfffff800);checks+=1
  expected=[]
  for opcode in (0x8a,0x84,0x82):
   row=bytearray(28);row[0]=opcode;row[20]=1;expected.append(list(row))
  for channel,arg5,arg6 in triples:
   value=(arg6&255)-256 if arg6&128 else arg6&255
   if legacy_shift:value>>=3
   bias=(arg5&255)-256 if arg5&128 else arg5&255
   row=bytearray(28);row[0]=0x83;row[4]=channel;row[5]=(value+128)&255;row[6]=(bias+128)&255;row[20]=1;expected.append(list(row))
  for channel in range(32):
   row=bytearray(28);row[0]=0x86;row[6:10]=bytes((0,121,channel,3));row[20]=1;expected.append(list(row))
  assert r['audio_queue_records']==expected;checks+=1
  assert r['midi_command_bytes']==expected[-1][:20];checks+=1
  assert [item['byte1'] for item in r['midi_context_bytes']]==[0,1];checks+=1
  library=int(r['audio_library_root'],16)
  assert [int(item['address'],16) for item in r['midi_context_bytes']]==[library+8,library+16];checks+=1
  assert r['clear_native']['writes']==r['clear_reference']['writes']==0x200000 and r['full_vram_bytes_compared']==0x800000 and r['vram_sha256']==hashlib.sha256(bytes(0x800000)).hexdigest();checks+=4
  assert not r['sound_trace_dropped'];checks+=1
  if profile=='warm':
   assert r['boundary_comparisons']==1870 and r['disc_read_attempts']==0 and r['flash_storage_snapshot'][5]==[1,5,0,0] and r['flash_storage_snapshot'][-3:]==[258,0,0];checks+=4
   assert not any(r['sound_state']) and not any(r['runtime_state'][4:11]);checks+=2
  else:
   driver=ROOT/'work/sonic-adventure/extracted/files/SONICADV/MASARU.DRV';files.append(driver)
   assert r['filesystem_root_before']==capture['before_interface']=='0x0' and r['filesystem_initialized_flag']==1;checks+=3
   assert r['disc_read_attempts']==89 and r['masaru_payload_matches'] and r['masaru_bytes']==33408 and r['masaru_sha256_at_stop']==digest(driver);checks+=4
   assert list(r['runtime_state'][4:])==[0,2,2,66752,2,0,0,1] and r['original_request_calls']==r['empty_server_calls']==r['context_vector_entries']==2;checks+=4
   wanted=bytearray(b'SEGA'*0x80000);wanted[:33408-32]=driver.read_bytes()[32:]
   assert r['sound_ram_sha256']==hashlib.sha256(wanted).hexdigest();checks+=1
   assert r['sound_state']==[18,1065264,72,4261056,1065282,0] and r['sound_trace_events']==1065282;checks+=2
   assert len(r['sound_fill_calls'])==2 and all(args==[0x8c67c1c4,0xa0800000,4,0x80000] for args in r['sound_fill_calls']);checks+=2
   assert c['fifo_ready_count']==0x200000 and c['irq_samples']==[0,8]+[0]*16;checks+=2
  keys=('input','status','stop_pc','steps','boundary_comparisons','disc_read_attempts','filesystem_root_before','filesystem_root_after','filesystem_initialized_flag','flash_storage_snapshot','flash_storage_sha256','packet_native_calls','midi_trace_return_sites','all_trace_calls','audio_protocol_legacy_shift','audio_queue_records','midi_command_bytes','midi_context_bytes','handoff_buffer_state','audio_library_root','audio_queue_metadata','audio_queue_first_record','audio_command_bytes','clear_native','clear_reference','full_vram_bytes_compared','sound_state','sound_ram_sha256','sound_trace_events','sound_trace_dropped','runtime_state','exception_state','original_request_calls','empty_server_calls','context_vector_entries','sound_fill_calls','full_state_match','ordered_boundaries_match')
  records.append({k:r[k] for k in keys}|dict(profile=profile))
 assert [row['profile'] for row in records]==['warm','cold']
 report=dict(cases_passed=len(records),independent_checks=checks,cases=records,actual_next_frontier='0x8c656940',direct_console_priming_used=False,completed_profiles=[r['profile'] for r in records],erased_flash_profile_qualified=False,dll_sha256=dll,sha256={str(p):digest(p) for p in files},scope='Two completed fresh configured0546c0startup comparisons run original initialization/flash/CRC,32native MIDI packing/sender calls,39SDK/copies and the existing buffer metadata handoff before656940. All1092bytes of39pendingqueueentries,readywords,channel0..31datafields,sourcecontexts and pointer/size metadata independently checked. WholeCPU/RAM/VRAM/Sound/FPU/banked/special/runtime/device/flash/ordered boundaries match;noSounddrops. Old199boundaryprefix retained exactly;alltracecalls retained and64MIDItracecalls identified byreturnPC. Explicitrawplatforminputs,notdirectpriming/checkpoint/helperreturns/ARMack. Cold retainsfull8MiBclear,two2MiBfills,two33376-byte transfers andIRQ/RTE/REQUESTreturns. Producerstate is not ARM/audio/physicaltiming/reset/gameplay proof.')
 (P/'analysis/movie_midi_commands_startup_verification.json').write_text(json.dumps(report,indent=2)+'\n')
 print('Completed configured startup profiles',report['completed_profiles'],'checks',checks)
if __name__=='__main__':main()
