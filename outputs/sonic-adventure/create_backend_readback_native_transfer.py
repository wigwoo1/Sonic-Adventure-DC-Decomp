from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_backend_readback_native_word.py').read_text().replace('from verify_tikal_controller import Call,P','from verify_tikal_controller import Call,P\nfrom verify_engine_cache_writeback import IO,Read,Write')
i=s.index('cases=0');s=s[:i]+"transfer=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/engine_transfer_read_strided_io.dll')).sa_engine_transfer_read_strided_io;transfer.argtypes=abi[:7]+[ctypes.POINTER(IO)]+abi[7:]\n"+s[i:]
s=s.replace('value,bank,answer in itertools.product','value,bank,answer,mode in itertools.product').replace('[0,0xffffffff]):','[0,0xffffffff],[0,1]):',1)
i=s.index(' before=bytes(ram)');s=s[:i]+" struct.pack_into('<I',ram,0x5fbb14,0x8cb08000);struct.pack_into('<I',ram,0xb080d4,mode)\n signed_selector=ctypes.c_int8(value&255).value;device=(0xa0800000+0x8ca30400+0x55*64+signed_selector*32)&0xffffffff\n payload=int.from_bytes(bytes((value+i)&255 for i in range(4)),'little');mmio={device:payload}\n"+s[i:]
i=s.index(' def service');s=s[:i]+" errors=[]\n @Read\n def read(ctx,a,n):\n  if a!=device or n!=4:errors.append((a,n));return 0\n  reads.append(a);return payload\n @Write\n def write(ctx,a,v,n):errors.append(('write',a,n))\n io=IO(None,read,write)\n"+s[i:]
a=s.index('  if a==0x8c10d460:');z=s.index('  assert a in dict(specs)',a)
s=s[:a]+"  if a==0x8c10d460:return transfer(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),ctypes.byref(io),None,call)\n  if a in [0x8c10ee9c,0x8c10eee0,0x8c10ee20]:r[0]=0;return 0\n"+s[z:]
a=s.index(' def read_hook');z=s.index(' def hook',a);s=s[:a]+" def poll(m,r,sr):r[0]=0;return sr\n"+s[z:]
s=s.replace('0x8c10d460:read_hook','**{a:poll for a in [0x8c10ee9c,0x8c10eee0,0x8c10ee20]}').replace("fpu=f,special={'macl':0}","fpu=f,special={'macl':0},mmio=mmio")
s=s.replace(' assert calls==', ' assert not errors\n assert calls==')
s=s.replace('28 complete','56 complete').replace('Transfer10d460 and nine other field readers modeled','Native transfer10d460 connected with controlled MMIO word and context modes0/1. Polling/control services and nine other field readers modeled').replace('backend_readback_native_word_verification.json','backend_readback_native_transfer_verification.json')
(p/'verify_backend_readback_native_transfer.py').write_text(s)
reg=(p/'register_backend_readback_native_word.py').read_text().replace('native_word','native_transfer').replace('==28','==56')
(p/'register_backend_readback_native_transfer.py').write_text(reg)
