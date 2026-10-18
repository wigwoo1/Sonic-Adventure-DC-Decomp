"""Read historical IDA metadata; only exact byte matches may inform this revision."""
import hashlib, json, pathlib, sys
ROOT = pathlib.Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'work/sonic-adventure/toolchain/python-packages'))
import idb

def main():
    reference = ROOT / 'work/sonic-adventure/reference_idb/1ST_READ.idb'
    binary = (ROOT / 'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    functions = []; errors = []
    with idb.from_file(str(reference)) as database:
        api = idb.IDAPython(database)
        names = [{'address': hex(address), 'name': name} for address, name in api.idautils.Names()]
        for address in api.idautils.Functions():
            name = api.idc.GetFunctionName(address)
            try:
                end = api.idc.GetFunctionAttr(address, api.idc.FUNCATTR_END)
                payload = api.idc.GetManyBytes(address, end - address)
                offset = address - 0x8C010000
                exact = 0 <= offset and binary[offset:offset + len(payload)] == payload
                functions.append({'address': hex(address), 'end': hex(end), 'name': name,
                                  'size': len(payload), 'sha256': hashlib.sha256(payload).hexdigest(),
                                  'same_address_exact_byte_match': exact})
            except Exception as error:
                errors.append({'address': hex(address), 'name': name, 'error': str(error)})
    report = {'reference': str(reference), 'reference_sha256': hashlib.sha256(reference.read_bytes()).hexdigest(),
              'retail_sha256': hashlib.sha256(binary).hexdigest(), 'names': names, 'functions': functions,
              'errors': errors, 'note': 'No labels are imported automatically; byte identity alone does not verify a historical semantic label.'}
    destination = ROOT / 'outputs/sonic-adventure/analysis/reference_idb.json'
    destination.write_text(json.dumps(report, indent=2))
    print('Names:', len(names), 'Functions:', len(functions), 'Read errors:', len(errors),
          'Exact same-address matches:', sum(f['same_address_exact_byte_match'] for f in functions))

if __name__ == '__main__': main()
