from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_work_complete_native_getters.py').read_text()
s=s.replace("build/sound_stream_work_complete.dll')).sa_sound_stream_work_complete","build/sound_stream_work_tick.dll')).sa_sound_stream_work_tick")
s=s.replace("[(0x8c6676f6,'sound_stream_total_samples_get')","[(0x8c65ba22,'sound_stream_work_complete'),(0x8c667718,'sound_stream_phase_get'),(0x8c6676f6,'sound_stream_total_samples_get')")
s=s.replace('services=[0x8c010300]','services=[0x8c667ad4,0x8c010300]')
s=s.replace('        if address==0x8c6676f6:r[0]=total',"        if address==0x8c667ad4:assert r[4]==0x8ca70000\n        elif address==0x8c6676f6:r[0]=total")
s=s.replace('s.pc=0x8c65ba22','s.pc=0x8c65bb4e')
s=s.replace('assert [a for a,args in actual]==([0x8c010300] if notify else [])','assert [a for a,args in actual]==[0x8c667ad4]+([0x8c010300] if notify else [])')
s=s.replace('completion comparisons65ba22..65bb4e','dispatcher-to-completion comparisons65bb4e to 65ba22').replace('only notification callback modeled.','stream update no-op and notification callback modeled. Phase getter and completion routine native; phase3 dispatch verified.').replace('Actual asset final trimming/full playback/fullgame unverified.','Decoder advancement in this integration test, actual asset final trimming/full playback/fullgame unverified.')
s=s.replace('work_complete_native_getters_verification','tick_native_complete_verification').replace('native getters work completion comparisons passed','native completion dispatcher comparisons passed')
(p/'verify_tick_native_complete.py').write_text(s)
r=(p/'register_work_complete_native_getters.py').read_text().replace('work_complete_native_getters','tick_native_complete').replace("'sa_sound_stream_decoded_samples_get'}","'sa_sound_stream_decoded_samples_get','sa_sound_stream_phase_get','sa_sound_stream_work_tick'}").replace('Native completion getters in stream work completion:','Native completion in dispatcher:')
(p/'register_tick_native_complete.py').write_text(r)
