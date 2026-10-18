"""Record integrated retail verification for readiness query and table scan."""
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
report_path = ROOT / "analysis/engine_render_ready_query_integrated_verification.json"
report = json.loads(report_path.read_text(encoding="utf-8-sig"))
status_path = ROOT / "project_status.json"
status = json.loads(status_path.read_text(encoding="utf-8-sig"))
status.setdefault("verified_progress", {})["engine_render_ready_query_integrated"] = {
    "verification": "analysis/engine_render_ready_query_integrated_verification.json",
    "cases": report["cases_passed"],
    "retail_instructions_executed": report["original_instructions_executed"],
    "scope": report["scope"],
    "limitations": report["limitations"],
}
status_path.write_text(json.dumps(status, indent=2) + "\n", encoding="utf-8")
print(json.dumps(status["verified_progress"]["engine_render_ready_query_integrated"], indent=2))
