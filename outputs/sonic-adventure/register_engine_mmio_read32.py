"""Register the retail-verified host-adapted MMIO word reader."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
binary = (ROOT.parent.parent / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
start, end = 0x8C653476, 0x8C65347C
file_start, file_end = start - 0x8C010000, end - 0x8C010000
routine = {
    "module": "1ST_READ.BIN",
    "name": "sa_engine_mmio_read32",
    "address": hex(start),
    "end_exclusive": hex(end),
    "retail_span_sha256": hashlib.sha256(binary[file_start:file_end]).hexdigest(),
    "source": "source/sa_engine_mmio_read32.c",
    "verification": "analysis/engine_mmio_read32_verification.json",
    "status": "native C matches full RAM/CPU/FPU state and MMIO read boundary in 48 cases (144 retail instructions)",
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
verification = json.loads((ROOT / "analysis/engine_mmio_read32_verification.json").read_text(encoding="utf-8-sig"))
status.setdefault("verified_progress", {})["engine_mmio_read32"] = {
    "verification": "analysis/engine_mmio_read32_verification.json",
    "cases": verification["cases_passed"],
    "retail_instructions_executed": sum(case["retail_instructions_executed"] for case in verification["cases"]),
    "scope": "Complete RAM/CPU/FPU state and exact MMIO address/read value for twelve aligned, unaligned and high offsets with non-wrapping effective addresses.",
    "limitations": ["Device behavior is injected through a host callback; physical hardware and full-game integration remain unverified.", "Address-addition wraparound remains unverified due to a limitation in the shared reference interpreter."],
}
status_path.write_text(json.dumps(status, indent=2) + "\n", encoding="utf-8")
print(json.dumps(routine, indent=2))
