from pathlib import Path
root=Path(__file__).resolve().parent
text=(root/'verify_file_decode_ack_sequence.py').read_text()
text=text.replace('setup.argtypes=fn.argtypes;activate.argtypes=fn.argtypes', "setup.argtypes=fn.argtypes;activate.argtypes=fn.argtypes\nrequest=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_stream_stereo_request.dll')).sa_sound_stream_stereo_request;request.argtypes=fn.argtypes")
text=text.replace('native=bytearray(ram);', "ram[0xa70000:0xa70100]=bytes([0x66])*256;struct.pack_into('<I',ram,0xa70008,0x8ca60000);struct.pack_into('<I',ram,0xa70050,0x8ca10000);struct.pack_into('<I',ram,0xa70064,0x8ca20008);struct.pack_into('<2I',ram,0xa7006c,65536,0);stream_before=bytes(ram[0xa70000:0xa70100])\n    native=bytearray(ram);")
text=text.replace('s.r[4:8]=[0x8ca60000,0x8ca10000,count,0x8ca20008]', 's.r[4:8]=[0x8ca70000,count//2,0x22340006,0x22340007]')
text=text.replace('assert address==0x8c65b238,address', "if address in [0x8c644c38,0x8c644c56]:\n            target={0x8c644c38:setup,0x8c644c56:activate}[address]\n            return target(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)\n        assert address==0x8c65b238,address")
start=text.index('    for entry,function in ');end=text.index('    s.pc=0x8c644cb0',start)
text=text[:start]+"""    s.pc=0x8c66780e
    reference(ram,s,fpu=f,special=special)
    assert request(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    nf.fpul=fpul.value
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
    assert struct.unpack_from('<5I',native,0xa60014)==(2,0x8ca10000,count,0x8ca20008,0x8ca40008)
    assert struct.unpack_from('<2I',native,0xa6000c)==(1,0)
    assert native[0xa70000:0xa70100]==stream_before
    s.r[4]=0x8ca60000;regs[4]=0x8ca60000
"""+text[end:]
text=text.replace('file_decode_ack_sequence_verification.json','sound_stream_decode_pipeline_verification.json').replace('native stereo request setup, activation, file dispatcher stereo decoder and completion acknowledgment sequence comparisons', 'native stream stereo wrapper, request setup, activation, file dispatcher, stereo decoder and completion acknowledgment sequence comparisons').replace('persistent RAM/register sequence idle setup', 'persistent RAM/register sequence stream buffer calculation->idle setup').replace('native audio completion sequence comparisons passed','native stream decode pipeline comparisons passed')
(root/'verify_sound_stream_decode_pipeline.py').write_text(text)
text=(root/'register_file_decode_ack_sequence.py').read_text().replace('file_decode_ack_sequence','sound_stream_decode_pipeline').replace("'sa_file_decode_completion_acknowledge'}", "'sa_file_decode_completion_acknowledge','sa_sound_stream_stereo_request'}").replace('Native audio completion sequence:','Native stream decode pipeline:');(root/'register_sound_stream_decode_pipeline.py').write_text(text)
