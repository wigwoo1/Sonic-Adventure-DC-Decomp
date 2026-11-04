"""Nonuniform palette banks, protected texture spans, mutated packet streams."""
import argparse,ctypes as C,hashlib,json,random,struct
from pathlib import Path
from native_packet_test_support import Decoder,Plan,bind
from native_texture_test_support import Runtime
from verify_native_packets import fixture
from verify_native_texels import Level,positions
U=C.c_uint32;S=C.c_size_t;P=C.c_void_p;I=C.c_int
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','packets','texels','report']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();assert not a.report.exists();bp=json.loads((a.packets/'build_verification.json').read_text());bt=json.loads((a.texels/'build_verification.json').read_text())
    for b in [bp,bt]:
        for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h
        assert sha(Path(b['DLL']))==b['DLL_sha256']
    runtime=Runtime(Path(bp['frames_DLL']));decoder=Decoder(Path(bp['DLL']));lib=C.CDLL(str(Path(bt['DLL'])))
    decode=bind(lib,'jp_texel_decode',I,U,U,U,U,P,S,P,S,P,S,P);checks=pixels=valid=invalid=palette_cases=0;rng=random.Random(0x434f4e53)
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,label
    palette=(U*1024)(*[((j%256)<<24)|(((j*13)%256)<<16)|(((j*29)%256)<<8)|((j*47)%256) for j in range(1024)])
    for fmt in [5,6]:
        for bank in range(64 if fmt==5 else 4):
            for w,h in [(8,16),(16,8),(16,16)]:
                count=w*h;limit=16 if fmt==5 else 256;values=[(j*7+j//w*3+5)%limit for j in range(count)]
                raw=bytearray(count//2 if fmt==5 else count)
                for k,(x,y) in enumerate(positions(w,h)):
                    index=values[y*w+x]
                    if fmt==5:raw[k//2]|=index<<((k%2)*4)
                    else:raw[k]=index
                src=C.create_string_buffer(bytes(raw));dst=C.create_string_buffer(count*4);info=Level();tsp=((w.bit_length()-4)<<3)|(h.bit_length()-4);tcw=(fmt<<27)|(bank<<(21 if fmt==5 else 25))
                check(decode(tsp,tcw,0,0,src,len(raw),palette,1024,dst,count*4,C.byref(info))==0,'nonuniform palette decode')
                expected=bytearray()
                for index in values:
                    c=palette[bank*limit+index];expected+=bytes([(c>>16)&255,(c>>8)&255,c&255,c>>24])
                check(dst.raw==expected,'independent bank/address pixels');pixels+=count;palette_cases+=1
    kernel=C.WinDLL('kernel32',use_last_error=True);alloc=bind(kernel,'VirtualAlloc',P,P,S,U,U);protect=bind(kernel,'VirtualProtect',I,P,S,U,C.POINTER(U));free=bind(kernel,'VirtualFree',I,P,S,U)
    address=alloc(None,16384,0x3000,4);check(bool(address),'texture protected allocation');old=U()
    check(protect(address+4096,4096,1,C.byref(old))!=0 and protect(address+12288,4096,1,C.byref(old))!=0,'protected source/output ends')
    for tcw,source_bytes in [(0,128),(0x04000000,128),(0x40000000,2064),(5<<27,32),(6<<27,64)]:
        src=address+4096-source_bytes;out=address+12288-256;C.memset(src,0,source_bytes);C.memset(out,0x55,256);info=Level()
        check(decode(0,tcw,0,0,src,source_bytes,palette,1024,out,256,C.byref(info))==0,'protected encoded surface')
        C.memset(out,0x5a,256);before=bytes(info)
        check(decode(0,tcw,0,0,src,source_bytes-1,palette,1024,out,256,C.byref(info))==3,'protected short source')
        check(C.string_at(out,256)==b'Z'*256 and bytes(info)==before,'protected atomic output')
        check(decode(0,tcw,0,0,src,source_bytes,palette,1024,out,255,C.byref(info))==4,'protected short output')
    check(free(address,0,0x8000)!=0,'protected texture free')
    # Deterministic mutation fuzzing validates syntax/sequence/NaN/error atomicity
    # independently of the known-value fixtures. It is not a coverage claim.
    for case in range(6000):
        lane=case%5
        raw,_,_=fixture(case%3,(case//3)%2,(case//6)%2,(case//12)%2,3+case%7,min(lane,2))
        mutated=bytearray(raw)
        for _ in range(1+case%8):
            at=rng.randrange(len(mutated));mutated[at]^=rng.randrange(1,256)
        if case%7==0:mutated=mutated[:rng.randrange(len(mutated)+1)]
        lists=[b'']*5;lists[lane]=bytes(mutated);rc,out,error=decoder.raw(lists,1<<20)
        if rc:
            invalid+=1;check(out.value==0x12345678 and error.code==rc and error.list<5 and error.offset<=len(mutated),'mutated failure location/atomicity')
        else:
            valid+=1;plan=Plan.from_address(out.value)
            check(plan.allocation_bytes<=1<<20,'mutated valid bounded allocation')
            for i in range(plan.triangle_count):check(all(j<plan.vertex_count for j in plan.triangles[i].indices),'mutated triangle bounds')
            for i in range(plan.batch_count):
                b=plan.batches[i];check(b.first_vertex+b.vertex_count<=plan.vertex_count and b.first_triangle+b.triangle_count<=plan.triangle_count,'mutated batch bounds')
            decoder.destroy(out)
    report=dict(status='native_consumer_extended_boundaries_verified',checks=checks,nonuniform_palette_cases=palette_cases,palette_pixels_compared=pixels,mutated_packet_cases=6000,mutated_valid=valid,mutated_rejected=invalid,
        protected_texture_source_and_output_verified=True,packet_build_report_sha256=sha(a.packets/'build_verification.json'),texel_build_report_sha256=sha(a.texels/'build_verification.json'),
        verifier_sha256=sha(Path(__file__)),support_source_sha256={n:sha(Path(__file__).parent/n) for n in ['native_packet_test_support.py','verify_native_packets.py','verify_native_texels.py']},
        game_frame_executed=False,hardware_pixels_verified=False,playable=False,overall_completion_percent=10)
    a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
if __name__=='__main__':main()
