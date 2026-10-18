from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_sound_source_commit.py').read_text().replace('sound_source_commit','sound_source_return').replace('sa_sound_source_commit','sa_sound_source_return').replace('0x8c65e58a','0x8c65e644')
s=s.replace('for mode,offset,count,callback,bank in itertools.product([0,1,2],[0,4,7,60],[0,1,4,16],[0,0x8c010100],[0,1]):',"locks[0x8c10cbb8]=load('integer_remainder.dll','sa_integer_remainder')\nfor mode,offset,count,callback,bank,mismatch in itertools.product([0,1,2],[0,4,7,60],[0,1,4,16],[0,0x8c010100],[0,1],[False,True]):")
s=s.replace('0xa4000c,5,20,0,0,','0xa4000c,5,20,offset,offset,')
s=s.replace("struct.pack_into('<2I',ram,0xa20004,0x8ca10000+offset,count)","struct.pack_into('<2I',ram,0xa20004,0x8ca10000+((offset+64-count)%64+int(mismatch))%64,count)")
s=s.replace('[0x8c10d25c,0x8c010100]','[0x8c010100]')
a=s.index('    if mode==0:');z=s.index('    assert native[0xa40000:',a)
s=s[:a]+'''    if mode in [0,1] and not mismatch:
        struct.pack_into('<I',wanted,0x14 if mode==0 else 0x18,(offset+64-count)%64)
        struct.pack_into('<I',wanted,0x10 if mode==0 else 0xc,(20 if mode==0 else 5)+count)
'''+s[z:]
s=s.replace("struct.pack('<2I',0x8ca10000+offset,count)","struct.pack('<2I',0x8ca10000+((offset+64-count)%64+int(mismatch))%64,count)")
s=s.replace('    assert sr.value&~1',"    assert len(actual)==int(bool(callback and (mode==2 or mismatch)))\n    assert sr.value&~1")
a=s.index("scope='");z=s.index("'),indent=2)",a)
s=s[:a]+"scope='384 retail RAM/CPU/FPU/SR/PR/service-trace comparisons for source return65e644..65e712. Native interrupt save/restore and signed remainder; only error callback modeled. Modes0/1/invalid,positions0/4/7/60,counts0/1/4/16,optional callback,both banks,matching/mismatched returned descriptor. Independent rollback position modulo64 and counter restoration only for matching descriptors; mismatches preserve object and report error; invalid modes clear descriptor. Full64-byte object,128-byte buffer,SR mask/stack preserved. Connected virtual buffer commits and full playback/fullgame unverified."+s[z:]
s=s.replace('source commit comparisons','source return comparisons')
(p/'verify_sound_source_return.py').write_text(s)
(p/'build_sound_source_return.ps1').write_text((p/'build_sound_source_commit.ps1').read_text().replace('sound_source_commit','sound_source_return'))
r=(p/'register_sound_source_commit.py').read_text().replace('sound_source_commit','sound_source_return').replace('==192','==384').replace("address='0x8c65e58a'","address='0x8c65e644'").replace("end_exclusive='0x8c65e644'","end_exclusive='0x8c65e712'").replace('0x64e58a:0x64e644','0x64e644:0x64e712')
(p/'register_sound_source_return.py').write_text(r)
path=p/'source/sa_sound_source_return.c';s=path.read_text();a=s.index('/* Static C recovery');b=s.index('SA_STAGE_EXPORT',a)
path.write_text(s[:a]+'/* Retail source return65e644..65e712, explicit SH4 ABI.\n * Rolls back matching ring reservations; scope: sound_source_return_verification.json. */\n'+s[b:])
