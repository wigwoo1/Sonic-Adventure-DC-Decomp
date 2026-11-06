"""Real retail character bindings -> retained frames -> D3D pixels/window."""
import argparse,ctypes as C,faulthandler,hashlib,json,struct
from pathlib import Path
from native_packet_test_support import bind
from native_texture_test_support import Runtime,List,Record
from native_asset_test_support import Assets,Limits
from native_pvm_test_support import PVM,index_archive,morton,color16
from native_video_test_support import Video
from native_gpu_test_support import GPU,Config,Report,Failure
from verify_native_character_textures import Definition
from capture_native_boot_progress import png
I,U,Q,P,S=C.c_int32,C.c_uint32,C.c_uint64,C.c_void_p,C.c_size_t
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def fw(v):return struct.unpack('<I',struct.pack('<f',v))[0]
def oracle(image,outw=None,outh=None):
    """Decode direct PVM bytes independently of native surfaces/packets/texels.
    Offsets follow pinned sdk/ninja/kmtex_kamui_lib.c. Mip prefixes are geometric
    series; VQ uses its four-colour codebook, Morton block index and local XY.
    Only the largest level and observed 16-bit character formats are in scope.
    """
    kind=image['kind'];category=kind>>8;fmt=kind&255;w,h=image['w'],image['h'];src=image['pixels'];assert fmt in (0,1,2) and category in (1,2,3,4,9)
    offset=2+2*(w*w-1)//3 if category==2 else 2048+(1+(w*w//4-1)//3 if category==4 else 0) if category in (3,4) else 0
    outw=w if outw is None else outw;outh=h if outh is None else outh;result=bytearray()
    for y in range(outh):
        sy=(2*y+1)*h//(2*outh)
        for x in range(outw):
            sx=(2*x+1)*w//(2*outw)
            if category in (3,4):
                code=src[offset+morton(sx//2,sy//2,w//2,h//2)];at=code*8+((sx%2)*2+sy%2)*2
            else:at=offset+2*(sy*w+sx if category==9 else morton(sx,sy,w,h))
            assert at+2<=len(src);pixel=color16(struct.unpack_from('<H',src,at)[0],fmt);result+=pixel[:3]+b'\xff'
    return bytes(result)
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','inputs','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=read(a.build/'build_verification.json');iv=read(a.inputs/'inputs.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(b['frames_DLL'])==b['frames_DLL_sha256'] and sha(b['GPU_DLL'])==b['GPU_DLL_sha256']
    fault=(d/'native_fault.txt').open('w');faulthandler.enable(file=fault,all_threads=True);checks=images=pixels=0;reports=[];shots=[];aliases=[]
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    t=Runtime(Path(b['frames_DLL']));lib=t.dll;assets=Assets(lib);pvm=PVM(lib);video=Video(lib);gpu=GPU(Path(b['GPU_DLL']));gl=gpu.lib
    acquire=bind(lib,'jp_character_textures_acquire',I,U);release=bind(lib,'jp_character_textures_release',I,U);get=bind(lib,'jp_character_texture_definition',I,U,P)
    reset=bind(lib,'jp_queue_reset_frame',I);reserve=bind(lib,'jp_queue_reserve',I,U,U);ctx=bind(lib,'jp_queue_set_context',I,U,P);pin=bind(lib,'jp_frames_prepare_texture',I,U)
    begin=bind(lib,'njGetkmPointer',None);finish=bind(lib,'njSetkmPointer',None);start=bind(lib,'njStartVertexTexOpaque',P);end=bind(lib,'njEndVertex',None,P)
    submit=bind(lib,'jp_frames_submit',I,P);retire=bind(lib,'jp_frame_release',I,Q);bound=bind(gl,'jp_gpu_draw_bound_frame',I,P,Q,P,P,S,P,P)
    check(bind(lib,'jp_queue_initialize',I,S)(8<<20)==0 and bind(lib,'jp_frames_initialize',I,U,U,S)(8,128,64<<20)==0 and t.init(1024,4096,128<<20)==0 and video.init(64,64,0xff123456,1)==0,'actual native queue frame texture video providers')
    check(assets.init(str((a.inputs/'assets').resolve()),C.byref(Limits(4<<20,8<<20,32<<20,512)))==0 and bind(lib,'jp_character_textures_initialize',I)()==0,'real local PRS asset and original character owners')
    decode=bind(lib,'prskit_decompress',I,P,S,P,S,P,P);defs=[]
    for i in range(17):desc=Definition();check(get(i,C.byref(desc))==0,'actual original character list descriptor');defs.append(desc)
    source=(a.inputs/'original_texture.c').read_text();import re
    def original_images(desc):
        filename=re.search(r'\{"([A-Z0-9_]+)", &'+re.escape(desc.name.decode())+r'\}',source)[1]+'.PRS';path=a.inputs/'assets'/filename
        check(sha(path)==iv['PRS_files'][filename]['sha256'],'unmodified original retail PRS '+filename);raw=path.read_bytes();n=S();at=S()
        check(decode(raw,len(raw),None,0,C.byref(n),C.byref(at))==0 and n.value<=8<<20,'bounded original PRS oracle source');buffer=C.create_string_buffer(n.value)
        check(decode(raw,len(raw),buffer,len(buffer),C.byref(n),C.byref(at))==0,'actual original PRS oracle decode');flags,rows,items=index_archive(buffer.raw[:n.value]);return filename,rows,items
    def packet(record,vertices):
        tsp=0x20800000|(record.tspparambuffer&~0x6f00)|0x400
        check(reserve(0,len(vertices)+1)==0 and ctx(0,(U*4)(0x8000000a,0xe0800000,tsp,record.texparambuffer))==0 and pin(record.texparambuffer&0x1fffff)==0,'actual loaded character binding retained with explicit point filtering and neutral mip bias')
        begin();pointer=start();assert pointer;words=[]
        for i,(x,y,u,v) in enumerate(vertices):words += [0xf0000000 if i==len(vertices)-1 else 0xe0000000,fw(x),fw(y),fw(1),fw(u),fw(v),0xffffffff,0]
        data=struct.pack('<'+'I'*len(words),*words);C.memmove(pointer,data,len(data));end(pointer+len(data));finish()
    config=Config.make();sonic_grid=None
    for character in range(8):
        check(acquire(character)==0,'actual original character texture acquisition '+str(character))
        # The original SDK reuses an already listed globalIndex before parsing
        # a later payload. E102HIT intentionally overlaps earlier E102 effects.
        # Derive first-loaded source identity from raw files in original call
        # and chunk order, independently of the actual native record/surface.
        decoded_files={};effective={}
        for desc in (x for x in defs if x.character==character):
            filename,rows,items=original_images(desc);decoded_files[desc.name]=(filename,rows,items)
            for sequence,item in enumerate(items):
                row=next(row for row in rows if row['number']==sequence)
                effective.setdefault(row['gid'],(filename,sequence,item))
        for desc in (x for x in defs if x.character==character):
            filename,rows,items=decoded_files[desc.name];listing=List.from_address(desc.list);result=hashlib.sha256();oracle_hash=hashlib.sha256()
            for index in range(listing.nbTexture):
                pointer=t.lookup(C.byref(listing),index);check(bool(pointer),'real bound character texture field');record=Record.from_address(pointer);entry=rows[index];first_file,first_index,image=effective[entry['gid']]
                if first_file!=filename or first_index!=entry['number']:aliases.append(dict(character=character,file=filename,entry=index,globalIndex=entry['gid'],first_loaded_file=first_file,first_loaded_chunk=first_index))
                check(record.globalIndex==entry['gid'] and record.texinfo.texsurface.nWidth==image['w'] and record.texinfo.texsurface.nHeight==image['h'],('original PVM identity and dimensions in actual record',filename,index,entry,record.globalIndex,(record.texinfo.texsurface.nWidth,record.texinfo.texsurface.nHeight),(image['w'],image['h'])))
                check(reset()==0,'fresh real packet frame');packet(record,[(-64,-64,-1,-1),(192,-64,3,-1),(-64,192,-1,3)])
                config=Config.make(image['w'],image['h']);frame=Q();check(submit(C.byref(frame))==0,'real native retained frame submission');output=C.create_string_buffer(image['w']*image['h']*4);report=Report();error=Failure()
                rc=bound(gpu.handle,frame.value,C.byref(config),output,len(output),C.byref(report),C.byref(error));expected=oracle(image)
                (d/'last_texture.json').write_text(json.dumps(dict(file=filename,index=index,kind=image['kind'],width=image['w'],height=image['h'],GPU_result=rc,failure_code=error.code,hresult=error.hresult),indent=2)+'\n')
                check(rc==0 and output.raw==expected,('complete real character GPU pixels match independent original-PVM oracle',filename,index,image['kind'],rc,error.code))
                result.update(output.raw);oracle_hash.update(expected);images+=1;pixels+=image['w']*image['h'];check(retire(frame.value)==0,'actual captured texture frame retired')
            reports.append(dict(file=filename,list=desc.name.decode(),textures=listing.nbTexture,GPU_RGBA_sha256=result.hexdigest(),independent_RGBA_sha256=oracle_hash.hexdigest()))
            if desc.name==b'sonic_texlist':
                width,height=512,256;check(reset()==0 and video.init(width,height,0xff123456,1)==0,'real Sonic texture-grid frame with matching video bounds')
                for index in range(listing.nbTexture):
                    record=Record.from_address(t.lookup(C.byref(listing),index));x=(index%8)*64;y=(index//8)*64;packet(record,[(x,y,0,0),(x+64,y,1,0),(x,y+64,0,1),(x+64,y+64,1,1)])
                frame=Q();check(submit(C.byref(frame))==0,'actual Sonic texture-grid retained frame');window=P();err=Failure();cfg=Config.make(width,height);cfg.logical_width=width;cfg.logical_height=height
                check(bind(gl,'jp_window_open',I,P,U,U,I,P,P)(gpu.handle,width,height,1,C.byref(window),C.byref(err))==0,'actual Win32 texture-grid test window')
                rep=Report();check(bind(gl,'jp_window_render_frame',I,P,Q,P,P,P)(window,frame.value,C.byref(cfg),C.byref(rep),C.byref(err))==0,'actual native texture-grid window rendered')
                buffer=C.create_string_buffer(width*height*4);check(bind(gl,'jp_window_readback',I,P,P,S,P)(window,buffer,len(buffer),C.byref(err))==0,'actual native window backbuffer readback')
                check(len(set(zip(buffer.raw[0::4],buffer.raw[1::4],buffer.raw[2::4])))>16 and bind(gl,'jp_window_present',I,P,U,P)(window,1,C.byref(err))==0,'nonblank actual native texture-grid presented after pre-swap readback')
                path=d/'sonic_texture_loader_window.png';png(path,buffer.raw,width,height);shots.append(dict(path=str(path),sha256=sha(path),width=width,height=height,actual_window_backbuffer=True,diagnostic_texture_grid_not_gameplay=True))
                bind(gl,'jp_window_close',None,P)(window);check(retire(frame.value)==0 and video.init(64,64,0xff123456,1)==0,'texture-grid frame retired and next test bounds restored')
        check(release(character)==0 and assets.evict()==0 and t.live()==0,'actual per-character bindings and all borrowed asset residencies retired')
    check(images==418 and pixels>0 and bind(lib,'jp_character_textures_shutdown',I)()==0 and assets.shutdown()==0,'all original entries rendered and real character/asset owners closed')
    check(reset()==0 and bind(lib,'jp_frames_shutdown',I)()==0 and pvm.shutdown()==0,'actual frame and memory providers closed');video.shutdown();t.shutdown();bind(lib,'jp_queue_shutdown',None)();gpu.close(gpu.handle)
    faulthandler.disable();fault.close();out=dict(status='all_418_real_character_texture_bindings_render_to_independent_PVM_pixels_and_actual_diagnostic_window',checks=checks,actual_GPU_images=images,pixels_compared=pixels,actual_Window_Presents=1,screenshots=shots,files=reports,
        build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),raw_PVM_oracle_handles_twiddled_mips_and_VQ=True,original_first_loaded_global_index_aliases=aliases,alpha_ignored_and_largest_level_only=True,
        full_3D_character_animation_or_gameplay_verified=False,retail_derived_captures_private=True,playable=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],checks,'checks;',images,'images;',pixels,'pixels',flush=True)
if __name__=='__main__':main()
