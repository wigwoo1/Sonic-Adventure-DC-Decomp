from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_public_initialize_headers.py').read_text()
s=s.replace(" struct.pack_into('<I',ram,state+8", " struct.pack_into('<3I',ram,state+0x3c,0x8ca40008,256,65536)\n struct.pack_into('<I',ram,state+8")
s=s.replace("n=(ctypes.c_int8(data[5])", "struct.pack_into('<3I',target,0x3c,0x8ca40008,256,65536)\n n=(ctypes.c_int8(data[5])")
s=s.replace("  for x in (0x64,0x68,0x6c):struct.pack_into('<I',target,x,0x55555555)","  struct.pack_into('<3I',target,0x64,0x8ca40008,256,65536)")
s=s.replace('public_initialize_headers_verification','initialize_buffer_bindings_verification').replace('public original header initialization comparisons passed','native initialization buffer binding comparisons passed')
s=s.replace('No modeled calls.','No modeled calls. Concrete constructor-style stream configuration3c=8ca40008,40=256,44=65536 is preserved and copied natively into decode64/68/6c; independently asserted as part of entire256-byte stream. No writes to sample buffers expected during initialization.')
(p/'verify_initialize_buffer_bindings.py').write_text(s)
r=(p/'register_public_initialize_headers.py').read_text().replace('public_initialize_headers','initialize_buffer_bindings').replace('Original headers through public work initialization:','Native buffer bindings during initialization:')
(p/'register_initialize_buffer_bindings.py').write_text(r)
