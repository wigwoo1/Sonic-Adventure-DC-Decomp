"""Compile complete original pad logic and qualify its real helper boundary.

The complete pad_scan object is audited without recorder stubs. The linked
helper runtime contains the original state/functions except pad_scan, pending
its genuine recording_update dependency. Original sources/proofs are retained.
"""
import argparse,hashlib,json,re,struct,subprocess
from pathlib import Path
from build_native_texture_runtime import function
from build_native_renderer_object_v3 import coff_symbols
from rewrite_sprite_float_operations import rewrite,span
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())


def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','baseline','input','binary','compiler','output']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args()
    for n in vars(a):setattr(a,n,getattr(a,n).resolve())
    r=a.workspace;u=a.upstream;p=r/'outputs/sonic-adventure';d=a.output
    assert d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build') and not d.exists()
    b=read(a.baseline/'build_verification.json');ib=read(a.input/'build_verification.json');pin=read(p/'jp_port_tools/jp_camera_external_inputs.json')
    assert b['exit_code']==b['diagnostic_bytes']==ib['exit_code']==ib['diagnostic_bytes']==0 and sha(a.compiler)==b['compiler_sha256']
    for old in [b,ib]:
        for n,h in old['authored_source_sha256'].items():assert sha(r/n)==h,n
    assert sha(a.binary)==pin['original_binary_sha256'] and sha(a.input/'input.obj')==ib['object_sha256']
    assert subprocess.check_output(['git','-C',str(u),'rev-parse','HEAD'],text=True).strip()=='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
    assert not subprocess.check_output(['git','-C',str(u),'status','--porcelain','--untracked-files=no'],text=True).strip()
    binary=a.binary.read_bytes();pools={0x8c054448:0x292bf9a8,0x8c054454:0x4622f986}
    for address,word in pools.items():assert struct.unpack_from('<I',binary,address-0x8c010000)[0]==word
    raw=(u/'src/system/pad.c').read_bytes();original_sha=hashlib.sha256(raw).hexdigest();changes=[]
    def replace(old,new,why):
        nonlocal raw
        assert raw.count(old)==1,(why,raw.count(old));raw=raw.replace(old,new);changes.append(why)
    replace(b'#ifdef GCC',b'#if 1','retain original GCC two-player bounds repair')
    for axis in ['x1','y1']:replace(('per_info[i]->'+axis+' << 8').encode(),('per_info[i]->'+axis+' * 256').encode(),'defined native scaling for signed '+axis)
    assert raw.count(b'atan2f(')==2;raw=raw.replace(b'atan2f(',b'jp_pad_atan2(')
    assert raw.count(b'ANGLE_PER_RADIAN')==2;raw=raw.replace(b'ANGLE_PER_RADIAN',b'0x1.45f30cp+13F')
    replace(b'njSqrt(squared)',b'jp_primitive_sqrt(squared)','existing exact RZ/DN1 square-root provider')
    replace(b'(1.0F / (PAD_STICK_REACH * PAD_STICK_REACH * PAD_STICK_REACH))',b'0x1.57f350p-45F','original literal-pool reciprocal 0x292bf9a8')
    prefix=b'#include "jp_native_primitive_numeric.h"\nfloat jp_pad_atan2(float,float);\n'
    raw=prefix+raw;d.mkdir();(d/'frames').mkdir();initial=d/'external_pad_unrewritten.c';initial.write_bytes(raw)
    common=[str(a.compiler),'--target=x86_64-pc-windows-msvc','-std=c17','-O2','-ffp-contract=off','-ffreestanding','-fno-builtin','-fno-strict-aliasing','-fwrapv','-Wall','-Wextra','-Werror','-Wno-unused-parameter','-Wno-unknown-pragmas','-Wno-ignored-qualifiers','-include',str(p/'native/jp_host_types.h'),'-I'+str(p/'native'),*['-I'+str(u/n) for n in ['sdk/include','src','modules','events','.']]]
    ast_command=common+['-Xclang','-ast-dump=json','-fsyntax-only',str(initial)];run=subprocess.run(ast_command,capture_output=True);(d/'ast_diagnostics.txt').write_bytes(run.stderr)
    assert run.returncode==0 and not run.stderr,run.stderr.decode(errors='replace')[:6000]
    tree=json.loads(run.stdout);names=[]
    for n in tree['inner']:
        if n.get('kind')=='FunctionDecl' and not n.get('loc',{}).get('includedFrom') and any(c.get('kind')=='CompoundStmt' for c in n.get('inner',[])):names.append(n['name'])
    assert len(names)==11 and 'pad_scan' in names,names
    adapted,arithmetic=rewrite(raw,tree,names);full=d/'external_pad_full.c';full.write_bytes(adapted)
    first=function(adapted,'pad_enable');helper_names=[n for n in names if n!='pad_scan']
    helpers=d/'external_pad_helpers.c';helpers.write_bytes(adapted[:adapted.index(first)]+b'\n'.join(function(adapted,n) for n in helper_names)+b'''
__declspec(dllexport) void jp_pad_bind_test_ports(void)
{for(int i=0;i<4;i++)per_info[i]=(PDS_PERIPHERAL *)njGetPeripheral((Uint32)i*6);}
__declspec(dllexport) void jp_pad_test_sticks(void){pad_read_sticks();}
__declspec(dllexport) void jp_pad_test_buttons(int slot){if(slot>=0&&slot<4)pad_read_buttons(slot);}
__declspec(dllexport) void jp_pad_test_quit(void){pad_count_quit();}
__declspec(dllexport) const PDS_PERIPHERAL *jp_pad_test_none(void){return &pad_none;}
__declspec(dllexport) void jp_pad_test_reset(void)
{
    pad_enable();pad_enable_player(255);quit_pressed=0;quit_held_scans=0;
    for(int i=0;i<4;i++){pad_repeat_scans[i]=0;pad_held[i]=pad_press_repeat[i]=pad_held_live[i]=pad_press_live[i]=0;}
}
''')
    # Reuse exact, already-qualified authored atan primitives, without another
    # camera errno/matrix owner. The outer two-argument rule is original _in_atan2s.
    camera=p/'native/jp_ninja_camera_math_rz.c';camera_build=read(r/'work/sonic-adventure/jp-native-camera-v1/build_verification.json')
    assert sha(camera)==camera_build['authored_source_sha256'][camera.relative_to(r).as_posix()]
    camraw=camera.read_bytes();constants=b'\n'.join(line for line in camraw.splitlines() if line.startswith(b'#define '))
    math=d/'authored_pad_atan2.c';math.write_bytes(b'#include "jp_ninja_scalar32_rz.h"\n#include "jp_ninja_fmac_rz.h"\n#include <string.h>\n'+constants+b'\n'+b'\n'.join(function(camraw,n) for n in ['scale_power','small_atan','atan_value'])+b'''
__declspec(dllexport) float jp_pad_atan2(float y,float x)
{
    uint32_t a,b,out;memcpy(&a,&y,4);memcpy(&b,&x,4);
    out=atan_value(jp_rz_scalar32_divide(a,b));
    if(b&0x80000000u)out=jp_rz_scalar32_add(out,(a&0x80000000u)?0xc0490fdbu:0x40490fdbu);
    float value;memcpy(&value,&out,4);return value;
}
''')
    commands=[common+['-c',str(src),'-o',str(d/(n+'.obj'))] for n,src in [('pad_full',full),('pad_helpers',helpers),('pad_atan2',math)]]
    commands.append(common+['-S','-emit-llvm',str(full),'-o',str(d/'pad_full.ll')])
    link=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b['frames_DLL']))
    add=[str(a.input/'input.obj'),str(d/'pad_helpers.obj'),str(d/'pad_atan2.obj')]
    exports=['pad_enable','pad_disable','pad_is_enabled','pad_enable_player','pad_disable_player','pad_player_is_enabled','pad_quit_pressed','njGetPeripheral','pdGetPeripheral','pdGetPeripheralInfo','pdExecPeripheralServer','pdExitPeripheral']
    data=['pad_sticks','pad_sticks_read','pad_held','pad_press_repeat','pad_held_live','pad_press_live','pad_records','per_info']
    at=link.index('-o');link[at:at]=add+['-Wl,/export:'+n for n in exports]+[v for n in data for v in ['-Xlinker','/export:'+n+',DATA']]+['-Wl,user32.lib'];link[-1]=str(d/'frames/jp_native_frames.dll');commands.append(link)
    presenter=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b['DLL']))
    oldlib=str(a.baseline/'frames/jp_native_frames.lib');assert presenter.count(oldlib)==1
    presenter[presenter.index(oldlib)]=str(d/'frames/jp_native_frames.lib');presenter[-1]=str(d/'jp_native_present.dll');commands.append(presenter)
    results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,cwd=r,capture_output=True);log=d/('build_'+str(i)+'.txt');log.write_bytes(run.stdout+run.stderr)
        results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size,log_sha256=sha(log)));assert run.returncode==0 and not log.stat().st_size,log.read_text()[:7000]
    residual=re.findall(r'\b(?:fadd|fsub|fmul|fdiv|fcmp)\b',(d/'pad_full.ll').read_text());assert not residual,residual
    # Genuine full pad link: a deliberate dependency audit, never a forced link.
    audit=[str(d/'pad_full.obj') if x==str(d/'pad_helpers.obj') else x for x in link]
    audit[-1]=str(d/'unlinked_full_pad.dll');run=subprocess.run(audit,capture_output=True);auditlog=d/'full_pad_link.txt';auditlog.write_bytes(run.stdout+run.stderr)
    missing=sorted(set(re.findall(r'undefined symbol: ([^\r\n]+)',run.stderr.decode())))
    assert run.returncode!=0 and missing==['recording_update'],auditlog.read_text()[:6000]
    result=dict(b,status='original_pad_helpers_RZ_native_input_and_shared_game_state_linked_full_scan_needs_recorder',commands=results,exit_code=0,diagnostic_bytes=0,frames_DLL=str(d/'frames/jp_native_frames.dll'),frames_DLL_sha256=sha(d/'frames/jp_native_frames.dll'),DLL=str(d/'jp_native_present.dll'),DLL_sha256=sha(d/'jp_native_present.dll'),baseline_build_sha256=sha(a.baseline/'build_verification.json'),input_build_sha256=sha(a.input/'build_verification.json'),AST_command=ast_command,original_pad_source_sha256=original_sha,original_binary_sha256=sha(a.binary),original_literal_pool_words={hex(k):hex(v) for k,v in pools.items()},source_adaptations=changes,arithmetic_adaptation=arithmetic,residual_native_FP_arithmetic=0,complete_original_pad_source_compiled=True,linked_original_helper_names=helper_names,original_pad_scan_linked=False,full_pad_link_audit=dict(command=audit,exit_code=run.returncode,remaining_imports=missing,log_sha256=sha(auditlog),forced_or_dummy_services=False),input_object_sha256=sha(a.input/'input.obj'),one_existing_camera_matrix_heap_clock_owner=True,frame_wait_input_trace_replaced=False,artifact_publishable=False,playable=False,overall_completion_percent=10)
    result['authored_source_sha256']=dict(b['authored_source_sha256'],**ib['authored_source_sha256'])
    result['authored_source_sha256'][Path(__file__).resolve().relative_to(r).as_posix()]=sha(Path(__file__))
    result['generated_sources_sha256']={x.name:sha(x) for x in [full,helpers,math]};result['reused_float_rewriter_sha256']=sha(p/'jp_port_tools/rewrite_sprite_float_operations.py')
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],arithmetic['total_operators_rewritten'],'operators; full scan remaining:',missing)


if __name__=='__main__':main()
