"""Prepare a Ghidra 12.1.4 big-endian SuperH analysis copy.

The bundled SuperH language definitions are big-endian. Swapping each 16-bit
word lets Ghidra decode retail SH-4 instruction words from the little-endian
Dreamcast image. This is an analysis-only copy; never use it as runtime data.
"""
import argparse
import hashlib
import json
from pathlib import Path


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("source", type=Path)
    parser.add_argument("destination", type=Path)
    args = parser.parse_args()
    original = args.source.read_bytes()
    if len(original) & 1:
        raise SystemExit("Input length must be even to swap 16-bit instruction words")
    swapped = bytearray(len(original))
    swapped[0::2] = original[1::2]
    swapped[1::2] = original[0::2]
    args.destination.parent.mkdir(parents=True, exist_ok=True)
    args.destination.write_bytes(swapped)
    report = {
        "source": str(args.source.resolve()),
        "source_bytes": len(original),
        "source_sha256": hashlib.sha256(original).hexdigest(),
        "analysis_copy": str(args.destination.resolve()),
        "analysis_copy_sha256": hashlib.sha256(swapped).hexdigest(),
        "transformation": "swap bytes within each 16-bit word",
        "purpose": "Ghidra big-endian SuperH instruction listing only",
        "runtime_use": False,
    }
    report_path = args.destination.with_suffix(args.destination.suffix + ".json")
    report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(report, indent=2))


if __name__ == "__main__":
    main()
