"""Genuine model-tree draw, real disc palette/textures, retained frame and GPU."""
import argparse,ctypes as C,faulthandler,hashlib,json,math,os,struct,subprocess,sys,types
from pathlib import Path
from native_packet_test_support import bind,Decoder,Plan,Failure as PacketFailure
from native_gpu_test_support import Config,Report,Failure
from native_gpu_jobs_test_support import Jobs
from native_texture_test_support import Runtime,List,Record
from native_video_test_support import Video
from native_asset_test_support import Limits
from verify_native_motion_store import View
from verify_native_motion_data import Entry
from verify_native_model_preflight import Model,Span,Bound
from verify_native_read_queue import Disc,Info as QueueInfo
from capture_native_boot_progress import png
U,I,Q,P,S,F=C.c_uint32,C.c_int32,C.c_uint64,C.c_void_p,C.c_size_t,C.c_float
class Node(C.Structure):_fields_=[('flags',U),('model',P),('pos',F*3),('ang',I*3),('scale',F*3),('child',P),('sibling',P)]
class Override(C.Structure):_fields_=[('object',P),('draw',P)]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def exercise(c,inputs,track):
    d,b,f,system,ex,game,queue=(c[n] for n in ('d','b','f','system','ex','game','queue'));check,snapshot,shutdown,clean=(c[n] for n in ('check','snapshot','shutdown','clean'));r=c['r'];motions=C.CDLL(b['motion_DLL']);gl=C.CDLL(b['GPU_DLL']);t=Runtime(Path(b['frames_DLL']));video=Video(f)
    check(C.sizeof(Node)==72 and C.sizeof(Model)==64,'actual full-width original model/object ABIs')
    check(bind(system,'jp_system_start_motions',I,S,U)(8<<20,16)==0 and bind(system,'jp_system_start_assets',I,C.c_wchar_p,P)(str(Path(inputs)/'assets'),C.byref(Limits(4<<20,8<<20,32<<20,512)))==0 and bind(system,'jp_system_start_character_textures',I)()==0,'same complete original parent starts actual Motion/Assets/Character owners')
    check(bind(f,'jp_character_switch',I,I)(0)==0 and bind(f,'jp_character_textures_acquire',I,U)(0)==0,'actual original set_character selects Sonic and caller retains an additional real texture consumer')
    view=View();check(bind(motions,'jp_motions_acquire',I,U,C.c_char_p,P)(7,b'sonic_object',C.byref(view))==0 and view.data>0xffffffff,'actual canonical original Sonic object model view retained')
    entries=[];get=bind(motions,'jp_motion_models_entry',I,U,P)
    for index in range(bind(motions,'jp_motion_models_count',U)()):
        e=Entry();assert get(index,C.byref(e))==0;entries.append(e)
    def extent(pointer,element):
        choices=[e for e in entries if e.data<=pointer<e.data+e.host_bytes and (pointer-e.data)%element==0 and (e.host_element==element or (e.kind==0 and e.host_element==1))]
        assert len(choices)==1,(hex(pointer),element,[(e.name,e.host_element) for e in choices]);return (choices[0].data+choices[0].host_bytes-pointer)//element
    def data_hash():
        out=hashlib.sha256()
        for e in entries:out.update(C.string_at(e.data,e.host_bytes))
        return out.hexdigest()
    original_model_hash=data_hash();nodes=[];models=[];seen=set();max_depth=0
    def visit(at,depth=0):
        nonlocal max_depth
        check(at not in seen and extent(at,72)>=1,'bounded distinct original model-tree node');seen.add(at);n=Node.from_address(at);nodes.append((at,n));max_depth=max(max_depth,depth)
        check(all(math.isfinite(v) for v in list(n.pos)+list(n.scale)),'finite real original node transforms')
        if n.model:
            check(extent(n.model,64)>=1,'genuine bounded original model descriptor');model=Model.from_address(n.model);spans=(Span*model.mesh_count)()
            for i,mesh in enumerate(model.meshes[:model.mesh_count]):spans[i]=Span(extent(C.cast(mesh.indices,P).value,2) if mesh.indices else 0,extent(mesh.uvs,4) if mesh.uvs else 0)
            models.append((n.model,model,spans))
        if n.child:visit(n.child,depth+1)
        if n.sibling:visit(n.sibling,depth)
    visit(view.data);check(len(nodes)==73 and len(models)==28,'complete original Sonic hierarchy contains73 nodes and28 genuine basic models')
    # Load a real palette using the same actual original read queue/GDFS owner.
    disc=Disc(track);files=disc.files();filename='PL_10B.BIN';fad,count,isdir=files[filename];assert not isdir;rounded=(count+2047)&~2047;expected=disc.sectors(fad,rounded//2048);disc.f.close()
    storage=C.create_string_buffer(rounded+96);destination=(C.addressof(storage)+63)&~31;C.memset(storage,0xa5,len(storage));qbefore=snapshot(queue,'jp_read_queue_snapshot',QueueInfo).completed
    check(bind(queue,'jp_read_queue_add',I,C.c_char_p,P,S)(filename.encode(),destination,rounded)==0,'actual original loader queues supplied JP stage palette into aligned bounded buffer')
    for _ in range(100000):
        check(bind(queue,'jp_read_queue_serve',I)()==0,'actual queued palette read advances')
        if not snapshot(queue,'jp_read_queue_snapshot',QueueInfo).queued:break
    else:raise AssertionError('actual palette read did not finish')
    check(C.string_at(destination,rounded)==expected and snapshot(queue,'jp_read_queue_snapshot',QueueInfo).completed==qbefore+1,'complete real palette-sector bytes match independent supplied-disc oracle')
    palette=bind(f,'jp_model_probe_palettes',P)();check(0<count<=rounded<=24576 and palette>0xffffffff,'same actual renderer palette/unlit owner has room for exact retail bytes');C.memmove(palette,destination,rounded)
    check(C.string_at(palette,rounded)==expected,'actual renderer uses unmodified retail palette data')
    listing=P();check(bind(f,'jp_character_textures_list',I,U,U,P)(0,0,C.byref(listing))==0,'genuine original Sonic texture list available');listing=C.cast(listing,C.POINTER(List));check(t.select(listing)==1,'genuine original Sonic texture list selected by actual SDK')
    check(bind(f,'jp_queue_reset_frame',I)()==0 and bind(f,'jp_render_state_capacity',U)()>=205,'real empty recording and adequate genuine renderer scratch capacity')
    check(bind(f,'jp_render_state_configure',I,U,U,F,F,F,F,F)(512,512,512,1,10000,1,1)==0 and video.init(512,512,0xff183048,1)==0,'actual native square diagnostic camera and video bounds')
    C.c_float.in_dll(f,'port_draw_aspect').value=1;C.c_float.in_dll(f,'port_view_aspect').value=1
    bind(f,'draw_model_init',None)();bind(f,'draw_set_palette',None,I)(2);bind(f,'draw_set_light',None,F,F,F)(0.866025,-0.6,0)
    bound=Bound();preflight=bind(f,'jp_model_preflight',I,P,S,S,S,S,P,P)
    for address,model,spans in models:
        item=Bound();check(preflight(address,extent(C.cast(model.points,P).value,12),extent(C.cast(model.normals,P).value,12),extent(C.cast(model.meshes,P).value,48),extent(C.cast(model.materials,P).value,20),spans,C.byref(item))==0,'whole genuine model validates against exact compiled-data array extents')
        bound.opaque+=item.opaque;bound.translucent+=item.translucent;bound.vertices=max(bound.vertices,item.vertices);bound.meshes+=item.meshes
    check(bind(f,'jp_model_reserve',I,P)(C.byref(bound))==0,'complete tree worst-case packet envelope reserved before unchecked original draw')
    original_list=listing.contents;words=(U*original_list.nbTexture)()
    for i in range(original_list.nbTexture):record=Record.from_address(t.lookup(listing,i));words[i]=record.texparambuffer&0x1fffff
    check(bind(f,'jp_frames_prepare_textures',I,P,U)(words,len(words))==0,'actual frame retains every genuine Sonic texture surface before original tree draw')
    # Measure rest-pose bounds through the same qualified Ninja transforms. This
    # chooses a diagnostic camera; it is not an independent transform oracle.
    setmatrix=bind(f,'njSetMatrix',None,P,P);getmatrix=bind(f,'njGetMatrix',None,P);translate=bind(f,'njTranslateV',None,P,P);scale=bind(f,'njScaleV',None,P,P);rotate=[bind(f,'njRotate'+axis,None,P,I) for axis in ('X','Y','Z')]
    identity=(F*16)(*[float(i%5==0) for i in range(16)]);world=[]
    def measure(at,parent):
        n=Node.from_address(at);setmatrix(None,parent);translate(None,C.byref(n.pos));rotate[2](None,n.ang[2]);rotate[0](None,n.ang[0]);rotate[1](None,n.ang[1]);scale(None,C.byref(n.scale));matrix=(F*16)();getmatrix(matrix)
        if n.model:
            model=Model.from_address(n.model)
            for v in model.points[:model.point_count]:world.append(tuple(v.x*matrix[j]+v.y*matrix[4+j]+v.z*matrix[8+j]+matrix[12+j] for j in range(3)))
        if n.child:measure(n.child,matrix)
        if n.sibling:measure(n.sibling,parent)
    measure(view.data,identity);angle=-0x3000;sn=math.sin(angle*2*math.pi/65536);cs=math.cos(angle*2*math.pi/65536);rotated=[(x*cs+z*sn,y,-x*sn+z*cs) for x,y,z in world]
    lo=[min(v[i] for v in rotated) for i in range(3)];hi=[max(v[i] for v in rotated) for i in range(3)];cx=(lo[0]+hi[0])/2;cy=(lo[1]+hi[1])/2;distance=-lo[2]+max(hi[0]-lo[0],hi[1]-lo[1])*1.8
    camera=(F*16)(1,0,0,0,0,-1,0,0,0,0,1,0,-cx,cy,distance,1);setmatrix(None,camera);rotate[1](None,angle)
    check(C.c_int32.in_dll(f,'_max_stack_size_').value-C.c_int32.in_dll(f,'_stack_size_').value>=2*max_depth+2,'actual Ninja saved-stack capacity covers complete original recursion')
    # FRAME_DRAWS must be true on the real logical clock, not a forced counter.
    for _ in range(4):
        if C.c_int32.in_dll(f,'frame_step').value==0:break
        bind(c['present'],'frame_wait',None)()
    check(C.c_int32.in_dll(f,'frame_step').value==0,'actual game clock reaches original model-drawing phase')
    overrides=(Override*1)();(d/'before_draw.json').write_text(json.dumps(dict(nodes=len(nodes),models=len(models),bounds=[lo,hi],camera=list(camera),queue_bound=[bound.opaque,bound.translucent],max_depth=max_depth),indent=2)+'\n')
    before_matrix=(F*16)();getmatrix(before_matrix);depth=C.c_int32.in_dll(f,'_stack_size_').value
    bind(f,'draw_object',None,P,P)(view.data,overrides)
    check(bind(f,'jp_render_state_check_guards',I)()==0 and bind(f,'jp_queue_status',I)()==0 and t.status()==0,'actual original complete tree draw preserves scratch guards and queue/texture state')
    after_matrix=(F*16)();getmatrix(after_matrix);check(bytes(after_matrix)==bytes(before_matrix) and C.c_int32.in_dll(f,'_stack_size_').value==depth,'original tree restores complete current matrix and stack depth')
    check(data_hash()==original_model_hash,'all2793 canonical model records remain byte-for-byte intact')
    frame=Q();check(bind(f,'jp_frames_submit',I,P)(C.byref(frame))==0,'actual original model packets captured in immutable retained frame');decoder=Decoder(b['GPU_DLL']);plan=P();failure=PacketFailure();check(decoder.frame(frame.value,64<<20,C.byref(plan),C.byref(failure))==0,'actual complete tree packet streams decode')
    decoded=C.cast(plan,C.POINTER(Plan)).contents;counts=dict(batches=decoded.batch_count,vertices=decoded.vertex_count,triangles=decoded.triangle_count);(d/'packets.json').write_text(json.dumps(counts,indent=2)+'\n');check(decoded.triangle_count>100 and decoded.vertex_count>100,'genuine original Sonic tree emitted substantial real3D geometry');decoder.destroy(plan)
    device=C.CDLL(b['device_DLL']);handle=bind(device,'jp_device_gpu',P)();check(bool(handle),'borrow actual same-System GPU handle without replacing its owner');gpu=types.SimpleNamespace(lib=gl,handle=P(handle));jobs=Jobs(gpu);cfg=Config.make(512,512);cfg.logical_width=cfg.logical_height=512;window=P();error=Failure()
    check(bind(gl,'jp_window_open',I,P,U,U,I,P,P)(handle,512,512,1,C.byref(window),C.byref(error))==0,'actual real Win32/DXGI Sonic test window')
    job=Q();report=Report();check(jobs.submit(handle,frame.value,C.byref(cfg),C.byref(job),C.byref(report),C.byref(error))==0,'actual D3D job consumes genuine captured character geometry and textures')
    check(bind(f,'jp_character_switch',I,I)(5)==5 and shutdown(5000)==11 and clean()==6,'real model/texture consumers protect character and complete parent while GPU result remains owned')
    timing,pending=jobs.wait(job.value);check(jobs.copy(window,job.value,C.byref(error))==0,'actual completed GPU job copied into Win32 backbuffer');output=C.create_string_buffer(512*512*4)
    check(bind(gl,'jp_window_readback',I,P,P,S,P)(window,output,len(output),C.byref(error))==0,'actual pre-Present Sonic window pixels read back');path=d/'sonic_original_model_tree.png';png(path,output.raw,512,512)
    rgba=output.raw;colors=set(zip(rgba[0::4],rgba[1::4],rgba[2::4]));bg=(0x18,0x30,0x48);foreground=sum(tuple(rgba[i:i+3])!=bg for i in range(0,len(rgba),4))
    (d/'image_observation.json').write_text(json.dumps(dict(colors=len(colors),foreground=foreground,report={n:getattr(report,n) for n,_ in Report._fields_},failure=error.code),indent=2)+'\n')
    check(len(colors)>32 and foreground>1000,'actual captured Sonic rendering contains varied visible model/texture pixels');check(bind(gl,'jp_window_present',I,P,U,P)(window,1,C.byref(error))==0,'actual original Sonic model test image presented')
    check(jobs.release(handle,job.value,C.byref(error))==0,'actual textured GPU result retired while same captured frame and consumers remain owned')
    off=Config.make(512,512,textures=0);off.logical_width=off.logical_height=512;off_job=Q();off_report=Report();check(jobs.submit(handle,frame.value,C.byref(off),C.byref(off_job),C.byref(off_report),C.byref(error))==0,'same genuine captured model geometry submitted with texture sampling disabled')
    jobs.wait(off_job.value);check(off_report.textures_enabled==off_report.shader_texture_bindings==off_report.shader_sampler_bindings==0 and off_report.texture_uploads==0,'actual texture-Off backend binds no texture or sampler and uploads no texture payload')
    check(jobs.copy(window,off_job.value,C.byref(error))==0 and bind(gl,'jp_window_readback',I,P,P,S,P)(window,output,len(output),C.byref(error))==0,'actual vertex/material-colour model read back before Present');off_rgba=output.raw;off_path=d/'sonic_original_model_tree_textures_off.png';png(off_path,off_rgba,512,512)
    check(off_rgba!=rgba and sum(tuple(off_rgba[i:i+3])!=bg for i in range(0,len(off_rgba),4))>1000 and bind(gl,'jp_window_present',I,P,U,P)(window,1,C.byref(error))==0,'texture-Off retains visible genuine mesh with vertex/material colours and actually Presents')
    check(jobs.release(handle,off_job.value,C.byref(error))==0 and bind(f,'jp_frame_release',I,Q)(frame.value)==0,'caller retires both GPU results and retained frame before releasing game-data consumers');bind(gl,'jp_window_close',None,P)(window)
    check(bind(f,'jp_character_textures_release',I,U)(0)==0 and bind(motions,'jp_motions_release',I,Q)(view.token)==0 and bind(f,'jp_character_switch_retire',I)()==0,'actual texture/model/game references retire after captured draw completes')
    check(bind(f,'jp_queue_reset_frame',I)()==0 and shutdown(5000)==0,'complete original initializer and actual child graph close after render-consumer retirement')
    check(bind(f,'jp_lcd_shutdown',I)()==0 and bind(f,'jp_vmu_store_shutdown',I)()==0 and bind(f,'jp_frames_shutdown',I)()==0,'caller closes genuine borrowed media/frame owners');bind(f,'jp_render_state_shutdown',None)();bind(f,'jp_textures_shutdown',None)();bind(f,'jp_queue_shutdown',None)()
    faulthandler.disable();c['fault'].close();out=dict(status='genuine_original_Sonic_model_tree_real_JP_textures_palette_full_parent_retained_GPU_frame_and_actual_Window_verified',checks=len(c['labels']),assertions=c['labels'],observations=c['obs'],original_nodes=len(nodes),original_models=len(models),packet_counts=counts,retail_palette=dict(name=filename,FAD=fad,bytes=count,sector_bytes=rounded,sha256=hashlib.sha256(expected).hexdigest()),screenshots=[dict(path=str(shot),sha256=sha(shot),width=512,height=512,actual_window_backbuffer=True,genuine_original_character_model_not_gameplay=True,mode=mode) for shot,mode in [(path,'original_textures'),(off_path,'textures_off_vertex_material_colours')]],foreground_pixels=foreground,actual_window_presents=2,texture_Off_backend_bindings_and_uploads_zero=True,manual_model_texture_consumer_retirement=True,automatic_task_render_view_acquisition=False,animation_main_loop_or_gameplay_verified=False,build_verification_sha256=sha(c['a'].build/'build_verification.json'),verifier_sha256=sha(__file__),playable=False,overall_completion_percent=10)
    (d/'result.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],len(c['labels']),'checks',counts,flush=True)
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','inputs','track','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve();assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir();b=read(a.build/'build_verification.json')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h
    original=p/'jp_port_tools/verify_native_game_init.py';raw=original.read_bytes();marker=b"        if a.case=='normal':\n";assert raw.count(marker)==1;prefix=raw.split(marker)[0]
    for old,new in [(b"('jp_textures_initialize',(32,1024,32<<20))",b"('jp_textures_initialize',(1024,4096,128<<20))"),(b"('jp_render_state_initialize',(128,1<<20))",b"('jp_render_state_initialize',(2048,16<<20))")]:assert prefix.count(old)==1;prefix=prefix.replace(old,new)
    caller=d/'private_actual_parent_caller.py';caller.write_bytes(prefix+('        from verify_native_character_tree import exercise\n        exercise(locals(), '+repr(str(a.inputs.resolve()))+', '+repr(str(a.track.resolve()))+")\nif __name__=='__main__':main()\n").encode())
    cmd=[sys.executable,'-B',str(caller),'--workspace',str(r),'--build',str(a.build.resolve()),'--track',str(a.track.resolve()),'--output',str(d/'run'),'--case','normal'];run=subprocess.run(cmd,capture_output=True,timeout=240,env=dict(os.environ,PYTHONPATH=str(p/'jp_port_tools')),creationflags=subprocess.CREATE_NO_WINDOW);(d/'stdout.txt').write_bytes(run.stdout);(d/'stderr.txt').write_bytes(run.stderr)
    assert not run.returncode,(run.returncode,run.stderr.decode(errors='replace')[-7000:]);out=read(d/'run/result.json');out.update(original_parent_verifier_sha256=sha(original),derived_caller_sha256=sha(caller));(d/'verification.json').write_text(json.dumps(out,indent=2)+'\n');print(out['status'],out['checks'],'checks',flush=True)
if __name__=='__main__':main()
