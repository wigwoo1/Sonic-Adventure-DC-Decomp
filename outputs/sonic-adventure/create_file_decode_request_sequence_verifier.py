from pathlib import Path
root=Path(__file__).resolve().parent
text=(root/'verify_file_decode_retail_pipeline.py').read_text()
text=text.replace('kernel.argtypes=fn.argtypes', "kernel.argtypes=fn.argtypes\nsetup=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_decode_stereo_request.dll')).sa_file_decode_stereo_request\nactivate=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_decode_request_activate.dll')).sa_file_decode_request_activate\nsetup.argtypes=fn.argtypes;activate.argtypes=fn.argtypes")
text=text.replace("struct.pack_into('<6I',ram,0xa6000c,1,0x55555555,2,0x8ca10000,count,0x8ca20008)", "struct.pack_into('<I',ram,0xa6000c,0)")
text=text.replace("struct.pack_into('<I4h2h',ram,0xa60024,0x8ca40008,0,0,0,0,*coefficients)", "struct.pack_into('<4h2h',ram,0xa60028,0,0,0,0,*coefficients)")
text=text.replace("s.r[4]=0x8ca60000;s.r[15]", "s.r[4:8]=[0x8ca60000,0x8ca10000,count,0x8ca20008];s.r[15]")
needle='    reference(ram,s,fpu=f,special=special)'
replacement="""    for entry,function in [(0x8c644c38,setup),(0x8c644c56,activate)]:
        s.pc=entry
        reference(ram,s,fpu=f,special=special)
        assert function(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
        nf.fpul=fpul.value
        assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
        if entry==0x8c644c38:
            assert regs[0]==1 and struct.unpack_from('<5I',native,0xa60014)==(2,0x8ca10000,count,0x8ca20008,0x8ca40008)
        else:assert struct.unpack_from('<2I',native,0xa6000c)==(1,0)
    s.pc=0x8c644cb0
    reference(ram,s,fpu=f,special=special)"""
assert needle in text;text=text.replace(needle,replacement)
text=text.replace('file_decode_retail_pipeline_verification.json','file_decode_request_sequence_verification.json')
text=text.replace('native file dispatcher plus stereo decoder comparisons', 'native stereo request setup, activation, file dispatcher and stereo decoder sequence comparisons')
text=text.replace('dispatcher phase1->3/result storage', 'persistent RAM/register sequence idle setup->activation phase1->decode phase3/result storage')
text=text.replace('original file-dispatch stereo pipeline comparisons passed','native audio request sequence comparisons passed')
(root/'verify_file_decode_request_sequence.py').write_text(text)
text=(root/'register_file_decode_retail_pipeline.py').read_text().replace('file_decode_retail_pipeline','file_decode_request_sequence').replace("'sa_sound_decode_block_stereo'}", "'sa_sound_decode_block_stereo','sa_file_decode_stereo_request','sa_file_decode_request_activate'}").replace('Original file-dispatch stereo pipeline:','Native audio request sequence:')
(root/'register_file_decode_request_sequence.py').write_text(text)
