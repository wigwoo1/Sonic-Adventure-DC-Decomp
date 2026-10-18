from pathlib import Path
import re
p=Path(__file__).resolve().parent
s=(p/'verify_public_audio_final_pair.py').read_text()
s=s.replace("build/sound_stream_work_update.dll')).sa_sound_stream_work_update","build/sound_work_pool_update.dll')).sa_sound_work_pool_update")
s=s.replace("[(0x8c65bb4e,'sound_stream_work_tick'","[(0x8c65bb88,'sound_stream_work_update','sa_sound_stream_work_update'),(0x8c65bb4e,'sound_stream_work_tick'")
def relocate(match):
    value=int(match[0],16)
    if 0xa90000<=value<=0xa90100:return hex(0x8a3910+value-0xa90000)
    if 0x8ca90000<=value<=0x8ca90100:return hex(0x8c8a3910+value-0x8ca90000)
    return match[0]
s=re.sub(r'0x[0-9a-f]+',relocate,s)
s=s.replace('    ram[0x8a3911]=2',"    ram[0x8a3910:0x8a3d90]=bytes(0x480)\n    ram[0x8a3910]=1;ram[0x8a3911]=2")
s=s.replace('actual=[];expected=[]','actual=[];expected=[];pool_calls=[]')
s=s.replace('        if address in completion_helpers:',"        if address==0x8c65bb88:pool_calls.append(r[4])\n        if address in completion_helpers:")
s=s.replace('s.pc=0x8c65bb88;s.r[4]','s.pc=0x8c65bbd4;s.r[4]')
s=s.replace('    assert native[0x8a3911]==2',"    assert native[0x8a3911]==2 and pool_calls==[0x8c8a3910]\n    assert native[0x8a39a0:0x8a3d90]==bytes(0x3f0)")
s=s.replace('public_audio_final_pair_verification','pool_audio_final_pair_verification').replace('public audio final pair comparisons passed','pool audio final pair comparisons passed')
s=s.replace('through public work update65bb88(state2)','through native8-slot pool scheduler65bbd4,public work update65bb88(state2)').replace('No modeled calls.','No modeled calls. Independent exactly one public update call at original pool8c8a3910 and entire inactive7-slot region unchanged; first slot active1/state2 preserved.')
(p/'verify_pool_audio_final_pair.py').write_text(s)
r=(p/'register_public_audio_final_pair.py').read_text().replace('public_audio_final_pair','pool_audio_final_pair').replace("'sa_sound_stream_work_update'}","'sa_sound_stream_work_update','sa_sound_work_pool_update'}").replace('Original final pairs through public native update:','Original final pairs through native pool update:')
(p/'register_pool_audio_final_pair.py').write_text(r)
