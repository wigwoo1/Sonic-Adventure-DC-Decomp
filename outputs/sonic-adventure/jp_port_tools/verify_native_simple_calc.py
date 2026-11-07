"""Compare helpers and complete model projection with whole original execution."""
import argparse,ctypes as C,hashlib,json,random,re,struct,sys
from pathlib import Path
from native_packet_test_support import bind
from verify_native_simple_projection import MMIO,Queues
from verify_native_model_preflight import Model,Vec
U,I,P,F,S=C.c_uint32,C.c_int32,C.c_void_p,C.c_float,C.c_size_t
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','build','binary','numeric-dll','output'):
        ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    b=read(a.build/'build_verification.json');pin=read(p/'jp_port_tools/jp_view_stack_external_inputs.json')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h
    assert sha(a.binary)==pin['original_binary_sha256'] and sha(a.numeric_dll)==pin['numerical_model_DLL_sha256']
    f=C.CDLL(b['frames_DLL'])
    sys.path.insert(0,str(p));import verify_startup as executor
    from model_reference import make_reference
    from verify_fpu_cleanup_transfer import Fpu
    reference,hashes=make_reference(executor)
    numeric=bind(C.CDLL(str(a.numeric_dll.resolve())),'sa_sh4_vector_numeric',I,U,P,U)
    def service(op,state):assert numeric(op,C.byref(state),state.fpul)==0
    symbols={n:int(at,16) for at,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    initial=bytearray(16<<20);raw=a.binary.read_bytes();initial[0x10000:0x10000+len(raw)]=raw
    def put(ram,at,data):ram[at&0xffffff:(at&0xffffff)+len(data)]=data
    def word(ram,at):return struct.unpack_from('<I',ram,at&0xffffff)[0]
    def original(ram,name,matrix,bank,args,fargs=(),mmio=None,pref=None):
        fp=Fpu();fp.fpscr=0x40001|(bank<<21);fp.banks[bank^1][:]=struct.unpack('<16I',struct.pack('<16f',*matrix))
        for i,x in enumerate(fargs):fp.banks[bank][4+i]=struct.unpack('<I',struct.pack('<f',x))[0]
        state=executor.State();state.pc=symbols[name];state.pr=0x8cfed000;state.r[15]=0x8cf00000;state.sr=0x70000000
        for i,x in enumerate(args):state.r[4+i]=x
        reference(ram,state,fpu=fp,mmio=mmio,vector_instruction=service,
                  special={'gbr':0,'r_bank':[0]*8},cache_prefetch=pref,
                  cache_invalidate=lambda at:None,instruction_budget=1000000)
        return state
    checks=0;helper_records=[];model_records=[]
    def check(ok,label):
        nonlocal checks
        assert ok,label;checks+=1
    matrices=[(1.,0,0,0,0,1.,0,0,0,0,1.,0,0,0,0,1.),
              (.75,.5,0,0,-.5,.75,0,0,0,0,1.,0,2.,-3.,4.,1.)]
    stack=C.create_string_buffer(128*64);bind(f,'njInitMatrix',None,P,I,I)(stack,128,0)
    for name in ('SimpleCalcModelMatrix','SimpleCalcModelLight'):
      for bank in (0,1):
       for mi,matrix in enumerate(matrices):
        for value in ((1.,2.,3.),(-2.,.25,8.),(1e-20,1e-20,1e-20),(0.,0.,0.)):
         for scale in ((.5,1.,1.5) if name.endswith('Light') else (0.,)):
          for alias in (False,True):
            ram=bytearray(initial);source=0x8cd00000;target=source if alias else source+64
            put(ram,source-16,b'\xa5'*128);put(ram,source,struct.pack('<3f',*value))
            state=original(ram,name,matrix,bank,[target,source],(scale,) if name.endswith('Light') else ())
            storage=C.create_string_buffer(b'\xa5'*128,128);ptr=C.addressof(storage)+16
            C.memmove(ptr,struct.pack('<3f',*value),12);out=ptr if alias else ptr+64
            bind(f,'njSetMatrix',None,P,P)(None,(F*16)(*matrix))
            fn=bind(f,name,None,P,P,F) if name.endswith('Light') else bind(f,name,None,P,P)
            if name.endswith('Light'):fn(out,ptr,scale)
            else:fn(out,ptr)
            expected=bytes(ram[(source-16)&0xffffff:((source-16)&0xffffff)+128])
            record=dict(function=name,bank=bank,matrix=mi,value=value,scale=scale,alias=alias,
                        expected=bytes(ram[target&0xffffff:(target&0xffffff)+12]).hex(),
                        actual=C.string_at(out,12).hex())
            (d/'last_helper.json').write_text(json.dumps(record,indent=2)+'\n')
            check(state.r[15]==0x8cf00000,'whole original helper returns')
            check(bytes(storage)==expected,record)
            helper_records.append(record)
    # Literal locations in the pinned full function identify its private BSS.
    private={'clip_or':0x8c8c2568,'direction':0x8c8c2570,'scale':0x8c8c2588,
             'projection_x':0x8c8c2594,'projection_y':0x8c8c2598}
    assert word(initial,0x8c62bf8c)==private['clip_or'] and word(initial,0x8c62bfa0)==private['projection_x']
    assert word(initial,0x8c62bfa8)==private['projection_y'] and word(initial,0x8c62c0c4)==private['scale']
    assert word(initial,0x8c62c0c8)==private['direction']
    init=bind(f,'jp_simple_calc_initialize',I,U,S);close=bind(f,'jp_simple_calc_shutdown',I)
    guards=bind(f,'jp_simple_calc_guards',I);records=bind(f,'jp_simple_calc_records',P)
    queueptr=bind(f,'jp_simple_calc_queues',P)();queues=Queues.from_address(queueptr)
    calc=bind(f,'jp_simple_calc_model',I,P,S,S,P,F,P)
    observe=bind(f,'jp_simple_calc_context_snapshot',None,P)
    check(init(32,1088)==0 and guards()==0,'independent guarded32-byte scratch owner initializes')
    address=records();check(address>0xffffffff and not address&31,'full-width aligned Simple scratch allocation')
    check(init(64,32)==2 and records()==address and guards()==0,'budget refusal preserves existing scratch owner')
    screen=(F*5).in_dll(f,'_nj_screen_');screen[:]=[256,640,480,320,240]
    clip=(F*4).in_dll(f,'_nj_2d_clip_');clip[:]=[0,0,640,480]
    F.in_dll(f,'_nj_near_clip_').value=1;F.in_dll(f,'_nj_far_clip_').value=1000
    F.in_dll(f,'port_draw_aspect').value=F.in_dll(f,'_nj_scr_aspect_x_').value=1
    F.in_dll(f,'_nj_scr_aspect_y_').value=-1
    vertex=P.in_dll(f,'_nj_vertex_buf_');vertex_count=I.in_dll(f,'_nj_vertex_num_')
    counter=I.in_dll(f,'_nj_calc_vertex_');control=U.in_dll(f,'_nj_control_3d_flag_')
    borrowed=C.create_string_buffer(b'\x6d'*128,128);vertex.value=C.addressof(borrowed);vertex_count.value=7
    light=Vec(.5,-1,.75);rng=random.Random(0x43414c43)
    mixed=[(0.,0.,0.),(-4.,2.,2.),(1.,-2.,12.),(0.,0.,1001.)]*4+[(0.,0.,2.)]
    for bank in (0,1):
     for mi,matrix in enumerate(matrices):
      for mode in (0,2,256,258):
       for normals in (False,True):
        for scenario in ('mixed','near','far','left'):
         for count in (0,1,4,16):
            values=mixed if scenario=='mixed' else [dict(near=(0.,0.,.25),far=(0.,0.,1500.),left=(-1000.,0.,2.))[scenario]]*17
            center=(0.,0.,12.) if scenario=='mixed' else values[0];radius=2000. if scenario=='mixed' else .01
            pts=(Vec*17)(*[Vec(*v) for v in values]);nrm=(Vec*17)(*[Vec(0.,1.,0.) for _ in values])
            model=Model();model.points=pts;model.normals=nrm if normals else None;model.point_count=count
            model.center=Vec(*center);model.radius=radius
            model_before=bytes(model);points_before=bytes(pts);normals_before=bytes(nrm)
            seeds=[rng.getrandbits(32) for _ in range(16)];queues.words[:]=seeds
            C.memset(address,0xa5,32*32);counter.value=17;control.value=mode
            prior=(U*6)();observe(prior)
            ram=bytearray(initial);output=0x8cd10000+((address>>5)&1)*32
            put(ram,output,b'\xa5'*(32*32));put(ram,0x8cd02000,bytes(pts));put(ram,0x8cd03000,bytes(nrm))
            put(ram,0x8cd00000,struct.pack('<IIiIIHH4f',0x8cd02000,0x8cd03000 if normals else 0,count,0,0,0,0,*center,radius))
            for name,data in [('_nj_screen_',bytes(screen)),('_nj_2d_clip_',bytes(clip)),
                ('_nj_near_clip_',struct.pack('<f',1)),('_nj_far_clip_',struct.pack('<f',1000)),
                ('_nj_scr_aspect_x_',struct.pack('<f',1)),('_nj_scr_aspect_y_',struct.pack('<f',-1)),
                ('_nj_control_3d_flag_',struct.pack('<I',mode)),('_nj_vertex_buf_',struct.pack('<I',output)),
                ('_nj_calc_vertex_',struct.pack('<I',17)),('_nj_current_matrix_ptr_',struct.pack('<I',0x8ce40000))]:
                put(ram,symbols[name],data)
            put(ram,private['direction'],bytes(light));put(ram,private['scale'],struct.pack('<f',.8))
            put(ram,symbols['light2'],bytes(prior)[4:16]);put(ram,0x8ce40000,struct.pack('<16f',*matrix))
            mmio=MMIO({0xff000038:0,0xff00003c:0})
            for i,value in enumerate(seeds):mmio[i*4]=value
            def pref(at):
                if at&0xfc000000==0xe0000000:
                    start=at&~31;target=(start&0x3ffffff)|0x8c000000
                    put(ram,target,struct.pack('<8I',*[mmio[start+j*4] for j in range(8)]))
            state=original(ram,'_njSimpleCalcModel',matrix,bank,[0x8cd00000],mmio=mmio,pref=pref)
            bind(f,'njSetMatrix',None,P,P)(None,(F*16)(*matrix))
            result=I(123);status=calc(C.byref(model),count,count,C.byref(light),.8,C.byref(result))
            snapshot=(U*6)();observe(snapshot)
            expected_snapshot=[word(ram,private['clip_or'])]+list(struct.unpack_from('<3I',ram,symbols['light2']&0xffffff))+[word(ram,private['projection_x']),word(ram,private['projection_y'])]
            expected=bytes(ram[output&0xffffff:(output&0xffffff)+32*32]);actual=C.string_at(address,32*32)
            record=dict(bank=bank,matrix=mi,control=mode,normals=normals,scenario=scenario,count=count,
                expected_result=state.r[0],actual_result=result.value&0xffffffff,
                expected_snapshot=expected_snapshot,actual_snapshot=list(snapshot),
                expected_sha256=hashlib.sha256(expected).hexdigest(),actual_sha256=hashlib.sha256(actual).hexdigest())
            (d/'last_model.json').write_text(json.dumps(record,indent=2)+'\n')
            check(status==0 and state.r[15]==0x8cf00000 and result.value&0xffffffff==state.r[0],record)
            check(actual==expected and list(snapshot)==expected_snapshot,record)
            check(list(queues.words)==[mmio[i*4] for i in range(16)],'both model-projection queue lines match')
            check(counter.value==word(ram,symbols['_nj_calc_vertex_']),'shared vertex counter matches including early sphere rejection')
            check(vertex.value==C.addressof(borrowed) and vertex_count.value==7 and bytes(borrowed)==b'\x6d'*128,
                  'borrowed16-byte renderer buffer pointer/count/data fully restored')
            check(guards()==0 and bytes(model)==model_before and bytes(pts)==points_before and bytes(nrm)==normals_before,
                  'scratch guards and all input model/geometry bytes preserved')
            model_records.append(record)
    for case in ('negative','capacity','points','normals','model_null','light_null','result_null'):
        model.point_count=-1 if case=='negative' else 33 if case=='capacity' else 1
        model.normals=nrm if case=='normals' else None
        out=I(123);before=C.string_at(address,32*32),bytes(queues),counter.value
        status=calc(None if case=='model_null' else C.byref(model),
                    0 if case=='points' else 33,0,
                    None if case=='light_null' else C.byref(light),.8,
                    None if case=='result_null' else C.byref(out))
        check(status==(2 if case=='capacity' else 1) and out.value==123 and
              before==(C.string_at(address,32*32),bytes(queues),counter.value) and
              vertex.value==C.addressof(borrowed) and vertex_count.value==7,
              'invalid model/span/capacity/null arguments preserve scratch,queues,counter,borrowed globals and result')
    suffix=address+32*32;C.c_uint8.from_address(suffix).value=0
    model.point_count=1;model.normals=None;out=I(123)
    check(guards()==5 and calc(C.byref(model),1,0,C.byref(light),.8,C.byref(out))==5 and out.value==123,
          'actual scratch guard corruption prevents model execution')
    C.c_uint8.from_address(suffix).value=0x5a
    check(guards()==0,'restored diagnostic guard is accepted')
    check(close()==0 and not records() and close()==0,'32-byte scratch owner closes cleanly and repeatedly')
    out=dict(status='sampled_original_matrix_light_helpers_and_full_Simple_model_projection_match',
        checks=checks,helper_calls=len(helper_records),model_calls=len(model_records),
        helper_records=helper_records,model_records=model_records,private_original_BSS=private,
        independent32_byte_scratch_and_borrowed_globals_restoration_verified=True,
        original_game_function_hooks_used=False,full_Simple_material_mesh_or_aura_rendering_verified=False,
        reference_hashes=hashes,verifier_sha256=sha(__file__),
        build_verification_sha256=sha(a.build/'build_verification.json'),playable=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(out,indent=2)+'\n')
    print(out['status'],checks,'checks',len(helper_records),'helper calls',len(model_records),'whole model calls',flush=True)
if __name__=='__main__':main()
