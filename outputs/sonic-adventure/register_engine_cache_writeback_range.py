"""Register the retail-verified cache-writeback range wrapper."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
binary = (ROOT.parent.parent / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
start, end = 0x8C65344E, 0x8C653476
file_start, file_end = start - 0x8C010000, end - 0x8C010000
routine = {
    "module": "1ST_READ.BIN",
    "name": "sa_engine_cache_writeback_range",
    "address": hex(start),
    "end_exclusive": hex(end),
    "retail_span_sha256": hashlib.sha256(binary[file_start:file_end]).hexdigest(),
    "source": "source/sa_engine_cache_writeback_range.c",
    "verification": "analysis/engine_cache_writeback_range_verification.json",
    "status": "native C matches full RAM/CPU/FPU state and exact cache-writeback sequence in 48 retail comparisons",
}
path = ROOT / "analysis/recovered_routines.json"
registry = json.loads(path.read_text(encoding="utf-8-sig"))
existing = next((entry for entry in registry["routines"]
                 if entry.get("module") == routine["module"]
                 and int(entry["address"], 16) == start), None)
if existing is None:
    registry["routines"].append(routine)
else:
    existing.update(routine)
path.write_text(json.dumps(registry, indent=2) + "\n", encoding="utf-8")
status_path = ROOT / "project_status.json"
status = json.loads(status_path.read_text(encoding="utf-8-sig"))
verification = json.loads((ROOT / "analysis/engine_cache_writeback_range_verification.json").read_text(encoding="utf-8-sig"))
status.setdefault("verified_progress", {})["engine_cache_writeback_range"] = {
    "verification": "analysis/engine_cache_writeback_range_verification.json",
    "cases": 48,
    "retail_instructions_executed": verification["original_instructions_executed"],
    "scope": "Full RAM/CPU/FPU and cache writeback sequences for 12 ranges covering inclusive endpoints, unaligned starts, zero lengths and arithmetic wraparound.",
    "limitations": ["Cache maintenance operation is injected as a host callback; physical hardware/cache coherence and full-game integration remain unverified.", "Ranges that make the 32-byte stepping loop wrap while the end remains high can fail to terminate and are excluded."],
}
status_path.write_text(json.dumps(status, indent=2) + "\n", encoding="utf-8")
print(json.dumps(routine, indent=2))
