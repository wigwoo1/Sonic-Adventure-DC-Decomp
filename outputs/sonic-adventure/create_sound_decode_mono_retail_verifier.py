from pathlib import Path
root=Path(__file__).resolve().parent
text=(root/'verify_sound_decode_block_mono.py').read_text()
helper=(root/'verify_audio_filter_native.py').read_text();oracle=helper[helper.index('def trig_oracle('):helper.index('cases=0')]
text=text.replace('import ctypes,itertools,json,struct','import ctypes,itertools,json,struct,math,hashlib')
text=text.replace('from sh4_binary32 import multiply,multiply_add,truncate_i32','from sh4_binary32 import multiply,multiply_add,truncate_i32,divide,add_sub')
a=text.index('cases=0');b=text.index('    ram=bytearray',a)
text=text[:a]+'''def f32(x):return struct.unpack('<f',struct.pack('<f',x))[0]
def raw(x):return struct.unpack('<I',struct.pack('<f',x))[0]
'''+oracle+'''assets=json.loads((out/'analysis/magic_audio_header_inventory.json').read_text())['assets'];records=[];cases=0
for asset,bank in itertools.product(assets,[0,1]):
    path=ROOT/asset['path']
    with path.open('rb') as file:
        header=file.read(64);offset=struct.unpack_from('>H',header,2)[0]+4
        file.seek(offset);payload=file.read(18)
    assert len(payload)==18 and header.hex()==asset['header_hex']
    count=1;terminal=bool(payload[0]&128);history=(0,0)
    scale=(int.from_bytes(payload[:2],'big')&8191)+1
    rate=struct.unpack_from('>I',header,8)[0];extension=struct.unpack_from('>h',header,16)[0]
    pi2=struct.unpack('<f',struct.pack('<I',0x40c90fda))[0]
    angle=raw(f32(f32(f32(float(extension))*pi2)/f32(float(rate))));trig=trig_oracle(angle)
    root2=f32(math.sqrt(2.));center=f32(root2-trig);denominator=f32(root2-1.)
    coefficient=f32(f32(center-f32(math.sqrt(f32(f32(center-denominator)*f32(center+denominator)))))/denominator)
    coefficients=(ctypes.c_int16(int(f32(coefficient*8192.))&65535).value,ctypes.c_int16(int(-f32(f32(coefficient*coefficient)*4096.))&65535).value)
'''+text[b:]
text=text.replace('multiply_add(raw(4.),','multiply_add(raw(float(scale)),')
text=text.replace('    cases+=1',"    records.append(dict(path=asset['path'],offset=offset,block_sha256=hashlib.sha256(payload).hexdigest(),bank=bank,scale=scale,coefficients=coefficients,decoded_blocks=decoded))\n    cases+=1")
text=text.replace('sound_decode_block_mono_verification.json','sound_decode_mono_retail_verification.json').replace('dict(cases_passed=cases,scope=', 'dict(cases_passed=cases,original_files=len(assets),assets=records,scope=')
a=text.index("scope='");b=text.index("'),indent=2)",a)
text=text[:a]+"scope='218 full retail RAM/CPU/FPU/SR/PR/FPUL/MACL mono decoder comparisons using first18-byte encoded block from109 original magic8000 audio files,both banks. Original BE header offset+4,rate/extension-derived predictor coefficients and original block scale. Independent signed-nibble exact binary32 recurrence and saturating conversion checks every sample/final history; original block hashes/source preservation,output guards,stack/return. No modeled calls. Zero starting history,only first single-channel block; stereo ordering,subsequent blocks,complete payload/playback/fullgame unverified.'"+text[b+1:]
text=text.replace('mono block decoder retail comparisons passed','original first-block mono decode comparisons passed')
(root/'verify_sound_decode_mono_retail.py').write_text(text)
