"""Register the retail-compared primary callback wrapper."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
binary = (ROOT.parent.parent / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
start, end = 0x8C64133E, 0x8C64135A
report_path = ROOT / "analysis/callback_register_primary_verification.json"
report = json.loads(report_path.read_text(encoding="utf-8-sig"))
routine = {
    "module": "1ST_READ.BIN", "name": "sa_callback_register_primary_8c64133e",
    "address": hex(start), "end_exclusive": hex(end),
    "retail_span_sha256": hashlib.sha256(binary[start - 0x8C010000:end - 0x8C010000]).hexdigest(),
    "source": "source/callback_install.c",
    "verification": "analysis/callback_register_primary_verification.json",
    "status": f"primary callback wrapper matches retail in {report['cases_passed']} cases; installer connected, zero-callback removal service modeled",
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
status.setdefault("verified_progress", {})["callback_register_primary"] = {
    "verification": routine["verification"], "cases": report["cases_passed"],
    "retail_instructions_executed": report["retail_instructions_executed"],
    "retail_span_sha256": routine["retail_span_sha256"],
    "scope": report["scope"], "limitations": report["limitations"],
}
status_path.write_text(json.dumps(status, indent=2) + "\n", encoding="utf-8")
print(json.dumps(routine, indent=2))
