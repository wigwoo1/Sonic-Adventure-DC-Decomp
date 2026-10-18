"""Register the retail-verified engine option dispatch wrapper."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
binary = (ROOT.parent.parent / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
start, end = 0x8C652E80, 0x8C652EA0
file_start, file_end = start - 0x8C010000, end - 0x8C010000
routine = {
    "module": "1ST_READ.BIN",
    "name": "sa_engine_dispatch_option",
    "address": hex(start),
    "end_exclusive": hex(end),
    "retail_span_sha256": hashlib.sha256(binary[file_start:file_end]).hexdigest(),
    "source": "source/sa_engine_dispatch_option.c",
    "verification": "analysis/engine_dispatch_option_verification.json",
    "status": "native C matches full RAM/CPU/FPU state and dispatcher boundary in 80 retail-instruction comparisons",
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
status.setdefault("verified_progress", {})["engine_dispatch_option"] = {
    "verification": "analysis/engine_dispatch_option_verification.json",
    "cases": 80,
    "retail_instructions_executed": 992,
    "scope": "Complete 16 MiB RAM, CPU registers, SR/PR, both FPU banks, FPUL/MACL preservation, dispatcher target and boundary state; values 0..3 and out-of-range values across two FPU banks and two memory patterns.",
    "limitations": ["The dispatcher at 0x8c653444 is modeled at the call boundary.",
                    "Function-level equivalence does not establish complete game integration."],
}
status_path.write_text(json.dumps(status, indent=2) + "\n", encoding="utf-8")
print(json.dumps(routine, indent=2))
