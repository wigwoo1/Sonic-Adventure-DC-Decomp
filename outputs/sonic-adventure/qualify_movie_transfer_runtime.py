"""Qualify continuous native transfer/IRQ integration without duplicate game credit."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 files=[Path(__file__)];reports={}
 for name in ('movie_transfer_runtime_connection','transfer_runtime_verification','movie_transfer_runtime_verification','movie_transfer_runtime_regression_verification','movie_transfer_runtime_compatibility_verification','movie_transfer_request_closure_verification','movie_transfer_wait_translation','movie_bulk_family_translation'):
  path=P/f'analysis/{name}.json';r=json.loads(path.read_text());reports[name]=r;files.append(path)
  for pinned,h in r['sha256'].items():assert digest(pinned)==h,pinned;files.append(Path(pinned))
 connected=reports['movie_transfer_runtime_connection'];device=reports['transfer_runtime_verification'];direct=reports['movie_transfer_runtime_verification'];reg=reports['movie_transfer_runtime_regression_verification'];compat=reports['movie_transfer_runtime_compatibility_verification'];closure=reports['movie_transfer_request_closure_verification']
 assert connected['compiled_source_count']==193 and connected['original_sources_changed'] is False
 assert device['cases_passed']==59 and device['newly_registered_entries']==0
 assert direct['lifecycles_passed']==16 and direct['continuous_requests_returned']==32 and direct['original_native_comparisons']==85 and direct['retained_wait_controls']==4 and direct['failure_controls']==3
 assert reg['total_comparisons']==596 and reg['previous_context_comparisons']==107 and compat['legacy_abi_comparisons']==6 and closure['cases_passed']==12
 dll=digest(ROOT/'work/sonic-adventure/build/movie_transfer_runtime.dll');assert all(r['dll_sha256']==dll for r in (device,direct,reg,compat,closure))
 definition=next(r for r in reports['movie_bulk_family_translation']['routines'] if r['address']=='0x8c65a5dc');rows=set(definition['reachable_addresses']);wanted=set(map(tuple,definition['conditional_edges']))
 seen=set().union(*(set(r['request_executed_rows']) for r in (direct,reg,closure)));edges=set().union(*(set(map(tuple,r['request_executed_branch_outcomes'])) for r in (direct,reg,closure)))
 request=dict(address=definition['address'],static_rows=len(rows),executed_rows=len(rows&seen),missing_rows=sorted(rows-seen),conditional_outcomes=len(wanted),executed_outcomes=len(wanted&edges),unverified_outcomes=[list(e) for e in sorted(wanted-edges)],additional_recovery_credit=0)
 assert request['static_rows']==request['executed_rows']==139 and not request['missing_rows']
 # Full own-row execution alone does not establish complete request behavior.
 assert request['unverified_outcomes']
 limitations=[
 'Existing recovered C bodies are unchanged. The native runtime is authored platform support and receives zero recovered-game credit. It captures actual CPU-triggered channel-register writes, copies real RAM/Sound bytes, updates start/enable state and derives pending status only after data movement. Four channels, both directions, cancellation, repeat, mask/ack and partial bus failures compare against an independent device oracle in59cases.',
 'Native scheduling occurs after the real666e02RTS/delay slot returns to65a6e8. Original wait instructions remain unchanged. IRQ6/INTEVT360 admission obeys BL/IMASK, then actual copied vector/context/ASIC/generic/registered handler/SDK/RTE executes inside the original native REQUEST call stack. The original caller resumes and returns. No guest checkpoint, completion flag, game-helper result or skipped original instruction is supplied.',
 'Sixteen explicit initialized lifecycles use original vector copying and event registration before two continuous requests. All32requests return, with changed repeated payload/destination offset, whole source/destination bytes and complete machine/boundary/runtime state checked. Four wait controls and three real missing-handler/vector/sound faults retain unfinished state. Separate initial setup and persistent device-state carry between ordinary ABI calls are explicit; this is not uninterrupted earliest startup.',
 'The request body executes all139own rows across current direct and regression proofs, but some conditional outcomes remain unverified. It stays partial and unregistered. Its research-only wait guard still stops after two continued tests when completion is unavailable; production retains the unbounded original wait. Partial bootstrapac000010 remains unresolved. Prior full-copy/video-clear/early-caller proofs are historical, not rerun.',
 'The platform is untimed and currently schedules at this particular original yield point. Physical IRQ/DMA timing, security-window enforcement, suspension/external triggering, continuous whole-game scheduling, ARM execution, audio and gameplay remain unfinished. All requested PC/Android/3DS/demake/FPS/8K/SUPIR/display/vertex-color TextureOff requirements remain unchanged. Overall10% is weighted milestones, not measured recovered-code coverage.'
 ]
 report=dict(newly_registered_entries=0,host_support_recovery_credit=0,compiled_source_count=193,continuous_requests_returned=32,continuous_lifecycles=16,direct_comparisons=85,regression_comparisons=596,legacy_abi_comparisons=6,request_rejection_comparisons=12,total_comparisons=699,authored_platform_cases=59,independent_direct_checks=direct['independent_checks'],regression_checks=reg['independent_and_record_checks'],partial_request=request,retained_default_frontier='0x8c65a6ec',partial_bootstrap_frontier='0xac000010',dll_sha256=dll,sha256={str(p):digest(p) for p in files},scope='Persistent authored native transfer/event service advances actual data and interrupts inside original REQUEST, which resumes and returns. All existing recovered sources and canonical entries preserved. Zero duplicate recovery credit; partial request and bootstrap claims remain explicitly bounded.',limitations=limitations)
 (P/'analysis/movie_transfer_runtime_qualification.json').write_text(json.dumps(report,indent=2)+'\n');print('Qualified699original/native comparisons,32continuous requests,59platform cases; no duplicate recovery credit');print('Partial request',request)
if __name__=='__main__':main()
