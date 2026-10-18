from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_tick_native_provider.py').read_text()
s=s.replace("assets=json.loads((out/'analysis/magic_audio_header_inventory.json').read_text())['assets'];records=[];cases=0","assets=[a for a in json.loads((out/'analysis/magic_audio_endings_inventory.json').read_text())['assets'] if a['first_word_after_end']=='0x8001'];assert len(assets)==108\nrecords=[];cases=0")
s=s.replace("        file.seek(offset);payload=file.read(288)","        offset=asset['declared_encoded_end']-36;file.seek(offset);payload=file.read(36)")
s=s.replace("    assert len(payload)==288 and header.hex()==asset['header_hex'] and header[7]==2","    assert len(payload)==36 and hashlib.sha256(payload).hexdigest()==asset['final_pair_sha256'] and header[7]==2\n    valid=asset['final_pair_valid_samples'];assert valid==(int.from_bytes(header[12:16],'big')%32 or 32)")
s=s.replace('count=16;','count=2;')
s=s.replace('(0x68,256),(0x74,256),(0x78,256)','(0x68,32),(0x74,32),(0x78,valid)').replace('ram,0xa70040,256','ram,0xa70040,32').replace('ram,0xa70018,256','ram,0xa70018,valid').replace('ram,0xa70010,16','ram,0xa70010,2').replace('ram,0xa90038,256,256,0','ram,0xa90038,32,valid,0').replace('288 if index==0 else 0','36 if index==0 else 0')
s=s.replace('wanted_stream,0xa0,256,288','wanted_stream,0xa0,valid,36').replace('(v+256)','(v+valid)').replace('wanted_stream,0x50,0x8ca10000,16','wanted_stream,0x50,0x8ca10000,2')
s=s.replace('[(0x2c,256),(0x30,288),(0x34,256),(0x40,256),(0x44,288)]','[(0x2c,valid),(0x30,36),(0x34,valid),(0x40,valid),(0x44,36)]').replace('[(0xc,0),(0x10,288),(0x18,288)]','[(0xc,0),(0x10,36),(0x18,36)]').replace('[(0xc,512),(0x10,88),(0x14,512)]','[(0xc,valid*2),(0x10,600-valid*2),(0x14,valid*2)]')
a=s.index("scope='");z=s.index("'),indent=2)",a)
s=s[:a]+"scope='216 full retail RAM/CPU/FPU/SR/PR/FPUL/MACL comparisons through native idle feeder, input provider,decoder,work completion and acknowledgment on final36-byte pairs from108 original assets,banks. Every decoded32 samples per channel and final history independently checked with binary32 recurrence from controlled zero initial histories and original-header coefficients. Independent valid samples from original header modulo32; only valid*2 bytes committed per channel,600-valid*2 reserved bytes returned,exact work counters,encoded36 committed,stream idle/cursor reset and guards. No modeled calls. Stream total/remaining fixture equals final valid count, not entire asset; preceding full-file history,physical playback/fullgame unverified. SSRACING invalid header end excluded."+s[z:]
s=s.replace('tick_native_provider_verification','tick_final_pair_verification').replace('native provider dispatcher lifecycle comparisons passed','original final pair native lifecycle comparisons passed')
(p/'verify_tick_final_pair.py').write_text(s)
r=(p/'register_tick_native_provider.py').read_text().replace('tick_native_provider','tick_final_pair').replace('==218','==216').replace('Native provider feeding/decoding/completion dispatcher:','Original final pairs in native dispatcher:')
(p/'register_tick_final_pair.py').write_text(r)
