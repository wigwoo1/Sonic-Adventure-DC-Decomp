from pathlib import Path
root=Path(__file__).resolve().parent
text=(root/'verify_sound_stream_decode_reset.py').read_text().replace('sound_stream_decode_reset','sound_stream_mono_request').replace('s.pc=0x8c66777c','s.pc=0x8c6677e2')
text=text.replace("reset=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_decode_handle_reset.dll')).sa_file_decode_handle_reset;reset.argtypes=fn.argtypes", "setup=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_decode_mono_request.dll')).sa_file_decode_mono_request;setup.argtypes=fn.argtypes\nactivate=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_decode_request_activate.dll')).sa_file_decode_request_activate;activate.argtypes=fn.argtypes")
text=text.replace('[0x8ca40000,0]', '[0,7]')
text=text.replace(";stream_before=bytes(ram[0xa60000:0xa60100])", ";struct.pack_into('<I',ram,0xa60050,0x8ca10000);struct.pack_into('<I',ram,0xa60064,0x8ca30000);struct.pack_into('<I',ram,0xa60070,right);stream_before=bytes(ram[0xa60000:0xa60100])")
text=text.replace('s.r[4:8]=[0x8ca60000,0x8ca10000,count,0x8ca30000]', 's.r[4:8]=[0x8ca60000,count,0x22340006,0x22340007]')
start=text.index('        if address==0x8c644c68:');end=text.index("    reference(ram,s",start)
text=text[:start]+"        target={0x8c644c1a:setup,0x8c644c56:activate}[address]\n        return target(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)\n"+text[end:]
text=text.replace("struct.pack_into('<I',wanted,0xc,0);wanted[0x28:0x30]=bytes(8)", "if phase==0:\n        struct.pack_into('<2I',wanted,0xc,1,0);struct.pack_into('<5I',wanted,0x14,1,0x8ca10000,count,0x8ca30000+right*2,0)")
text=text.replace("stream_expected=bytearray(stream_before);struct.pack_into('<I',stream_expected,4,0)", 'stream_expected=bytearray(stream_before)')
start=text.index("(out/'analysis/sound_stream_mono_request_verification.json')")
text=text[:start]+"""(out/'analysis/sound_stream_mono_request_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='100 full retail RAM/CPU/FPU/SR/PR native stream mono request6677e2..66780e comparisons connected to native setup644c1a and activation644c56; no modeled calls. Five decoder phases, five unsigned counts including0xffffffff, output offsets0/7 and both FPU banks. Independent handle oracle verifies idle request fields, output base+2*sample offset, phase1/result0, busy handle preservation; all256 stream bytes preserved. No audio decoding in this test; full stream scheduling/playback/fullgame unverified.'),indent=2)+'\\n')
print(cases,'stream mono request comparisons passed')
"""
(root/'verify_sound_stream_mono_request.py').write_text(text)
text=(root/'build_sound_stream_decode_reset.ps1').read_text().replace('sound_stream_decode_reset','sound_stream_mono_request');(root/'build_sound_stream_mono_request.ps1').write_text(text)
text=(root/'register_sound_stream_decode_reset.py').read_text().replace('sound_stream_decode_reset','sound_stream_mono_request').replace('66777c','6677e2').replace('667796','66780e').replace('65777c','6577e2').replace('657796','65780e').replace('Stream decode reset:','Stream mono request:');(root/'register_sound_stream_mono_request.py').write_text(text)
