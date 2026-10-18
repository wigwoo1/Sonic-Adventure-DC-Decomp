"""Require completed import/export artifacts, then summarize candidate analysis."""
import csv,hashlib,json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];OUT=ROOT/'outputs/sonic-adventure'
def main():
    inputs=json.loads((OUT/'analysis/additional_mapped_modules.json').read_text())
    rows=[];logs={}
    for item in inputs:
        directory=OUT/'analysis/additional-modules'/item['module']
        index=directory/'function_index.tsv';code=directory/'initial_pseudocode.c'
        assert index.is_file() and code.is_file(),f"Missing completed export: {item['module']}"
        base=int(item['load_address'],16)
        log=ROOT/f'work/sonic-adventure/ghidra-additional-{base:08x}.log'
        text=log.read_text(encoding='utf-8',errors='replace')
        assert f"/{item['module']}: file created" in text,f"Import completion missing: {item['module']}"
        logs[base]=text
        functions=list(csv.DictReader(index.open(),delimiter='\t'))
        counts={}
        for function in functions:counts[function['decompilation_status']]=counts.get(function['decompilation_status'],0)+1
        rows.append({'module':item['module'],'load_address':item['load_address'],'sha256':item['decoded_sha256'],'candidate_functions':len(functions),'export_status_counts':counts,'function_index_sha256':hashlib.sha256(index.read_bytes()).hexdigest(),'draft_pseudocode_sha256':hashlib.sha256(code.read_bytes()).hexdigest(),'classification':'Candidate analysis only; executable/data boundaries and recovered source remain incomplete'})
    assert sum(text.count('REPORT: Import succeeded') for text in logs.values())>=len(inputs)
    report={'modules_processed':len(rows),'candidate_functions':sum(row['candidate_functions'] for row in rows),'empty_function_indexes':[row['module'] for row in rows if not row['candidate_functions']],'draft_export_failures':sum(row['export_status_counts'].get('failed',0) for row in rows),'modules':rows,'limitations':['Ghidra pseudocode is unverified and not buildable native source.','An empty candidate index does not prove absence of executable code.','Load addresses have static retail evidence; runtime reachability remains unverified.']}
    (OUT/'analysis/additional-modules/analysis_summary.json').write_text(json.dumps(report,indent=2)+'\n')
    print(f"{report['modules_processed']} modules, {report['candidate_functions']} function candidates, {report['draft_export_failures']} draft export failures")
if __name__=='__main__':main()
