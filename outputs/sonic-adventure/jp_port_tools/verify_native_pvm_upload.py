"""Bounded original PVM metadata plus real texture ownership, palettes and GPU.
Retail inputs are read privately. Reports contain hashes/counts, never pixels.
"""
import argparse,ctypes as C,hashlib,json,struct,threading
from pathlib import Path
from native_packet_test_support import bind
from native_texture_test_support import Runtime,Record,Name,List
from native_gpu_test_support import GPU,Config,Report,Failure
from native_video_test_support import Video
from native_pvm_test_support import PVM,chunk,texture,palette,archive,destination,index_archive,image_oracle
U=C.c_uint32;I=C.c_int;P=C.c_void_p;S=C.c_size_t;Q=C.c_uint64
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def fw(x):return struct.unpack('<I',struct.pack('<f',x))[0]
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','inputs','report']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.build.resolve();b=json.loads((d/'build_verification.json').read_text());assert not a.report.exists()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n in ['frames_DLL','DLL','GPU_DLL']:assert sha(Path(b[n]))==b[n+'_sha256']
    runtime=Runtime(Path(b['frames_DLL']));lib=runtime.dll;pvm=PVM(lib);video=Video(lib);checks=0;groups={};keep=[]
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    def recover():pvm.unbind();pvm.end();pvm.clear();runtime.clear();video.clear()
    def init(records=64,bindings=256,budget=32<<20):
        recover();check(runtime.init(records,bindings,budget)==0,'actual native record owner initialized')
    qi=bind(lib,'jp_queue_initialize',I,S);qs=bind(lib,'jp_queue_shutdown',None);reset=bind(lib,'jp_queue_reset_frame',I);ctx=bind(lib,'jp_queue_set_context',I,U,P)
    fi=bind(lib,'jp_frames_initialize',I,U,U,S);fs=bind(lib,'jp_frames_shutdown',I);submit=bind(lib,'jp_frames_submit',I,P);release=bind(lib,'jp_frame_release',I,Q)
    reserve=bind(lib,'jp_queue_reserve',I,U,U);get=bind(lib,'njGetkmPointer',None);put=bind(lib,'njSetkmPointer',None);endvertex=bind(lib,'njEndVertex',None,P);starts=[bind(lib,n,P) for n in ['njStartVertexTexOpaque','njStartVertexTexTrans']];pin=bind(lib,'jp_frames_prepare_texture',I,U)
    frame_list=bind(lib,'jp_frame_list',P,Q,U,P)
    check(qi(8<<20)==0 and fi(8,128,32<<20)==0 and video.init(64,64,0xff123456,1)==0,'actual shared queue/frame/device/video initialized')
    init()
    # Every supported header-field combination, out-of-order entry numbers,
    # exact original metadata copy and actual surface payload/reference release.
    header_cases=0
    for flags in range(16):
        for extra in [0,0x100]:
            init();numbers=[2,0,1];gids=[1000+x for x in numbers];data=archive([texture(bytes([i+1])*128) for i in range(3)],flags|extra,numbers,gids)
            names,listing,buffers=destination(3,gids);src=C.create_string_buffer(data);keep.extend([names,listing,buffers,src]);before=[v.raw for v in buffers]
            stride=2+sum(n for flag,n in [(1,28),(2,2),(4,2),(8,4)] if flags&flag)
            check(pvm.stride(flags|extra)==stride,'original SDK entry stride covers all header fields')
            check(pvm.load(C.byref(listing),3,28,src,len(data))==1 and pvm.status()==runtime.status()==0,'bounded real original metadata and memory load')
            check(listing.nbTexture==3 and pvm.loaded(C.byref(listing))==3 and runtime.live()==3,'actual records acquired for all mapped entries')
            for i,number in enumerate(numbers):
                record=Record.from_address(runtime.lookup(C.byref(listing),i));surface=record.texinfo.texsurface;word=runtime.surface_word(C.byref(surface))
                check(names[i].attr&0xffff==number and record.globalIndex==gids[i] and record.count==1,'actual SDK entry number and global ID mapping')
                check(C.string_at(runtime.surface_data(word),128)==bytes([number+1])*128,'PVR sequence maps into its numbered name')
                check(buffers[i].raw[:16]==b'\xa5'*16 and buffers[i].raw[44:64]==b'\xa5'*20,'metadata copy stays within 28-byte output')
                check(buffers[i].raw[16:44]==(('texture_'+str(i)).encode().ljust(28,b'\0') if flags&1 else before[i][16:44]),'filename metadata or untouched filename output')
            check(pvm.release(C.byref(listing))==1 and runtime.live()==0 and pvm.loaded(C.byref(listing))==0,'actual mapped record release')
            header_cases+=1
    groups['all_header_fields_original_helpers_mapping_names_and_release']=checks
    # Native SDK memory entry point requires both explicit capacity and source
    # views. Bounded original cursor handles an archive at a nonzero source base.
    data=archive([texture(bytes(128))]);names,listing,buffers=destination(1);src=C.create_string_buffer(b'pre'+data+b'post');addr=C.addressof(src)+3
    check(pvm.sdk(addr,C.byref(listing))==-1 and pvm.status()==2,'SDK call without bound destination/source refused');recover()
    check(pvm.begin(src,len(src))==0 and pvm.destination(C.byref(listing),1,28)==0,'explicit borrowed source and output views')
    check(pvm.shutdown()==2 and pvm.active()==1,'memory owner cannot shut down while output view is bound');pvm.clear()
    check(pvm.sdk(addr,C.byref(listing))==1,'original cursor table copy at nonzero source offset')
    check(pvm.unbind()==0 and pvm.end()==0 and pvm.release(C.byref(listing))==1,'low-level scope ends and records release')
    # Palette banks share real surface storage. ARGB8888 is four bytes per
    # color in a bank other than 6; final context bank is bank, not mode.
    init();numbers=[2,0,1];banks=[32,0,16];gids=[(bank<<26)|777 for bank in banks]
    colors=[0xffff0000,0xff00ff00,0xff0000ff];pal=b''.join(palette(6,bank,7,[color]) for bank,color in zip([0,16,32],colors))
    data=archive([texture(bytes([0x77])*32,0x500) for _ in range(3)],0x409,numbers,gids,palettes=pal)
    names,listing,buffers=destination(3);src=C.create_string_buffer(data)
    check(pvm.load(C.byref(listing),3,28,src,len(data))==1 and runtime.live()==1,'banked PVM global index store and other-bank surface sharing')
    snap=video.snapshot();check(snap.palette_mode==3 and [snap.palette[bank*16+7] for bank in [0,16,32]]==colors and pvm.bank()==32,'real 32-bit palette payload and corrected context bank')
    for i,bank in enumerate(banks):
        record=Record.from_address(runtime.lookup(C.byref(listing),i))
        check(record.globalIndex==777 and record.bank==bank and record.texinfo.texsurface.BitDepth==3 and names[i].attr&0x1000000,'banked header metadata retained in actual SDK records')
    check(pvm.release(C.byref(listing))==1 and runtime.live()==0,'banked sharing releases final surface once')
    palette_cases=0
    for mode,color in [(0,0xfc00),(1,0x07e0),(2,0xf00f),(6,0xff34ab78)]:
        init();data=archive([texture(bytes([7])*64,0x700)],0x408,gids=[(16<<26)|888],palettes=palette(mode,16,7,[color]));names,listing,buffers=destination(1);src=C.create_string_buffer(data)
        check(pvm.load(C.byref(listing),1,28,src,len(data))==1,'all valid palette source modes load')
        snap=video.snapshot();check(snap.palette_mode==(3 if mode==6 else mode) and snap.palette[263]==color and pvm.bank()==16,'palette native register oracle')
        check(pvm.release(C.byref(listing))==1,'palette fixture released');palette_cases+=1
    init();data=archive([texture(bytes([0xff])*32,0x500)],0x408,gids=[(63<<26)|999],palettes=palette(6,63,15,[0xffabcdef]));names,listing,buffers=destination(1);src=C.create_string_buffer(data)
    check(pvm.load(C.byref(listing),1,28,src,len(data))==1 and video.snapshot().palette[1023]==0xffabcdef,'last palette word is written, without end clipping bug')
    check(pvm.release(C.byref(listing))==1,'last-word fixture released')
    init();names,listing,buffers=destination(0);data=archive([]);src=C.create_string_buffer(data)
    check(pvm.load(C.byref(listing),0,0,src,len(data))==1 and listing.nbTexture==0 and runtime.live()==0,'empty valid archive changes no textures')
    groups['explicit_views_palette_banks_modes_and_sharing']=checks-sum(groups.values())
    # Complete validation before writing output lists/names/palette, including
    # every truncated byte prefix of a valid single-texture archive.
    init();valid=archive([texture(bytes(128))]);failures=[]
    for cut in range(len(valid)):failures.append((valid[:cut],1,28,None))
    def changed(data,offset,fmt,value):
        out=bytearray(data);struct.pack_into(fmt,out,offset,value);return bytes(out)
    failures += [(b'NOPE'+valid[4:],1,28,None),(changed(valid,10,'<h',-1),1,28,None),(valid,0,28,None),
        (changed(valid,4,'<I',3),1,28,None),(changed(valid,12,'<H',1),1,28,None),(valid,1,27,None),
        (valid,1,28,'null_filename'),(valid,1,28,'source_filename')]
    two=archive([texture(bytes(128)),texture(bytes(128))]);size=struct.unpack_from('<I',two,4)[0]+8
    failures += [(changed(two,12+34,'<H',0),2,28,None),(two,2,28,'duplicate_filename'),
        (changed(valid,struct.unpack_from('<I',valid,4)[0]+16,'<I',0xb00),1,28,None),
        (changed(valid,struct.unpack_from('<I',valid,4)[0]+16,'<I',0x903),1,28,None)]
    for pal in [palette(6,16,7,[1])[:-1],palette(5,16,7,[1]),palette(6,64,0,[1]),palette(6,0,-1,[1]),palette(6,63,15,[1,2])]:
        failures.append((archive([texture(bytes(128))],palettes=pal),1,28,None))
    invalid_cases=0
    for data,capacity,filename_bytes,scenario in failures:
        recover();names,listing,buffers=destination(2 if capacity==2 else 1);src=C.create_string_buffer(data)
        if scenario=='null_filename':names[0].filename=None
        if scenario=='source_filename':names[0].filename=C.addressof(src)
        if scenario=='duplicate_filename':names[1].filename=names[0].filename
        old_names=bytes(names);old_list=bytes(listing);old_buffers=[buf.raw for buf in buffers];old_video=bytes(video.snapshot());allocated=runtime.allocated()
        check(pvm.load(C.byref(listing),capacity,filename_bytes,src,len(data))==-1 and pvm.status()!=0,'malformed/bounded archive rejected')
        check(bytes(names)==old_names and bytes(listing)==old_list and [buf.raw for buf in buffers]==old_buffers and bytes(video.snapshot())==old_video,'validation failure leaves output metadata and palette unchanged')
        check(runtime.live()==0 and runtime.allocated()==allocated,'validation failure creates no native surfaces');invalid_cases+=1
    recover();arena=C.create_string_buffer(archive([texture(bytes(128))],flags=8));overlap=(Name*1).from_buffer(arena,32);listing=List(overlap,1);before=arena.raw
    check(pvm.load(C.byref(listing),1,0,arena,len(arena)-1)==-1 and arena.raw==before,'source/name-array overlap rejected before metadata writes');recover()
    # Runtime failure keeps safely owned progress for deliberate release; shared
    # aliases are forgotten so no dangling field bindings survive cleanup.
    init(1,8);data=archive([texture(bytes(128)) for _ in range(3)],gids=[1,1,2]);names,listing,buffers=destination(3);src=C.create_string_buffer(data);base_alloc=runtime.allocated()
    check(pvm.load(C.byref(listing),3,28,src,len(data))==-1 and pvm.loaded(C.byref(listing))==2 and runtime.live()==1,'full record table preserves two successful aliases')
    check(pvm.release(C.byref(listing))==-1,'recovery requires deliberate error clear');recover()
    check(pvm.release(C.byref(listing))==1 and pvm.loaded(C.byref(listing))==0 and runtime.live()==0 and runtime.allocated()==base_alloc,'partial archive cleanup releases alias surface once and clears bindings')
    init(1,8);data=archive([texture(bytes(128)),texture(bytes(128))],flags=1);names,listing,buffers=destination(2,[101,102]);src=C.create_string_buffer(data)
    check(pvm.load(C.byref(listing),2,28,src,len(data))==-1 and pvm.loaded(C.byref(listing))==1,'archive without stored global IDs preserves partial progress')
    recover();check(pvm.release(C.byref(listing))==1 and [v.texaddr for v in names]==[101,102],'cleanup restores caller global IDs for archive retry')
    init(2,8);check(pvm.load(C.byref(listing),2,28,src,len(data))==1 and pvm.loaded(C.byref(listing))==2,'retry without global-ID table succeeds after capacity recovery')
    check(pvm.release(C.byref(listing))==1,'retry cleanup releases all actual surfaces')
    init();src=C.create_string_buffer(valid);names,listing,buffers=destination(1)
    check(pvm.begin(src,len(valid))==0 and pvm.destination(C.byref(listing),1,28)==0,'thread ownership fixture')
    foreign=[]
    def other():foreign.extend([pvm.sdk(C.addressof(src),C.byref(listing)),pvm.unbind(),pvm.loaded(C.byref(listing)),pvm.status()])
    thread=threading.Thread(target=other);thread.start();thread.join();check(foreign==[-1,6,0,6] and pvm.status()==0 and pvm.active()==1,'foreign thread cannot mutate active PVM/source scope')
    check(pvm.sdk(C.addressof(src),C.byref(listing))==1 and pvm.unbind()==pvm.end()==0 and pvm.release(C.byref(listing))==1,'owner finishes real SDK call and releases')
    groups['malformed_bounds_capacity_output_preservation_failure_cleanup_and_thread']=checks-sum(groups.values())
    # Real native GPU draw from PVM-owned pixels, independently decoded here.
    gpu=GPU(Path(b['GPU_DLL']));config=Config.make();retail_images=0;alpha_images=0;pixels=0;asset_reports=[]
    bound=bind(gpu.lib,'jp_gpu_draw_bound_frame',I,P,Q,P,P,S,P,P)
    def draw(frame):
        output=C.create_string_buffer(64*64*4);report=Report();failure=Failure()
        result=bound(gpu.handle,frame,C.byref(config),output,len(output),C.byref(report),C.byref(failure))
        return result,output.raw,report,failure
    def record(listing,index,lane=0):
        pointer=runtime.lookup(C.byref(listing),index);record=Record.from_address(pointer);tcw=record.texparambuffer;tsp=record.tspparambuffer
        check(reset()==0 and reserve(lane,4)==0 and ctx(0,(U*4)(0x8000000a|(lane*2<<24),0xe0800000,(0x20900000 if lane else 0x20800000)|tsp,tcw))==0 and pin(tcw&0x1fffff)==0,'real PVM texture recording and retention')
        get();pointer=starts[lane]();check(bool(pointer),'real SDK packet writer');words=[]
        for i,(x,y,u,v) in enumerate([(-64,-64,-1,-1),(192,-64,3,-1),(-64,192,-1,3)]):words += [0xf0000000 if i==2 else 0xe0000000,fw(x),fw(y),fw(1),fw(u),fw(v),0xffffffff,0]
        raw=struct.pack('<'+'I'*len(words),*words);C.memmove(pointer,raw,len(raw));endvertex(pointer+len(raw));put()
        frame=Q();check(submit(C.byref(frame))==0,'actual immutable frame captures PVM texture')
        size=S();header=(U*4).from_address(frame_list(frame.value,lane,C.byref(size)))
        check(bool(header[2]&0x80000)==(lane==0) and (header[2]>>6&3)==0,'actual SDK opaque ignore-alpha versus translucent decal contract')
        return frame.value
    inputs=json.loads((a.inputs/'input_manifest.json').read_text())
    for asset in inputs['assets']:
        path=Path(asset['PVM']);check(sha(path)==asset['PVM_sha256'],'unchanged supplied-disc PVM hash');data=path.read_bytes();flags,rows,images=index_archive(data);init();names,listing,buffers=destination(len(rows));src=C.create_string_buffer(data)
        check(pvm.load(C.byref(listing),len(rows),28,src,len(data))==1 and pvm.loaded(C.byref(listing))==len(rows),'supplied-disc archive loads all textures through real original metadata helpers')
        for sequence,image in enumerate(images):
            n=next(i for i,row in enumerate(rows) if row['number']==sequence);record_info=Record.from_address(runtime.lookup(C.byref(listing),n));check(record_info.globalIndex==rows[n]['gid'],'retail header global ID preserved')
            frame=record(listing,n);rc,rgba,report,failure=draw(frame);expected=image_oracle(image,ignore_alpha=True)
            check(rc==0 and rgba==expected,('retail GPU image matches independent Morton/channel/sampling oracle',path.name,sequence,rc,failure.code,rgba[:16].hex(),expected[:16].hex()))
            check(release(frame)==0,'retail frame releases actual resource');retail_images+=1;pixels+=4096
            if image['kind'] in [0x902,0x102] and (sequence==8 or sequence==14):
                frame=record(listing,n,1);rc,rgba,report,failure=draw(frame);expected=image_oracle(image)
                check(rc==0 and rgba==expected,('retail translucent image preserves independent ARGB4444 alpha',path.name,sequence,rc,rgba[:16].hex(),expected[:16].hex()))
                check(release(frame)==0,'alpha frame retired');alpha_images+=1;pixels+=4096
        check(pvm.release(C.byref(listing))==1 and runtime.live()==0,'all supplied-disc texture records released')
        asset_reports.append(dict(PVM_sha256=asset['PVM_sha256'],disc_path=asset['disc_path'],texture_count=len(images),GPU_images=len(images),pixels_compared=len(images)*4096))
    # Old frames retain BOTH the surface and captured palette when a subsequent
    # PVM changes the live palette and reuses a texture global index.
    init();frames=[];old_lists=[]
    for color in [0xff00ff00,0xff0000ff]:
        data=archive([texture(bytes([0x77])*32,0x500)],0x408,gids=[(16<<26)|456],palettes=palette(6,16,7,[color]));names,listing,buffers=destination(1);src=C.create_string_buffer(data);keep.extend([names,listing,buffers,src])
        check(pvm.load(C.byref(listing),1,28,src,len(data))==1,'new PVM palette update');frame=record(listing,0);frames.append(frame);old_lists.append(listing)
        check(pvm.release(C.byref(listing))==1,'live PVM releases while old captured frame retains ownership')
    for frame,rgba in zip(frames,[bytes([0,255,0,255])*4096,bytes([0,0,255,255])*4096]):
        rc,actual,report,failure=draw(frame);check(rc==0 and actual==rgba,('captured PVM palette and texture survive later archive mutation',rc,failure.code,actual[:16].hex()));check(release(frame)==0,'retained palette frame retired');pixels+=4096
    gpu.close(gpu.handle);check(runtime.live()==0,'no live PVM surfaces after draw/retire');qs();runtime.shutdown();check(fs()==0 and pvm.shutdown()==0,'all original and native owners shut down')
    groups['supplied_disc_PVM_GPU_images_and_immutable_palette_lifetimes']=checks-sum(groups.values())
    result=dict(status='bounded_PVM_memory_original_metadata_real_upload_palette_and_disc_pixels_verified',checks=checks,groups=groups,header_field_cases=header_cases,palette_mode_cases=palette_cases,invalid_archive_cases=invalid_cases,
        supplied_disc_archives=asset_reports,retail_GPU_images_compared=retail_images,retail_translucent_alpha_GPU_images_compared=alpha_images,synthetic_palette_GPU_images_compared=2,pixels_compared=pixels,
        six_original_PVM_metadata_helpers_retained=True,banked_index_and_palette_mode_bank_repairs_verified=True,explicit_source_and_output_capacities=True,validation_failure_preserves_names_list_and_palette=True,
        partial_runtime_failure_owned_progress_cleanup_verified=True,shared_palette_and_texture_frame_retention_verified=True,source_game_bodies_and_retail_assets_published=False,
        full_game_loop_executed=False,playable=False,overall_completion_percent=10,build_report_sha256=sha(d/'build_verification.json'),input_manifest_sha256=sha(a.inputs/'input_manifest.json'),
        verifier_sha256=sha(Path(__file__)),support_sha256=sha(Path(__file__).with_name('native_pvm_test_support.py')))
    a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
if __name__=='__main__':main()
