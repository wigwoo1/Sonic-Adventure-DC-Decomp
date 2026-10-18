from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_feed_native_start.py').read_text()
s=s.replace("(0x8c66771c,'sound_stream_source_set')","(0x8c65e4b8,'sound_source_query'),(0x8c65e644,'sound_source_return'),(0x8c65e368,'sound_source_lock_enter'),(0x8c65e380,'sound_source_lock_exit'),(0x8c10cbb8,'integer_remainder'),(0x8c66771c,'sound_stream_source_set')")
s=s.replace('        stack=0x8cf00000','        source_capacity=length if source_capacity else 0\n        length=source_capacity\n        stack=0x8cf00000')
s=s.replace('(0x18,0x8c010100)','(0x18,0x8c65e4b8)').replace('(0x1c,0x8c010300)','(0x1c,0x8c65e644)')
s=s.replace("        struct.pack_into('<I',ram,0xa8000c,source_capacity)","        struct.pack_into('<11I',ram,0xa80004,1,0,source_capacity,0,0,0,0x8ca10000,256,0,0,0)")
s=s.replace('reference(ram,s,{a:hook(a) for a in [0x8c010100,0x8c010300]},','reference(ram,s,{},')
s=s.replace('            service(address,mem,r,actual);return 0',"            raise AssertionError('Unexpected modeled call '+hex(address))")
a=s.index('        addresses=[]');z=s.index('        wanted_stream=',a)
s=s[:a]+'''        assert actual==expected==[]
        assert native[0xa60001]==(3 if ended else 0x55) and regs[15]==stack
        started=not ended and capacity//2>=4
        wanted_object=bytearray(object_before[0])
        if started:
            struct.pack_into('<I',wanted_object,0xc,0)
            struct.pack_into('<I',wanted_object,0x18,length)
        assert native[0xa80000:0xa80030]==wanted_object
        assert native[0xa80100:0xa80130]==object_before[1]
        if not early:assert struct.unpack_from('<2I',native,0xa60014)==(0x8ca10000,length)
''' +s[z:]
s=s.replace('Encoded query/return modeled; full native feeding/full playback/fullgame unverified.','Encoded query/return, interrupt helpers and signed remainder also native: no modeled calls. Source reservations independently exhausted and write position advanced on start, fully rolled back on rejection; sample object unchanged. Zero/one/36-byte reservations, controlled source words; actual file streaming/full playback/fullgame unverified.')
s=s.replace('feed_native_start_verification','feed_native_reservations_verification').replace('native source start feeder comparisons passed','native reservation feeder comparisons passed')
(p/'verify_feed_native_reservations.py').write_text(s)
r=(p/'register_feed_native_start.py').read_text().replace('feed_native_start','feed_native_reservations').replace("'sa_sound_stream_start'}","'sa_sound_stream_start','sa_sound_source_query','sa_sound_source_return','sa_sound_source_lock_enter','sa_sound_source_lock_exit','sa_integer_remainder'}").replace('Native source setup/start in feeding:','Native reservations in source feeding:')
(p/'register_feed_native_reservations.py').write_text(r)
