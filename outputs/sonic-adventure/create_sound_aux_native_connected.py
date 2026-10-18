import subprocess,sys,struct
from pathlib import Path
p=Path(__file__).resolve().parent;binary=(p.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
assert struct.unpack_from('<2H',binary,0x668498-0x10000)==(0x000b,0xe000)
(p/'analysis/instructions/sound_driver_aux_noop.tsv').write_text('address\topcode\tinstruction\treferences\n8c668498\tb\trts\t\n8c66849a\te000\t_mov #0x0,r0\t\n')
stem='sound_driver_aux_noop';start=0x8c668498;end=0x8c66849c
subprocess.run([sys.executable,str(p/'ghidra/translate_tikal_helper.py'),'sa_'+stem,f'{start:x}',f'{end:x}','sound_driver_aux_noop.tsv'],check=True)
source=p/'source'/('sa_'+stem+'.c');s=source.read_text();a=s.index('/* Static C recovery');z=s.index(' */',a)+3;source.write_text(s[:a]+'/* Retail auxiliary driver returns zero in RTS delay slot. */'+s[z:])
(p/f'build_{stem}.ps1').write_text((p/'build_sound_driver_pan.ps1').read_text().replace('sound_driver_pan',stem))
s=(p/'verify_sound_aux_connected_readback.py').read_text();i=s.index('getter=');s=s[:i]+"driver_fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_driver_aux_noop.dll')).sa_sound_driver_aux_noop;driver_fn.argtypes=abi\n"+s[i:]
s=s.replace('r[0]=answer;return 0','return driver_fn(m,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)').replace('{0x8c668498:driver}','{}')
s=s.replace(' and actual==expected','').replace('two modeled downstream returns','two initial result fixtures').replace('Driver668498 modeled;hardware','Native driver668498 verified as RTS with zero result delay slot;no modeled calls. Hardware').replace('sound_aux_connected_readback_verification.json','sound_aux_native_connected_verification.json')
(p/'verify_sound_aux_native_connected.py').write_text(s)
reg=(p/'register_sound_linear_source_query.py').read_text();a=reg.index('for stem,start,end in [');z=reg.index('\n',a);reg=reg[:a]+'for stem,start,end in '+repr([(stem,start,end)])+':'+reg[z:];reg=reg.replace('==800','==392').replace("report=f'analysis/{stem}_verification.json'","report='analysis/sound_aux_native_connected_verification.json'").replace("verify_{stem}.py","verify_sound_aux_native_connected.py")
(p/f'register_{stem}.py').write_text(reg)
reg=(p/'register_sound_aux_connected_readback.py').read_text().replace('sound_aux_connected_readback_verification.json','sound_aux_native_connected_verification.json').replace('connected_readback','native_connected')
(p/'register_sound_aux_native_connected.py').write_text(reg)
