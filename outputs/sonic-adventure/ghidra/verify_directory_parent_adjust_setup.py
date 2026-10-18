from pathlib import Path
p=Path(__file__).resolve().parents[1]
t=(p/'verify_file_volume_helpers.py').read_text().replace('file_volume_helpers.dll','file_directory_parent_sector_adjust.dll')
a=t.index('fixtures=[]');b=t.index('cases=0',a)
t=t[:a]+'''fixtures=[]
for count,present,bank,sector in itertools.product([0,1,2,3,0xffffffff],[0,1],[0,0x200000],[0,1234,0xffffffff]):
 data=bytearray(128);struct.pack_into('<I',data,0,count);struct.pack_into('<I',data,16,sector);struct.pack_into('<I',data,60,0xdeadbeef)
 fixtures.append((0x8c5ffcfe,'sa_file_directory_parent_sector_adjust',data,0,bank,(count,present,sector)))
''' +t[b:]
t=t.replace('s.r[4]=0x8c000000+offset;', 's.r[4]=0x8c000000+offset if expected[1] else 0;')
t=t.replace(' assert r[0]==expected',''' count,present,sector=expected
 wanted=bytearray(data)
 if present and 2<=count<0x80000000:struct.pack_into('<I',wanted,60,sector)
 assert native[offset:offset+len(data)]==wanted''')
a=t.index('report=');b=t.index('\n(root/',a)
t=t[:a]+"report={'cases_passed':cases,'scope':'Native directory parent-sector adjustment compared with full retail RAM/CPU/FPU/SR/PR without hooks. Null/present directory, signed counts0/1/2/3/-1, sector values0/1234/ffffffff and banks. Independent complete directory bytes, including copied first-to-second sector only for count>=2.'}"+t[b:]
t=t.replace('file_volume_helpers_verification.json','file_directory_parent_sector_adjust_verification.json').replace('volume helper comparisons passed','directory parent-sector adjustment comparisons passed')
(p/'verify_file_directory_parent_sector_adjust.py').write_text(t)
