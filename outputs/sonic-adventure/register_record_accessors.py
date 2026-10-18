"""Register retail-compared fixed-offset record setters."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
binary = (ROOT.parent.parent / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
report_path = ROOT / "analysis/record_accessors_verification.json"
report = json.loads(report_path.read_text(encoding="utf-8-sig"))
registry_path = ROOT / "analysis/recovered_routines.json"
registry = json.loads(registry_path.read_text(encoding="utf-8-sig"))
for evidence in report["routines"]:
    start = int(evidence["routine_start"], 16)
    end = int(evidence["routine_end_exclusive"], 16)
    span_hash = hashlib.sha256(binary[start - 0x8C010000:end - 0x8C010000]).hexdigest()
    assert span_hash == evidence["retail_span_sha256"]
    routine = {
        "module": "1ST_READ.BIN", "name": evidence["name"],
        "address": hex(start), "end_exclusive": hex(end),
        "retail_span_sha256": span_hash,
        "source": "source/record_accessors.c",
        "verification": "analysis/record_accessors_verification.json",
        "status": f"retail record-write behavior matched in {evidence['cases_passed']} cases",
    }
    existing = next((entry for entry in registry["routines"]
                     if entry.get("module") == routine["module"]
                     and int(entry["address"], 16) == start), None)
    if existing is None:
        registry["routines"].append(routine)
    else:
        existing.update(routine)
registry_path.write_text(json.dumps(registry, indent=2) + "\n", encoding="utf-8")
status_path = ROOT / "project_status.json"
status = json.loads(status_path.read_text(encoding="utf-8-sig"))
status.setdefault("verified_progress", {})["record_accessors"] = {
    "verification": "analysis/record_accessors_verification.json",
    "cases": report["cases_passed"], "routines": report["routines"],
    "scope": report["scope"], "limitations": report["limitations"],
}
status["verified_progress"]["recovered_buildable_routines"] = len(registry["routines"])
status_path.write_text(json.dumps(status, indent=2) + "\n", encoding="utf-8")
print(f"Registered {len(report['routines'])} record accessors; {len(registry['routines'])} routines in registry")
