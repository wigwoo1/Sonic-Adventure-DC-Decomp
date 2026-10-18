from pathlib import Path
root=Path(__file__).resolve().parent
text=(root/'verify_sound_stream_start.py').read_text().replace('sound_stream_start','sound_stream_input_callback_set').replace('s.pc=0x8c66776e','s.pc=0x8c66768c')
text=text.replace('0xa20040','0xa20100').replace('bytes([0x55])*64','bytes([0x55])*256')
text=text.replace("if phase==0:struct.pack_into('<I',wanted,4,1)", "struct.pack_into('<2I',wanted,0x88,0x8ca10000,count)")
start=text.index("(out/'analysis/sound_stream_input_callback_set_verification.json')")
text=text[:start]+"""(out/'analysis/sound_stream_input_callback_set_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='100 full retail RAM/CPU/FPU/SR/PR comparisons of five-instruction stream input callback setter66768c..667696. Independent256-byte stream oracle verifies callback pointer+88 and user data+8c, including RTS delay-slot store; five phases,five user-data bit patterns,varied stack,both FPU banks. No calls. This is the input callback consumed by magic update, not the output callback at+90. Full callback implementation and continuous playback/fullgame unverified.'),indent=2)+'\\n')
print(cases,'stream input callback setter comparisons passed')
"""
(root/'verify_sound_stream_input_callback_set.py').write_text(text)
text=(root/'build_sound_stream_start.ps1').read_text().replace('sound_stream_start','sound_stream_input_callback_set');(root/'build_sound_stream_input_callback_set.ps1').write_text(text)
text=(root/'register_sound_stream_start.py').read_text().replace('sound_stream_start','sound_stream_input_callback_set').replace('66776e','66768c').replace('66777c','667696').replace('65776e','65768c').replace('65777c','657696').replace('Stream start:','Stream input callback setter:');(root/'register_sound_stream_input_callback_set.py').write_text(text)
