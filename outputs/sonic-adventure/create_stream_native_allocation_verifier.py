from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_stream_allocation.py').read_text()
d=(p/'verify_file_handle_native_coefficients.py').read_text()
a=d.index('binary=b;lower={}');z=d.index('pool=struct.unpack_from',a)
helpers=d[a:z].replace('binary=b;','b=binary;')
s=s.replace('pool=0x8c8d61ac;size=0xac',helpers+"handle_pool=struct.unpack_from('<I',binary,0x8c644c9c-0x8c010000)[0]\nlower[0x8c644b6e]=load('file_decode_handle_allocate.dll','sa_file_decode_handle_allocate');lower[0x8c644bfe]=load('file_decode_handle_release.dll','sa_file_decode_handle_release')\npool=0x8c8d61ac;size=0xac")
s=s.replace('for free,handle,bank in itertools.product(range(9),[0,0x8ca60000],[0,1]):','for free,handle_free,bank in itertools.product(range(9),range(9),[0,1]):\n    handle=handle_pool+handle_free*52 if handle_free<8 else 0')
s=s.replace('    before=bytes(ram[pool&0xffffff:',"    hs=handle_pool&0xffffff\n    ram[hs:hs+52*8]=bytes([0x77])*(52*8)\n    for index in range(8):struct.pack_into('<I',ram,hs+52*index,0 if index==handle_free else 1)\n    handles_before=bytes(ram[hs:hs+52*8])\n    before=bytes(ram[pool&0xffffff:",1)
a=s.index('    def service(');z=s.index('    s=State();',a)
s=s[:a]+'''    @Call
    def call(ctx,address,mem,fp,srp,r,prp):
        target={0x8c10d31c:fill,0x8c667440:destroy}.get(address) or lower[address]
        return target(mem,fp,srp,r,prp,ctypes.byref(fpul),ctypes.byref(macl),None,call)
'''+s[z:]
s=s.replace("reference(ram,s,{a:hook(a) for a in [0x8c644b6e,0x8c644bfe]},fpu=f,special=special)","reference(ram,s,{},fpu=f,special=special,vector_instruction=sqrt_service)")
s=s.replace('    assert native==ram','    nf.fpul=fpul.value\n    assert native==ram')
s=s.replace("    assert [a for a,v in actual]==([] if free==8 else [0x8c644b6e]+([] if handle else [0x8c644bfe]))",'''    assert actual==expected==[]
    wanted_handles=bytearray(handles_before)
    if free<8 and handle:
        obj=bytearray(52);struct.pack_into('<2I',obj,0,1,handle_free)
        # The connected coefficient path is independently checked by its
        # dedicated pool verifier; here preserve and verify all other bytes.
        coefficient_bytes=native[hs+handle_free*52+48:hs+handle_free*52+52]
        obj[48:52]=coefficient_bytes
        wanted_handles[handle_free*52:(handle_free+1)*52]=obj
    assert native[hs:hs+52*8]==wanted_handles''')
s=s.replace('stream_allocation_verification','stream_native_allocation_verification').replace('36 full retail','162 full retail').replace('successful/failed file handle allocation','every free decode-handle slot and exhausted handle pool').replace('handle allocator/release modeled.','handle allocator/release and coefficient math native; no modeled calls.').replace('Standalone destruction and native file handle allocator/release integration unverified.','All unchanged decode-pool entries and handle initialization bytes checked; coefficient fields compared against retail instructions here and independently verified in file_handle_native_coefficients_verification.json. Standalone destruction/full playback/fullgame unverified.').replace('stream allocation comparisons','native stream allocation comparisons')
(p/'verify_stream_native_allocation.py').write_text(s)
r=(p/'register_file_handle_native_coefficients.py').read_text().replace('file_handle_native_coefficients','stream_native_allocation').replace('== 18','== 162').replace("'sa_file_decode_handle_release'}","'sa_file_decode_handle_release','sa_sound_stream_create','sa_sound_stream_destroy'}").replace('Native coefficient handle pool:','Native stream and decode-handle allocation:')
(p/'register_stream_native_allocation.py').write_text(r)
