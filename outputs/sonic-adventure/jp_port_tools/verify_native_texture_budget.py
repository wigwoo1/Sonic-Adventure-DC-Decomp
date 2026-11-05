"""Build and test a read-only query in the actual standalone texture owner.
Private linked DLL retains external game/SDK objects; never publish that DLL.
"""
import argparse,ctypes as C,hashlib,json,subprocess
from pathlib import Path
from native_texture_test_support import Runtime,Surface,Info
from native_packet_test_support import bind
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
class Budget(C.Structure):_fields_=[(n,C.c_size_t) for n in ['limit','charged','available']]
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','baseline','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=json.loads((a.baseline/'build_verification.json').read_bytes())
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(b['frames_DLL'])==b['frames_DLL_sha256'];d.mkdir()
    common=list(b['commands'][0]['command']);common=common[:common.index('-c')]
    source=r/'outputs/sonic-adventure/native/jp_native_texture_budget.c';obj=d/'texture_budget.obj';dll=d/'jp_native_frames.dll'
    compile=common+['-c',str(source),'-o',str(obj)]
    link=list(next(x['command'] for x in b['commands'] if x['command'][-1]==b['frames_DLL']))
    old=next(n for n in link if Path(n).name=='standalone_texture_owner.obj');kept={n:sha(n) for n in link if n.endswith('.obj') and n!=old}
    link[link.index(old)]=str(obj);link[-1]=str(dll);commands=[]
    for i,cmd in enumerate([compile,link]):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        commands.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));assert run.returncode==0 and not log.stat().st_size,log.read_text()
    for n,h in kept.items():assert sha(n)==h,n
    t=Runtime(dll);query=bind(t.dll,'jp_texture_budget_query',C.c_int,C.POINTER(Budget));replace=bind(t.dll,'jp_texture_replace_standalone_surface',C.c_int,C.POINTER(Surface),C.c_void_p,C.c_size_t);free=bind(t.dll,'kmFreeTexture',C.c_int,C.POINTER(Surface))
    checks=0
    def check(ok,label):
        nonlocal checks
        checks+=1;assert ok,(checks,label)
    value=Budget(11,22,33);check(query(C.byref(value))==8 and tuple(getattr(value,n) for n in ['limit','charged','available'])==(11,22,33),'uninitialized output unchanged')
    for limit in [4096,8192,65536]:
        check(t.init(2,8,limit)==0,'actual owner initialized');check(query(C.byref(value))==0,'initial budget query')
        base=t.allocated();check(value.limit==limit and value.charged==base and value.available==limit-base,'real metadata charged')
        info=Info();s=info.texsurface;s.nWidth=s.nHeight=8;s.TextureSize=128;s.Type=0x10000;s.BitDepth=1;s.PixelFormat=0x08000000;s.fSurfaceFlags=4
        raw=C.create_string_buffer(bytes(range(128)));check(t.copy(C.byref(info),raw,128)==0,'actual record surface allocated')
        font=Surface();font.nWidth=font.nHeight=8;font.TextureSize=128
        check(replace(C.byref(font),raw,128)==0,'actual standalone font allocation')
        word=t.surface_word(C.byref(s));pointer=t.surface_data(word);font_pointer=font.pSurface
        for _ in range(32):
            check(query(C.byref(value))==0 and value.charged==base+256 and value.charged==t.allocated() and value.available==limit-base-256,'both real payloads charged once')
            check(t.surface_data(word)==pointer and font.pSurface==font_pointer and C.string_at(pointer,128)==raw.raw[:128] and C.string_at(font_pointer,128)==raw.raw[:128],'query preserves textures and font bytes/identity')
        saved=bytes(value);check(query(None)==1,'null query rejected');check(query(C.byref(value))==1 and bytes(value)==saved,'sticky failure leaves output unchanged');t.clear()
        check(free(C.byref(font))==0 and query(C.byref(value))==0 and value.charged==base+128,'standalone release restores allowance')
        check(replace(C.byref(font),raw,128)==0,'font restored')
        # Oversized real allocation fails, preserving previous font and query.
        bad=Surface();bad.nWidth=bad.nHeight=64;bad.TextureSize=limit
        check(replace(C.byref(bad),raw,limit)==3,'budget failure occurs before input copy');t.clear()
        check(query(C.byref(value))==0 and value.charged==base+256 and C.string_at(font.pSurface,128)==raw.raw[:128],'failed allocation preserves accounting and existing font')
        t.shutdown();saved=bytes(value);check(query(C.byref(value))==8 and bytes(value)==saved,'shutdown query preserves caller output')
    result=dict(status='actual_texture_owner_budget_and_font_preservation_verified',checks=checks,commands=commands,baseline_build_sha256=sha(a.baseline/'build_verification.json'),DLL=str(dll),DLL_sha256=sha(dll),owner_object_sha256=sha(obj),kept_objects_sha256=kept,authored_source_sha256={str(p.relative_to(r)):sha(p) for p in [source,source.with_suffix('.h'),Path(__file__).resolve()]},verifier_sha256=sha(__file__),native_functions_replaced=False,retail_inputs_used=False,artifact_publishable=False,host_budget_not_console_VRAM_or_OS_contiguous_space=True,njInitVertexBuffer_integrated=False,playable=False,overall_completion_percent=10)
    (d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],checks,'checks',flush=True)
if __name__=='__main__':main()
