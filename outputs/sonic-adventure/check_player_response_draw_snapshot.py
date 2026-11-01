"""Limited publication checks; this does not qualify the renderer's full behavior."""
import ast,csv,hashlib,json,re,subprocess
from pathlib import Path

P=Path(__file__).resolve().parent
R=P.parents[1]

def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def main():
    target=P/'analysis/player_response_draw_snapshot_checks.json'
    assert not target.exists(),'Preserve the historical report; use an additive revision.'
    translation=P/'analysis/player_response_draw_translation.json'
    connection=P/'analysis/player_response_draw_connection.json'
    t=json.loads(translation.read_text());c=json.loads(connection.read_text())
    assert t['newly_registered_entries']==c['newly_registered_entries']==0
    assert c['compiled_source_count']==273
    for report in (t,c):
        for name,value in report['sha256'].items():assert sha(name)==value,name
    body=t['routines'][0]
    assert len(t['routines'])==1 and body['address']=='0x8c63ce24'
    listing=Path(body['listing'])
    rows=list(csv.DictReader(listing.open(),delimiter='\t'))
    retail=R/'work/sonic-adventure/extracted/1ST_READ.BIN'
    raw=retail.read_bytes()
    assert sha(retail)=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
    source=P/t['source'];code=source.read_text()
    delay_rows=0
    for i,row in enumerate(rows):
        a=int(row['address'],16);b=bytes.fromhex(row['bytes'])
        assert len(b)==int(row['length'])==2 and raw[a-0x8c010000:a-0x8c010000+2]==b
        if f'L{a:x}:' not in code:
            # The retained translator inlines branch delay slots in the owner.
            assert i and int(rows[i-1]['address'],16)+2==a
            assert rows[i-1]['instruction'].lstrip('_').startswith(('jsr ','bsr ','bra ','braf ','jmp ','bt/s ','bf/s ','rts'))
            delay_rows+=1
    assert len(rows)==body['instruction_rows']==684
    assert len(body['conditional_edges'])==32
    python_files=[P/n for n in ('recover_player_response_draw.py','connect_player_response_draw.py',
        'player_response_draw_reference.py','write_watch_player_response_draw.py','player_response_fixtures.py',Path(__file__).name)]
    for p in python_files:ast.parse(p.read_text(),filename=str(p))
    ps_files=[P/'build_player_response_draw.ps1',P/'ghidra/export_player_response_draw.ps1']
    for p in ps_files:
        command='$tokens=$null;$errors=$null;[System.Management.Automation.Language.Parser]::ParseFile($args[0],[ref]$tokens,[ref]$errors)|Out-Null;if($errors.Count){$errors|Out-String|Write-Error;exit 1}'
        # Pass the literal path as a PowerShell string, with apostrophes escaped.
        command=command.replace('$args[0]',"'"+str(p).replace("'","''")+"'")
        subprocess.run(['pwsh','-NoProfile','-Command',command],check=True,capture_output=True,text=True)
    import player_response_fixtures as q
    assert q.v.effects.DLL==R/'work/sonic-adventure/build/player_response_draw.dll'
    ram=q.v.seed_ram();cases=[];seen=set()
    inputs=[dict(flags=0,debug=0x80),
            dict(flags=1,collision_flags=0x88,velocity=(2,0,0),debug=0x80),
            dict(flags=1,collision_flags=0x88,velocity=(2,0,0),debug=0)]
    for item in inputs:
        m,s,f,cfg=q.prepare(ram,item)
        q.put(m,0x8c752bdc,item['debug'],1)
        result,path,m,s,f=q.fast.compare(m,s,f,cfg)
        assert result['full_state_match'] and result['ordered_boundaries_match']
        if item['debug']==0x80:assert result['status']==0,result
        seen.update(path)
        cases.append(dict(input=item,status=result['status'],stop_pc=result['stop_pc'],steps=len(path),
                          full_state_match=True,ordered_boundaries_match=True))
    covered=set(body['reachable_addresses'])&{hex(a) for a in seen}
    files=python_files+ps_files+[translation,connection,source,listing,
        P/'analysis/player_response_draw_returns.json',P/'source/sa_player_response_draw_connected.c',
        P/'source/player_response_draw_probe.c',q.v.effects.DLL,q.v.effects.DLL.with_suffix('.lib')]
    report=dict(status='limited_snapshot_checks_passed',original_rows_checked=684,inlined_delay_rows=delay_rows,static_conditional_outcomes=32,
        python_scripts_parsed=len(python_files),powershell_scripts_parsed=len(ps_files),compiled_sources=273,
        original_native_comparisons=len(cases),cases=cases,renderer_observed_rows=len(covered),
        full_renderer_behavior_qualified=False,new_registered_routines=0,supplied_game_callee_outputs=0,
        supplied_execution_frames=0,overall_completion_percent=10,
        scope='Publication smoke checks only. Actual original instruction bytes and non-delay instruction labels checked; '
              'delay rows are inlined by the retained translator and are not independently behavior-qualified by this static check. '
              'Three selected whole-state/ordered-boundary comparisons do not establish full renderer behavior, '
              'all branch outcomes, physical hardware accuracy, or a playable port. Matched software stops remain limitations.',
        sha256={str(p):sha(p) for p in files})
    target.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({k:v for k,v in report.items() if k not in ('sha256','cases')},indent=2),flush=True)
    print(json.dumps(cases,indent=2),flush=True)

if __name__=='__main__':main()
