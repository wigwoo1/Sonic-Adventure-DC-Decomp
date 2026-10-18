"""Register the retail-verified 1ST_READ.BIN ready-flag routine."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
binary = (ROOT.parent.parent / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
routine = {
    "module": "1ST_READ.BIN",
    "name": "sa_main_ready_flag_8c04c680",
    "address": "0x8c04c680",
    "end_exclusive": "0x8c04c688",
    "retail_span_sha256": hashlib.sha256(binary[0x3C680:0x3C688]).hexdigest(),
    "source": "source/main_ready_flag.c",
    "verification": "analysis/main_ready_flag_verification.json",
    "status": "native C source matches full RAM and general-register effects in 16 randomized retail-instruction comparisons",
}
path = ROOT / "analysis/recovered_routines.json"
registry = json.loads(path.read_text(encoding="utf-8-sig"))
existing = next((r for r in registry["routines"]
                 if r.get("module") == routine["module"]
                 and int(r["address"], 16) == 0x8C04C680), None)
if existing is None:
    registry["routines"].append(routine)
else:
    existing.update(routine)
path.write_text(json.dumps(registry, indent=2) + "\n", encoding="utf-8")
print(json.dumps(routine, indent=2))
