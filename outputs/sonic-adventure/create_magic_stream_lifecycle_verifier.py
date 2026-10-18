from pathlib import Path
root=Path(__file__).resolve().parent
text=(root/'verify_magic_update_retail_compaction.py').read_text()
text=text.replace("sample_copy=extra('sound_stream_sample_copy.dll','sa_sound_stream_sample_copy')", "sample_copy=extra('sound_stream_sample_copy.dll','sa_sound_stream_sample_copy')\ninput_set=extra('sound_stream_input_callback_set.dll','sa_sound_stream_input_callback_set')\noutput_set=extra('sound_stream_output_callback_set.dll','sa_sound_stream_output_callback_set')\nstart_stream=extra('sound_stream_start.dll','sa_sound_stream_start')")
text=text.replace("struct.pack_into('<I',ram,0xa70004,1)", "struct.pack_into('<I',ram,0xa70004,0)")
text=text.replace(";struct.pack_into('<4I',ram,0xa70088,0x8c010100,0x12345678,0x8c010200,0x87654321)", '')
needle='    s.pc=0x8c667ad4'
prefix="""    for entry,target,arg1,arg2 in [(0x8c66768c,input_set,0x8c010100,0x12345678),(0x8c667696,output_set,0x8c010200,0x87654321),(0x8c66776e,start_stream,0,0)]:
        s.r[4:7]=[0x8ca70000,arg1,arg2]
        for index in range(4,7):regs[index]=s.r[index]
        s.pc=entry
        reference(ram,s,fpu=f,special=special)
        assert target(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
        nf.fpul=fpul.value
        assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
    assert struct.unpack_from('<I',native,0xa70004)[0]==1
    assert struct.unpack_from('<4I',native,0xa70088)==(0x8c010100,0x12345678,0x8c010200,0x87654321)
    stream_before=bytes(native[0xa70000:0xa70100])
"""
assert needle in text;text=text.replace(needle,prefix+needle)
text=text.replace('magic_update_retail_compaction_verification.json','magic_stream_lifecycle_verification.json').replace('native format dispatch, magic stream update','native input/output callback registration, stream start, format dispatch, magic stream update').replace('native retail audio compaction update comparisons passed','native original-audio stream lifecycle comparisons passed')
(root/'verify_magic_stream_lifecycle.py').write_text(text)
text=(root/'register_magic_update_retail_compaction.py').read_text().replace('magic_update_retail_compaction','magic_stream_lifecycle').replace("'sa_sound_stream_sample_copy'}", "'sa_sound_stream_sample_copy','sa_sound_stream_input_callback_set','sa_sound_stream_output_callback_set','sa_sound_stream_start'}").replace('Native retail audio compaction update:','Native original-audio stream lifecycle:');(root/'register_magic_stream_lifecycle.py').write_text(text)
