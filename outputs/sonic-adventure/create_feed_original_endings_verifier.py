from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_feed_native_reservations.py').read_text().replace('import ctypes,itertools,json,struct','import ctypes,itertools,json,struct,hashlib')
a=s.index('for stop,source_capacity');z=s.index('        stack=',a)
s=s[:a]+'''inventory=json.loads((p/'analysis/magic_audio_endings_inventory.json').read_text())
assets=[a for a in inventory['assets'] if a['first_word_after_end']=='0x8001'];assert len(assets)==108
for asset,marker,bank in itertools.product(assets,[False,True],[0,1]):
        original=(ROOT/asset['path']).read_bytes();end=asset['declared_encoded_end']
        final_pair=original[end-36:end];terminal_block=original[end:end+18]
        assert hashlib.sha256(final_pair).hexdigest()==asset['final_pair_sha256']
        assert terminal_block[:2]==bytes.fromhex('8001')
        payload=terminal_block if marker else final_pair
        length=len(payload);source_capacity=length;stop=1;fmt=0;exhausted=False;capacity=8
''' +s[z:]
s=s.replace("        ram[0xa10000:0xa10002]=bytes.fromhex('8001' if marker else '1234')","        ram[0xa10000:0xa10200]=bytes([0x5a])*512\n        ram[0xa10000:0xa10000+length]=payload\n        buffer_before=bytes(ram[0xa10000:0xa10200])")
s=s.replace('        cases+=1','        assert native[0xa10000:0xa10200]==buffer_before\n        cases+=1')
a=s.index("scope='");z=s.index("'),indent=2)",a)
s=s[:a]+"scope='432 all-native feeder retail RAM/CPU/FPU/SR/PR comparisons:108 original valid audio endings, final36-byte pair or actual18-byte 0x8001 terminal block,both banks. Original bytes re-read and final pair SHA256 checked. Independent final-pair source phase1/unit2/pointer/reset counters/reservation consumption, terminal state3 and full reservation rollback, complete source/sample objects and512-byte encoded buffer unchanged where expected. No modeled function calls. Controlled stream metadata/capacities and consumed fixture: full-file history,sample decoding,production streaming/full playback/fullgame unverified; SSRACING out-of-file ending excluded."+s[z:]
s=s.replace('feed_native_reservations_verification','feed_original_endings_verification').replace('native reservation feeder comparisons passed','original ending feeder comparisons passed')
(p/'verify_feed_original_endings.py').write_text(s)
r=(p/'register_feed_native_reservations.py').read_text().replace('feed_native_reservations','feed_original_endings').replace('== 256','== 432').replace('Native reservations in source feeding:','Original audio endings in native feeding:')
(p/'register_feed_original_endings.py').write_text(r)
