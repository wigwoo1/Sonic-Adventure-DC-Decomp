from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_feed_native_metadata.py').read_text()
s=s.replace("(0x8c6676a4,'sound_stream_format_get')","(0x8c66771c,'sound_stream_source_set'),(0x8c66776e,'sound_stream_start'),(0x8c10ca5c,'signed_division'),(0x8c6676a4,'sound_stream_format_get')")
s=s.replace("        struct.pack_into('<H',ram,0xa700a8,fmt)","        struct.pack_into('<H',ram,0xa700a8,fmt)\n        struct.pack_into('<I',ram,0xa70004,0)\n        ram[0xa7000d:0xa70010]=bytes([16,2,18])")
s=s.replace('[0x8c010100,0x8c010300,0x8c66771c,0x8c66776e]','[0x8c010100,0x8c010300]')
s=s.replace('else [0x8c66771c,0x8c66776e]','else []')
s=s.replace('        assert native[0xa70000:0xa70100]==stream_before',"        wanted_stream=bytearray(stream_before)\n        started=not ended and capacity//2>=4\n        if started:\n            for offset,value in [(4,1),(0x50,0x8ca10000),(0x54,length//(18 if fmt==0 else 4)),(0x84,0),(0xa0,0),(0xa4,0)]:struct.pack_into('<I',wanted_stream,offset,value)\n        assert native[0xa70000:0xa70100]==wanted_stream")
s=s.replace('entire256 stream bytes independently unchanged. Encoded query/return and source/start modeled;','Source setter/start and signed division native; entire256 stream independently checked for phase1,source pointer,derived units and reset counters on start, unchanged on rejection. Encoded query/return modeled;')
s=s.replace('feed_native_metadata_verification','feed_native_start_verification').replace('native metadata feeder comparisons passed','native source start feeder comparisons passed')
(p/'verify_feed_native_start.py').write_text(s)
r=(p/'register_feed_native_metadata.py').read_text().replace('feed_native_metadata','feed_native_start').replace("'sa_sound_work_total_samples_get'}","'sa_sound_work_total_samples_get','sa_sound_stream_source_set','sa_sound_stream_start'}").replace('Native metadata in source feeding:','Native source setup/start in feeding:')
(p/'register_feed_native_start.py').write_text(r)
