"""Fresh command7 success/abort traces with exact gateway and return coverage."""
import json,runpy,types
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def main():
    parent=P/'verify_movie_filesystem_persist_tasks.py';original=runpy.run_path(str(parent));old=original['ns'];v=old['v']
    previous=P/'analysis/movie_filesystem_delete_translation.json';body=next(b for b in json.loads(previous.read_text())['routines'] if b['address']=='0x8c649d20')
    source=original['source'];marker="name='movie_filesystem_persist_tasks'";assert source.count(marker)==1;source=source.replace(marker,"name='movie_delete_gateway_async'")
    runtime=dict(__name__='gateway_async_replay',__file__=str(Path(__file__)),transaction_plan=original['transaction_plan'],parent=old['parent'],ROOT=ROOT,ROUTES={0x8c64ab88,0x8c64af0e,0x8c64b00a,0x8c64b06c,0x8c64ac38,0x8c64ac42,0x8c649db6,0x8c647a70})
    exec(compile(source,str(Path(__file__)),'exec'),runtime)
    runtime['v']=types.SimpleNamespace(**dict(vars(v),manifest=dict(v.manifest,routines=[body]+v.manifest['routines'])))
    cases=[c for c in original['configuration_list']() if c['kind'] in ('happy_a','dir_abort')];assert len(cases)==2
    runtime['configurations']=lambda:cases;runtime['main']()
    out=P/'analysis/movie_delete_gateway_async_verification.json';report=json.loads(out.read_text())
    report['sha256'].update({str(p):runtime['digest'](p) for p in (parent,previous)})
    report['scope']='Two fresh raw-initialized command7 cases: happy_a and dir_abort only. Published independent request/ACK/buffer/counter/callback/cleanup assertions retained. No secondary two-dirty mutation case is run here; that remains prior evidence. Gateway649d20 coverage added through read-only manifest projection. Correct FATpass return64af0e observed after real response wait/resume;precall64af0c not credited. No supplied parsedmetadata,frames,IDs,PR,calleeoutputs orcallbackresults. NULLwritehook and explicitabortpolicy0 domain only;not physicalDMA/flashdurability. Original source andprior reports unchanged.'
    out.write_text(json.dumps(report,indent=2)+'\n')
if __name__=='__main__':main()
