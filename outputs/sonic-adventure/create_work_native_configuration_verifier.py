from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_sound_work_create.py').read_text()
s=s.replace("pool=0x8a3910;cases=0", "helpers={0x8c65b606:reset}\nfor address,stem,name in [(0x8c65e798,'sound_source_buffer_get','sa_sound_source_buffer_get'),(0x8c66768c,'sound_stream_callback_setters','sa_sound_stream_input_callback_set')]:\n helpers[address]=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{stem}.dll')),name);helpers[address].argtypes=fn.argtypes\npool=0x8a3910;cases=0")
s=s.replace('[0,0x8ca70000]','[0x8ca70000]')
s=s.replace(' before=bytes(ram[pool:'," struct.pack_into('<I',ram,0xa3001c,0x8ca40000)\n ram[0xa70000:0xa70100]=bytes([0x55])*256;stream_before=bytes(ram[0xa70000:0xa70100])\n before=bytes(ram[pool:")
s=s.replace('if address==0x8c65b606:return reset(', 'if address in helpers:return helpers[address](')
s=s.replace('{a:hook(a) for a in services}', '{a:hook(a) for a in services if a not in helpers}')
s=s.replace('assert len(actual)==(5 if slot<8 else 0)',"assert len(actual)==(3 if slot<8 else 0)\n wanted_stream=bytearray(stream_before)\n if slot<8:struct.pack_into('<2I',wanted_stream,0x8c,0x8c65b88c,selected)\n assert native[0xa70000:0xa70100]==wanted_stream")
s=s.replace('sound_work_create_verification','work_native_configuration_verification').replace('108 original','54 original').replace('creation failure/success','creation success').replace('Buffer/size/mirror getters,stream creation and callback setter modeled. Null creation still invokes callback setter with null;', 'Buffer getter65e798 and input callback setter66768c native,independent complete stream callback fields8c/90. Size/mirror getters and stream creation modeled; null-creation callback path excluded from native setter integration;')
s=s.replace('sound_stream_callback_setters','sound_stream_input_callback_set')
s=s.replace("wanted_stream,0x8c,", "wanted_stream,0x88,").replace('fields8c/90','fields88/8c')
(p/'verify_work_native_configuration.py').write_text(s)
