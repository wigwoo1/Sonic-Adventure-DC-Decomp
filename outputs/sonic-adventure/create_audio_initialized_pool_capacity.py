from pathlib import Path
p=Path(__file__).resolve().parent
text=(p/'verify_audio_initialized_allocation.py').read_text()
start=text.index(' s.r[4:7]=[0x8ca80000,channels,0x8ca10000]\n',text.index(' before=bytes(1152)'))
end=text.index('\n cases+=1',start)
block=text[start:end]
block=block.replace('wanted_stream[:172]=obj','wanted_stream[slot*172:(slot+1)*172]=obj')
block=block.replace("struct.pack_into('<I',obj,8,handle_pool)","struct.pack_into('<I',obj,8,handle_pool+slot*52)")
block=block.replace("struct.pack_into('<2I',obj,0,1,0)","struct.pack_into('<2I',obj,0,1,slot)")
block=block.replace('obj[48:52]=native[hs+48:hs+52]','obj[48:52]=native[hs+slot*52+48:hs+slot*52+52]')
block=block.replace('wanted_handles[:52]=obj','wanted_handles[slot*52:(slot+1)*52]=obj')
setup=""" for slot in range(9):
  selected=0x8c000000|(pool+slot*144)
  created=0x8c000000|(stream_pool+slot*172)
  before=bytes(native[pool:pool+1152]);stream_before=bytes(native[stream_pool:stream_pool+1376]);handles_before=bytes(native[hs:hs+416])
"""
text=text[:start]+setup+'\n'.join(' '+line for line in block.splitlines())+text[end:]
text=text.replace('audio_initialized_allocation_verification.json','audio_initialized_pool_capacity_verification.json')
begin=text.index("scope='36 persistent")
finish=text.index("'),indent=2)",begin)
scope='36 persistent native initialization-to-full-pool sequences with 324 constructor calls: six prior byte patterns, channels 0/1/2, both FPU banks. Eight consecutive successful allocations independently checked across complete work/stream/handle pools, slot indices, bindings, callbacks, source preservation and returned pointers; ninth work allocation returns null and preserves pools. Full retail RAM/CPU/FPU/SR/PR comparison at initialization and each allocation; all subordinate helpers native with no modeled calls. Coefficients checked against retail and independently by dedicated coefficient verification. Unequal subordinate pool exhaustion, allocation failure callback behavior, full playback and full game remain unverified.'
text=text[:begin]+'allocation_calls=cases*9,scope='+repr(scope)+text[finish+1:]
text=text.replace("print(cases,'sound work constructor comparisons passed')","print(cases, 'persistent pool-capacity sequences passed;', cases*9, 'constructor calls')")
(p/'verify_audio_initialized_pool_capacity.py').write_text(text)
reg=(p/'register_audio_initialized_allocation.py').read_text().replace('audio_initialized_allocation','audio_initialized_pool_capacity').replace('Native audio pool initialization followed by allocation:','Native audio initialization and full pool capacity:')
(p/'register_audio_initialized_pool_capacity.py').write_text(reg)
