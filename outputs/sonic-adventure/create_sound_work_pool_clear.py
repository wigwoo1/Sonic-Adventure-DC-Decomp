import subprocess,sys
from pathlib import Path
p=Path(__file__).resolve().parent
stem='sound_work_pool_clear'
subprocess.run([sys.executable,str(p/'ghidra/translate_tikal_helper.py'),'sa_'+stem,'8c65b5fc','8c65b606','sound_work_pool_initialize_65b5e8.tsv'],check=True)
source=p/'source'/('sa_'+stem+'.c');text=source.read_text();a=text.index('/* Static C recovery');z=text.index(' */',a)+3
source.write_text(text[:a]+'/* Retail work pool clear 8c65b5fc..8c65b606. Tail-calls byte fill for\n * 1152 bytes at 8c8a3910; stream and decode-handle pools are not released. */'+text[z:])
s=(p/'verify_file_decode_pool_initialize.py').read_text().replace('file_decode_pool_initialize',stem)
for old,new in [('0x8c644b64','0x8c65b5fc'),('0x890678','0x8a3908'),('0x890680','0x8a3910'),('0x890820','0x8a3d90'),('0x890828','0x8a3d98'),('0x8c890680','0x8c8a3910'),('432','1168'),('416','1152')]:s=s.replace(old,new)
a=s.index(' def hook(');z=s.index(' @Call',a);s=s[:a]+s[z:]
a=s.index('  if address==0x8c667370:');z=s.index('  assert address==0x8c10d31c',a);s=s[:a]+s[z:]
s=s.replace(" struct.pack_into('<I',ram,0xf00000,stack)"," struct.pack_into('<I',ram,0xf00000,stack)")
s=s.replace(' native=bytearray(ram)'," ram[0x8d61ac:0x8d670c]=bytes([pattern^0x55])*1376;ram[0x890680:0x890820]=bytes([pattern^0xaa])*416\n native=bytearray(ram)")
s=s.replace(' assert r[15]==0x8cf00000;'," assert native[0x8d61ac:0x8d670c]==bytes([pattern^0x55])*1376 and native[0x890680:0x890820]==bytes([pattern^0xaa])*416\n assert r[15]==0x8cf00000;")
a=s.index("scope='");z=s.index("'),indent=2)",a)
scope='24 full retail RAM/CPU/FPU/SR/PR comparisons of work pool clear65b5fc..65b606: six byte patterns,both banks,two stack values. Native byte fill tail-call clears1152 bytes at8c8a3910,8-byte guards preserved; independent entire1376-byte stream and416-byte decoder handle pools unchanged. No modeled calls. This is a raw work-pool clear,not stream/handle destruction. Caller lifecycle/fullgame remain unverified.'
s=s[:a]+'scope='+repr(scope)+s[z+1:]
(p/f'verify_{stem}.py').write_text(s)
(p/f'build_{stem}.ps1').write_text((p/'build_sound_work_pool_initialize.ps1').read_text().replace('sound_work_pool_initialize',stem))
s=(p/'register_sound_work_pool_initialize.py').read_text().replace('sound_work_pool_initialize',stem).replace('0x8c65b5e8','0x8c65b5fc').replace("end_exclusive='0x8c65b5fc'","end_exclusive='0x8c65b606'").replace('0x64b5e8:0x64b5fc','0x64b5fc:0x64b606')
(p/f'register_{stem}.py').write_text(s)
