from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_linear_input_audio_decode.py').read_text()
s=s.replace("['assets'];records=[]","['assets'];assets=sorted(assets,key=lambda item:item['bytes'])[:1];records=[]")
s=s.replace(' data=original_prefix[:64]'," full_data=path.read_bytes();total_samples=int.from_bytes(original_prefix[12:16],'big');assert len(full_data)<0x30000\n data=original_prefix[:64]")
s=s.replace('ram[0xa10000:0xa11000]=original_prefix','ram[0xa10000:0xa10000+len(full_data)]=full_data')
s=s.replace('(0x8c65e0ea,[0x8ca10000,4096])','(0x8c65e0ea,[0x8ca10000,len(full_data)])')
s=s.replace('0x8c672424,4096,0,0x8ca10000,4096','0x8c672424,len(full_data),0,0x8ca10000,len(full_data)')
s=s.replace('(0x8c65bc1c,[0x8c8a3910,256])','(0x8c65bc1c,[0x8c8a3910,total_samples])')
s=s.replace('0xa40300]=bytes([0x5a])*768','0xa40900]=bytes([0x5a])*2304').replace('0xa50300]=bytes([0x5a])*768','0xa50900]=bytes([0x5a])*2304')
s=s.replace('(0xc,4096-header_length)','(0xc,len(full_data)-header_length)').replace('reserved=4096-header_length','reserved=len(full_data)-header_length').replace('==(0,4096)','==(0,len(full_data))')
body=(p/'native_audio_full_file_body.txt').read_text()
a=body.index('   while file_cursor<');z=body.index('   history=',a);body=body[:a]+body[z:]
body=body.replace('file_cursor=4096','file_cursor=len(full_data)')
body=body.replace('[0x8a65e8,0x8a6618]','[0x8a65b8,0x8a65e8]')
body=body.replace("ready=struct.unpack_from('<I',native,0x8a65c4)[0];input_cursor=struct.unpack_from('<I',native,0x8a65d0)[0]","ready,input_cursor=struct.unpack_from('<2I',native,0x8a6144)")
body=body.replace('min(ready,8192-input_cursor+64)//36','ready//36')
body=body.replace("list(struct.unpack_from('<12I',native,0x8a65b8))","list(struct.unpack_from('<9I',native,0x8a6138))")
body=body.replace("    diagnostic['terminal_trace']=terminal_trace\n",'').replace('native_audio_full_file_diagnostic.json','linear_input_audio_full_file_diagnostic.json')
body=body.replace('   update_index+=1',"   assert native[0xa10000:0xa10000+len(full_data)]==full_data\n   update_index+=1")
s=s.replace('  feed_updates+=1',body+'  feed_updates+=1')
s=s.replace('work_initialized=success))','work_initialized=success,total_samples=total_samples,streamed_updates=update_index,full_file_available=True))')
a=s.index("(out/'analysis/linear_input_audio_decode_verification.json')")
scope='Complete shortest original magic8000 audio file loaded into one native linear input source,both FPU banks. Native linear/ring/work/stream/handle pool initialization,construction,start,header,feed,public updates and native sample consumer query/commit with persistent RAM/CPU/FPU. No modeled calls. Full retail RAM/CPU/FPU/SR/PR/FPUL/MACL after each public operation;independent input object/header reservations/feed state,exact binary32 decoded PCM and predictor histories after every update,output ring/mirror/guards,sample totals and final state3 termination. Entire original input bytes checked preserved every update. Controlled caller normally requests256 or contiguous output space,then512 at final tail fitting empty ring;native completion trims padding. Actual report records sample and update totals. Other whole files,physical playback/game callers/fullgame unverified.'
s=s[:a]+f"(out/'analysis/linear_input_audio_full_file_verification.json').write_text(json.dumps(dict(cases_passed=cases,feed_updates_compared=feed_updates,original_files=len(assets),assets=records,scope={scope!r}),indent=2)+'\\n')\nprint(cases,'complete linear-input audio files passed')\n"
(p/'verify_linear_input_audio_full_file.py').write_text(s)
