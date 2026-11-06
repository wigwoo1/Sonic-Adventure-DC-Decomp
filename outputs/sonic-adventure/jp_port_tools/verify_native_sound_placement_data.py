"""Compare recovered marker values and composite rotations with JP retail bytes.

No retail bytes are written to the public report. Rotation scope is sampled
finite matrices in both SH-4 register banks, implicit and explicit destinations.
"""
import argparse,ctypes as C,hashlib,json,random,re,struct,sys
from pathlib import Path
from verify_native_model_preflight import Vec,Model
from verify_native_audio import bind
from model_reference import make_reference
from camera_reference_numeric import service_for
I,U,P,F=C.c_int32,C.c_uint32,C.c_void_p,C.c_float
class Object(C.Structure):
    _fields_=[('flags',U),('model',C.POINTER(Model)),('pos',Vec),('ang',I*3),('scl',Vec),('child',P),('sibling',P)]
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','build','binary','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=json.loads((a.build/'build_verification.json').read_bytes());p=r/'outputs/sonic-adventure'
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    assert sha(a.binary)=='e038db9029bc0ea8ffdb8ec8fc2e0690d1c324f5f7179f3dc58f56574b1aeeaf'
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['placement_original_source_sha256'].items():assert sha(a.upstream/n)==h,n
    for k in ('game_sound_DLL','frames_DLL'):assert sha(b[k])==b[k+'_sha256']
    frame=C.CDLL(b['frames_DLL']);lib=C.CDLL(b['game_sound_DLL']);raw=a.binary.read_bytes();checks=0;sections={}
    def check(ok,label):
        nonlocal checks
        assert ok,label;checks+=1
    def original(at,size):return raw[at-0x8c010000:at-0x8c010000+size]
    obj=C.cast(bind(lib,'peer_sound_marker',P)(),C.POINTER(Object)).contents;m=obj.model.contents
    check(m.point_count==92 and m.mesh_count==2 and m.material_count==1,'real model counts')
    for ptr,at,size in ((m.points,0x8c1ae624,1104),(m.normals,0x8c1aea74,1104),(m.materials,0x8c1ae2e8,20)):
        check(C.string_at(ptr,size)==original(at,size),'all original vector/material bytes')
    for i,at,size in ((0,0x8c1ae2fc,120),(1,0x8c1ae374,640)):
        mesh=m.meshes[i];wire=original(0x8c1ae5f4+i*24,24)
        check(struct.pack('<HH',mesh.kind,mesh.count)==wire[:4],'mesh kind and count')
        check(C.string_at(mesh.indices,size)==original(at,size),'all original indices')
        check(not any((mesh.attrs,mesh.normals,mesh.colors,mesh.uvs)) and wire[8:]==bytes(16),'original absent optional mesh arrays')
    check(bytes(m.center)+struct.pack('<f',m.radius)==original(0x8c1aeec4+24,16),'model bounds')
    check(struct.pack('<I',obj.flags)==original(0x8c1aeeec,4),'object flags')
    check(bytes(obj.pos)+bytes(obj.ang)+bytes(obj.scl)==original(0x8c1aeeec+8,36),'object transforms')
    check(not obj.child and not obj.sibling and original(0x8c1aeeec+44,8)==bytes(8),'leaf object links')
    sections['marker_original_binary_value_checks']=checks
    sys.path.insert(0,str(p));import verify_startup as executor
    from verify_fpu_cleanup_transfer import Fpu
    reference,reference_hashes=make_reference(executor)
    numeric_path=r/'work/sonic-adventure/build/sh4_vector_numeric.dll'
    check(sha(numeric_path)=='2e56428fadb1903c319da319cf6fa3731839eab1b16163e4e820821d6e4c2db2','pinned numerical oracle')
    numeric=bind(C.CDLL(str(numeric_path)),'sa_sh4_vector_numeric',I,U,C.POINTER(Fpu),U);service=service_for(numeric)
    symbols={n:int(v,16) for v,n in re.findall(r'^(0x[0-9a-f]+)\s+(\S+)',(a.upstream/'config/1st_read/symbols.txt').read_text(),re.M)}
    ram=bytearray(16<<20);ram[0x10000:0x10000+len(raw)]=raw;rng=random.Random(0x534e4450)
    stack=C.create_string_buffer(4096);arena=C.create_string_buffer(256);current=bind(frame,'jp_ninja_current_matrix_storage',P)()
    init=bind(frame,'njInitMatrix',None,P,I,I);push=bind(frame,'njPushMatrix',I,P)
    samples=[(0,0,0),(0x4000,0,0),(0,0x4000,0),(0,0,0x4000),(0x1000,0x2000,0x3000),(-1,0x10000,0x7fffffff)]
    samples += [tuple(rng.randrange(-0x80000000,0x80000000) for _ in range(3)) for _ in range(58)]
    calls={};start=checks
    for name in ('njRotateXYZ','njRotateZXY'):
        fn=bind(lib,name,None,P,I,I,I)
        for case,angles in enumerate(samples):
            active=struct.pack('<16f',*[float(i%5==0) if case<6 else rng.uniform(-4,4) for i in range(16)])
            for bank in range(2):
                for implicit in (True,False):
                    init(stack,32,case%2)
                    for _ in range(case%4):check(push(None)==1,'live stack push')
                    stack_before=rng.randbytes(4096);C.memmove(stack,stack_before,4096);C.memmove(current,active,64)
                    memory=bytearray(rng.randbytes(256));memory[64:128]=active;C.memmove(arena,bytes(memory),256);ram[0xd10000:0xd10100]=memory
                    metadata={n:I.in_dll(frame,n).value for n in ('_stack_size_','_max_stack_size_','_nj_matrix_flag_')}
                    cpu=executor.State();cpu.pc=symbols[name];cpu.pr=0x8cfed000;cpu.sr=0x70000000;cpu.r[15]=0x8cf00000
                    cpu.r[4]=0 if implicit else 0x8cd10040
                    for i,value in enumerate(angles):cpu.r[5+i]=value&0xffffffff
                    fp=Fpu();fp.fpscr=0x40001|(bank<<21);fp.banks[bank^1][:]=struct.unpack('<16I',active)
                    reference(ram,cpu,fpu=fp,vector_instruction=service,instruction_budget=5000)
                    fn(None if implicit else C.addressof(arena)+64,*angles)
                    expected=struct.pack('<16I',*fp.banks[bank^1])
                    check(cpu.pc==0x8cfed000 and cpu.r[15]==0x8cf00000,'complete original return and balanced guest stack')
                    check(C.string_at(current,64)==expected,(name,case,bank,implicit,'active matrix exact original bits'))
                    check(bytes(arena)==bytes(ram[0xd10000:0xd10100]),(name,case,bank,implicit,'complete explicit matrix and guards'))
                    check(bytes(stack)==stack_before,'native stack memory unchanged')
                    check(all(I.in_dll(frame,n).value==v for n,v in metadata.items()),'native stack metadata unchanged')
                    calls[name]=calls.get(name,0)+1
    sections['composite_rotation_original_SH4_checks']=checks-start
    result=dict(status='marker_values_and_sampled_finite_composite_rotations_match_JP_binary',checks=checks,sections=sections,original_calls=calls,reference_source_sha256=reference_hashes,binary_sha256=sha(a.binary),build_verification_sha256=sha(a.build/'build_verification.json'),verifier_sha256=sha(__file__),all_float_classes_or_all_tree_branches_qualified=False,playable=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],checks,'checks',flush=True)
if __name__=='__main__':main()
