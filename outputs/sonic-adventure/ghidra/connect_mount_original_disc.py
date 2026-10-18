from pathlib import Path
p=Path(__file__).resolve().parents[1]
t=(p/'verify_file_mount_native_success.py').read_text().replace('import ctypes,itertools,json,struct','import ctypes,itertools,json,struct,shlex,hashlib')
setup='''manifest=json.loads((root/'disc_inventory.json').read_text())
tracks=[shlex.split(line) for line in manifest['gdi'].splitlines()[1:] if line.strip()]
track=next(x for x in reversed(tracks) if x[2]=='4');base=int(track[1]);stride=int(track[3]);start=int(track[5]);source=ROOT/'unused'
source=__import__('pathlib').Path(manifest['source'])/track[4]
def disc_read(lba,count):
 data=bytearray()
 with source.open('rb') as stream:
  for i in range(count):
   stream.seek(start+(lba+i-base)*stride+(16 if stride==2352 else 0));block=stream.read(2048);assert len(block)==2048;data.extend(block)
 return bytes(data)
original_volume=disc_read(base+16,1)
assert original_volume[:7]==b'\\x01CD001\\x01'
root_lba=struct.unpack_from('<I',original_volume,158)[0];root_size=struct.unpack_from('<I',original_volume,166)[0]
original_directory=disc_read(root_lba,(root_size+2047)//2048)
expected_records=[];pos=0
while pos<root_size:
 length=original_directory[pos]
 if not length:pos=(pos//2048+1)*2048;continue
 record=original_directory[pos:pos+length];name=record[33:33+record[32]]
 if record[25]&2:name=b'.' if name==b'\\0' else b'..' if name==b'\\1' else name
 else:name=name[:-2]
 expected_records.append((struct.unpack_from('<I',record,2)[0],struct.unpack_from('<I',record,10)[0],name));pos+=length
'''
t=t.replace('cases=0',setup+'cases=0').replace('(0xa03004,3)','(0xa03004,1024)')
a=t.index(' volume=');b=t.index(' events=',a)
t=t[:a]+' volume=original_volume;directory=original_directory\n'+t[b:]
a=t.index('   assert regs[4] in');b=t.index('  else:raise AssertionError',a)
t=t[:a]+'''   assert regs[5]==1 and regs[6]==0x8ca01040
   put(m,0xa01040,disc_read(regs[4]-150,1));regs[0]=0x1234
''' +t[b:]
a=t.index(" assert struct.unpack_from('<II',native,0xa01004)");b=t.index(' assert sum(',a)
t=t[:a]+''' assert struct.unpack_from('<II',native,0xa01004)==(struct.unpack_from('<I',volume,140)[0]+150,struct.unpack_from('<I',volume,132)[0])
 assert struct.unpack_from('<3I',native,0xa03000)==(len(expected_records),1024,root_lba+150)
 for i,(lba,size,name) in enumerate(expected_records):
  o=0xa03010+i*44;assert struct.unpack_from('<II',native,o)==(lba,size);assert native[o+10:o+11+len(name)]==name+b'\\0'
 assert [e[1][0] for e in actual if e[0]==0x8cb70000]==[base+16+150]+list(range(root_lba+150,root_lba+150+(root_size+2047)//2048))
''' +t[b:]
a=t.index('report=');b=t.index('\n(root/',a)
t=t[:a]+"report={'cases_passed':cases,'root_records':len(expected_records),'volume_sha256':hashlib.sha256(original_volume).hexdigest(),'root_directory_sha256':hashlib.sha256(original_directory).hexdigest(),'scope':'Original user disc volume/root bytes through native mount, volume decoding, directory load/parser and sector-read pipeline; full retail RAM/CPU/FPU/SR/PR equality. Device/registration callbacks modeled, boot selector0. Independent mounted flag, every root record and sector sequence. Physical disc hardware and alternate boot-directory path unverified.'}"+t[b:]
t=t.replace('file_mount_native_success_verification.json','file_mount_original_disc_verification.json').replace('native mount success comparisons passed','original disc native mount comparisons passed')
(p/'verify_file_mount_original_disc.py').write_text(t)
