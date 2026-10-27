"""Byte/layout/canonical and exact integration preservation for channel decoders."""
import csv,hashlib,json,zipfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 tp=P/'analysis/movie_channel_decoder_translation.json';t=json.loads(tp.read_text());binary=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=binary.read_bytes();assert digest(binary)=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
 def data(a,n):return raw[a-0x8c010000:a-0x8c010000+n]
 def value(a,n=4,signed=False):return int.from_bytes(data(a,n),'little',signed=signed)
 rows={}
 for r in t['routines']:
  for row in csv.DictReader(Path(r['listing']).open(),delimiter='\t'):
   pc=int(row['address'],16);assert pc not in rows and data(pc,2)==bytes.fromhex(row['bytes']);rows[pc]=row
 assert len(t['routines'])==9 and len(rows)==t['instruction_rows']==669
 assert all(value(a,2,signed=True)==0x148 for a in (0x8c648360,0x8c648490,0x8c6485ae,0x8c6475ec,0x8c64b2e0,0x8c660ad0))
 assert all(value(a)==0x8c89c550 for a in (0x8c648368,0x8c648494,0x8c6485c8))
 assert all(value(a,2,signed=True)==-255 for a in (0x8c6479b8,0x8c648362,0x8c648492,0x8c6485b0))
 assert (value(0x8c64b5be,2,True),value(0x8c64b5c0,2,True),value(0x8c64b5c2,2,True),value(0x8c64b5c4,2,True))==(2047,255,511,1023)
 assert value(0x8c648374)==value(0x8c6485b8)==0x8c673ba8 and value(0x8c64849c)==0x8c673bec
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'];canonical=next(r for r in registry if r['address']=='0x8c647bde' and r.get('module','1ST_READ.BIN')=='1ST_READ.BIN')
 adapted=next(r for r in t['routines'] if r['address']=='0x8c647bde');assert canonical['retail_span_sha256']==adapted['retail_span_sha256'] and adapted['instruction_rows']==17
 old=(P/'source/sa_movie_channel_worker_connected.c').read_text();bridge=(P/'source/sa_movie_channel_decoder_connected.c').read_text();marker='#include "sh4_exception_state.h"';expected=old.replace(marker,''.join(f'VDECL({r["name"]});\n' for r in t['routines'])+marker)
 marker=' return -2;';assert expected.count(marker)==1
 expected=expected.replace(marker,''.join(f' if(target=={r["address"]}u)return {r["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n' for r in t['routines'])+marker)
 for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):expected=expected.replace('sa_movie_channel_worker_'+suffix,'sa_movie_channel_decoder_'+suffix)
 assert expected==bridge
 assert (P/'source/movie_channel_decoder_probe.c').read_text()==(P/'source/movie_channel_worker_probe.c').read_text().replace('sa_movie_channel_worker_','sa_movie_channel_decoder_')
 source=(P/t['source']).read_text();assert 'SA_BOUND_RETAIL_STALLS' not in source
 receipt=ROOT/'work/sonic-adventure/channel_decoder_previous_build_manifest.json'
 if not receipt.exists():
  with zipfile.ZipFile(ROOT/'publication/release-assets/sonic-adventure-windows-research-builds.zip') as z:
   previous=z.read('BUILD_MANIFEST.json');assert json.loads(previous)['source_manifest_sha256']=='8ed55c0db8c4f822b53060891edebe8cbe6e11249b9b1ca24a3a0d84038cf5be';receipt.write_bytes(previous)
 previous=json.loads(receipt.read_text());assert len(previous['files'])==2671
 for row in previous['files']:assert digest(ROOT/row['path'])==row['sha256']
 files=[Path(__file__),tp,P/t['source'],P/'source/sa_movie_channel_worker_connected.c',P/'source/sa_movie_channel_decoder_connected.c',P/'source/movie_channel_worker_probe.c',P/'source/movie_channel_decoder_probe.c',P/canonical['source'],P/canonical['verification'],P/'ghidra/export_movie_channel_decoders.ps1',P/'ghidra/export_movie_channel_decoder_lower.ps1',P/'ghidra/export_movie_channel_decoder_io.ps1']
 report=dict(instruction_rows_byte_checked=669,proposed_entries=8,proposed_rows=652,canonical_adapted_entries=1,canonical_rows=17,canonical_additional_credit=0,canonical_entry=canonical['address'],descriptor_stride=0x148,descriptor_base='0x8c89c550',sentinel=-255,validator_kinds=[255,511,1023,2047],descriptor_capacity_limit=2047,parameter_tables={'command1_mode1':'0x8c673ba8','command18':'0x8c673bec'},exact_bridge_extension_checked=True,prior_context_runtime_exception_guards_unchanged=True,prior_probe_ABI_layouts_unchanged=True,new_research_loop_guards=0,existing_worker_source_unchanged=True,preserved_archived_builds=2671,sha256={str(p):digest(p) for p in files},scope='Staticauditonly:all669originalrows,descriptorstride/base/signedsentinel/typebounds/twoDISTINCTparameter tables andcanonical647bdehash checked. Exactold219bridgeplus9routes/exportprefixchanges;oldworker/continuations/SRFD/context/runtime/ABI unchanged. All2671oldbuildbytespreserved. No decoder/callback results or taskcontext/counter repairs. Loops/nonlocalIOcontinuations andactivecodecpaths remainunqualified;statictranslation alone grantszero recoverycredit.')
 (P/'analysis/movie_channel_decoder_contract_audit.json').write_text(json.dumps(report,indent=2)+'\n');print('DECODER AUDIT669rows;canonical17zero credit;old2671builds preserved')
if __name__=='__main__':main()
