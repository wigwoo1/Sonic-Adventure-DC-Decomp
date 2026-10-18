"""Register two verified wrappers around the shared engine dispatcher."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
binary = (ROOT.parent.parent / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
routines = []
for start, end, symbol, report in [
    (0x8C652F56, 0x8C652F66, "sa_engine_dispatch_40_8c652f56",
     "analysis/engine_dispatch_8c652f56_verification.json"),
    (0x8C652D6E, 0x8C652D7E, "sa_engine_dispatch_b0_8c652d6e",
     "analysis/engine_dispatch_8c652d6e_verification.json"),
    (0x8C652E38, 0x8C652E50, "sa_engine_dispatch_b8_8c652e38",
     "analysis/engine_dispatch_8c652e38_verification.json"),
    (0x8C666968, 0x8C66697C, "sa_engine_dispatch_pair_8c666968",
     "analysis/engine_dispatch_8c666968_verification.json"),
]:
    file_start, file_end = start - 0x8C010000, end - 0x8C010000
    routines.append({
        "module": "1ST_READ.BIN",
        "name": symbol,
        "address": hex(start),
        "end_exclusive": hex(end),
        "retail_span_sha256": hashlib.sha256(binary[file_start:file_end]).hexdigest(),
        "source": "source/engine_dispatch_wrappers.c",
        "verification": report,
        "status": "native C matches full RAM/CPU/FPU state and dispatcher arguments in 24 retail-instruction comparisons; dispatcher body remains external",
    })

path = ROOT / "analysis/recovered_routines.json"
registry = json.loads(path.read_text(encoding="utf-8-sig"))
for routine in routines:
    existing = next((r for r in registry["routines"]
                     if r.get("module") == routine["module"]
                     and int(r["address"], 16) == int(routine["address"], 16)), None)
    if existing is None:
        registry["routines"].append(routine)
    else:
        existing.update(routine)
path.write_text(json.dumps(registry, indent=2) + "\n", encoding="utf-8")
print(json.dumps(routines, indent=2))
