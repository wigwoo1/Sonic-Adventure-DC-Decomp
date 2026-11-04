"""Adversarial background-plan bounds and atomic frame-resource capture/retry."""
import argparse,ctypes as C,hashlib,json,struct
from pathlib import Path
from native_packet_test_support import bind
from native_texture_test_support import Runtime,Record
from native_video_test_support import Video,Context
from native_pvm_test_support import PVM,archive,texture,destination
U=C.c_uint32;I=C.c_int;P=C.c_void_p;Q=C.c_uint64;S=C.c_size_t
class Plan(C.Structure):_fields_=[(n,S) for n in ['allocation_bytes','batch_count','vertex_count','triangle_count']]+[(n,P) for n in ['batches','vertices','triangles']]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def fw(f):return struct.unpack('<I',struct.pack('<f',f))[0]
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','report']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.build.resolve();b=json.loads((d/'build_verification.json').read_text());assert not a.report.exists()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    t=Runtime(Path(b['frames_DLL']));lib=t.dll;v=Video(lib);pvm=PVM(lib);gpu=C.CDLL(b['GPU_DLL']);checks=0;keep=[]
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    qi=bind(lib,'jp_queue_initialize',I,S);qs=bind(lib,'jp_queue_shutdown',None);reset=bind(lib,'jp_queue_reset_frame',I);fi=bind(lib,'jp_frames_initialize',I,U,U,S);fs=bind(lib,'jp_frames_shutdown',I);fc=bind(lib,'jp_frames_clear_error',None)
    submit=bind(lib,'jp_frames_submit',I,P);release=bind(lib,'jp_frame_release',I,Q);pinned=bind(lib,'jp_frames_pinned_bytes',S);accounted=bind(lib,'jp_frames_accounted_bytes',S);live=bind(lib,'jp_frames_live_count',U);pin=bind(lib,'jp_frames_prepare_texture',I,U);surface_count=bind(lib,'jp_frame_surface_count',U,Q)
    state=bind(lib,'kmSetBackGroundRenderState',I,P);plane=bind(lib,'kmSetBackGroundPlane',I,P,I,I);back=bind(lib,'njSetBackColor',None,U,U,U);ri=bind(lib,'jp_render_state_initialize',I,U,S);rs=bind(lib,'jp_render_state_shutdown',None)
    check(qi(1<<20)==0 and fi(2,2,1<<20)==0 and t.init(8,32,1<<20)==0 and v.init(64,64,0xff000000,1)==0 and ri(64,1<<20)==0,'real shared production owners')
    c=Context();c.face[:]=[1,1,1,1];c.words[:]=[0x80000002,0x80800000,0x20800000,0,0];check(state(C.byref(c))==0,'real SDK state');back(0xff123456,0xff123456,0xff123456)
    snapshot=v.snapshot();old=Plan();old.allocation_bytes=C.sizeof(old);append=bind(gpu,'jp_background_prepend',I,P,P,S,P);destroy=bind(gpu,'jp_packets_destroy',None,P)
    before=bytes(snapshot);out=P(0x1234);check(append(C.byref(snapshot),C.byref(old),0,C.byref(out))!=0 and out.value==0x1234 and bytes(snapshot)==before,'zero budget preserves output and immutable source')
    check(append(C.byref(snapshot),C.byref(old),1<<20,C.byref(out))==0,'bounded native plane allocation');plan=Plan.from_address(out.value);cost=plan.allocation_bytes;check((plan.batch_count,plan.vertex_count,plan.triangle_count)==(1,4,2),'full-viewport plane allocation counts');destroy(out)
    out=P(0x1234);check(append(C.byref(snapshot),C.byref(old),cost+C.sizeof(old)-1,C.byref(out))==7 and out.value==0x1234,'simultaneous old/new plan peak budget enforced')
    check(append(C.byref(snapshot),C.byref(old),cost+C.sizeof(old),C.byref(out))==0,'exact peak budget succeeds');destroy(out)
    for kind,value in [('bytes',0),('bytes',248),('position',0x7fc00000),('q',0),('q',0xbf800000)]:
        broken=type(snapshot).from_buffer_copy(bytes(snapshot))
        if kind=='bytes':broken.background_bytes=value
        else:broken.background_packet[3 if kind=='position' else 5]=value
        before=bytes(broken);out=P(0x1234);check(append(C.byref(broken),C.byref(old),1<<20,C.byref(out))!=0 and out.value==0x1234 and bytes(broken)==before,'malformed packet rejected without allocation/output mutation')
    names,listing,buffers=destination(2);payload=archive([texture(bytes(128),kind=0x101),texture(b'\xff'*128,kind=0x101)]);data=C.create_string_buffer(payload);check(pvm.load(C.byref(listing),2,28,data,len(payload))==1,'actual two-texture fixture')
    records=[Record.from_address(t.lookup(C.byref(listing),i)) for i in range(2)];ids=[rec.texparambuffer&0x1fffff for rec in records]
    def background(index):
        rec=records[index];context=Context();context.face[:]=[1,1,1,1];context.words[:]=[0x8000000a,0x82800000,0x20800000|rec.tspparambuffer,rec.texparambuffer,0];check(state(C.byref(context))==0,'actual textured background context')
        vertices=[(U*8)(0xe0000000,fw(x),fw(y),fw(.5),fw(.25),fw(.25),0xffffffff,0) for x,y in [(0,0),(64,0),(0,64)]];pointers=(P*3)(*[C.addressof(x) for x in vertices]);check(plane(pointers,3,32)==0,'actual background packet');keep.extend(vertices)
    check(fs()==0 and fi(2,1,1<<20)==0 and pin(ids[0])==0,'one resource slot filled by earlier recording');background(1);out=Q(0x12345678)
    check(submit(C.byref(out))==3 and out.value==0x12345678 and pinned()==128 and live()==0,'background capacity failure preserves earlier reference and output')
    fc();background(0);check(submit(C.byref(out))==0 and surface_count(out.value)==1 and pinned()==128,'same-texture retry deduplicates existing reference');check(release(out.value)==0 and pinned()==0,'capacity retry frame drained')
    check(fs()==0 and fi(2,2,1<<20)==0,'measure actual metadata');meta=accounted();check(fs()==0 and fi(2,2,meta+128+192)==0 and pin(ids[0])==0,'tight capture budget fits earlier texture and snapshot only')
    background(1);reserve=bind(lib,'jp_queue_reserve',I,U,U);ctx=bind(lib,'jp_queue_set_context',I,U,P);get=bind(lib,'njGetkmPointer',None);put=bind(lib,'njSetkmPointer',None);start=bind(lib,'njStartVertexNonTexOpaque',P);end=bind(lib,'njEndVertex',None,P);used=bind(lib,'jp_queue_committed_bytes',S,U);stream=bind(lib,'jp_queue_data',P,U)
    check(reserve(0,4)==0 and ctx(0,(U*4)(0x80000002,0x80800000,0x20800000,0))==0,'real foreground packet');get();ptr=start();words=[]
    for i,(x,y) in enumerate([(-64,-64),(192,-64),(-64,192)]):words.extend([0xf0000000 if i==2 else 0xe0000000,fw(x),fw(y),fw(1),0,0,0xffffffff,0])
    raw=struct.pack('<'+'I'*len(words),*words);C.memmove(ptr,raw,len(raw));end(ptr+len(raw));put();before=C.string_at(stream(0),used(0));check(len(before)==128,'actual guarded snapshot byte cost')
    out=Q(0x12345678);check(submit(C.byref(out))==4 and out.value==0x12345678 and live()==0 and pinned()==128 and accounted()==meta+128 and C.string_at(stream(0),used(0))==before,'post-pin snapshot-budget failure rolls back only new background resource and preserves recording')
    fc();background(0);check(submit(C.byref(out))==0 and surface_count(out.value)==1 and live()==1,'exact-budget retry preserves complete foreground and background');check(release(out.value)==0 and pinned()==0 and accounted()==meta,'retry frame frees every resource and allocation')
    check(t.release_list(C.byref(listing))==1 and t.live()==0,'fixture textures released');qs();rs();t.shutdown();check(fs()==0 and pvm.shutdown()==0,'all production owners drained')
    result=dict(status='background_plan_bounds_peak_budget_and_atomic_frame_resource_capture_retry_verified',checks=checks,post_pin_allocation_failure_rollback_verified=True,resource_capacity_retry_and_deduplication_verified=True,immutable_packet_and_failed_output_preservation_verified=True,exact_peak_budget_verified=True,build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(Path(__file__)),full_game_boot_executed=False,playable=False,overall_completion_percent=10)
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
if __name__=='__main__':main()
