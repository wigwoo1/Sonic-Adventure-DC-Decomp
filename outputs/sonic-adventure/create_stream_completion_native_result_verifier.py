from pathlib import Path
root=Path(__file__).resolve().parent
text=(root/'verify_sound_stream_decode_complete.py').read_text()
text=text.replace('cases=0', "getter=load('file_decode_result_get.dll','sa_file_decode_result_get')\ncases=0")
text=text.replace('    before=bytes(ram[0xa70000:0xa70100]);', "    ram[0xa60000:0xa60040]=bytes([0x55])*64;struct.pack_into('<I',ram,0xa60010,decoded);handle_before=bytes(ram[0xa60000:0xa60040])\n    before=bytes(ram[0xa70000:0xa70100]);")
text=text.replace('        if address in [0x8c644c8c,0x8c66792c,0x8c667960]:', "        if address==0x8c644c8c:return getter(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)\n        if address in [0x8c66792c,0x8c667960]:")
text=text.replace('for a in [0x8c644c8c,0x8c66792c,0x8c667960]},fpu=f', 'for a in [0x8c66792c,0x8c667960]},fpu=f')
text=text.replace('    cases+=1', '    assert native[0xa60000:0xa60040]==handle_before\n    cases+=1')
text=text.replace('sound_stream_decode_complete_verification.json','stream_completion_native_result_verification.json').replace('result getter and downstream compaction modeled','native result getter644c8c; only downstream compaction modeled').replace('Native getter/compaction integration','Native compaction integration').replace('stream completion comparisons passed','stream completion native-result comparisons passed')
(root/'verify_stream_completion_native_result.py').write_text(text)
text=(root/'register_file_decode_native_flag.py').read_text().replace('file_decode_native_flag','stream_completion_native_result').replace('== 240','== 288').replace("{'sa_file_decode_request_process', 'sa_file_decode_odd_flag_set'}", "{'sa_sound_stream_decode_complete', 'sa_file_decode_result_get'}").replace('Native file decoder odd-result flag integration:','Stream completion native result integration:');(root/'register_stream_completion_native_result.py').write_text(text)
