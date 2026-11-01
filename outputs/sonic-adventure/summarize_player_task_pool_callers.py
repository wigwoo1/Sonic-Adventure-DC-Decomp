"""Byte-check original owner exports; references are leads,not recovered routines."""
import collections,csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parents[1]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
    original=R/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=original.read_bytes()
    assert sha(original)=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
    folder=P/'analysis/player-task-pool-caller-trace';refs=list(csv.DictReader((folder/'references.tsv').open(),delimiter='\t'))
    summaries=[]
    for target in ('8c06c350','8c0730d8','8c0777e0','8c09844a','8c098a10'):
        selected=[x for x in refs if x['resource_address']==target]
        summaries.append(dict(target='0x'+target,indexed_reference_rows=len(selected),indexed_owners=sorted({x['function'] for x in selected if x['function']}),direct_calls=[x['reference_from'] for x in selected if x['reference_type'] in ('COMPUTED_CALL','UNCONDITIONAL_CALL')]))
    owners=P/'analysis/player-task-pool-owner-leads';diagnostics=list(csv.DictReader((owners/'diagnostics.tsv').open(),delimiter='\t'))
    assert len(diagnostics)==2 and all(x['completed']=='true' and not x['error'] for x in diagnostics)
    files=[Path(__file__),original,P/'ghidra/ExportResourceReferences.java',P/'ghidra/ExportFailedFunctionEvidence.java',P/'ghidra/export_player_task_pool_callers.ps1',P/'ghidra/export_player_task_pool_owner_leads.ps1',folder/'targets.tsv',folder/'references.tsv',owners/'diagnostics.tsv']
    count=0;listings={}
    for address in ('8c072b44','8c08f210'):
        listing=owners/(address+'_instructions.tsv');rows=list(csv.DictReader(listing.open(),delimiter='\t'));listings[address]={int(x['address'],16):x for x in rows}
        for row in rows:
            a=int(row['address'],16);data=bytes.fromhex(row['bytes']);assert len(data)==int(row['length'])==2 and raw[a-0x8c010000:a-0x8c010000+2]==data
        count+=len(rows);files.extend([listing,owners/(address+'_draft.c')])
    assert count==675
    for a in (0x8c072ff8,0x8c0730ae):assert listings['8c072b44'][a]['instruction'].lstrip('_')=='bsr 0x8c0730d8'
    for literal,target in ((0x8c08f318,0x8c098a10),(0x8c08f31c,0x8c09844a),(0x8c0b64e0,0x8c06c350),(0x8c0e0340,0x8c06c350)):
        assert int.from_bytes(raw[literal-0x8c010000:literal-0x8c010000+4],'little')==target
    result=dict(status='original_owner_exports_byte_checked;reference_leads_only',reference_rows=len(refs),targets=summaries,original_exported_rows_byte_checked=count,
                constructor_computed_call_sites_with_no_indexed_owner=['0x8c0b6316','0x8c0e01d4'],
                inspected_original_owners=['0x8c072b44','0x8c08f210'],
                next_recovery='Complete072b44 is an indexed1258-byte owner with two original calls to0730d8. Recover its full body and argument data flow. The constructor calls at0b6316/e01d4 need actual function-boundary discovery;do not assign them to a merely preceding indexed function.',
                limits='Ghidra indexed references are incomplete analysis leads,not a complete caller inventory. Computed-call reference ownership is missing at the two constructor sites. Drafts are not buildable/qualified implementations.08f210 calls real heap then pool initialization in its original prefix,but its full execution and normal boot/caller stack are unverified. No new routine,caller-alias,stack or gameplay credit.',
                new_original_routine_credit=0,overall_completion_percent=10,sha256={str(p):sha(p) for p in files})
    target=P/'analysis/player_task_pool_caller_leads.json';assert not target.exists();target.write_text(json.dumps(result,indent=2)+'\n')
    print('CALLER LEADS675original rows byte-checked;072b44->0730d8 at072ff8/0730ae;constructor ownership unresolved',flush=True)
if __name__=='__main__':main()
