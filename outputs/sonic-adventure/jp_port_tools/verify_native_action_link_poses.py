"""Whole original POS/ANG action-link execution: sampled genuine Sonic poses."""
import argparse,ctypes as C,hashlib,json,re,struct,sys
from pathlib import Path
from native_packet_test_support import bind
from verify_native_motion_data import Entry
from verify_native_motion_store import View
from verify_native_character_tree import Node
from native_texture_test_support import Runtime
from native_video_test_support import Video
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
class Action(C.Structure):_fields_=[('object',C.c_void_p),('motion',C.c_void_p)]
class Motion(C.Structure):_fields_=[('data',C.c_void_p),('frames',C.c_uint32),('type',C.c_uint16),('inputs',C.c_uint16)]
class MotionLink(C.Structure):_fields_=[('motions',C.c_void_p*2),('frames',C.c_float*2)]
class ActionLink(C.Structure):_fields_=[('object',C.c_void_p),('link',C.c_void_p)]
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','build','binary','numeric-dll','motion-inputs','output'):ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();b=read(a.build/'build_verification.json');assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h
    for key,value in b.items():
        if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and value.endswith('.dll'):assert sha(value)==b[key+'_sha256']
    for path,h in b['action_link_kept_frame_objects_sha256'].items():assert sha(path)==h
    pin=read(p/'jp_port_tools/jp_view_stack_external_inputs.json');assert sha(a.binary)==pin['original_binary_sha256'] and sha(a.numeric_dll)==pin['numerical_model_DLL_sha256']
    iv=read(a.motion_inputs/'inputs.json');bank=a.motion_inputs/'s_mot/content.bin';assert sha(bank)==iv['files']['s_mot/bin']['sha256']
    sys.path.insert(0,str(p));import verify_startup as executor
    from verify_fpu_cleanup_transfer import Fpu
    from model_reference import make_reference
    reference,reference_hashes=make_reference(executor);U,I,P,S,F,Q=C.c_uint32,C.c_int32,C.c_void_p,C.c_size_t,C.c_float,C.c_uint64
    f=C.CDLL(b['frames_DLL']);motions=C.CDLL(b['motion_DLL']);numeric=bind(C.CDLL(str(a.numeric_dll.resolve())),'sa_sh4_vector_numeric',I,U,P,U)
    def service(op,state):assert numeric(op,C.byref(state),state.fpul)==0
    checks=0;records=[]
    def check(ok,label):
        nonlocal checks
        assert ok,label;checks+=1
    textures=Runtime(Path(b['frames_DLL']));video=Video(f)
    check(bind(f,'jp_queue_initialize',I,S)(8<<20)==0 and bind(f,'jp_frames_initialize',I,U,U,S)(4,128,16<<20)==0 and textures.init(8,128,1<<20)==0 and video.init(64,64,0xff102030,1)==0 and bind(f,'jp_render_state_initialize',I,U,S)(2048,16<<20)==0 and bind(f,'jp_ninja_contexts_initialize',I)()==0,'actual original action queue/frame/texture/video/scratch/context prerequisites')
    check(bind(motions,'jp_motions_initialize',I,S,U)(8<<20,16)==0 and bind(motions,'jp_motions_select',I,I)(0)==0,'same actual original Sonic motion owner initialized')
    view=View();check(bind(motions,'jp_motions_acquire',I,U,C.c_char_p,P)(0,b'sonic_updraft_action',C.byref(view))==0,'genuine original Sonic action view retained');action=Action.from_address(view.data);motion=Motion.from_address(action.motion)
    check(motion.frames==33 and motion.type==3 and motion.inputs==2,'source-backed genuine motion extent and POS/ANG factors')
    second_view=View();check(bind(motions,'jp_motions_acquire',I,U,C.c_char_p,P)(7,b'sonic_full_speed_action',C.byref(second_view))==0,'resident bank7 full-speed action retained alongside bank0 updraft');second_action=Action.from_address(second_view.data);check(Motion.from_address(second_action.motion).frames==33,'source-backed second motion extent')
    entries=[];action_address=None
    for name in ('s_mot','models'):
        get=bind(motions,'jp_motion_'+name+'_entry',I,U,P)
        for i in range(bind(motions,'jp_motion_'+name+'_count',U)()):
            e=Entry();assert get(i,C.byref(e))==0;entries.append(e)
            if e.name==b'sonic_updraft_action':action_address=e.address
    def original_address(ptr):
        e=next(e for e in entries if e.data<=ptr<e.data+e.host_bytes);assert (ptr-e.data)%e.host_element==0;return e.address+(ptr-e.data)//e.host_element*e.original_element
    table={n:int(at,16) for at,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    initial=bytearray(16<<20);raw=a.binary.read_bytes();initial[0x10000:0x10000+len(raw)]=raw;rawbank=bank.read_bytes();initial[0xc00000:0xc00000+len(rawbank)]=rawbank
    def put(ram,address,raw):at=address&0xffffff;ram[at:at+len(raw)]=raw
    def iw(ram,address,values):put(ram,address,struct.pack('<'+'I'*len(values),*values))
    matrix=(U*16)(*[0x3f800000 if i%5==0 else 0 for i in range(16)]);stack=(C.c_uint8*(128*64))();observe=C.CFUNCTYPE(None,P);callback_pointer=P.in_dll(f,'_nj_motion_callback_');previous=callback_pointer.value;native=[];callback_errors=[]
    @observe
    def callback(ptr):
        try:
            node=Node.from_address(ptr)
            if node.model and not node.flags&8:
                out=(U*16)();bind(f,'njGetMatrix',None,P)(out);native.append((original_address(node.model),bytes(out).hex()))
        except BaseException as e:callback_errors.append(str(e))
    callback_pointer.value=C.cast(callback,P).value;draw=bind(f,'draw_action_link',None,P,F)
    link=MotionLink((P*2)(action.motion,action.motion),(F*2)(0,0));linked=ActionLink(action.object,C.addressof(link))
    for bankmode in range(2):
        for mode,frame,second,ratio in ([(m,x,y,t) for m in ('same','cross') for x,y in ((0.,16.),(8.5,24.),(32.,4.)) for t in (0.,.25,.5,.75,1.)]+[(m,8.5,16.,t) for m in ('null_first','null_second','null_both') for t in (0.,.5,1.)]+[('cross',0.,16.,1.25),('cross',33.,16.,.5),('cross',0.,33.,.5)]+[('cross',8.5,16.,t/32.) for t in (0.,4.,8.5,12.,16.,20.,24.,28.,32.)]):
            ram=bytearray(initial);iw(ram,table['_stack_size_'],[1]);iw(ram,table['_max_stack_size_'],[128]);iw(ram,table['_nj_matrix_flag_'],[0]);iw(ram,table['_nj_base_matrix_ptr_'],[0x8ce00000]);iw(ram,table['_nj_current_matrix_ptr_'],[0x8ce00000]);iw(ram,table['frame_step'],[1]);iw(ram,table['_nj_motion_callback_'],[0])
            CTX=0x8cd00000;CURSORS=0x8cd00200;COLOR=0x8cd00400;iw(ram,table['_nj_current_ctx_'],[CTX]);iw(ram,table['_nj_vertex_buffer_'],[CURSORS]);iw(ram,table['_nj_color_mode_'],[COLOR]);iw(ram,table['_nj_vertex_buf_'],[0x8ce10000]);iw(ram,CTX+144,[0x80000000,0x20800000,0x20800000,0])
            for i,j in enumerate((0,2,4,1,3)):iw(ram,CURSORS+4*j,[0x8cd10000+0x10000*i])
            fp=Fpu();fp.fpscr=0x40001|(bankmode<<21);fp.banks[bankmode^1][:]=list(matrix);fp.banks[bankmode][4]=struct.unpack('<I',struct.pack('<f',ratio))[0]
            original_object,original_motion=struct.unpack_from('<II',ram,action_address&0xffffff);first_ptr=action.motion if mode not in ('null_first','null_both') else None;second_ptr=second_action.motion if mode=='cross' else action.motion if mode not in ('null_second','null_both') else None;link.motions[:]=[first_ptr,second_ptr];link.frames[:]=[frame,second];iw(ram,0x8cd00800,[original_object,0x8cd00820]);put(ram,0x8cd00820,struct.pack('<IIff',original_address(first_ptr) if first_ptr else 0,original_address(second_ptr) if second_ptr else 0,frame,second));early=(ratio>1. or frame>=33. or second>=33.)
            s=executor.State();s.pc=table['draw_action_link'];s.pr=0x8cfed000;s.r[15]=0x8cf00000;s.r[4]=0x8cd00800;s.sr=0x70000000;original=[];seen=set();targets={table[n]:n for n in ('draw_action_link','action_sample_vector','action_sample_angles','njRotateXYZ','njRotateZXY','draw_model','njGetkmPointer','njSetkmPointer')}
            def trace(pc,word,regs,sr,pr,fpu):
                if pc in targets:seen.add(targets[pc])
                if pc==table['draw_model']:
                    captured=Fpu.from_buffer_copy(fpu);original.append((regs[4],struct.pack('<16I',*captured.banks[((captured.fpscr>>21)&1)^1]).hex()))
            reference(ram,s,fpu=fp,vector_instruction=service,special={'gbr':table['draw_work'],'r_bank':[0]*8},trace=trace,instruction_budget=3000000)
            bind(f,'njInitMatrix',None,P,I,I)(stack,128,0);bind(f,'njSetMatrix',None,P,P)(None,matrix);bind(f,'jp_sprite_probe_frame',None,I,I)(1,0);native.clear();draw(C.byref(linked),ratio)
            (d/'last_pose.json').write_text(json.dumps(dict(mode=mode,early=early,frame=frame,second=second,ratio=ratio,bank=bankmode,original=original,native=native,seen=sorted(seen),callback_errors=callback_errors),indent=2)+'\n')
            check(s.r[15]==0x8cf00000 and not callback_errors,'entire original action and passive native pose observer return safely')
            check((not ({'njGetkmPointer','njSetkmPointer'}&seen)) if early else {'njGetkmPointer','njSetkmPointer'}<=seen,'source-backed early exit or genuine command-pointer entry/exit')
            check((len(original)==len(native)==0) if early else (len(original)==len(native)==28),'same original visible model count or source-backed empty early exit')
            check(native==original,('actual native pose matrices match complete original machine execution',frame,bankmode,[i for i,(x,y) in enumerate(zip(native,original)) if x!=y]))
            now=(U*16)();bind(f,'njGetMatrix',None,P)(now);check(bytes(now)==bytes(matrix) and I.in_dll(f,'_stack_size_').value==1,'actual original action restores caller matrix and stack')
            records.append(dict(mode=mode,early=early,frame=frame,second=second,ratio=ratio,bank=bankmode,visible_models=len(native),pose_sha256=hashlib.sha256(json.dumps(native).encode()).hexdigest(),actual_original_functions_seen=sorted(seen)))
    callback_pointer.value=previous;check(bind(motions,'jp_motions_release',I,Q)(second_view.token)==0,'second resident bank reference retires');check(bind(motions,'jp_motions_release',I,Q)(view.token)==0 and bind(motions,'jp_motions_shutdown',I)()==0,'actual action view and motion owner retire');check(bind(f,'jp_queue_reset_frame',I)()==0 and bind(f,'jp_frames_shutdown',I)()==0,'actual pose frame owner retires');video.shutdown();textures.shutdown();bind(f,'jp_queue_shutdown',None)();bind(f,'jp_render_state_shutdown',None)()
    out=dict(scope='two genuine Sonic POS/ANG motions; same/cross banks; null slots; upper frame and ratio exits; both SH4 banks; no shape/scale/render/gameplay claim',out_of_range_negative_frames_tested=False,shape_and_scale_paths_verified=False,status='sampled_genuine_Sonic_blended_poses_match_complete_original_SH4_action_link_execution',checks=checks,original_complete_action_calls=len(records),records=records,original_binary_sha256=sha(a.binary),original_motion_content_sha256=sha(bank),reference_hashes=reference_hashes,passive_native_callback_and_original_instruction_trace_only=True,original_game_function_hooks_used=False,actual_original_njGetkmPointer_and_set_executed=True,model_rasterization_skipped_by_actual_original_frame_step_policy=True,rendered_animation_verified=False,build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),playable=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],checks,'checks',len(records),'whole original calls',flush=True)
if __name__=='__main__':main()
