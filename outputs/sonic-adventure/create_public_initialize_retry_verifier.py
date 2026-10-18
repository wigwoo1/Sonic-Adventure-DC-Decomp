from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_public_initialize_headers.py').read_text()
needle=" special={'macl':0};reference(ram,s,fpu=f,special=special,vector_instruction=sqrt_service)"
assert needle in s
s=s.replace(needle,''' special={'macl':0}
 # Simulated producer availability: incomplete header, then enough bytes on the same objects.
 struct.pack_into('<I',ram,0xa8000c,15);struct.pack_into('<I',native,0xa8000c,15)
 short_object=bytes(native[0xa80000:0xa80030]);short_stream=bytes(native[state:state+256]);short_handle=bytes(native[0xa30000:0xa30040])
 reference(ram,s,fpu=f,special=special,vector_instruction=sqrt_service)
 assert lower[0x8c65bb88](view,ctypes.byref(nf),ctypes.byref(sr),r,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
 nf.fpul=fpul.value
 assert native==ram and list(r)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(f)==bytes(nf) and macl.value==special['macl']
 assert native[0xa60000:0xa60100]==work_before and native[0xa80000:0xa80030]==short_object
 assert native[state:state+256]==short_stream and native[0xa30000:0xa30040]==short_handle and not actual
 assert r[15]==0x8cf00000
 struct.pack_into('<I',ram,0xa8000c,4096);struct.pack_into('<I',native,0xa8000c,4096)
 s.r[4]=0x8ca60000;r[4]=s.r[4];s.pc=0x8c65bb88
 reference(ram,s,fpu=f,special=special,vector_instruction=sqrt_service)''')
s=s.replace('cases_passed=cases,original_files=','cases_passed=cases,public_updates_compared=cases*2,original_files=')
s=s.replace('218 full retail RAM/CPU/FPU/SR/PR/FPUL/MACL comparisons','436 full retail RAM/CPU/FPU/SR/PR/FPUL/MACL update comparisons in218 persistent sequences')
s=s.replace('No modeled calls.','No modeled calls. First public update has15-byte availability: independent entire work/stream/handle/source object preserved, no configuration call, stack restored. Simulated producer then exposes4096 bytes on the same objects; second update initializes header without resetting CPU/FPU or object state. Producer availability change is fixture-controlled, not production streaming.')
s=s.replace('public_initialize_headers_verification','public_initialize_retry_verification').replace('public original header initialization comparisons passed','persistent public initialization retry sequences passed')
(p/'verify_public_initialize_retry.py').write_text(s)
r=(p/'register_public_initialize_headers.py').read_text().replace('public_initialize_headers','public_initialize_retry').replace('Original headers through public work initialization:','Persistent original header initialization retry:')
(p/'register_public_initialize_retry.py').write_text(r)
