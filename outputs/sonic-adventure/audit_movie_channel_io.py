"""Check original rows, exact continuation adaptation and retained guard policies."""
import csv,hashlib,json,re,zipfile
from pathlib import Path
from channel_io_continuations import adapt_decoder,adapt_worker,body_end
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 tp=P/'analysis/movie_channel_io_translation.json';t=json.loads(tp.read_text());bp=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=bp.read_bytes();assert digest(bp)=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
 older_paths=[P/'analysis/movie_channel_decoder_translation.json',P/'analysis/movie_channel_worker_translation.json'];older=[json.loads(p.read_text()) for p in older_paths];all_rows=0
 for manifest in [t]+older:
  for body in manifest['routines']:
   for row in csv.DictReader(Path(body['listing']).open(),delimiter='\t'):
    pc=int(row['address'],16);assert raw[pc-0x8c010000:pc-0x8c010000+2]==bytes.fromhex(row['bytes']);all_rows+=1
 assert all_rows==1332 and t['instruction_rows']==391 and len(t['routines'])==12
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];prior=registry[:1488];canonical=[]
 for body in t['routines']:
  old=next((r for r in prior if r['address']==body['address'] and r.get('module','1ST_READ.BIN')=='1ST_READ.BIN'),None)
  if old:assert old['retail_span_sha256']==body['retail_span_sha256'];canonical.append(dict(address=body['address'],instruction_rows=body['instruction_rows'],source=old['source'],verification=old['verification']))
 assert len(canonical)==8 and sum(r['instruction_rows'] for r in canonical)==96
 decoder=P/'source/sa_movie_channel_decoder.c';worker=P/'source/sa_movie_channel_worker.c'
 for source,name in ((decoder,'movie_channel_decoder_qualification.json'),(worker,'movie_channel_worker_qualification.json')):
  prior_proof=json.loads((P/'analysis'/name).read_text());assert prior_proof['sha256'][str(source)]==digest(source)
 expected,decoder_routes=adapt_decoder(decoder.read_text(),older[0]['routines'],'sa_movie_channel_io_decoder');assert expected==(P/t['adapted_decoder_source']).read_text()
 expected,worker_routes=adapt_worker(worker.read_text(),next(r for r in older[1]['routines'] if r['address']=='0x8c647980'),'sa_movie_channel_io_worker');assert expected==(P/t['adapted_worker_source']).read_text()
 assert len(t['resume_routes'])==len({r['pc'] for r in t['resume_routes']})==82
 assert all(any(q==r for q in t['resume_routes']) for r in decoder_routes+worker_routes)
 old=(P/'source/sa_movie_channel_decoder_connected.c').read_text();bridge=(P/'source/sa_movie_channel_io_connected.c').read_text();stripped=bridge
 for r in t['routines']+t['resume_routes']:
  line=f'VDECL({r["name"]});\n';assert stripped.count(line)==1;stripped=stripped.replace(line,'')
 for r in t['routines']:
  line=f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n';assert stripped.count(line)==1;stripped=stripped.replace(line,'')
 for r in t['resume_routes']:
  line=f' if(target=={r["pc"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n';assert stripped.count(line)==1;stripped=stripped.replace(line,'')
 start=stripped.index('int sa_movie_channel_io_return_allowed(');end=body_end(stripped,start);guard=stripped[start:end];stripped=stripped[:start]+stripped[end:]
 allowed=[int(v,16) for v in re.search(r'static const uint32_t allowed\[\]=\{([^}]+)\}',guard)[1].replace('u','').split(',')]
 data=json.loads((P/t['return_manifest']).read_text());assert allowed==sorted(map(lambda x:int(x,16),data['allowed_call_returns'])) and len(allowed)==1920
 assert 'if(target==c->root_pr)return 1;' in guard and 'unsigned mid=low+(high-low)/2;if(allowed[mid]<target)low=mid+1;else high=mid;' in guard and 'allowed[low]==target' in guard
 marker='struct sa_flash_storage *flash;uint32_t root_pr;};';assert stripped.count(marker)==1;stripped=stripped.replace(marker,'struct sa_flash_storage *flash;};')
 marker=',s->pr};int status=dispatch';assert stripped.count(marker)==4;stripped=stripped.replace(marker,'};int status=dispatch')
 for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):stripped=stripped.replace('sa_movie_channel_io_'+suffix,'sa_movie_channel_decoder_'+suffix)
 assert [line for line in stripped.splitlines() if line.strip()]==[line for line in old.splitlines() if line.strip()]
 assert (P/'source/movie_channel_io_probe.c').read_text()==(P/'source/movie_channel_decoder_probe.c').read_text().replace('sa_movie_channel_decoder_','sa_movie_channel_io_')
 begin=old.index('static int runtime_poll(');end=old.index('static int dispatch(void *opaque',begin);assert old[begin:end] in bridge
 import movie_channel_io_reference as v
 assert v.code==v.old.code and set(allowed)==v.namespace['RETURNS'] and data['prior_return_sites']==1883
 assert data['prior_model_code_sha256']==hashlib.sha256(v.old.code.encode()).hexdigest()
 assert data['prior_rows_sha256']==hashlib.sha256(json.dumps(sorted(v.old.ROWS.items())).encode()).hexdigest()
 for name in ('REGISTER_ROWS','CONTEXT_ROWS','CD_FPU_ROWS','AI_FPU_ROWS','GUARDS','GUARD_ENTRY'):assert v.namespace[name]==v.old.namespace[name]
 assert all(getattr(v,name)==value for name,value in v.INHERITED_INTEGER_CONSTANTS.items())
 assert 0x8cfef000 not in allowed and 'SA_BOUND_RETAIL_STALLS' not in (P/t['source']).read_text()
 receipt=ROOT/'work/sonic-adventure/channel_io_previous_build_manifest.json'
 if not receipt.exists():
  with zipfile.ZipFile(ROOT/'publication/release-assets/sonic-adventure-windows-research-builds.zip') as z:
   previous=z.read('BUILD_MANIFEST.json');assert json.loads(previous)['source_manifest_sha256']=='813e488de5f22fdaa0fd3c6274db6dede6892ff336ce127e942caf9362f28957';receipt.write_bytes(previous)
 previous=json.loads(receipt.read_text());assert len(previous['files'])==2673
 for row in previous['files']:assert digest(ROOT/row['path'])==row['sha256']
 files=[Path(__file__),P/'channel_io_continuations.py',tp,*older_paths,decoder,worker,P/t['source'],P/t['adapted_decoder_source'],P/t['adapted_worker_source'],P/t['return_manifest'],P/'source/sa_movie_channel_decoder_connected.c',P/'source/sa_movie_channel_io_connected.c',P/'source/movie_channel_decoder_probe.c',P/'source/movie_channel_io_probe.c',Path(v.__file__)]
 for r in canonical:files.extend((P/r['source'],P/r['verification']))
 report=dict(instruction_rows_byte_checked=1332,new_integration_rows=391,new_proposed_original_rows=295,canonical_integration_rows=96,canonical_entries=canonical,canonical_duplicate_credit=0,adapted_previous_rows=941,adapted_additional_credit=0,explicit_nonfunction_continuation_routes=82,guard_allowed_call_returns=1920,prior_guard_call_returns=1883,root_pr_captured_at_four_unchanged_external_ABIs=True,exact_flat_original_return_guard_membership=True,return_target_captured_before_delay=True,ordinary_entry_pr_checks_preserved=True,original_model_execution_code_identical=True,inherited_SR_FD_guards_preserved=True,bridge_inverse_restores_all_old_nonblank_executable_text=True,old_runtime_blocks_byte_preserved=True,old_probe_argument_layouts_preserved=True,decoder_worker_adaptation_exact=True,prior_decoder_worker_sources_hash_preserved=True,new_source_wait_limits=0,preserved_archived_builds=2673,sha256={str(p):digest(p) for p in files},scope='391integrationrows+941adaptedolderrows retailbytechecked;8canonical96rows/82nonfunctionresumeexports addzerooriginalcredit. Exactdecoder/worker transformation andinversebridge changecheck;oldruntime/SRFD/ABI/modelcode preserved. NewinternalhostrootPRmetadata andresumedRTS checkusesexact1920flatmodelcall-returnset plusrootentryPR;ordinaryentrychecks unchanged andRTStargetcapturedpredelay. Thismembershipdoesnotproveall1920nativecontinuationgraphs;onlyactualnestedfailure paths/faultsqualifiedbybehaviorreports. Arbitraryreturns/callbackresumegraphs/activeheaderparsing remainpartial. No fakeparserresults/guesttaskrepairs.')
 (P/'analysis/movie_channel_io_contract_audit.json').write_text(json.dumps(report,indent=2)+'\n');print('IO AUDIT1332bytechecked/82continuations/1920guardreturns;old2673builds preserved')
if __name__=='__main__':main()
