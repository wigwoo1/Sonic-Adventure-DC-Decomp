from pathlib import Path
p=Path(__file__).resolve().parents[1]
t=(p/'verify_file_volume_helpers.py').read_text().replace('file_volume_helpers.dll','file_directory_parser.dll')
a=t.index('@Call');b=t.index('cases=0',a)
t=t[:a]+'''helpers={0x8c64535c:lib.sa_file_directory_bytes_copy}
names=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_filename_lookup.dll'))
for address,name in [(0x8c645442,'sa_file_name_checksum'),(0x8c645348,'sa_file_name_upper')]:helpers[address]=getattr(names,name)
for helper in helpers.values():helper.argtypes=abi
@Call
def call(ctx,target,m,f,status,regs,pr):return helpers[target](m,f,status,regs,pr,ctypes.byref(fpul),ctypes.byref(macl),None,call)
fixtures=[]
for name,flags,alignment,bank in itertools.product([b'E_00B7.MLT;1',b'other.bin;1',b'\\0',b'\\1'],[0,2],range(4),[0,0x200000]):
 data=bytearray(64);data[0]=34+len(name);struct.pack_into('<I',data,2,1234);struct.pack_into('<I',data,10,13120);data[25]=flags;data[32]=len(name);data[33:33+len(name)]=name
 fixtures.append((0x8c645468,'sa_file_directory_record_parse',data,alignment,bank,data[0]))
''' +t[b:]
t=t.replace('s.r[4]=0x8c000000+offset;', 's.r[4]=0x8c000000+offset;s.r[5]=0x8cb00000;')
t=t.replace(' assert r[0]==expected',''' assert r[0]==expected
 assert struct.unpack_from('<II',native,0xb00000)==(1234,13120)
 rawname=bytes(data[33:33+data[32]])
 outname=rawname if data[25]&2 else rawname[:-2]
 if data[25]&2 and rawname==b'\\0':outname=b'.'
 if data[25]&2 and rawname==b'\\1':outname=b'..'
 assert native[0xb0000a:0xb0000b+len(outname)]==outname+b'\\0'
 seed=struct.unpack_from('<h',binary,0x64554e-0x10000)[0]
 assert native[0xb00009]==((seed+sum(outname.upper()))&0xffffffff)>>2&255''')
# Special one-byte entries are directory records; a file entry shorter than its suffix is invalid input.
t=t.replace("data=bytearray(64);", "if flags==0 and len(name)<2:continue\n data=bytearray(64);")
a=t.index('report=');b=t.index('\n(root/',a)
t=t[:a]+"report={'cases_passed':cases,'scope':'Native record conversion, byte copy, filename checksum and uppercase against full retail RAM/CPU/FPU/SR/PR without hooks. File/directory names, suffix stripping, special dot entries, unaligned source and FPU banks; independent decoded fields, name and checksum. Directory loader compiled but not behaviorally verified.'}"+t[b:]
t=t.replace('file_volume_helpers_verification.json','file_directory_record_verification.json').replace('volume helper comparisons passed','directory record comparisons passed')
(p/'verify_file_directory_record.py').write_text(t)
