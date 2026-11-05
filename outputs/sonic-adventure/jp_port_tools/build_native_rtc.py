"""Build the original RTC algorithms with a real native clock/chain boundary.
Original generated source, SDK client and combined DLLs stay private.
"""
import argparse,hashlib,json,shutil,subprocess
from pathlib import Path
from build_native_texture_runtime import function

def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure'
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build')
    b=json.loads((a.baseline/'build_verification.json').read_bytes())
    original_names=['sdk/shinobi/sg_syrtc.c','sdk/include/shinobi.h','sdk/include/sg_xpt.h','src/save/card.c']
    originals={n:sha(u/n) for n in original_names};git='C:/Program Files/Git/cmd/git.exe'
    for n,h in originals.items():assert hashlib.sha256(subprocess.check_output([git,'-C',str(u),'show','25a3d3154dbe657325e97dbe8c5291086f6af5c0:'+n])).hexdigest()==h,n
    d.mkdir();copies={}
    for key,value in b.items():
        if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and Path(value).suffix=='.dll':
            source=Path(value);assert sha(source)==b[key+'_sha256'];target=d/source.name
            shutil.copyfile(source,target);shutil.copyfile(source.with_suffix('.lib'),target.with_suffix('.lib'))
            copies[key]=dict(path=str(target),sha256=sha(target))
    names={'syRtcExecServer':'jp_rtc_source_server','syRtcGetDate':'jp_rtc_source_date','syRtcSetServerMode':'jp_rtc_source_mode',
        'syRtcGetStat':'jp_rtc_source_stat','syRtcCompareDate':'jp_rtc_source_compare','syRtcDateToCount':'jp_rtc_source_to_count',
        'syRtcCountToDate':'jp_rtc_source_to_date','syRtcGetAndCheckCount':'jp_rtc_source_check',
        'syRtcMakeDayOfWeek':'jp_rtc_source_week','syRtcMakeAgeOfMoon':'jp_rtc_source_moon'}
    raw=(u/'sdk/shinobi/sg_syrtc.c').read_bytes();bodies={n:function(raw,n) for n in names}
    macros=dict(names,syRtcAicaGetCount='jp_rtc_host_read',syRtcGetCountForce='jp_rtc_bounded_force')
    prefix=''.join('#define '+k+' '+v+'\n' for k,v in macros.items()).encode()
    source=prefix+raw[:raw.index(function(raw,'syRtcInit'))]+b'\n'+b'\n'.join(bodies.values())
    generated=d/'external_rtc_native.c';generated.write_bytes(source)
    card=function((u/'src/save/card.c').read_bytes(),'card_get_time')
    client=d/'external_rtc_sdk_client.c';client.write_bytes(b'''#include <shinobi.h>
'''+card+b'''
__declspec(dllexport) void client_card_time(BUS_TIME *out){card_get_time(out);}
__declspec(dllexport) long client_get_date(SYS_RTC_DATE *out){return syRtcGetDate(out);}
__declspec(dllexport) long client_set_date(const SYS_RTC_DATE *date){return syRtcSetDate(date);}
__declspec(dllexport) unsigned long client_date_size(void){return sizeof(SYS_RTC_DATE);}
__declspec(dllexport) unsigned long client_time_size(void){return sizeof(BUS_TIME);}
''')
    common=list(next(x['command'] for x in b['commands'] if '-c' in x['command'] and any(n.startswith('-I') and Path(n[2:]).resolve()==u/'sdk/include' for n in x['command'])))
    common=common[:common.index('-c')];compiler=common[0]
    host=[compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-Wall','-Wextra','-Werror']
    commands=[common+['-c',str(generated),'-o',str(d/'rtc_source.obj')],
        host+['-c',str(p/'native/jp_native_rtc.c'),'-o',str(d/'rtc_owner.obj')],
        [compiler,'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',str(d/'rtc_source.obj'),str(d/'rtc_owner.obj'),str(d/'jp_native_present.lib'),'-Wl,kernel32.lib','-o',str(d/'jp_native_rtc.dll')],
        common+['-c',str(client),'-o',str(d/'rtc_sdk_client.obj')],
        [compiler,'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',str(d/'rtc_sdk_client.obj'),str(d/'jp_native_rtc.lib'),'-o',str(d/'rtc_sdk_client.dll')]]
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n')
        assert not run.returncode and not log.stat().st_size,log.read_text()
    for n,h in originals.items():assert sha(u/n)==h,n
    authored=[p/'native'/('jp_native_rtc'+s) for s in ('.c','.h','_internal.h')]+[Path(__file__).resolve()]
    result=dict(status='original_RTC_calendar_server_and_real_native_clock_chain_linked_unverified',commands=results,
        RTC_DLL=str(d/'jp_native_rtc.dll'),RTC_DLL_sha256=sha(d/'jp_native_rtc.dll'),client_DLL=str(d/'rtc_sdk_client.dll'),client_DLL_sha256=sha(d/'rtc_sdk_client.dll'),
        authored_source_sha256={x.relative_to(r).as_posix():sha(x) for x in authored},original_source_sha256=originals,
        original_function_sha256={n:hashlib.sha256(body).hexdigest() for n,body in bodies.items()},
        original_card_get_time_sha256=hashlib.sha256(card).hexdigest(),generated_sources_sha256={x.name:sha(x) for x in (generated,client)},
        copied_DLLs=copies,baseline_build=str(a.baseline.resolve()),baseline_build_sha256=sha(a.baseline/'build_verification.json'),
        original_hardware_polling_and_register_write_bodies_compiled=False,original_calendar_server_bodies_unchanged=True,
        OS_clock_modified=False,full_system_bootstrap_or_general_exit_integration=False,artifact_publishable=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)
if __name__=='__main__':main()
