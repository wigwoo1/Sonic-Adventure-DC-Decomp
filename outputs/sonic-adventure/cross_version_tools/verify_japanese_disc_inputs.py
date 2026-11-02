"""Hash user-provided disc inputs without changing any input file."""
import hashlib
import json
from pathlib import Path

R = Path(__file__).resolve().parents[2]
W = R / 'work/sonic-adventure'
disc = Path(r'F:\AI Games\Sonic Adventure v1.007 (1998)(Sega)(JP)[!][H1C11A, H1C11E]')
descriptors = list(disc.glob('*.gdi'))
assert len(descriptors) == 1
descriptor = descriptors[0]
lines = descriptor.read_text().splitlines()
assert lines[0] == '3' and len(lines) == 4
tracks = [line.split()[4] for line in lines[1:]]
assert tracks == ['track01.bin', 'track02.raw', 'track03.bin']
files = []
for path in [descriptor, *(disc / name for name in tracks)]:
    before = path.stat()
    h = hashlib.sha256()
    with path.open('rb') as stream:
        for block in iter(lambda: stream.read(4 * 1024 * 1024), b''):
            h.update(block)
    after = path.stat()
    assert (before.st_size, before.st_mtime_ns) == (after.st_size, after.st_mtime_ns)
    files.append(dict(path=str(path), bytes=after.st_size,
                      mtime_ns=after.st_mtime_ns, sha256=h.hexdigest()))
result = dict(status='user_provided_GDI_inputs_hashed_before_build',
              descriptor=str(descriptor), files=files,
              original_inputs_modified=False, retail_inputs_private=True,
              revision_checksum_verification_pending=True,
              overall_completion_percent=10, goal_status='active')
target = W / 'japanese_v1007_disc_input_manifest.json'
assert not target.exists()
target.write_text(json.dumps(result, indent=2) + '\n')
print(json.dumps(dict(files=len(files), bytes=sum(x['bytes'] for x in files),
                      manifest=str(target)), indent=2))
