"""Check raw input provenance and preserve every legacy module path/base rule."""
import hashlib,json
from pathlib import Path
import audit_recovery_coverage as audit
P=Path(__file__).resolve().parent
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 inventory=P/'analysis/movie_runtime_inventory.json';q=json.loads(inventory.read_text());row={k:q[k] for k in ('retail_binary','retail_binary_sha256','load_address')}
 path,base=audit.retail_input(row,'MOVIE.BIN');assert base==0x8ceb0000 and path.stat().st_size==184592 and digest(path)==row['retail_binary_sha256'];positive=1
 bad=[dict(row,retail_binary='../MOVIE.BIN'),dict(row,retail_binary_sha256='0'*64),dict(row,load_address=None),{k:v for k,v in row.items() if k!='load_address'},{k:v for k,v in row.items() if k!='retail_binary_sha256'}];rejected=0
 for wrong in bad:
  try:audit.retail_input(wrong,'MOVIE.BIN')
  except (AssertionError,TypeError,ValueError):rejected+=1
  else:raise AssertionError('Invalid raw input provenance accepted')
 for module in ('1ST_READ.BIN','../MOVIE.BIN'):
  try:audit.retail_input(row,module)
  except AssertionError:rejected+=1
  else:raise AssertionError('Unapproved module identity accepted')
 registry=P/'analysis/recovered_routines.json';rows=json.loads(registry.read_text())['routines'];legacy=0;raw=0
 for r in rows:
  module=r.get('module','1ST_READ.BIN');path,base=audit.retail_input(r,module)
  if 'retail_binary' in r:assert module=='MOVIE.BIN' and base==0x8ceb0000;raw+=1;continue
  expected=audit.ROOT/('work/sonic-adventure/extracted/1ST_READ.BIN' if module=='1ST_READ.BIN' else f'work/sonic-adventure/decoded/SONICADV/{module}')
  assert path==expected and base==(0x8c010000 if module=='1ST_READ.BIN' else int(r.get('load_address','0x0c900000'),16)) and path.is_file();legacy+=1
 files=[Path(__file__),P/'audit_recovery_coverage.py',inventory]
 report=dict(raw_input_cases=positive,rejected_provenance_cases=rejected,legacy_registry_paths_checked=legacy,raw_registry_paths_checked=raw,sha256={str(p):digest(p) for p in files},scope='Observed raw MOVIE hash/base accepted, wrong/missing provenance and unapproved module identities rejected, all legacy module path/base resolutions retained. Registry shape is inspected but not pinned here, avoiding a registration/proof hash cycle.')
 (P/'analysis/movie_module_input_verification.json').write_text(json.dumps(report,indent=2)+'\n');print('Raw input accepted;',rejected,'bad inputs rejected;',legacy,'legacy path/base rules unchanged')
if __name__=='__main__':main()
