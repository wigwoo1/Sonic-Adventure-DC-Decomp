"""Register the retail-compared object accessor routines."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
binary = (ROOT.parent.parent / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
report_path = ROOT / "analysis/object_accessors_verification.json"
report = json.loads(report_path.read_text(encoding="utf-8-sig"))
by_name = {item["name"]: item for item in report["routines"]}
registry_path = ROOT / "analysis/recovered_routines.json"
registry = json.loads(registry_path.read_text(encoding="utf-8-sig"))
for name, start, end in (
        ("sa_object_byte_at_1_get_8c644500", 0x8C644500, 0x8C644504),
        ("sa_object_field38_set_8c644602", 0x8C644602, 0x8C644606)):
    evidence = by_name[name]
    span = binary[start - 0x8C010000:end - 0x8C010000]
    span_hash = hashlib.sha256(span).hexdigest()
    assert span_hash == evidence["retail_span_sha256"]
    routine = {
        "module": "1ST_READ.BIN", "name": name, "address": hex(start),
        "end_exclusive": hex(end), "retail_span_sha256": span_hash,
        "source": "source/object_accessors.c",
        "verification": "analysis/object_accessors_verification.json",
        "status": f"retail behavior matched in {evidence['cases_passed']} cases",
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
status.setdefault("verified_progress", {})["object_accessors"] = {
    "verification": "analysis/object_accessors_verification.json",
    "cases": report["cases_passed"], "routines": report["routines"],
    "scope": report["scope"], "limitations": report["limitations"],
}
status["verified_progress"]["recovered_buildable_routines"] = len(registry["routines"])
status_path.write_text(json.dumps(status, indent=2) + "\n", encoding="utf-8")
print(f"Registered {len(report['routines'])} object accessors; {len(registry['routines'])} routines in registry")
