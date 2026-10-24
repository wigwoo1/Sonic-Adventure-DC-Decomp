"""Audit registry evidence and indexed candidates without claiming whole-game coverage."""
import csv,hashlib,json,re
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
OUT=ROOT/'outputs/sonic-adventure'
def read_json(path):return json.loads(path.read_text(encoding='utf-8-sig'))
def sha(data):return hashlib.sha256(data).hexdigest()
def retail_input(routine,module):
    default='work/sonic-adventure/extracted/1ST_READ.BIN' if module=='1ST_READ.BIN' else f'work/sonic-adventure/decoded/SONICADV/{module}'
    selected=routine.get('retail_binary',default)
    if 'retail_binary' in routine:
        assert re.fullmatch(r'[A-Z0-9_]+\.BIN',module),module
        allowed=f'work/sonic-adventure/extracted/files/SONICADV/{module}'
        assert module!='1ST_READ.BIN' and selected==allowed,selected
        assert routine.get('retail_binary_sha256') and 'load_address' in routine
    path=ROOT/selected
    if routine.get('retail_binary_sha256'):assert sha(path.read_bytes())==routine['retail_binary_sha256'],selected
    base=0x8c010000 if module=='1ST_READ.BIN' else int(routine.get('load_address','0x0c900000'),16)
    return path,base

def main():
    registry=read_json(OUT/'analysis/recovered_routines.json')
    fixes={'sa_prs_decompress_8c04c5e0':'source/prs_decompress.c','sa_load_compressed_module_8c09da44':'source/module_loader.c'}
    evidence=[];registered={};registry_groups={};errors=[];stale=[]
    for routine in registry['routines']:
        module=routine.setdefault('module','1ST_READ.BIN')
        if not routine.get('source') and routine['name'] in fixes:routine['source']=fixes[routine['name']]
        address=int(routine['address'],16);end=int(routine['end_exclusive'],16)
        binary_path,base=retail_input(routine,module)
        data=binary_path.read_bytes();span=data[address-base:end-base]
        expected=routine.get('retail_span_sha256',routine.get('instruction_sha256'))
        span_matches=len(span)==end-address and sha(span)==expected
        source=OUT/routine.get('source','missing_source')
        verification=OUT/routine['verification'];report=read_json(verification)
        source_exists=source.is_file()
        source_current=sha(source.read_bytes()) if source_exists else None
        report_hash=report.get('source_sha256')
        if not report_hash:
            scoped_hashes={entry.get('source_sha256') for entry in report.get('routines',[])
                           if isinstance(entry,dict) and entry.get('source')==routine.get('source')
                           and entry.get('source_sha256')}
            if len(scoped_hashes)==1:report_hash=scoped_hashes.pop()
        if report_hash and report_hash!=source_current:stale.append({'module':module,'address':routine['address'],'source':routine.get('source'),'verification':routine['verification'],'reason':'Whole source-file hash changed since report; routine-specific scope must be revalidated, not inferred from an old report.'})
        row={'module':module,'address':f'{address:08x}','name':routine['name'],'source':routine.get('source'),'verification':routine['verification'],'retail_span_matches':span_matches,'source_exists':source_exists,'source_contains_name':source_exists and routine['name'] in source.read_text(),'current_source_sha256':source_current,'report_whole_source_hash_current':None if not report_hash else report_hash==source_current,'span_bytes_including_possible_literal_pools':end-address}
        evidence.append(row);registered[(module,address)]=row
        registry_groups.setdefault((module,address),[]).append(row)
        if not span_matches or not row['source_contains_name']:errors.append(row)
    indexes=[('1ST_READ.BIN',OUT/'analysis/ghidra-all/function_index.tsv')]
    for group in ['overlays','common-overlays','additional-modules']:
        indexes.extend((path.parent.name,path) for path in sorted((OUT/'analysis'/group).glob('*/function_index.tsv')))
    movie_index=OUT/'analysis/movie-runtime/function_index.tsv'
    if movie_index.is_file():indexes.append(('MOVIE.BIN',movie_index))
    candidates=[];modules=[];seen=set()
    for module,path in indexes:
        rows=list(csv.DictReader(path.open(encoding='utf-8-sig'),delimiter='\t'))
        matched=0
        for row in rows:
            address=int(row['address'],16);key=(module,address)
            if key in seen:continue
            seen.add(key);recovered=registered.get(key);matched+=bool(recovered)
            candidates.append({'module':module,'address':f'{address:08x}','name':row['name'],'indexed_body_bytes':row['body_bytes'],'state':'registered_source_with_limited_verification' if recovered else 'unrecovered_candidate','source':recovered['source'] if recovered else ''})
        modules.append({'module':module,'index':str(path.relative_to(OUT)).replace('\\','/'),'indexed_candidates':len(rows),'registered_entries_matching_snapshot':matched})
    scope=read_json(OUT/'analysis/module_scope.json')
    classified={module for module,_ in indexes if module!='1ST_READ.BIN'}
    unresolved=[]
    for item in scope['unclassified_binaries']:
        module=Path(item['source']).stem.upper()+'.BIN'
        unresolved.append({'module':module,'decoded_bytes':item['decoded_bytes'],'has_function_index':module in classified,'code_data_boundary_complete':False})
    duplicate_groups=[{'module':key[0],'address':f'0x{key[1]:08x}','entries':[{'name':row['name'],'source':row['source']} for row in rows]} for key,rows in registry_groups.items() if len(rows)>1]
    report={'registered_routines':len(evidence),'registered_unique_module_address_pairs':len(registered),'duplicate_registry_address_groups':duplicate_groups,'registry_evidence_errors':errors,'whole_source_hash_refresh_needed':stale,'indexed_candidates_in_export_snapshots':len(candidates),'registered_entries_matching_export_snapshots':sum(bool(registered.get(key)) for key in seen),'registered_entries_missing_from_snapshots':[r for key,r in registered.items() if key not in seen],'module_indexes':modules,'non_texture_decoded_binaries_awaiting_complete_boundary_analysis':len(unresolved),'non_texture_binaries_without_function_index':sum(not r['has_function_index'] for r in unresolved),'whole_game_decompilation_complete':False,'limitations':['Function indexes are candidate snapshots, not exhaustive executable-code inventories.','Span sizes may include literal pools and are not recovered-code byte counts.','Registration and limited differential tests do not prove full routine ABI coverage or playable behavior.','No whole-game completion percentage is inferred.']}
    (OUT/'analysis/recovery_coverage_audit.json').write_text(json.dumps(report,indent=2)+'\n')
    (OUT/'analysis/recovery_registry_evidence.json').write_text(json.dumps(evidence,indent=2)+'\n')
    (OUT/'analysis/unresolved_module_boundaries.json').write_text(json.dumps(unresolved,indent=2)+'\n')
    with (OUT/'analysis/unrecovered_function_candidates.tsv').open('w',newline='') as file:
        writer=csv.DictWriter(file,fieldnames=list(candidates[0]),delimiter='\t');writer.writeheader();writer.writerows(candidates)
    (OUT/'analysis/recovered_routines.json').write_text(json.dumps(registry,indent=2)+'\n')
    status_path=OUT/'project_status.json'
    status=read_json(status_path)
    status['recovery_coverage_audit']={
        'report':'analysis/recovery_coverage_audit.json',
        'candidate_inventory':'analysis/unrecovered_function_candidates.tsv',
        'boundary_inventory':'analysis/unresolved_module_boundaries.json',
        'registered_routines':report['registered_routines'],
        'registered_unique_module_address_pairs':report['registered_unique_module_address_pairs'],
        'indexed_candidates_in_export_snapshots':report['indexed_candidates_in_export_snapshots'],
        'registered_entries_matching_export_snapshots':report['registered_entries_matching_export_snapshots'],
        'non_texture_binaries_without_function_index':report['non_texture_binaries_without_function_index'],
        'registry_evidence_errors':len(errors),
        'stale_whole_source_hash_reports':len(stale),
        'limitation':'Candidate export snapshots do not establish exhaustive game-code boundaries or a completion percentage'
    }
    status.setdefault('verified_progress',{})['recovered_buildable_routines']=len(registry['routines'])
    status_path.write_text(json.dumps(status,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({k:report[k] for k in ['registered_routines','registered_unique_module_address_pairs','indexed_candidates_in_export_snapshots','registered_entries_matching_export_snapshots','non_texture_binaries_without_function_index']}))
    print(f'{len(errors)} registry evidence errors; {len(stale)} reports need whole-source hash refresh')
    if errors:raise SystemExit(1)
if __name__=='__main__':main()
