"""Static preservation and byte evidence for guarded input-pump integration."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]

def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
 mp=P/'analysis/movie_input_pump_translation.json';t=json.loads(mp.read_text());cp=P/'analysis/movie_input_pump_connection.json';c=json.loads(cp.read_text())
 for report in (t,c):
  for pin,h in report['sha256'].items():assert digest(pin)==h,pin
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();count=0
 for body in t['routines']:
  for row in csv.DictReader(Path(body['listing']).open(),delimiter='\t'):
   a=int(row['address'],16);raw=bytes.fromhex(row['bytes']);assert binary[a-0x8c010000:a-0x8c010000+2]==raw;count+=1
 assert (count,t['canonical_rows'],t['proposed_rows'],len(t['canonical_reuse']))==(1035,370,665,6)
 old=(P/'source/sa_movie_channel_request_connected.c').read_text();new=(P/c['source']).read_text()
 for body in t['routines']:
  decl=f'VDECL({body["name"]});\n';route=f' if(target=={body["address"]}u)return {body["name"]}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n';assert new.count(decl)==new.count(route)==1;new=new.replace(decl,'').replace(route,'')
 for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):new=new.replace('sa_movie_input_pump_'+suffix,'sa_movie_channel_request_'+suffix)
 def allowed(text):return text.split('static const uint32_t allowed[]={')[1].split('};',1)[0]
 returns={int(x[:-1],16) for x in allowed(new).split(',')};new=new.replace(allowed(new),allowed(old));assert new==old
 assert (P/'source/movie_input_pump_probe.c').read_text()==(P/'source/movie_channel_request_probe.c').read_text().replace('sa_movie_channel_request_','sa_movie_input_pump_')
 import movie_input_pump_reference as v
 import movie_channel_request_reference as previous
 import input_pump_reference_cpu as cpu
 assert v.code==previous.code and returns==v.namespace['RETURNS']
 assert cpu.code.replace('if pc in (0x8c66049a,0x8c6604ec,0x8c65f1d2):','if pc in (0x8c66049a,0x8c6604ec):')==cpu.old.code
 inherited={pc:set(rows) for pc,rows in previous.namespace['PACKET_INTERNAL_ENTRY_ROWS'].items()};new_internal={pc:set(rows) for pc,rows in v.namespace['PACKET_INTERNAL_ENTRY_ROWS'].items()};new_internal[0x8c668b24].remove(0x8c668b32)
 if not new_internal[0x8c668b24]:del new_internal[0x8c668b24]
 assert new_internal==inherited
 assert binary[0x8c668b30-0x8c010000:0x8c668b34-0x8c010000]==bytes.fromhex('f88f0474')
 for name in ('REGISTER_ROWS','CONTEXT_ROWS','CD_FPU_ROWS','AI_FPU_ROWS','GUARDS','GUARD_ENTRY'):assert v.namespace[name]==previous.namespace[name]
 text=(P/t['source']).read_text();assert 'SA_BOUND_RETAIL_STALLS' not in text;line=next(x for x in text.splitlines() if x.startswith('L8c65f1d2:'))
 for token in ('valid_ram(r[0],1u)','bus->cache(bus->context,r[0],3u)','value|0x80u','(value==0)'):assert token in line
 previous_builds=P/'analysis/movie_input_pump_previous_build_manifest.json';builds=json.loads(previous_builds.read_text());assert len(builds['files'])==2677
 for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'],row['path']
 files=[Path(__file__),mp,cp,P/t['source'],P/c['source'],P/'input_pump_reference_cpu.py',P/'movie_input_pump_reference.py',P/'write_watch_input_pump.py',previous_builds]
 report=dict(instruction_rows_byte_checked=1035,canonical_registered_adapters=6,canonical_rows=370,unregistered_response_proposals=7,proposed_rows=665,canonical_duplicate_credit=0,already_connected_helpers_retained=['0x8c6461ae','0x8c65f50c','0x8c6602d2','0x8c6607a8'],exact_prior_bridge_plus_thirteen_routes=True,prior_runtime_external_ABI_SRFD_guards_unchanged=True,prior_flat_execution_code_identical=True,only_CPU_change_new_TAS_purge=True,only_observer_change_local_word_swap_loop=True,all_original_word_swap_iterations_preserved=True,guard_call_return_sites=len(returns),new_resume_exports=0,new_source_wait_limits=0,preserved_archived_builds=2677,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='Staticauditonly,notbehaviorcredit. All1035sourceinstructionsbytechecked;exactpreviousbridge/runtime/ABI inverse,13routes/newcall-returnmembership only. OriginalCPUnewTASpurge,observationdataonlyexcludesknownlocal668b32->668b24loopfromentryevents;allinstruction/RAM/state comparisons retained. Old2677buildbytes preserved.')
 (P/'analysis/movie_input_pump_audit.json').write_text(json.dumps(report,indent=2)+'\n');print('INPUT PUMP AUDIT PASS1035rows;',len(returns),'returns;2677priorbuilds preserved')
if __name__=='__main__':main()
