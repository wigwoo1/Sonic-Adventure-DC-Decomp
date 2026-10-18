"""Audit provenance and classify analyzed FPSCR changes; no reachability claim."""
import collections,csv,hashlib,json,pathlib
ROOT=pathlib.Path(__file__).resolve().parents[2]
def main():
    output=ROOT/'outputs/sonic-adventure';inventory=output/'analysis/fpu_mode_writers.tsv'
    rows=list(csv.DictReader(inventory.open(),delimiter='\t'))
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    functions=collections.defaultdict(list)
    for row in rows:
        address=int(row['address'],16);offset=address-0x8c010000
        assert int.from_bytes(binary[offset:offset+2],'little')==int(row['opcode'],16),row
        functions[row['function']].append(row)
    report={'inventory_sha256':hashlib.sha256(inventory.read_bytes()).hexdigest(),
            'binary_sha256':hashlib.sha256(binary).hexdigest(),'instruction_matches':len(rows),
            'assigned_function_matches':sum(row['function']!='unassigned' for row in rows),
            'unassigned_matches':sum(row['function']=='unassigned' for row in rows),
            'opcode_counts':dict(collections.Counter(row['opcode'] for row in rows)),
            'explicit_loads':[row for row in rows if row['opcode'].startswith('4')],
            'functions_with_changes':[{ 'function':name,'writer_count':len(items),
                'size_toggles':sum(item['opcode']=='f3fd' for item in items),
                'bank_toggles':sum(item['opcode']=='fbfd' for item in items),
                'has_explicit_load':any(item['opcode'].startswith('4') for item in items)}
                for name,items in functions.items() if name!='unassigned'],
            'processor_model_limits':[
                'Bundled Ghidra SuperH4.sinc explicitly does not simulate FPSCR.FR bank selection.',
                'Bundled definition ignores many floating-point error conditions.',
                'Analyzed instruction matches may include mistakenly disassembled data.',
                'An even number of toggles does not prove balanced paths or a known entry mode.'],
            'cleanup_targets':[
                {'routine':'0x8c04b48c','clear_address_offset':60,'opcode':'fe37','single_source':'FR3','double_source':'XD2'},
                {'routine':'0x8c04b82e','clear_address_offset':72,'opcode':'f337','single_source':'FR3','double_source':'XD2'}],
            'next_required_evidence':[
                'Establish caller entry FPSCR, including SZ/FR/PR and valid-instruction conditions.',
                'Verify alternate-bank register pairing and little-endian store order against SH-4 primary documentation.',
                'Test the actual cleanup instructions with bank-aware transfer semantics; retain unresolved mode evidence.'],
            'full_game_float_behavior_verified':False}
    (output/'analysis/fpu_mode_audit.json').write_text(json.dumps(report,indent=2))
    print(f"{len(rows)} retail opcodes checked; {report['assigned_function_matches']} assigned, {report['unassigned_matches']} unassigned")
if __name__=='__main__':main()
