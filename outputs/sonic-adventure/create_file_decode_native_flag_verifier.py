from pathlib import Path
root=Path(__file__).resolve().parent
text=(root/'verify_file_decode_request_process.py').read_text()
text=text.replace('cases=0', "flag=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_decode_odd_flag_set.dll')).sa_file_decode_odd_flag_set\nflag.argtypes=fn.argtypes\ncases=0")
text=text.replace('original=bytes(ram[0xa20000:0xa20040]);', "ram[0x673b74:0x673b80]=bytes([0x55])*12;struct.pack_into('<I',ram,0x673b78,0x12345678)\n    original=bytes(ram[0xa20000:0xa20040]);")
text=text.replace('def call(ctx,address,mem,fp,sr,regs,pr):service(address,mem,regs,actual);return 0', 'def call(ctx,address,mem,fp,sr,regs,pr):\n        if address==0x8c644c90:return flag(mem,fp,sr,regs,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)\n        service(address,mem,regs,actual);return 0')
text=text.replace('[0x8c65b120,0x8c65b238,0x8c644c90]},fpu=f', '[0x8c65b120,0x8c65b238]},fpu=f')
text=text.replace('if not mono and result<0x80000000 and result&1:trace.append((0x8c644c90,))','')
text=text.replace('    cases+=1', "    wanted=1 if phase in [1,2] and channels!=1 and result<0x80000000 and result&1 else 0x12345678\n    assert native[0x673b74:0x673b80]==bytes([0x55])*4+wanted.to_bytes(4,'little')+bytes([0x55])*4\n    cases+=1")
text=text.replace('file_decode_request_process_verification.json','file_decode_native_flag_verification.json')
text=text.replace('Decoder kernels65b120/65b238 and flag setter644c90 modeled.', 'Native flag setter644c90 connected; only decoder kernels65b120/65b238 modeled. Independent global673b78 and neighbor guards checked for eligible/ineligible results.')
text=text.replace('file decoder dispatcher comparisons passed','file dispatcher native flag comparisons passed')
(root/'verify_file_decode_native_flag.py').write_text(text)
