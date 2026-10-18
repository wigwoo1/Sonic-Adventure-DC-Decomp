from pathlib import Path
p=Path(__file__).resolve().parent
name='sound_stream_phase_get'
(p/f'build_{name}.ps1').write_text((p/'build_sound_source_buffer_get.ps1').read_text().replace('sound_source_buffer_get',name))
s=(p/'verify_sound_source_buffer_get.py').read_text().replace('sound_source_buffer_get',name).replace('0x8c65e798','0x8c667718').replace("struct.pack_into('<I',ram,0xa2001c,count)","struct.pack_into('<I',ram,0xa20004,count)")
s=s.replace('two-instruction source buffer getter8c65e798','stream phase getter8c667718')
(p/f'verify_{name}.py').write_text(s)
r=(p/'register_sound_source_buffer_get.py').read_text().replace('sound_source_buffer_get',name).replace('0x8c65e798','0x8c667718').replace('0x8c65e79c','0x8c66771c').replace('0x64e798','0x657718').replace('0x64e79c','0x65771c')
(p/f'register_{name}.py').write_text(r)
s=(p/'verify_feed_native_reservations.py').read_text()
s=s.replace("build/sound_stream_work_feed.dll')).sa_sound_stream_work_feed","build/sound_stream_work_tick.dll')).sa_sound_stream_work_tick")
s=s.replace("(0x8c65e4b8,'sound_source_query')","(0x8c65b930,'sound_stream_work_feed'),(0x8c667718,'sound_stream_phase_get'),(0x8c65e4b8,'sound_source_query')")
a=s.index('        def service(');z=s.index('        def hook(',a)
s=s[:a]+'''        def service(address,mem,r,trace):
            assert address==0x8c667ad4 and r[4]==0x8ca70000
            trace.append(address)
''' +s[z:]
s=s.replace("            raise AssertionError('Unexpected modeled call '+hex(address))","            service(address,mem,r,actual);return 0")
s=s.replace('s.pc=0x8c65b930','s.pc=0x8c65bb4e').replace('reference(ram,s,{},','reference(ram,s,{0x8c667ad4:hook(0x8c667ad4)},').replace('assert actual==expected==[]','assert actual==expected==[0x8c667ad4]')
a=s.index("scope='");z=s.index("'),indent=2)",a)
s=s[:a]+"scope='256 retail RAM/CPU/FPU/SR/PR comparisons through dispatcher65bb4e and native feeder65b930, native phase/metadata getters,capacity/query/return/locks/remainder/division/source setter/start. Independent stream source/phase/reset fields, source reservation rollback/advance and entire source/sample objects checked. Stream update modeled as no-op: confirms idle dispatcher feeding and subsequent update call only; decode/completion dispatch/full playback/fullgame unverified."+s[z:]
s=s.replace('feed_native_reservations_verification','tick_native_feed_verification').replace('native reservation feeder comparisons passed','native feeder dispatcher comparisons passed')
(p/'verify_tick_native_feed.py').write_text(s)
r=(p/'register_feed_native_reservations.py').read_text().replace('feed_native_reservations','tick_native_feed').replace("'sa_integer_remainder'}","'sa_integer_remainder','sa_sound_stream_phase_get','sa_sound_stream_work_tick'}").replace('Native reservations in source feeding:','Native feeder in dispatcher:')
(p/'register_tick_native_feed.py').write_text(r)
