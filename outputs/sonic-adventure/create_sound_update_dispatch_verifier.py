from pathlib import Path
root=Path(__file__).resolve().parent
text=(root/'verify_file_decode_result_get.py').read_text().replace('file_decode_result_get','sound_stream_update_dispatch').replace('s.pc=0x8c644c8c','s.pc=0x8c667ad4').replace('[0,1,2,15,0xffffffff]','[0,1,2,3,4,15,0xffffffff]')
text=text.replace("    native=bytearray(ram);", "    struct.pack_into('<H',ram,0xa200a8,count&65535);actual=[];expected=[]\n    native=bytearray(ram);")
text=text.replace("def call(*args):raise AssertionError('Unexpected service')", "def call(ctx,address,mem,fp,srp,r,prp):actual.append((address,r[4]));return 0\n    def hook(address):\n        def run(mem,r,sr):expected.append((address,r[4]));return sr\n        return run")
text=text.replace("reference(ram,s,fpu=f,special={'macl':0})", "reference(ram,s,{a:hook(a) for a in [0x8c667a5c,0x8c66ab5e,0x8c66a524,0x8c669bfc,0x8c66a348]},fpu=f,special={'macl':0})")
text=text.replace('    assert regs[0]==count', "    target={0:0x8c667a5c,1:0x8c66ab5e,2:0x8c66a524,3:0x8c669bfc,4:0x8c66a348}.get(count&65535)\n    assert actual==expected==([] if target is None else [(target,0x8ca20000)])")
start=text.index("(out/'analysis/sound_stream_update_dispatch_verification.json')")
text=text[:start]+"""(out/'analysis/sound_stream_update_dispatch_verification.json').write_text(json.dumps(dict(cases_passed=cases,scope='140 full retail RAM/CPU/FPU/SR/PR update format dispatcher comparisons667ad4..667b16. Formats0/1/2/3/4/15/ffff,varied phase/register/stack inputs,both banks. All five downstream update bodies modeled; independent target selection and unchanged handle bytes,stack checked. Shared format0 body reached through external conditional branch. Native update integration/playback/fullgame unverified.'),indent=2)+'\\n')
print(cases,'stream update dispatcher comparisons passed')
"""
(root/'verify_sound_stream_update_dispatch.py').write_text(text)
text=(root/'register_sound_stream_decode_complete.py').read_text().replace('sound_stream_decode_complete','sound_stream_update_dispatch').replace('==288','==140').replace('667978','667ad4').replace('667a4a','667b16').replace('657978','657ad4').replace('657a4a','657b16').replace('Stream decode completion:','Stream update dispatcher:');(root/'register_sound_stream_update_dispatch.py').write_text(text)
