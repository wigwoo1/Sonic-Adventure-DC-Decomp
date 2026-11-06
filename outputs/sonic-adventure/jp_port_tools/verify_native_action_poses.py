"""Whole original action execution:sampled real Sonic matrices before rasterization."""
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
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','build','binary','numeric-dll','motion-inputs','output'):ap.add_argument('--'+n,dest=n.replace('-','_'),type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();b=read(a.build/'build_verification.json');assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h
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
    callback_pointer.value=C.cast(callback,P).value;draw=bind(f,'draw_action',None,P,F)
    for bankmode in range(2):
        for frame in (0.,1.,4.,8.5,16.,24.,32.):
            ram=bytearray(initial);iw(ram,table['_stack_size_'],[1]);iw(ram,table['_max_stack_size_'],[128]);iw(ram,table['_nj_matrix_flag_'],[0]);iw(ram,table['_nj_base_matrix_ptr_'],[0x8ce00000]);iw(ram,table['_nj_current_matrix_ptr_'],[0x8ce00000]);iw(ram,table['frame_step'],[1]);iw(ram,table['_nj_motion_callback_'],[0])
            CTX=0x8cd00000;CURSORS=0x8cd00200;COLOR=0x8cd00400;iw(ram,table['_nj_current_ctx_'],[CTX]);iw(ram,table['_nj_vertex_buffer_'],[CURSORS]);iw(ram,table['_nj_color_mode_'],[COLOR]);iw(ram,table['_nj_vertex_buf_'],[0x8ce10000]);iw(ram,CTX+144,[0x80000000,0x20800000,0x20800000,0])
            for i,j in enumerate((0,2,4,1,3)):iw(ram,CURSORS+4*j,[0x8cd10000+0x10000*i])
            fp=Fpu();fp.fpscr=0x40001|(bankmode<<21);fp.banks[bankmode^1][:]=list(matrix);fp.banks[bankmode][4]=struct.unpack('<I',struct.pack('<f',frame))[0]
            s=executor.State();s.pc=table['draw_action'];s.pr=0x8cfed000;s.r[15]=0x8cf00000;s.r[4]=action_address;s.sr=0x70000000;original=[];seen=set();targets={table[n]:n for n in ('draw_action','action_sample_vector','action_sample_angles','njRotateXYZ','njRotateZXY','draw_model_nested','njGetkmPointer','njSetkmPointer')}
            def trace(pc,word,regs,sr,pr,fpu):
                if pc in targets:seen.add(targets[pc])
                if pc==table['draw_model_nested']:
                    captured=Fpu.from_buffer_copy(fpu);original.append((regs[4],struct.pack('<16I',*captured.banks[((captured.fpscr>>21)&1)^1]).hex()))
            reference(ram,s,fpu=fp,vector_instruction=service,special={'gbr':table['draw_work'],'r_bank':[0]*8},trace=trace,instruction_budget=3000000)
            bind(f,'njInitMatrix',None,P,I,I)(stack,128,0);bind(f,'njSetMatrix',None,P,P)(None,matrix);bind(f,'jp_sprite_probe_frame',None,I,I)(1,0);native.clear();draw(view.data,frame)
            (d/'last_pose.json').write_text(json.dumps(dict(frame=frame,bank=bankmode,original=original,native=native,seen=sorted(seen),callback_errors=callback_errors),indent=2)+'\n')
            check(s.r[15]==0x8cf00000 and not callback_errors,'entire original action and passive native pose observer return safely')
            check({'njGetkmPointer','njSetkmPointer'}<=seen,'actual original command-pointer entry/exit routines execute without dependency hooks')
            check(len(original)>10 and len(native)==len(original),'same visible original model order and count at sampled pose')
            check(native==original,('actual native pose matrices match complete original machine execution',frame,bankmode,[i for i,(x,y) in enumerate(zip(native,original)) if x!=y]))
            now=(U*16)();bind(f,'njGetMatrix',None,P)(now);check(bytes(now)==bytes(matrix) and I.in_dll(f,'_stack_size_').value==1,'actual original action restores caller matrix and stack')
            records.append(dict(frame=frame,bank=bankmode,visible_models=len(native),pose_sha256=hashlib.sha256(json.dumps(native).encode()).hexdigest(),actual_original_functions_seen=sorted(seen)))
    callback_pointer.value=previous;check(bind(motions,'jp_motions_release',I,Q)(view.token)==0 and bind(motions,'jp_motions_shutdown',I)()==0,'actual action view and motion owner retire');check(bind(f,'jp_queue_reset_frame',I)()==0 and bind(f,'jp_frames_shutdown',I)()==0,'actual pose frame owner retires');video.shutdown();textures.shutdown();bind(f,'jp_queue_shutdown',None)();bind(f,'jp_render_state_shutdown',None)()
    out=dict(status='sampled_genuine_Sonic_action_poses_match_complete_original_SH4_action_execution',checks=checks,original_complete_action_calls=len(records),records=records,original_binary_sha256=sha(a.binary),original_motion_content_sha256=sha(bank),reference_hashes=reference_hashes,passive_native_callback_and_original_instruction_trace_only=True,original_game_function_hooks_used=False,actual_original_njGetkmPointer_and_set_executed=True,model_rasterization_skipped_by_actual_original_frame_step_policy=True,rendered_animation_verified=False,build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),playable=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],checks,'checks',len(records),'whole original calls',flush=True)
if __name__=='__main__':main()
