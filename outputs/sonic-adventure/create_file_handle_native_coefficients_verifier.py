from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_file_handle_pool.py').read_text()
donor=(p/'verify_magic_retail_native_initializer.py').read_text()
helper=donor[donor.index('import math'):donor.index('assets=json.loads')]
helper=helper.replace('binary,','b,')
s=s.replace('pool=struct.unpack_from',"binary=b;lower={}\n"+helper+"\npool=struct.unpack_from",1)
start=s.index('    def service(');end=s.index('    s=State();',start)
s=s[:start]+'''    @Call
    def call(ctx,address,mem,fp,srp,r,prp):
        target=fill if address==0x8c10d31c else lower[address]
        return target(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
'''+s[end:]
s=s.replace('reference(ram,s,{0x8c65adc0:hook},fpu=f,special=special)','reference(ram,s,{},fpu=f,special=special,vector_instruction=sqrt_service)')
s=s.replace('    assert native==ram', '    nf.fpul=fpul.value\n    assert native==ram')
s=s.replace('reference(ram,s,fpu=f,special=special)','reference(ram,s,fpu=f,special=special,vector_instruction=sqrt_service)')
formula=donor[donor.index('  signed=ctypes.c_int16(extension)'):donor.index('  file_target=bytearray')]
formula=formula.replace('  ','        ')
formula=formula.replace('signed=ctypes.c_int16(extension).value;',"extension=struct.unpack_from('<h',b,0x8c644c9a-base)[0];rate=struct.unpack_from('<I',b,0x8c644ca4-base)[0];signed=extension;")
s=s.replace("        obj=bytearray(size);struct.pack_into('<2I',obj,0,1,free);struct.pack_into('<2h',obj,48,123,-456);wanted[free*size:(free+1)*size]=obj",formula+"        obj=bytearray(size);struct.pack_into('<2I',obj,0,1,free);struct.pack_into('<2H',obj,48,a,b);wanted[free*size:(free+1)*size]=obj")
# Avoid shadowing the retail binary with the oracle's second coefficient.
s=s.replace('a=int(f32(coefficient*8192.))&65535;b=int(', 'coef_a=int(f32(coefficient*8192.))&65535;coef_b=int(').replace("obj,48,a,b)","obj,48,coef_a,coef_b)")
s=s.replace('file_handle_pool_verification','file_handle_native_coefficients_verification').replace('Byte fill native; coefficient generation65adc0 modeled with argument assertions and distinct signed results. Native coefficient integration and concurrency unverified.','Byte fill, coefficient generation65adc0, trig wrapper/dispatcher/range reducer/polynomials and numerical helpers native. Independent default header-derived binary32 coefficient calculation. No modeled calls; host concurrency and fullgame unverified.').replace('file handle pool sequences','native coefficient file handle pool sequences')
(p/'verify_file_handle_native_coefficients.py').write_text(s)
r=(p/'register_input_provider_native_remainder.py').read_text().replace('input_provider_native_remainder','file_handle_native_coefficients').replace("== 108","== 18")
a=r.index('names = ');z=r.index('\npath = ',a)
r=r[:a]+"names = {'sa_file_decode_handle_allocate','sa_file_decode_handle_release'}"+r[z:]
r=r.replace('Input provider native remainder integration:','Native coefficient handle pool:')
(p/'register_file_handle_native_coefficients.py').write_text(r)
