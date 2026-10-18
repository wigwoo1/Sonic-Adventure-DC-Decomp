from pathlib import Path
root=Path(__file__).resolve().parent
text=(root/'verify_file_decode_stereo_request.py').read_text().replace('file_decode_stereo_request','file_decode_mono_request').replace('s.pc=0x8c644c38','s.pc=0x8c644c1a')
text=text.replace("0x14,2,0x8ca10000,count,0x8ca30000,right)", "0x14,1,0x8ca10000,count,0x8ca30000,0)")
text=text.replace('stereo request setup644c38..644c56','mono request setup644c1a..644c38').replace('null/non-null right destinations','varied unused stack words').replace('selector/source/count/output fields','selector1/source/count/left output and cleared right output fields')
text=text.replace('stereo request setup comparisons passed','mono request setup comparisons passed')
(root/'verify_file_decode_mono_request.py').write_text(text)
text=(root/'build_file_decode_stereo_request.ps1').read_text().replace('file_decode_stereo_request','file_decode_mono_request')
(root/'build_file_decode_mono_request.ps1').write_text(text)
text=(root/'register_file_decode_stereo_request.py').read_text().replace('file_decode_stereo_request','file_decode_mono_request').replace('644c38','644c1a').replace('644c56','644c38').replace('634c38','634c1a').replace('634c56','634c38').replace('Stereo decode request setup:','Mono decode request setup:')
(root/'register_file_decode_mono_request.py').write_text(text)
