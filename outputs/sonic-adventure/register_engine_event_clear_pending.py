"""Register the retail-compared event pending-clear handler."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
binary = (ROOT.parent.parent / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
start, end = 0x8C6521F2, 0x8C652210
begin, finish = start - 0x8C010000, end - 0x8C010000
report_path = ROOT / "analysis/engine_event_clear_pending_verification.json"
report = json.loads(report_path.read_text(encoding="utf-8-sig"))
routine = {
    "module": "1ST_READ.BIN", "name": "sa_engine_event_clear_pending",
    "address": hex(start), "end_exclusive": hex(end),
    "retail_span_sha256": hashlib.sha256(binary[begin:finish]).hexdigest(),
    "source": "source/sa_engine_event_clear_pending.c",
    "verification": "analysis/engine_event_clear_pending_verification.json",
    "status": f"full RAM/CPU/FPU/SR/PR/MMIO behavior matches retail in {report['cases_passed']} cases; MMIO uses a host callback",
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
status.setdefault("verified_progress", {})["engine_event_clear_pending"] = {
    "verification": "analysis/engine_event_clear_pending_verification.json",
    "cases": report["cases_passed"],
    "retail_instructions_executed": report["original_instructions_executed"],
    "retail_span_sha256": routine["retail_span_sha256"],
    "scope": report["scope"], "limitations": report["limitations"],
}
status_path.write_text(json.dumps(status, indent=2) + "\n", encoding="utf-8")
print(json.dumps(routine, indent=2))
