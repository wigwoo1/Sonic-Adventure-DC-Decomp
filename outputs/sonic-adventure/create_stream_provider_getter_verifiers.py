from pathlib import Path
root=Path(__file__).resolve().parent
for name,entry,field,values,expected in [
    ('sound_stream_channel_count_get','8c6676ae','0xe','[0,1,2,127,128,255,0xffffffff]','ctypes.c_int8(count&255).value&0xffffffff'),
    ('sound_stream_position_get','8c6676a0','0x3c','[0,1,2,127,128,255,0xffffffff]','count')]:
    text=(root/'verify_file_decode_result_get.py').read_text().replace('file_decode_result_get',name).replace('s.pc=0x8c644c8c','s.pc=0x'+entry).replace('[0,1,2,15,0xffffffff]',values)
    text=text.replace("struct.pack_into('<2I',ram,0xa2000c,phase,count)", "struct.pack_into('<I',ram,0xa20004,phase);"+("ram[0xa2000e]=count&255" if field=='0xe' else "struct.pack_into('<I',ram,0xa2003c,count)"))
    text=text.replace('regs[0]==count','regs[0]=='+expected)
    start=text.index("(out/'analysis/"+name+'_verification.json'+"')")
    scope=f'140 full retail RAM/CPU/FPU/SR/PR comparisons of two-instruction getter{entry}, including load in RTS delay slot. Seven bit patterns with signed-byte boundary128/255,five phase fixtures,both banks,varied stack. Independent R0 value and all64 state bytes unchanged; no modeled calls. Native provider integration/full playback/fullgame unverified.'
    text=text[:start]+f"(out/'analysis/{name}_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope={scope!r}),indent=2)+'\\n')\nprint(cases,'{name} comparisons passed')\n"
    (root/f'verify_{name}.py').write_text(text)
    text=(root/'build_file_decode_result_get.ps1').read_text().replace('file_decode_result_get',name);(root/f'build_{name}.ps1').write_text(text)
    end=int(entry,16)+4
    text=(root/'register_file_decode_result_get.py').read_text().replace('file_decode_result_get',name).replace('==100','==140').replace('0x8c644c8c','0x'+entry).replace('0x8c644c90',f'0x{end:08x}').replace('0x634c8c',hex(int(entry,16)-0x8c010000)).replace('0x634c90',hex(end-0x8c010000)).replace('Decode result getter:',name+':')
    (root/f'register_{name}.py').write_text(text)
