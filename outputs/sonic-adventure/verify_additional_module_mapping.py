"""Verify staged binary hashes and retail load/filename literals without restaging."""
import hashlib,json,struct
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];OUT=ROOT/'outputs/sonic-adventure'
def main():
    binary=(ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN').read_bytes()
    rows=json.loads((OUT/'analysis/additional_mapped_modules.json').read_text())
    checks=[]
    for row in rows:
        actual=struct.unpack_from('<I',binary,int(row['load_address_literal'],16)-0x8c010000)[0]
        assert actual==int(row['load_address'],16),row['module']
        pointer=struct.unpack_from('<I',binary,int(row['filename_pointer_literal'],16)-0x8c010000)[0]
        offset=pointer-0x8c010000;end=binary.index(0,offset)
        filename=binary[offset:end].decode('ascii')
        assert filename.lower()==row['resource'].lower(),(filename,row['resource'])
        data=Path(row['staged_path']).read_bytes()
        assert hashlib.sha256(data).hexdigest()==row['decoded_sha256'],row['module']
        checks.append({'module':row['module'],'load_address':hex(actual),'retail_filename':filename,'staged_sha256':row['decoded_sha256']})
    report={'modules_checked':len(checks),'retail_checks_passed':len(checks)*2,'staged_hashes_passed':len(checks),'checks':checks,'scope':'Retail pointer/literal bytes and exact staged payload hashes. Does not prove runtime load reachability or executable/data boundaries.'}
    (OUT/'analysis/additional_module_mapping_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(f'{len(checks)} staged hashes and {len(checks)*2} retail mapping checks passed')
if __name__=='__main__':main()
