from pathlib import Path
p=Path(__file__).resolve().parents[1]
t=(p/'verify_file_context_bios_initialize.py').read_text()
extras=[(0x8c644ff6,'sa_file_device_read','file_device_read'),(0x8c10d938,'sa_file_bios_request','file_bios_request')]
t=t.replace('cases=0','for address,name,dll in '+repr(extras)+':\n helper=getattr(ctypes.CDLL(str(ROOT/("work/sonic-adventure/build/"+dll+".dll"))),name);helper.argtypes=abi;functions[address]=helper\ncases=0')
t=t.replace('(0x66d244,0x8cb70000)','(0x66d244,0x8c644ff6)')
t=t.replace('  elif target==0x8cba0000:regs[0]=0','''  elif target==0x8cba0000:
   assert regs[6]==0
   if regs[7]==3:regs[0]=0
   else:
    assert regs[7]==0 and regs[4]==0x11
    o=regs[5]&0xffffff;request=struct.unpack('<4I',bytes(m[o:o+16]))
    log[-1]=(target,log[-1][1],request)
    assert request[1]==1 and request[2]==0x0ca01040 and request[3]==0
    put(m,request[2]&0xffffff,disc_read(request[0]-150,request[1]));regs[0]=0x1234
  elif target in [0x8c5fc0d8,0x8c5fc336]:regs[0]=0''')
t=t.replace(',0x8cb70000,0x8cb80000]',',0x8c5fc0d8,0x8c5fc336,0x8cb80000]')
t=t.replace("[e[1][0] for e in actual if e[0]==0x8cb70000]", "[e[2][0] for e in actual if e[0]==0x8cba0000 and len(e)==3]")
t=t.replace('if e[0]==0x8cba0000]==[(0,3)]','if e[0]==0x8cba0000 and len(e)==2]==[(0,3)]')
t=t.replace('file_context_bios_initialize_verification.json','file_context_bios_sector_read_verification.json').replace('Native device initializer wrapper/BIOS dispatch','Native device read formatter/request BIOS dispatch and initializer wrapper/BIOS dispatch').replace('Device/registration callbacks modeled','BIOS sector delivery, cache helpers and remaining device/registration callbacks modeled').replace('BIOS boundary initialization and mount comparisons passed','BIOS sector boundary initialization and mount comparisons passed')
(p/'verify_file_context_bios_sector_read.py').write_text(t)
