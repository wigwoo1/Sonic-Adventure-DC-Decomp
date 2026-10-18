from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_tick_native_decode.py').read_text()
needle='def f32(x):'
s=s.replace(needle,"completion_helpers={}\nfor address,stem,name in [(0x8c65ba22,'sound_stream_work_complete','sa_sound_stream_work_complete'),(0x8c668af4,'sound_driver_split_range','sa_sound_driver_split_range'),(0x8c10cd38,'engine_frame_pool_queue_initialize','sa_engine_record_copy_dispatch'),(0x8c10cd8e,'engine_frame_pool_queue_initialize','sa_engine_record_copy8_tail'),(0x8c65e58a,'sound_source_commit','sa_sound_source_commit'),(0x8c65e644,'sound_source_return','sa_sound_source_return'),(0x8c10d25c,'byte_move','sa_byte_move'),(0x8c667796,'sound_stream_decode_acknowledge','sa_sound_stream_decode_acknowledge'),(0x8c6676f6,'sound_stream_total_samples_get','sa_sound_stream_total_samples_get'),(0x8c6677b6,'sound_stream_decoded_bytes_get','sa_sound_stream_decoded_bytes_get'),(0x8c6677d8,'sound_stream_decoded_samples_get','sa_sound_stream_decoded_samples_get')]:completion_helpers[address]=extra(stem+'.dll',name)\n"+needle)
s=s.replace("    struct.pack_into('<I',ram,0xa90004,0x8ca70000)",'''    struct.pack_into('<4I',ram,0xa90004,0x8ca70000,0x8cab0000,0x8cab0100,0x8cab0200)
    for offset,base,length in [(0x14,0x8ca10000,320),(0x1c,0x8ca20008,600),(0x24,0x8ca40008,600)]:struct.pack_into('<2I',ram,0xa90000+offset,base,length)
    struct.pack_into('<I',ram,0xa9003c,0xffffffff)
    struct.pack_into('<2I',ram,0xac001c,0x8c65e644,0x8c65e58a)
    objects_before=[]
    for index,address in enumerate([0xab0000,0xab0100,0xab0200]):
        base=[0x8ca10000,0x8ca20008,0x8ca40008][index]
        struct.pack_into('<12I',ram,address,0x8cac0000,1,0,0,0,0 if index==0 else 600,320 if index==0 else 0,base,1024,0,0,0)
        objects_before.append(bytes(ram[address:address+48]))''')
s=s.replace("        if address==0x8c65ba22:\n            service(address,mem,r,actual);return 0","        if address in completion_helpers:return completion_helpers[address](mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)")
s=s.replace('reference(ram,s,{0x8c65ba22:hook(0x8c65ba22)},','reference(ram,s,{},').replace('assert actual==expected==[0x8c65ba22]','assert actual==expected==[]')
s=s.replace("struct.pack_into('<I',wanted_stream,4,3)","struct.pack_into('<I',wanted_stream,4,0)")
s=s.replace("    assert native[0xa70000:0xa70100]==wanted_stream",'''    struct.pack_into('<I',wanted_stream,0x9c,0)
    assert native[0xa70000:0xa70100]==wanted_stream
    for offset,value in [(0x2c,256),(0x30,288),(0x34,256),(0x40,256),(0x44,288)]:assert struct.unpack_from('<I',native,0xa90000+offset)[0]==value
    for index,address in enumerate([0xab0000,0xab0100,0xab0200]):
        wanted=bytearray(objects_before[index])
        for offset,value in ([(0xc,32),(0x10,288),(0x18,288)] if index==0 else [(0xc,512),(0x10,88),(0x14,512)]):struct.pack_into('<I',wanted,offset,value)
        assert native[address:address+48]==wanted''')
s=s.replace('Work completion65ba22 modeled as no-op with independent work pointer,phase3 and restored-stack assertions; all stream decoding calls native.','Work completion65ba22 native with range splitting,source commit/return,metadata getters and stream acknowledgment: no modeled calls. Independent work counters and complete three source objects; encoded288 committed/32 returned,512 sample bytes per channel published/88 returned,stream phase0 and cursor9c reset.')
s=s.replace('tick_native_decode_verification','tick_decode_complete_verification').replace('dispatcher original-audio decode comparisons passed','dispatcher native decode completion comparisons passed')
(p/'verify_tick_decode_complete.py').write_text(s)
r=(p/'register_tick_native_decode.py').read_text().replace('tick_native_decode','tick_decode_complete').replace("'sa_sound_stream_phase_get'}","'sa_sound_stream_phase_get','sa_sound_stream_work_complete','sa_sound_source_commit','sa_sound_source_return','sa_sound_driver_split_range'}").replace('Native decoding in work dispatcher:','Native decoding/completion in work dispatcher:')
(p/'register_tick_decode_complete.py').write_text(r)
