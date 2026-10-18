from pathlib import Path
p=Path(__file__).resolve().parent
name='sound_stream_work_feed'
(p/f'build_{name}.ps1').write_text((p/'build_sound_source_buffer_get.ps1').read_text().replace('sound_source_buffer_get',name))
path=p/f'source/sa_{name}.c';s=path.read_text();a=s.index('/* Static C recovery');z=s.index('SA_STAGE_EXPORT',a);path.write_text(s[:a]+'/* Retail stream source feeding 8c65b930..8c65ba22; subordinate services use explicit SH4 ABI. */\n'+s[z:])
r=(p/'register_sound_source_buffer_get.py').read_text().replace('sound_source_buffer_get',name).replace('0x8c65e798','0x8c65b930').replace('0x8c65e79c','0x8c65ba22').replace('0x64e798','0x64b930').replace('0x64e79c','0x64ba22').replace('==140','==256')
(p/f'register_{name}.py').write_text(r)
s=(p/'verify_sound_stream_work_tick.py').read_text().replace('sound_stream_work_tick','sound_stream_work_feed').replace('0x8c65bb4e','0x8c65b930')
a=s.index('for initial,after,bank');z=s.index('        native=bytearray',a)
s=s[:a]+'''for stop,source_capacity,fmt,length,marker,exhausted,capacity,bank in itertools.product([0,1],[0,8],[0,1],[1,36],[False,True],[False,True],[6,8],[0,1]):
        stack=0x8cf00000
        ram=bytearray(16*1024*1024);ram[0x10000:0x10000+len(binary)]=binary
        ram[0xa60000:0xa60100]=bytes([0x55])*256;ram[0xa60003]=stop
        struct.pack_into('<3I',ram,0xa60004,0x8ca70000,0x8ca80000,0x8ca80100)
        struct.pack_into('<I',ram,0xa60034,100 if exhausted else 0)
        for address in [0xa80000,0xa80100]:struct.pack_into('<I',ram,address,0x8ca90000)
        for offset,target in [(0x18,0x8c010100),(0x24,0x8c010200),(0x1c,0x8c010300)]:struct.pack_into('<I',ram,0xa90000+offset,target)
        ram[0xa10000:0xa10002]=bytes.fromhex('8001' if marker else '1234')
''' +s[z:]
a=s.index('        def service(');z=s.index('        def hook(',a)
s=s[:a]+'''        def service(address,mem,r,trace):
            trace.append((address,tuple(r[i] for i in range(4,8))))
            if address==0x8c010200:
                assert r[4] in [0x8ca80000,0x8ca80100]
                r[0]=source_capacity if r[4]==0x8ca80000 else capacity
            elif address==0x8c010100:
                assert (r[4],r[5],r[6],r[7])==(0x8ca80000,1,0x7fffffff,0x8ca60014)
                for i,v in enumerate(struct.pack('<2I',0x8ca10000,length)):mem[0xa60014+i]=v
            elif address==0x8c6676a4:r[0]=fmt
            elif address==0x8c65bc84:r[0]=100
            elif address==0x8c65bc6c:r[0]=4
            elif address==0x8c010300:assert (r[4],r[5],r[6])==(0x8ca80000,1,0x8ca60014)
            elif address==0x8c66771c:assert (r[4],r[5],r[6])==(0x8ca70000,0x8ca10000,length)
            else:assert address==0x8c66776e and r[4]==0x8ca70000
''' +s[z:]
s=s.replace('[0x8c667718,0x8c667ad4,0x8c65b930,0x8c65ba22]','[0x8c010100,0x8c010200,0x8c010300,0x8c6676a4,0x8c65bc84,0x8c65bc6c,0x8c66771c,0x8c66776e]')
a=s.index('        assert [a for a,');z=s.index('        cases+=1',a)
s=s[:a]+'''        early=stop==1 and source_capacity==0
        terminal=fmt==0 and length>=2 and marker
        ended=early or terminal or exhausted
        addresses=([0x8c010200] if stop else [])
        if not early:
            addresses += [0x8c010100,0x8c6676a4]
            if not terminal:addresses += [0x8c65bc84]
            if not terminal and not exhausted:addresses += [0x8c65bc6c,0x8c010200]
            addresses += [0x8c010300] if terminal or exhausted or capacity//2<4 else [0x8c66771c,0x8c66776e]
        assert [a for a,args in actual]==addresses
        assert native[0xa60001]==(3 if ended else 0x55) and regs[15]==stack
''' +s[z:]
a=s.index("scope='");z=s.index("'),indent=2)",a)
s=s[:a]+"scope='256 full retail RAM/CPU/FPU/SR/PR and ordered call comparisons of source feeder65b930..65ba22. Stop flag/source capacity,format,descriptor length,original 0x8001 marker,total exhaustion,sample capacity,banks. Independent state3 termination, reservation return versus source/start call decisions and restored stack assertions. All subordinate methods modeled; native feeding integration/full playback/fullgame unverified."+s[z:]
(p/'verify_sound_stream_work_feed.py').write_text(s)
