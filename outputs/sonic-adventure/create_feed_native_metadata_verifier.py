from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_feed_native_capacity.py').read_text()
s=s.replace('cases=0',"helpers={}\nfor address,stem in [(0x8c6676a4,'sound_stream_format_get'),(0x8c6676ee,'sound_stream_unit_count_get'),(0x8c6676f6,'sound_stream_total_samples_get'),(0x8c65bc6c,'sound_work_unit_count_get'),(0x8c65bc84,'sound_work_total_samples_get')]:\n    helpers[address]=getattr(ctypes.CDLL(str(ROOT/'work/sonic-adventure/build'/(stem+'.dll'))),'sa_'+stem);helpers[address].argtypes=fn.argtypes\ncases=0")
s=s.replace('        object_before=',"        ram[0xa70000:0xa70100]=bytes([0x77])*256\n        struct.pack_into('<H',ram,0xa700a8,fmt)\n        struct.pack_into('<I',ram,0xa70010,4);struct.pack_into('<I',ram,0xa70018,100)\n        stream_before=bytes(ram[0xa70000:0xa70100])\n        object_before=")
s=s.replace('            service(address,mem,r,actual);return 0',"            if address in helpers:return helpers[address](mem,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)\n            service(address,mem,r,actual);return 0")
s=s.replace('0x8c010300,0x8c6676a4,0x8c65bc84,0x8c65bc6c,0x8c66771c','0x8c010300,0x8c66771c')
s=s.replace('addresses += [0x8c010100,0x8c6676a4]','addresses += [0x8c010100]').replace('            if not terminal:addresses += [0x8c65bc84]\n','').replace('            if not terminal and not exhausted:addresses += [0x8c65bc6c]\n','')
s=s.replace('        cases+=1','        assert native[0xa70000:0xa70100]==stream_before\n        cases+=1')
s=s.replace('Other subordinate methods modeled; full native feeding/full playback/fullgame unverified.','Format/unit/total getters and work wrappers also native, entire256 stream bytes independently unchanged. Encoded query/return and source/start modeled; full native feeding/full playback/fullgame unverified.')
s=s.replace('feed_native_capacity_verification','feed_native_metadata_verification').replace('native capacity feeder comparisons passed','native metadata feeder comparisons passed')
(p/'verify_feed_native_metadata.py').write_text(s)
r=(p/'register_feed_native_capacity.py').read_text().replace('feed_native_capacity','feed_native_metadata').replace("'sa_sound_source_capacity'}","'sa_sound_source_capacity','sa_sound_stream_format_get','sa_sound_stream_unit_count_get','sa_sound_stream_total_samples_get','sa_sound_work_unit_count_get','sa_sound_work_total_samples_get'}").replace('Native capacity in source feeding:','Native metadata in source feeding:')
(p/'register_feed_native_metadata.py').write_text(r)
