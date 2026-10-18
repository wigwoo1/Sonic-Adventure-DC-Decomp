from pathlib import Path
root=Path(__file__).resolve().parent
text=(root/'verify_file_decode_request_activate.py').read_text().replace('file_decode_request_activate','file_decode_result_get').replace('s.pc=0x8c644c56','s.pc=0x8c644c8c')
text=text.replace("struct.pack_into('<I',ram,0xa2000c,phase)", "struct.pack_into('<2I',ram,0xa2000c,phase,count)")
text=text.replace("if phase==0:struct.pack_into('<2I',wanted,0xc,1,0)", 'assert regs[0]==count')
start=text.index("(out/'analysis/file_decode_result_get_verification.json')")
text=text[:start]+"""(out/'analysis/file_decode_result_get_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='100 full retail RAM/CPU/FPU/SR/PR comparisons of two-instruction result getter644c8c..644c90, including load in RTS delay slot. Five handle phases,five result bit patterns0/1/2/15/ffffffff,varied stack words,both FPU banks. Independent R0 result and all64 handle bytes unchanged; no modeled calls. Full stream completion integration/playback/fullgame unverified.'),indent=2)+'\\n')
print(cases,'decode result getter comparisons passed')
"""
(root/'verify_file_decode_result_get.py').write_text(text)
text=(root/'build_file_decode_request_activate.ps1').read_text().replace('file_decode_request_activate','file_decode_result_get');(root/'build_file_decode_result_get.ps1').write_text(text)
text=(root/'register_file_decode_request_activate.py').read_text().replace('file_decode_request_activate','file_decode_result_get').replace('644c56','644c8c').replace('644c68','644c90').replace('634c56','634c8c').replace('634c68','634c90').replace('Decode request activation:','Decode result getter:');(root/'register_file_decode_result_get.py').write_text(text)
