from pathlib import Path
import re
p=Path(__file__).resolve().parent
s=(p/'verify_pool_audio_final_pair.py').read_text()
s=s.replace('for asset,bank in itertools.product(assets,[0,1]):','assets=assets[:1]\nfor asset,bank,slot in itertools.product(assets,[0,1],range(8)):\n    work_addr=0x8a3910+slot*0x90;work_pointer=0x8c000000|work_addr')
def replace(match):
    value=int(match[0],16)
    if 0x8a3910<=value<0x8a39a0:return '(work_addr+'+hex(value-0x8a3910)+')'
    if 0x8c8a3910<=value<0x8c8a39a0:return '(work_pointer+'+hex(value-0x8c8a3910)+')'
    return match[0]
# Only replace fixture/verification address uses after loop setup, preserving actual pool bounds.
at=s.index('    path=ROOT/asset');s=s[:at]+re.sub(r'0x[0-9a-f]+',replace,s[at:])
s=s.replace('ram[(work_addr+0x0):0x8a3d90]=bytes(0x480)','ram[0x8a3910:0x8a3d90]=bytes(0x480)')
s=s.replace('    assert native[0x8a39a0:0x8a3d90]==bytes(0x3f0)',"    for other in range(8):\n        if other!=slot:\n            address=0x8a3910+other*0x90\n            assert native[address:address+0x90]==bytes(0x90)")
s=s.replace("records.append(dict(path=asset['path'],","records.append(dict(slot=slot,path=asset['path'],")
a=s.index("scope='");z=s.index("'),indent=2)",a)
s=s[:a]+"scope='16 full retail RAM/CPU/FPU/SR/PR/FPUL/MACL comparisons using original ADVAMY final pair in each of8 original work pool slots,both banks. Native scheduler/public update/feed/input provider/decoder/complete/ack, no models. Independent exact32 samples/channel and histories from zero initial fixture, valid13 samples committed per header,total/source/work counters,returned reservations,sole public call at selected slot and all seven inactive90-byte slots unchanged. Stream fixtures represent final pair only; preceding history,full-file streaming/playback/fullgame unverified."+s[z:]
s=s.replace('pool_audio_final_pair_verification','pool_audio_slot_verification').replace('pool audio final pair comparisons passed','all-slot native audio comparisons passed')
(p/'verify_pool_audio_slot.py').write_text(s)
r=(p/'register_pool_audio_final_pair.py').read_text().replace('pool_audio_final_pair','pool_audio_slot').replace('==216','==16').replace('Original final pairs through native pool update:','Every work slot in native pool decoding:')
(p/'register_pool_audio_slot.py').write_text(r)
