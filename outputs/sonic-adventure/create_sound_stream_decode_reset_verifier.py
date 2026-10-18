from pathlib import Path
root=Path(__file__).resolve().parent
text=(root/'verify_file_decode_handle_reset.py').read_text().replace('file_decode_handle_reset.dll','sound_stream_decode_reset.dll').replace('.sa_file_decode_handle_reset','.sa_sound_stream_decode_reset').replace('s.pc=0x8c644c68','s.pc=0x8c66777c')
text=text.replace('cases=0', "reset=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_decode_handle_reset.dll')).sa_file_decode_handle_reset;reset.argtypes=fn.argtypes\ncases=0")
text=text.replace('native=bytearray(ram);', "ram[0xa60000:0xa60100]=bytes([0x66])*256;struct.pack_into('<2I',ram,0xa60004,count,0x8ca20000);stream_before=bytes(ram[0xa60000:0xa60100])\n    native=bytearray(ram);")
text=text.replace('s.r[4:8]=[0x8ca20000,0x8ca10000,count,0x8ca30000]', 's.r[4:8]=[0x8ca60000,0x8ca10000,count,0x8ca30000]')
text=text.replace('assert address==0x8c10d31c', "if address==0x8c644c68:return reset(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)\n        assert address==0x8c10d31c")
text=text.replace('    cases+=1', "    stream_expected=bytearray(stream_before);struct.pack_into('<I',stream_expected,4,0)\n    assert native[0xa60000:0xa60100]==stream_expected\n    cases+=1")
text=text.replace('file_decode_handle_reset_verification.json','sound_stream_decode_reset_verification.json').replace('11-instruction handle reset644c68..644c7e connected to native byte fill10d31c','13-instruction stream reset66777c..667796 connected to native handle reset644c68 and byte fill10d31c').replace('all other handle bytes preserved','all other handle bytes preserved; stream phase+4 independently cleared and all other256 stream bytes preserved').replace('decode handle reset comparisons passed','stream decode reset comparisons passed')
(root/'verify_sound_stream_decode_reset.py').write_text(text)
text=(root/'build_file_decode_handle_reset.ps1').read_text().replace('file_decode_handle_reset','sound_stream_decode_reset')
(root/'build_sound_stream_decode_reset.ps1').write_text(text)
text=(root/'register_file_decode_handle_reset.py').read_text().replace('file_decode_handle_reset','sound_stream_decode_reset').replace('644c68','66777c').replace('644c7e','667796').replace('634c68','65777c').replace('634c7e','657796').replace('Decode handle reset:','Stream decode reset:')
(root/'register_sound_stream_decode_reset.py').write_text(text)
