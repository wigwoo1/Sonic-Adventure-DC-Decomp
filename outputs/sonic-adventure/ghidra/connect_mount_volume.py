from pathlib import Path
p=Path(__file__).resolve().parents[1]
t=(p/'verify_file_mount_native_request_errors.py').read_text()
extra=[(0x8c5ffc98,'sa_file_mount_sector_read','file_mount_sector_read'),(0x8c600f1e,'sa_file_read_sectors','file_read_sectors'),(0x8c601084,'sa_file_read_aligned','file_read_aligned'),(0x8c600fac,'sa_file_read_unaligned','file_read_unaligned'),(0x8c5ff9e6,'sa_file_volume_signature_check','file_volume_helpers')]
t=t.replace('cases=0','for address,name,dll in '+repr(extra)+':\n helper=getattr(ctypes.CDLL(str(ROOT/("work/sonic-adventure/build/"+dll+".dll"))),name);helper.argtypes=abi;functions[address]=helper\ncases=0')
t=t.replace('(0xb03048,0x8cb50000)', '(0xb03048,0x8cb50000),(0xb0304c,0x8cb60000),(0xb03004,0x8cb70000),(0xb03040,0x8cb80000)')
t=t.replace(' events=[];actual=[]'," volume=bytearray(2048);volume[1:6]=b'CD001';volume[1+scenario]^=0x80\n events=[];actual=[]")
a=t.index('  elif target==0x8cb30000:');b=t.index('  else:raise AssertionError',a)
t=t[:a]+'''  elif target==0x8cb30000:regs[0]=0x1234
  elif target==0x8cb50000 or target==0x8cb80000:
   put(m,0xa020d0,struct.pack('<H',1));put(m,0xa0101c,bytes(4));regs[0]=0
  elif target==0x8cb40000:
   put(m,regs[4]&0xffffff,struct.pack('<II',3,128));regs[0]=1
  elif target==0x8cb60000:regs[0]=0
  elif target==0x8cb70000:
   assert regs[5]==1 and regs[6]==0x8ca01040
   put(m,0xa01040,volume);regs[0]=0x1234
''' +t[b:]
t=t.replace('[0x8c64594c,0x8cb30000,0x8cb40000,0x8cb50000]', '[0x8c64594c,0x8cb30000,0x8cb40000,0x8cb50000,0x8cb60000,0x8cb70000,0x8cb80000]')
t=t.replace('assert r[0]==([0xfffffffe,0xfffffffe,0xffffffe3,0xffffffe9,0xffffffef][scenario])',"assert r[0]==0xfffffffb\n assert bytes(native[0xa01040:0xa01840])==bytes(volume)\n assert struct.unpack_from('<H',native,0xa01038)[0]==0\n assert [e[0] for e in actual if e[0]!=0x8c64594c]==[0x8cb30000,0x8cb50000,0x8cb40000,0x8cb60000,0x8cb70000,0x8cb80000,0x8cb50000]")
a=t.index('report=');b=t.index('\n(root/',a)
t=t[:a]+"report={'cases_passed':cases,'scope':'Native mount through successful first disc request and native volume sector read pipeline to signature rejection, full retail RAM/CPU/FPU/SR/PR equality. Device and registration callbacks modeled; all five CD001 bytes individually corrupted, registration slot variants and FPU banks. Independent -5 result, unmounted flag, exact volume bytes and device order. Successful directory mounting pending.'}"+t[b:]
t=t.replace('file_mount_native_request_errors_verification.json','file_mount_native_volume_rejection_verification.json').replace('mount native request-error comparisons passed','mount native volume rejection comparisons passed')
(p/'verify_file_mount_native_volume_rejection.py').write_text(t)
