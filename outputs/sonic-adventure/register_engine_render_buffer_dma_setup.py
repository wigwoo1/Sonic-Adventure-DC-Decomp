"""Register the retail-verified renderer DMA setup routine."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
binary = (ROOT.parent.parent / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
start, end = 0x8C6534E0, 0x8C6535C2
begin, finish = start - 0x8C010000, end - 0x8C010000
report_path = ROOT / "analysis/engine_render_buffer_dma_setup_verification.json"
report = json.loads(report_path.read_text(encoding="utf-8-sig"))
routine = {
    "module": "1ST_READ.BIN",
    "name": "sa_engine_render_buffer_dma_setup",
    "address": hex(start),
    "end_exclusive": hex(end),
    "retail_span_sha256": hashlib.sha256(binary[begin:finish]).hexdigest(),
    "source": "source/sa_engine_render_buffer_dma_setup.c",
    "verification": "analysis/engine_render_buffer_dma_setup_verification.json",
    "status": f"native DMA/fallback and transfer-helper composition matches complete RAM/CPU/FPU/MMIO state in {report['cases_passed']} selected retail cases",
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
status.setdefault("verified_progress", {})["engine_render_buffer_dma_setup"] = {
    "verification": "analysis/engine_render_buffer_dma_setup_verification.json",
    "cases": report["cases_passed"],
    "retail_instructions_executed": report["original_instructions_executed"],
    "dma_submissions": report["dma_submissions"],
    "cache_writebacks": report["cache_writebacks"],
    "scope": report["scope"],
    "limitations": report["limitations"],
}
status_path.write_text(json.dumps(status, indent=2) + "\n", encoding="utf-8")
print(json.dumps(routine, indent=2))
