"""Owned frames from actual SDK/model/sprite producers and real texture release.
Tests submission/lifetime/rollback, not pixels, render/flip, hardware or gameplay.
"""
import argparse,ctypes as C,hashlib,json,random,struct
from pathlib import Path
from native_texture_test_support import Runtime,Info,Record,Name,List,field
from verify_native_model_preflight import Vec,Material,Mesh,Model,Span,Bound
from verify_native_sprites import Point,Frame,Sprite
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','report']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.build.resolve();assert not a.report.exists()
    b=json.loads((d/'build_verification.json').read_text());assert b['exit_code']==b['diagnostic_bytes']==0
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    dll=Path(b['DLL']);assert sha(dll)==b['DLL_sha256'];t=Runtime(dll);lib=t.dll;U=C.c_uint32;I=C.c_int32;P=C.c_void_p;S=C.c_size_t;Q=C.c_uint64;F=C.c_float
    def bind(n,result,*args):
        f=getattr(lib,n);f.restype=result;f.argtypes=list(args);return f
    fi=bind('jp_frames_initialize',I,U,U,S);fs=bind('jp_frames_shutdown',I);fstatus=bind('jp_frames_status',I);fc=bind('jp_frames_clear_error',None)
    pin=bind('jp_frames_prepare_texture',I,U);pins=bind('jp_frames_prepare_textures',I,P,U);submit=bind('jp_frames_submit',I,C.POINTER(Q));release=bind('jp_frame_release',I,Q)
    flist=bind('jp_frame_list',P,Q,U,C.POINTER(S));surface=bind('jp_frame_surface',P,Q,U,C.POINTER(S));count=bind('jp_frame_surface_count',U,Q)
    accounted=bind('jp_frames_accounted_bytes',S);pinned=bind('jp_frames_pinned_bytes',S);orphaned=bind('jp_frames_orphaned_bytes',S);live=bind('jp_frames_live_count',U)
    qi=bind('jp_queue_initialize',I,S);qs=bind('jp_queue_shutdown',None);reset=bind('jp_queue_reset_frame',I);qstatus=bind('jp_queue_status',I);qc=bind('jp_queue_clear_error',None)
    reserve=bind('jp_queue_reserve',I,U,U);used=bind('jp_queue_committed_bytes',S,U);data=bind('jp_queue_data',P,U);ctx=bind('jp_queue_set_context',I,U,P)
    get=bind('njGetkmPointer',None);put=bind('njSetkmPointer',None);end=bind('njEndVertex',None,P);discard=bind('jp_queue_discard_active',I);mode=bind('njSetkmColorMode',None,U)
    selector=bind('njSetkmTextureNum',I,U);names=['njStartVertexTexOpaque','njStartVertexTexTrans','njStartVertexTexOpaqueIntens','njStartVertexTexTransIntens','njStartVertexTexOpaqueIntensH','njStartVertexTexTransIntensH','njStartVertexTexPunch','njStartVertexTexPunchIntens','njStartVertexTexPunchIntensH']
    starts={n:bind(n,P,*([P,P] if n.endswith('H') else [P] if 'Intens' in n else [])) for n in names}
    plain=bind('njStartVertexNonTexOpaque',P)
    renderinit=bind('jp_render_state_initialize',I,U,S);guard=bind('jp_render_state_check_guards',I);matrix=bind('njSetMatrix',None,P,P)
    modelinit=bind('draw_model_init',None);setlight=bind('draw_set_light',None,F,F,F);modelbegin=bind('draw_models_begin',None);modelend=bind('draw_models_end',None);drawmodel=bind('draw_model_unculled',None,P)
    measure=bind('jp_model_preflight',I,P,S,S,S,S,P,P);modelreserve=bind('jp_model_reserve',I,P)
    draw2=bind('njDrawSprite2D',None,P,I,F,U);draw3=bind('njDrawSprite3D',None,P,I,U);drawflat=bind('njDrawTexture',None,P,I,I,I)
    control=U.in_dll(lib,'_nj_control_3d_flag_');frame_step=bind('jp_sprite_probe_frame',None,I,I);callback=bind('jp_sprite_probe_callback',None,I,I)
    rng=random.Random(0x4652414d);checks=0;groups={};keep=[];submitted=0;real_draws=0
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,label
    def clear():fc();qc();t.clear()
    def setup(slots=4,resources=16,budget=1<<20):
        qs();check(fs()==0,'old frame owner shutdown');check(qi(16<<20)==0,'queue initialize');check(fi(slots,resources,budget)==0,'frame owner initialize');check(t.init(8,128,1<<20)==0,'texture owner initialize')
        check(ctx(0,(U*4)(0x8000000a,0x20800000,0x20800000,0))==0,'context');control.value=0
    def textures(n=1):
        ns=(Name*n)();payloads={};ids=[];infos=[]
        for j in range(n):
            info=Info();s=info.texsurface;s.Type=0x10000;s.BitDepth=1;s.PixelFormat=0x08000000;s.nWidth=s.nHeight=8;s.TextureSize=128;s.fSurfaceFlags=4
            raw=rng.randbytes(128);check(t.copy(C.byref(info),C.create_string_buffer(raw),128)==0,'owned texture copy')
            t.setname(C.byref(ns[j]),None,1000+j,0);t.entry(field(ns[j]),C.byref(info),1000+j,2,0xffffffff);check(t.status()==0,'actual SDK record entry')
            addr=t.resolve(ns[j].texaddr);check(addr and addr>0xffffffff,'native record');t.param(addr);check(t.status()==0,'real SDK texture word')
            word=t.surface_word(C.byref(s));check(Record.from_address(addr).texparambuffer&0x1fffff==word,'native TCW ID');ids.append(word);payloads[word]=raw;infos.append(info)
        listing=List(ns,n);keep.extend([ns,listing,*infos]);check(t.select(C.byref(listing))==1,'actual texture list selected');return listing,ids,payloads
    def batch(name,index=0,cancel=False):
        lane=2 if 'Punch' in name else 1 if 'Trans' in name else 0
        check(reserve(lane,8)==0,'whole raw batch reservation');get();mode(0x0008000a if lane!=1 else 0x0210000a);check(selector(index)==1,'real texture selection')
        c=(U*4)(0x3f800000,0x3f000000,0x3e800000,0x3f800000);o=(U*4)(1,2,3,4)
        ptr=starts[name](*([c,o] if name.endswith('H') else [c] if 'Intens' in name else []));check(ptr and ptr>0xffffffff,'actual SDK batch start')
        if cancel:check(discard()==0,'active batch cancellation')
        else:
            raw=rng.randbytes(96);C.memmove(ptr,raw,96);end(ptr+96);check(qstatus()==0,'complete batch commit')
        put();check(qstatus()==0,'SDK context commit');return lane
    def capture():
        nonlocal submitted
        expected=[C.string_at(data(i),used(i)) if used(i) else b'' for i in range(5)];out=Q(0xfeedface)
        check(submit(C.byref(out))==0 and out.value!=0xfeedface,'atomic frame submission');submitted+=1
        check([used(i) for i in range(5)]==[0]*5 and qstatus()==0,'submission advances recording queue')
        compare(out.value,expected);return out.value,expected
    def compare(fid,expected):
        for li,raw in enumerate(expected):
            size=S(0x1234);ptr=flist(fid,li,C.byref(size));check(size.value==len(raw) and (C.string_at(ptr,size.value) if ptr else b'')==raw,'owned immutable list bytes')
            if raw:check(ptr>0xffffffff and ptr%32==0,'snapshot native width/alignment')
    def resources_match(fid,payloads):
        check(count(fid)==len(payloads),'unique frame texture references')
        for word,raw in payloads.items():
            size=S(0x1234);ptr=surface(fid,word,C.byref(size));check(ptr and size.value==len(raw) and C.string_at(ptr,size.value)==raw,'retained original encoded payload')
    check(qi(16<<20)==0 and renderinit(2048,1<<20)==0,'real native owners initialized')
    # Every real SDK texture-header form, with release BEFORE submission.
    for name in names:
        setup();listing,ids,payloads=textures();batch(name);check(pinned()==128 and orphaned()==0,'capture at first header')
        t.shutdown();check(orphaned()==128,'texture shutdown defers live queued payload')
        fid,raw=capture();resources_match(fid,payloads)
        check(reset()==0 and qi(16<<20)==0,'queue reset and reinitialize');compare(fid,raw);resources_match(fid,payloads)
        check(release(fid)==0 and pinned()==orphaned()==live()==0,'last frame releases deferred payload exactly once')
    groups['all_nine_textured_SDK_forms_release_before_submit']=checks
    # Shared resources, cancelled batches, pending release and failed starts.
    setup();listing,ids,payloads=textures();batch(names[0]);f1,r1=capture();batch(names[4]);f2,r2=capture()
    check(pinned()==128 and live()==2,'two frames share one pinned allocation');check(t.release_num(0)==1 and t.live()==0,'actual SDK texture release')
    check(orphaned()==128,'SDK free defers shared allocation');resources_match(f1,payloads);resources_match(f2,payloads)
    check(release(f1)==0 and pinned()==128 and orphaned()==128,'first shared frame release');check(release(f2)==0 and pinned()==orphaned()==0,'last shared frame release')
    clear();check(release(f1)==7,'stale frame ID');fc()
    # Reject stale words even when an older snapshot retains their payload.
    check(t.init(8,128,1<<20)==0,'new texture owner');listing,ids,payloads=textures();batch(names[0]);fid,raw=capture();t.shutdown();get()
    before=[used(i) for i in range(5)];check(starts[names[0]]() is None and fstatus()==6 and qstatus()==2,'stale texture header rejected before writing')
    check([used(i) for i in range(5)]==before,'rejected header is atomic');clear();put();resources_match(fid,payloads);check(release(fid)==0,'stale owner cleanup')
    setup();listing,ids,payloads=textures();check(reserve(0,4)==0,'cancel reservation');get();check(selector(0)==1,'cancel select');ptr=starts[names[0]]();check(ptr and pinned()==128,'pending resource pin')
    check(t.release_num(0)==1 and orphaned()==128,'release during active batch');check(discard()==0 and pinned()==orphaned()==0,'cancel rolls back and frees orphan');put();check(used(0)==0,'cancel leaves no packets')
    setup();listing,ids,payloads=textures();batch(names[0]);baseline=pinned();batch(names[4],cancel=True);check(pinned()==baseline,'cancel does not remove earlier committed reference');check(reset()==0 and pinned()==0,'queue reset discards recording references')
    # A failed underlying start must undo its newly acquired reference.
    get();check(selector(0)==1,'failed begin select');check(starts[names[0]]() is None and qstatus()==3 and pinned()==0,'missing queue reserve rolls back pin');qc();put()
    groups['shared_release_cancel_stale_and_failed_start']=checks-groups['all_nine_textured_SDK_forms_release_before_submit']
    # Resource capacity/budget and atomic batch preparation.
    setup(resources=1);listing,ids,payloads=textures(2)
    check(pin(ids[0])==0 and pinned()==128,'preflight pin');check(pins((U*2)(ids[0],ids[1]),2)==3 and pinned()==128,'capacity rollback preserves earlier pin');fc();check(reset()==0 and pinned()==0,'prepare cleanup')
    check(pins((U*2)(ids[0],0x1fffff),2)==6 and pinned()==0,'bad second resource rolls back first');fc()
    check(pins((U*3)(ids[0],ids[0],ids[0]),3)==0 and pinned()==128,'duplicate prepare is idempotent');reset()
    meta=accounted();check(fi(4,1,meta+64)==4,'failed reinitialize preserves existing owner');fc();check(accounted()==meta,'owner survives failed replacement')
    qs();check(fs()==0 and qi(16<<20)==0 and fi(1,1,1<<20)==0,'measure minimal owner');meta=accounted();check(fs()==0 and fi(1,1,meta+64)==0,'tight resource budget owner')
    check(pin(ids[0])==4 and pinned()==0,'resource budget failure before any batch');fc()
    setup(slots=1);listing,ids,payloads=textures();batch(names[0]);fid,raw=capture();batch(names[1]);before=[C.string_at(data(i),used(i)) if used(i) else b'' for i in range(5)];out=Q(0x12345678)
    check(submit(C.byref(out))==3 and out.value==0x12345678,'full slot failure preserves output ID');compare(fid,raw);resources_match(fid,payloads)
    check([C.string_at(data(i),used(i)) if used(i) else b'' for i in range(5)]==before,'full slot preserves recording');fc();check(release(fid)==0,'slot release permits retry');fid2,raw2=capture();check(fid2!=fid and release(fid2)==0,'retry reuses slot with a new ID')
    # With two slots, force a BYTE budget failure rather than slot exhaustion.
    setup(slots=2,resources=1);meta=accounted();check(fs()==0 and fi(2,1,meta+128+256)==0,'snapshot byte budget owner');listing,ids,payloads=textures()
    batch(names[0]);fid,raw=capture();batch(names[0]);before=[C.string_at(data(i),used(i)) if used(i) else b'' for i in range(5)];out=Q(0x12345678)
    check(submit(C.byref(out))==4 and out.value==0x12345678 and live()==1,'snapshot allocation budget failure is atomic')
    check([C.string_at(data(i),used(i)) if used(i) else b'' for i in range(5)]==before,'snapshot byte failure retains source stream');compare(fid,raw)
    t.shutdown();check(orphaned()==128,'failed submission retains its pending resource reference');check(release(fid)==0 and pinned()==128,'release earlier snapshot frees bytes without retiring pending texture')
    fc();fid2,raw2=capture();resources_match(fid2,payloads);check(release(fid2)==0 and pinned()==orphaned()==0,'byte-budget retry and final cleanup')
    # Inactive/completed context and owner lifecycle prerequisites.
    setup();listing,ids,payloads=textures();check(reserve(0,4)==0,'active submission reservation');get();selector(0);ptr=starts[names[0]]();out=Q(99)
    check(submit(C.byref(out))==2 and out.value==99,'active submit rejected');fc();end(ptr);check(submit(C.byref(out))==2 and out.value==99,'uncommitted SDK context rejected');fc()
    check(fs()==2,'shutdown refuses an unsent recording');fc();put();fid,raw=capture();check(fs()==0,'empty queue allows owner shutdown');check(release(fid)==9,'shutdown expires handles');fc()
    groups['resource_capacity_budget_slot_and_lifecycle_failures']=checks-sum(groups.values())
    # Distinct resources in four queued frames, with texture records/global IDs
    # repeatedly reused before older frame consumers have finished.
    for iteration in range(16):
        setup();pending=[];all_ids=set()
        for j in range(4):
            check(t.init(8,128,1<<20)==0,'overlapping texture generations');listing,ids,payloads=textures(3)
            check(all_ids.isdisjoint(ids),'cross-frame resource identity');all_ids.update(ids);check(pins((U*3)(*ids),3)==0,'multi-resource preflight')
            batch(names[(iteration+j)%len(names)]);fid,raw=capture();pending.append((fid,raw,payloads))
            for old,stream,pixels in pending:compare(old,stream);resources_match(old,pixels)
            check(live()==len(pending) and pinned()==len(all_ids)*128,'independent overlapping-frame resource accounting')
        check(t.init(8,128,1<<20)==0,'fifth texture generation');listing,ids,payloads=textures(3);check(all_ids.isdisjoint(ids),'fifth resource identity');check(pins((U*3)(*ids),3)==0,'fifth resource preparation')
        batch(names[iteration%len(names)]);t.shutdown();check(orphaned()==15*128,'all five generations protected, including pending recording')
        out=Q(0x12345678);check(submit(C.byref(out))==3 and out.value==0x12345678,'fifth frame waits for a real free slot');fc()
        first,stream,pixels=pending.pop(0);check(release(first)==0 and orphaned()==12*128,'retire only first generation');fid,raw=capture();pending.append((fid,raw,payloads))
        rng.shuffle(pending)
        for old,stream,pixels in pending:compare(old,stream);resources_match(old,pixels);check(release(old)==0,'out-of-order consumer completion')
        check(pinned()==orphaned()==live()==0,'all concurrent generations retired')
    groups['overlapping_resource_generations_and_out_of_order_completion']=checks-sum(groups.values())
    setup();out=Q(123);check(submit(None)==1 and live()==0,'null submission output rejected');fc()
    empty_ids=[]
    for j in range(64):
        fid,stream=capture();check(not any(stream) and count(fid)==0,'empty recording is owned without fabricated geometry');empty_ids.append(fid);check(release(fid)==0,'empty frame release')
    check(len(set(empty_ids))==64 and empty_ids==sorted(empty_ids),'opaque frame IDs never recycle')
    size=S(0x1234);check(flist(empty_ids[0],0,C.byref(size)) is None and size.value==0x1234 and fstatus()==7,'stale frame view preserves output');fc()
    check(flist(1,5,C.byref(size)) is None and size.value==0x1234 and fstatus()==1,'invalid list argument');fc()
    meta=accounted();check(fi(0,1,1<<20)==1 and accounted()==meta,'invalid owner size preserves old owner');fc()
    check(fi(0xfffffffe,0xffffffff,1<<20)==1 and accounted()==meta,'metadata overflow rejected before allocation');fc()
    groups['empty_frames_stale_views_and_invalid_owner_arguments']=checks-sum(groups.values())
    # Real complete model, both sprites and flat textured strips in one frame.
    actual_modes=set()
    for iteration in range(32):
        setup();listing,ids,payloads=textures(3);check(pins((U*3)(*ids),3)==0,'complete producer resource preflight')
        identity=(F*16)(*[1. if i%5==0 else 0. for i in range(16)]);matrix(None,identity);modelinit();setlight(0,0,1);frame_step(0,0);callback(0,1)
        pts=(Vec*8)(*(Vec((i%3-1)*0.25,(i%2-0.5)*0.25,0.5 if (i+iteration)%4==0 else 2.) for i in range(8)));norms=(Vec*8)(*(Vec(0,0,1) for _ in range(8)))
        meshes=(Mesh*3)();materials=(Material*3)();spans=(Span*3)();arrays=[]
        for j in range(3):
            indices=(C.c_int16*3)(j,j+1,j+2);uvs=(C.c_int16*6)(0,0,255,0,128,255);arrays.extend([indices,uvs]);flags=0x200000|(0x400000 if j==2 else 0)|(0x100000 if j==1 else 0)
            meshes[j]=Mesh(j,1,indices,None,None,None,C.cast(uvs,P));materials[j]=Material(0xffffffff,0,1.,j,flags);spans[j]=Span(3,3)
        model=Model(pts,norms,8,meshes,materials,3,3,Vec(0,0,2),3.);bound=Bound()
        check(measure(C.byref(model),8,8,3,3,spans,C.byref(bound))==0 and modelreserve(C.byref(bound))==0,'actual complete model validation and bound')
        modelbegin();drawmodel(C.byref(model));modelend();check(qstatus()==fstatus()==0 and guard()==0,'complete model producer with retention');actual_modes.add('model')
        animation=Frame(16,16,8,8,0,0,256,256,0,0);sprite=Sprite(Point(320,240,2),1,1,0,C.addressof(listing),C.addressof(animation))
        draw2(C.byref(sprite),0,F(0.5),0x10);sprite.p=Point(0,0,2);draw3(C.byref(sprite),0,0x10);actual_modes.update(['sprite2d','sprite3d'])
        vertices=(U*18)(*[rng.getrandbits(32) for _ in range(18)]);drawflat(vertices,3,1002,0);actual_modes.add('flat_strip')
        check(qstatus()==fstatus()==0 and guard()==0,'combined real drawing parents');check(any(used(i) for i in range(3)),'actual parents emitted packets');real_draws+=4
        before=[C.string_at(data(i),used(i)) if used(i) else b'' for i in range(5)];t.shutdown();check(orphaned()==384,'all real draw resources retained before submit')
        fid,raw=capture();check(raw==before,'actual producer bytes captured exactly');resources_match(fid,payloads)
        # Reusing the same record/global IDs cannot alias the old surface IDs.
        check(t.init(8,128,1<<20)==0,'reuse texture owner');listing2,newids,newpayloads=textures(3);check(set(ids).isdisjoint(newids),'surface identities never recycle');compare(fid,raw);resources_match(fid,payloads)
        check(release(fid)==0 and pinned()==orphaned()==0,'real producer frame release');check(t.status()==0,'new texture owner unaffected')
    groups['complete_actual_drawing_producers_and_owner_reuse']=checks-sum(groups.values())
    qs();t.shutdown();check(fs()==0 and accounted()==pinned()==orphaned()==live()==0,'complete owner cleanup')
    report=dict(status='actual_native_frame_capture_resource_lifetime_and_rollback_verified',checks=checks,groups=groups,submitted_frames=submitted,actual_complete_parent_draws=real_draws,actual_producers=sorted(actual_modes),
        all_nine_textured_SDK_start_forms=True,release_before_submission=True,queue_texture_shutdown_and_reinitialize_survived=True,cancelled_batches_release_only_their_new_references=True,atomic_prepare_and_submission_failures=True,overlapping_texture_generations=5,out_of_order_completion_runs=16,empty_submissions_verified=64,actual_nonempty_producer_lists=[0,1,2],modifier_list_producers_verified=False,
        DLL_sha256=sha(dll),build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(Path(__file__)),
        qualification_scope='single game-thread real native owners, committed raw packets and encoded owned payload lifetimes; submitted frame views are read-only; existing payloads have no writable-update API',
        render_flip_implemented=False,texture_decoding_verified=False,full_frame_executed=False,rendered_pixels_verified=False,playable=False,overall_completion_percent=10,goal_paused=False)
    a.report.write_text(json.dumps(report,indent=2)+'\n');print(report['status'],checks,'checks;',submitted,'submissions;',real_draws,'real parent draws',flush=True)
if __name__=='__main__':main()
