from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_input_provider_native_buffer.py').read_text()
s=s.replace('services=[0x8c010100];cases=0',"query=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_source_query.dll')).sa_sound_source_query;query.argtypes=fn.argtypes\nservices=[0x8c65e368,0x8c65e380,0x8c10cbb8];cases=0")
s=s.replace('struct.pack_into(\'<I\',ram,0xa4001c,0x8ca10000);struct.pack_into(\'<I\',ram,0xa50018,0x8c010100);',"\n    for index,address in enumerate([0xa40000,0xa40100]):\n        struct.pack_into('<8I',ram,address+0x0c,0,capacity+index*16,40+index*8,0,0x8ca10000,512,0,0)\n    objects_before=[bytes(ram[a:a+48]) for a in [0xa40000,0xa40100]]\n    struct.pack_into('<I',ram,0xa50018,0x8c65e4b8);")
start=s.index('        if address==0x8c6676ae:')
end=s.index('    def hook(address):',start)
s=s[:start]+'''        if address==0x8c65e368:
            assert r[4] in [0x8ca40000,0x8ca40100];r[0]=0x13579bdf
        elif address==0x8c65e380:
            assert r[4] in [0x8ca40000,0x8ca40100] and r[5]==0x13579bdf;r[0]=0x2468ace0
        else:
            assert address==0x8c10cbb8 and r[0]==512;r[0]=r[1]%512
'''+s[end:]
s=s.replace('0x8c65e798:buffer_get}', '0x8c65e798:buffer_get,0x8c65e4b8:query}')
s=s.replace("    assert [a for a,args in actual]==[0x8c010100]*channels",'''    assert [a for a,args in actual]==[a for index in range(channels) for a in [0x8c65e368,0x8c10cbb8,0x8c65e380]]
    for index,address in enumerate([0xa40000,0xa40100]):
        wanted_object=bytearray(objects_before[index])
        if index<channels:
            struct.pack_into('<I',wanted_object,0x10,0)
            struct.pack_into('<I',wanted_object,0x14,(40+index*8+capacity+index*16)%512)
        assert native[address:address+48]==wanted_object''')
s=s.replace('input_provider_native_buffer_verification','input_provider_native_query_verification').replace('only virtual channel queries modeled.','native virtual source query65e4b8; only lock enter/exit and signed remainder modeled.').replace('Remaining virtual query integration,','Native lock/remainder integration,').replace('native-buffer comparisons','native-query comparisons')
(p/'verify_input_provider_native_query.py').write_text(s)
r=(p/'register_input_provider_native_buffer.py').read_text().replace('input_provider_native_buffer','input_provider_native_query').replace("'sa_sound_source_buffer_get'}","'sa_sound_source_buffer_get','sa_sound_source_query'}").replace('Input provider native buffer integration:','Input provider native source query integration:').replace('Native file decoder flag integration','Native input provider source query integration')
(p/'register_input_provider_native_query.py').write_text(r)
