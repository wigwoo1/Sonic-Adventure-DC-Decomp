from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_native_sources_audio_decode.py').read_text()
s=s.replace("['assets'];records=[]", "['assets'];assets=sorted(assets,key=lambda item:item['bytes'])[:1];records=[]")
s=s.replace(" data=original_prefix[:64]", " full_data=path.read_bytes();total_samples=int.from_bytes(original_prefix[12:16],'big')\n data=original_prefix[:64]")
s=s.replace('(0x8c65e3c6,[0x8ca10000,8192,0])','(0x8c65e3c6,[0x8ca10000,8192,64])')
s=s.replace('(0x8a65b8,0x8ca10000,8192,0)','(0x8a65b8,0x8ca10000,8192,64)')
s=s.replace('(0x8c65bc1c,[0x8c8a3910,256])','(0x8c65bc1c,[0x8c8a3910,total_samples])')
s=s.replace('0xa40300]=bytes([0x5a])*768','0xa40900]=bytes([0x5a])*2304').replace('0xa50300]=bytes([0x5a])*768','0xa50900]=bytes([0x5a])*2304')
extra=(p/'native_audio_full_file_body.txt').read_text()
s=s.replace('  feed_updates+=1',extra+'  feed_updates+=1')
s=s.replace(' assert native[0xa10000:0xa11000]==original_prefix\n','')
s=s.replace('actual=[];expected=[]','actual=[];expected=[];terminal_trace=[]')
s=s.replace(' def call(ctx,a,m,f,sr,r,pr):', " def call(ctx,a,m,f,sr,r,pr):\n  if a in [0x8c667844,0x8c667978,0x8c644c38] and struct.unpack_from('<I',native,0x8a3944)[0]>total_samples-1024:\n   terminal_trace.append(dict(entry=hex(a),arguments=[r[i] for i in range(4,8)],stream_words=list(struct.unpack_from('<43I',native,state))))")
s=s.replace('native_sources_audio_decode_verification','native_audio_full_file_verification')
s=s.replace('work_initialized=success))','work_initialized=success,total_samples=total_samples,streamed_updates=update_index,full_file_published=file_cursor==len(full_data)))')
a=s.index("scope='218 full");z=s.index("'),indent=2)",a)
scope='Complete shortest original magic-audio asset, both FPU banks. Native source/work/stream/handle initialization and construction, producer query/copy/commit with repeated refill, consumer query/commit and public audio updates, with persistent RAM/CPU/FPU. Every public operation compared against retail full RAM/CPU/FPU/SR/PR/FPUL/MACL. Independent producer source fields and complete ring/mirror after every refill, exact decoded PCM and predictor history after every update, reported sample trimming, output ring/mirror,guards,consumed total and final work termination. Actual records report asset, sample count, update count and publication extent. No modeled calls. Other complete files,physical playback/fullgame unverified.'
scope+=' Controlled caller requests normally256 samples or contiguous output space,then512 for the final tail when remaining samples fit an empty ring; native completion trims padding. Variable input-window update sizes are independently derived. The failed earlier exact-tail request diagnostic is retained as an investigation artifact,not passing evidence.'
s=s[:a]+'scope='+repr(scope)+s[z+1:]
(p/'verify_native_audio_full_file.py').write_text(s)
