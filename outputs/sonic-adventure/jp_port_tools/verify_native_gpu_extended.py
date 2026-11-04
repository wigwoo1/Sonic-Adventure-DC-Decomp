"""All ordinary GPU vertex layouts, native flat choices and retained generations."""
import argparse,ctypes as C,hashlib,json,struct
from pathlib import Path
from native_gpu_test_support import GPU,Config,Report,Failure
from native_packet_test_support import Decoder,Plan,Failure as PF,bind
from native_texture_test_support import Runtime,Info,Name,List,Record,field
from verify_native_packets import fixture
from verify_native_gpu import expected,fw
U=C.c_uint32;S=C.c_size_t;P=C.c_void_p;Q=C.c_uint64;I=C.c_int
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','report']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.build.resolve();assert not a.report.exists()
    b=json.loads((d/'build_verification.json').read_text());primary=json.loads((d/'verification.json').read_text())
    assert primary['verifier_sha256']==sha(Path(__file__).parent/'verify_native_gpu.py') and primary['DLL_sha256']==b['DLL_sha256']==sha(Path(b['DLL']))
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h
    runtime=Runtime(Path(b['frames_DLL']));lib=runtime.dll;gpu=GPU(Path(b['DLL']));decoder=Decoder(Path(b['DLL']))
    qi=bind(lib,'jp_queue_initialize',I,S);qs=bind(lib,'jp_queue_shutdown',None);reset=bind(lib,'jp_queue_reset_frame',I);ctx=bind(lib,'jp_queue_set_context',I,U,P)
    fi=bind(lib,'jp_frames_initialize',I,U,U,S);fs=bind(lib,'jp_frames_shutdown',I);submit=bind(lib,'jp_frames_submit',I,C.POINTER(Q));release=bind(lib,'jp_frame_release',I,Q)
    reserve=bind(lib,'jp_queue_reserve',I,U,U);get=bind(lib,'njGetkmPointer',None);put=bind(lib,'njSetkmPointer',None);end=bind(lib,'njEndVertex',None,P)
    pin=bind(lib,'jp_frames_prepare_texture',I,U);qs_status=bind(lib,'jp_queue_status',I)
    starts={n:bind(lib,n,P,*([P,P] if n.endswith('H') else [P] if 'Intens' in n else [])) for n in ['njStartVertexNonTexTrans','njStartVertexTexTrans','njStartVertexNonTexTransIntens','njStartVertexTexTransIntens','njStartVertexTexTransIntensH']}
    checks=pixels=images=0;layouts=set();keep=[];textures={};groups={}
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,label
    check(qi(64<<20)==0 and fi(8,32,64<<20)==0 and runtime.init(16,128,8<<20)==0,'extended owners initialize')
    palette=(U*1024)(*[0xff20c060]*1024)
    def surface(fmt=1,word=0x07e0,vq=False,palbank=0):
        info=Info();s=info.texsurface;s.Type=0x10000;s.BitDepth=1;s.PixelFormat=fmt<<27;s.nWidth=s.nHeight=8;s.fSurfaceFlags=4|(8 if vq else 0)|(0x20 if fmt>=5 else 0)
        if vq:raw=struct.pack('<4H',*([word]*4))+bytes(2040+16)
        elif fmt==5:raw=bytes([0x55]*32)
        elif fmt==6:raw=bytes([0x55]*64)
        else:raw=struct.pack('<H',word)*64
        s.TextureSize=len(raw);check(runtime.copy(C.byref(info),C.create_string_buffer(raw),len(raw))==0,'extended encoded copy')
        names=(Name*1)();runtime.setname(C.byref(names[0]),None,2000+len(keep),0);runtime.entry(field(names[0]),C.byref(info),2000+len(keep),2,0xffffffff);address=runtime.resolve(names[0].texaddr);runtime.param(address)
        check(runtime.status()==0,'extended actual SDK parameter');tcw=Record.from_address(address).texparambuffer
        # Explicit palette selector is frame configuration state, not a cast
        # of the registered host surface pointer into a GPU address.
        if fmt>=5:tcw=(tcw&~0x07e00000)|(palbank<<(21 if fmt==5 else 25))
        keep.append((info,names,List(names,1)));sid=tcw&0x1fffff
        if fmt==1:color=[((word>>11)&31)/31,((word>>5)&63)/63,(word&31)/31,1]
        elif fmt==0:color=[((word>>10)&31)/31,((word>>5)&31)/31,(word&31)/31,1 if word&0x8000 else 0]
        elif fmt==2:color=[((word>>8)&15)/15,((word>>4)&15)/15,(word&15)/15,(word>>12)/15]
        else:color=[32/255,192/255,96/255,1]
        textures[sid]=(8,8,[color]*64);return tcw
    tcw=surface()
    def emit(ct=0,tex=1,uv16=0,off=0,tcw=tcw,gouraud=True,tsp=0x209000c0,isp=0xe0800000):
        raw,typ,_=fixture(ct,tex,uv16,off,3,1);header_size=64 if ct==2 and off else 32;raw=bytearray(raw);control=struct.unpack_from('<I',raw)[0]
        if not gouraud:control&=~2
        check(reserve(1,8)==0 and ctx(0,(U*4)(control,isp,tsp,tcw))==0,'complete extended batch bound/context')
        if tex:check(pin(tcw&0x1fffff)==0,'extended texture prepared')
        get();face=(U*4)(*[fw(v) for v in [0.625,0.25,0.5,0.75]]);spec=(U*4)(*[fw(v) for v in [0.125,0.875,0.375,0.0625]])
        name='njStartVertex'+('Tex' if tex else 'NonTex')+'Trans'+('IntensH' if ct==2 and off and tex else 'Intens' if ct==2 else '')
        # Non-textured offset format does not exist in this layout coverage.
        ptr=starts[name](*([face,spec] if name.endswith('H') else [face] if ct==2 else []));check(bool(ptr),'real extended SDK header')
        stride=64 if typ in [5,6] else 32;vertices=raw[header_size:]
        for j,(x,y,q) in enumerate([(8,8,0.5),(56,8,0.25),(8,56,0.75)]):
            at=j*stride;struct.pack_into('<3f',vertices,at+4,x,y,q)
            if ct==0:struct.pack_into('<I',vertices,at+24,[0x80ff0000,0xc000ff00,0xff0000ff][j])
            elif ct==1:
                first=32 if tex else 16;struct.pack_into('<4f',vertices,at+first,0.25+j*0.25,1 if j==0 else 0,1 if j==1 else 0,1 if j==2 else 0)
            else:struct.pack_into('<f',vertices,at+24,j*0.5)
        C.memmove(ptr,bytes(vertices),len(vertices));end(ptr+len(vertices));put();check(qs_status()==0,'extended batch complete')
        return typ
    def capture():
        q=Q();check(submit(C.byref(q))==0,'extended actual frame');return q.value
    def compare(fid,cfg):
        nonlocal pixels,images
        rc,data,rep,e=gpu.image(fid,cfg);check(rc==0,('extended GPU draw',rc,e.batch,e.hresult));pp=P();pe=PF();check(decoder.frame(fid,64<<20,C.byref(pp),C.byref(pe))==0,'extended packet plan');plan=Plan.from_address(pp.value)
        for y in range(64):
            for x in range(64):
                want,uncertain=expected(plan,cfg,x,y,textures)
                if uncertain:continue
                got=data[(y*64+x)*4:(y*64+x+1)*4];check(max(abs(v-w) for v,w in zip(got,want))<=2,('extended native pixel',x,y,list(got),list(want)));pixels+=1
        decoder.destroy(pp);images+=1;return data,rep
    for ct,tex,uv16 in [(0,0,0),(1,0,0),(2,0,0)]+[(c,1,u) for c in range(3) for u in range(2)]:
        for gouraud,flat in [(True,0),(False,0),(False,2)]:
            check(reset()==0,'layout reset');typ=emit(ct,tex,uv16,int(bool(tex)),gouraud=gouraud);layouts.add(typ);fid=capture();cfg=Config.make();cfg.flat_color_vertex=flat;compare(fid,cfg);check(release(fid)==0,'layout frame done')
    groups['all_nine_GPU_layouts_Gouraud_first_and_last_flat']=checks
    for fmt,vq,bank,word in [(0,False,0,0xffff),(2,False,0,0xf248),(1,True,0,0x07e0),(5,False,63,0),(6,False,3,0)]:
        selected=surface(fmt,word,vq,bank);check(reset()==0,'format reset');emit(tcw=selected);fid=capture();cfg=Config.make();cfg.palette=C.addressof(palette);cfg.palette_entries=1024;_,rep=compare(fid,cfg);check(rep.texture_uploads==1 and rep.decoded_texels==64,'actual format upload');check(release(fid)==0,'format frame done')
    groups['GPU_1555_4444_VQ_and_palette_banks']=checks-sum(groups.values())
    # Retain an old resource across release/reinitialization, draw a fresh
    # generation and then draw/complete the older frame out of order.
    check(reset()==0,'retained GPU reset');emit(tcw=tcw);old=capture();cfg=Config.make();before,_=compare(old,cfg);runtime.shutdown();after,_=compare(old,cfg);check(before==after,'GPU reads old payload after actual SDK texture shutdown')
    check(runtime.init(16,128,8<<20)==0,'replace texture owner');fresh=surface(1,0xf800);check((fresh&0x1fffff)!=(tcw&0x1fffff),'new GPU resource generation')
    check(reset()==0,'fresh recording');emit(tcw=fresh);new=capture();new_image,_=compare(new,cfg);old_image,rep=compare(old,cfg);check(old_image==before and new_image!=old_image,'overlapping GPU generations stay distinct')
    check(release(new)==0 and release(old)==0,'GPU out-of-order final release')
    check(reset()==0,'budget recording');emit(tcw=fresh);fid=capture();image,rep=compare(fid,cfg);cfg.budget=rep.accounted_payload_bytes;exact,_=compare(fid,cfg);check(exact==image,'exact accounted payload budget succeeds')
    cfg.budget-=1;out=C.create_string_buffer(b'Q'*(64*64*4));rr=Report();C.memset(C.byref(rr),0x7e,C.sizeof(rr));saved=bytes(rr);e=Failure()
    check(gpu.draw(gpu.handle,fid,C.byref(cfg),out,64*64*4,C.byref(rr),C.byref(e))==7,'one byte below budget fails before output')
    check(out.raw[:-1]==b'Q'*(64*64*4) and bytes(rr)==saved,'GPU budget transaction preserves output/report');check(release(fid)==0,'budget frame done')
    groups['retained_generations_out_of_order_and_exact_budget']=checks-sum(groups.values())
    gpu.close(gpu.handle);qs();runtime.shutdown();check(fs()==0,'extended all owners clean')
    result=dict(status='native_GPU_layouts_formats_and_retention_verified',checks=checks,images_compared=images,interior_pixels_compared=pixels,vertex_layouts=sorted(layouts),groups=groups,
        real_texture_shutdown_and_reinitialize_survived=True,old_and_new_GPU_resources_disjoint=True,out_of_order_frame_completion_verified=True,exact_payload_budget_failure_and_retry=True,
        native_first_and_last_flat_choices_verified=True,console_flat_convention_verified=False,console_pixel_fidelity_verified=False,
        primary_verification_sha256=sha(d/'verification.json'),build_report_sha256=sha(d/'build_verification.json'),verifier_sha256=sha(Path(__file__)),
        reference_source_sha256=sha(Path(__file__).parent/'verify_native_gpu.py'),gameplay_verified=False,playable=False,overall_completion_percent=10)
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
if __name__=='__main__':main()
