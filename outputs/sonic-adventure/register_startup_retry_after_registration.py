"""Register the retail-compared post-registration retry routine."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
binary = (ROOT.parent.parent / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
report = json.loads((ROOT / "analysis/startup_retry_after_registration_verification.json").read_text(encoding="utf-8-sig"))
start, end = int(report["routine_start"], 16), int(report["routine_end_exclusive"], 16)
span_hash = hashlib.sha256(binary[start - 0x8c010000:end - 0x8c010000]).hexdigest()
assert span_hash == report["retail_span_sha256"]
routine = {
    "module": "1ST_READ.BIN", "name": "sa_startup_retry_after_registration_8c011524",
    "address": hex(start), "end_exclusive": hex(end),
    "retail_span_sha256": span_hash,
    "source": "source/startup_retry_after_registration.c",
    "verification": "analysis/startup_retry_after_registration_verification.json",
    "status": f"startup post-registration retry matched in {report['cases_passed']} full-state cases",
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
status.setdefault("verified_progress", {})["startup_retry_after_registration"] = {
    "verification": "analysis/startup_retry_after_registration_verification.json",
    "cases": report["cases_passed"], "retail_span_sha256": span_hash,
    "scope": report["scope"], "limitations": report["limitations"],
}
status["verified_progress"]["recovered_buildable_routines"] = len(registry["routines"])
status_path.write_text(json.dumps(status, indent=2) + "\n", encoding="utf-8")
print(f"Registered post-registration retry routine; {len(registry['routines'])} routines in registry")
