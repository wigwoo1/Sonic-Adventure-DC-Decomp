"""Compare native initialization state/percent bits with original JP SH-4 code.
Guest platform allocation/config/vblank delivery are explicit harness services;
the original initialization, floating operations, njBlank and njCalcTexture
bodies execute. This is scalar/lifecycle qualification, not hardware emulation.
"""
import argparse,ctypes as C,hashlib,json,random,re,struct,sys
from pathlib import Path
from native_packet_test_support import bind
from native_texture_test_support import Runtime
from native_clock_test_support import Clock,Counter
from model_reference import make_reference
U=C.c_uint32;P=C.c_void_p;I=C.c_int
class Config(C.Structure):
    _fields_=[('size',U),('flags',U),('surfaces',P)]+[(n,U) for n in ['frames','width','height','bpp','texture']]+[('descriptor',P),('vertex',P),('words',U),('shares',U*5),('reserved',U*7),('model',U)]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','build','binary','gpu','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=json.loads((a.build/'verification.json').read_bytes());assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    assert sha(b['DLL'])==b['DLL_sha256'];assert sha(r/'outputs/sonic-adventure/native/jp_native_vertex_lifecycle.c')==b['source_sha256']
    binary=a.binary.read_bytes();assert hashlib.sha256(binary).hexdigest()=='e038db9029bc0ea8ffdb8ec8fc2e0690d1c324f5f7179f3dc58f56574b1aeeaf'
    t=Runtime(a.build/'jp_native_frames.dll');C.CDLL(str(a.gpu.resolve()));clock=Clock(a.build/'jp_native_present.dll');counter=Counter();assert counter.start(clock,781250)==0
    Idle=C.CFUNCTYPE(I,P,U)
    @Idle
    def idle(ctx,ms):counter.tick+=(ms*781250+999)//1000;return 1
    assert bind(clock.lib,'jp_wait_initialize',I,U,U,U,P,P)(60,1,7,idle,None)==0
    lib=C.CDLL(b['DLL']);native=bind(lib,'jp_vertex_lifecycle_initialize',I,*([C.c_int32]*5));cancel=bind(clock.lib,'jp_vblank_cancel_ninja_blank',I);qi=bind(t.dll,'jp_queue_initialize',I,C.c_size_t)
    assert t.init(8,16,64<<20)==0
    config=Config.in_dll(lib,'_nj_SystemConfigStruct_');assert Config.shares.offset==60 and C.sizeof(Config)==112
    names=['_nj_vertex_buffer_'+n+'_' for n in ['op','om','tp','tm','pt','size']]+['_nj_vertex_2v_type_']
    sys.path.insert(0,str(r/'outputs/sonic-adventure'));import verify_startup as executor
    from verify_fpu_cleanup_transfer import Fpu
    reference,refhash=make_reference(executor);table={n:int(at,16) for at,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    ram=bytearray(16<<20);ram[0x10000:0x10000+len(binary)]=binary
    def put(at,raw):at&=0xffffff;ram[at:at+len(raw)]=raw
    def read(at,n):return bytes(ram[at&0xffffff:(at&0xffffff)+n])
    def word(at):return struct.unpack('<I',read(at,4))[0]
    def iw(at,n):put(at,struct.pack('<I',n&0xffffffff))
    checks=0;calls=0;blank_calls=0;service_calls={};ticks=0;armed=False;guest_config=[];entered={};available=64*1024*1024-t.allocated()
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    def service(name):service_calls[name]=service_calls.get(name,0)+1
    def malloc(ram,regs,sr):service('syMalloc');regs[0]=0x8c800000;return sr
    def free(ram,regs,sr):service('syFree');return sr
    def arm(ram,regs,sr):
        nonlocal armed
        service('syChainAddHandler');check(regs[4]==0x900 and regs[5]==table['njBlank'] and regs[6]==88 and regs[7]==0,'original blank registration arguments');armed=True;regs[0]=1;return sr
    def configure(ram,regs,sr):service('kmSetSystemConfiguration');guest_config.append(read(regs[4],96));regs[0]=0;return sr
    def texture(ram,regs,sr):service('kmGetFreeTextureMem');iw(regs[4],available);iw(regs[5],available);regs[0]=0;return sr
    hooks={table[n]:fn for n,fn in [('syMalloc',malloc),('syFree',free),('syChainAddHandler',arm),('kmSetSystemConfiguration',configure),('kmGetFreeTextureMem',texture)]}
    def trace(pc,*unused):
        nonlocal ticks,blank_calls
        for n in ['njInitVertexBuffer','njCalcTexture']:
            if pc==table[n]:entered[n]=entered.get(n,0)+1
        ticks+=1
        if armed and ticks%128==0:
            s=executor.State();s.pc=table['njBlank'];s.pr=0x8cfed000;s.r[15]=0x8cf10000;s.sr=0x70000000
            f=Fpu();f.fpscr=0x40001;reference(ram,s,fpu=f,instruction_budget=1000);blank_calls+=1
    rng=random.Random(0x564c4946)
    cases=[(-1,0,131072,0,32768),(100,0,100,0,0),(1,2,-1,4,5),(1,2,3,4,-1),(-1,3,5,7,11)]
    cases += [tuple(rng.randrange(16384) for _ in range(5)) for _ in range(120)]
    cases += [(n,1,1,0,0) for n in [(1<<24)-1,1<<24,(1<<24)+1,(1<<24)+3,(1<<25)+1]]
    for args in cases:
        check(cancel()==0 and qi(512<<20)==0,'empty actual queue and blank before case')
        C.c_int.in_dll(clock.lib,'_nj_render_flag_').value=0
        C.memset(C.addressof(config),0xa5,C.sizeof(config));config.vertex=None
        put(table['_nj_SystemConfigStruct_'],b'\xa5'*96);iw(table['_nj_SystemConfigStruct_']+36,0);iw(table['_nj_usr_vertex_buffer_ptr_'],0);iw(table['_nj_render_flag_'],0);iw(table['_nj_blank_count_'],0)
        armed=False;ticks=0;guest_config.clear()
        state=executor.State();state.pc=table['njInitVertexBuffer'];state.pr=0x8cfed000;state.sr=0x70000000;state.r[15]=0x8cf00000
        for i,n in enumerate(args[:4]):state.r[4+i]=n&0xffffffff
        iw(state.r[15],args[4]);fpu=Fpu();fpu.fpscr=0x40001
        reference(ram,state,fpu=fpu,hooks=hooks,trace=trace,instruction_budget=20000);calls+=1
        check(state.r[15]==0x8cf00000 and len(guest_config)==1,'whole original entry returns through explicit config service')
        check(native(*args)==0,'real native initialization completes')
        for n in names:check(U.in_dll(lib,n).value==word(table[n]),('whole original scalar state',n,args))
        raw=guest_config[0]
        check(config.flags==struct.unpack_from('<I',raw,4)[0] and config.model==struct.unpack_from('<I',raw,92)[0],'original flags and model')
        check(config.words==struct.unpack_from('<I',raw,40)[0] and config.vertex is None,'original word count; documented native pointer replacement')
        check(bytes(config.shares)==raw[44:64],('all five exact original percentage bits',args,bytes(config.shares).hex(),raw[44:64].hex()))
        check(U.in_dll(t.dll,'_nj_tex_max_free_size').value==word(table['_nj_tex_max_free_size'])==available,'original final texture query/cache')
        check(config.size==config.frames==config.width==config.height==config.bpp==config.texture==0xa5a5a5a5 and list(config.reserved)==[0xa5a5a5a5]*7,'unrelated native config scalars preserved')
    check(entered=={'njInitVertexBuffer':len(cases),'njCalcTexture':len(cases)},'original bodies entered on every case')
    cancel();bind(clock.lib,'jp_wait_shutdown',I)();clock.shutdown();t.shutdown();bind(t.dll,'jp_queue_shutdown',None)()
    result=dict(status='original_JP_vertex_initialization_scalar_and_percentage_bits_verified',checks=checks,original_entry_calls=calls,actual_original_njBlank_calls=blank_calls,original_bodies_entered=entered,guest_platform_service_calls=service_calls,guest_platform_services_hooked=sorted(service_calls),original_initialization_or_float_operations_hooked=False,native_functions_replaced=False,cases=len(cases),large_integer_RZ_cases=5,reference_hashes=refhash,original_binary_sha256=sha(a.binary),build_proof_sha256=sha(a.build/'verification.json'),verifier_sha256=sha(__file__),native_pointer_and_allocator_adaptation_explicit=True,original_caller_supplied_buffer_branch_verified=False,full_hardware_configuration_or_gameplay_verified=False,playable=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],checks,'checks',calls,'original calls',flush=True)
if __name__=='__main__':main()
