"""Link original FM/CRC with native durable flash; preserve the qualified graph."""
import argparse,ctypes as C,hashlib,json,re,subprocess
from pathlib import Path
from build_native_texture_runtime import function
from build_native_renderer_object_v3 import coff_symbols
PIN='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
FM=('fmMngInit','fmMngFormat','fmMngRead','fmMngWrite','fmMngExecServer',
    'fmMngGetErr','fmMngFinish','fmMngRefresh','fmMngGetFree')

def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r,u,base,d=(getattr(a,n).resolve() for n in ('workspace','upstream','baseline','output'))
    p=r/'outputs/sonic-adventure';b=read(base/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    sources=('sdk/shinobi/fm.c','sdk/shinobi/fmcrc.c','sdk/shinobi/sybtfl.src',
             'sdk/include/shinobi.h','sdk/include/sg_xpt.h','src/main.c')
    originals={}
    for n in sources:
        raw=(u/n).read_bytes();pin=subprocess.check_output(['C:/Program Files/Git/cmd/git.exe','-C',str(u),'show',PIN+':'+n])
        assert raw==pin,n;originals[n]=sha(u/n)
    d.mkdir();raw=(u/'sdk/shinobi/fm.c').read_bytes()
    # Load SDK declarations in their existing native-compatible mode first.
    # GCC selects the original initialized map-byte branch only afterward;
    # defining it before machine.h would enable SH-4 inline assembly on x64.
    prefix=b''.join(('#define '+n+' jp_flash_source_'+n+'\n').encode() for n in FM)
    prefix+=b'#include <shinobi.h>\n#define GCC\n'
    suffix=b'\n'+b''.join(('#undef '+n+'\n').encode() for n in FM)+(p/'native/jp_native_flash_fm.inc').read_bytes()
    generated=d/'external_flash_manager.c';generated.write_bytes(prefix+raw+suffix)
    crc=d/'external_flash_crc.c';crc.write_bytes((u/'sdk/shinobi/fmcrc.c').read_bytes())
    common=list(next(row['command'] for row in b['commands'] if '-c' in row['command']
        and any(x.startswith('-I') and Path(x[2:]).resolve()==u/'sdk/include' for x in row['command'])))
    common=common[:common.index('-c')];compiler=common[0]
    host=[compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror','-I'+str(p/'native')]
    commands=[common+['-c',str(generated),'-o',str(d/'flash_original.obj')],
        common+['-c',str(crc),'-o',str(d/'flash_crc.obj')],
        host+['-c',str(p/'native/jp_native_flash.c'),'-o',str(d/'flash_owner.obj')],
        [compiler,'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',
         str(d/'flash_original.obj'),str(d/'flash_crc.obj'),str(d/'flash_owner.obj'),
         '-Wl,/export:fmcalccrc','-o',str(d/'jp_native_flash.dll')]]
    # Compile the full parent without linking or executing unresolved services.
    main=(u/'src/main.c').read_bytes();body=function(main,'game_init')
    probe=d/'external_game_init_probe.c';probe.write_bytes(main[:main.index(b'enum menu_entrance menu_entrance;')]+b'\n'+body)
    commands.append(common+['-c',str(probe),'-o',str(d/'game_init_probe.obj')])
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'compile_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size))
        (d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n')
        assert not run.returncode and not log.stat().st_size,log.read_text()
    modules={};seen=set()
    for key,v in b.items():
        if (key=='DLL' or key.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll') and v not in seen:
            assert sha(v)==b[key+'_sha256'];seen.add(v);modules[key]=C.CDLL(v)
    unresolved={s['name']:[] for s in coff_symbols(d/'game_init_probe.obj') if s['section']==0 and s['name']}
    for n in unresolved:
        for key,lib in modules.items():
            if hasattr(lib,n.removeprefix('__imp_')):unresolved[n].append(key)
    authored=[p/'native'/n for n in ('jp_native_flash.h','jp_native_flash.c','jp_native_flash_fm.inc')]+[Path(__file__).resolve()]
    local=dict(status='native_durable_flash_original_packet_manager_and_CRC_linked_runtime_unverified',
        flash_DLL=str(d/'jp_native_flash.dll'),flash_DLL_sha256=sha(d/'jp_native_flash.dll'),
        flash_baseline_sha256=sha(base/'build_verification.json'),commands=results+b['commands'],
        authored_source_sha256=dict(b['authored_source_sha256'],**{x.relative_to(r).as_posix():sha(x) for x in authored}),
        flash_original_source_sha256=originals,
        flash_generated_source_sha256={x.name:sha(x) for x in (generated,crc,probe)},
        original_FM_translation_unit_preserved=True,original_CRC_translation_unit_preserved=True,
        original_FM_export_names_privately_renamed_for_native_guards=True,
        original_FM_cache_and_request_state_not_duplicated=True,
        flash_object_sha256={x.name:sha(x) for x in d.glob('*.obj')},
        game_init_original_body_sha256=hashlib.sha256(body).hexdigest(),
        game_init_actual_import_providers=unresolved,full_game_init_executed=False,
        combined_game_SDK_binaries_publishable=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(dict(b,**local),indent=2)+'\n')
    print(local['status'],flush=True);print('Parent unresolved:',','.join(n for n,v in unresolved.items() if not v),flush=True)
if __name__=='__main__':main()
