from pathlib import Path
root=Path(__file__).resolve().parent
text=(root/'verify_sound_decode_block_mono.py').read_text().replace('sa_sound_decode_block_mono','sa_sound_decode_block_stereo').replace('s.pc=0x8c65b120','s.pc=0x8c65b238')
text=text.replace('[0,1,2],[False,True],range(0,256,16)','[0,2,4],[False,True],[0,120]')
text=text.replace('range(2):payload','range(4):payload')
text=text.replace("struct.pack_into('<2i',ram,0xf00000,*coefficients)","ram[0xa40000:0xa40100]=bytes([0x55])*256;struct.pack_into('<2h',ram,0xa50000,-321,654)\n    struct.pack_into('<2I2i',ram,0xf00000,0x8ca40008,0x8ca50000,*coefficients)")
text=text.replace('decoded=0 if terminal else count','decoded=0 if terminal else count\n    channel_blocks=decoded//2')
text=text.replace('decoded*64','channel_blocks*64')
a=text.index('    newest,older=');b=text.index('    cases+=1',a)
text=text[:a]+"    assert native[0xa40000:0xa40008]==bytes([0x55])*8 and native[0xa40008+channel_blocks*64:0xa40100]==bytes([0x55])*(248-channel_blocks*64)\n    if terminal and count:assert native[0xa50000:0xa50004]==struct.pack('<2h',-321,654)\n"+text[b:]
text=text.replace('sound_decode_block_mono_verification.json','sound_decode_stereo_initial_verification.json')
a=text.index("scope='");b=text.index("'),indent=2)",a)
text=text[:a]+"scope='144 full retail RAM/CPU/FPU/SR/PR/FPUL/MACL comparisons of238-instruction stereo decoder65b238..65b428,no modeled calls. ABI R4source,R5totalblocks,R6leftoutput,R7lefthistory,stack rightoutput/righthistory/two signed coefficients. Even counts0/2/4,first-block termination,two packed patterns,two left histories,distinct right history,three coefficient pairs,both banks. Independent return,total per-channel output extent/guards,terminal histories,source/stack preservation. Numerical samples/history compared to retail only; independent stereo recurrence,odd counts/right termination,original payloads/fullgame unverified.'"+text[b+1:]
text=text.replace('mono block decoder retail comparisons passed','stereo decoder initial retail comparisons passed')
(root/'verify_sound_decode_stereo_initial.py').write_text(text)
