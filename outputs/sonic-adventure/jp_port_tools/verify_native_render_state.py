"""Qualify actual SDK state setters, native scratch and original ClipZ values."""
import argparse,ctypes as C,hashlib,json,random,re,struct,sys
from pathlib import Path
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','build','binary','report']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';b=a.build
    sys.path.insert(0,str(p));from verify_startup import State,reference
    from verify_fpu_cleanup_transfer import Fpu
    historical=json.loads((p/'analysis/jp_ninja_math_v2_verification.json').read_text())
    assert sha(a.binary)==historical['original_binary_sha256']
    symbols=a.upstream/'config/1st_read/symbols.txt';assert sha(symbols)==historical['original_symbols_sha256']
    table={n:int(addr,16) for addr,n in re.findall(r'^(0x[0-9a-fA-F]+)\s+(\S+)',symbols.read_text(),re.M)}
    build=json.loads((b/'build_verification.json').read_text())
    assert build['status']=='native_render_state_linked_behavior_unverified'
    for n,h in build['authored_source_sha256'].items():assert sha(r/n)==h
    dll=b/'jp_native_render_state.dll';assert sha(dll)==build['dll_sha256'];lib=C.CDLL(str(dll))
    U=C.c_uint32;F=C.c_float;V=C.c_void_p;I=C.c_int
    def bind(n,result,*args):
        f=getattr(lib,n);f.restype=result;f.argtypes=list(args);return f
    init=bind('jp_render_state_initialize',I,U,C.c_size_t);shutdown=bind('jp_render_state_shutdown',None)
    status=bind('jp_render_state_status',I);clear=bind('jp_render_state_clear_error',None)
    scratch=bind('jp_render_state_scratch',V);allocated=bind('jp_render_state_allocated_bytes',C.c_size_t)
    capacity=bind('jp_render_state_capacity',U);guards=bind('jp_render_state_check_guards',I)
    reset=bind('jp_render_state_reset_scratch',I)
    configure=bind('jp_render_state_configure',I,U,U,F,F,F,F,F)
    functions={
        'njInit3D':bind('njInit3D',None,V,I),
        'njControl3D':bind('njControl3D',None,U),
        'njSetConstantAttr':bind('njSetConstantAttr',None,U,U),
        'njSetConstantMaterial':bind('njSetConstantMaterial',None,V),
        'njSetScreen':bind('njSetScreen',None,V),
        'njSetDepthQueue':bind('njSetDepthQueue',None,F,F),
        'njClipZ':bind('njClipZ',None,F,F)}
    globals={
        '_nj_screen_':20,'_nj_2d_clip_':16,'_nj_near_clip_':4,'_nj_far_clip_':4,
        '_nj_scr_aspect_x_':4,'_nj_scr_aspect_y_':4,'_nj_control_3d_flag_':4,
        '_nj_constant_attr_and_':4,'_nj_constant_attr_or_':4,'_nj_constant_material_':16,
        '_nj_vertex_num_':4,'_nj_calc_vertex_':4,'_nj_calc_polygon_':4,'_nj_draw_polygon_':4,
        '_nj_depth_near_':4,'_nj_depth_far_':4,'_nj_depth_range_':4}
    views={n:(C.c_ubyte*length).in_dll(lib,n) for n,length in globals.items()}
    vertex=V.in_dll(lib,'_nj_vertex_buf_')
    host_checks=0;comparisons=0;calls={};rng=random.Random(0x53544154)
    def check(value):
        nonlocal host_checks
        host_checks+=1;assert value,f'host render-state check {host_checks}'
    def same(value,why):
        nonlocal comparisons
        comparisons+=1;assert value,why
    check(status()==6);check(init(0,1024)==1);check(init(2049,1<<20)==1)
    check(init(4,127)==2);check(init(4,128)==0)
    ptr=scratch();check(ptr>0xffffffff and ptr%32==0)
    check(allocated()==128 and capacity()==4 and vertex.value==ptr)
    check(bytes(views['_nj_constant_material_'])==struct.pack('<4f',255,0,0,0))
    check(bytes(views['_nj_screen_'])==struct.pack('<5f',500,640,480,320,240))
    C.memset(ptr,0x33,64);check(reset()==0);check(C.string_at(ptr,64)==bytes(64))
    check(init(5,256)==2);clear();check(scratch()==ptr and allocated()==128)
    for offset in [-1,64]:
        guard=C.c_ubyte.from_address(ptr+offset);saved=guard.value;guard.value^=1
        check(guards()==5);clear();guard.value=saved;check(guards()==0)
    borrowed=(C.c_byte*64)();functions['njInit3D'](borrowed,4)
    check(status()==4 and vertex.value==ptr);clear()
    for width,height in [(640,480),(1920,1080),(7680,4320),(8192,4320)]:
        check(configure(width,height,500,1,60000,1,1)==0)
        check(bytes(views['_nj_screen_'])==struct.pack('<5f',500,width,height,width/2,height/2))
        check(bytes(views['_nj_2d_clip_'])==struct.pack('<4f',0,0,width-1,height-1))
    before={n:bytes(v) for n,v in views.items()}
    invalid=[(0,480,500,1,60000,1,1),(8193,480,500,1,60000,1,1),
        (640,480,float('nan'),1,60000,1,1),(640,480,500,2,1,1,1),
        (640,480,500,1,65536,1,1),(640,480,500,1,60000,0,1)]
    for values in invalid:
        check(configure(*values)==1);clear();check({n:bytes(v) for n,v in views.items()}==before)
    check(init(2048,1<<20)==0);check(capacity()==2048 and allocated()==2048*16+64)
    check(reset()==0 and guards()==0)
    ram=bytearray(16<<20);binary=a.binary.read_bytes();ram[0x10000:0x10000+len(binary)]=binary
    BASE=0x8c000000;GUEST=0x8cd00040;ARG=0x8cd10000
    def store(address,data):ram[address-BASE:address-BASE+len(data)]=data
    def read(address,count):return bytes(ram[address-BASE:address-BASE+count])
    def original_call(name,ints=(),floats=()):
        s=State();s.r[:]=[0x12340000+i for i in range(16)]
        for i,v in enumerate(ints):s.r[i+4]=v&0xffffffff
        s.r[15]=0x8cf00000;s.pc=table[name];s.pr=0x8cfed000;s.sr=0x70000000
        f=Fpu();f.fpscr=0x40000
        for i,bits in enumerate(floats):f.banks[0][4+i]=bits
        reference(ram,s,fpu=f,instruction_budget=2000)
        calls[name]=calls.get(name,0)+1
    def seed():
        for name,v in views.items():store(table[name],bytes(v))
        store(table['_nj_vertex_buf_'],struct.pack('<I',GUEST))
    def compare(name):
        for n,v in views.items():same(bytes(v)==read(table[n],len(v)),name+' '+n)
        same(vertex.value==scratch() and read(table['_nj_vertex_buf_'],4)==struct.pack('<I',GUEST),name+' owned pointer domain')
    def float_bits(v):return struct.unpack('<I',struct.pack('<f',v))[0]
    special=[0x00000000,0x80000000,0x3f800000,0xbf800000,0xc77fff00,0xc7800000,
             0x7f800000,0xff800000,0x7fc12345,0xffc54321,0x00000001,0x80000001]
    for iteration in range(96):
        seed();flags=rng.getrandbits(32);original_call('njControl3D',[flags]);functions['njControl3D'](flags);compare('control')
        seed();a0,a1=rng.getrandbits(32),rng.getrandbits(32)
        original_call('njSetConstantAttr',[a0,a1]);functions['njSetConstantAttr'](a0,a1);compare('attributes')
        values=[rng.getrandbits(32) for _ in range(4)]
        data=struct.pack('<4I',*values);native=(U*4)(*values);store(ARG,data);seed()
        original_call('njSetConstantMaterial',[ARG]);functions['njSetConstantMaterial'](native);compare('material bits')
        data=struct.pack('<5f',rng.uniform(50,2000),-17,-19,rng.uniform(0,1000),rng.uniform(0,1000))
        store(ARG,data);native=(C.c_ubyte*20).from_buffer_copy(data);seed()
        original_call('njSetScreen',[ARG]);functions['njSetScreen'](native);compare('screen fields')
        floats=[float_bits(-rng.uniform(1,100)),float_bits(-rng.uniform(1000,60000))];seed()
        original_call('njSetDepthQueue',floats=floats)
        functions['njSetDepthQueue'](*(F.from_buffer_copy(struct.pack('<I',v)) for v in floats));compare('depth queue')
        floats=[special[iteration%len(special)],special[(iteration*5+3)%len(special)]];seed()
        original_call('njClipZ',floats=floats)
        functions['njClipZ'](*(F.from_buffer_copy(struct.pack('<I',v)) for v in floats));compare('clip bits')
        seed();original_call('njInit3D',[GUEST,2048]);functions['njInit3D'](scratch(),2048);compare('initialize')
        check(status()==0 and guards()==0)
    shutdown();check(status()==6 and scratch() is None and allocated()==0)
    report=dict(status='native_render_state_and_Japanese_setter_contracts_verified',host_checks=host_checks,
        Japanese_contract_comparisons=comparisons,original_calls=sum(calls.values()),calls_by_function=calls,
        screen_state_configurations=[[640,480],[1920,1080],[7680,4320],[8192,4320]],
        high_resolution_image_rendering_verified=False,maximum_model_points=2048,
        dll_sha256=sha(dll),build_report_sha256=sha(b/'build_verification.json'),verifier_sha256=sha(Path(__file__)),
        original_binary_sha256=sha(a.binary),original_symbols_sha256=sha(symbols),
        historical_executor_sha256=sha(p/'verify_startup.py'),historical_executor_modified=False,
        numeric_scope='SDK state values; FPSCR nearest/DN1; no physical timing, flags or full-game floating-point claim',
        complete_renderer_linked_or_executed=False,gameplay_verified=False,
        overall_completion_percent=10,goal_paused=False)
    a.report.write_text(json.dumps(report,indent=2)+'\n')
    print(report['status'],host_checks,'host checks;',comparisons,'comparisons;',sum(calls.values()),'original calls')
if __name__=='__main__':main()
