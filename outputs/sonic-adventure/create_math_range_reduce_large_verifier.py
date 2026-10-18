from pathlib import Path
root=Path(__file__).resolve().parent
text=(root/'verify_math_range_reduce.py').read_text()
a=text.index('inputs=');b=text.index('cases=0',a)
text=text[:a]+"inputs=[v|sign for v in [0x4effffff,0x4f000000,0x4f000001,0x4f7fffff,0x4f800000,0x4f800001,0x50000000] for sign in [0,0x80000000]]\ndivisors=[0x3f800000]\n"+text[b:]
text=text.replace('x=abs(number(value));y=abs(number(divisor));index=int(f32(x/y))\n        answer=raw(f32(x-f32(f32(float(index))*y)))|(value&0x80000000)', '''x=abs(number(value));y=abs(number(divisor));q=f32(x/y)
        if q<4294967296.:q=f32(float(max(-2147483648,min(2147483647,int(q)))))
        converted=f32(q-4294967296.) if q>2147483648. else q
        signed_index=max(-2147483648,min(2147483647,int(converted)))
        index=signed_index&0xffffffff
        reconstructed=f32(float(signed_index))
        if signed_index<0:reconstructed=f32(reconstructed+4294967296.)
        answer=raw(f32(x-f32(reconstructed*y)))|(value&0x80000000)''')
text=text.replace('math_range_reduce_verification.json','math_range_reduce_large_verification.json')
a=text.index("scope='");b=text.index("'),indent=2)",a)
text=text[:a]+"scope='Native range reducer large-quotient branch.28 full retail RAM/CPU/FPU/SR/PR/FPUL/MACL comparisons,no modeled calls. Signed inputs around2^31/2^32 and2^33,divisor1,both banks. Independent retail threshold (>2^31),binary32 subtraction of2^32,saturating signed truncation,stored uint32 index,unsigned reconstruction and signed remainder. Preserves retail saturation quirks at exactly2^31 and beyond uint32 range. PR/SZ/RM0; denormals,other modes/traps,caller integration/fullgame unverified."+text[b:]
text=text.replace('native range reduction comparisons passed','large quotient range reduction comparisons passed')
(root/'verify_math_range_reduce_large.py').write_text(text)
