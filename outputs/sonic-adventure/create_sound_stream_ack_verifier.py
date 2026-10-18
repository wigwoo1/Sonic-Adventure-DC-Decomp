from pathlib import Path
root=Path(__file__).resolve().parent
text=(root/'verify_sound_stream_decode_reset.py').read_text().replace('sound_stream_decode_reset','sound_stream_decode_acknowledge').replace('file_decode_handle_reset','file_decode_completion_acknowledge').replace('s.pc=0x8c66777c','s.pc=0x8c667796').replace('address==0x8c644c68','address==0x8c644c7e')
text=text.replace('[0,1,2,15,0xffffffff]','[0,1,2,3,0xffffffff]')
text=text.replace("struct.pack_into('<I',wanted,0xc,0);wanted[0x28:0x30]=bytes(8)", "if count==3 and phase==3:struct.pack_into('<I',wanted,0xc,0)")
text=text.replace("stream_expected=bytearray(stream_before);struct.pack_into('<I',stream_expected,4,0)", "stream_expected=bytearray(stream_before)\n    if count==3:\n        struct.pack_into('<I',stream_expected,4,0);struct.pack_into('<I',stream_expected,0x9c,0)")
start=text.index("(out/'analysis/sound_stream_decode_acknowledge_verification.json')")
text=text[:start]+"""(out/'analysis/sound_stream_decode_acknowledge_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='100 full retail RAM/CPU/FPU/SR/PR comparisons of16-instruction stream acknowledgment667796..6677b6 connected to native handle acknowledgment644c7e; no modeled calls. Five stream phases crossed with five decoder phases and both FPU banks. Independent256-byte stream/64-byte handle oracle: stream phase3 clears +4 and +9c and acknowledges handle; handle phase3 alone becomes0, other phases and all history/result/request bytes preserved. Full stream scheduling/playback/fullgame unverified.'),indent=2)+'\\n')
print(cases,'stream decode acknowledgment comparisons passed')
"""
(root/'verify_sound_stream_decode_acknowledge.py').write_text(text)
text=(root/'build_sound_stream_decode_reset.ps1').read_text().replace('sound_stream_decode_reset','sound_stream_decode_acknowledge')
(root/'build_sound_stream_decode_acknowledge.ps1').write_text(text)
text=(root/'register_sound_stream_decode_reset.py').read_text().replace('sound_stream_decode_reset','sound_stream_decode_acknowledge').replace('667796','6677b6').replace('66777c','667796').replace('657796','6577b6').replace('65777c','657796').replace('Stream decode reset:','Stream decode acknowledgment:')
(root/'register_sound_stream_decode_acknowledge.py').write_text(text)
