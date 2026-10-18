from pathlib import Path
root=Path(__file__).resolve().parent
text=(root/'verify_sound_decode_stereo_samples.py').read_text()
mono=(root/'verify_sound_decode_mono_retail.py').read_text()
prefix=mono[mono.index('def f32('):mono.index('    ram=bytearray')]
prefix=prefix.replace('file.read(18)','file.read(36)').replace('len(payload)==18','len(payload)==36').replace('count=1','count=2')
prefix=prefix.replace("assert len(payload)==36 and header.hex()==asset['header_hex']", "assert len(payload)==36 and header.hex()==asset['header_hex'] and header[7]==2\n    pattern=0")
text=text.replace('import ctypes,itertools,json,struct','import ctypes,itertools,json,struct,math,hashlib')
text=text.replace('from sh4_binary32 import multiply,multiply_add,truncate_i32','from sh4_binary32 import multiply,multiply_add,truncate_i32,divide,add_sub')
a=text.index('cases=0');b=text.index('    ram=bytearray',a);text=text[:a]+prefix+text[b:]
text=text.replace('struct.pack_into(\'<2h\',ram,0xa50000,-321,654)',"struct.pack_into('<2h',ram,0xa50000,0,0)")
text=text.replace("struct.pack('<2h',-321,654)","struct.pack('<2h',0,0)").replace('(1,(-321,654),','(1,(0,0),')
text=text.replace('block=pair*2+channel','block=pair*2+channel\n            scale=(int.from_bytes(payload[block*18:block*18+2],\'big\')&8191)+1')
text=text.replace('multiply_add(raw(4.),','multiply_add(raw(float(scale)),')
text=text.replace('    cases+=1',"    records.append(dict(path=asset['path'],offset=offset,pair_sha256=hashlib.sha256(payload).hexdigest(),bank=bank,coefficients=coefficients,decoded_blocks=decoded))\n    cases+=1")
text=text.replace('sound_decode_stereo_samples_verification.json','sound_decode_stereo_retail_verification.json').replace('dict(cases_passed=cases,scope=', 'dict(cases_passed=cases,original_files=len(assets),assets=records,scope=')
a=text.index("scope='");b=text.index("'),indent=2)",a)
text=text[:a]+"scope='218 full retail RAM/CPU/FPU/SR/PR/FPUL/MACL stereo decoder comparisons using first36-byte encoded pair from109 original two-channel magic8000 files,both banks. Original data offsets,header-derived predictor coefficients,distinct original left/right bytes and per-block scales. Independent per-channel exact binary32 signed-nibble recurrence/saturating conversion checks every sample/final history. Pair hashes,source/guards,stack/return checks,no modeled calls. Zero starting histories;subsequent pairs,fullpayload,odd-count/termination branches,physical playback/fullgame unverified.'"+text[b+1:]
text=text.replace('stereo decoder independent sample comparisons passed','original stereo first-pair comparisons passed')
(root/'verify_sound_decode_stereo_retail.py').write_text(text)
