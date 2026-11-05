"""Select actual modules for frozen test metadata; replace no native bodies."""
import argparse,hashlib,json,shutil
from pathlib import Path
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','build']:ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--asset-metadata',type=Path);a=ap.parse_args();r=a.workspace.resolve();d=a.build.resolve();assert d.is_relative_to(r/'work/sonic-adventure');bp=d/'build_verification.json';b=read(bp)
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n in ['frames_DLL','GPU_DLL','DLL']:assert sha(b[n])==b[n+'_sha256']
    for name,key in [('gpu_view','GPU_DLL'),('owner_view','frames_DLL'),('texture_view','frames_DLL')]:
        view=d/name;assert not view.exists();view.mkdir();x=dict(b,DLL=b[key],DLL_sha256=b[key+'_sha256'],fixture_projection_only=True,native_function_bodies_replaced=False,parent_build_report_sha256=sha(bp),view_preparer_sha256=sha(__file__))
        if name=='texture_view':
            alias=view/'jp_native_textures.dll';shutil.copyfile(b[key],alias);assert sha(alias)==b[key+'_sha256'];x.update(status='native_texture_runtime_linked_behavior_unverified',dll_sha256=sha(alias),byte_identical_DLL_filename_alias_for_frozen_harness=True,alias_source=b[key])
        (view/'build_verification.json').write_text(json.dumps(x,indent=2)+'\n')
    if a.asset_metadata:
        old=read(a.asset_metadata)
        for n,h in old['vendored_MIT_source_sha256'].items():assert sha(r/n)==h,n
        for name in ['assets.obj','prskit.obj','game_texture.obj']:
            n,h=next((n,h) for n,h in b['kept_frame_objects_sha256'].items() if Path(n).name==name);assert sha(n)==h==old['production_objects_sha256'][name]
        view=d/'asset_view';assert not view.exists();view.mkdir();x=dict(b,vendored_MIT_source_sha256=old['vendored_MIT_source_sha256'],fixture_projection_only=True,native_function_bodies_replaced=False,parent_build_report_sha256=sha(bp),preserved_asset_build_sha256=sha(a.asset_metadata),view_preparer_sha256=sha(__file__))
        (view/'build_verification.json').write_text(json.dumps(x,indent=2)+'\n')
    print('Prepared verified metadata views; actual native function bodies unchanged.',flush=True)
if __name__=='__main__':main()
