"""Register the callback-contract-verified renderer readiness query."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
binary = (ROOT.parent.parent / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
start, end = 0x8C6533E0, 0x8C653404
begin, finish = start - 0x8C010000, end - 0x8C010000
report_path = ROOT / "analysis/engine_render_buffer_ready_query_verification.json"
report = json.loads(report_path.read_text(encoding="utf-8-sig"))
routine = {
    "module": "1ST_READ.BIN", "name": "sa_engine_render_buffer_ready_query",
    "address": hex(start), "end_exclusive": hex(end),
    "retail_span_sha256": hashlib.sha256(binary[begin:finish]).hexdigest(),
    "source": "source/sa_engine_render_buffer_ready_query.c",
    "verification": "analysis/engine_render_buffer_ready_query_verification.json",
    "status": f"native readiness mapping and exact callback boundary checked in {report['cases_passed']} wrapper cases; native table scanner composes with this wrapper and matches retail in the integrated verifier",
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
status.setdefault("verified_progress", {})["engine_render_buffer_ready_query"] = {
    "verification": "analysis/engine_render_buffer_ready_query_verification.json",
    "cases": report["cases_passed"], "scope": report["scope"],
    "limitations": [
        "The standalone wrapper test models 0x8c650f78 as a host callback.",
        "The linked helper is separately recovered and composed in analysis/engine_render_ready_query_integrated_verification.json.",
        "Complete graphics-engine integration remains unverified.",
    ],
}
status_path.write_text(json.dumps(status, indent=2) + "\n", encoding="utf-8")
print(json.dumps(routine, indent=2))
