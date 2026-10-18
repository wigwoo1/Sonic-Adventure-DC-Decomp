from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_sound_source_commit.py').read_text()
s=s.replace('for mode,offset,count,callback,bank',"move=load('byte_move.dll','sa_byte_move')\nfor mode,offset,count,callback,bank")
s=s.replace('if address in locks:return locks[address]', 'if address==0x8c10d25c:return move(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)\n        if address in locks:return locks[address]')
s=s.replace('[0x8c10d25c,0x8c010100]','[0x8c010100]')
s=s.replace('sound_source_commit_verification','source_commit_native_copy_verification').replace('Byte copy10d25c and error callback modeled; native copy integration,','Native byte move10d25c; only error callback modeled.').replace('source commit comparisons','native copy source commit comparisons')
(p/'verify_source_commit_native_copy.py').write_text(s)
r=(p/'register_work_complete_native_ranges.py').read_text().replace('work_complete_native_ranges','source_commit_native_copy').replace('== 432','== 192').replace("{'sa_sound_stream_work_complete','sa_sound_driver_split_range'}","{'sa_sound_source_commit','sa_byte_move'}").replace('Native ranges in stream work completion:','Native byte move in source commit:')
(p/'register_source_commit_native_copy.py').write_text(r)
(p/'build_byte_move.ps1').write_text((p/'build_sound_source_commit.ps1').read_text().replace('sound_source_commit','byte_move'))
