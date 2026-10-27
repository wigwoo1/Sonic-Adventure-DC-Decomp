"""Bound the dispatch/continuation claims to original code and preserved state."""
import csv,hashlib,json,re,zipfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 tpath=P/'analysis/movie_channel_worker_translation.json';t=json.loads(tpath.read_text());binary_path=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=binary_path.read_bytes()
 assert digest(binary_path)=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
 def data(a,n):return raw[a-0x8c010000:a-0x8c010000+n]
 def word(a):return int.from_bytes(data(a,4),'little')
 routines=t['routines'];assert len(routines)==9 and sum(r['instruction_rows'] for r in routines)==272
 worker=next(r for r in routines if r['address']=='0x8c647980');addresses={int(a,16) for a in worker['reachable_addresses']}
 rows={}
 for body in routines:
  for row in csv.DictReader(Path(body['listing']).open(),delimiter='\t'):
   pc=int(row['address'],16);encoded=bytes.fromhex(row['bytes']);assert data(pc,2)==encoded;assert pc not in rows;rows[pc]=row
 assert len(rows)==272
 targets=[]
 for i in range(21):
  target=0x8c6479b8+int.from_bytes(data(0x8c6479c8+i*2,2),'little',signed=True);assert target in addresses;targets.append(target)
 assert worker['computed_table_targets']==list(map(hex,targets)) and len(set(targets))==19
 assert data(0x8c6479a2,2)==bytes.fromhex('15e1') and data(0x8c6479a4,2)==bytes.fromhex('1230') and data(0x8c6479b4,2)==bytes.fromhex('2300') and data(0x8c6479b6,2)==bytes.fromhex('0900')
 selectors=[]
 for channel in range(8):
  entry=word(0x8c647938+8*channel);payload=word(0x8c647934+8*channel)
  assert entry==(0x8c64790e+4*channel if channel<7 else 0x8c647980) and payload==0x8c894538+0x1000*channel
  if channel<7:
   assert data(entry+2,2)==bytes((channel,0xe4))
   branch=int.from_bytes(data(entry,2),'little');offset=branch&4095;offset-=4096 if offset&2048 else 0
   assert branch>>12==10 and entry+4+2*offset==0x8c647982
  else:assert data(entry,2)==bytes.fromhex('07e4')
  selectors.append(dict(channel=channel,entry=hex(entry),payload=hex(payload)))
 assert word(0x8c647c44)==0x8c66086a and word(0x8c660890)==0x8c668e5c
 assert rows[0x8c66086c]['bytes']=='2b42' and rows[0x8c66086e]['bytes']=='0900'
 old=(P/'source/sa_movie_task_owner_connected.c').read_text();bridge=(P/'source/sa_movie_channel_worker_connected.c').read_text();marker='#include "sh4_exception_state.h"'
 expected=old.replace(marker,'VDECL(sa_movie_channel_worker_resume_8c647982);\nVDECL(sa_movie_channel_worker_resume_8c647b74);\n'+''.join(f'VDECL({r["name"]});\n' for r in routines)+marker)
 marker=' return -2;';assert expected.count(marker)==1
 expected=expected.replace(marker,''.join(f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n' for r in routines)+marker)
 for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):expected=expected.replace('sa_movie_task_owner_'+suffix,'sa_movie_channel_worker_'+suffix)
 marker=' if(c->observe)c->observe(c->user,target,m,f,sr,r,pr);'
 expected=expected.replace(marker,''.join(f' if(target==0x{pc:x}u)return sa_movie_channel_worker_resume_{pc:08x}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n' for pc in (0x8c647982,0x8c647b74))+marker)
 assert expected==bridge
 assert (P/'source/movie_channel_worker_probe.c').read_text()==(P/'source/movie_task_owner_probe.c').read_text().replace('sa_movie_task_owner_','sa_movie_channel_worker_')
 source=(P/t['source']).read_text();assert 'SA_BOUND_RETAIL_STALLS' not in source
 assert 'target=0x8c6479b8u+r[0];' in source and 'default:if(!call)return -2;return call(ctx,target,m,f,sr,r,pr);' in source
 for pc in set(targets):assert f'case 0x{pc:x}u:goto L{pc:x};' in source
 assert 'if(resume_pc==0x8c647982u)goto L8c647982;if(resume_pc==0x8c647b74u)goto L8c647b74;goto L8c647980;' in source
 # Preserve the exact last published build collection before the next archive.
 receipt=ROOT/'work/sonic-adventure/channel_worker_previous_build_manifest.json'
 if not receipt.exists():
  with zipfile.ZipFile(ROOT/'publication/release-assets/sonic-adventure-windows-research-builds.zip') as z:
   previous=z.read('BUILD_MANIFEST.json');assert json.loads(previous)['source_manifest_sha256']=='9ec68df6fef34cb934a3e3eab9779530bd0a6c31c81fd409bfa422ecd0a30a86';receipt.write_bytes(previous)
 previous=json.loads(receipt.read_text());assert len(previous['files'])==2669
 for row in previous['files']:assert digest(ROOT/row['path'])==row['sha256']
 files=[Path(__file__),tpath,P/'source/sa_movie_task_owner_connected.c',P/'source/sa_movie_channel_worker_connected.c',P/'source/movie_task_owner_probe.c',P/'source/movie_channel_worker_probe.c',P/t['source'],P/'ghidra/export_movie_channel_worker.ps1',P/'ghidra/export_movie_channel_decoders.ps1']
 report=dict(instruction_rows_byte_checked=272,dispatch_slots=21,unique_dispatch_targets=19,idle_selectors=[i for i,a in enumerate(targets) if a==0x8c647b6e],table_targets=list(map(hex,targets)),selector_entries=selectors,unsigned_out_of_range_guard_preserved=True,signed_table_load_preserved=True,dynamic_braf_target_preserved=True,unknown_computed_target_dispatched=True,original_yield_thunk_target='0x8c668e5c',shared_entry_and_yield_resume_before_observer=True,worker_reinitialization_skipped_on_resume=True,new_research_loop_guards=0,exact_bridge_extension_checked=True,prior_context_runtime_exception_guards_unchanged=True,prior_probe_ABI_layouts_unchanged=True,preserved_archived_builds=2669,new_routine_credit=0,sha256={str(p):digest(p) for p in files},scope='Static claims only:all272 originalrows bytechecked,real21-slot signedoffsettable/unsignedrangeguard/eighttaskselectors/payloads/thunk targets checked. Exactprior218bridge+9routes+2nonfunction continuations andexportprefix changes;prior runtime/context/guards/ABI unchanged. Fullworkerpostdecoder/callback behavior remainsunqualified. No counters/slots/contexts/results repaired or supplied by integration. This audit alone grantszero behavior/recoverycredit.')
 (P/'analysis/movie_channel_worker_contract_audit.json').write_text(json.dumps(report,indent=2)+'\n');print('CHANNEL AUDIT272/21slots/19targets;old2669builds preserved')
if __name__=='__main__':main()
