from pathlib import Path
p=Path(__file__).resolve().parents[1]
t=(p/'verify_file_mount_original_disc.py').read_text()
t=t.replace('cases=0',"select=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_directory_select.dll')).sa_file_directory_select;select.argtypes=abi;functions[0x8c60041a]=select\nlookup=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_filename_lookup.dll')).sa_file_filename_lookup;lookup.argtypes=abi;functions[0x8c6455ce]=lookup\nfold=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_filename_lookup.dll')).sa_file_name_equal_folded;fold.argtypes=abi;functions[0x8c6453e2]=fold\n"+'''selected=next(x for x in manifest['files'] if x['path']=='SONICADV' and x['directory'])
selected_data=disc_read(selected['lba'],(selected['size']+2047)//2048)
root_records=expected_records;expected_records=[];pos=0
while pos<selected['size']:
 length=selected_data[pos]
 if not length:pos=(pos//2048+1)*2048;continue
 record=selected_data[pos:pos+length];name=record[33:33+record[32]]
 if record[25]&2:name=b'.' if name==b'\\0' else b'..' if name==b'\\1' else name
 else:name=name[:-2]
 expected_records.append((struct.unpack_from('<I',record,2)[0],struct.unpack_from('<I',record,10)[0],name));pos+=length
if len(expected_records)>=2:expected_records[1]=(expected_records[0][0],expected_records[1][1],expected_records[1][2])
'''+'cases=0')
t=t.replace('(0xa03004,1024)','(0xa03004,4096)')
t=t.replace(' volume=original_volume;'," ram[0x80f0:0x80fe]=b'SONICADV    '+bytes([1,0])\n volume=original_volume;")
t=t.replace('(len(expected_records),1024,root_lba+150)',"(len(expected_records),4096,selected['lba']+150)")
t=t.replace("list(range(root_lba+150,root_lba+150+(root_size+2047)//2048))", "list(range(root_lba+150,root_lba+150+(root_size+2047)//2048))+list(range(selected['lba']+150,selected['lba']+150+(selected['size']+2047)//2048))")
t=t.replace("'root_records':len(expected_records)","'root_records':len(root_records),'selected_records':len(expected_records),'selected_directory':'SONICADV'")
t=t.replace('boot selector0','boot selector1 selecting original SONICADV directory').replace('Physical disc hardware and alternate boot-directory path unverified.','Physical disc hardware callbacks remain modeled; boot directory selection, lookup and final parent-sector adjustment execute natively.')
t=t.replace('file_mount_original_disc_verification.json','file_mount_original_boot_directory_verification.json').replace('original disc native mount comparisons passed','original boot directory native mount comparisons passed')
(p/'verify_file_mount_original_boot_directory.py').write_text(t)
