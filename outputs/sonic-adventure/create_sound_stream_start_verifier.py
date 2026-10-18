from pathlib import Path
root=Path(__file__).resolve().parent
text=(root/'verify_file_decode_phase_get.py').read_text().replace('file_decode_phase_get','sound_stream_start').replace('s.pc=0x8c644c16','s.pc=0x8c66776e')
text=text.replace("struct.pack_into('<2I',ram,0xa2000c,phase,count)", "struct.pack_into('<I',ram,0xa20004,phase);struct.pack_into('<I',ram,0xa20010,count)")
text=text.replace('    assert regs[0]==phase', "    if phase==0:struct.pack_into('<I',wanted,4,1)")
start=text.index("(out/'analysis/sound_stream_start_verification.json')")
text=text[:start]+"""(out/'analysis/sound_stream_start_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='100 full retail RAM/CPU/FPU/SR/PR comparisons of seven-instruction stream start66776e..66777c. Five initial phases0/1/2/3/ffffffff,varied registers/stack,both FPU banks. Independent all64-byte state oracle: only idle phase0 changes to1, every other byte and nonidle state preserved. No modeled calls. Continuous stream restart/input/output/playback/fullgame unverified.'),indent=2)+'\\n')
print(cases,'stream start comparisons passed')
"""
(root/'verify_sound_stream_start.py').write_text(text)
text=(root/'build_file_decode_phase_get.ps1').read_text().replace('file_decode_phase_get','sound_stream_start');(root/'build_sound_stream_start.ps1').write_text(text)
text=(root/'register_file_decode_phase_get.py').read_text().replace('file_decode_phase_get','sound_stream_start').replace('644c16','66776e').replace('644c1a','66777c').replace('634c16','65776e').replace('634c1a','65777c').replace('Decode phase getter:','Stream start:');(root/'register_sound_stream_start.py').write_text(text)
