"""Register the retail-compared internal-name registration wrapper."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
binary = (ROOT.parent.parent / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
report = json.loads((ROOT / "analysis/startup_register_internal_name_verification.json").read_text(encoding="utf-8-sig"))
start, end = int(report["routine_start"], 16), int(report["routine_end_exclusive"], 16)
span_hash = hashlib.sha256(binary[start - 0x8c010000:end - 0x8c010000]).hexdigest()
assert span_hash == report["retail_span_sha256"]
routine = {
    "module": "1ST_READ.BIN", "name": "sa_startup_register_internal_name_8c0113da",
    "address": hex(start), "end_exclusive": hex(end),
    "retail_span_sha256": span_hash,
    "source": "source/startup_register_internal_name.c",
    "verification": "analysis/startup_register_internal_name_verification.json",
    "status": f"startup internal-name wrapper matched in {report['cases_passed']} full-state cases",
    "limitations": report["limitations"],
}
registry_path = ROOT / "analysis/recovered_routines.json"
registry = json.loads(registry_path.read_text(encoding="utf-8-sig"))
existing = next((entry for entry in registry["routines"]
                 if entry.get("module") == routine["module"] and
                 int(entry["address"], 16) == start), None)
if existing is None:
    registry["routines"].append(routine)
else:
    existing.update(routine)
registry_path.write_text(json.dumps(registry, indent=2) + "\n", encoding="utf-8")
status_path = ROOT / "project_status.json"
status = json.loads(status_path.read_text(encoding="utf-8-sig"))
status.setdefault("verified_progress", {})["startup_register_internal_name"] = {
    "verification": "analysis/startup_register_internal_name_verification.json",
    "cases": report["cases_passed"], "retail_span_sha256": span_hash,
    "scope": report["scope"], "limitations": report["limitations"],
}
status["verified_progress"]["recovered_buildable_routines"] = len(registry["routines"])
status_path.write_text(json.dumps(status, indent=2) + "\n", encoding="utf-8")
print(f"Registered startup internal-name wrapper; {len(registry['routines'])} routines in registry")
