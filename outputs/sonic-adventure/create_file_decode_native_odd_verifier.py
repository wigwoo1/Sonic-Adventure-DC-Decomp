from pathlib import Path

root = Path(__file__).resolve().parent
text = (root / 'verify_file_decode_retail_pipeline.py').read_text()
text = text.replace('kernel.argtypes=fn.argtypes', "kernel.argtypes=fn.argtypes\nflag=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_decode_odd_flag_set.dll')).sa_file_decode_odd_flag_set\nflag.argtypes=fn.argtypes")
text = text.replace('for asset,bank in itertools.product(assets,[0,1]):', 'for asset,bank,count in itertools.product(assets,[0,1],[1,3,15]):')
text = text.replace('count=16;terminal=', 'terminal=')
text = text.replace('native=bytearray(ram);', "ram[0x673b74:0x673b80]=bytes([0x55])*12;struct.pack_into('<I',ram,0x673b78,0x12345678)\n    native=bytearray(ram);")
text = text.replace('assert address==0x8c65b238,address', "if address==0x8c644c90:return flag(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)\n        assert address==0x8c65b238,address")
text = text.replace('channel_blocks=decoded//2', "channel_blocks=decoded//2\n    wanted=1 if decoded&1 else 0x12345678\n    assert native[0x673b74:0x673b80]==bytes([0x55])*4+wanted.to_bytes(4,'little')+bytes([0x55])*4")
text = text.replace('bank=bank,coefficients=coefficients,decoded_blocks=decoded', 'bank=bank,requested_blocks=count,coefficients=coefficients,decoded_blocks=decoded')
start = text.index("(out/'analysis/file_decode_retail_pipeline_verification.json')")
text = text[:start] + """(out/'analysis/file_decode_native_odd_verification.json').write_text(json.dumps(dict(cases_passed=cases,original_files=len(assets),assets=records,scope='654 full retail RAM/CPU/FPU/SR/PR/FPUL/MACL comparisons of native file dispatcher, stereo decoder and odd-result flag setter using original prefixes from109 two-channel magic8000 files, requested block counts1/3/15 and both FPU banks. No modeled calls. Independent exact binary32 recurrence checks every produced sample and stored history; source/output guards, global flag and neighboring bytes, phase/result/stack checked. Preserves original odd-count behavior: requested count returned, trailing unpaired block ignored, global flag set. Full payload scheduling, termination markers, physical playback and fullgame remain unverified by this test.'),indent=2)+'\\n')
print(cases,'native file dispatcher odd-count comparisons passed')
"""
(root / 'verify_file_decode_native_odd.py').write_text(text)
