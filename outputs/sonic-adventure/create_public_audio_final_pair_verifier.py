from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_tick_final_pair.py').read_text()
s=s.replace("build/sound_stream_work_tick.dll')).sa_sound_stream_work_tick","build/sound_stream_work_update.dll')).sa_sound_stream_work_update")
s=s.replace("[(0x8c65b930,'sound_stream_work_feed'","[(0x8c65bb4e,'sound_stream_work_tick','sa_sound_stream_work_tick'),(0x8c65b930,'sound_stream_work_feed'")
s=s.replace("    struct.pack_into('<4I',ram,0xa90004", "    ram[0xa90001]=2\n    struct.pack_into('<4I',ram,0xa90004")
s=s.replace('s.pc=0x8c65bb4e;s.r[4]','s.pc=0x8c65bb88;s.r[4]')
s=s.replace('through native idle feeder','through public work update65bb88(state2),tick65bb4e,native idle feeder')
s=s.replace('tick_final_pair_verification','public_audio_final_pair_verification').replace('original final pair native lifecycle comparisons passed','public audio final pair comparisons passed')
s=s.replace('        assert native[address:address+48]==wanted','        assert native[address:address+48]==wanted\n    assert native[0xa90001]==2')
(p/'verify_public_audio_final_pair.py').write_text(s)
r=(p/'register_tick_final_pair.py').read_text().replace('tick_final_pair','public_audio_final_pair').replace("'sa_sound_source_query'}","'sa_sound_source_query','sa_sound_stream_work_update'}").replace('Original final pairs in native dispatcher:','Original final pairs through public native update:')
(p/'register_public_audio_final_pair.py').write_text(r)
