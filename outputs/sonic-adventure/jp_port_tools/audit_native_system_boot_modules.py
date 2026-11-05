"""Check original system-boot dependencies against actual DLL exports.
An export is availability evidence, not proof of full startup behavior.
"""
import argparse,ctypes as C,hashlib,json
from pathlib import Path
from build_native_texture_runtime import function
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser()
    for n in ['workspace','upstream','build','report']:ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure';b=json.loads((a.build/'build_verification.json').read_bytes());oldpath=p/'analysis/jp_native_boot_buffers_system_boot_frontier.json';old=json.loads(oldpath.read_bytes());source=a.upstream/'src/main.c'
    assert sha(source)==old['upstream_main_sha256']==b['main_display_source_sha256'] and hashlib.sha256(function(source.read_bytes(),'system_boot_init')).hexdigest()==old['original_function_sha256']
    modules={key:C.CDLL(b[key]) for key in ['frames_DLL','GPU_DLL','present_DLL','DLL']};names=sorted(set(old['remaining_symbols'])|set(old['provided']));exports={n:[key for key,lib in modules.items() if hasattr(lib,n)] for n in names};assert exports['frame_pacing_init']==['present_DLL']
    result=dict(status='original_system_boot_dependencies_actual_native_module_export_inventory',symbols=exports,not_exported=[n for n,rows in exports.items() if not rows],modules={key:dict(path=b[key],sha256=sha(b[key])) for key in modules},previous_frames_only_inventory_sha256=sha(oldpath),original_main_source_sha256=sha(source),original_system_boot_function_sha256=old['original_function_sha256'],build_report_sha256=sha(a.build/'build_verification.json'),audit_tool_sha256=sha(__file__),frame_pacing_init_already_provided_by_existing_presentation_module=True,higher_level_system_boot_module_required_to_import_frames_and_present=True,full_system_boot_linked=False,full_system_boot_executed=False,playable=False,overall_completion_percent=10)
    assert len(result['not_exported'])==10 and not a.report.exists();a.report.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(dict(status=result['status'],not_exported=result['not_exported']),indent=2))
if __name__=='__main__':main()
