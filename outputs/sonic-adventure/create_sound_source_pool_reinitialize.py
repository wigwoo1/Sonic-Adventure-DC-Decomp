from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_sound_source_pool_connected.py').read_text()
s=s.replace("(0x8c65e42c,'sound_source_destroy')", "(0x8c65e3b8,'sound_source_pool_counter_decrement')")
a=s.index('  for slot in list(range(1,64,2))');z=s.index('\n cases+=1',a)
extra='''  live_pool=bytes(native[pool:pool+3072]);before_counter=bytes(native)
  execute(0x8c65e3b8)
  expected_counter=bytearray(before_counter);struct.pack_into('<I',expected_counter,0x67c264,0)
  assert native==expected_counter and native[pool:pool+3072]==live_pool and sr.value&1==0
  execute(0x8c65e39c)
  assert struct.unpack_from('<I',native,0x67c264)[0]==1 and native[pool:pool+3072]==bytes(3072)
  wanted=bytearray(3072)
'''
s=s[:a]+extra+s[z:]
s=s.replace('sound_source_pool_connected_verification','sound_source_pool_reinitialize_verification')
a=s.index("scope='");z=s.index("'),indent=2)",a)
scope='36 persistent native source-pool reinitialization sequences:six initial byte patterns,ring sizes0/64/1024,both banks. Initialize counter0 to1;two cycles each allocate all64 objects plus exhaustion check,verify positive-counter initializer preserves entire RAM,decrement1 to0 without touching live pool,then initialize to clear entire3072-byte pool and restore counter1. Second cycle reallocates cleared slots with exact original bindings and pointers. 4932 public native operations,each compared against retail full RAM/CPU/FPU/SR/PR,with independent all12 object fields,wholepool contents,counter transition,SR.T,guards,stack and preservation checks. Native source allocator/reset,bytefill,counterdecrement and initializer connected,no modeled calls. Reinitialization after decrement invalidates all old objects; this verifies raw retail behavior,not authorization or lifetime handling at game call sites. Game caller synchronization/fullgame remain unverified.'
s=s[:a]+'scope='+repr(scope)+s[z+1:]
(p/'verify_sound_source_pool_reinitialize.py').write_text(s)
reg=(p/'register_sound_source_pool_connected.py').read_text().replace('sound_source_pool_connected','sound_source_pool_reinitialize').replace('9396','4932').replace("'sa_sound_source_destroy'","'sa_sound_source_pool_counter_decrement'").replace('Persistent native source-pool lifecycles:', 'Native source-pool counter decrement and reinitialization:')
(p/'register_sound_source_pool_reinitialize.py').write_text(reg)
