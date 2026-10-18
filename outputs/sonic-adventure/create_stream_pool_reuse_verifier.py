from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'verify_stream_native_allocation.py').read_text()
s=s.replace('    cases+=1', '''    stream_pointer=regs[0]
    allocated_streams=bytes(wanted);allocated_handles=bytes(wanted_handles)
    # Destroy a successfully allocated stream, or pass null after a failure.
    s.r[4]=stream_pointer;regs[4]=stream_pointer;s.pc=0x8c667440
    reference(ram,s,{},fpu=f,special=special,vector_instruction=sqrt_service)
    assert destroy(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    nf.fpul=fpul.value
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
    if stream_pointer:
        wanted[free*size:(free+1)*size]=bytes(size)
        wanted_handles[handle_free*52:(handle_free+1)*52]=bytes(52)
    assert native[pool&0xffffff:(pool&0xffffff)+size*8]==wanted
    assert native[hs:hs+52*8]==wanted_handles
    # Null destruction must preserve both entire pools and every RAM byte.
    prior_ram=bytes(native);s.r[4]=0;regs[4]=0;s.pc=0x8c667440
    reference(ram,s,{},fpu=f,special=special,vector_instruction=sqrt_service)
    assert destroy(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    nf.fpul=fpul.value
    # SH4 prologue/epilogue stack stores are the sole permitted RAM changes.
    assert native[:0xeffff8]==prior_ram[:0xeffff8] and native[0xf00000:]==prior_ram[0xf00000:]
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
    for index in range(4,8):s.r[index]=args[index-4];regs[index]=s.r[index]
    s.pc=0x8c6673b4
    reference(ram,s,{},fpu=f,special=special,vector_instruction=sqrt_service)
    assert create(view,ctypes.byref(nf),ctypes.byref(sr),regs,ctypes.byref(pr),ctypes.byref(fpul),ctypes.byref(macl),None,call)==0
    nf.fpul=fpul.value
    assert native==ram and list(regs)==list(s.r) and sr.value==s.sr and pr.value==s.pr and bytes(nf)==bytes(f) and macl.value==special['macl']
    assert native[pool&0xffffff:(pool&0xffffff)+size*8]==allocated_streams
    assert native[hs:hs+52*8]==allocated_handles
    assert regs[0]==stream_pointer and regs[15]==0x8cf00000
    cases+=1''')
s=s.replace('stream_native_allocation_verification','stream_pool_reuse_verification').replace('162 full retail','162 persistent construction/destruction/null-destruction/reallocation sequences with full retail').replace('Standalone destruction/full playback/fullgame unverified.','Successful destruction independently clears the complete selected stream and decode handle; null destruction leaves all RAM outside prologue stack writes unchanged; reallocation restores identical objects and selects the same freed slots. Failed/exhausted cases also repeat. Full playback/fullgame unverified.').replace('native stream allocation comparisons','native stream pool reuse sequences')
(p/'verify_stream_pool_reuse.py').write_text(s)
r=(p/'register_stream_native_allocation.py').read_text().replace('stream_native_allocation','stream_pool_reuse').replace('Native stream and decode-handle allocation:','Native stream and decode-handle pool reuse:')
r=r.replace("found.add(routine['name'])","found.add(routine['name'])\n        routine['status'] = 'Compiled and matched within primary and additional verification report scopes'")
(p/'register_stream_pool_reuse.py').write_text(r)
