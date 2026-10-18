from pathlib import Path
root=Path(__file__).resolve().parent
text=(root/'verify_audio_filter_wrapper.py').read_text()
text=text.replace('cases=0', '''from sh4_binary32 import multiply,multiply_add,divide,add_sub
lower={}
for address,dll,name in [(0x8c10fcbc,'math_trig_octant_dispatch','sa_math_trig_octant_dispatch'),(0x8c10fbb8,'math_range_reduce','sa_math_range_reduce'),(0x8c110218,'math_trig_polynomials','sa_math_cosine_polynomial'),(0x8c110238,'math_trig_polynomials','sa_math_sine_polynomial')]:
    lower[address]=getattr(ctypes.CDLL(str(ROOT/f'work/sonic-adventure/build/{dll}.dll')),name);lower[address].argtypes=fn.argtypes
def trig_oracle(angle):
    x=angle&0x7fffffff;index=int(struct.unpack('<f',struct.pack('<I',divide(x,0x3f490fdb)[0]))[0])
    remainder=add_sub(x,multiply(raw(float(index)),0x3f490fdb)[0],True)[0];octant=index&7
    argument=add_sub(remainder,0xbf490fdb)[0] if octant&1 else remainder
    cosine=octant in [0,3,4,7];table=0x8c160444 if cosine else 0x8c160460
    coefficients=struct.unpack_from('<7I',binary,table-0x8c010000);square=multiply(argument,argument)[0];answer=coefficients[0]
    for coefficient in coefficients[1:]:answer=multiply_add(square,answer,coefficient)[0]
    if not cosine:answer=multiply(argument,answer)[0]
    if octant in [1,2,3,4]:answer^=0x80000000
    return struct.unpack('<f',struct.pack('<I',answer))[0]
cases=0''')
text=text.replace('extension,rate,trig,bank','extension,rate,bank').replace('[0.,0.5,1.],','')
a=text.index('    def hook(');b=text.index('    s=State()',a)
text=text[:a]+'''    @Call
    def call(ctx,address,mem,fp,sr,regs,pr):
        if address==0x8c10f90c:
            actual.append(fp.contents.banks[bank][4])
            return wrapper(mem,fp,sr,regs,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
        assert address in lower
        return lower[address](mem,fp,sr,regs,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
'''+text[b:]
text=text.replace('reference(ram,s,{0x8c10fcbc:hook},','reference(ram,s,')
text=text.replace('assert actual==expected==[raw(f32(f32(f32(float(signed))*pi2)/f32(float(rate))))]', 'angle=raw(f32(f32(f32(float(signed))*pi2)/f32(float(rate))))\n    assert actual==[angle]\n    trig=trig_oracle(angle)')
text=text.replace('audio_filter_wrapper_verification.json','audio_filter_native_verification.json')
a=text.index("scope='");b=text.index("'),indent=2)",a)
text=text[:a]+"scope='18 full retail RAM/CPU/FPU/SR/PR/FPUL/MACL comparisons with native audio coefficient routine65adc0,result wrapper10f90c,trig dispatcher10fcbc,reducer10fbb8,polynomials110218/110238 and numerical helpers. No modeled calls. Signed extension0/+500/-500,rates22050/32000/44100,both banks. Independent binary32 angle/reduction/Horner coefficient outputs,exact rational square-root oracle,guarded16-bit outputs,unchanged prior error,nested stack/PR restoration. PR/SZ/RM0; other inputs/modes,traps,original-header configuration integration,physical audio/fullgame unverified."+text[b:]
text=text.replace('audio filter with native wrapper comparisons passed','fully native audio filter comparisons passed')
(root/'verify_audio_filter_native.py').write_text(text)
