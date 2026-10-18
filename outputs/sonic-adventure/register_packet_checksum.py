"""Register the two retail-compared packet checksum routines."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent
binary = (ROOT.parent.parent / "work/sonic-adventure/extracted/1ST_READ.BIN").read_bytes()
report_path = ROOT / "analysis/packet_checksum_verification.json"
report = json.loads(report_path.read_text(encoding="utf-8-sig"))
registry_path = ROOT / "analysis/recovered_routines.json"
registry = json.loads(registry_path.read_text(encoding="utf-8-sig"))
for evidence in report["routines"]:
    start, end = int(evidence["routine_start"], 16), int(evidence["routine_end_exclusive"], 16)
    span_hash = hashlib.sha256(binary[start - 0x8C010000:end - 0x8C010000]).hexdigest()
    assert span_hash == evidence["retail_span_sha256"]
    symbol = {
        "crc16": "sa_crc16_packet_8c64bda0",
        "packet_validator": "sa_packet_validate_8c603cd2",
        "packet_buffer_check": "sa_packet_buffer_check_8c603f8c",
    }[evidence["name"]]
    routine = {
        "module": "1ST_READ.BIN", "name": symbol,
        "address": hex(start), "end_exclusive": hex(end),
        "retail_span_sha256": span_hash,
        "source": "source/packet_checksum.c",
        "verification": "analysis/packet_checksum_verification.json",
        "status": f"bounded {evidence['name']} behavior matched in {evidence['cases_passed']} cases",
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
status.setdefault("verified_progress", {})["packet_checksum"] = {
    "verification": "analysis/packet_checksum_verification.json",
    "routines": report["routines"], "limitations": report["limitations"],
}
status["verified_progress"]["recovered_buildable_routines"] = len(registry["routines"])
status_path.write_text(json.dumps(status, indent=2) + "\n", encoding="utf-8")
print(f"Registered packet checksum routines; {len(registry['routines'])} routines in registry")
