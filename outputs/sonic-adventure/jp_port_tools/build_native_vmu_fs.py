"""Build the pinned original VMS filesystem over actual host fibers/card I/O.
Generated SDK code and the linked DLL remain private research artifacts.
"""
import argparse, hashlib, json, re, subprocess
from pathlib import Path
from build_native_texture_runtime import function

def sha(p): return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for name in ('workspace','upstream','baseline','store','output'):
        ap.add_argument('--'+name,type=Path,required=True)
    a=ap.parse_args(); r=a.workspace.resolve(); u=a.upstream.resolve(); d=a.output.resolve()
    p=r/'outputs/sonic-adventure'; w=r/'work/sonic-adventure'
    assert not d.exists() and d.is_relative_to(w) and not d.is_relative_to(w/'build')
    base=json.loads((a.baseline/'build_verification.json').read_bytes())
    store=json.loads((a.store/'verification.json').read_bytes())
    assert sha(a.store/'jp_native_vmu_store.dll')==store['DLL_sha256']
    assert sha(a.store/'vmu_store.obj')==store['owner_object_sha256']
    for name,h in store['authored_source_sha256'].items(): assert sha(r/name)==h,name
    for name,h in base['authored_source_sha256'].items(): assert sha(r/name)==h,name
    files=['sdk/shinobi/'+name+'.c' for name in ('vmsphys','vms_rw','vmscmd','vmsapi','vmsfib')]
    files+=['sdk/include/vmsfs.h','sdk/include/rfiber.h','sdk/shinobi/rfiber.c','sdk/shinobi/vmsio.c','sdk/shinobi/vmsasm.src']
    files+=['sdk/include/'+n+'.h' for n in ('sg_xpt','shinobi','machine','sg_chain','stdarg','stdio','string')]
    originals={name:sha(u/name) for name in files}
    git=Path('C:/Program Files/Git/cmd/git.exe')
    assert subprocess.check_output([str(git),'-C',str(u),'rev-parse','HEAD']).decode().strip()=='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
    for name,h in originals.items():
        tracked=subprocess.check_output([str(git),'-C',str(u),'show','HEAD:'+name])
        assert hashlib.sha256(tracked).hexdigest()==h,'modified pinned external source '+name
    d.mkdir(); inc=d/'include'; inc.mkdir(); changes=[]
    def replace(raw,old,new,label,count=1):
        assert raw.count(old)==count,(label,raw.count(old))
        changes.append(dict(reason=label,count=count,old_sha256=hashlib.sha256(old).hexdigest(),new_sha256=hashlib.sha256(new).hexdigest()))
        return raw.replace(old,new)
    header=(u/'sdk/include/vmsfs.h').read_bytes()
    for n in range(1,7):
        old=('Sint32 param'+str(n)).encode()
        header=replace(header,old,('intptr_t param'+str(n)).encode(),'native pointer-bearing command slot '+str(n),2)
    (inc/'vmsfs.h').write_bytes(b'#include <stdint.h>\n'+header)
    sources=[]
    # Parse hardware headers first without enabling their SH-4 assembly.
    # GCC then enables upstream explicit-return/loop repairs and native varargs.
    prefix=b'#include <sg_xpt.h>\n#include <shinobi.h>\n#include <machine.h>\n#include <sg_chain.h>\n#include <vmsfs.h>\n#define GCC\n#include <stdarg.h>\nint jp_vmu_debug_format(char *,size_t,const char *,va_list);\n'
    for name in files[:5]:
        raw=(u/name).read_bytes()
        if name.endswith('vmsphys.c'):
            raw=replace(raw,b'vsprintf(print_buf, format, args);',b'jp_vmu_debug_format(print_buf, sizeof(print_buf), format, args);','bounded native CRT debug formatting')
        if name.endswith('vmscmd.c'):
            for n in range(1,7):
                raw=replace(raw,('Sint32 param'+str(n)).encode(),('intptr_t param'+str(n)).encode(),'native command setter argument '+str(n))
        if name.endswith('vmsapi.c'):
            for n in ('buInit','buDisableServer','buExit','buInterruptHandler'):
                raw=replace(raw,function(raw,n),b'','replace hardware lifecycle '+n)
            for n,new in [('buExecServer','jp_vms_exec_original'),('buStat','jp_vms_stat_original')]:
                body=function(raw,n); raw=replace(raw,body,body.replace((n+'(').encode(),(new+'(').encode(),1),'native service wrapper '+n)
            for n in ('volume','time','file','buf','fname','from','to','work'):
                old=('(Sint32)'+n).encode(); count=raw.count(old)
                if count: raw=replace(raw,old,('(intptr_t)'+n).encode(),'preserve full native pointer '+n,count)
            raw+=b'\nvoid jp_vms_reset_api_state(void *work) { buWorkTop=work; drives_last_ready=drives_ready=drives_to_mount=drives_to_remove=0; }\n'
        target=d/('external_'+Path(name).name); target.write_bytes(prefix+raw); sources.append(target)
    common=list(base['commands'][0]['command']); common=common[:common.index('-c')]
    # These SDK locals intentionally reproduce register allocation in SHC;
    # neither is a missing host operation (the original comments mark them).
    common.append('-Wno-unused-but-set-variable')
    # Put the private pointer-width overlay ahead of the unchanged SDK headers.
    common.insert(1,'-I'+str(inc))
    host=[common[0],'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffreestanding','-fno-builtin','-Wall','-Wextra','-Werror']
    own=[p/'native/jp_native_vmu_fs.c',p/'native/jp_native_fibers.c',p/'native/jp_native_vmu_host.c']
    commands=[]; objects=[]
    for source in sources+own:
        obj=d/(source.stem+'.obj'); objects.append(obj)
        flags=common if source.name=='jp_native_vmu_fs.c' or source in sources else host
        commands.append(flags+['-c',str(source),'-o',str(obj)])
    # The previously qualified block owner is linked unchanged; it owns the
    # only card image and cross-process lock, rather than a second cache.
    objects.append(a.store/'vmu_store.obj')
    dll=d/'jp_native_vmu_fs.dll'
    api=(u/'sdk/include/shinobi.h').read_text()
    exports=sorted(set(re.findall(r'\b(bu[A-Za-z0-9_]+)\s*\(',api)))
    # The reconstructed public header omits original handle APIs used by the
    # command dispatcher. Export those actual definitions too.
    api_source=(u/'sdk/shinobi/vmsapi.c').read_text()
    exports=sorted(set(exports+re.findall(r'(?m)^(?:Sint32|Uint32|void|BUFILE)\s+(bu\w+)\s*\(',api_source)))
    # Codec helpers remain in the separately qualified backup-image owner.
    exports=[n for n in exports if n not in ['buMakeBackupFileImage','buCalcBackupFileSize','buAnalyzeBackupFileImage','buCalcCRC','buInterruptHandler']]
    commands.append([common[0],'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',*[str(x) for x in objects],*['-Wl,/export:'+n for n in exports],'-o',str(dll)])
    results=[]
    for i,cmd in enumerate(commands):
        if i==len(commands)-1:
            assert all(not x['exit_code'] and not x['diagnostic_bytes'] for x in results),'see compile_attempt.json and build logs'
        run=subprocess.run(cmd,capture_output=True); log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size))
        (d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n')
    assert all(not x['exit_code'] and not x['diagnostic_bytes'] for x in results),'see compile_attempt.json and build logs'
    for name,h in originals.items(): assert sha(u/name)==h,name
    result=dict(status='original_VMS_filesystem_native_fibers_and_block_IO_linked_not_runtime_verified',commands=results,DLL=str(dll),DLL_sha256=sha(dll),original_source_sha256=originals,generated_source_sha256={str(x.relative_to(d)):sha(x) for x in [inc/'vmsfs.h']+sources},authored_source_sha256={str(x.relative_to(r)):sha(x) for x in own+[p/'native/jp_native_fibers.h',Path(__file__).resolve()]},changes=changes,store_verification_sha256=sha(a.store/'verification.json'),store_object_sha256=sha(a.store/'vmu_store.obj'),exports=exports,artifact_publishable=False,whole_game_integration=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n'); print(result['status'],flush=True)

if __name__=='__main__': main()
