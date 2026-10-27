"""Locate literal references to the original active-input interface global."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
binary=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=binary.read_bytes()
assert hashlib.sha256(raw).hexdigest()=='45bc5d3c25dcfe2cc2c5701311d46783f33e4daa44a951cd9896724d66f0367a'
target=0x8c89c54c;needle=target.to_bytes(4,'little');literals=[];start=0
while True:
    at=raw.find(needle,start)
    if at<0:break
    start=at+1
    if at%4==0:literals.append(at+0x8c010000)
rows=[(target,'active input interface pointer cell')]+[(a,'literal word pointing to 8c89c54c') for a in literals]
out=P/'analysis/movie-input-interface-references';out.mkdir(exist_ok=True)
targets=out/'targets.tsv';targets.write_text(''.join(f'{a:08x}\t{label}\n' for a,label in rows))
report=dict(target=hex(target),literal_words=list(map(hex,literals)),binary_sha256=hashlib.sha256(raw).hexdigest(),sha256={str(targets):hashlib.sha256(targets.read_bytes()).hexdigest()},scope='Aligned literal words only; Ghidra references and original instruction behavior must identify actual readers/writers. Literal occurrence alone is not function recovery or callback provenance.')
(out/'literal_evidence.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
