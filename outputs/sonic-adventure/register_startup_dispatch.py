"""Register both retail-compared startup tail-dispatch wrappers."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
binary = (ROOT.parent.parent / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
report = json.loads((ROOT / "analysis/startup_dispatch_verification.json").read_text(encoding="utf-8-sig"))
registry_path = ROOT / "analysis/recovered_routines.json"
registry = json.loads(registry_path.read_text(encoding="utf-8-sig"))
status_path = ROOT / "project_status.json"
status = json.loads(status_path.read_text(encoding="utf-8-sig"))

for item in report["routine_ranges"]:
    start, end = int(item["start"], 16), int(item["end_exclusive"], 16)
    span_hash = hashlib.sha256(binary[start - 0x8c010000:end - 0x8c010000]).hexdigest()
    symbol = item["symbol"]
    key = f"startup_dispatch_{start:08x}"
    routine = {
        "module": "1ST_READ.BIN", "name": symbol,
        "address": hex(start), "end_exclusive": hex(end),
        "retail_span_sha256": span_hash, "source": "source/startup_dispatch.c",
        "verification": "analysis/startup_dispatch_verification.json",
        "status": f"tail dispatch to {item['target']} matched in 48 cases",
    }
    existing = next((entry for entry in registry["routines"]
                     if entry.get("module") == routine["module"]
                     and int(entry["address"], 16) == start), None)
    if existing is None:
        registry["routines"].append(routine)
    else:
        existing.update(routine)
    status.setdefault("verified_progress", {})[key] = {
        "verification": "analysis/startup_dispatch_verification.json",
        "cases": 48, "retail_span_sha256": span_hash,
        "scope": report["scope"], "limitations": report["limitations"],
    }

registry_path.write_text(json.dumps(registry, indent=2) + "\n", encoding="utf-8")
status["verified_progress"]["recovered_buildable_routines"] = len(registry["routines"])
status_path.write_text(json.dumps(status, indent=2) + "\n", encoding="utf-8")
print(f"Registered startup tail-dispatch wrappers; {len(registry['routines'])} routines in registry")
