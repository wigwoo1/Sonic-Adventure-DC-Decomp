from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_sound_driver_pan_native_enqueue.py').read_text()
s=s.replace("[(0x8c10ca5c,'sound_volume_dispatch'","[(0x8c66837c,'sound_driver_pan','sa_sound_driver_pan'),(0x8c667b7a,'sound_error_report','sa_sound_error_report'),(0x8c10ca5c,'signed_division'")
s=s.replace('[-16,-15,-1,0,1,15,16]','[-32768,-16,-15,-1,0,1,15,16,32767]')
s=s.replace('entry=0x8c66837c','entry=0x8c65dcca').replace("build/sound_driver_pan.dll')).sa_sound_driver_pan","build/sound_subordinate_pan.dll')).sa_sound_subordinate_pan")
s=s.replace(" struct.pack_into('<I',ram,0xa08028", " ram[0xa06000:0xa06080]=bytes([0x55])*128\n struct.pack_into('<I',ram,0xa06040,0x8ca08000)\n upper_before=bytes(ram[0xa06000:0xa06080])\n struct.pack_into('<2I',ram,0x67c37c,0,0)\n struct.pack_into('<I',ram,0xa08028",1)
s=s.replace('converted=(abs(127*level)//15)*(-1 if level<0 else 1)', 'clamped=max(-15,min(15,level));converted=(abs(127*clamped)//15)*(-1 if clamped<0 else 1)')
s=s.replace('s.r[4]=0x8ca08000','s.r[4]=0x8ca06000')
s=s.replace("hooks={a:hook(a) for a in addresses if a not in (entry,*helpers)}","hooks={error_target:hook(error_target)}")
s=s.replace(' cases+=1'," wanted_upper=bytearray(upper_before);struct.pack_into('<i',wanted_upper,0x70+4*selector,clamped)\n assert native[0xa06000:0xa06080]==wanted_upper\n assert all(event[0]==error_target for event in calls)\n cases+=1")
a=s.rindex("(out/'analysis/")
scope='252 full retail RAM/CPU/FPU/SR/PR/MACL comparisons of upper pan65dcca through native driver66837c,division10ca5c,wrapper669498,canonical command26 builder66b374,commit65a464,copy20 and loaded copy. Nine signed levels including clamps,mono/stereo,selectors0..3,both banks,empty/full256-entry queue. Mono selector1 excluded because the retail builder reads an uninitialized second-channel stack word. Independent complete128-byte upper object/clamped cache70+4*channel,canonical forwarded arguments,converted signedbyte/voice cache,entire20-byte command/stereo/lookup/aux fields,accepted queue record/tag/counters and full-queue preservation. Error reporter667b7a executes natively with callback unset. Only queue diagnostic65acac modeled. Physical audio/game caller integration/fullgame remain unverified.'
s=s[:a]+f"(out/'analysis/sound_subordinate_pan_native_enqueue_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope={scope!r}),indent=2)+'\\n')\nprint(cases,'upper pan native enqueue comparisons passed')\n"
(p/'verify_sound_subordinate_pan_native_enqueue.py').write_text(s)
reg=(p/'register_sound_setters_native_error.py').read_text().replace("['sound_subordinate_field8_set','sound_subordinate_fieldc_set']","['sound_subordinate_pan']").replace('_native_error','_native_enqueue').replace('==224','==252').replace(' native error:',' native enqueue:')
reg=reg.replace("'sa_sound_linear_source_identity_get'","'sa_signed_division','sa_sound_driver_pan','sa_sound_driver_pan_wrapper','sa_sound_pan_request'")
(p/'register_sound_subordinate_pan_native_enqueue.py').write_text(reg)
