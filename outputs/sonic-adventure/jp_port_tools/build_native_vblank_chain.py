"""Reuse original Shinobi list algorithms on the one native logical clock.
CPU/DMA/hblank registration remains explicitly unsupported. Original source
and its hardware-dependent objects stay private and unchanged.
"""
import argparse,hashlib,json,re,shutil,subprocess
from pathlib import Path
from build_native_texture_runtime import function
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def block(raw,anchor):
    start=raw.index(anchor);at=raw.index(b'{',start);depth=1;end=at+1
    while depth:
        if raw[end]==123:depth+=1
        elif raw[end]==125:depth-=1
        end+=1
    return raw[start:end]
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','baseline','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args()
    for n in vars(a):setattr(a,n,getattr(a,n).resolve())
    r,u,base,c,d=a.workspace,a.upstream,a.baseline,a.compiler,a.output;p=r/'outputs/sonic-adventure';b=read(base/'build_verification.json');fb=read(Path(b['frame_build'])/'build_verification.json')
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    assert sha(c)==b['compiler_sha256'] and all(x['exit_code']==x['diagnostic_bytes']==0 for x in b['commands'])
    assert subprocess.check_output(['git','-C',str(u),'rev-parse','HEAD'],text=True).strip()=='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
    assert not subprocess.check_output(['git','-C',str(u),'status','--porcelain','--untracked-files=no'],text=True).strip()
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['generated_sources_sha256'].items():assert sha(base/n)==h,n
    present_link=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b['present_DLL']));old=next(n for n in present_link if n.endswith('wait_service.obj'))
    kept={n:sha(n) for n in present_link if n.endswith('.obj') and n!=old}
    oldbuild=read(r/'work/sonic-adventure/jp-native-task-presentation-runtime-v6/build_verification.json');compile_old=next(x['command'] for x in oldbuild['commands'] if x['command'][-1]==old)
    frozen=(p/'native/jp_native_frame_wait.c').read_bytes();extension=(p/'native/jp_native_frame_service_extension.c').read_bytes();prior_wait=Path(old).with_name('present_wait_service.c').read_bytes()
    assert prior_wait.replace(b'\r\n',b'\n').strip()==(frozen+b'\n'+extension).replace(b'\r\n',b'\n').strip()
    chain_raw=(u/'sdk/shinobi/syChain.c').read_bytes();system_raw=(u/'sdk/ninja/njSystem.c').read_bytes();globals_raw=(u/'sdk/ninja/njGlobal.c').read_bytes()
    names=['syChainInit','syChainAddHandler','syChainDeleteHandler','syChainMasterHandlerPVR_V'];bodies={n:function(chain_raw,n) for n in names};original_hashes={n:hashlib.sha256(v).hexdigest() for n,v in bodies.items()}
    add=bodies['syChainAddHandler'];start=add.index(b'            if (event <');end=add.index(b'            keyTbl[KEY(event)]',start);platform=add[start:end];add=add[:start]+b'            jp_chain_native_callback(jp_chain_original_vblank, NULL);\n'+add[end:]
    bus=block(add,b'            if (KIND(event) == KIND_NORMAL)');assert add.count(bus)==1;add=add.replace(bus,b'')
    # block() selected the first if arm; remove its paired external else arm.
    external=block(add,b' else if (KIND(event) == KIND_EXTERNAL)');assert add.count(external)==1;add=add.replace(external,b'')
    delete=bodies['syChainDeleteHandler'];hardware=block(delete,b'                if (keyTbl[i] == NULL)');delete=delete.replace(hardware,b'                if (keyTbl[i] == NULL) jp_chain_native_callback(NULL, NULL);')
    bodies['syChainAddHandler']=add;bodies['syChainDeleteHandler']=delete
    adapted=b'\n'.join(bodies.values()).replace(b'get_imask()',b'jp_chain_native_mask_get()').replace(b'set_imask(',b'jp_chain_native_mask_set(')
    sdk_names=['njBlank','njSetEORflag'];sdk_bodies={n:function(system_raw,n) for n in sdk_names}
    declaration=[]
    for name in ['_nj_eor_flag_','_nj_render_flag_']:
        rows=re.findall(rb'^volatile Int '+name.encode()+rb';\n',globals_raw,re.M);assert len(rows)==1;declaration+=rows
    rows=re.findall(rb'^volatile Int _nj_blank_count_ = 0;\n',system_raw,re.M);assert len(rows)==1;declaration+=rows
    d.mkdir();(d/'include').mkdir();header=d/'include/clock_import.h';header.write_bytes((p/'native/jp_native_clock.h').read_bytes().replace(b'__declspec(dllexport)',b'__declspec(dllimport)')+b'\n#undef JP_CLOCK_API\n#define JP_CLOCK_API __declspec(dllexport)\n')
    assert sha(header)==sha(Path(old).parent/'include/clock_import.h')
    sdk=d/'external_vblank_chain.c';sdk.write_bytes(b'#include "jp_native_vblank_chain.h"\n#undef NULL\n#include <sg_chain.h>\n#define ISR_MAX 256\n#define KEY_MAX 192\n#define ISR_USED 0x80000000\n#define KEY(event) ((event) >> 4)\n#define IMASK_ALL 15\nSYS_CHAIN_ISR isrPool[ISR_MAX];\nSYS_CHAIN_ISR *keyTbl[KEY_MAX];\n#define syChainInit jp_chain_original_init\n#define syChainAddHandler jp_chain_original_add\n#define syChainDeleteHandler jp_chain_original_delete\n#define syChainMasterHandlerPVR_V jp_chain_original_vblank\n'+adapted+b'\n#undef syChainInit\n#undef syChainAddHandler\n#undef syChainDeleteHandler\n#undef syChainMasterHandlerPVR_V\nstatic SYCHAIN blank_handle;\n'+b''.join(declaration)+b'\n'.join(sdk_bodies.values())+b'''
uint32_t jp_chain_original_count(void){uint32_t n=0;SYS_CHAIN_ISR *p=keyTbl[KEY(0x900)];while(p){++n;p=p->next;}return n;}
int jp_chain_original_live(uint32_t n){return n>0 && n<ISR_MAX && (isrPool[n].priority&ISR_USED)!=0;}
int jp_chain_original_describe(uint32_t n,struct jp_chain_record *out){if(!jp_chain_original_live(n))return 0;SYS_CHAIN_ISR *p=&isrPool[n];out->priority=p->priority;out->next=p->next?(uint32_t)(p->next-isrPool):0;out->callback=(uintptr_t)p->usrIsr;out->argument=(uintptr_t)p->arg;return 1;}
uint32_t jp_ninja_blank_handle(void){return blank_handle;}
void jp_ninja_blank_bind(uint32_t handle){blank_handle=handle;}
_Static_assert(sizeof(Int)==4 && sizeof(SYCHAIN)==4 && sizeof(void*)==8,"native chain ABI");
''')
    common=compile_old[:compile_old.index('-c')];previous=str(Path(old).parent/'include/clock_import.h');common[common.index(previous)]=str(header);common+=['-fwrapv']
    owner=p/'native/jp_native_vblank_chain.c';commands=[common+['-c',str(owner),'-o',str(d/'vblank_wait_owner.obj')],common+['-Wno-unused-parameter','-c',str(sdk),'-o',str(d/'vblank_chain_sdk.obj')]]
    present_link[present_link.index(old)]=str(d/'vblank_wait_owner.obj');at=present_link.index('-o');present_link[at:at]=[str(d/'vblank_chain_sdk.obj')]+['-Wl,/export:'+n for n in ['syChainInit','syChainAddHandler','syChainDeleteHandler','njBlank','njSetEORflag']]+[v for n in ['_nj_blank_count_','_nj_eor_flag_','_nj_render_flag_'] for v in ['-Xlinker','/export:'+n+',DATA']];present_link[-1]=str(d/'jp_native_present.dll');commands.append(present_link)
    libraries={'present_DLL':d/'jp_native_present.lib'}
    for label,name,imports in [('authored_exit_DLL','jp_native_exit.dll',['present_DLL']),('DLL','jp_native_logo_start.dll',['present_DLL','authored_exit_DLL'])]:
        cmd=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b[label]))
        for n in imports:
            previous=str(Path(b[n]).with_suffix('.lib'));assert cmd.count(previous)==1;cmd[cmd.index(previous)]=str(libraries[n])
        cmd[-1]=str(d/name);libraries[label]=Path(cmd[-1]).with_suffix('.lib');commands.append(cmd)
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/('build_'+str(i)+'.txt');log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)));assert run.returncode==0 and not log.stat().st_size,log.read_text()[:6000]
    for n,h in kept.items():assert sha(n)==h,n
    sources=dict(b['authored_source_sha256'])
    for path in [owner,p/'native/jp_native_vblank_chain.h',Path(__file__).resolve()]:sources[path.relative_to(r).as_posix()]=sha(path)
    generated=dict(b['generated_sources_sha256']);generated[sdk.name]=sha(sdk);generated['include/clock_import.h']=sha(header)
    for n,h in b['generated_sources_sha256'].items():target=d/n;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(base/n,target);assert sha(target)==h
    shutil.copytree(base/'inputs',d/'inputs')
    shared=dict(status='original_vblank_chain_and_single_native_wait_owner_linked_behavior_unverified',commands=results,exit_code=0,diagnostic_bytes=0,authored_source_sha256=sources,generated_sources_sha256=generated,previous_boot_buffers_build_sha256=sha(base/'build_verification.json'),kept_presentation_objects_sha256=kept,replaced_wait_object=dict(path=old,sha256=sha(old)),native_vblank_owner_sha256=sha(d/'vblank_wait_owner.obj'),private_chain_sdk_object_sha256=sha(d/'vblank_chain_sdk.obj'),original_chain_functions_sha256=original_hashes,original_ninja_blank_EOR_functions_sha256={n:hashlib.sha256(v).hexdigest() for n,v in sdk_bodies.items()},chain_source_adaptations=dict(first_event_registration_removed_sha256=hashlib.sha256(platform).hexdigest(),last_event_hardware_removed_sha256=hashlib.sha256(hardware).hexdigest(),same_list_insertion_deletion_and_traversal=True,only_logical_vblank_source_supported=True,physical_interrupt_mask_replaced_by_serialized_host_context=True),chain_external_sources_sha256={n:sha(u/n) for n in ['sdk/shinobi/syChain.c','sdk/include/sg_chain.h','sdk/ninja/njSystem.c','sdk/ninja/njGlobal.c']},frames_DLL_bytes_unchanged=True,GPU_DLL_bytes_unchanged=True,one_existing_clock_and_wait_owner=True,artifact_publishable=False,game_containing_DLL_publishable=False,playable=False,overall_completion_percent=10)
    frame=dict(fb,**shared);frame.update(DLL=str(d/'jp_native_present.dll'),DLL_sha256=sha(d/'jp_native_present.dll'),present_DLL=str(d/'jp_native_present.dll'));fd=d/'frame_view';fd.mkdir()
    for n in generated:target=fd/n;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(d/n,target)
    (fd/'build_verification.json').write_text(json.dumps(frame,indent=2)+'\n')
    result=dict(b,**shared);result.update(DLL=str(d/'jp_native_logo_start.dll'),DLL_sha256=sha(d/'jp_native_logo_start.dll'),present_DLL=frame['DLL'],authored_exit_DLL=str(d/'jp_native_exit.dll'),authored_exit_DLL_sha256=sha(d/'jp_native_exit.dll'),frame_build=str(fd),frame_build_sha256=sha(fd/'build_verification.json'))
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)
if __name__=='__main__':main()
