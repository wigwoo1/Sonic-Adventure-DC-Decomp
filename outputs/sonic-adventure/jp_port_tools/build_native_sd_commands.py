"""Extend qualified original SDK with real MLT, host queue and MIDI producers."""
import argparse,hashlib,json,re,shutil,subprocess
from pathlib import Path
from build_native_sd_memory import PIN
from build_native_gdfs import functions

UNITS=('sd_multi_unit_download','sd_bank_download','sd_snd_mem_allocate_bank','sd_snd_mem_get_bank_stat',
    'sd_set_host_cmd','sd_2nd_host_cmd_send_lst','sd_prim_host_cmd_flush','sd_prim_host_cmd_write','sd_sys_flush_host_cmd','sd_sys_server',
    'sd_snd_mem_read_uint8','sd_snd_mem_write_uint8','sd_snd_mem_write_uint32','sd_snd_mem_write_sint32',
    'sd_module_open_port','sd_module_close_port','sd_midi_open_port','sd_midi_set_mes','sd_midi_send_mes','sd_midi_play','sd_midi_stop','sd_midi_set_vol',
    'sd_snd_set_space','sd_snd_set_fx_prg','sd_snd_set_fx_out_prm')
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','compiler','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure'
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir();inc=d/'include';inc.mkdir()
    base=json.loads((a.baseline/'build_verification.json').read_bytes());assert sha(base['DLL'])==base['DLL_sha256']
    original_generated=a.baseline/'external_sd_driver.c';assert sha(original_generated)==base['generated_source_sha256']
    for name,h in base['authored_source_sha256'].items():assert sha(r/name)==h,name
    for name,h in base['original_source_sha256'].items():assert sha(u/name)==h,name
    for name,h in base['dependencies'].items():assert sha(a.baseline/name)==h,name;shutil.copyfile(a.baseline/name,d/name)
    header=(a.baseline/'include/shinobi.h').read_bytes();assert sha(a.baseline/'include/shinobi.h')==base['private_header_sha256']
    old=b'const Uint8 midi_mes, ...';assert header.count(old)==1;header=header.replace(old,b'const int midi_mes, ...');(inc/'shinobi.h').write_bytes(header)
    originals={'sdk/shinobi/'+n+'.c':sha(u/'sdk/shinobi'/f'{n}.c') for n in UNITS};git='C:/Program Files/Git/cmd/git.exe'
    for n,h in originals.items():assert hashlib.sha256(subprocess.check_output([git,'-C',str(u),'show',PIN+':'+n])).hexdigest()==h,n
    raws={n:(u/'sdk/shinobi'/f'{n}.c').read_bytes() for n in UNITS};before={name:hashlib.sha256(body).hexdigest() for raw in raws.values() for name,body in functions(raw).items()};changes=[]
    def replace(unit,old,new,reason,count=1):
        raw=raws[unit];assert raw.count(old)==count,(unit,reason,raw.count(old));raws[unit]=raw.replace(old,new)
        changes.append(dict(unit=unit,reason=reason,count=count,old_sha256=hashlib.sha256(old).hexdigest(),new_sha256=hashlib.sha256(new).hexdigest()))
    replace('sd_midi_set_mes',b'const Uint8 midi_mes, ...',b'const int midi_mes, ...','host C17 va_start requires a fixed argument without default integer promotion')
    replace('sd_midi_set_mes',b'    static struct sd_host_command unused_command;',b'','omit unreferenced matching-only BSS placeholder in native build')
    replace('sd_snd_mem_allocate_bank',b'    struct sd_bank *driver_bank;',b'    uint32_t driver_offset;','driver bank location is a numeric32bit sound offset')
    for name in ('midi_seq','midi_prg','shot','pstm_ring'):
        old=('&((struct sd_banks *)work->data_map)->'+name+'[(Sint8)bank_num]').encode()
        new=('(uint32_t)work->data_map+offsetof(struct sd_banks,'+name+')+(uint32_t)(Sint8)bank_num*sizeof(struct sd_bank)').encode()
        replace('sd_snd_mem_allocate_bank',old,new,'explicit driver bank array offset '+name)
    for name in ('fx_prg','fx_out','fx_wrk'):
        replace('sd_snd_mem_allocate_bank',('&((struct sd_banks *)work->data_map)->'+name).encode(),('(uint32_t)work->data_map+offsetof(struct sd_banks,'+name+')').encode(),'explicit single driver bank offset '+name)
    replace('sd_snd_mem_allocate_bank',b'driver_bank =',b'driver_offset =','keep offset arithmetic separate from native bank pointer',7)
    for field,arg in (('address','allocate_ptr'),('size','allocate_size')):
        old=('    sdSndMemWriteSint32((Sint32)&driver_bank->'+field+', '+arg+');').encode()
        new=('    if (sdSndMemWriteSint32((Sint32)(driver_offset+offsetof(struct sd_bank,'+field+')), '+arg+')!=SDE_ERR_NOTHING) return SDE_ERR_UNKNOW_NUM;').encode()
        replace('sd_snd_mem_allocate_bank',old,new,'write actual bank map before recording successful host allocation')
    for unit in ('sd_snd_mem_write_uint32','sd_snd_mem_write_sint32'):
        replace(unit,b'(void *)(SD_SND_MEM +',b'(void *)(uintptr_t)(SD_SND_MEM +','explicit numeric bus token')
        raw=raws[unit];call=re.search(rb'        syG2Write\([^;]+\);',raw).group()
        replace(unit,call,b'        if ('+call.strip()[:-1]+b' != SYE_G2_ERR_NOTHING) return SDE_ERR_UNKNOW_NUM;','propagate actual bus write failure')
    for unit in ('sd_snd_mem_read_uint8','sd_snd_mem_write_uint8'):
        call=re.search(rb'        sdSndMemReadUint32\([^;]+\);',raws[unit]).group()
        replace(unit,call,b'        if ('+call.strip()[:-1]+b' != SDE_ERR_NOTHING) return SDE_ERR_UNKNOW_NUM;','do not use uninitialized data after failed real read')
    replace('sd_snd_mem_write_uint8',b'        sdSndMemWriteUint32(word_address, data.word);',b'        if (sdSndMemWriteUint32(word_address, data.word)!=SDE_ERR_NOTHING) return SDE_ERR_UNKNOW_NUM;','propagate actual status-byte write failure')
    replace('sd_2nd_host_cmd_send_lst',b'    sdSndMemReadUint8(work->status, &status);',b'    if (sdSndMemReadUint8(work->status,&status)!=SDE_ERR_NOTHING) return SDE_ERR_UNKNOW_NUM;','do not publish host commands after failed actual status read')
    for name in ('sdMemBlkSetPrm','sdMemBlkTransfer'):
        call=re.search(('        '+name+r'\([^;]+\);').encode(),raws['sd_prim_host_cmd_flush']).group()
        replace('sd_prim_host_cmd_flush',call,b'        if ('+call.strip()[:-1]+b' != SDE_ERR_NOTHING) return SDE_ERR_UNKNOW_NUM;','propagate actual primary descriptor/copy failure')
    replace('sd_prim_host_cmd_flush',b'    sdSndMemWriteUint8(sdgWrkPtr->status, LIST_WAITING);',b'    if (sdSndMemWriteUint8(sdgWrkPtr->status,LIST_WAITING)!=SDE_ERR_NOTHING) return;','failed signal retains busy primary list for owner failure handling')
    body=functions(raws['sd_sys_server'])['sdSysServer'];new=body.replace(b'{\n',b'{\n    if (jp_sd_service_completions()) return SDE_ERR_UNKNOW_NUM;\n',1)
    new=new.replace(b'    return SDE_ERR_NOTHING;',b'    if (jp_sd_service_completions()) return SDE_ERR_UNKNOW_NUM;\n    return SDE_ERR_NOTHING;')
    replace('sd_sys_server',body,new,'real creator-thread completion service before/after original server work')
    oldbase=original_generated.read_bytes();base_generated=oldbase
    body=functions(base_generated)['sdSndMemReadUint32'];call=re.search(rb'        syG2Read\([^;]+\);',body).group();changed=body.replace(call,b'        if ('+call.strip()[:-1]+b' != SYE_G2_ERR_NOTHING) return SDE_ERR_UNKNOW_NUM;')
    assert base_generated.count(body)==1;base_generated=base_generated.replace(body,changed)
    after={name:hashlib.sha256(body).hexdigest() for raw in raws.values() for name,body in functions(raw).items()}
    helper=b'''\nstatic int jp_sd_service_completions(void)
{
    uint32_t completed=0;
    return jp_g2_service(64,&completed);
}
'''
    boundary=p/'jp_port_tools/sd_commands_boundary.inc';fixture=p/'jp_port_tools/sd_commands_observation.inc'
    generated=d/'external_sd_commands.c';generated.write_bytes(b'static int jp_sd_service_completions(void);\n'+base_generated+helper+
        b'\n'.join(b'\n#undef SOURCE\n'+raw for raw in raws.values())+b'\n'+boundary.read_bytes()+b'\n'+fixture.read_bytes())
    compiler=str(a.compiler.resolve());flags=[compiler,'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-fno-strict-aliasing','-Wall','-Wextra','-Werror','-Wno-unknown-pragmas','-Wno-unused-parameter','-include',str(p/'native/jp_host_types.h'),'-I'+str(inc),'-I'+str(p/'native'),'-I'+str(u/'sdk/include'),'-I'+str(u/'sdk')]
    exports=sorted(set(base['adapted_function_sha256'])|set(after))
    commands=[flags+['-c',str(generated),'-o',str(d/'sd_commands.obj')],
        [compiler,'--target=x86_64-pc-windows-msvc','-shared','-fuse-ld=lld',str(d/'sd_commands.obj'),str(d/'jp_native_g2_audio.lib'),str(d/'jp_native_aica.lib'),*['-Wl,/export:'+n for n in exports],'-Wl,legacy_stdio_definitions.lib','-o',str(d/'jp_native_sd_commands.dll')]]
    results=[]
    for index,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{index}.txt';log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n')
        assert not run.returncode and not log.stat().st_size,(log,log.read_text())
    for n,h in originals.items():assert sha(u/n)==h,n
    result=dict(status='original_SDK_MLT_banks_host_queue_MIDI_and_server_native_linked_unverified',commands=results,DLL=str(d/'jp_native_sd_commands.dll'),DLL_sha256=sha(d/'jp_native_sd_commands.dll'),
        baseline_build_sha256=sha(a.baseline/'build_verification.json'),baseline_generated_source_sha256=sha(original_generated),
        additional_original_modules=len(UNITS),additional_original_source_sha256=originals,additional_original_function_sha256=before,adapted_function_sha256=after,
        additional_unchanged_function_bodies=sum(before[n]==after[n] for n in before),native_adaptations=changes,
        base_readUint32_error_propagation_adapted=True,private_header_sha256=sha(inc/'shinobi.h'),generated_source_sha256=sha(generated),
        dependencies=base['dependencies'],authored_source_sha256={x.relative_to(r).as_posix():sha(x) for x in (boundary,fixture,Path(__file__).resolve())},
        no_synthetic_bank_copies_wire_command_builders_or_driver_ACKs=True,artifact_publishable=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],len(UNITS),'additional original modules',flush=True)

if __name__=='__main__':main()
