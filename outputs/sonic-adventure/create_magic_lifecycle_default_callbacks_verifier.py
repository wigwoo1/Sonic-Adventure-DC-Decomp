from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_magic_lifecycle_native_callbacks.py').read_text()
s=s.replace("output_default=extra('sound_stream_default_output.dll','sa_sound_stream_default_output')","output_default=extra('sound_stream_default_output.dll','sa_sound_stream_default_output')\ninput_default=extra('sound_stream_default_input.dll','sa_sound_stream_default_input')")
a=s.index('    ram[0xa90000:');z=s.index('    stream_before=',a)
s=s[:a]+"    struct.pack_into('<I',ram,0xa70018,256);struct.pack_into('<2I',ram,0xa70098,0,0)\n"+s[z:]
s=s.replace("struct.pack_into('<I',ram,0xa70040,128)","struct.pack_into('<I',ram,0xa70040,256)")
s=s.replace('0x8c6673a2:output_default}', '0x8c6673a2:output_default,0x8c667384:input_default}')
s=s.replace('input_set,0x8c65b88c,0x8ca90000','input_set,0x8c667384,0x8ca70000').replace('(0x8c65b88c,0x8ca90000,0x8c6673a2','(0x8c667384,0x8ca70000,0x8c6673a2')
s=s.replace('compacted=samples[128:]+samples[128:]','compacted=samples')
a=s.index('    for index,address in enumerate([0xa80000,0xa80100]):',s.index('    wanted_stream='));z=s.index('    records.append',a)
s=s[:a]+s[z:]
a=s.index("scope='");z=s.index("'),indent=2)",a)
s=s[:a]+"scope='218 retail RAM/CPU/FPU/SR/PR/FPUL/MACL comparisons using first288-byte encoded prefixes from109 original two-channel magic8000 assets,both FPU banks. Native default input667384 and output6673a2,callback registration,start,format dispatch,magic update,scheduler,request,decode,completion,zero-length compaction and acknowledgment. No modeled functions. Independent every sample exact binary32 recurrence and final predictor history,output guards,complete stream state and counters+98/+9c increment256. Controlled stream capacity256,position0,total256,zero initial histories; native constructor/header initialization integration,repeated updates,fullpayload,physical playback/fullgame unverified."+s[z:]
s=s.replace('magic_lifecycle_native_callbacks_verification','magic_lifecycle_default_callbacks_verification').replace('native-callback original-audio lifecycle','default-callback original-audio lifecycle')
(p/'verify_magic_lifecycle_default_callbacks.py').write_text(s)
r=(p/'register_magic_lifecycle_native_callbacks.py').read_text().replace('magic_lifecycle_native_callbacks','magic_lifecycle_default_callbacks')
a=r.index('names=');z=r.index(';found=set()',a)
r=r[:a]+"names={'sa_sound_stream_default_input','sa_sound_stream_default_output','sa_sound_stream_magic_update','sa_sound_stream_decode_complete','sa_sound_stream_update_dispatch'}"+r[z:]
r=r.replace('Native callbacks original-audio stream lifecycle:','Default callbacks original-audio stream lifecycle:')
(p/'register_magic_lifecycle_default_callbacks.py').write_text(r)
