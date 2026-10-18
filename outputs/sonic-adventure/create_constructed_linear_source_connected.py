from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_sound_linear_source_connected.py').read_text()
s=s.replace("specs=[","specs=[(0x8c65e0c0,'sound_linear_source_pool_initialize'),(0x8c65e0ea,'sound_linear_source_create'),(0x8c65e150,'sound_linear_source_destroy'),(0x8c10d31c,'byte_fill'),")
s=s.replace('obj=0x8ca40000','pool=0x8a6138;obj=0x8c8a6138')
a=s.index(' ram[0xa40000:');z=s.index(' s=State()',a)
s=s[:a]+''' ram[pool-8:pool+1160]=bytes([0x55])*1168;ram[0xa20000:0xa20010]=bytes([0x66])*16
 ram[0xa10000:0xa10040]=bytes([0x77])*64
 struct.pack_into('<I',ram,0x67c22c,0)
 wanted=bytearray(36);wanted_pool=bytearray(1152)
 native=bytearray(ram);view=(ctypes.c_uint8*len(native)).from_buffer(native)
'''+s[z:]
s=s.replace('  assert native[0xa40000:0xa40040]==wanted',"  wanted_pool[:36]=wanted\n  assert native[pool:pool+1152]==wanted_pool\n  assert native[pool-8:pool]==bytes([0x55])*8 and native[pool+1152:pool+1160]==bytes([0x55])*8\n  assert native[0xa10000:0xa10040]==bytes([0x77])*64\n  assert struct.unpack_from('<I',native,0x67c22c)[0]==1\n  assert True")
marker=" struct.pack_into('<2I',wanted,0xc,length,0);execute(0x8c65e178,[obj])"
setup=""" execute(0x8c65e0c0,[])
 values=[0x8c67c1fc,1,0x8c672424,length,0,base,length,0x8c65e0bc,obj]
 struct.pack_into('<9I',wanted,0,*values);execute(0x8c65e0ea,[base,length]);assert r[0]==obj
"""
s=s.replace(marker,setup+marker,1)
s=s.replace(' cases+=1'," wanted[:]=bytes(36);execute(0x8c65e150,[obj])\n struct.pack_into('<9I',wanted,0,*values);execute(0x8c65e0ea,[base,length]);assert r[0]==obj\n execute(0x8c65e182,[obj,1]);assert r[0]==length\n cases+=1")
a=s.index("scope='");z=s.index('\n(p/',a)
scope='50 persistent natively constructed linear-source sequences;five nonnegative lengths and requests,both FPU banks. Guarded1152-byte pool initialized natively,counter0 to1;native constructor selects first slot,then reset/capacity/contiguous check/query/commit/return/locks reserve,commit,return unused tail,check remaining,reserve all remaining,commit,verify exhausted,reset and restore capacity. Native destruction clears36-byte object;constructor reuses same slot,capacity restored. Caller adjusts unused-tail descriptor in both memories. No RAM/CPU/FPU resets between calls. Full retail RAM/CPU/FPU/SR/PR comparison after every public call;independent entire pool/all9 object words,8-byte pool guards,descriptor fields/guards,buffer bytes,counter,stack and interrupt mask. Native byte fill connected,no modeled calls. Default callback not executed;game caller integration and playback remain unverified.'
s=s[:a]+'scope='+repr(scope)+s[z:];s=s.replace('sound_linear_source_connected_verification.json','constructed_linear_source_connected_verification.json')
(p/'verify_constructed_linear_source_connected.py').write_text(s)
reg=(p/'register_sound_linear_source_connected.py').read_text().replace('sound_linear_source_connected','constructed_linear_source_connected').replace('==600','==850')
reg=reg.replace("['lock_enter'","['pool_initialize','create','destroy','lock_enter'").replace('Persistent linear-source reservation sequences:','Persistent native constructed linear-source sequences:')
(p/'register_constructed_linear_source_connected.py').write_text(reg)
