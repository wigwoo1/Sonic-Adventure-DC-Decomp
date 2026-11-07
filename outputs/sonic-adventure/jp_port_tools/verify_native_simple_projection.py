"""Whole-original Simple projection calls with explicit two-queue MMIO values."""
import argparse,ctypes as C,hashlib,json,random,re,struct,sys
from pathlib import Path
from native_packet_test_support import bind
U,I,P,F,S=C.c_uint32,C.c_int32,C.c_void_p,C.c_float,C.c_size_t
class Work(C.Structure):
    _fields_=[('count',I),('out',P),('points',P),('normals',P),('clip_and',U),('clip_or',U),
              ('scale_x',F),('scale_y',F),('clip_left',F),('clip_right',F),('clip_top',F),('clip_bottom',F)]
class Queues(C.Structure):_fields_=[('words',U*16)]
class MMIO(dict):
    def __contains__(self,at):return (at&0xfc000000)==0xe0000000 or dict.__contains__(self,at)
    def __getitem__(self,at):return dict.__getitem__(self,(at&63) if (at&0xfc000000)==0xe0000000 else at)
    def __setitem__(self,at,value):dict.__setitem__(self,(at&63) if (at&0xfc000000)==0xe0000000 else at,value)
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
    layout=b['simple_projection_recipe']['layouts']['native']
    assert C.sizeof(Work)==layout['sizeof']==64
    for n,_ in Work._fields_:assert getattr(Work,n).offset==layout[n]
    f=C.CDLL(b['frames_DLL']);project=bind(f,'jp_simple_project',I,P,S,S,S,P,U)
    screen=(F*5).in_dll(f,'_nj_screen_');screen[:]=[256,640,480,320,240]
    near=F.in_dll(f,'_nj_near_clip_');far=F.in_dll(f,'_nj_far_clip_');near.value=1;far.value=1000
    sys.path.insert(0,str(p));import verify_startup as executor
    from model_reference import make_reference
    from verify_fpu_cleanup_transfer import Fpu
    reference,hashes=make_reference(executor)
    numeric=bind(C.CDLL(str(a.numeric_dll.resolve())),'sa_sh4_vector_numeric',I,U,P,U)
    def service(op,state):assert numeric(op,C.byref(state),state.fpul)==0
    symbols={n:int(at,16) for at,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)\s*$',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    initial=bytearray(16<<20);raw=a.binary.read_bytes();initial[0x10000:0x10000+len(raw)]=raw
    def put(ram,at,data):ram[at&0xffffff:(at&0xffffff)+len(data)]=data
    matrices=[(1.,0,0,0,0,1.,0,0,0,0,1.,0,0,0,0,1.),
              (.75,.5,0,0,-.5,.75,0,0,0,0,1.,0,2.,-3.,4.,1.)]
    points=[(0.,0.,0.),(0.,0.,1.),(0.,0.,1000.),(0.,0.,1001.),
            (-4.,0.,2.),(4.,0.,2.),(0.,-3.,2.),(0.,3.,2.),
            (1.,2.,-1.),(-.25,.5,.75),(12.,-14.,20.),(.1,.2,1e-15),
            (1e-20,-1e-20,1e-20),(20.,40.,100.),(-10.,2.,4.),(1.,1.,3.),(0.,0.,2.)]
    normals=[(float(i),-float(i),.5) for i in range(len(points))]
    rng=random.Random(0x53494d50);checks=0;records=[];masks=set()
    def check(ok,label):
        nonlocal checks
        assert ok,label;checks+=1
    stack=C.create_string_buffer(128*64);bind(f,'njInitMatrix',None,P,I,I)(stack,128,0)
    for kind in range(1,5):
      for count in (0,1,2,3,7,16):
       for parity in (0,1):
        for bank in (0,1):
         for mi,matrix in enumerate(matrices):
            ram=bytearray(initial);output=0x8cd10000+parity*32;workat=0x8cd00000
            pointat=0x8cd02000;normalat=0x8cd03000
            put(ram,pointat,b''.join(struct.pack('<3f',*v) for v in points))
            put(ram,normalat,b''.join(struct.pack('<3f',*v) for v in normals))
            put(ram,output-32,b'\xa5'*(count*32+64))
            rawwork=struct.pack('<iIIII Iffffff',count,output,pointat,normalat,0xffffffff,0,
                                256.,-256.,0.,640.,0.,480.)
            assert len(rawwork)==48;put(ram,workat,rawwork)
            put(ram,symbols['_nj_screen_'],bytes(screen))
            put(ram,symbols['_nj_near_clip_'],struct.pack('<f',1.))
            put(ram,symbols['_nj_far_clip_'],struct.pack('<f',1000.))
            seeds=[rng.getrandbits(32) for _ in range(16)]
            mmio=MMIO({0xff000038:0xeeeeeeee,0xff00003c:0xdddddddd})
            for i,word in enumerate(seeds):mmio[i*4]=word
            writes=[];invalidated=[]
            def pref(at):
                if (at&0xfc000000)!=0xe0000000:return
                start=at&~31;target=(start&0x3ffffff)|0x8c000000
                words=[mmio[start+4*j] for j in range(8)]
                put(ram,target,struct.pack('<8I',*words));writes.append(target)
            fp=Fpu();fp.fpscr=0x40001|(bank<<21);fp.banks[bank^1][:]=struct.unpack('<16I',struct.pack('<16f',*matrix))
            state=executor.State();state.pc=symbols['njSimpleCalcModel_Loop'+str(kind)]
            state.pr=0x8cfed000;state.r[15]=0x8cf00000;state.sr=0x70000000
            state.r[4]=workat;state.r[5]=0x8cd04000
            reference(ram,state,fpu=fp,mmio=mmio,vector_instruction=service,
                      special={'gbr':0,'r_bank':[0]*8},cache_prefetch=pref,
                      cache_invalidate=invalidated.append,instruction_budget=1000000)
            pts=(F*(len(points)*3))(*[x for v in points for x in v])
            nrm=(F*(len(normals)*3))(*[x for v in normals for x in v])
            storage=C.create_string_buffer(count*32+160);C.memset(storage,0xa5,C.sizeof(storage))
            address=(C.addressof(storage)+63)&~31
            if (address>>5)&1!=parity:address+=32
            queues=Queues((U*16)(*seeds))
            work=Work(count,address,C.addressof(pts),C.addressof(nrm),0xffffffff,0,256.,-256.,0.,640.,0.,480.)
            before=bytes(work)
            bind(f,'njSetMatrix',None,P,P)(None,(F*16)(*matrix))
            check(project(C.byref(work),count*32,count,count,C.byref(queues),kind)==0,'bounded native original projection entry returns')
            expected=bytes(ram[output&0xffffff:(output&0xffffff)+count*32]);actual=C.string_at(address,count*32)
            record=dict(kind=kind,count=count,queue_parity=parity,bank=bank,matrix=mi,
                        expected_sha256=hashlib.sha256(expected).hexdigest(),actual_sha256=hashlib.sha256(actual).hexdigest())
            (d/'last_case.json').write_text(json.dumps(record,indent=2)+'\n')
            check(actual==expected,record)
            check(list(queues.words)==[mmio[i*4] for i in range(16)],'both retained queue lines match including every untouched word')
            check(work.clip_and==struct.unpack_from('<I',ram,(workat&0xffffff)+16)[0] and
                  work.clip_or==struct.unpack_from('<I',ram,(workat&0xffffff)+20)[0],'clip AND/OR match whole original loop')
            mutable=set(range(Work.clip_and.offset,Work.clip_or.offset+4)) if kind>=3 else set()
            check(all(bytes(work)[i]==before[i] for i in range(C.sizeof(work)) if i not in mutable),'all other native work bytes preserved')
            prefix=address-C.addressof(storage);tail=prefix+count*32
            check(bytes(storage)[:prefix]==b'\xa5'*prefix and bytes(storage)[tail:]==b'\xa5'*(C.sizeof(storage)-tail),'all output allocation guards preserved')
            check(state.r[15]==0x8cf00000 and writes==list(range(output,output+count*32,32)) and invalidated==writes,'original stack returns and cache-line/store-queue operations are observed')
            check(mmio[0xff000038]==mmio[0xff00003c]==12,'original QACRs select supplied RAM page even for zero count')
            if kind>=3:masks.update(struct.unpack_from('<I',expected,i*32+12)[0] for i in range(count))
            records.append(record)
    # Invalid spans must be rejected before touching the work or either queue.
    work=Work(1,None,None,None,0xffffffff,0,1,1,0,1,0,1);queues=Queues()
    for kind in (0,1,2,3,4,5):
        before=bytes(work),bytes(queues)
        check(project(C.byref(work),0,0,0,C.byref(queues),kind)==1 and before==(bytes(work),bytes(queues)),
              'invalid kind/null/short spans fail without mutation')
    # Put the final point against an inaccessible OS page. The original reads
    # a speculative next point; the native value path deliberately does not.
    kernel=C.WinDLL('kernel32',use_last_error=True)
    alloc=bind(kernel,'VirtualAlloc',P,P,S,U,U);protect=bind(kernel,'VirtualProtect',I,P,S,U,P)
    free=bind(kernel,'VirtualFree',I,P,S,U)
    page=alloc(None,8192,0x3000,4);check(bool(page),'actual guarded input allocation succeeds')
    old=U();check(protect(page+4096,4096,1,C.byref(old))!=0,'following actual input page is inaccessible')
    bind(f,'njSetMatrix',None,P,P)(None,(F*16)(*matrices[0]))
    try:
        for count in (1,16):
            point=page+4096-count*12
            C.memmove(point,struct.pack('<3f',1,2,3)*count,count*12)
            for kind in range(1,5):
                storage=C.create_string_buffer(count*32+96);address=(C.addressof(storage)+63)&~31
                work=Work(count,address,point,point,0xffffffff,0,256,-256,0,640,0,480)
                check(project(C.byref(work),count*32,count,count,C.byref(queues),kind)==0,
                      'all native projection entries accept exact point span ending at a real guard page')
        work=Work(0,None,None,None,0xffffffff,0,1,1,0,1,0,1)
        before=bytes(work),bytes(queues)
        check(project(C.byref(work),0,0,0,C.byref(queues),1)==0 and before==(bytes(work),bytes(queues)),
              'zero-count call accepts empty spans and preserves all work/queue bytes')
    finally:
        released=free(page,0,0x8000)
    check(released!=0,'actual guarded input allocation retires')
    out=dict(status='all_four_native_Simple_projection_entries_match_sampled_whole_original_loops',
        checks=checks,original_complete_calls=len(records),records=records,observed_clip_masks=sorted(masks),
        exact_output_and_both_persistent_queue_lines_compared=True,
        original_QACR_PREF_OCBI_observed=True,native_hardware_side_effects_emulated=False,
        native_unused_next_point_reads_removed=True,original_game_function_hooks_used=False,
        actual_Windows_guard_page_proves_no_next_point_read=True,
        full_Simple_model_or_aura_or_gameplay_verified=False,
        reference_hashes=hashes,verifier_sha256=sha(__file__),
        build_verification_sha256=sha(a.build/'build_verification.json'),playable=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(out,indent=2)+'\n')
    print(out['status'],checks,'checks',len(records),'whole original calls',flush=True)
if __name__=='__main__':main()
