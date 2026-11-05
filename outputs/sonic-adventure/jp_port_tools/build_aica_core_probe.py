"""Build pinned external ARM/AICA core privately for real-driver qualification.

No original game/SDK code, retail assets, third-party source or combined core
binary is selected for public publishing by this workflow.
"""
import argparse,hashlib,json,subprocess
from pathlib import Path

PIN='2998a4bf550949cd2daee249e725a64462cf15e0'
EXPORTS=('dcsound_get_state_size','dcsound_clear_state','dcsound_get_arm_state','dcsound_get_yam_state','dcsound_getword','dcsound_setword','dcsound_upload_to_ram','dcsound_execute','dcsound_get_pc','arm_getreg','yam_aica_load_reg','yam_aica_store_reg','yam_enable_dry','yam_enable_dsp','yam_enable_dsp_dynarec')
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','core','compiler','output'):ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--ram-mib',type=int,choices=(2,8),default=2)
    a=ap.parse_args();r=a.workspace.resolve();u=a.core.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure'
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    git='C:/Program Files/Git/cmd/git.exe';assert subprocess.check_output([git,'-C',str(u),'rev-parse','HEAD'],text=True).strip()==PIN
    names=['LICENSE.TXT','Core/COPYING.txt','Core/Readme.txt']+['Core/'+x for x in ('arm.c','arm.h','dcsound.c','dcsound.h','yam.c','yam.h','emuconfig.h')]
    originals={name:sha(u/name) for name in names}
    for name in names:
        assert hashlib.sha256(subprocess.check_output([git,'-C',str(u),'show',PIN+':'+name])).hexdigest()==originals[name],name
    adapted=(u/'Core/dcsound.c').read_bytes();changes=[];ram_bytes=a.ram_mib<<20
    replacements=((b'0x800000',b'0x200000',5),
                          (b'{ 0x007FFFFF, ARM_MAP_TYPE_POINTER',b'{ 0x001FFFFF, ARM_MAP_TYPE_POINTER',2),
                          (b'a&0x7FFFFC',b'a&0x1FFFFC',2),
                          (b')&0x7FFFFF',b')&0x1FFFFF',1)) if a.ram_mib==2 else ()
    for old,new,count in replacements:
        assert adapted.count(old)==count,(old,adapted.count(old));adapted=adapted.replace(old,new)
        changes.append(dict(original=old.decode(),replacement=new.decode(),occurrences=count))
    coherence=p/'jp_port_tools/aica_ram_write_coherence.inc';marker=b'static const struct ARM_MEMORY_MAP dcsound_map_load[] = {';assert adapted.count(marker)==1
    adapted=adapted.replace(marker,b'#define JP_AICA_RAM_MASK '+hex(ram_bytes-1).encode()+b'U\n'+coherence.read_bytes()+b'\n'+marker)
    marker=b'static const struct ARM_MEMORY_MAP dcsound_map_store[] = {';head,tail=adapted.split(marker,1);assert tail.count(b'ARM_MAP_TYPE_POINTER , NULL')==1
    adapted=head+marker+tail.replace(b'ARM_MAP_TYPE_POINTER , NULL',b'ARM_MAP_TYPE_CALLBACK, NULL')
    old=b'mapstore[0].type.p = RAMBYTEPTR;';assert adapted.count(old)==1;adapted=adapted.replace(old,b'mapstore[0].type.p = jp_aica_ram_store;')
    generated=d/'external_dcsound_2MiB.c';generated.write_bytes(adapted)
    yam=(u/'Core/yam.c').read_bytes();yam_changes=[];yam_newline=b'\r\n' if b'\r\n' in yam else b'\n'
    # AICA DSP time and RAM writes must advance even when effects have no
    # audible send. The player optimization skipped this state evolution.
    for old,new,count in ((b'if(buf && (state->dsp_emulation_enabled)) {',b'if(state->version == 2 && state->dsp_emulation_enabled) {\n    wantreverb = 1;\n  } else if(buf && (state->dsp_emulation_enabled)) {',1),
                          (b'if(buf) {\n    memset(outbuf, 0, 4*2*samples);',b'if(buf || wantreverb) {\n    memset(outbuf, 0, 4*2*samples);',1),
                          (b'if(a <  0x2048) {\n    if(mask & 0x00FF)',b'if(a <  0x2048) {\n    yam_flush(YAMSTATE);\n    if(mask & 0x00FF)',1)):
        old=old.replace(b'\n',yam_newline);new=new.replace(b'\n',yam_newline)
        assert yam.count(old)==count,(old,yam.count(old));yam=yam.replace(old,new);yam_changes.append(dict(original=old.decode(),replacement=new.decode(),occurrences=count))
    generated_yam=d/'external_yam_timing.c';generated_yam.write_bytes(yam)
    cc=str(a.compiler.resolve());flags=[cc,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-DEMU_COMPILE','-DEMU_LITTLE_ENDIAN','-DWIN32=1','-I'+str(u/'Core')]
    fixture=p/'jp_port_tools/aica_probe_fixture.inc';peer=d/'aica_probe_api.c';peer.write_bytes(fixture.read_bytes());commands=[]
    for name in ('arm','dcsound','yam'):
        commands.append(flags+['-Wall','-Wextra','-c',str(generated if name=='dcsound' else generated_yam if name=='yam' else u/'Core'/f'{name}.c'),'-o',str(d/f'{name}.obj')])
    commands.append(flags+['-Wall','-Wextra','-Werror','-c',str(peer),'-o',str(d/'api.obj')])
    objects=[str(d/(name+'.obj')) for name in ('arm','dcsound','yam','api')]
    commands.append([cc,'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',*objects,*['-Wl,/export:'+x for x in EXPORTS],'-Wl,kernel32.lib','-o',str(d/'aica_core_probe.dll')])
    results=[]
    for j,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{j}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n');assert not run.returncode,(log,log.read_text())
        if j>=3:assert not log.stat().st_size,log.read_text()
    for n,h in originals.items():assert sha(u/n)==h,n
    result=dict(status='pinned_external_ARM_AICA_core_native_probe_linked_unverified',upstream_repository='https://github.com/kode54/Highly_Theoretical',upstream_revision=PIN,upstream_license='GPL-3.0',upstream_source_sha256=originals,commands=results,
        authored_source_sha256={x.relative_to(r).as_posix():sha(x) for x in (fixture,coherence,Path(__file__).resolve())},DLL=str(d/'aica_core_probe.dll'),DLL_sha256=sha(d/'aica_core_probe.dll'),compiler_sha256=sha(a.compiler),
        external_checkout_modified=False,private_generated_core_adaptation=changes,ARM_RAM_stores_flush_pending_AICA=True,ARM_RAM_reads_and_fetch_retain_direct_map=True,
        private_generated_YAM_adaptation=yam_changes,AICA_DSP_runs_when_inaudible=True,effect_routing_changes_flush_pending_audio=True,generated_YAM_source_sha256=sha(generated_yam),generated_source_sha256=sha(generated),core_RAM_bytes=ram_bytes,physical_game_RAM_bytes=2<<20,DSP_dynarec_enabled_on_x64=False,cycle_policy='inherited 128 interpreter cycles per 44100 Hz frame; not qualified as physical ARM cycle timing',
        core_clock_or_hardware_fidelity_qualified=False,retail_assets_embedded=False,original_game_or_SDK_bodies_linked=False,artifact_publishable=False,game_sound_or_parent_integrated=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],'diagnostic bytes',sum(x['diagnostic_bytes'] for x in results),flush=True)

if __name__=='__main__':main()
