from pathlib import Path
root=Path(__file__).resolve().parent
text=(root/'verify_file_decode_request_sequence.py').read_text()
text=text.replace('setup.argtypes=fn.argtypes;activate.argtypes=fn.argtypes', "setup.argtypes=fn.argtypes;activate.argtypes=fn.argtypes\nack=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_decode_completion_acknowledge.dll')).sa_file_decode_completion_acknowledge\nack.argtypes=fn.argtypes")
needle='    records.append('
prefix="""    before=bytes(native[0xa60000:0xa60040]);expected_handle=bytearray(before);struct.pack_into('<I',expected_handle,12,0)
    s.r[4]=0x8ca60000;regs[4]=0x8ca60000
    s.pc=0x8c644c7e
    reference(ram,s,fpu=f,special=special)
    assert ack(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    nf.fpul=fpul.value
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f)
    assert native[0xa60000:0xa60040]==expected_handle
"""
assert needle in text;text=text.replace(needle,prefix+needle)
text=text.replace('file_decode_request_sequence_verification.json','file_decode_ack_sequence_verification.json').replace('and stereo decoder sequence comparisons','stereo decoder and completion acknowledgment sequence comparisons').replace('decode phase3/result storage','decode phase3/result storage->acknowledged idle phase0 with result/history preserved').replace('native audio request sequence comparisons passed','native audio completion sequence comparisons passed')
(root/'verify_file_decode_ack_sequence.py').write_text(text)
text=(root/'register_file_decode_request_sequence.py').read_text().replace('file_decode_request_sequence','file_decode_ack_sequence').replace("'sa_file_decode_request_activate'}", "'sa_file_decode_request_activate','sa_file_decode_completion_acknowledge'}").replace('Native audio request sequence:','Native audio completion sequence:')
(root/'register_file_decode_ack_sequence.py').write_text(text)
