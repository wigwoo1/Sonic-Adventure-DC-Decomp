from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_sound_driver_control_native_enqueue.py').read_text()
s=s.replace("[(0x8c66b28c","[(0x8c6682be,'sound_driver_control_6682be','sa_sound_driver_control_6682be'),(0x8c667b7a,'sound_error_report','sa_sound_error_report'),(0x8c65acac,'engine_error_record','sa_engine_error_record'),(0x8c66b28c",1)
s=s.replace('channels,level,cached,bank,answer in itertools.product','channels,level,cached,bank,answer,effect in itertools.product').replace('[-43,-42,-1,0,1]','[-32768,-46,-45,-43,-42,-1,0,1,32767]').replace('[0,123]):','[0,123],[0,7,16]):')
s=s.replace('entry=0x8c6682be;handle=0x8ca08000','entry=0x8c65dd26;handle=0x8ca06000')
s=s.replace("build/sound_driver_control_6682be.dll')).sa_sound_driver_control_6682be","build/sound_subordinate_effect.dll')).sa_sound_subordinate_effect")
s=s.replace(" struct.pack_into('<I',ram,0xa08008", " ram[0xa06000:0xa06080]=bytes([0x55])*128\n struct.pack_into('<I',ram,0xa06040,0x8ca08000)\n upper_before=bytes(ram[0xa06000:0xa06080])\n struct.pack_into('<2I',ram,0x67c37c,0,0)\n struct.pack_into('<I',ram,0xa08008",1)
s=s.replace('s.r[5]=7','s.r[5]=effect')
s=s.replace(' special={"macl":0};reference(ram,s,fpu=f,special=special,hooks={a:hook(a) for a in addresses if a not in (entry,*helpers)})',' special={"macl":0};reference(ram,s,fpu=f,special=special,hooks={})')
s=s.replace('  calls.append((a,r[4],r[5],r[6]));r[0]=answer;return 0',"  raise AssertionError('Unexpected native dependency '+hex(a))")
s=s.replace(' converted=-127 if level<-42 else 127 if level>=0 else 2*level-(abs(level)//2)', ' clamped=max(-45,min(0,level));converted=-127 if clamped<-42 else 127 if clamped>=0 else 2*clamped-(abs(clamped)//2)')
s=s.replace('cached!=7','cached!=effect').replace('(setup_lower,0x8ca0b000,7,127)','(setup_lower,0x8ca0b000,effect,127)').replace('0xa08040)[0]==7','0xa08040)[0]==effect').replace('bytes([0x42,7])','bytes([0x42,effect])')
s=s.replace(' cases+=1'," wanted_upper=bytearray(upper_before);struct.pack_into('<2I',wanted_upper,0x78,effect,clamped&0xffffffff)\n assert native[0xa06000:0xa06080]==wanted_upper\n assert not calls and not events\n cases+=1")
a=s.rindex("(out/'analysis/")
scope='648 full retail RAM/CPU/FPU/SR/PR/MACL comparisons of effects setter65dd26 through native driver6682be,setup/level wrappers669470/669488,command29/2a builders66b28c/66b304,queue commit65a464 and copies. Effects0/7/16,channels0/1/3,nine levels around -45/-42/0,two prior effect caches,both banks,empty/full256-entry queue. Independent128-byte upper object/effect+78/clampedlevel+7c including error caching,native driver effect cache,exact setup-on-change and level call order/arguments,entire20-byte queue records/tags/counters,and full-queue preservation. Native error callback dispatcher (callback unset) and queue diagnostic recorder connected,no modeled calls. Invalid upper inputs separately verified. Physical audio/game caller integration/fullgame unverified.'
s=s[:a]+f"(out/'analysis/sound_subordinate_effect_native_queue_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope={scope!r}),indent=2)+'\\n')\nprint(cases,'native upper effects queue comparisons passed')\n"
(p/'verify_sound_subordinate_effect_native_queue.py').write_text(s)
reg=(p/'register_sound_subordinate_pan_native_diagnostics.py').read_text().replace('sound_subordinate_pan','sound_subordinate_effect').replace('_native_diagnostics','_native_queue').replace('==252','==648').replace(' native diagnostics:',' native queue:')
reg=reg.replace("'sa_signed_division','sa_sound_driver_pan','sa_sound_driver_pan_wrapper','sa_sound_pan_request'","'sa_sound_driver_control_6682be','sa_sound_driver_setup_wrapper','sa_sound_driver_level_wrapper','sa_sound_control_setup_request','sa_sound_control_level_request'")
(p/'register_sound_subordinate_effect_native_queue.py').write_text(reg)
