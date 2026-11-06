"""Original ADX header/block/expander modules with actual native numerical services."""
import argparse,hashlib,json,re,shutil,struct,subprocess
from pathlib import Path
from build_native_texture_runtime import function
PIN='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','baseline','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();u=a.upstream.resolve();base=a.baseline.resolve();d=a.output.resolve();p=r/'outputs/sonic-adventure'
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir();b=read(base/'build_verification.json')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for src in base.iterdir():
        if src.is_file() and src.suffix in ('.c','.h','.obj','.dll','.lib'):shutil.copyfile(src,d/src.name)
    for n in ('include','inputs','sound_imports'):shutil.copytree(base/n,d/n)
    for n,h in b['generated_sources_sha256'].items():
        src=base/n;dst=d/n;assert src.resolve().is_relative_to(base) and dst.resolve().is_relative_to(d) and sha(src)==h,n
        dst.parent.mkdir(parents=True,exist_ok=True)
        if dst.exists():assert sha(dst)==h,n
        else:shutil.copyfile(src,dst)
    names=['sdk/sofdec/'+n+'.c' for n in ('adx_dcd','adx_dcd2','adx_dcda','adx_qtbl','adx_xpnd')]+['sdk/runtime/'+n+'.c' for n in ('_a_coss','_a_sins','_in_coss','_i_fmds','cos')]
    originals={n:sha(u/n) for n in names};git='C:/Program Files/Git/cmd/git.exe'
    for n,h in originals.items():assert hashlib.sha256(subprocess.check_output([git,'-C',str(u),'show',PIN+':'+n])).hexdigest()==h,n
    constants=[]
    for file,name in (('_a_coss','adx_cos_coeff'),('_a_sins','adx_sin_coeff')):
        raw=(u/f'sdk/runtime/{file}.c').read_text();values=re.search(r'coefficients\[7\] = \{(.*?)\};',raw,re.S).group(1)
        values=[float(s.strip().rstrip('Ff')) for s in values.split(',') if s.strip()];assert len(values)==7
        words=[struct.unpack('<I',struct.pack('<f',x))[0] for x in values]
        constants.append('static const uint32_t '+name+'[7]={'+','.join(hex(x)+'u' for x in words)+'};\n')
    (d/'adx_runtime_constants.h').write_text(''.join(constants))
    raw=(u/'sdk/sofdec/adx_dcd.c').read_bytes();header=raw[:raw.index(b'void ADX_GetCoefficient(')]
    header=re.sub(rb'#define ADX_SWAP16\(x\)[^\n]*',b'#define ADX_SWAP16(x) (((((uint32_t)(x))&0xffu)<<8)|((((uint32_t)(x))>>8)&0xffu))',header,count=1)
    header=re.sub(rb'#define ADX_SWAP32\(x\).*?\n\n',b'#define ADX_SWAP32(x) jp_adx_bswap32((uint32_t)(x))\n\n',header,count=1,flags=re.S)
    pieces=[b'#include "jp_native_adx_numeric.h"\n'+header]+[function(raw,n) for n in ('ADX_DecodeInfo','ADX_DecodeInfoExADPCM2','ADX_DecodeInfoExLoop')]
    integer=(u/'sdk/sofdec/adx_dcd2.c').read_bytes()
    integer=integer.replace(b'(scale << 8)',b'jp_adx_shift(scale,8)').replace(b'(*(Sint16 *)in << 8)',b'jp_adx_shift(*(Sint16 *)in,8)')
    stereo=(u/'sdk/sofdec/adx_dcda.c').read_bytes()
    for name,shift in [('delayl[0]',16),('delayl[1]',16),('delayr[0]',16),('delayr[1]',16),('k0',4),('k1',4)]:
        old=('('+name+' << '+str(shift)+')').encode();assert stereo.count(old)==1;stereo=stereo.replace(old,('jp_adx_shift('+name+','+str(shift)+')').encode())
    for side in ('l','r'):
        old=f'x = (c0 * newer_{side}) + (c1 * older_{side}) + (gain * step);'.encode();assert stereo.count(old)==2
        stereo=stereo.replace(old,f'x = jp_adx_madd(gain,step,jp_adx_madd(c0,newer_{side},jp_adx_multiply(c1,older_{side})));'.encode())
    stereo=stereo.replace(b'sample = (int)x;',b'sample = jp_adx_truncate(x);').replace(b'sample >> 16',b'jp_adx_asr(sample,16)')
    for name in ('newer_l','older_l','newer_r','older_r'):stereo=stereo.replace(('(int)'+name+' >> 16').encode(),('jp_adx_asr(jp_adx_truncate('+name+'),16)').encode())
    pieces += [integer,stereo,(u/'sdk/sofdec/adx_qtbl.c').read_bytes(),(u/'sdk/sofdec/adx_xpnd.c').read_bytes(),(p/'jp_port_tools/adx_observation.inc').read_bytes()]
    generated=d/'external_adx.c';generated.write_bytes(b'\n'.join(pieces))
    preserved={};updates={}
    for key,value in b.items():
        if (key=='DLL' or key.endswith('_DLL')) and isinstance(value,str) and Path(value).suffix=='.dll':
            assert sha(value)==b[key+'_sha256'];preserved[key]=dict(path=value,sha256=sha(value));updates[key]=str(d/Path(value).name);updates[key+'_sha256']=sha(updates[key])
    def row(path):return list(next(x['command'] for x in b['commands'] if '-c' in x['command'] and Path(x['command'][x['command'].index('-c')+1])==path))
    prior=[row(base/'external_game_sound.c'),row(base/'sound_owner_qsound.c'),row(p/'native/jp_native_sound_spatial.c'),row(p/'native/jp_native_sound_math.c')]
    prior=[[x.replace(str(base),str(d)) for x in cmd] for cmd in prior]
    sdk=prior[0];common=sdk[:sdk.index('-c')]+['-I'+str(u/'sdk'),'-I'+str(d),'-fwrapv','-Wno-unused-function']
    new=[]
    for source,obj in ((generated,'adx'),(p/'native/jp_native_adx_numeric.c','adx_numeric'),(p/'source/sh4_float_i32.c','adx_float'),(p/'source/sh4_truncate_i32.c','adx_truncate')):
        new.append(common+['-c',str(source),'-o',str(d/(obj+'.obj'))])
    link=list(next(x['command'] for x in b['commands'] if '-shared' in x['command'] and Path(x['command'][-1])==Path(b['game_sound_DLL'])))
    link=[x.replace(str(base),str(d)) for x in link];link[4:4]=[str(d/(n+'.obj')) for n in ('adx','adx_numeric','adx_float','adx_truncate')]
    exports=re.findall(rb'^(?:void|ADXPD|Sint32) (ADX(?:PD|_Decode)[A-Za-z0-9_]*)\(',generated.read_bytes(),re.M)
    link[4:4]=['-Wl,/export:'+x.decode() for x in exports]
    commands=[]
    for i,cmd in enumerate(prior+new+[link]):
        run=subprocess.run(cmd,capture_output=True);log=d/f'adx_build_{i}.txt';log.write_bytes(run.stdout+run.stderr)
        commands.append(dict(command=cmd,exit_code=run.returncode,diagnostic_bytes=log.stat().st_size));(d/'adx_compile_attempt.json').write_text(json.dumps(commands,indent=2)+'\n');assert not run.returncode and not log.stat().st_size,(log,log.read_text())
    for key in ('game_sound_DLL','sound_SDK_DLL'):updates[key+'_sha256']=sha(updates[key])
    authored=[p/'native/jp_native_adx_numeric.c',p/'native/jp_native_adx_numeric.h',p/'source/sh4_float_i32.c',p/'source/sh4_float_i32.h',p/'source/sh4_truncate_i32.c',p/'jp_port_tools/adx_observation.inc',Path(__file__).resolve()]
    updates.update(status='original_ADX_header_mono_stereo_expander_and_native_coefficient_path_linked_unverified',commands=commands+b['commands'],
        authored_source_sha256=dict(b['authored_source_sha256'],**{x.relative_to(r).as_posix():sha(x) for x in authored}),
        ADX_baseline_sha256=sha(base/'build_verification.json'),ADX_preserved_prior_DLLs=preserved,ADX_original_source_sha256=originals,
        ADX_generated_source_sha256=sha(generated),ADX_constants_sha256=sha(d/'adx_runtime_constants.h'),ADX_exports=[x.decode() for x in exports],
        ADX_policy='original RZ_DN1 values and original FMAC ordering; integer overflow wraps explicitly',
        music_voice_streaming_or_full_gameplay_verified=False,public_artifact_qualified=False,playable=False,overall_completion_percent=10)
    (d/'build_verification.json').write_text(json.dumps(dict(b,**updates),indent=2)+'\n');print(updates['status'],len(exports),'SDK exports',flush=True)
if __name__=='__main__':main()
