"""Register the fully compared startup callback/flag-reset helper."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
binary = (ROOT.parent.parent / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
report_path = ROOT / "analysis/startup_callback_flag_reset_8c107aaa_verification.json"
report = json.loads(report_path.read_text(encoding="utf-8-sig"))
start, end = int(report["routine_start"], 16), int(report["routine_end_exclusive"], 16)
span_hash = hashlib.sha256(binary[start - 0x8c010000:end - 0x8c010000]).hexdigest()
assert span_hash == report["retail_span_sha256"]
assert report["cases_passed"] == 48 and report["covered_rows"] == report["instruction_rows"] == 14
source_path = ROOT / "source/startup_callback_flag_reset_8c107aaa.c"
assert hashlib.sha256(source_path.read_bytes()).hexdigest() == report["source_sha256"]
routine = {
    "module": "1ST_READ.BIN",
    "name": "sa_startup_callback_flag_reset_8c107aaa",
    "address": hex(start), "end_exclusive": hex(end),
    "retail_span_sha256": span_hash,
    "source": "source/startup_callback_flag_reset_8c107aaa.c",
    "verification": "analysis/startup_callback_flag_reset_8c107aaa_verification.json",
    "status": "14-row callback gate and flag reset matched in 48 full-state cases",
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
status.setdefault("verified_progress", {})["startup_callback_flag_reset_8c107aaa"] = {
    "verification": "analysis/startup_callback_flag_reset_8c107aaa_verification.json",
    "cases": report["cases_passed"], "retail_span_sha256": span_hash,
    "scope": report["scope"], "limitations": report["limitations"],
}
status["verified_progress"]["recovered_buildable_routines"] = len(registry["routines"])
status_path.write_text(json.dumps(status, indent=2) + "\n", encoding="utf-8")
print(f"Registered startup callback flag reset; {len(registry['routines'])} registry entries")
