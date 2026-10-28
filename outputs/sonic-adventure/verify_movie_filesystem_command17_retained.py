"""Replay unchanged IRQ/context expectations and selected original task contracts."""
import ast,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def integers(source):return [n.value for n in ast.walk(ast.parse(source)) if isinstance(n,ast.Constant) and type(n.value) is int]
def main():
    parent=P/'verify_movie_input_interface_retained.py';original=parent.read_text()
    source=original.replace('movie_input_interface','movie_filesystem_command17').replace('write_watch_input_interface','write_watch_filesystem_command17').replace('225','243').replace('INPUT PUMP RETAINED','FILESYSTEM RETAINED')
    assert integers(source)==integers(original)
    ns=dict(__name__='filesystem_retained_replay',__file__=str(parent));exec(compile(source,str(parent),'exec'),ns);ns['main']()
    path=P/'analysis/movie_filesystem_command17_retained_verification.json';report=json.loads(path.read_text())
    parent_tasks=P/'verify_movie_input_interface_tasks.py';original=parent_tasks.read_text()
    source=original.replace('movie_input_interface','movie_filesystem_command17').replace('write_watch_input_interface','write_watch_filesystem_command17').replace('movie_filesystem_command17_tasks_verification.json','movie_filesystem_command17_retained_tasks_verification.json')
    assert integers(source)==integers(original)
    ns=dict(__name__='filesystem_retained_task_replay',__file__=str(parent_tasks));exec(compile(source,str(parent_tasks),'exec'),ns)
    original_configs=ns['configurations'];selected=[c for c in original_configs() if c['kind']=='core' and c['order']==c['endian'] and c['geometry']==(33 if c['command']==1 else 511)]
    assert len(selected)==8
    ns['configurations']=lambda:iter(selected);ns['main']()
    tp=P/'analysis/movie_filesystem_command17_retained_tasks_verification.json';tasks=json.loads(tp.read_text())
    assert tasks['lifecycles_passed']==8 and tasks['original_native_comparisons']==160
    tasks['scope']='Eight selected prior task contracts replayed in243:channels0/7,matching transport/interface byteorders,command1 invalid geometry andcommand18 successful512byte read/callback/idle/teardown. Original numeric expectations unchanged;valid command1 frontier is tested separately. No new routine/route credit from retained evidence.'
    tasks['sha256'][str(Path(__file__))]=digest(Path(__file__));tp.write_text(json.dumps(tasks,indent=2)+'\n')
    report['suites'].append(dict(suite='selected_original_task_contracts',original_native_comparisons=tasks['original_native_comparisons'],independent_checks=tasks['independent_checks'],proof=tp.relative_to(P).as_posix()))
    report['original_native_comparisons']+=tasks['original_native_comparisons'];report['independent_checks']+=tasks['independent_checks']
    report['sha256'].update({str(Path(__file__)):digest(Path(__file__)),str(parent):digest(parent),str(parent_tasks):digest(parent_tasks),str(tp):digest(tp)})
    report['scope']='Fresh243 retained controls:83softwareIRQ/contextSRFD comparisons and160phases across8selected prior original tasks. Prior numeric assertions unchanged,verified byAST. Old proofs/builds preserved;all other old evidence historical,not newly rerun. No new routine credit;no fresh243 full startup or physical hardware claim.'
    assert report['original_native_comparisons']==243
    path.write_text(json.dumps(report,indent=2)+'\n');print('FILESYSTEM RETAINED PASS',report['original_native_comparisons'],report['independent_checks'],flush=True)
if __name__=='__main__':main()
