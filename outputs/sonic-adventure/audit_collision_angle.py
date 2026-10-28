"""Audit original inverse-trigonometry integration and preservation of the prior ABI."""
import csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 mp=P/'analysis/collision_angle_translation.json';t=json.loads(mp.read_text());cp=P/'analysis/collision_angle_connection.json';c=json.loads(cp.read_text());files=[Path(__file__),mp,cp]
 for report in (t,c):
  for pin,h in report['sha256'].items():assert digest(pin)==h,pin;files.append(Path(pin))
 assert len(t['routines'])==5 and t['instruction_rows']==223 and len(t['resume_routes'])==3 and t['canonical_reused_entries']==['0x8c10fbb8']
 binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes();checked=0
 for body in t['routines']:
  listing=Path(body['listing']);files.append(listing)
  for row in csv.DictReader(listing.open(),delimiter='\t'):
   at=int(row['address'],16)-0x8c010000;assert binary[at:at+2]==bytes.fromhex(row['bytes']);checked+=1
 assert checked==223
 old=(P/'source/sa_collision_math_connected.c').read_text();new=(P/c['source']).read_text()
 for body in t['routines']+t['resume_routes']:
  address=body.get('address',body.get('pc'));name=body['name'];decl=f'VDECL({name});\n';route=f' if(target=={address}u)return {name}(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,&c->stop,c->gbr,c->bus,c->integer,c->vbr);\n'
  assert new.count(decl)==new.count(route)==1;new=new.replace(decl,'').replace(route,'')
 for suffix in ('connected','initialized_connected','exception_connected','running_connected','storage_connected'):new=new.replace('sa_collision_angle_'+suffix,'sa_collision_math_'+suffix)
 marker='static const uint32_t allowed[]={';a=new.index(marker)+len(marker);b=new.index('};',a);oa=old.index(marker)+len(marker);ob=old.index('};',oa)
 before=set(old[oa:ob].split(','));after=set(new[a:b].split(','));returns=json.loads((P/'analysis/collision_angle_returns.json').read_text())
 assert len(before)==2268 and after-before=={x+'u' for x in returns['new_call_returns']} and before<=after
 new=new[:a]+old[oa:ob]+new[b:];assert new==old
 assert (P/'source/collision_angle_probe.c').read_text()==(P/'source/collision_math_probe.c').read_text().replace('sa_collision_math_','sa_collision_angle_')
 import collision_angle_reference as v
 import collision_math_reference as previous
 assert v.code==previous.code and v.namespace['reference'] is previous.namespace['reference']
 assert v.namespace['RETURNS']-previous.namespace['RETURNS']=={int(x,16) for x in returns['new_call_returns']}
 for name in ('REGISTER_ROWS','CONTEXT_ROWS','CD_FPU_ROWS','AI_FPU_ROWS','GUARDS','GUARD_ENTRY','PACKET_INTERNAL_ENTRY_ROWS'):assert v.namespace[name]==previous.namespace[name],name
 assert (P/'write_watch_collision_angle.py').read_text()==(P/'write_watch_collision_math.py').read_text().replace('collision_math_reference','collision_angle_reference')
 assert 'SA_BOUND_RETAIL_STALLS' not in (P/t['source']).read_text()
 bm=P/'analysis/collision_angle_previous_build_manifest.json';builds=json.loads(bm.read_text());assert len(builds['files'])==2721;files.append(bm)
 for row in builds['files']:assert digest(ROOT/row['path'])==row['sha256'] and (ROOT/row['path']).stat().st_size==row['bytes']
 registry=json.loads((P/'analysis/recovered_routines.json').read_text())['routines'][:1584];assert len(registry)==1584
 canonical=next(r for r in registry if r['address']=='0x8c10fbb8');body=next(r for r in t['routines'] if r['address']=='0x8c10fbb8');assert canonical['retail_span_sha256']==body['retail_span_sha256']
 report=dict(instruction_rows_byte_checked=223,new_instruction_rows_byte_checked=109,canonical_adapter_rows=114,canonical_duplicate_credit=0,prior_return_membership=2268,return_membership=len(after),new_resume_exports=3,explicit_nonfunction_exports_total=379,exact_previous_bridge_plus_five_bodies_and_actual_returns=True,reference_CPU_flat_execution_and_observer_identical=True,runtime_ABI_SRFD_exception_and_guard_policy_unchanged=True,new_source_wait_limits=0,preserved_archived_builds=2721,dll_sha256=digest(v.effects.DLL),sha256={str(p):digest(p) for p in files},scope='223 original rows bytechecked:109new/unregistered and114canonicalrange-reduction adapter rows,no duplicatecredit. Exactprevious244bridge plus5bodies/3actualcontinuations. CPU/flatcode/guards/observer/rawsession/ABIunchanged;nonewwaitlimits. Staticauditgivesnocredit;2721oldbuildspreserved.')
 (P/'analysis/collision_angle_audit.json').write_text(json.dumps(report,indent=2)+'\n');print('COLLISION ANGLE AUDIT PASS223rows',len(after),'returns2721priorbuilds',flush=True)
if __name__=='__main__':main()
