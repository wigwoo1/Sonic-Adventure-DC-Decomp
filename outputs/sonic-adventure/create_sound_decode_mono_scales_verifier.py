from pathlib import Path
root=Path(__file__).resolve().parent
text=(root/'verify_sound_decode_block_mono.py').read_text()
text=text.replace('count,terminal,pattern,history,bank,coefficients','count,terminal,pattern,history,bank,coefficients,scale')
text=text.replace('[0,1,2],[False,True],range(0,256,16)', '[1],[False],[0,16,112,128,240]')
text=text.replace('[(0,0),(1000,-500),(-300,200)]):','[(0,0),(1000,-500),(-300,200)],[1,4,8192]):')
text=text.replace(r"else b'\x00\x03'", "else (scale-1).to_bytes(2,'big')")
text=text.replace('multiply_add(raw(4.),','multiply_add(raw(float(scale)),')
text=text.replace('sound_decode_block_mono_verification.json','sound_decode_mono_scales_verification.json')
a=text.index("scope='");b=text.index("'),indent=2)",a)
text=text[:a]+"scope='180 full retail RAM/CPU/FPU/SR/PR/FPUL/MACL mono block comparisons,no modeled calls. Scales1/4/8192,packed sample ranges00..0f/10..1f/70..7f/80..8f/f0..ff,two histories,three predictor pairs,both banks. Independent signed-nibble exact binary32 recurrence and saturating conversion checks every output sample and final history,source/output guards,return and stack. PR/SZ/RM0; original payloads,stereo,other modes/fullgame unverified."+text[b:]
text=text.replace('mono block decoder retail comparisons passed','mono decoder scale comparisons passed')
(root/'verify_sound_decode_mono_scales.py').write_text(text)
