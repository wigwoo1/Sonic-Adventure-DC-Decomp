from pathlib import Path
root=Path(__file__).resolve().parent
text=(root/'verify_sound_decode_stereo_samples.py').read_text()
text=text.replace('count,terminal,pattern,history,bank,coefficients','count,terminal,pattern,history,bank,coefficients,scale')
text=text.replace('[0,2,4],[False,True],range(0,256,16)','[2],[False],[0,16,112,128,240]')
text=text.replace('[(0,0),(1000,-500),(-300,200)]):','[(0,0),(1000,-500),(-300,200)],[1,4,8192]):')
text=text.replace(r"else b'\x00\x03'","else (scale-1).to_bytes(2,'big')")
text=text.replace('multiply_add(raw(4.),','multiply_add(raw(float(scale)),')
text=text.replace('sound_decode_stereo_samples_verification.json','sound_decode_stereo_scales_verification.json')
a=text.index("scope='");b=text.index("'),indent=2)",a)
text=text[:a]+"scope='180 full retail RAM/CPU/FPU/SR/PR/FPUL/MACL stereo scale comparisons,no modeled calls. Scales1/4/8192,five packed byte ranges with distinct channel patterns,two left histories,distinct right history,three predictor pairs,both banks. Independent signed-nibble exact binary32 recurrence/saturating conversion checks every sample/final history in both channels;source/output guards,stack/return. PR/SZ/RM0;other modes,traps,complete original streams/playback/fullgame unverified.'"+text[b+1:]
text=text.replace('stereo decoder independent sample comparisons passed','stereo decoder scale comparisons passed')
(root/'verify_sound_decode_stereo_scales.py').write_text(text)
