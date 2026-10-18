from pathlib import Path
root=Path(__file__).resolve().parent
text=(root/'verify_sound_decode_stereo_initial.py').read_text()
text=text.replace('[False,True],[0,120]','[False,True],range(0,256,16)')
text=text.replace('bytes(range(pattern,pattern+16))','bytes(v^(128 if block&1 else 0) for v in range(pattern,pattern+16))')
insert='''    for channel,initial,destination,histaddr in [(0,history,0xa20008,0xa30000),(1,(-321,654),0xa40008,0xa50000)]:
        newest,older=[raw(float(v*65536)) for v in initial]
        c1,c2=[raw(v/4096.) for v in coefficients];samples=[]
        for pair in range(channel_blocks):
            block=pair*2+channel
            for packed in payload[block*18+2:block*18+18]:
                for nibble in [packed>>4,packed&15]:
                    signed=nibble-16 if nibble&8 else nibble
                    prediction=multiply_add(c1,newest,multiply(c2,older)[0])[0]
                    sample=multiply_add(raw(4.),raw(float(signed*65536)),prediction)[0]
                    samples.append((truncate_i32(sample)[0]>>16)&65535)
                    older,newest=newest,sample
        assert native[destination:destination+len(samples)*2]==struct.pack('<'+'H'*len(samples),*samples)
        if not (terminal and count):
            expected_history=struct.pack('<2H',(truncate_i32(newest)[0]>>16)&65535,(truncate_i32(older)[0]>>16)&65535)
            assert native[histaddr:histaddr+4]==expected_history
'''
text=text.replace('    cases+=1',insert+'    cases+=1')
text=text.replace('sound_decode_stereo_initial_verification.json','sound_decode_stereo_samples_verification.json')
a=text.index("scope='");b=text.index("'),indent=2)",a)
text=text[:a]+"scope='1152 full retail RAM/CPU/FPU/SR/PR/FPUL/MACL comparisons of238-instruction stereo decoder65b238..65b428,no modeled calls. Even counts0/2/4,first-block termination,all256 packed bytes with distinct channel patterns,two left histories,distinct right history,three signed predictor pairs,both banks. Independent per-channel signed-nibble exact binary32 recurrence and saturating conversion checks every sample/final history;source/output guards,stack/return. Scale4,PR/SZ/RM0;other scales,odd counts,right termination,original payloads/fullgame unverified.'"+text[b+1:]
text=text.replace('stereo decoder initial retail comparisons passed','stereo decoder independent sample comparisons passed')
(root/'verify_sound_decode_stereo_samples.py').write_text(text)
