from pathlib import Path
root=Path(__file__).resolve().parent
text=(root/'verify_sound_scheduled_decode.py').read_text()
text=text.replace("fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_decode_request_process.dll')).sa_file_decode_request_process", "fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_stream_update_dispatch.dll')).sa_sound_stream_update_dispatch")
text=text.replace('kernel.argtypes=fn.argtypes', "kernel.argtypes=fn.argtypes\ndef extra(dll,name):\n    target=getattr(ctypes.CDLL(str(ROOT/'work/sonic-adventure/build'/dll)),name);target.argtypes=fn.argtypes;return target\ndecoder=extra('file_decode_request_process.dll','sa_file_decode_request_process')\nmagic=extra('sound_stream_magic_update.dll','sa_sound_stream_magic_update')\nphase_get=extra('file_decode_phase_get.dll','sa_file_decode_phase_get')\ncomplete=extra('sound_stream_decode_complete.dll','sa_sound_stream_decode_complete')\nresult_get=extra('file_decode_result_get.dll','sa_file_decode_result_get')")
text=text.replace('stream_before=bytes(ram[0xa70000:0xa70100])', "struct.pack_into('<I',ram,0xa70004,1);struct.pack_into('<I',ram,0xa70040,512);struct.pack_into('<I',ram,0xa7005c,18);struct.pack_into('<H',ram,0xa700a8,0);struct.pack_into('<4I',ram,0xa70088,0x8c010100,0x12345678,0x8c010200,0x87654321)\n    stream_before=bytes(ram[0xa70000:0xa70100]);actual=[];expected=[]")
text=text.replace('    @Call\n    def call', "    def service(address,mem,r,trace):\n        args=tuple(r[i] for i in range(4,8));trace.append((address,args))\n        if address==0x8c010100:\n            assert args==(0x12345678,0x8ca70070,0x8ca70074,0x8ca70078)\n            for offset,value in [(0xa70070,0),(0xa70074,256),(0xa70078,256)]:\n                for byte in range(4):mem[offset+byte]=(value>>(byte*8))&255\n        else:assert address==0x8c010200 and args[:3]==(0x87654321,288,256)\n    def hook(address):\n        def run(mem,r,sr):service(address,mem,r,expected);return sr\n        return run\n    @Call\n    def call")
text=text.replace('        if address in [0x8c10ca5c', "        if address in [0x8c010100,0x8c010200]:service(address,mem,r,actual);return 0\n        target={0x8c667a5c:magic,0x8c644c16:phase_get,0x8c644cb0:decoder,0x8c667844:request,0x8c667978:complete,0x8c644c8c:result_get,0x8c644c7e:ack}.get(address)\n        if target:return target(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)\n        if address in [0x8c10ca5c")
start=text.index('    s.pc=0x8c667844');end=text.index('    assert code==0',start)
text=text[:start]+"""    s.pc=0x8c667ad4
    reference(ram,s,{a:hook(a) for a in [0x8c010100,0x8c010200]},fpu=f,special=special)
    code=fn(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)
"""+text[end:]
text=text.replace("struct.unpack_from('<2I',native,0xa6000c)==(3,decoded)", "struct.unpack_from('<2I',native,0xa6000c)==(0,decoded)")
start=text.index('    before=bytes(native[0xa60000:0xa60040])');end=text.index('    records.append(',start)
text=text[:start]+"""    assert actual==expected and [address for address,args in actual]==[0x8c010100,0x8c010200]
    wanted_stream=bytearray(stream_before);struct.pack_into('<I',wanted_stream,4,3);struct.pack_into('<2I',wanted_stream,0xa0,256,288)
    assert native[0xa70000:0xa70100]==wanted_stream
"""+text[end:]
text=text.replace('sound_scheduled_decode_verification.json','magic_update_retail_pipeline_verification.json').replace('native scheduler with signed division/remainder, stream stereo wrapper, request setup, activation, file dispatcher, stereo decoder and completion acknowledgment sequence comparisons','native format dispatch, magic stream update, scheduler, request setup/activation, file decoder, stereo kernel, completion count update and acknowledgment comparisons').replace('stack/return checks,no modeled calls','stack/return checks; only input/output callbacks modeled, all internal calls native').replace('native scheduled audio decoding comparisons passed','native retail audio update comparisons passed')
(root/'verify_magic_update_retail_pipeline.py').write_text(text)
text=(root/'register_sound_scheduled_decode.py').read_text().replace('sound_scheduled_decode','magic_update_retail_pipeline')
start=text.index('names=');end=text.index(';found=set()',start)
text=text[:start]+"names={'sa_sound_stream_update_dispatch','sa_sound_stream_magic_update','sa_sound_stream_decode_schedule','sa_sound_stream_stereo_request','sa_file_decode_stereo_request','sa_file_decode_request_activate','sa_file_decode_request_process','sa_sound_decode_block_stereo','sa_sound_stream_decode_complete','sa_file_decode_result_get','sa_file_decode_phase_get','sa_file_decode_completion_acknowledge'}"+text[end:]
text=text.replace('Native scheduled audio decoding:','Native retail audio update pipeline:');(root/'register_magic_update_retail_pipeline.py').write_text(text)
