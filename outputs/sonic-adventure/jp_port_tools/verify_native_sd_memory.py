"""Original sound descriptor pool versus complete unhooked Japanese instructions."""
import argparse,ctypes as C,hashlib,json,re,struct,subprocess,sys
from pathlib import Path
from native_packet_test_support import bind
from model_reference import make_reference

P=C.c_void_p;U=C.c_uint32;I=C.c_int32;Q=C.c_uint64
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','build','binary','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();d=a.output.resolve();b=json.loads((a.build/'build_verification.json').read_bytes())
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    assert sha(b['DLL'])==b['DLL_sha256'] and sha(a.binary)=='e038db9029bc0ea8ffdb8ec8fc2e0690d1c324f5f7179f3dc58f56574b1aeeaf'
    for field,base in (('authored_source_sha256',r),('original_source_sha256',u)):
        for n,h in b[field].items():assert sha(base/n)==h,n
    # 32-bit pointer/long/enum layout probe; no retail data in this fixture.
    members=('memblk','memblk_max','memblk_num','transfer_num','transfer_end_num','transfer')
    layout_src=d/'layout32.c';layout_src.write_text('#include "shinobi/sd_work.h"\n_Static_assert(sizeof(void*)==4 && sizeof(long)==4,"guest-width layout");\nconst unsigned long layout[] = {0x53444c59,sizeof(struct sd_work),sizeof(struct sd_memblk),'+','.join('__builtin_offsetof(struct sd_work,'+x+')' for x in members)+'};\n')
    compiler=b['commands'][0]['command'][0];cmd=[compiler,'--target=i686-pc-windows-msvc','-std=c17','-O2','-Werror','-Wno-unknown-pragmas','-include',str(r/'outputs/sonic-adventure/native/jp_host_types.h'),'-I'+str(u/'sdk/include'),'-I'+str(u/'sdk'),'-c',str(layout_src),'-o',str(d/'layout32.obj')]
    run=subprocess.run(cmd,capture_output=True);(d/'layout_compile.txt').write_bytes(run.stdout+run.stderr);assert not run.returncode and not run.stdout and not run.stderr,(run.stdout+run.stderr).decode()
    obj=(d/'layout32.obj').read_bytes();machine,count,_,_,_,optional,_=struct.unpack_from('<HHIIIHH',obj);assert machine==0x14c;constants=None
    for j in range(count):
        at=20+optional+j*40;name=obj[at:at+8].rstrip(b'\0');size,offset=struct.unpack_from('<II',obj,at+16)
        if name==b'.rdata':constants=struct.unpack('<'+'I'*(size//4),obj[offset:offset+size])
    assert constants and constants[0]==0x53444c59 and constants[2]==32
    layout=dict(work_bytes=constants[1],block_bytes=constants[2],**dict(zip(members,constants[3:3+len(members)])))
    lib=C.CDLL(b['DLL']);initialize=bind(lib,'peer_initialize',I);table=bind(lib,'peer_table',P);count_=bind(lib,'peer_count',I);record=bind(lib,'peer_record',I,P,P);error=bind(lib,'peer_error',I);error_api=bind(lib,'peer_error_api',P)
    create=bind(lib,'sdMemBlkCreate',I,P);destroy=bind(lib,'sdMemBlkDestroy',I,P);setprm=bind(lib,'sdMemBlkSetPrm',I,P,P,I,P,P);execute=bind(lib,'sdMemBlkExecuteCallback',I,P);setstat=bind(lib,'sdMemBlkSetStat',I,P,I)
    check_labels=[]
    def check(ok,label):assert ok,label;check_labels.append(label)
    check(bind(lib,'peer_block_bytes',U)()==48,'host opaque descriptor and real native struct both48bytes')
    check(not any(hasattr(lib,n) for n in ('sdLibInit','sdDrvInit','sound_system_init','syG2Write','syG2DmaTrigger')),'pool fixture cannot supply false driver/init/hardware exports')
    sys.path.insert(0,str(r/'outputs/sonic-adventure'));import verify_startup as executor
    from verify_fpu_cleanup_transfer import Fpu
    ref,refhash=make_reference(executor);image=a.binary.read_bytes();ram=bytearray(16<<20);ram[0x10000:0x10000+len(image)]=image
    symbols={name:int(at,16) for at,name in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(u/'config/1st_read/symbols.txt').read_text(),re.M)};calls={}
    work=0x8cd00000;blocks=0x8ce00000;output=0x8cf10000;data=0x8cf11000;context=0x8cf12000
    def put(at,raw):at&=0xffffff;ram[at:at+len(raw)]=raw
    def word(at):return struct.unpack_from('<I',ram,at&0xffffff)[0]
    def original(name,*args):
        cpu=executor.State();cpu.pc=symbols[name];cpu.pr=0x8cfed000;cpu.r[15]=0x8cf00000;cpu.sr=0x70000000
        for j,value in enumerate(args[:4]):cpu.r[4+j]=value&0xffffffff
        for j,value in enumerate(args[4:]):put(cpu.r[15]+j*4,struct.pack('<I',value&0xffffffff))
        fpu=Fpu();fpu.fpscr=0x40001;ref(ram,cpu,fpu=fpu,instruction_budget=1000000);calls[name]=calls.get(name,0)+1
        check(cpu.pc==0x8cfed000 and cpu.r[15]==0x8cf00000,'complete unhooked original call returns with original stack');return C.c_int32(cpu.r[0]).value
    def pair_init():
        put(work,bytes(layout['work_bytes']));put(blocks,b'\xa5'*(64*32));put(symbols['sdgWrkPtr'],struct.pack('<I',work));put(symbols['sdgPrintFunc'],bytes(4));put(symbols['sdgDebugMode'],bytes(4))
        check(initialize()==original('sdMemBlkInit',blocks,64)==0,'original native and instruction paths initialize actual descriptor pools')
        check(word(work+layout['memblk'])==blocks and word(work+layout['memblk_max'])==64 and word(work+layout['memblk_num'])==count_()==0,'guest layout matches actual original writes and native live count')
    def rec(handle):
        values=(Q*8)();check(record(handle,values)==0,'native descriptor readback');return list(values)
    def guest(handle):return list(struct.unpack_from('<8I',ram,handle&0xffffff))
    pair_init();base=table();check(base>0xffffffff,'native descriptor pool actually above4GiB');handles=[]
    for j in range(64):
        h=P(0x1234);put(output,struct.pack('<I',0x1234));check(create(C.byref(h))==original('sdMemBlkCreate',output)==0,'original pool allocation succeeds')
        guest_handle=word(output);check(h.value==base+j*48 and guest_handle==blocks+j*32 and rec(h)==guest(guest_handle),'full-width native slot and original slot preserve all initialized fields');handles.append((h.value,guest_handle))
    h=P(0x1234);put(output,struct.pack('<I',0x1234));check(create(C.byref(h))==original('sdMemBlkCreate',output) and h.value is None and word(output)==0,'actual pool exhaustion clears outputs and agrees on original error')
    check(count_()==word(work+layout['memblk_num'])==64 and error_api()==C.cast(lib.sdMemBlkCreate,P).value and word(symbols['sdgErrApi'])==symbols['sdMemBlkCreate'],'actual original error trap records allocator API identity')
    native_data=C.create_string_buffer(b'driver-header'+bytes(116));native_context=C.create_string_buffer(b'callback-context');live=handles[0];ctx=C.addressof(native_context);src=C.addressof(native_data)
    check(setprm(live[0],src,128,0xffffffff,ctx)==original('sdMemBlkSetPrm',live[1],data,128,0xffffffff,context)==0,'parameter setter preserves real high pointers and sync sentinel')
    def comparable(n,g):
        n=list(n);n[2]=data if n[2]==src else n[2];n[5]=context if n[5]==ctx else n[5];return n==g
    check(comparable(rec(live[0]),guest(live[1])),'all descriptor parameters match after pointer relocation')
    check(execute(live[0])==original('sdMemBlkExecuteCallback',live[1])==0 and rec(live[0])[6]==guest(live[1])[6]==1,'synchronous completion marks actual descriptor done')
    before=rec(live[0]);check(destroy(live[0])==original('sdMemBlkDestroy',live[1])==0,'actual pool release agrees with original instructions');after=rec(live[0])
    check(after[0]==0 and after[1:]==before[1:] and comparable(after,guest(live[1])) and count_()==63,'release clears used/count but retains data, callback and every other field')
    # The released slot remains accessible to the original callback routine.
    check(setstat(live[0],before[1])==original('sdMemBlkSetStat',live[1],before[1])==0 and execute(live[0])==original('sdMemBlkExecuteCallback',live[1])==0,'original setters/callback do not treat pool release as heap free')
    again=P();put(output,bytes(4));check(create(C.byref(again))==original('sdMemBlkCreate',output)==0 and again.value==live[0] and word(output)==live[1],'next allocation really aliases the released outer driver descriptor')
    check(rec(again)==guest(live[1]) and rec(again)[2]==0,'slot reuse resets retained fields only when allocated again')
    check(destroy(again)==original('sdMemBlkDestroy',live[1])==0,'release reused descriptor');check(destroy(again)==original('sdMemBlkDestroy',live[1]) and error()!=0,'double release records original invalid-handle result')
    # Actual native callback receives an untruncated context pointer.
    peer=P();check(create(C.byref(peer))==0,'real callback test owns an actual native slot');seen=[]
    @C.CFUNCTYPE(None,P)
    def callback(value):seen.append(value)
    check(setprm(peer,src,128,C.cast(callback,P),ctx)==0 and execute(peer)==0 and seen==[ctx] and ctx>0xffffffff,'actual C callback dispatch retains native-width caller context')
    check(destroy(peer)==0,'release actual callback slot')
    result=dict(status='original_sound_descriptor_pool_native64_and_complete_unhooked_JP_calls_verified',checks=len(check_labels),labels=check_labels,original_complete_calls=sum(calls.values()),original_calls=calls,reference_hashes=refhash,
        original_instruction_function_hooks_used=False,guest_layout=layout,layout_compile_command=cmd,layout_probe_object_sha256=sha(d/'layout32.obj'),
        native_descriptor_bytes=48,guest_descriptor_bytes=32,released_pool_descriptor_data_retained=True,actual_reuse_alias_verified=True,full_native_callback_context_verified=True,
        test_fixture_seeds_controlled_work=True,sdDrvInit_full_repeated_execution_not_yet_verified=True,original_game_or_SDK_bootstrap_integrated=False,
        build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),original_binary_sha256=sha(a.binary),playable=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],result['checks'],'checks',result['original_complete_calls'],'original calls',flush=True)

if __name__=='__main__':main()
