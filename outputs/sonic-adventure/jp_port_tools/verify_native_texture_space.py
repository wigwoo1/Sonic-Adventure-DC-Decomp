"""Link original njCalcTexture to the actual native texture budget provider."""
import argparse,ctypes as C,hashlib,json,subprocess
from pathlib import Path
from build_native_texture_runtime import function
from native_texture_test_support import Runtime,Surface
from native_packet_test_support import bind
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','baseline','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();d=a.output.resolve();b=json.loads((a.baseline/'verification.json').read_bytes())
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['kept_objects_sha256'].items():assert sha(n)==h,n
    assert sha(b['DLL'])==b['DLL_sha256'];d.mkdir()
    body=function((u/'sdk/ninja/njTexture.c').read_bytes(),'njCalcTexture')
    sdk=d/'external_texture_space.c';sdk.write_bytes(b'#include <ninja.h>\n#include <kamui.h>\nUint32 _nj_tex_max_free_size;\n'+body)
    owner=r/'outputs/sonic-adventure/native/jp_native_texture_space.c';compile=list(b['commands'][0]['command']);compile[compile.index('-c')+1]=str(owner);compile[-1]=str(d/'texture_space.obj')
    original=list(compile);original[original.index('-c')+1]=str(sdk);original[-1]=str(d/'texture_space_sdk.obj')
    link=list(b['commands'][1]['command']);old=next(n for n in link if Path(n).name=='texture_budget.obj');link[link.index(old)]=compile[-1];link[-1]=str(d/'jp_native_frames.dll');at=link.index('-o');link[at:at]=[original[-1],'-Wl,/export:njCalcTexture','-Xlinker','/export:_nj_tex_max_free_size,DATA']
    link.insert(link.index('-o'),'-Wl,/export:kmGetFreeTextureMem')
    commands=[]
    for i,cmd in enumerate([compile,original,link]):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr);commands.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));assert run.returncode==0 and not log.stat().st_size,log.read_text()
    t=Runtime(Path(link[-1]));calc=bind(t.dll,'njCalcTexture',C.c_uint32,C.c_uint32);free=bind(t.dll,'kmGetFreeTextureMem',C.c_int,C.POINTER(C.c_uint32),C.POINTER(C.c_uint32));replace=bind(t.dll,'jp_texture_replace_standalone_surface',C.c_int,C.POINTER(Surface),C.c_void_p,C.c_size_t);release=bind(t.dll,'kmFreeTexture',C.c_int,C.POINTER(Surface));cache=C.c_uint32.in_dll(t.dll,'_nj_tex_max_free_size');checks=0
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    x=C.c_uint32(17);y=C.c_uint32(23)
    check(free(C.byref(x),C.byref(y))==3 and x.value==y.value==0,'uninitialized provider safely zeroes original consumer outputs');check(calc(0)==0,'original consumer safe on owner failure')
    for limit in [4096,8192,65536]:
        check(t.init(2,8,limit)==0,'actual texture owner initialized');initial=limit-t.allocated();cache.value=initial
        font=Surface();font.nWidth=font.nHeight=8;font.TextureSize=128;raw=C.create_string_buffer(bytes(range(128)))
        check(replace(C.byref(font),raw,128)==0,'live standalone font');pointer=font.pSurface
        for _ in range(32):
            check(free(C.byref(x),C.byref(y))==0 and x.value==y.value==initial-128,'Kamui receives actual host allowance in bytes')
            check(calc(0)==calc(1)==calc(0xffffffff)==initial-128 and calc(2)==initial,'unchanged original flag dispatch and cached maximum')
            check(font.pSurface==pointer and C.string_at(pointer,128)==raw.raw[:128],'original query does not discard font')
        x.value=99;check(free(C.byref(x),None)==0xdddd and x.value==99,'invalid output pointer leaves valid output untouched')
        check(release(C.byref(font))==0 and calc(0)==initial and calc(2)==initial,'free restores live budget independently of cache')
        t.shutdown();check(calc(0)==0 and calc(2)==initial,'shutdown zero live allowance preserves cached original maximum')
    check(t.init(2,8,1<<33)==0,'64-bit host budget uses only small actual metadata allocation');check(free(C.byref(x),C.byref(y))==0 and x.value==y.value==calc(0)==0xffffffff,'32-bit SDK results saturate instead of wrapping');t.shutdown()
    result=dict(status='original_njCalcTexture_native_Kamui_host_budget_dependency_verified',checks=checks,commands=commands,baseline_proof_sha256=sha(a.baseline/'verification.json'),DLL=link[-1],DLL_sha256=sha(link[-1]),authored_source_sha256={str(p.relative_to(r)):sha(p) for p in [owner,Path(__file__).resolve()]},original_source_sha256=sha(u/'sdk/ninja/njTexture.c'),original_function_sha256=hashlib.sha256(body).hexdigest(),generated_source_sha256=sha(sdk),original_njCalcTexture_body_unchanged=True,host_allocation_policy_explicit=True,console_VRAM_fragmentation_emulated=False,kept_objects_sha256=b['kept_objects_sha256'],artifact_publishable=False,retail_inputs_used=False,njInitVertexBuffer_integrated=False,playable=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],checks,'checks',flush=True)
if __name__=='__main__':main()
