from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_file_decode_native_termination.py').read_text()
a=s.index('for count,terminal_position,pattern,history,bank,coefficients');z=s.index('    ram=bytearray',a)
s=s[:a]+'''assets=[a for a in json.loads((out/'analysis/magic_audio_endings_inventory.json').read_text())['assets'] if a['first_word_after_end']=='0x8001']
for asset,terminal_position,history,bank,coefficients in itertools.product(assets,[0,2],[(0,0),(123,-456)],[0,1],[(0,0),(1000,-500),(-300,200)]):
    count=4;pattern=0
    path=ROOT/asset['path']
    with path.open('rb') as file:
        file.seek(asset['declared_encoded_end']-36);final_pair=file.read(36);marker=file.read(18)
    assert final_pair.hex()==asset['final_pair_hex'] and marker[:2]==b'\\x80\\x01'
    assert marker.hex()==asset['after_declared_end_hex'][:36]
    # Bytes after the terminating block are explicit guard data, never decoded.
    payload=(final_pair+marker+bytes(18)) if terminal_position==2 else marker+bytes(54)
'''+s[z:]
s=s.replace('sample=multiply_add(raw(4.),',"scale=(int.from_bytes(payload[block*18:block*18+2],'big')&8191)+1\n                    sample=multiply_add(raw(float(scale)),")
a=s.index("scope='");z=s.index("'),indent=2)",a)
s=s[:a]+"scope='2592 full retail RAM/CPU/FPU/SR/PR/FPUL/MACL native file dispatcher/stereo decoder/global flag comparisons using actual terminal block8001 from108 original magic8000 files. Terminal block first or after actual final stereo pair,two controlled predictor histories,three coefficient pairs,both FPU banks. No modeled calls. Independent every emitted sample using original per-block scales,phase3/result0or2,unchanged stored histories on termination,complete outputs/source/global guards. Original tail bytes and pair hashes matched inventory. Starting histories/coefficients controlled,not reconstructed from full preceding files. Final valid-sample trimming,SSRACING inconsistent header,looping,fullpayload/playback/fullgame unverified."+s[z:]
s=s.replace('file_decode_native_termination_verification','original_audio_terminal_verification').replace('native file dispatcher termination comparisons','original terminal comparisons')
(p/'verify_original_audio_terminal.py').write_text(s)
r=(p/'register_magic_empty_tail.py').read_text().replace('magic_empty_tail','original_audio_terminal').replace('==654','==2592')
a=r.index('names=');z=r.index(';found=set()',a)
r=r[:a]+"names={'sa_file_decode_request_process','sa_sound_decode_block_stereo','sa_file_decode_odd_flag_set'}"+r[z:]
r=r.replace('Native original-audio empty-tail updates:','Original audio terminal blocks:')
(p/'register_original_audio_terminal.py').write_text(r)
