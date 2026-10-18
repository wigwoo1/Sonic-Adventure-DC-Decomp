from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_magic_lifecycle_native_source.py').read_text()
s=s.replace("build/sound_stream_update_dispatch.dll')).sa_sound_stream_update_dispatch","build/sound_stream_work_tick.dll')).sa_sound_stream_work_tick")
s=s.replace("decoder=extra(","dispatch=extra('sound_stream_update_dispatch.dll','sa_sound_stream_update_dispatch')\nstream_phase=extra('sound_stream_phase_get.dll','sa_sound_stream_phase_get')\ndecoder=extra(",1)
s=s.replace("    stream_before=bytes(ram", "    struct.pack_into('<I',ram,0xa90004,0x8ca70000)\n    stream_before=bytes(ram",1)
a=s.index('    def service(');z=s.index('    def hook(',a)
s=s[:a]+'''    def service(address,mem,r,trace):
        assert address==0x8c65ba22 and r[4]==0x8ca90000 and r[15]==0x8cf00000
        assert sum(int(mem[0xa70004+i])<<(i*8) for i in range(4))==3
        trace.append(address)
''' +s[z:]
s=s.replace("        assert address!=0x8c010200,'Modeled output callback forbidden'","        if address==0x8c65ba22:\n            service(address,mem,r,actual);return 0")
s=s.replace('target={0x8c667a5c:magic','target={0x8c667ad4:dispatch,0x8c667718:stream_phase,0x8c667a5c:magic')
s=s.replace('    s.pc=0x8c667ad4\n    reference(ram,s,{},',"    s.pc=0x8c65bb4e;s.r[4]=0x8ca90000;regs[4]=s.r[4]\n    reference(ram,s,{0x8c65ba22:hook(0x8c65ba22)},")
s=s.replace('assert actual==expected==[]','assert actual==expected==[0x8c65ba22]')
s=s.replace('No modeled functions.','Work completion65ba22 modeled as no-op with independent work pointer,phase3 and restored-stack assertions; all stream decoding calls native. Dispatcher65bb4e executes native stream phase getter and native update dispatch.')
s=s.replace('magic_lifecycle_native_source_verification','tick_native_decode_verification').replace('native-source original-audio lifecycle comparisons passed','dispatcher original-audio decode comparisons passed')
(p/'verify_tick_native_decode.py').write_text(s)
r=(p/'register_magic_lifecycle_native_source.py').read_text().replace('magic_lifecycle_native_source','tick_native_decode').replace("'sa_sound_stream_source_set'}","'sa_sound_stream_source_set','sa_sound_stream_work_tick','sa_sound_stream_phase_get'}").replace('Native source setup original-audio lifecycle:','Native decoding in work dispatcher:')
(p/'register_tick_native_decode.py').write_text(r)
