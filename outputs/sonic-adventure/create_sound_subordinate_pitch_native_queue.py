from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_sound_driver_pitch_native_enqueue.py').read_text()
s=s.replace("[(0x8c66b504", "[(0x8c6683be,'sound_driver_pitch','sa_sound_driver_pitch'),(0x8c667b7a,'sound_error_report','sa_sound_error_report'),(0x8c65acac,'engine_error_record','sa_engine_error_record'),(0x8c66b504")
s=s.replace('entry=0x8c6683be;handle=0x8ca08000','entry=0x8c65ddb2;handle=0x8ca06000')
s=s.replace("build/sound_driver_pitch.dll')).sa_sound_driver_pitch", "build/sound_subordinate_pitch.dll')).sa_sound_subordinate_pitch")
needle=" struct.pack_into('<I',ram,0xa08008,channels)"
i=s.index(needle)
s=s[:i]+" ram[0xa06000:0xa06090]=bytes([0x55])*144\n struct.pack_into('<I',ram,0xa06040,0x8ca08000);upper_before=bytes(ram[0xa06000:0xa06090])\n struct.pack_into('<2I',ram,0x67c37c,0,0)\n struct.pack_into('<2I',ram,0x67c1c8,0x12345678,0x9abcdef0)\n"+s[i:]
s=s.replace("  calls.append((a,r[4],r[5],r[6],r[7]));r[0]=answer;return 0", "  raise AssertionError(hex(a))")
s=s.replace("hooks={a:hook(a) for a in addresses if a not in (entry,wrapper_target,division_target,*helpers)}", "hooks={}")
s=s.replace(' assert calls==events and canonical_calls==[x[1:] for x in wanted]', ' assert canonical_calls==[x[1:] for x in wanted]')
s=s.replace(" assert len([x for x in calls if x[0]==error_target])==(channels if result else 0)", " wanted_upper=bytearray(upper_before);struct.pack_into('<2I',wanted_upper,0x80,coarse&0xffffffff,fine&0xffffffff)\n assert native[0xa06000:0xa06090]==wanted_upper\n assert r[15]==0x8cf00000")
s=s.replace('sound_driver_pitch_native_enqueue_verification.json','sound_subordinate_pitch_native_queue_verification.json')
a=s.index("scope='");z=s.index("'),indent=2",a)
s=s[:a]+"scope='588 connected native pitch setter/driver/wrapper/conversion/request/queue/copies/error reporter/diagnostic comparisons. Channels0/1/3, seven signed coarse/fine thresholds, both FPU banks, empty/full queues. Complete retail RAM/CPU/FPU/SR/PR/MACL agreement without modeled services. Independent144-byte upper object cache, pitch conversion, range rejection,20-byte packet/record tags/cursors and queue rejection preservation. Callback unset; hardware playback and game caller integration remain unverified.'"+s[z+1:]
s=s.replace('driver pitch native enqueue comparisons','subordinate pitch native queue comparisons')
(p/'verify_sound_subordinate_pitch_native_queue.py').write_text(s)
reg=(p/'register_sound_subordinate_pan_native_diagnostics.py').read_text().replace('sound_subordinate_pan','sound_subordinate_pitch').replace('native_diagnostics','native_queue').replace('==252','==588').replace('sa_sound_driver_pan','sa_sound_driver_pitch').replace('sa_sound_pan_request','sa_sound_pitch_request')
(p/'register_sound_subordinate_pitch_native_queue.py').write_text(reg)
