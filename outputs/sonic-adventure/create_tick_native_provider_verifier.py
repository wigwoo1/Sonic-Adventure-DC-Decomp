from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_tick_idle_lifecycle.py').read_text()
s=s.replace("    struct.pack_into('<I',ram,0xa9003c,0xffffffff)","    struct.pack_into('<3I',ram,0xa90038,256,256,0)")
s=s.replace('0 if index==0 else 600,0,base,1024','0,0,base,1024')
s=s.replace('(0x8c66768c,input_set,0x8c667384,0x8ca70000)','(0x8c66768c,input_set,0x8c65b88c,0x8ca90000)')
s=s.replace('==(0x8c667384,0x8ca70000,0x8c6673a2,0x8ca70000)','==(0x8c65b88c,0x8ca90000,0x8c6673a2,0x8ca70000)')
s=s.replace('[(0xc,512),(0x10,688),(0x14,512)]','[(0xc,512),(0x10,88),(0x14,512)]')
s=s.replace('controlled sample reservations and initial available600 yield688 (production input provider reservation not exercised)','native input provider reserves600 available bytes per channel before decode;512 committed and88 returned with available88 and read position512')
s=s.replace('default input667384 and output6673a2','native input provider65b88c with work max256/remaining256 and default output6673a2')
s=s.replace('tick_idle_lifecycle_verification','tick_native_provider_verification').replace('idle dispatcher native lifecycle comparisons passed','native provider dispatcher lifecycle comparisons passed')
(p/'verify_tick_native_provider.py').write_text(s)
r=(p/'register_tick_idle_lifecycle.py').read_text().replace('tick_idle_lifecycle','tick_native_provider').replace("'sa_sound_work_total_samples_get'}","'sa_sound_work_total_samples_get','sa_sound_stream_input_provider','sa_sound_source_query'}").replace('Native idle feeding/decoding/completion dispatcher:','Native provider feeding/decoding/completion dispatcher:')
(p/'register_tick_native_provider.py').write_text(r)
