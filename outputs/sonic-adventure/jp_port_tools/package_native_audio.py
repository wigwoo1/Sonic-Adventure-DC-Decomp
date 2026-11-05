"""Package metadata and explicitly authored, qualified audio support artifacts."""
import argparse
import hashlib
import json
import re
import shutil
import subprocess
from pathlib import Path

def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def read(path):return json.loads(Path(path).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','build','verification','audit','preserved_graph','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';d=a.output.resolve()
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure');d.mkdir()
    build_file=a.build/'build_verification.json';check_file=a.verification/'verification.json';audit_file=a.audit/'dependency_inventory.json';graph_file=a.preserved_graph/'build_verification.json'
    b=read(build_file);v=read(check_file);audit=read(audit_file);g=read(graph_file)
    assert sha(build_file)==v['build_verification_sha256'] and sha(b['DLL'])==b['DLL_sha256']==v['DLL_sha256']
    assert v['checks']>0 and len(v['cases'])==7 and not v['actual_native_audio_services_replaced'] and not v['game_driver_MIDI_music_effects_or_parent_integrated']
    assert all(not row['exit_code'] and not row['diagnostic_bytes'] for row in b['commands'])
    sources=b['authored_source_sha256'].copy()
    for path,h in sources.items():assert sha(r/path)==h,path
    assert sha(a.build/'audio.obj')==b['object_sha256']
    assert sha(p/'jp_port_tools/verify_native_audio.py')==v['verifier_sha256']
    assert sha(p/'jp_port_tools/audit_native_sound_init.py')==audit['auditor_sha256']
    assert not audit['driver_emulated'] and audit['original_inputs_modified'] is False
    link=[x['command'] for x in b['commands'] if '-shared' in x['command']];assert len(link)==1
    objects=[x for x in link[0] if Path(x).suffix=='.obj'];assert objects==[str(a.build.resolve()/'audio.obj')]
    assert not any(not x.startswith('-') and Path(x).suffix.lower()=='.lib' for x in link[0])
    assert [x for x in link[0] if x.startswith('-Wl,')]==['-Wl,kernel32.lib,ole32.lib']
    readobj=Path(b['commands'][0]['command'][0]).parent/'llvm-readobj.exe'
    pe=subprocess.check_output([str(readobj),'--coff-exports','--coff-imports',b['DLL']]);(d/'PE_imports_exports.txt').write_bytes(pe)
    exports=re.findall(rb'Export \{[^}]*?Name: (\S+)',pe,re.S);imports=re.findall(rb'Import \{\s*Name: (\S+)',pe)
    expected={('jp_audio_'+n).encode() for n in ('open','submit','drain','discard_queued','close','status','snapshot')}
    assert set(exports)==expected and {x.lower() for x in imports}=={b'kernel32.dll',b'ole32.dll'}
    preserved={}
    for key,value in g.items():
        if isinstance(value,str) and (key=='DLL' or key.endswith('_DLL')) and value.lower().endswith('.dll'):
            assert sha(value)==g[key+'_sha256'],key;preserved[key]=dict(path=value,sha256=g[key+'_sha256'])
    assert preserved
    proof_files=[build_file,check_file,audit_file,graph_file]
    proofs={path.resolve().relative_to(r).as_posix():sha(path) for path in proof_files}
    for path in (p/'jp_port_tools/verify_native_audio.py',p/'jp_port_tools/audit_native_sound_init.py',Path(__file__).resolve(),p/'JP_NATIVE_AUDIO.txt'):
        sources[path.relative_to(r).as_posix()]=sha(path)
    public={}
    for path in (Path(b['DLL']),Path(b['DLL']).with_suffix('.lib')):
        raw=path.read_bytes();assert raw.startswith(b'MZ') if path.suffix=='.dll' else raw.startswith(b'!<arch>')
        public[str(path)]=dict(bytes=len(raw),sha256=sha(path))
    package=dict(status='qualified_authored_muted_WASAPI_PCM_transport_source_matched',public_artifacts=public,authored_source_sha256=sources,verification_files_sha256=proofs,
        artifact_link_command=link[0],one_authored_object_sha256=b['object_sha256'],exports=[x.decode() for x in exports],system_imports=[x.decode() for x in imports],
        PE_evidence_sha256=sha(d/'PE_imports_exports.txt'),preserved_original_native_graph=preserved,
        public_artifacts_contain_original_game_or_Sega_SDK_bodies=False,public_artifacts_contain_retail_assets=False,
        third_party_emulator_core_merged=False,artifact_publishable=True,playable=False,game_sound_or_parent_integrated=False,overall_completion_percent=10)
    (d/'public_package.json').write_text(json.dumps(package,indent=2)+'\n')
    for source,name in ((build_file,'jp_native_audio_build.json'),(check_file,'jp_native_audio_output.json'),(audit_file,'jp_native_sound_init_audit.json'),(d/'public_package.json','jp_native_audio_public_package.json')):
        target=p/'analysis'/name;assert not target.exists();shutil.copyfile(source,target);assert sha(source)==sha(target)
    (d/'qualification.json').write_text(json.dumps(dict(status=package['status'],current_source_files=len(sources),qualified_public_artifacts=len(public),preserved_native_DLLs=len(preserved),proof_files_sha256=proofs,public_package_sha256=sha(d/'public_package.json'),packager_sha256=sha(__file__),overall_completion_percent=10),indent=2)+'\n')
    print(package['status'],'public artifacts',len(public),'preserved DLLs',len(preserved),flush=True)

if __name__=='__main__':main()
