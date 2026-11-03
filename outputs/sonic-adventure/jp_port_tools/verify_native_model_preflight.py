"""Check bounded native model descriptions and real owned queue reservations.
Capacity probes simulate a conservative writer envelope; they do not execute
the complete renderer or qualify image output.
"""
import argparse,ctypes as C,hashlib,json,random
from pathlib import Path
from native_texture_test_support import Runtime,Info,Name,List,field
U=C.c_uint32;F=C.c_float;P=C.c_void_p;S=C.c_size_t
class Vec(C.Structure):_fields_=[(n,F) for n in ['x','y','z']]
class Material(C.Structure):_fields_=[('diffuse',U),('specular',U),('exponent',F),('texture',U),('flags',U)]
class Mesh(C.Structure):_fields_=[('kind',C.c_uint16),('count',C.c_uint16),('indices',C.POINTER(C.c_int16)),('attrs',P),('normals',P),('colors',P),('uvs',P)]
class Model(C.Structure):_fields_=[('points',C.POINTER(Vec)),('normals',C.POINTER(Vec)),('point_count',C.c_int32),('meshes',C.POINTER(Mesh)),('materials',C.POINTER(Material)),('mesh_count',C.c_uint16),('material_count',C.c_uint16),('center',Vec),('radius',F)]
class Span(C.Structure):_fields_=[('indices',S),('uvs',S)]
class Bound(C.Structure):_fields_=[(n,U) for n in ['opaque','translucent','vertices','meshes']]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build','report']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();root=a.workspace.resolve();build=json.loads((a.build/'build_verification.json').read_text())
    assert build['status']=='native_model_preflight_linked_behavior_unverified'
    for n,h in build['authored_source_sha256'].items():assert sha(root/n)==h
    dll=a.build/'jp_model_preflight.dll';assert sha(dll)==build['dll_sha256'];t=Runtime(dll);lib=t.dll
    def bind(n,result,*args):
        f=getattr(lib,n);f.restype=result;f.argtypes=list(args);return f
    init=bind('jp_render_state_initialize',C.c_int,U,S)
    qinit=bind('jp_queue_initialize',C.c_int,S);qreset=bind('jp_queue_reset_frame',C.c_int)
    qdata=bind('jp_queue_data',P,U);qused=bind('jp_queue_committed_bytes',S,U)
    qstatus=bind('jp_queue_status',C.c_int);qclear=bind('jp_queue_clear_error',None)
    qget=bind('njGetkmPointer',None);qput=bind('njSetkmPointer',None)
    begin=[bind(n,P) for n in ['njStartVertexNonTexOpaque','njStartVertexNonTexTrans']]
    end=bind('njEndVertex',None,P)
    guard=bind('jp_render_state_check_guards',C.c_int)
    control=bind('njControl3D',None,U);attributes=bind('njSetConstantAttr',None,U,U)
    measure=bind('jp_model_preflight',C.c_int,C.POINTER(Model),S,S,S,S,C.POINTER(Span),C.POINTER(Bound))
    reserve=bind('jp_model_reserve',C.c_int,C.POINTER(Bound))
    checks=0;rng=random.Random(0x424f554e);cases=0
    def check(v):
        nonlocal checks
        checks+=1;assert v,f'preflight check {checks}'
    check(init(64,1<<20)==0);check(t.init(4,8,1<<20)==0);check(qinit(16<<20)==0)
    info=Info();info.texsurface.nWidth=info.texsurface.nHeight=8;info.texsurface.TextureSize=128
    info.texsurface.BitDepth=1;info.texsurface.PixelFormat=0x08000000;info.texsurface.fSurfaceFlags=4
    pixels=C.create_string_buffer(bytes(range(128)));check(t.copy(C.byref(info),pixels,128)==0)
    names=(Name*1)();t.setname(C.byref(names[0]),None,1000,0)
    t.entry(field(names[0]),C.byref(info),1000,2,0xffffffff)
    address=t.resolve(names[0].texaddr);t.param(address)
    listing=List(names,1);check(t.select(C.byref(listing))==1)
    points=(Vec*64)(*(Vec(rng.random(),rng.random(),2) for _ in range(64)))
    normals=(Vec*64)(*(Vec(0,0,1) for _ in range(64)))
    for iteration in range(128):
        check(qreset()==0);count=1+iteration%4
        meshes=(Mesh*count)();materials=(Material*count)();spans=(Span*count)();keep=[]
        lower=[0,0]
        control(4 if iteration%3==0 else 0);forced_alpha=0x100000 if iteration%6==0 else 0
        attributes(0xffffffff,forced_alpha)
        for i in range(count):
            kind=(iteration+i)%4;polygons=1+rng.randrange(7);indices=[];uvs=0;lower_mesh=1
            for j in range(polygons):
                n=(3 if kind==0 else 4) if kind<2 else 3+rng.randrange(8)
                if kind>=2:indices.append(n|(0x8000 if j%2 else 0))
                indices.extend(rng.randrange(64) for _ in range(n));uvs+=n
                # A clipped triangle can close two vertices and output four;
                # allow a final endpoint after each strip.
                lower_mesh+=6*(n-2)+1
            packed=(C.c_int16*len(indices))(*indices);tex=(U*uvs)();keep.extend([packed,tex])
            own=(0x200000 if iteration%2 else 0)|(0x100000 if i%2 else 0)
            meshes[i]=Mesh((kind<<14)|i,polygons,packed,None,None,None,C.cast(tex,P))
            materials[i]=Material(0xffffffff,0,0,0,own);spans[i]=Span(len(indices),uvs)
            drawn=own|forced_alpha if iteration%3==0 else own
            lower[bool(drawn&0x100000)]+=lower_mesh
        model=Model(points,normals,64,meshes,materials,count,count,Vec(0,0,2),2)
        result=Bound();check(measure(C.byref(model),64,64,count,count,spans,C.byref(result))==0)
        check(result.vertices==64 and result.meshes==count)
        check(result.opaque>=lower[0] and result.translucent>=lower[1])
        check(reserve(C.byref(result))==0);qget()
        for lane in range(2):
            if lower[lane]:
                vertex=begin[lane]();check(vertex is not None)
                # Header is already written; emulate the maximal writer envelope
                # in actual owned storage, then commit and check native guards.
                count_bytes=(lower[lane]-1)*32
                if count_bytes:C.memset(vertex,0x5c,count_bytes)
                end(vertex+count_bytes);check(qstatus()==0)
                check(qused(lane)==lower[lane]*32)
        qput();check(qstatus()==0 and guard()==0);cases+=1
    check(qreset()==0);control(0)
    index=(C.c_int16*3)(0,1,2);mesh=(Mesh*1)(Mesh(0,1,index,None,None,None,None))
    material=(Material*1)(Material(0xffffffff,0,0,0,0));spans=(Span*1)(Span(3,0))
    model=Model(points,normals,3,mesh,material,1,1,Vec(0,0,2),2)
    def reject(change,restore,code=1):
        change();out=Bound(0xcccccccc,0xcccccccc,0xcccccccc,0xcccccccc)
        before=bytes(out);check(measure(C.byref(model),64,64,1,1,spans,C.byref(out))==code)
        check(bytes(out)==before);check(qused(0)==qused(1)==0);restore();t.clear()
    reject(lambda:setattr(model,'point_count',65),lambda:setattr(model,'point_count',3))
    reject(lambda:setattr(model,'point_count',0),lambda:setattr(model,'point_count',3))
    reject(lambda:setattr(mesh[0],'kind',1),lambda:setattr(mesh[0],'kind',0))
    reject(lambda:setattr(spans[0],'indices',2),lambda:setattr(spans[0],'indices',3))
    reject(lambda:index.__setitem__(0,-1),lambda:index.__setitem__(0,0))
    reject(lambda:setattr(points[0],'x',float('nan')),lambda:setattr(points[0],'x',0))
    reject(lambda:setattr(material[0],'flags',0x200000),lambda:setattr(material[0],'flags',0))
    material[0].flags=0x200000;uv=(U*3)();mesh[0].uvs=C.cast(uv,P);spans[0].uvs=3
    old=names[0].texaddr
    reject(lambda:setattr(names[0],'texaddr',0),lambda:setattr(names[0],'texaddr',old),4)
    result=Bound();check(measure(C.byref(model),64,64,1,1,spans,C.byref(result))==0)
    check(qinit(64)==0);check(reserve(C.byref(result))==2);qclear()
    check(qused(0)==qused(1)==0)
    t.shutdown();lib.jp_render_state_shutdown();lib.jp_queue_shutdown()
    report=dict(status='native_model_preflight_and_owned_queue_capacity_verified',checks=checks,valid_models=cases,
        mesh_types_tested=['triangle','quad','polygon','strip'],
        actual_complete_renderer_executed=False,capacity_probe_is_conservative_simulated_writer=True,
        caller_must_supply_truthful_accessible_array_lengths=True,
        pending_frame_resource_retention=True,dll_sha256=sha(dll),
        build_report_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(Path(__file__)),
        test_support_sha256=sha(Path(__file__).with_name('native_texture_test_support.py')),
        gameplay_verified=False,overall_completion_percent=10,goal_paused=False)
    a.report.write_text(json.dumps(report,indent=2)+'\n')
    print(report['status'],checks,'checks;',cases,'valid models')
if __name__=='__main__':main()
