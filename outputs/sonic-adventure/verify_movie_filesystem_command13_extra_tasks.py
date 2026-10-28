"""Additional original command13 high-flag failure sessions, with the same literal oracle."""
import json,runpy
from pathlib import Path
P=Path(__file__).resolve().parent

def main():
    parent=P/'verify_movie_filesystem_command13_tasks.py';source=parent.read_text();marker="name='movie_filesystem_command13_tasks'";assert source.count(marker)==1
    source=source.replace(marker,"name='movie_filesystem_command13_extra_tasks'")
    ns=dict(__name__='command13_extra',__file__=str(Path(__file__)));exec(compile(source,str(Path(__file__)),'exec'),ns)
    configs=[dict(c,flags=c['flags']|0x80000000) for c in ns['configurations']() if c['kind'] in ('pool_full','data_abort','dir_abort')]
    assert len(configs)==3;ns['configurations']=lambda:configs;ns['main']()
    path=P/'analysis/movie_filesystem_command13_extra_tasks_verification.json';r=json.loads(path.read_text());r['sha256'][str(parent)]=ns['digest'](parent);path.write_text(json.dumps(r,indent=2)+'\n')
if __name__=='__main__':main()
