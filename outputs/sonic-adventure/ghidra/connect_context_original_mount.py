from pathlib import Path
p=Path(__file__).resolve().parents[1]
t=(p/'verify_file_mount_original_boot_directory.py').read_text()
t=t.replace('cases=0',"functions[0x8c5ffd28]=fn\ninitialize=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_context_initialize.dll')).sa_file_context_initialize;initialize.argtypes=abi\nfor address,name,dll in "+repr([(0x8c600dd0,'sa_file_directory_storage_initialize','file_directory_storage'),(0x8c600464,'sa_file_handle_open_initialize','file_handle_open_initialize')])+':\n helper=getattr(ctypes.CDLL(str(ROOT/("work/sonic-adventure/build/"+dll+".dll"))),name);helper.argtypes=abi;functions[address]=helper\ncases=0')
t=t.replace('(0xb0301c,0x8cb30000)', '(0xb03000,0x8cb90000),(0xb0301c,0x8cb30000)')
t=t.replace('  if target==0x8c64594c:', '  if target==0x8c645328:regs[0]=0x8cb03000\n  elif target==0x8cb90000:regs[0]=0\n  elif target==0x8c64594c:')
t=t.replace('[0x8c64594c,0x8cb30000', '[0x8c645328,0x8cb90000,0x8c64594c,0x8cb30000')
t=t.replace('s.pc=0x8c5ffd28','s.pc=0x8c600038;s.r[4]=1;s.r[5]=0x8ca01000;s.r[6]=4096;s.r[7]=0x8ca03000')
t=t.replace(' native=bytearray(ram);'," struct.pack_into('<I',ram,0xf00000,0x8cb03000)\n native=bytearray(ram);")
t=t.replace('assert fn(view,','assert initialize(view,')
t=t.replace('file_mount_original_boot_directory_verification.json','file_context_original_mount_verification.json').replace('Original user disc volume/root bytes through native mount','Original user disc volume/root bytes through native context/handle/directory initialization and mount').replace('original boot directory native mount comparisons passed','original disc context initialization and mount comparisons passed')
(p/'verify_file_context_original_mount.py').write_text(t)
