from pathlib import Path
p=Path(__file__).resolve().parents[1]
t=(p/'verify_file_mount_native_volume_rejection.py').read_text()
extras=[(0x8c5ff9c0,'sa_file_volume_word_get','file_volume_helpers'),(0x8c6454fe,'sa_file_directory_load','file_directory_parser'),(0x8c645468,'sa_file_directory_record_parse','file_directory_parser'),(0x8c64535c,'sa_file_directory_bytes_copy','file_directory_parser'),(0x8c645442,'sa_file_name_checksum','file_filename_lookup'),(0x8c645348,'sa_file_name_upper','file_filename_lookup'),(0x8c5ffcfe,'sa_file_directory_parent_sector_adjust','file_directory_parent_sector_adjust')]
t=t.replace('cases=0','for address,name,dll in '+repr(extras)+':\n helper=getattr(ctypes.CDLL(str(ROOT/("work/sonic-adventure/build/"+dll+".dll"))),name);helper.argtypes=abi;functions[address]=helper\ncases=0')
t=t.replace('[0,1,2,3,4]','[0]')
t=t.replace('(0xa0100c,0x8cb03000)', '(0xa01010,0x8ca03000),(0xa03004,3),(0xa0100c,0x8cb03000)')
a=t.index(' volume=');b=t.index(' events=',a)
t=t[:a]+''' volume=bytearray(2048);volume[1:6]=b'CD001'
 for o,v in [(0x8c,400),(0x84,999),(0x9e,300),(0xa6,2048)]:struct.pack_into('<I',volume,o,v)
 directory=bytearray(2048)
 for i,name in enumerate([b'FIRST.MLT;1',b'SECOND.MLT;1']):
  o=i*64;directory[o]=64;struct.pack_into('<I',directory,o+2,700+i);struct.pack_into('<I',directory,o+10,2048);directory[o+32]=len(name);directory[o+33:o+33+len(name)]=name
''' +t[b:]
t=t.replace('put(m,0xa01040,volume);regs[0]=0x1234','assert regs[4] in [450,struct.unpack_from("<I",binary,0x66c530-0x10000)[0]]\n   put(m,0xa01040,directory if regs[4]==450 else volume);regs[0]=0x1234')
a=t.index(' assert r[0]==');b=t.index(' assert sum(',a)
t=t[:a]+''' assert r[0]==0
 assert struct.unpack_from('<H',native,0xa01038)[0]==1
 assert struct.unpack_from('<II',native,0xa01004)==(550,999)
 assert struct.unpack_from('<3I',native,0xa03000)==(2,3,450)
 for i,name in enumerate([b'FIRST.MLT',b'SECOND.MLT']):
  o=0xa03010+i*44;assert struct.unpack_from('<II',native,o)==(700+i,2048);assert native[o+10:o+11+len(name)]==name+b'\\0'
 assert [e[1][0] for e in actual if e[0]==0x8cb70000]==[struct.unpack_from('<I',binary,0x66c530-0x10000)[0],450]
''' +t[b:]
a=t.index('report=');b=t.index('\n(root/',a)
t=t[:a]+"report={'cases_passed':cases,'scope':'Native mount succeeds through volume decoding, directory loading, record parsing and reserved-handle sector read pipeline; full retail RAM/CPU/FPU/SR/PR equality. Synthetic valid volume/root directory, boot selector0, registration slot variants and FPU banks. Device/registration callbacks modeled. Independent mounted flag, volume fields, root records and read sector sequence. Original disc mounting and alternate boot-directory path unverified.'}"+t[b:]
t=t.replace('file_mount_native_volume_rejection_verification.json','file_mount_native_success_verification.json').replace('mount native volume rejection comparisons passed','native mount success comparisons passed')
(p/'verify_file_mount_native_success.py').write_text(t)
