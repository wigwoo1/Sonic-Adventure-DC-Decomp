"""Register the retail-compared callback registration wrappers for indices 3 and 5."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
binary = (ROOT.parent.parent / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
report_path = ROOT / "analysis/callback_register_index_wrappers_verification.json"
report = json.loads(report_path.read_text(encoding="utf-8-sig"))
by_name = {item["name"]: item for item in report["routines"]}
registry_path = ROOT / "analysis/recovered_routines.json"
registry = json.loads(registry_path.read_text(encoding="utf-8-sig"))
for name, start, end in (
        ("sa_callback_register_index3_8c6415f2", 0x8C6415F2, 0x8C64160E),
        ("sa_callback_register_index5_8c64162a", 0x8C64162A, 0x8C641646),
        ("sa_callback_register_index8_8c64167e", 0x8C64167E, 0x8C64169A)):
    evidence = by_name[name]
    span = binary[start - 0x8C010000:end - 0x8C010000]
    assert hashlib.sha256(span).hexdigest() == evidence["retail_span_sha256"]
    routine = {
        "module": "1ST_READ.BIN", "name": name, "address": hex(start),
        "end_exclusive": hex(end), "retail_span_sha256": evidence["retail_span_sha256"],
        "source": "source/callback_install.c",
        "verification": "analysis/callback_register_index_wrappers_verification.json",
        "status": f"index {evidence['index']} wrapper compared with retail in {evidence['cases_passed']} nonzero-callback cases",
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
status.setdefault("verified_progress", {})["callback_register_index_wrappers"] = {
    "verification": "analysis/callback_register_index_wrappers_verification.json",
    "cases": report["cases_passed"], "routines": report["routines"],
    "scope": report["scope"], "limitations": report["limitations"],
}
status["verified_progress"]["recovered_buildable_routines"] = len(registry["routines"])
status_path.write_text(json.dumps(status, indent=2) + "\n", encoding="utf-8")
print(f"Registered {len(report['routines'])} callback wrappers; {len(registry['routines'])} routines in registry")
