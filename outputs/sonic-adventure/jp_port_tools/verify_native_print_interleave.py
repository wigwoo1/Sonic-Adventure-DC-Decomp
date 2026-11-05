"""Print descriptor writes followed by ordinary SDK reservation/draw calls."""
import argparse,ctypes as C,hashlib,json,struct
from pathlib import Path
from native_packet_test_support import bind
from native_texture_test_support import Runtime
from native_video_test_support import Video
U=C.c_uint32;I=C.c_int;P=C.c_void_p;S=C.c_size_t;Q=C.c_uint64
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','report']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();b=json.loads((a.build/'build_verification.json').read_bytes());assert not a.report.exists()
    for n,h in b['authored_source_sha256'].items():assert sha(a.workspace/n)==h,n
    t=Runtime(Path(b['frames_DLL']));lib=t.dll;v=Video(lib);checks=0
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    qi=bind(lib,'jp_queue_initialize',I,S);qs=bind(lib,'jp_queue_shutdown',None);reset=bind(lib,'jp_queue_reset_frame',I);reserve=bind(lib,'jp_queue_reserve',I,U,U)
    data=bind(lib,'jp_queue_data',P,U);used=bind(lib,'jp_queue_committed_bytes',S,U);status=bind(lib,'jp_queue_status',I);get=bind(lib,'njGetkmPointer',None);put=bind(lib,'njSetkmPointer',None);start=bind(lib,'njStartVertexNonTexTrans',P);end=bind(lib,'njEndVertex',None,P)
    printc=bind(lib,'njPrintC',None,I,P);desc=bind(lib,'jp_queue_descriptor_cursor',P,U);param=bind(lib,'jp_queue_parameter_cursor',P,U);setctx=bind(lib,'jp_queue_set_context',I,U,P)
    fi=bind(lib,'jp_frames_initialize',I,U,U,S);fs=bind(lib,'jp_frames_shutdown',I);submit=bind(lib,'jp_frames_submit',I,P);release=bind(lib,'jp_frame_release',I,Q)
    check(qi(8<<20)==0 and fi(4,8,8<<20)==0 and t.init(4,16,8<<20)==0 and v.init(320,96,0xff000000,1)==0 and bind(lib,'jp_ninja_contexts_initialize',I)()==0,'actual shared owners')
    bind(lib,'njInitPrint',None,P,U,U)(None,0,0)
    check(setctx(0,(U*4)(0x82000002,0xe0000000,0x209000c0,0))==0,'ordinary nontextured translucent context')
    rows=[]
    for round in range(24):
        printc(0,C.create_string_buffer(b'A'*(round%7+1)));check(status()==0,'genuine print appended')
        before=used(1);prefix=C.string_at(data(1),before);saved=(param(1) or 0)-data(1)
        # This is the existing real producer contract: reserve the full bound,
        # then the SDK loads the descriptor with njGetkmPointer.
        check(reserve(1,4)==0 and (param(1) or 0)-data(1)==saved,'reserve honours newest descriptor without changing Ninja parameter offset')
        get();pointer=start();check(bool(pointer) and status()==0,'ordinary SDK draw starts after direct print')
        triangle=[]
        for i,(x,y) in enumerate([(0.,0.),(1.,0.),(0.,1.)]):
            triangle += [0xf0000000 if i==2 else 0xe0000000,*[struct.unpack('<I',struct.pack('<f',n))[0] for n in [x,y,1.]],0xff123456,0,0,0]
        C.memmove(pointer,struct.pack('<24I',*triangle),96);end(pointer+96);put()
        check(status()==0 and used(1)==before+128 and C.string_at(data(1),before)==prefix,'ordinary geometry appends without overwriting preceding print')
        check(param(1)==desc(1)==data(1)+used(1),'ordinary SDK commits shared cursor state')
        rows.append(used(1))
    fid=Q();check(submit(C.byref(fid))==0 and release(fid.value)==0,'mixed actual queue captured/released by frame owner');bind(lib,'njExitPrint',None)();qs();t.shutdown();check(fs()==0,'all resources drained');v.shutdown()
    result=dict(status='original_print_and_ordinary_SDK_queue_reservations_interleave_verified',checks=checks,mixed_print_and_regular_batches=48,successive_growth_positions=rows,existing_reserve_before_get_contract_preserved=True,earlier_print_packets_preserved=True,one_queue_owner=True,gameplay_verified=False,playable=False,overall_completion_percent=10,build_report_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__))
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result),flush=True)
if __name__=='__main__':main()
