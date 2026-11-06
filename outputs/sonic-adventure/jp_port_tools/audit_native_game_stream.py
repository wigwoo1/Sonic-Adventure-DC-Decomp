"""Audit the actual linked game stream, original data owner and retained bodies."""
import argparse,ctypes as C,hashlib,json,re,struct
from pathlib import Path
from build_native_renderer_object_v3 import coff_symbols
from build_native_texture_runtime import function
def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def read(path):return json.loads(Path(path).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','upstream','build','output'):ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();d=a.output.resolve();b=read(a.build/'build_verification.json')
    assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    for n,h in b['authored_source_sha256'].items():assert sha(r/n)==h,n
    for n,h in b['game_stream_original_source_sha256'].items():assert sha(a.upstream/n)==h,n
    source=a.build/'external_game_stream.c';original=(a.upstream/'src/sound/stream.c').read_bytes();raw=source.read_bytes()
    assert sha(source)==b['game_stream_generated_source_sha256'] and sha(b['game_sound_DLL'])==b['game_sound_DLL_sha256']
    names=sorted(set(x.decode() for x in re.findall(rb'^(?:static )?(?:void|int|Sint32|Bool)\s+(\w+)\(',original,re.M)))
    changed=set(b['game_stream_adapted_functions']);bodies={}
    for n in names:
        before,after=function(original,n),function(raw,n)
        if n not in changed:assert before==after,n
        bodies[n]=dict(original_sha256=hashlib.sha256(before).hexdigest(),native_sha256=hashlib.sha256(after).hexdigest(),adapted=n in changed)
    assert b'static void *music_work = NULL;' in raw and b'static void *voice_work = NULL;' in raw
    assert b'#define SERVER_FREQ 60' in raw and b'jp_game_stream_player_create' in raw
    link=next(x for x in b['commands'] if '-shared' in x['command'] and x['command'][-1]==b['game_sound_DLL'])
    assert link['exit_code']==link['diagnostic_bytes']==0
    objects=[Path(x) for x in link['command'] if x.endswith('.obj')];definitions=[]
    for obj in objects:
        for symbol in coff_symbols(obj):
            if symbol['name']=='adxpd_internal_error' and symbol['section']>0:definitions.append(dict(object=str(obj),sha256=sha(obj)))
    assert len(definitions)==1
    refs=sorted(set(s['name'] for s in coff_symbols(a.build/'external_game_stream.obj') if not s['section'] and s['name']))
    assert 'adxpd_internal_error' in refs and not {'task_init','jp_heap_reset'}&set(refs)
    lib=C.CDLL(b['game_sound_DLL'])
    for n in b['game_stream_exports']:assert hasattr(lib,n),n
    address=lib.peer_game_stream_error_address;address.restype=C.c_void_p;flag=address()
    image=Path(b['game_sound_DLL']).read_bytes();pe=struct.unpack_from('<I',image,0x3c)[0];assert image[pe:pe+4]==b'PE\0\0' and struct.unpack_from('<H',image,pe+24)[0]==0x20b
    image_size=struct.unpack_from('<I',image,pe+24+56)[0];assert lib._handle<=flag<lib._handle+image_size
    prior=Path(b['game_stream_preserved_prior_DLLs']['game_sound_DLL']['path']).parent
    unchanged={}
    for key in ('frames_DLL','present_DLL','System_DLL','GDFS_DLL'):
        assert sha(b[key])==b[key+'_sha256']==sha(prior/Path(b[key]).name)
        unchanged[key]=b[key+'_sha256']
    result=dict(status='complete_original_game_stream_source_native_task_work_and_unique_actual_decoder_data_binding_verified',
        original_functions=len(names),original_exported_entries=len(b['game_stream_exports']),function_bodies=bodies,
        actual_linked_objects=[dict(path=str(x),sha256=sha(x)) for x in objects],actual_unique_ADXPD_error_definition=definitions,
        actual_error_flag_address_in_loaded_sound_module=True,actual_stream_object_references=refs,
        fixed_game_work_initializers_replaced=True,original_game_task_pool_not_reinitialized=True,unchanged_task_frame_System_GDFS_DLLs=unchanged,
        build_verification_sha256=sha(a.build/'build_verification.json'),auditor_sha256=sha(__file__),runtime_proof_required=True,playable=False,overall_completion_percent=10)
    d.mkdir();(d/'verification.json').write_text(json.dumps(result,indent=2)+'\n');print(result['status'],len(names),'original functions',flush=True)
if __name__=='__main__':main()
