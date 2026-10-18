from pathlib import Path
root=Path(__file__).resolve().parent
text=(root/'verify_sound_stream_magic_update.py').read_text()
text=text.replace('services=[0x8c644c16,', "getter=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_decode_phase_get.dll')).sa_file_decode_phase_get;getter.argtypes=fn.argtypes\nservices=[")
text=text.replace('def call(ctx,address,mem,fp,srp,r,prp):service(address,mem,r,actual);return 0', "def call(ctx,address,mem,fp,srp,r,prp):\n        if address==0x8c644c16:return getter(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)\n        service(address,mem,r,actual);return 0")
text=text.replace('==addresses and native', '==[a for a in addresses if a!=0x8c644c16] and native')
text=text.replace('sound_stream_magic_update_verification.json','magic_update_native_phase_verification.json').replace('All downstream bodies and two callbacks modeled','Native decoder phase getter644c16; other downstream bodies and two callbacks modeled').replace('Full native integration','Further native integration').replace('magic stream update comparisons passed','magic stream native-phase comparisons passed')
(root/'verify_magic_update_native_phase.py').write_text(text)
text=(root/'register_stream_completion_native_result.py').read_text().replace('stream_completion_native_result','magic_update_native_phase').replace('== 288','== 48').replace("{'sa_sound_stream_decode_complete', 'sa_file_decode_result_get'}", "{'sa_sound_stream_magic_update', 'sa_file_decode_phase_get'}").replace('Stream completion native result integration:','Magic update native phase integration:');(root/'register_magic_update_native_phase.py').write_text(text)
