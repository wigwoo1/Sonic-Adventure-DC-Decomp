"""Reuse original VMU initialization, complete draw module and SDK LCD stack.
Generated SDK/game code and combined DLLs remain private.
"""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
from build_native_texture_runtime import function
from build_native_renderer_object_v3 import coff_symbols
INIT=['screen_probe','screens_probe','vmu_init','vmu_request','vmu_screen_due']
DRAW=['vmu_clear','vmu_blit','vmu_copy','vmu_shift','vmu_pixelate','vmu_unpack','vmu_draw_picture','vmu_draw_chao','vmu_step_anim','vmu_set_anim','vmu_draw_puzzle','vmu_draw_tally','vmu_pack','vmu_pack_turned','vmu_write']
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure'
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure') and not d.is_relative_to(r/'work/sonic-adventure/build')
    b=json.loads((a.baseline/'build_verification.json').read_bytes())
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['kept_frame_objects_sha256'].items():assert sha(n)==h,n
    assert sha(b['frames_DLL'])==b['frames_DLL_sha256'] and sha(b['present_DLL'])==b['present_DLL_sha256']
    files=['src/vmu/screen.c','src/vmu/screen.h','src/vmu/draw.c','src/vmu/draw.h','sdk/shinobi/lcd.c','sdk/shinobi/lcdvms.c','sdk/shinobi/lcdasm.src','sdk/shinobi/kdapi.c','sdk/include/sg_lcd.h','sdk/include/kaede.h']
    originals={n:sha(u/n) for n in files};git='C:/Program Files/Git/cmd/git.exe'
    assert subprocess.check_output([git,'-C',str(u),'rev-parse','HEAD']).decode().strip()=='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
    for n,h in originals.items():assert hashlib.sha256(subprocess.check_output([git,'-C',str(u),'show','HEAD:'+n])).hexdigest()==h,n
    d.mkdir();raw=(u/'src/vmu/screen.c').read_bytes();src=b'#include <sg_lcd.h>\n#include <sg_xpt.h>\n#include <shinobi.h>\n#include <string.h>\n#include "vmu/screen.h"\n#include "vmu/draw.h"\n#include "jp_native_vmu_state.h"\n#define SCREENS 4\n'
    for pattern in [rb'enum screen_mode\s*\{.*?\};',rb'struct vmu_state\s*\{.*?\};',rb'static const Uint32 lcd_ports\[\]\s*=\s*\{.*?\};']:
        found=re.findall(pattern,raw,re.S);assert len(found)==1;src+=found[0]+b'\n'
    src+=b'static struct vmu_state vmu;\n'
    bodies={n:function(raw,n) for n in INIT}
    for n in INIT:src+=bodies[n]+b'\n'
    src+=b'''int jp_vmu_state_read(struct jp_vmu_state_info *out) {
 if(!out)return 1;out->game_mode=vmu.game_mode;out->skip=vmu.skip;
 out->updates=vmu.updates;out->request=vmu.request;
 out->screen_bytes=sizeof(struct vmu_screen);out->animation_frame_bytes=sizeof(struct vmu_frame);return 0;
}
void *jp_vmu_screen(uint32_t port) {return port<SCREENS ? &vmu.screens[port] : NULL;}
'''
    generated=d/'external_vmu_init.c';generated.write_bytes(src)
    sources=[generated]
    for name in ('src/vmu/draw.c','sdk/shinobi/lcd.c','sdk/shinobi/lcdvms.c'):
        f=d/('external_'+Path(name).name);f.write_bytes((u/name).read_bytes());sources.append(f)
    kd=(u/'sdk/shinobi/kdapi.c').read_bytes();f=d/'external_lcd_device_info.c'
    f.write_bytes(b'#include <kaede.h>\n'+function(kd,'kdDevGetFD')+function(kd,'kdDevGetLCDInfo'));sources.append(f)
    common=list(b['commands'][0]['command']);common=common[:common.index('-c')]
    own=p/'native/jp_native_lcd.c';sources.append(own);commands=[];objects=[]
    for source in sources:
        obj=d/(source.stem+'.obj');objects.append(obj)
        commands.append(common+['-Wno-unused-const-variable','-c',str(source),'-o',str(obj)])
    fl=list(b['commands'][1]['command']);assert '-shared' in fl
    fl[fl.index('-o'):fl.index('-o')]=[str(x) for x in objects]
    exports=[n for n in INIT if not n.startswith('screen')]+[n for n in DRAW if not n in ('vmu_blit','vmu_copy','vmu_unpack','vmu_pack','vmu_pack_turned')]+['pdLcdGetDirection','pdVmsLcdIsReady','pdVmsLcdWrite','pdVmsLcdWrite1','pdVmsLcdBlt','kdDevGetFD','kdDevGetLCDInfo']
    fl[fl.index('-o'):fl.index('-o')]=['-Wl,/export:'+n for n in exports];fl[-1]=str(d/'jp_native_frames.dll');commands.append(fl)
    present=(a.baseline/'present_with_card_service.c').read_bytes();old=b'#define buExecServer() (buExecServer(),event(1))';assert present.count(old)==1
    present=present.replace(old,b'#include "jp_native_lcd.h"\n#define buExecServer() (buExecServer(),jp_lcd_service(),event(1))')
    ps=d/'present_with_card_lcd.c';ps.write_bytes(present);pc=list(b['commands'][2]['command']);pc[-3]=str(ps);pc[-1]=str(d/'present_with_card_lcd.obj');commands.append(pc)
    pl=list(b['commands'][3]['command']);oldobj=next(n for n in pl if n.endswith('present_with_card_service.obj'));pl[pl.index(oldobj)]=pc[-1]
    for i,n in enumerate(pl):
        if n.endswith('jp_native_frames.lib'):pl[i]=str(d/'jp_native_frames.lib')
    pl[-1]=str(d/'jp_native_present.dll');commands.append(pl);results=[]
    for i,cmd in enumerate(commands):
        run=subprocess.run(cmd,capture_output=True);log=d/f'build_{i}.txt';log.write_bytes(run.stdout+run.stderr);results.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'compile_attempt.json').write_text(json.dumps(results,indent=2)+'\n')
        assert not run.returncode and not log.stat().st_size,log.read_text()
    for n,h in originals.items():assert sha(u/n)==h,n
    for n,h in b['kept_frame_objects_sha256'].items():assert sha(n)==h,n
    result=dict(status='original_VMU_init_complete_draw_and_SDK_LCD_stack_native_endpoints_linked_unverified',commands=results,frames_DLL=str(d/'jp_native_frames.dll'),frames_DLL_sha256=sha(d/'jp_native_frames.dll'),present_DLL=str(d/'jp_native_present.dll'),present_DLL_sha256=sha(d/'jp_native_present.dll'),authored_source_sha256={str(f.relative_to(r)):sha(f) for f in [own,p/'native/jp_native_lcd.h',p/'native/jp_native_vmu_state.h',Path(__file__).resolve()]},original_source_sha256=originals,original_functions_sha256={**{n:hashlib.sha256(v).hexdigest() for n,v in bodies.items()},**{n:hashlib.sha256(function((u/'src/vmu/draw.c').read_bytes(),n)).hexdigest() for n in DRAW}},generated_sources_sha256={f.name:sha(f) for f in sources[:-1]+[ps]},kept_frame_objects_sha256={n:sha(n) for n in fl if n.endswith('.obj')},baseline_build_sha256=sha(a.baseline/'build_verification.json'),original_VM_screen_function_bodies_unchanged=True,complete_original_draw_module=True,original_SDK_LCD_C_modules_unchanged=True,real_LCD_presence_from_attached_cards=True,actual_frame_loop_commits_LCD_commands=True,VMU_program_CPU_emulator_implemented=False,artifact_publishable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],flush=True)
if __name__=='__main__':main()
