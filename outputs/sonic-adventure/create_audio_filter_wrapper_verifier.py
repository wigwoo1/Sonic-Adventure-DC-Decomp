from pathlib import Path
root=Path(__file__).resolve().parent
text=(root/'verify_audio_filter_coefficients.py').read_text()
text=text.replace('cases=0', "wrapper=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/math_result_error_wrapper.dll')).sa_math_result_error_wrapper\nwrapper.argtypes=fn.argtypes\ncases=0")
text=text.replace('assert address==0x8c10f90c', "if address==0x8c10f90c:\n            return wrapper(mem,fp,sr,regs,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)\n        assert address==0x8c10fcbc")
text=text.replace('{0x8c10f90c:hook}', '{0x8c10fcbc:hook}')
text=text.replace("ram[0xa20000:0xa20010]=bytes([0x55])*16", "ram[0xa20000:0xa20010]=bytes([0x55])*16\n    struct.pack_into('<I',ram,0x7ac108,0x12345678)")
text=text.replace('assert regs[15]==0x8cf00000', "assert struct.unpack_from('<I',native,0x7ac108)[0]==0x12345678\n    assert regs[15]==0x8cf00000")
text=text.replace('audio_filter_coefficients_verification.json','audio_filter_wrapper_verification.json')
text=text.replace('Trigonometric callee10f90c modeled;', 'Native result-error wrapper10f90c connected; only lower trigonometric routine10fcbc modeled; prior error field preserved for finite results;')
text=text.replace('audio filter coefficient comparisons passed','audio filter with native wrapper comparisons passed')
(root/'verify_audio_filter_wrapper.py').write_text(text)
