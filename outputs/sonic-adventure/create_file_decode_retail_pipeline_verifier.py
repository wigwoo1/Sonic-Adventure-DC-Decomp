from pathlib import Path
root=Path(__file__).resolve().parent
text=(root/'verify_sound_decode_stereo_prefix.py').read_text()
text=text.replace("fn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_decode_blocks_draft.dll')).sa_sound_decode_block_stereo", "kernel=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/sound_decode_blocks_draft.dll')).sa_sound_decode_block_stereo\nfn=ctypes.CDLL(str(ROOT/'work/sonic-adventure/build/file_decode_request_process.dll')).sa_file_decode_request_process")
text=text.replace('def f32(', 'kernel.argtypes=fn.argtypes\ndef f32(',1)
text=text.replace('native=bytearray(ram);',"ram[0xa60000:0xa60040]=bytes([0x55])*64\n    struct.pack_into('<6I',ram,0xa6000c,1,0x55555555,2,0x8ca10000,count,0x8ca20008)\n    struct.pack_into('<I4h2h',ram,0xa60024,0x8ca40008,0,0,0,0,*coefficients)\n    native=bytearray(ram);")
text=text.replace('s.r[4:8]=[0x8ca10000,count,0x8ca20008,0x8ca30000]', 's.r[4]=0x8ca60000').replace('s.pc=0x8c65b238','s.pc=0x8c644cb0')
text=text.replace("def call(*args):raise AssertionError('Unexpected call')", "def call(ctx,address,mem,fp,srp,r,prp):\n        assert address==0x8c65b238,address\n        return kernel(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)")
text=text.replace('assert regs[0]==decoded and regs[15]==0x8cf00000', "assert struct.unpack_from('<2I',native,0xa6000c)==(3,decoded) and regs[15]==0x8cf00000")
text=text.replace('(0,history,0xa20008,0xa30000),(1,(0,0),0xa40008,0xa50000)', '(0,history,0xa20008,0xa60028),(1,(0,0),0xa40008,0xa6002c)')
text=text.replace('sound_decode_stereo_prefix_verification.json','file_decode_retail_pipeline_verification.json')
text=text.replace('stereo decoder comparisons','native file dispatcher plus stereo decoder comparisons').replace('source/guards,stack/return checks','source/guards,dispatcher phase1->3/result storage,stack/return checks')
text=text.replace('original stereo eight-pair prefix comparisons passed','original file-dispatch stereo pipeline comparisons passed')
(root/'verify_file_decode_retail_pipeline.py').write_text(text)
