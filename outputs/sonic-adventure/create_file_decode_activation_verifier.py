from pathlib import Path
root=Path(__file__).resolve().parent
text=(root/'verify_file_decode_stereo_request.py').read_text().replace('file_decode_stereo_request','file_decode_request_activate').replace('s.pc=0x8c644c38','s.pc=0x8c644c56')
text=text.replace("if phase==0:struct.pack_into('<5I',wanted,0x14,2,0x8ca10000,count,0x8ca30000,right)", "if phase==0:struct.pack_into('<2I',wanted,0xc,1,0)")
text=text.replace(' and regs[0]==int(phase==0)', '')
text=text.replace('100 full retail RAM/CPU/FPU/SR/PR comparisons of15-instruction stereo request setup644c38..644c56. Idle and four busy states, five unsigned counts, both banks and null/non-null right destinations. Independent file-handle guards, selector/source/count/output fields, success return and unchanged phase; no modeled calls. Scheduling/activation and fullgame unverified.', '100 full retail RAM/CPU/FPU/SR/PR comparisons of nine-instruction request activation644c56..644c68. Idle and four busy states, varied count/register/stack inputs and both FPU banks. Independent full64-byte handle oracle: idle becomes phase1 with result0, busy states preserve every byte. No modeled calls. Connected scheduling and fullgame unverified.')
text=text.replace('stereo request setup comparisons passed','decode request activation comparisons passed')
(root/'verify_file_decode_request_activate.py').write_text(text)
text=(root/'build_file_decode_stereo_request.ps1').read_text().replace('file_decode_stereo_request','file_decode_request_activate')
(root/'build_file_decode_request_activate.ps1').write_text(text)
text=(root/'register_file_decode_stereo_request.py').read_text().replace('file_decode_stereo_request','file_decode_request_activate').replace('644c38','644c56').replace('644c56\u0027,source','644c68\u0027,source').replace('634c38:0x634c56','634c56:0x634c68').replace('Stereo decode request setup:','Decode request activation:')
(root/'register_file_decode_request_activate.py').write_text(text)
