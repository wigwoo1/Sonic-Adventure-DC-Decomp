from pathlib import Path
root=Path(__file__).resolve().parent
text=(root/'verify_math_trig_octant_dispatch.py').read_text()
text=text.replace('cases=0', "from sh4_binary32 import multiply,multiply_add\npolydll=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/math_trig_polynomials.dll'))\npoly={}\nfor address,name in [(0x8c110218,'sa_math_cosine_polynomial'),(0x8c110238,'sa_math_sine_polynomial')]:\n    poly[address]=getattr(polydll,name);poly[address].argtypes=fn.argtypes\ncases=0")
text=text.replace('def call(ctx,address,mem,fp,sr,regs,pr):apply(address,mem,fp.contents,regs,actual);return 0', 'def call(ctx,address,mem,fp,sr,regs,pr):\n        if address in poly:return poly[address](mem,fp,sr,regs,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)\n        apply(address,mem,fp.contents,regs,actual);return 0')
text=text.replace('[0x8c10fbb8,0x8c110218,0x8c110238]},fpu=f', '[0x8c10fbb8]},fpu=f')
text=text.replace('[(0x8c10fbb8,raw(abs(input_value)),0x3f490fdb),(chosen,arg,0x3f490fdb)]', '[(0x8c10fbb8,raw(abs(input_value)),0x3f490fdb)]')
text=text.replace("answer=raw(0.75 if chosen==0x8c110218 else 0.5)", "table=0x8c160444 if chosen==0x8c110218 else 0x8c160460\n    coefficients=struct.unpack_from('<7I',binary,table-0x8c010000)\n    square=multiply(arg,arg)[0];answer=coefficients[0]\n    for coefficient in coefficients[1:]:answer=multiply_add(square,answer,coefficient)[0]\n    if chosen==0x8c110238:answer=multiply(arg,answer)[0]")
text=text.replace('math_trig_octant_dispatch_verification.json','math_trig_native_polynomials_verification.json')
text=text.replace('Lower reducer10fbb8 and result helpers110218/110238 modeled.', 'Native polynomial helpers110218/110238 connected with native FMAC/multiplication; only reducer10fbb8 modeled. Independent original-table Horner evaluation checks all8 result selections/signs.')
text=text.replace('actual reduction/polynomials,','actual reduction,')
text=text.replace('math octant dispatch comparisons passed','trig dispatcher with native polynomials comparisons passed')
(root/'verify_math_trig_native_polynomials.py').write_text(text)
