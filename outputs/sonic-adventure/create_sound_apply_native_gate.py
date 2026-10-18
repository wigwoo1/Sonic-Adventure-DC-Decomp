import subprocess,sys,struct
from pathlib import Path
p=Path(__file__).resolve().parent;b=(p.parents[1]/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
specs=[('sound_driver_apply_noop',0x8c668042,0x8c668046,[0xb,0xe000],['rts','_mov #0x0,r0']),('sound_driver_gate_get',0x8c6680e2,0x8c6680ea,[0xe400,0x2542,0xb,0x6043],['mov #0x0,r4','mov.l r4,@r5','rts','_mov r4,r0'])]
for stem,start,end,ops,ins in specs:
 assert list(struct.unpack_from('<'+'H'*len(ops),b,start-0x8c010000))==ops
 tsv=stem+'.tsv';(p/'analysis/instructions'/tsv).write_text('address\topcode\tinstruction\treferences\n'+''.join(f'{start+2*i:x}\t{op:x}\t{ins[i]}\t\n' for i,op in enumerate(ops)))
 subprocess.run([sys.executable,str(p/'ghidra/translate_tikal_helper.py'),'sa_'+stem,f'{start:x}',f'{end:x}',tsv],check=True)
 (p/f'build_{stem}.ps1').write_text((p/'build_sound_driver_pan.ps1').read_text().replace('sound_driver_pan',stem))
s=(p/'verify_sound_apply_native_zero_queries.py').read_text()
s=s.replace("for address,name in [(0x8c66869a,'sa_sound_driver_zero_query_a'),(0x8c6686a4,'sa_sound_driver_zero_query_b')]:", "for address,name in [(0x8c668042,'sa_sound_driver_apply_noop'),(0x8c6680e2,'sa_sound_driver_gate_get')]:")
s=s.replace("'work/sonic-adventure/build/sound_driver_zero_queries.dll'", "f'work/sonic-adventure/build/{name[3:]}.dll'")
s=s.replace(' ram=bytearray(', ' gate=0\n ram=bytearray(',1)
s=s.replace('  service(a,m,r);return 0','  raise AssertionError(hex(a))')
a=s.index("scope='");z=s.index('\n(p/',a);s=s[:a]+"scope='168 complete retail RAM/CPU/FPU/SR/PR comparisons of control apply65df78 with native no-op668042 and zero gate getter6680e2;native reporter callback unset for null objects. Null/present,seven word fixtures,both banks,initial argument/result fixtures. No modeled services. Exact two-call native non-null path,zero gate stack output,entire144-byte object preservation and restored stack. Retail gate always zero so downstream seven-setter chain bypassed. Hardware and game integration remain unverified.'"+s[z:]
s=s.replace('sound_apply_native_zero_queries_verification.json','sound_apply_native_gate_verification.json')
(p/'verify_sound_apply_native_gate.py').write_text(s)
reg=(p/'register_sound_linear_source_query.py').read_text();a=reg.index('for stem,start,end in [');z=reg.index('\n',a);reg=reg[:a]+'for stem,start,end in '+repr([(x[0],x[1],x[2]) for x in specs])+':'+reg[z:];reg=reg.replace('==800','==168').replace("report=f'analysis/{stem}_verification.json'","report='analysis/sound_apply_native_gate_verification.json'").replace('verify_{stem}.py','verify_sound_apply_native_gate.py')
(p/'register_sound_apply_native_gate_helpers.py').write_text(reg)
reg=(p/'register_sound_apply_native_zero_queries.py').read_text().replace('native_zero_queries','native_gate').replace("{'sa_sound_subordinate_apply','sa_sound_driver_zero_query_a','sa_sound_driver_zero_query_b'}","{'sa_sound_subordinate_apply','sa_sound_driver_apply_noop','sa_sound_driver_gate_get'}")
(p/'register_sound_apply_native_gate.py').write_text(reg)
