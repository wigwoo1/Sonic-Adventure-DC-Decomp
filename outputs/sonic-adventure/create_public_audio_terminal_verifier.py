from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_feed_original_endings.py').read_text()
s=s.replace("build/sound_stream_work_feed.dll')).sa_sound_stream_work_feed","build/sound_stream_work_update.dll')).sa_sound_stream_work_update")
s=s.replace("[(0x8c65e4b8,'sound_source_query')","[(0x8c65bb4e,'sound_stream_work_tick'),(0x8c65b930,'sound_stream_work_feed'),(0x8c667718,'sound_stream_phase_get'),(0x8c667ad4,'sound_stream_update_dispatch'),(0x8c667a5c,'sound_stream_magic_update'),(0x8c644c16,'file_decode_phase_get'),(0x8c65e4b8,'sound_source_query')")
s=s.replace("itertools.product(assets,[False,True],[0,1])","itertools.product(assets,[True],[0,1])")
s=s.replace("ram[0xa60003]=stop","ram[0xa60003]=stop;ram[0xa60001]=2")
s=s.replace("        struct.pack_into('<I',ram,0xa70004,0)","        struct.pack_into('<I',ram,0xa70004,0)\n        struct.pack_into('<I',ram,0xa70008,0x8caa0000)\n        ram[0xaa0000:0xaa0040]=bytes([0x66])*64;struct.pack_into('<I',ram,0xaa000c,0)\n        handle_before=bytes(ram[0xaa0000:0xaa0040])")
s=s.replace('s.pc=0x8c65b930','s.pc=0x8c65bb88')
s=s.replace('        cases+=1',"        assert native[0xaa0000:0xaa0040]==handle_before\n        cases+=1")
a=s.index("scope='");z=s.index("'),indent=2)",a)
s=s[:a]+"scope='216 retail RAM/CPU/FPU/SR/PR comparisons of public work update65bb88 through native tick/feed/update dispatch/magic idle check, using actual18-byte terminal markers from108 original valid audio endings,both banks. Original final pair SHA256 and marker bytes validated. Independent work state3, full encoded reservation rollback, entire source/sample objects and512-byte encoded buffer preserved, stream phase0/source fields unchanged, entire64-byte decoder handle unchanged. No modeled calls; no decode requested. Controlled idle stream/handle fixture, full-file histories/streaming/playback/fullgame unverified; SSRACING invalid header ending excluded."+s[z:]
s=s.replace('feed_original_endings_verification','public_audio_terminal_verification').replace('original ending feeder comparisons passed','public audio terminal comparisons passed')
(p/'verify_public_audio_terminal.py').write_text(s)
r=(p/'register_feed_original_endings.py').read_text().replace('feed_original_endings','public_audio_terminal').replace('== 432','== 216').replace("'sa_integer_remainder'}","'sa_integer_remainder','sa_sound_stream_work_update','sa_sound_stream_work_tick','sa_sound_stream_phase_get','sa_sound_stream_update_dispatch','sa_sound_stream_magic_update','sa_file_decode_phase_get'}").replace('Original audio endings in native feeding:','Original audio markers through public update:')
(p/'register_public_audio_terminal.py').write_text(r)
