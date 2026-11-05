"""Build/test real image files, atomic block writes and cross-process ownership.
Only fresh files under the specified workspace test-output directory are used.
"""
import argparse,ctypes as C,hashlib,json,random,subprocess,sys,threading
from pathlib import Path
U=C.c_uint32;I=C.c_int;P=C.c_void_p;S=C.c_size_t
class Info(C.Structure):_fields_=[(n,C.c_uint64) for n in ['reads','writes','created']]+[(n,U) for n in ['mask','status','os_error','reserved']]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def binding(dll):
    lib=C.CDLL(str(dll));api={}
    for name,result,args in [('initialize',I,[C.c_wchar_p]),('shutdown',I,[]),('status',I,[]),('clear_error',None,[]),('snapshot',I,[C.POINTER(Info)]),('create',I,[U]),('open',I,[U]),('close',I,[U]),('read',I,[U,U,P]),('write',I,[U,U,P])]:
        f=getattr(lib,'jp_vmu_store_'+name);f.restype=result;f.argtypes=args;api[name]=f
    return lib,api
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','baseline','output','probe-root','dll']:ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path)
    ap.add_argument('--probe-drive',type=int,default=0);a=ap.parse_args()
    if a.probe_root:
        lib,api=binding(a.dll);assert api['initialize'](str(a.probe_root))==0;rc=api['open'](a.probe_drive);data=C.create_string_buffer(512);read=api['read'](a.probe_drive,0,data) if not rc else None;result=dict(open=rc,read=read,first_block_sha256=hashlib.sha256(data.raw).hexdigest() if read==0 else None);assert api['shutdown']()==0;print(json.dumps(result));return
    assert a.workspace and a.baseline and a.output;r=a.workspace.resolve();out=a.output.resolve();assert not out.exists() and out.is_relative_to(r/'work/sonic-adventure') and not out.is_relative_to(r/'work/sonic-adventure/build');out.mkdir();root=out/'media';root.mkdir()
    b=json.loads((a.baseline/'build_verification.json').read_bytes())
    # This owner is host-only. The external SDK's errno/string headers are
    # intentionally partial and must not shadow the Windows CRT headers.
    common=[b['commands'][0]['command'][0],'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffreestanding','-fno-builtin','-Wall','-Wextra','-Werror']
    source=r/'outputs/sonic-adventure/native/jp_native_vmu_store.c';obj=out/'vmu_store.obj';dll=out/'jp_native_vmu_store.dll'
    commands=[common+['-c',str(source),'-o',str(obj)],[common[0],'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',str(obj),'-o',str(dll)]];results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=out/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));assert run.returncode==0 and not log.stat().st_size,log.read_text()
    lib,api=binding(dll);checks=0;rng=random.Random(0x564d5544)
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    def clear():api['clear_error']()
    def info():v=Info();check(api['snapshot'](C.byref(v))==0,'owner snapshot');return v
    def path(drive):return root/('VMU_'+chr(65+drive//2)+str(1+drive%2)+'.VMU')
    def probe():
        cmd=[sys.executable,'-B',str(Path(__file__).resolve()),'--probe-root',str(root),'--dll',str(dll),'--probe-drive','0'];run=subprocess.run(cmd,capture_output=True,creationflags=subprocess.CREATE_NO_WINDOW);check(run.returncode==0 and not run.stderr,'real companion process exits cleanly');return json.loads(run.stdout)
    sentinel=root/'SONICADV_JP.SAV';sentinel.write_bytes(b'EXISTING RECORD FORMAT MUST STAY UNCHANGED');sentinel_hash=sha(sentinel)
    buffer=C.create_string_buffer(b'\xa5'*576);check(api['read'](0,0,C.byref(buffer,32))==2 and buffer.raw[:-1]==b'\xa5'*576,'uninitialized read leaves output untouched')
    check(api['initialize'](str(root/'missing'))==1 and api['initialize'](str(root))==0,'directory validation and real owner')
    check(api['initialize'](str(root))==4,'duplicate initialize rejected')
    check(api['open'](0)==5 and not path(0).exists(),'open never silently creates media');clear()
    for drive in range(8):
        check(api['create'](drive)==0 and path(drive).read_bytes()==bytes(131072),'explicit real unformatted card creation')
        before=sha(path(drive));check(api['create'](drive)==9 and sha(path(drive))==before,'create never overwrites existing media');clear()
        check(api['open'](drive)==0,'actual card attached')
    check(info().mask==255 and info().created==8,'all eight slots independently attached')
    check(probe()['open']==7,'a real second process cannot acquire the active card')
    expected=bytearray(131072)
    for block in [0,17,255]:
        data=rng.randbytes(512);buf=C.create_string_buffer(data);check(api['write'](0,block,buf)==0,'actual flushed atomic block write');expected[block*512:(block+1)*512]=data
        check(path(0).read_bytes()==expected,'every persisted image byte matches independent block oracle')
        check(api['read'](0,block,C.byref(buffer,32))==0 and buffer.raw[:32]==buffer.raw[544:576]==b'\xa5'*32 and buffer.raw[32:544]==data,'bounded block read and both output canaries')
    for drive,index in [(8,0),(0,256),(0,0xffffffff)]:
        before=bytes(buffer);check(api['read'](drive,index,C.byref(buffer,32))==1 and bytes(buffer)==before,'invalid drive/block cannot alter caller output');clear()
    foreign=[]
    def wrong_thread():foreign.extend([api['read'](0,0,C.byref(buffer,32)),api['shutdown']()])
    thread=threading.Thread(target=wrong_thread);thread.start();thread.join();check(foreign==[3,3] and api['status']()==0 and info().mask==255,'foreign thread cannot access or shut down owner')
    # Permit readers but deny replacement, so failure occurs after staging and
    # flushing the real temporary file, not during the initial read.
    kernel=C.WinDLL('kernel32',use_last_error=True);create=kernel.CreateFileW;create.restype=P;create.argtypes=[C.c_wchar_p,U,U,P,U,U,P];close=kernel.CloseHandle;close.argtypes=[P];close.restype=I
    held=create(str(path(0)),0x80000000,1,None,3,0x80,None);check(held not in [None,P(-1).value],'real target reader denies replacement')
    changed=C.create_string_buffer(rng.randbytes(512));check(api['write'](0,1,changed)==5 and path(0).read_bytes()==expected,'failed replacement preserves prior file');check(not list(root.glob('.vmu-*.tmp')),'failed replacement cleans only its own temporary');clear()
    check(api['read'](0,1,C.byref(buffer,32))==0 and buffer.raw[32:544]==bytes(512),'failed replacement preserves cached card too');check(close(held)!=0,'test reader released')
    check(api['write'](0,1,changed)==0,'actual write retries after failure');expected[512:1024]=changed.raw[:512];check(path(0).read_bytes()==expected,'retry persisted correct complete image')
    check(api['close'](0)==0 and info().mask==254,'handoff releases only requested card')
    result=probe();check(result['open']==result['read']==0 and result['first_block_sha256']==hashlib.sha256(expected[:512]).hexdigest(),'new process reads persisted progress after handoff')
    check(api['open'](0)==0,'original owner can reacquire released card')
    external=bytearray(expected);external[5000]^=0xff;path(0).write_bytes(external)
    check(api['write'](0,2,changed)==4 and path(0).read_bytes()==external,'pre-existing external edits are not silently overwritten');clear()
    check(api['read'](0,9,C.byref(buffer,32))==0 and buffer.raw[32:544]==expected[4608:5120],'external-edit failure preserves owned cached view')
    check(api['close'](0)==0 and api['open'](0)==0 and api['read'](0,9,C.byref(buffer,32))==0 and buffer.raw[32:544]==external[4608:5120],'explicit reopen safely accepts externally changed image')
    check(api['close'](7)==0,'detach before malformed-image test');path(7).write_bytes(b'TRUNCATED')
    check(api['open'](7)==6 and path(7).read_bytes()==b'TRUNCATED' and info().mask==127,'malformed image rejects without altering it or other cards');clear()
    check(api['read'](7,0,C.byref(buffer,32))==4,'failed open leaves no attached fake card')
    check(api['shutdown']()==0 and api['shutdown']()==0,'cleanup works after sticky error and is idempotent')
    check(probe()['open']==0,'shutdown releases process ownership')
    check(sha(sentinel)==sentinel_hash,'existing native record-save file never touched');check(not list(root.glob('.vmu-*.tmp')),'no staging files left')
    result=dict(status='real_128KB_VMU_block_persistence_failure_atomicity_and_process_handoff_verified',checks=checks,commands=results,DLL=str(dll),DLL_sha256=sha(dll),owner_object_sha256=sha(obj),authored_source_sha256={str(p.relative_to(r)):sha(p) for p in [source,source.with_suffix('.h'),Path(__file__).resolve()]},test_root=str(root),eight_virtual_cards_tested=True,actual_second_process_lock_and_handoff_verified=True,atomic_block_replacement_failure_and_retry_verified=True,external_edit_detected=True,raw_save_record_file_preserved=True,test_images_only=True,user_save_files_used=False,formatted_VMU_filesystem_verified=False,buInit_card_init_or_VM_game_implemented=False,artifact_publishable=True,overall_completion_percent=10)
    (out/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],checks,'checks',flush=True)
if __name__=='__main__':main()
