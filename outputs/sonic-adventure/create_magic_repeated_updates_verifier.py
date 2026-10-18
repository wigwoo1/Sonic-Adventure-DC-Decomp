from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_magic_lifecycle_native_source.py').read_text()
s=s.replace("source_set=extra('sound_stream_source_set.dll','sa_sound_stream_source_set')","source_set=extra('sound_stream_source_set.dll','sa_sound_stream_source_set')\nstream_ack=extra('sound_stream_decode_acknowledge.dll','sa_sound_stream_decode_acknowledge')")
s=s.replace('payload=file.read(288)','payload=file.read(576)\n    assert len(payload)==576\n    fullpayload=payload;payload=fullpayload[:288]')
s=s.replace('ram[0xa10000:0xa10000+len(payload)]=payload','ram[0xa10000:0xa10000+len(fullpayload)]=fullpayload')
s=s.replace('ram,0xa70018,256','ram,0xa70018,512').replace('(0x74,256),(0x78,256)','(0x74,512),(0x78,512)')
a=s.index('    stream_before=bytes(native[0xa70000:0xa70100])');z=s.index("(out/'analysis/",a)
body=s[a:z]
body=body.replace("native[0xa10000:0xa10000+len(payload)]==payload","native[0xa10000:0xa10000+len(fullpayload)]==fullpayload")
body=body.replace('(0,history,0xa20008,0xa60028),(1,(0,0),0xa40008,0xa6002c)','(0,oracle_histories[0],0xa20008,0xa60028),(1,oracle_histories[1],0xa40008,0xa6002c)')
body=body.replace('            assert native[histaddr:histaddr+4]==expected_history','            assert native[histaddr:histaddr+4]==expected_history\n            oracle_histories[channel]=struct.unpack(\'<2h\',expected_history)')
body=body.replace("    assert native[0xa70000:0xa70100]==wanted_stream","    struct.pack_into('<2I',wanted_stream,0x74,256,512-chunk*256)\n    assert native[0xa70000:0xa70100]==wanted_stream")
body=body.replace("bank=bank,coefficients=coefficients","bank=bank,chunk=chunk,coefficients=coefficients")
prefix='''    oracle_histories={0:(0,0),1:(0,0)}
    for chunk in range(2):
        payload=fullpayload[chunk*288:(chunk+1)*288]
        terminal=bool(payload[0]&128)
        if chunk:
            prior_stream=bytes(native[0xa70000:0xa70100]);prior_handle=bytes(native[0xa60000:0xa60040])
            for entry,target,arg1,arg2 in [(0x8c667796,stream_ack,0,0),(0x8c66771c,source_set,0x8ca10000+chunk*288,288),(0x8c66776e,start_stream,0,0)]:
                s.r[4:7]=[0x8ca70000,arg1,arg2]
                for index in range(4,7):regs[index]=s.r[index]
                s.pc=entry
                reference(ram,s,{},fpu=f,special=special)
                assert target(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
                nf.fpul=fpul.value
                assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
                if entry==0x8c667796:
                    wanted=bytearray(prior_stream);struct.pack_into('<I',wanted,4,0);struct.pack_into('<I',wanted,0x9c,0)
                    assert native[0xa70000:0xa70100]==wanted and native[0xa60000:0xa60040]==prior_handle
            assert struct.unpack_from('<2I',native,0xa70050)==(0x8ca10120,16)
            assert struct.unpack_from('<2I',native,0xa70098)==(256,0)
'''
s=s[:a]+prefix+''.join('    '+line+'\n' for line in body.splitlines())+s[z:]
s=s.replace('magic_lifecycle_native_source_verification','magic_repeated_updates_verification').replace('218 retail','436 retail').replace('first288-byte encoded prefixes','two successive288-byte encoded chunks from first576-byte prefixes').replace('Controlled stream capacity256,position0,total256,zero initial histories; native constructor/header initialization integration,repeated updates,fullpayload,physical playback/fullgame unverified.','Controlled stream capacity256,total512. Persistent RAM/register/FPU state; native stream acknowledgment clearsphase/+9c without clearing histories; source replacement and restart native. Independent predictor histories retained between chunks and each decoded sample checked. Completion consumed counter256 then512. Native constructor/header initialization integration,fullpayload,physical playback/fullgame unverified.').replace('native-source original-audio lifecycle','repeated native original-audio update')
(p/'verify_magic_repeated_updates.py').write_text(s)
r=(p/'register_magic_lifecycle_native_source.py').read_text().replace('magic_lifecycle_native_source','magic_repeated_updates').replace('==218','==436').replace("'sa_sound_stream_source_set'}","'sa_sound_stream_source_set','sa_sound_stream_decode_acknowledge'}").replace('Native source setup original-audio lifecycle:','Repeated native original-audio updates:')
(p/'register_magic_repeated_updates.py').write_text(r)
