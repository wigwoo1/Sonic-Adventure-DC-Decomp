import subprocess,sys,struct
from pathlib import Path
p=Path(__file__).resolve().parent;b=(p.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
assert struct.unpack_from('<3H',b,0x669278-0x10000)==(0xd301,0x432b,9)
assert struct.unpack_from('<I',b,0x669280-0x10000)[0]==0x8c66adc4
stem='sound_driver_readback_wrapper';start=0x8c669278;end=0x8c66927e
(p/'analysis/instructions'/f'{stem}.tsv').write_text('address\topcode\tinstruction\treferences\n8c669278\td301\tmov.l 0x8c669280,r3\tREAD:8c669280=8c66adc4\n8c66927a\t432b\tjmp @r3\tCOMPUTED_JUMP:8c66adc4\n8c66927c\t9\t_nop\t\n')
subprocess.run([sys.executable,str(p/'ghidra/translate_tikal_helper.py'),'sa_'+stem,f'{start:x}',f'{end:x}',stem+'.tsv'],check=True)
(p/f'build_{stem}.ps1').write_text((p/'build_sound_driver_pan.ps1').read_text().replace('sound_driver_pan',stem))
s=(p/'verify_sound_driver_volume_get.py').read_text();i=s.index('cases=0');s=s[:i]+"wrapper=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_readback_wrapper.dll')).sa_sound_driver_readback_wrapper;wrapper.argtypes=abi\n"+s[i:]
s=s.replace('  assert a==0x8c669278;', '  if a==0x8c669278:return wrapper(m,f,sr,r,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)\n  assert a==0x8c66adc4;')
s=s.replace('{0x8c669278:backend}','{0x8c66adc4:backend}')
s=s.replace('Backend669278 modeled','Native three-instruction readback wrapper669278 connected;backend66adc4 modeled').replace('sound_driver_volume_get_verification.json','sound_volume_get_native_wrapper_verification.json')
(p/'verify_sound_volume_get_native_wrapper.py').write_text(s)
reg=(p/'register_sound_linear_source_query.py').read_text();a=reg.index('for stem,start,end in [');z=reg.index('\n',a);reg=reg[:a]+'for stem,start,end in '+repr([(stem,start,end)])+':'+reg[z:];reg=reg.replace('==800','==1024').replace("report=f'analysis/{stem}_verification.json'","report='analysis/sound_volume_get_native_wrapper_verification.json'").replace('verify_{stem}.py','verify_sound_volume_get_native_wrapper.py')
(p/'register_sound_driver_readback_wrapper.py').write_text(reg)
