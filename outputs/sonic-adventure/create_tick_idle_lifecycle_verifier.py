from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_tick_decode_complete.py').read_text()
s=s.replace("[(0x8c65ba22,'sound_stream_work_complete'","[(0x8c65b930,'sound_stream_work_feed','sa_sound_stream_work_feed'),(0x8c65e462,'sound_source_capacity','sa_sound_source_capacity'),(0x8c6676a4,'sound_stream_format_get','sa_sound_stream_format_get'),(0x8c6676ee,'sound_stream_unit_count_get','sa_sound_stream_unit_count_get'),(0x8c65bc6c,'sound_work_unit_count_get','sa_sound_work_unit_count_get'),(0x8c65bc84,'sound_work_total_samples_get','sa_sound_work_total_samples_get'),(0x8c66771c,'sound_stream_source_set','sa_sound_stream_source_set'),(0x8c66776e,'sound_stream_start','sa_sound_stream_start'),(0x8c65ba22,'sound_stream_work_complete'")
s=s.replace("struct.pack_into('<4I',ram,0xa90004", "struct.pack_into('<I',ram,0xa70010,16)\n    struct.pack_into('<4I',ram,0xa90004")
s=s.replace("    struct.pack_into('<2I',ram,0xac001c", "    struct.pack_into('<I',ram,0xac0018,0x8c65e4b8);struct.pack_into('<I',ram,0xac0024,0x8c65e462)\n    struct.pack_into('<2I',ram,0xac001c")
s=s.replace('0x8cac0000,1,0,0,0,0 if index==0 else 600,320 if index==0 else 0','0x8cac0000,1,0,288 if index==0 else 0,0 if index==0 else 600,0 if index==0 else 600,0')
s=s.replace('[(0x8c66771c,source_set,0x8ca10000,len(payload)),','[').replace(',(0x8c66776e,start_stream,0,0)]',']')
a=s.index("    assert struct.unpack_from('<2I',native,0xa70050)");z=s.index("    assert struct.unpack_from('<4I',native,0xa70088)",a)
s=s[:a]+"    assert struct.unpack_from('<I',native,0xa70004)[0]==0\n"+s[z:]
s=s.replace('    assert native[0xa70000:0xa70100]==wanted_stream',"    struct.pack_into('<2I',wanted_stream,0x50,0x8ca10000,16);struct.pack_into('<I',wanted_stream,0x84,0)\n    assert native[0xa70000:0xa70100]==wanted_stream")
s=s.replace('[(0xc,32),(0x10,288),(0x18,288)]','[(0xc,0),(0x10,288),(0x18,288)]').replace('[(0xc,512),(0x10,88),(0x14,512)]','[(0xc,512),(0x10,688),(0x14,512)]')
s=s.replace('encoded288 committed/32 returned,','idle native feeder reserves288 encoded bytes, commits288/returns0,').replace('512 sample bytes per channel published/88 returned,','512 sample bytes per channel published/88 returned; controlled sample reservations and initial available600 yield688 (production input provider reservation not exercised),')
s=s.replace('tick_decode_complete_verification','tick_idle_lifecycle_verification').replace('dispatcher native decode completion comparisons passed','idle dispatcher native lifecycle comparisons passed')
(p/'verify_tick_idle_lifecycle.py').write_text(s)
r=(p/'register_tick_decode_complete.py').read_text().replace('tick_decode_complete','tick_idle_lifecycle').replace("'sa_sound_driver_split_range'}","'sa_sound_driver_split_range','sa_sound_stream_work_feed','sa_sound_source_capacity','sa_sound_work_unit_count_get','sa_sound_work_total_samples_get'}").replace('Native decoding/completion in work dispatcher:','Native idle feeding/decoding/completion dispatcher:')
(p/'register_tick_idle_lifecycle.py').write_text(r)
