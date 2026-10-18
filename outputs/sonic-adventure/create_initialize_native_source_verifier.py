from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_sound_stream_work_initialize.py').read_text()
s=s.replace('cases=0',"for a,stem in [(0x8c65e4b8,'sound_source_query'),(0x8c65e58a,'sound_source_commit'),(0x8c65e644,'sound_source_return'),(0x8c65e368,'sound_source_lock_enter'),(0x8c65e380,'sound_source_lock_exit'),(0x8c10cbb8,'integer_remainder')]:helpers[a]=load(stem,'sa_'+stem);helpers[a].argtypes=fn.argtypes\ncases=0")
s=s.replace("    struct.pack_into('<I',ram,0xa80000,0x8ca90000)","    struct.pack_into('<12I',ram,0xa80000,0x8ca90000,1,0,length,0,0,0,0x8ca10000,8192,0,0,0)\n    object_before=bytes(ram[0xa80000:0xa80030])")
s=s.replace('(0x18,0x8c010100),(0x1c,0x8c010200),(0x20,0x8c010300)','(0x18,0x8c65e4b8),(0x1c,0x8c65e644),(0x20,0x8c65e58a)')
s=s.replace('[0x8c010100,0x8c010200,0x8c010300,0x8c667596,0x8c6676a4]','[0x8c667596,0x8c6676a4]')
s=s.replace("[0x8c010100]+([0x8c667596] if length>=16 else [])+([0x8c6676a4,0x8c6676a4,0x8c010300,0x8c010200] if success else [0x8c010200])","([0x8c667596] if length>=16 else [])+([0x8c6676a4,0x8c6676a4] if success else [])")
s=s.replace('    cases+=1',"    wanted_object=bytearray(object_before)\n    if success:\n        for offset,value in [(0xc,length-result),(0x10,result),(0x18,result)]:struct.pack_into('<I',wanted_object,offset,value)\n    assert native[0xa80000:0xa80030]==wanted_object\n    cases+=1")
s=s.replace('Native split/copy and byte move; query/header initializer/format/commit/return modeled.','Native split/copy/byte move/query/commit/return/interrupt helpers/remainder; only header initializer and format getter modeled. Entire source object independently checked: success commits result,returns length-result and rolls write position to result; failure restores all fields.')
s=s.replace('sound_stream_work_initialize_verification','initialize_native_source_verification').replace('audio work initialization comparisons passed','native source initialization comparisons passed')
(p/'verify_initialize_native_source.py').write_text(s)
r=(p/'register_tick_native_complete.py').read_text().replace('tick_native_complete','initialize_native_source').replace('== 216','== 120')
a=r.index('names = ');z=r.index('\npath = ',a)
r=r[:a]+"names = {'sa_sound_stream_work_initialize','sa_sound_source_query','sa_sound_source_commit','sa_sound_source_return','sa_sound_source_lock_enter','sa_sound_source_lock_exit','sa_integer_remainder','sa_sound_driver_split_range'}"+r[z:]
r=r.replace('Native completion in dispatcher:','Native source methods in initialization:')
(p/'register_initialize_native_source.py').write_text(r)
