"""Read preserved WSL GCC data without modifying either source/build baseline."""
import argparse,hashlib,json,subprocess
from pathlib import Path
from build_native_motion_data import BANKS

def sha(raw):return hashlib.sha256(raw).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','output'):ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--distribution',default='sa-dc-source-review');ap.add_argument('--checkout',default='/root/sa-native-integration')
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    prefix=['wsl.exe','-d',a.distribution,'--cd',a.checkout,'--']
    def run(*args):
        p=subprocess.run(prefix+list(args),capture_output=True);assert not p.returncode and not p.stderr,p.stderr.decode(errors='replace');return p.stdout
    pin='25a3d3154dbe657325e97dbe8c5291086f6af5c0'
    assert run('git','rev-parse','HEAD').decode().strip()==pin and not run('git','status','--porcelain','--untracked-files=no')
    qualification=json.loads((r/'outputs/sonic-adventure/analysis/jp_gcc_baseline_v2_verification.json').read_bytes())
    compiler='tools/sh-elf/bin/sh-elf-gcc';expected=next(row['sha256'] for row in qualification['toolchain_files'] if row['path']==compiler)
    assert run('sha256sum',compiler).decode().split()[0]==expected
    d.mkdir();rows={}
    for bank in BANKS+('1st_read',):
        out=d/bank;out.mkdir()
        for ext in ('c','txt','bin'):
            source=a.checkout+'/build/gen/'+bank+'/content.'+ext;raw=run('cat',source);assert sha(raw)==run('sha256sum',source).decode().split()[0]
            (out/('content.'+ext)).write_bytes(raw);rows[bank+'/'+ext]=dict(source=source,bytes=len(raw),sha256=sha(raw))
    (d/'inputs.json').write_text(json.dumps(dict(source='read_only_verified_GCC_build',upstream=pin,files=rows),indent=2)+'\n')
    extra=d/'model_sources-v3';extra.mkdir();records={}
    target=['-ml','-m4-single-only','-std=gnu17','-ffreestanding','-nostdinc','-isystem','tools/sh4zam/include','-iwithprefix','include','-DGCC','-DNDEBUG','-DSHZ_TLS_MODEL=SHZ_TLS_DISABLED']
    includes=['-I'+n for n in ('sdk/include','src','modules','events','build/gen/src','build/gen/modules','build/gen/events','build/gen/sdk')]
    for name in ('amy_textures','big_textures','e102_textures'):
        args=[compiler,'-E','-P',*target,*includes,'src/player/'+name+'.c'];raw=run(*args);(extra/(name+'.i')).write_bytes(raw)
        body=run('cat',a.checkout+'/build/gen/src/player/'+name+'/bodies.txt');(extra/(name+'.txt')).write_bytes(body)
        records[name]=dict(preprocessor_command=prefix+args,preprocessed_sha256=sha(raw),body_table_sha256=sha(body))
    (extra/'inputs.json').write_text(json.dumps(records,indent=2)+'\n')
    print('Read-only copied 24 generated files and preprocessed three model sources; original matching/GCC baselines untouched.',flush=True)

if __name__=='__main__':main()
