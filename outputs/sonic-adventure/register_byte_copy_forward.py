"""Register the retail-compared forward byte-copy routine."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
binary = (ROOT.parent.parent / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
report_path = ROOT / "analysis/byte_copy_forward_verification.json"
report = json.loads(report_path.read_text(encoding="utf-8-sig"))
start, end = int(report["routine_start"], 16), int(report["routine_end_exclusive"], 16)
span_hash = hashlib.sha256(binary[start - 0x8C010000:end - 0x8C010000]).hexdigest()
assert span_hash == report["retail_span_sha256"]
routine = {
    "module": "1ST_READ.BIN", "name": "sa_byte_copy_forward_8c604500",
    "address": hex(start), "end_exclusive": hex(end),
    "retail_span_sha256": span_hash,
    "source": "source/byte_copy_forward.c",
    "verification": "analysis/byte_copy_forward_verification.json",
    "status": f"forward byte-copy behavior matched in {report['cases_passed']} nonzero-length cases",
}
registry_path = ROOT / "analysis/recovered_routines.json"
registry = json.loads(registry_path.read_text(encoding="utf-8-sig"))
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
status.setdefault("verified_progress", {})["byte_copy_forward"] = {
    "verification": "analysis/byte_copy_forward_verification.json",
    "cases": report["cases_passed"], "retail_instructions_executed": report["retail_instructions_executed"],
    "retail_span_sha256": span_hash, "scope": report["scope"],
    "limitations": report["limitations"],
}
status["verified_progress"]["recovered_buildable_routines"] = len(registry["routines"])
status_path.write_text(json.dumps(status, indent=2) + "\n", encoding="utf-8")
print(f"Registered forward byte-copy routine; {len(registry['routines'])} routines in registry")
