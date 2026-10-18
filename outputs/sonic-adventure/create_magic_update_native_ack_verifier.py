from pathlib import Path
root=Path(__file__).resolve().parent
text=(root/'verify_magic_update_native_phase.py').read_text()
text=text.replace('services=[', "ack=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_decode_completion_acknowledge.dll')).sa_file_decode_completion_acknowledge;ack.argtypes=fn.argtypes\nservices=[",1)
text=text.replace('0x8c667978,0x8c644c7e,0x8c010100','0x8c667978,0x8c010100')
text=text.replace('        service(address,mem,r,actual);return 0', "        if address==0x8c644c7e:return ack(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)\n        service(address,mem,r,actual);return 0")
text=text.replace('if a!=0x8c644c16]', 'if a not in [0x8c644c16,0x8c644c7e]]')
text=text.replace('magic_update_native_phase_verification.json','magic_update_native_ack_verification.json').replace('Native decoder phase getter644c16; other downstream bodies','Native decoder phase getter644c16 and completion acknowledgment644c7e; other downstream bodies').replace('magic stream native-phase comparisons passed','magic stream native-acknowledgment comparisons passed')
(root/'verify_magic_update_native_ack.py').write_text(text)
text=(root/'register_magic_update_native_phase.py').read_text().replace('magic_update_native_phase','magic_update_native_ack').replace("'sa_file_decode_phase_get'}", "'sa_file_decode_phase_get', 'sa_file_decode_completion_acknowledge'}").replace('Magic update native phase integration:','Magic update native acknowledgment integration:');(root/'register_magic_update_native_ack.py').write_text(text)
