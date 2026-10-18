from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_work_complete_native_ack.py').read_text()
needle='services=[0x8c6676f6'
s=s.replace(needle,"for address,stem,name in [(0x8c65e58a,'sound_source_commit','sa_sound_source_commit'),(0x8c65e644,'sound_source_return','sa_sound_source_return'),(0x8c65e368,'sound_source_lock_enter','sa_sound_source_lock_enter'),(0x8c65e380,'sound_source_lock_exit','sa_sound_source_lock_exit'),(0x8c10cbb8,'integer_remainder','sa_integer_remainder'),(0x8c10d25c,'byte_move','sa_byte_move')]:\n    helpers[address]=getattr(ctypes.CDLL(str(ROOT/'work/sonic-adventure/build'/(stem+'.dll'))),name);helpers[address].argtypes=fn.argtypes\n"+needle)
s=s.replace('0x8c6677d8,0x8c010100,0x8c010200,0x8c010300','0x8c6677d8,0x8c010300')
s=s.replace('[0,5,32,100],[0,4,40]','[5,32,100],[0,4]')
s=s.replace("    struct.pack_into('<2I',ram,0xa9001c,0x8c010100,0x8c010200)",'''    struct.pack_into('<2I',ram,0xa9001c,0x8c65e644,0x8c65e58a)
    objects_before=[]
    for index,address in enumerate([0xa80000,0xa80100,0xa80200]):
        base=0x8ca10000+index*0x10000
        struct.pack_into('<11I',ram,address+4,1,0,0,0,0 if index==0 else 100,64 if index==0 else 0,base,256,0,0,0)
        objects_before.append(bytes(ram[address:address+48]))
        ram[base&0xffffff:(base&0xffffff)+512]=bytes([0x5a])*512''')
old="[0x8c6676f6,0x8c6677b6,0x8c6677d8,0x8c010200,0x8c010100]+[a for index in range(channels) for a in [0x8c010200,0x8c010100]]+([0x8c010300] if notify else [])"
s=s.replace(old,"[0x8c6676f6,0x8c6677b6,0x8c6677d8]+([0x8c010300] if notify else [])")
s=s.replace('    cases+=1', '''    for index,address in enumerate([0xa80000,0xa80100,0xa80200]):
        wanted_object=bytearray(objects_before[index])
        if index==0:
            struct.pack_into('<I',wanted_object,0x10,36)
            struct.pack_into('<I',wanted_object,0xc,28)
            struct.pack_into('<I',wanted_object,0x18,36)
        elif index<=channels:
            published=chosen*2
            struct.pack_into('<I',wanted_object,0xc,published)
            struct.pack_into('<I',wanted_object,0x10,100-published)
            struct.pack_into('<I',wanted_object,0x14,published)
        assert native[address:address+48]==wanted_object
        base=(0x8ca10000+index*0x10000)&0xffffff
        assert native[base:base+512]==bytes([0x5a])*512
    cases+=1''')
a=s.index("scope='");z=s.index("'),indent=2)",a)
s=s[:a]+"scope='216 retail RAM/CPU/FPU/SR/PR/service-trace completion comparisons65ba22..65bb4e with native range splitting/copying,source commit/return,interrupt save/restore,remainder,byte move,channel getter and stream/file acknowledgment. Total/byte/sample getters and notification callback modeled. Independent min(decoded32,total-consumed),complete144-byte work counters,256-byte stream and64-byte decoder states; encoded object consumes36 of64 reserved bytes and returns28; each active channel publishes twice trimmed sample count of100 reserved bytes and returns remainder. Entire three source objects and512-byte buffers checked,ring rollback positions asserted. Totals5/32/100,consumed0/4,channels0/1/2,limits-1/0/50,callback presence,both banks. Negative remaining already separately tested; current valid reservations avoid negative copy counts. Actual asset final trimming/full playback/fullgame unverified."+s[z:]
s=s.replace('work_complete_native_ack_verification','work_complete_native_commits_verification').replace('native ack work completion comparisons','native commits work completion comparisons')
(p/'verify_work_complete_native_commits.py').write_text(s)
r=(p/'register_work_complete_native_ack.py').read_text().replace('work_complete_native_ack','work_complete_native_commits').replace('== 432','== 216').replace("'sa_file_decode_completion_acknowledge'}","'sa_file_decode_completion_acknowledge','sa_sound_source_commit','sa_sound_source_return'}").replace('Native acknowledgment in stream work completion:','Native source commits in stream work completion:')
(p/'register_work_complete_native_commits.py').write_text(r)
