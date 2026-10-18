from pathlib import Path
root=Path(__file__).resolve().parent
text=(root/'verify_sound_stream_mono_request.py').read_text().replace('sound_stream_mono_request','sound_stream_stereo_request').replace('file_decode_mono_request','file_decode_stereo_request').replace('s.pc=0x8c6677e2','s.pc=0x8c66780e').replace('0x8c644c1a:setup','0x8c644c38:setup')
text=text.replace("struct.pack_into('<I',ram,0xa60070,right)", "struct.pack_into('<I',ram,0xa60070,right);struct.pack_into('<I',ram,0xa6006c,128)")
text=text.replace("0x14,1,0x8ca10000,count,0x8ca30000+right*2,0)", "0x14,2,0x8ca10000,(count*2)&0xffffffff,0x8ca30000+right*2,0x8ca30000+right*2+256)")
text=text.replace('stream mono request6677e2..66780e','stream stereo request66780e..667844').replace('setup644c1a','setup644c38').replace('output base+2*sample offset','left output base+2*sample offset, right output left+2*channel span, doubled block count with32-bit wrap').replace('stream mono request comparisons passed','stream stereo request comparisons passed')
(root/'verify_sound_stream_stereo_request.py').write_text(text)
text=(root/'build_sound_stream_mono_request.ps1').read_text().replace('sound_stream_mono_request','sound_stream_stereo_request');(root/'build_sound_stream_stereo_request.ps1').write_text(text)
text=(root/'register_sound_stream_mono_request.py').read_text().replace('sound_stream_mono_request','sound_stream_stereo_request').replace('66780e','667844').replace('6677e2','66780e').replace('65780e','657844').replace('6577e2','65780e').replace('Stream mono request:','Stream stereo request:');(root/'register_sound_stream_stereo_request.py').write_text(text)
