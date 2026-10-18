from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_magic_repeated_updates.py').read_text().replace('for chunk in range(2):','for chunk in range(3):')
s=s.replace('        terminal=bool(payload[0]&128)','        count=len(payload)//18\n        terminal=bool(payload and payload[0]&128)')
s=s.replace('source_set,0x8ca10000+chunk*288,288','source_set,0x8ca10000+chunk*288,len(payload)')
s=s.replace('==(0x8ca10120,16)','==(0x8ca10000+chunk*288,len(payload)//18)').replace('==(256,0)','==(chunk*256,0)')
s=s.replace('        s.pc=0x8c667ad4',"        previous_buffers=[bytes(native[a:a+768]) for a in [0xa20000,0xa40000]]\n        previous_histories=bytes(native[0xa60028:0xa60030])\n        s.pc=0x8c667ad4")
a=s.index('        assert native[0xa20000:0xa20008]');z=s.index('        assert actual==expected==[]',a)
body=s[a:z]
s=s[:a]+'        if count:\n'+''.join('    '+line+'\n' for line in body.splitlines())+'''        else:
            assert [bytes(native[a:a+768]) for a in [0xa20000,0xa40000]]==previous_buffers
            assert bytes(native[0xa60028:0xa60030])==previous_histories
            assert native[0xa10000:0xa10000+len(fullpayload)]==fullpayload
'''+s[z:]
s=s.replace("wanted_stream,0xa0,256,288)","wanted_stream,0xa0,count//2*32,len(payload))").replace('(v+256)&0xffffffff for v in initial_counters','(v+count//2*32)&0xffffffff for v in initial_counters')
s=s.replace('magic_repeated_updates_verification','magic_empty_tail_verification').replace('436 retail','654 retail').replace('Completion consumed counter256 then512.','Completion consumed counter256 then512,followed by an empty source/zero-block request. Independently checked zero completed samples/bytes,unchanged counters512/0,entire decoded buffers and predictor histories preserved. Empty source is a controlled test boundary after the576-byte prefix,not the original file EOF.').replace('repeated native original-audio update','native original-audio empty-tail update')
(p/'verify_magic_empty_tail.py').write_text(s)
r=(p/'register_magic_repeated_updates.py').read_text().replace('magic_repeated_updates','magic_empty_tail').replace('==436','==654').replace('Repeated native original-audio updates:','Native original-audio empty-tail updates:')
(p/'register_magic_empty_tail.py').write_text(r)
