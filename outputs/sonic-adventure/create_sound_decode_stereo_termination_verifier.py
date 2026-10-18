from pathlib import Path
root=Path(__file__).resolve().parent
text=(root/'verify_sound_decode_stereo_samples.py').read_text()
text=text.replace('count,terminal,pattern,history,bank,coefficients','count,terminal_position,pattern,history,bank,coefficients')
text=text.replace('[0,2,4],[False,True],range(0,256,16)','[4],range(4),[0,120]')
text=text.replace('terminal and block==0','block==terminal_position')
text=text.replace('count,terminal,pattern,history,code','count,terminal_position,pattern,history,code')
text=text.replace('decoded=0 if terminal else count','decoded=terminal_position')
text=text.replace('channel_blocks=decoded//2','left_blocks=(decoded+1)//2;right_blocks=decoded//2')
text=text.replace('0xa20008+channel_blocks*64','0xa20008+left_blocks*64').replace('(248-channel_blocks*64)', '(248-left_blocks*64)',1)
text=text.replace('0xa40008+channel_blocks*64','0xa40008+right_blocks*64').replace('(248-channel_blocks*64)', '(248-right_blocks*64)',1)
text=text.replace('if terminal and count:','if True:').replace('range(channel_blocks)','range(left_blocks if channel==0 else right_blocks)').replace('if not (terminal and count):','if False:')
text=text.replace('sound_decode_stereo_samples_verification.json','sound_decode_stereo_termination_verification.json')
a=text.index("scope='");b=text.index("'),indent=2)",a)
text=text[:a]+"scope='96 full retail RAM/CPU/FPU/SR/PR/FPUL/MACL stereo termination comparisons,no modeled calls. Termination at each of four alternating left/right blocks,two packed patterns,two left histories,distinct right history,three coefficient pairs,both banks. Independent per-channel recurrence checks all samples emitted before marker,left/right output extents,return marker index,source/guards and unchanged stored histories despite partial output. Scale4,PR/SZ/RM0;other scales,complete original streams/playback/fullgame unverified.'"+text[b+1:]
text=text.replace('stereo decoder independent sample comparisons passed','stereo decoder termination comparisons passed')
(root/'verify_sound_decode_stereo_termination.py').write_text(text)
