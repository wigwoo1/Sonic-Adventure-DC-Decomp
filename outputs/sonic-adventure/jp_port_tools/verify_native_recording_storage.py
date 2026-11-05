"""Exercise actual owned buffers, Unicode Windows files and recorder boundaries.

This tests the platform dependency, not original recording_update or pad_scan.
All files are synthetic and written beneath a fresh private test directory.
"""
import argparse,ctypes as C,hashlib,json,struct,threading
from pathlib import Path

def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.build.resolve();out=a.output.resolve()
    assert out.is_relative_to(r/'work/sonic-adventure') and not out.exists();out.mkdir()
    b=json.loads((d/'build_verification.json').read_bytes());assert sha(b['DLL'])==b['DLL_sha256']
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    dll=C.CDLL(b['DLL'])
    class Info(C.Structure):
        _fields_=[('save_count',C.c_uint64),('debug_count',C.c_uint64)]+[(n,C.c_uint32) for n in ['status','os_error','loaded_bytes','saved_bytes','legacy_cursor']]
    signatures={'initialize':([C.c_wchar_p],C.c_int),'shutdown':([],C.c_int),'status':([],C.c_int),'clear_error':([],None),'snapshot':([C.POINTER(Info)],C.c_int),'begin_record':([],C.c_int),'load_memory':([C.c_void_p,C.c_size_t],C.c_int),'load_file':([C.c_wchar_p],C.c_int),'buffer':([],C.c_void_p),'storage_bytes':([],C.c_size_t),'check_cursor':([C.c_void_p,C.c_int],C.c_int),'save':([C.c_char_p,C.c_void_p,C.c_int],C.c_int),'debug_cursor':([C.c_void_p],C.c_int)}
    for n,(args,ret) in signatures.items():f=getattr(dll,'jp_recording_'+n);f.argtypes=args;f.restype=ret
    f=lambda n,*args:getattr(dll,'jp_recording_'+n)(*args)
    checks=0
    def eq(got,want,label):
        nonlocal checks
        assert got==want,(label,got,want);checks+=1
    def info():
        i=Info();eq(f('snapshot',C.byref(i)),0,'snapshot');return i
    def clear():f('clear_error');eq(f('status'),0,'error cleared')
    def load(raw):return f('load_memory',C.create_string_buffer(bytes(raw)),len(raw))
    root=out/'記録';root.mkdir()
    eq(f('status'),2,'uninitialized');eq(f('buffer'),None,'uninitialized buffer')
    eq(f('initialize',None),1,'null root');eq(f('initialize',str(root/'missing')),1,'missing root')
    eq(f('initialize',str(root)),0,'initialize Unicode root');eq(f('initialize',str(root)),2,'duplicate initialize')
    base=f('buffer');capacity=f('storage_bytes');eq(capacity,131074*12,'owned capacity')
    eq(f('snapshot',None),1,'null snapshot')
    eq(f('begin_record'),0,'begin record');eq(C.string_at(base,capacity),bytes(capacity),'zero owned buffer')
    # Every original recording cursor, the sentinel slot, and historical
    # double-increment cursor are explicitly checked.
    for index in range(10922):eq(f('check_cursor',base+index*12,0),0,'valid record slot')
    eq(f('check_cursor',base+10922*12,0),6,'past record sentinel slot');clear()
    eq(f('check_cursor',base+10923*12,2),0,'original stop double increment')
    for cursor in [0,base-12,base+1,base+capacity,base+capacity+12]:
        eq(f('check_cursor',cursor,2),6,'bad cursor');eq(f('status'),6,'sticky bounds');clear()
    eq(f('check_cursor',base,3),1,'bad mode');clear()
    eq(f('check_cursor',base,1),6,'unloaded playback');clear()
    marker=struct.pack('<IHHhh',0xffffffff,0x9876,0x4567,-12345,23456)
    for raw in [b'',b'\xff'*11,bytes(12),bytes(24),marker+b'\x00']:
        before=C.string_at(base,48);eq(load(raw),5,'malformed playback');eq(C.string_at(base,48),before,'invalid load atomic');clear()
    eq(f('load_memory',None,12),1,'null source');clear()
    eq(f('load_memory',base,12),1,'overlapping source');clear()
    eq(f('load_memory',base-1,24),1,'overlap from below');clear()
    eq(f('load_memory',1,1572877),1,'oversize before dereference');clear()
    raw=struct.pack('<IHHhh',0x1234,0,65535,-32768,32767)+marker
    eq(load(raw),0,'load two samples');eq(C.string_at(base,len(raw)),raw,'preserve sample bits');eq(info().loaded_bytes,24,'loaded extent')
    eq(f('check_cursor',base+12,1),0,'sentinel readable');eq(f('check_cursor',base+24,1),6,'loaded end bounded');clear()
    trailing=raw+struct.pack('<IHHhh',5,6,7,8,9)
    eq(load(trailing),0,'original trailing whole samples allowed');eq(C.string_at(base,len(trailing)),trailing,'trailing bytes preserved')
    maximum=bytearray(1572876);maximum[-12:]=marker
    eq(load(maximum),0,'full historical playback extent')
    for index in range(131073):eq(f('check_cursor',base+index*12,1),0,'all original playback slots')
    eq(f('check_cursor',base+1572876,1),6,'loaded maximum end');clear()
    eq(C.string_at(base+1572864,12),marker,'last inclusive marker')
    eq(load(raw),0,'reload shorter');eq(C.string_at(base+24,capacity-24),bytes(capacity-24),'old playback tail cleared')
    # Real file effects, all six original export names, replacement and exact
    # little-endian bytes. No save callback or fake filesystem is involved.
    names=['K_SONIC.BIN','K_MILES.BIN','K_KNUCKLES.BIN','K_AMY.BIN','K_E102.BIN','K_BIG.BIN']
    for index,name in enumerate(names):
        data=struct.pack('<IHHhh',index+1,index,65535-index,-32768+index,32767-index)+marker
        eq(load(data),0,'prepare save data');eq(f('save',name.encode(),base,len(data)),0,'actual save')
        eq((root/name).read_bytes(),data,'exact saved bytes');eq(info().saved_bytes,len(data),'saved byte count')
        eq(f('begin_record'),0,'clear before disk load');eq(f('load_file',str(root/name)),0,'actual disk playback load')
        eq(C.string_at(base,len(data)),data,'disk playback exact')
    eq(info().save_count,6,'six completed saves')
    data=marker;eq(load(data),0,'sentinel-only recording');eq(f('save',b'K_SONIC.BIN',base,12),0,'replace existing recording');eq((root/names[0]).read_bytes(),data,'atomic replacement bytes')
    largest=bytearray(131064);largest[-12:]=marker;eq(load(largest),0,'maximum recording')
    eq(f('save',b'K_BIG.BIN',base,len(largest)),0,'maximum disk save');eq((root/'K_BIG.BIN').read_bytes(),largest,'maximum disk contents')
    for name,ptr,size,error in [(b'../outside.BIN',base,12,1),(b'K_SONIC.bin',base,12,1),(None,base,12,1),(b'K_SONIC.BIN',base+12,12,1),(b'K_SONIC.BIN',base,-1,1),(b'K_SONIC.BIN',base,131076,1),(b'K_SONIC.BIN',base,13,1),(b'K_SONIC.BIN',base,12,5)]:
        eq(f('save',name,ptr,size),error,'invalid save');clear()
    eq(f('load_file',str(root/'absent.bin')),4,'missing file IO');eq(info().os_error!=0,True,'OS error reported');clear()
    malformed=root/'malformed.bin';malformed.write_bytes(bytes(12));before=C.string_at(base,48)
    eq(f('load_file',str(malformed)),5,'file missing sentinel');eq(C.string_at(base,48),before,'failed file load preserves old data');clear()
    malformed.write_bytes(marker+b'!');eq(f('load_file',str(malformed)),5,'partial file sample');clear()
    # A sharing violation must preserve the existing output and clean the
    # uncommitted temporary. Reopening succeeds after releasing the lock.
    eq(load(raw),0,'prepare locked save');target=root/names[0];prior=target.read_bytes()
    k=C.WinDLL('kernel32',use_last_error=True);k.CreateFileW.argtypes=[C.c_wchar_p,C.c_uint32,C.c_uint32,C.c_void_p,C.c_uint32,C.c_uint32,C.c_void_p];k.CreateFileW.restype=C.c_void_p;k.CloseHandle.argtypes=[C.c_void_p]
    handle=k.CreateFileW(str(target),0x80000000,1,None,3,0x80,None)
    assert handle not in [None,C.c_void_p(-1).value]
    try:eq(f('save',names[0].encode(),base,len(raw)),4,'replace locked file IO');eq(target.read_bytes(),prior,'failed replace preserves previous file');eq(list(root.glob('.jp-recording-*.tmp')),[],'failed temp cleaned')
    finally:k.CloseHandle(handle)
    clear();eq(f('save',names[0].encode(),base,len(raw)),0,'save after unlock');eq(target.read_bytes(),raw,'unlocked save bytes')
    eq(f('debug_cursor',base+10923*12),0,'native debugger cursor');i=info();eq(i.legacy_cursor,0x0cbe0000+10923*12,'guest debug address');eq(i.debug_count,1,'debug count')
    foreign=[]
    def other():
        for name,args in [('status',()),('begin_record',()),('load_memory',(None,0)),('load_file',(None,)),('save',(None,None,0)),('check_cursor',(base,0)),('shutdown',())]:foreign.append(f(name,*args))
        foreign.append(f('buffer'));f('clear_error')
    thread=threading.Thread(target=other);thread.start();thread.join();eq(foreign,[3]*7+[None],'foreign thread rejected');eq(f('status'),0,'foreign calls leave owner state intact')
    eq(list(root.glob('.jp-recording-*.tmp')),[],'no temporary remnants');eq((out/'outside.BIN').exists(),False,'path traversal rejected')
    eq(f('shutdown'),0,'shutdown');eq(f('status'),2,'post shutdown state');eq(f('initialize',str(root)),0,'reinitialize');eq(C.string_at(f('buffer'),capacity),bytes(capacity),'new owner zero storage');eq(info().save_count,0,'new owner counters');eq(f('shutdown'),0,'final shutdown')
    report=dict(status='authored_recording_storage_and_actual_Windows_file_effects_verified',checks=checks,build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(Path(__file__)),DLL_sha256=b['DLL_sha256'],all_original_record_cursor_slots=10922,all_original_play_cursor_slots=131073,actual_saved_names=names,unicode_directory=True,atomic_replacement_and_failure_preservation=True,loaded_extent_enforced=True,retail_data_used=False,original_recording_update_executed=False,original_pad_scan_executed=False,playable=False,overall_completion_percent=10)
    (out/'verification.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report),flush=True)
if __name__=='__main__':main()
