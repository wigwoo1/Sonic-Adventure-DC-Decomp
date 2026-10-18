import subprocess,sys
from pathlib import Path
p=Path(__file__).resolve().parent;stem='sound_linear_source_pool_counter_decrement';start=0x8c65e0dc;end=0x8c65e0ea
subprocess.run([sys.executable,str(p/'ghidra/translate_tikal_helper.py'),'sa_'+stem,f'{start:x}',f'{end:x}','sound_linear_source_constructor_65e0dc.tsv'],check=True)
source=p/'source'/('sa_'+stem+'.c');s=source.read_text();a=s.index('/* Static C recovery');z=s.index(' */',a)+3;source.write_text(s[:a]+'/* Retail linear-source pool counter decrement. T reports signed-positive result; pool unchanged. */'+s[z:])
(p/f'build_{stem}.ps1').write_text((p/'build_sound_source_buffer_get.ps1').read_text().replace('sound_source_buffer_get',stem))
s=(p/'verify_sound_source_pool_counter_decrement.py').read_text()
for old,new in [('sound_source_pool_counter_decrement',stem),('0x8c65e3b8','0x8c65e0dc'),('0x67c264','0x67c22c'),('0x8c67c264','0x8c67c22c'),('0x8a65b8','0x8a6138'),('0x8a71b8','0x8a65b8'),('3072','1152'),('65e3b8..65e3c6','65e0dc..65e0ea'),('global67c264','global67c22c')]:s=s.replace(old,new)
(p/f'verify_{stem}.py').write_text(s)
reg=(p/'register_sound_linear_source_query.py').read_text();a=reg.index('for stem,start,end in [');z=reg.index('\n',a);reg=reg[:a]+'for stem,start,end in '+repr([(stem,start,end)])+':'+reg[z:];reg=reg.replace('==800','==84');(p/f'register_{stem}.py').write_text(reg)
s=(p/'verify_sound_linear_source_pool_connected.py').read_text().replace("(0x8c65e150,'sound_linear_source_destroy')","(0x8c65e0dc,'sound_linear_source_pool_counter_decrement')")
a=s.index('  for slot in list(range(1,32,2))');z=s.index('\n cases+=1',a)
s=s[:a]+'''  live_pool=bytes(native[pool:pool+1152]);before_counter=bytes(native)
  execute(0x8c65e0dc)
  expected_counter=bytearray(before_counter);struct.pack_into('<I',expected_counter,0x67c22c,0)
  assert native==expected_counter and native[pool:pool+1152]==live_pool and sr.value&1==0
  execute(0x8c65e0c0)
  assert struct.unpack_from('<I',native,0x67c22c)[0]==1 and native[pool:pool+1152]==bytes(1152)
  wanted=bytearray(1152)
'''+s[z:]
a=s.index("(p/'analysis/")
scope='36 persistent native linear-source pool reinitialization sequences:six byte patterns,lengths0/64/1024,both banks. Initialize counter0 to1;two cycles each allocate all32 objects plus exhaustion,verify positive-counter initializer preserves entire RAM,decrement1 to0 without touching live objects,then reinitialize to clear entire1152-byte pool and restore counter1. Reallocate cleared slots with exact original pointers and bindings. Full retail RAM/CPU/FPU/SR/PR after each public call;independent all9 object words,wholepool,counter,SR.T,guards,stack and preservation. Native reset/bytefill,no modeled calls. Reinitialization invalidates old objects;game caller lifetime/synchronization/fullgame unverified.'
s=s[:a]+f"(p/'analysis/sound_linear_source_pool_reinitialize_verification.json').write_text(json.dumps(dict(cases_passed=cases,native_operations_compared=operations,scope={scope!r}),indent=2)+'\\n')\nprint(cases,'linear pool reinitializations;',operations,'operations passed')\n"
(p/'verify_sound_linear_source_pool_reinitialize.py').write_text(s)
reg=(p/'register_sound_linear_source_pool_connected.py').read_text().replace('sound_linear_source_pool_connected','sound_linear_source_pool_reinitialize').replace('4788','2628').replace("'sa_sound_linear_source_destroy'","'sa_sound_linear_source_pool_counter_decrement'").replace('Persistent native linear source-pool lifecycles:','Native linear pool counter and reinitialization:')
(p/'register_sound_linear_source_pool_reinitialize.py').write_text(reg)
