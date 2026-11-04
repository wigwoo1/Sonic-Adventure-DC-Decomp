"""Extract/decompress supplied Japanese PVM inputs privately, never publish assets."""
import argparse,hashlib,json,struct,subprocess
from pathlib import Path
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','output']:ap.add_argument('--'+n,type=Path,required=True)
    for n in ['distribution','gdi-tool','prs-tool','disc']:ap.add_argument('--'+n,dest=n.replace('-','_'),required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();assert d.is_relative_to(r/'work/sonic-adventure') and not d.exists();d.mkdir()
    linux='/mnt/'+d.drive[0].lower()+d.as_posix()[2:];paths=['SONICADV/SEGALOGO.PRS','SONICADV/TITLE.PRS','SONICADV/AVA_TITLE.PRS']
    commands=[]
    def run(args):
        command=['wsl.exe','-d',a.distribution,'-u','root','--',*args];result=subprocess.run(command,capture_output=True)
        log=d/('command_'+str(len(commands))+'.txt');log.write_bytes(result.stdout+result.stderr);commands.append(dict(command=command,exit_code=result.returncode,log_sha256=sha(log)))
        assert result.returncode==0,log.read_text()[:3000]
    run([a.gdi_tool,'extract',a.disc,linux,*paths]);assets=[]
    for name in paths:
        source=d/name;target=d/(Path(name).stem+'.PVM');assert not target.exists()
        run([a.prs_tool,'decompress',linux+'/'+name,linux+'/'+target.name])
        raw=target.read_bytes();assert raw[:4]==b'PVMH' and len(raw)>=12
        size,status,count=struct.unpack_from('<IHh',raw,4);assert size+8<=len(raw) and count>=0
        assets.append(dict(disc_path=name,compressed_path=str(source),compressed_bytes=source.stat().st_size,compressed_sha256=sha(source),
            PVM=str(target),PVM_bytes=len(raw),PVM_sha256=sha(target),header_bytes=size+8,entry_flags=status,texture_count=count))
    manifest=r/'work/sonic-adventure/japanese_v1007_disc_input_manifest.json';assert manifest.exists()
    result=dict(status='supplied_Japanese_archives_extracted_and_PRS_decompressed_privately_not_render_qualified',commands=commands,assets=assets,
        original_disc_manifest_sha256=sha(manifest),retail_assets_published=False,original_inputs_modified=False,script_sha256=sha(Path(__file__)),overall_completion_percent=10)
    (d/'input_manifest.json').write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(dict(status=result['status'],assets=[{k:v for k,v in row.items() if k in ['disc_path','PVM_bytes','PVM_sha256','entry_flags','texture_count']} for row in assets]),indent=2))
if __name__=='__main__':main()
