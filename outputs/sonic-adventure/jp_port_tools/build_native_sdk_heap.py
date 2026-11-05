"""Recover the real SDK heap algorithms with bounded native ownership.
Generated original source/combined DLL remain private; no CRT interposition.
"""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
from build_native_texture_runtime import function
from build_native_renderer_object_v3 import coff_symbols
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure'
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build')
    b=json.loads((a.baseline/'build_verification.json').read_bytes());names=['syMallocInit','syMallocFinish','syMallocStat','syMalloc','syFree']
    for path,h in b['kept_frame_objects_sha256'].items():
        assert sha(path)==h,path
        assert not any(s['section']>0 and s['name'] in names for s in coff_symbols(Path(path))),path
    originals={n:sha(u/n) for n in ('sdk/shinobi/syMalloc.c','sdk/include/shinobi.h','sdk/include/sg_xpt.h','sdk/include/stdlib.h','sdk/include/machine.h')}
    git='C:/Program Files/Git/cmd/git.exe'
    for n,h in originals.items():assert hashlib.sha256(subprocess.check_output([git,'-C',str(u),'show','25a3d3154dbe657325e97dbe8c5291086f6af5c0:'+n])).hexdigest()==h,n
    d.mkdir();raw=(u/'sdk/shinobi/syMalloc.c').read_bytes();selected=['syMallocInit','syMallocStat','syMalloc','malloc','syFree','free','calloc'];bodies={n:function(raw,n) for n in selected}
    prefix=b'''#define syMallocInit jp_sdk_source_init
#define syMallocFinish jp_sdk_source_unused_finish
#define syMallocStat jp_sdk_source_stat
#define syMalloc jp_sdk_source_syMalloc
#define syFree jp_sdk_source_syFree
#define malloc jp_sdk_source_malloc
#define free jp_sdk_source_free
#define calloc jp_sdk_source_calloc
#define realloc jp_sdk_source_unsafe_realloc_not_defined
'''
    source=prefix+raw[:raw.index(bodies['syMallocInit'])]+b'''\n#include <stdint.h>
#include "jp_native_sdk_heap_internal.h"
#undef get_imask
#undef set_imask
#define get_imask() jp_sdk_heap_mask_get()
#define set_imask(value) jp_sdk_heap_mask_set(value)
'''
    changes=[]
    for name,body in bodies.items():
        altered=body
        if name=='syMallocInit':
            assert altered.count(b'(Uint32)heap')==1;altered=altered.replace(b'(Uint32)heap',b'(uintptr_t)heap')
            changes.append(dict(function=name,adaptation='one pointer-width alignment cast; caller supplies bounded usable bytes'))
        if name=='free':
            for old,new,count in [(b'block > before',b'(uintptr_t)block > (uintptr_t)before',2),(b'block < before->header.next',b'(uintptr_t)block < (uintptr_t)before->header.next',2),(b'before >= before->header.next',b'(uintptr_t)before >= (uintptr_t)before->header.next',1)]:
                assert altered.count(old)==count,(name,old,altered.count(old));altered=altered.replace(old,new)
            changes.append(dict(function=name,adaptation='five native uintptr_t comparisons preserve circular address ordering without unrelated-pointer relational UB'))
        source+=altered+b'\n'
    source+=b'\n'+(p/'native/jp_native_sdk_heap_observer.inc').read_bytes()
    generated=d/'external_sdk_heap_native.c';generated.write_bytes(source)
    common=list(next(x['command'] for x in b['commands'] if '-c' in x['command'] and any(n.startswith('-I') and Path(n[2:]).resolve()==u/'sdk/include' for n in x['command'])))
    common=common[:common.index('-c')];compiler=common[0]
    own=p/'native/jp_native_sdk_heap.c';commands=[common+['-c',str(generated),'-o',str(d/'sdk_heap_source.obj')],
        [compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-c',str(own),'-o',str(d/'sdk_heap_owner.obj')],
        [compiler,'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',str(d/'sdk_heap_source.obj'),str(d/'sdk_heap_owner.obj'),'-Wl,kernel32.lib','-o',str(d/'jp_native_sdk_heap.dll')]]
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n')
        assert not run.returncode and not log.stat().st_size,log.read_text()
    for n,h in originals.items():assert sha(u/n)==h,n
    authored=[own,p/'native/jp_native_sdk_heap.h',p/'native/jp_native_sdk_heap_internal.h',p/'native/jp_native_sdk_heap_observer.inc',Path(__file__).resolve()]
    result=dict(status='actual_original_SDK_heap_algorithms_bounded_native_owner_linked_unverified',commands=results,DLL=str(d/'jp_native_sdk_heap.dll'),DLL_sha256=sha(d/'jp_native_sdk_heap.dll'),
        authored_source_sha256={str(x.relative_to(r)):sha(x) for x in authored},original_source_sha256=originals,
        original_function_sha256={n:hashlib.sha256(v).hexdigest() for n,v in bodies.items()},selected_original_functions=selected,source_adaptations=changes,
        generated_sources_sha256={generated.name:sha(generated)},original_unsafe_realloc_or_empty_finish_body_compiled=False,
        native_CRT_malloc_free_realloc_calloc_replaced=False,heap_local_mask_bookkeeping_under_real_native_SRW_lock=True,
        standalone_original_SDK_allocator_DLL=True,sbInitSystem_global_interrupt_mask_or_general_exit_integration_implemented=False,
        baseline_build_sha256=sha(a.baseline/'build_verification.json'),artifact_publishable=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)
if __name__=='__main__':main()
