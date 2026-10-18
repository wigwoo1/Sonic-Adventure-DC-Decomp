"""Verify extracted disc files against the source inventory manifest."""
import argparse
import hashlib
import json
import pathlib
import sys


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("inventory", nargs="?", type=pathlib.Path,
                        default=pathlib.Path("outputs/sonic-adventure/disc_inventory.json"))
    parser.add_argument("extracted_root", nargs="?", type=pathlib.Path,
                        default=pathlib.Path("work/sonic-adventure/extracted/files"))
    parser.add_argument("--report", type=pathlib.Path,
                        help="Write the verification report to this path")
    args = parser.parse_args()
    inventory_path = args.inventory.resolve()
    extracted_root = args.extracted_root.resolve()
    if not inventory_path.is_file():
        parser.error(f"inventory manifest does not exist: {inventory_path}")
    inventory = json.loads(inventory_path.read_text(encoding="utf-8"))
    errors, checked, total_bytes = [], 0, 0
    for record in inventory["files"]:
        if record["directory"]:
            continue
        path = extracted_root / pathlib.PurePosixPath(record["path"])
        if not path.is_file():
            errors.append(f"{record['path']}: missing")
            continue
        data = path.read_bytes()
        checked += 1
        total_bytes += len(data)
        if len(data) != record["size"] or hashlib.sha256(data).hexdigest() != record.get("sha256"):
            errors.append(f"{record['path']}: hash or size mismatch")
    report = {"inventory": str(inventory_path), "extracted_root": str(extracted_root),
              "files_checked": checked, "bytes_checked": total_bytes, "errors": errors}
    report_path = (args.report.resolve() if args.report else
                   inventory_path.parent / "analysis" / "extraction_verification.json")
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(report, indent=2))
    return 1 if errors else 0


if __name__ == "__main__":
    raise SystemExit(main())
