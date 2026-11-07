"""Freeze scoped event/task/GPU evidence and unchanged-provider link audit."""
import argparse
import ast
import hashlib
import json
import re
import shlex
import shutil
import subprocess
from pathlib import Path
from PIL import Image
from package_native_sound_placement import prior
import verify_native_event_queue_frames as suite

def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def read(p):return json.loads(Path(p).read_bytes())

def main():
    ap=argparse.ArgumentParser()
    for n in ('workspace','baseline','build','frames','queue','init','output'):
        ap.add_argument('--'+n,type=Path,required=True)
    a=ap.parse_args();r=a.workspace.resolve();p=r/'outputs/sonic-adventure'
    d=a.output.resolve();assert not d.exists() and d.is_relative_to(r/'work/sonic-adventure')
    previous=p/'analysis/jp_native_event_queue_package.json'
    seen=set();prior(r,previous,seen);assert len(seen)==40
    old=read(a.baseline/'build_verification.json');b=read(a.build/'build_verification.json')
    bh=sha(a.build/'build_verification.json')
    assert b['event_cleanup_baseline_sha256']==sha(a.baseline/'build_verification.json')==read(previous)['build_verification_sha256']
    for meta in (old,b):
        for n,h in meta['authored_source_sha256'].items():assert sha(r/n)==h
        for k,v in meta.items():
            if (k=='DLL' or k.endswith('_DLL')) and isinstance(v,str) and v.endswith('.dll'):
                assert sha(v)==meta[k+'_sha256']
                if k!='frames_DLL':assert b[k+'_sha256']==old[k+'_sha256']
    def link(meta):
        return next(x['command'] for x in meta['commands']
                    if '-shared' in x['command'] and Path(x['command'][-1]).name=='jp_native_frames.dll')
    before,after=link(old),link(b)
    expected=list(before);expected.insert(expected.index('-o'),'-Wl,/export:event_free');expected[-1]=after[-1]
    assert after==expected and b['commands'][0]['exit_code']==0
    assert shlex.split((a.build/'link.rsp').read_text())==after[1:]
    assert sha(a.build/'link.rsp')==b['commands'][0]['response_file_sha256']
    assert {n:sha(n) for n in before if n.endswith('.obj')}==b['event_cleanup_objects_sha256']
    reader=Path(after[0]).with_name('llvm-readobj.exe')
    def pe(flag,path):
        x=subprocess.run([str(reader),flag,str(path)],capture_output=True)
        assert x.returncode==0 and not x.stderr
        return set(re.findall(r'^  Name: (.+)$',x.stdout.decode(),re.M))
    assert pe('--coff-exports',b['frames_DLL'])==pe('--coff-exports',old['frames_DLL'])|{'event_free'}
    assert pe('--coff-imports',b['frames_DLL'])==pe('--coff-imports',old['frames_DLL'])
    f=read(a.frames/'verification.json');q=read(a.queue/'verification.json');init=read(a.init/'verification.json')
    for row,tool in ((f,'verify_native_event_queue_frames.py'),(q,'verify_native_event_queue.py'),(init,'verify_native_game_init.py')):
        assert row['build_verification_sha256']==bh and row['verifier_sha256']==sha(p/'jp_port_tools'/tool)
    assert f['checks']==439 and q['checks']==1019169 and init['checks']==530
    assert q['original_complete_event_calls']==288 and q['original_entry_frees']==36 and q['original_player_weld_calls']==176
    raw,_=suite.derive()
    assert (a.frames/'run/private_event_queue_frame_body.py').read_text()==raw
    assert sha(a.frames/'run/private_event_queue_frame_body.py')==f['derived_exercise_sha256']
    assert f['event_owner_sha256']==sha(p/'jp_port_tools/native_event_frame_owner.py')
    assert f['actual_window_presents']==10 and f['texture_Off_backend_bindings_and_uploads_zero']
    assert f['original_event_motions_draw_connected_to_real_rendering'] and f['original_player_weld_connected_to_real_rendering']
    assert f['all_canonical_bytes_restored_before_Motion_shutdown'] and f['canonical_records']==3322
    for name in ('event_samples','camera_fit_event_samples'):
        rows=f[name];assert len(rows)==9
        for i,row in enumerate(rows,1):
            assert row['step']==i and row['frame']==(float(i) if i<8 else 0. if i==8 else .5)
            assert row['previous_live']==(i<8)
            assert row['free_bytes']==rows[0]['free_bytes']+(80 if i>=8 else 0)
    images=f['rendered_original_action_frames'];assert len(images)==9
    for i,row in enumerate(images,1):
        assert row['event_draw_step']==i and row['current_motion_frame']==0.
        assert row['blend_ratio']==(i/9. if i<9 else None)
    for row in f['screenshots']+images+[f['animated_preview']]:assert sha(row['path'])==row['sha256']
    preview=Image.open(f['animated_preview']['path']);assert preview.n_frames==9
    for i,row in enumerate(images):
        preview.seek(i)
        assert preview.convert('RGBA').tobytes()==Image.open(row['path']).convert('RGBA').tobytes()
    assert len({Image.open(row['path']).convert('RGB').tobytes() for row in images})>=3
    assert len([x for x in f['task_callback_events'] if x[0]=='draw'])==9
    assert len([x for x in f['task_callback_events'] if x[0]=='destroy'])==1
    d.mkdir()
    audit=dict(status='unchanged_objects_original_cleanup_export_real_event_weld_task_GPU_and_images_audited',
               auditor_sha256=sha(__file__),build_verification_sha256=bh,
               prior_packages_verified=40,new_checks=439,selected_regression_checks=1019699,
               compiled_game_objects_unchanged=True,only_added_Frame_export='event_free',
               all_nine_lossless_preview_frames_verified=True,playable=False)
    (d/'audit.json').write_text(json.dumps(audit,indent=2)+'\n')
    evidence={'build':a.build/'build_verification.json','frames':a.frames/'verification.json',
              'queue':a.queue/'verification.json','init':a.init/'verification.json','audit':d/'audit.json'}
    proof={}
    for n,path in evidence.items():
        to=p/'analysis'/('jp_native_event_frames_'+n+'.json');assert not to.exists()
        shutil.copyfile(path,to);proof[to.relative_to(r).as_posix()]=sha(to)
    files=['jp_port_tools/'+n for n in ('build_native_event_frame_cleanup.py','native_event_frame_owner.py',
            'verify_native_event_queue_frames.py','qualify_native_event_frames.py')]+['JP_NATIVE_EVENT_FRAMES.txt']
    for n in files:
        if n.endswith('.py'):ast.parse((p/n).read_text())
    out=dict(status='original_event_queue_and_player_welding_real_task_GPU_scoped_qualification',
             new_checks=439,selected_regression_checks=1019699,
             combined_qualified_scoped_checks=read(previous)['combined_qualified_scoped_checks']+439,
             original_event_calls_in_repeated_regression=288,actual_visible_event_draws=9,
             separate_camera_fit_event_draws=9,actual_window_presents=10,
             texture_Off_zero_sampling_bindings_and_uploads=True,
             original_weld_FREE_restores_all_3322_canonical_records=True,
             original_event_cleanup_and_heap_retirement_verified=True,
             original_game_objects_unchanged_only_cleanup_export_added=True,
             prior_qualifications_preserved={'event_queue':dict(path=previous.relative_to(r).as_posix(),sha256=sha(previous))},
             authored_source_sha256={(p/n).relative_to(r).as_posix():sha(p/n) for n in files},
             proof_files_sha256=proof,build_verification_sha256=bh,
             private_frames_DLL_sha256=b['frames_DLL_sha256'],
             production_original_player_actor_callback_FSM_mainloop_or_gameplay_verified=False,
             public_research_build_payloads_changed=False,
             retail_generated_external_source_captures_or_combined_DLLs_publishable=False,
             original_source_and_proofs_preserved=True,playable=False,
             overall_completion_percent=10,goal_active=True)
    target=p/'analysis/jp_native_event_frames_package.json';assert not target.exists()
    (d/'qualification.json').write_text(json.dumps(out,indent=2)+'\n')
    shutil.copyfile(d/'qualification.json',target)
    print(out['status'],'439 new checks;1019699 repeated regression checks',flush=True)

if __name__=='__main__':main()
