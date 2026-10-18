"""Register the retail-verified engine callback flag-reset routine."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
binary = (ROOT.parent.parent / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
start, end = 0x8C651368, 0x8C651384
file_start, file_end = start - 0x8C010000, end - 0x8C010000
routine = {
    "module": "1ST_READ.BIN",
    "name": "sa_engine_callback_flag_reset_8c651368",
    "address": hex(start),
    "end_exclusive": hex(end),
    "retail_span_sha256": hashlib.sha256(binary[file_start:file_end]).hexdigest(),
    "source": "source/engine_callback_flag_reset.c",
    "verification": "analysis/engine_callback_flag_reset_verification.json",
    "status": "native C matches full RAM/CPU/FPU state in 32 retail-instruction comparisons across both branches",
}
path = ROOT / "analysis/recovered_routines.json"
registry = json.loads(path.read_text(encoding="utf-8-sig"))
existing = next((r for r in registry["routines"]
                 if r.get("module") == routine["module"]
                 and int(r["address"], 16) == start), None)
if existing is None:
    registry["routines"].append(routine)
else:
    existing.update(routine)
path.write_text(json.dumps(registry, indent=2) + "\n", encoding="utf-8")
print(json.dumps(routine, indent=2))
