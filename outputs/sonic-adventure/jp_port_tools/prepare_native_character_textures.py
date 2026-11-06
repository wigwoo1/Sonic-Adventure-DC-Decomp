"""Read genuine GCC texture definitions and supplied JP PRS inputs privately."""
import argparse,hashlib,json,subprocess
from pathlib import Path
from verify_native_read_queue import Disc
SOURCES=('src/player/sonic_textures','src/player/tails_textures','src/player/knuckles_textures','src/player/amy_textures','src/player/big_textures','src/player/e102_textures','src/player/big_kaeru_textures','src/player/e102_bonus','src/player/e102_hit','src/player/effects','src/player/eggman_textures','modules/tikal_prog/tikal_textures')
def sha(raw):return hashlib.sha256(raw).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','track','output'):ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--distribution',default='sa-dc-source-review');ap.add_argument('--checkout',default='/root/sa-native-integration');a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');prefix=['wsl.exe','-d',a.distribution,'--cd',a.checkout,'--']
    def run(*args):
        v=subprocess.run(prefix+list(args),capture_output=True);assert not v.returncode and not v.stderr,v.stderr.decode(errors='replace');return v.stdout
    pin='25a3d3154dbe657325e97dbe8c5291086f6af5c0';assert run('git','rev-parse','HEAD').decode().strip()==pin and not run('git','status','--porcelain','--untracked-files=no')
    proof=json.loads((r/'outputs/sonic-adventure/analysis/jp_gcc_baseline_v2_verification.json').read_bytes());compiler='tools/sh-elf/bin/sh-elf-gcc'
    assert run('sha256sum',compiler).decode().split()[0]==next(x['sha256'] for x in proof['toolchain_files'] if x['path']==compiler)
    target=['-ml','-m4-single-only','-std=gnu17','-ffreestanding','-nostdinc','-isystem','tools/sh4zam/include','-iwithprefix','include','-DGCC','-DNDEBUG','-DSHZ_TLS_MODEL=SHZ_TLS_DISABLED']
    includes=['-I'+x for x in ('sdk/include','src','modules','events','build/gen/src','build/gen/modules','build/gen/events','build/gen/sdk')];d.mkdir();records={}
    for name in SOURCES:
        key=Path(name).name;args=[compiler,'-E','-P',*target,*includes,name+'.c'];raw=run(*args);table=run('cat','build/gen/'+name+'/bodies.txt')
        (d/(key+'.i')).write_bytes(raw);(d/(key+'.txt')).write_bytes(table)
        records[key]=dict(source=name+'.c',source_sha256=run('sha256sum',name+'.c').decode().split()[0],preprocessed_sha256=sha(raw),table_sha256=sha(table),command=prefix+args)
    for ext in ('bin','txt'):
        raw=run('cat','build/gen/tikal_prog/content.'+ext);(d/('tikal_prog.'+ext)).write_bytes(raw)
    source=run('cat','src/system/texture.c');(d/'original_texture.c').write_bytes(source)
    disc=Disc(a.track);files=disc.files();prs={};assets=d/'assets';assets.mkdir()
    for name in ('SONIC','SON_EFF','MILES','KNUCKLES','KNU_EFF','AMY','AMY_EFF','BIG','FISHING','BIG_KAERU','E102','E102BEAM','E102EFFECT','E102TIME','E102HIT','EGGMAN','TIKAL','TIKAL_PROG'):
        filename=name+'.PRS';fad,count,isdir=files[filename];assert not isdir and 0<count<8<<20;raw=disc.sectors(fad,(count+2047)//2048)[:count];assert len(raw)==count
        (assets/filename).write_bytes(raw);prs[filename]=dict(FAD=fad,bytes=count,sha256=sha(raw))
    disc.f.close();out=dict(status='private_pinned_original_character_texture_definitions_and_actual_JP_disc_inputs_prepared',pin=pin,files=records,original_texture_source_sha256=sha(source),
        tikal_module={ext:sha((d/('tikal_prog.'+ext)).read_bytes()) for ext in ('bin','txt')},PRS_files=prs,track=str(a.track.resolve()),originals_modified=False,retail_and_generated_data_not_publishable=True)
    (d/'inputs.json').write_text(json.dumps(out,indent=2)+'\n');print('Prepared12 source units and18 actual private PRS files;matching/GCC baselines unchanged.',flush=True)
if __name__=='__main__':main()
