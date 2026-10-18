# Sonic Adventure Dreamcast decompilation research

Work in progress for Sonic Adventure (USA), Dreamcast Rev A. **This is not a
playable PC port or a complete decompilation.** Overall milestone estimate: 10%.
The estimate is not measured recovered-code coverage.

The project aims for a native PC port at 60/120/240 FPS and up to 8K, later
Android and stereoscopic Nintendo 3DS ports, and eventual N64/PlayStation demakes.
SUPIR HD textures and display settings are planned; they are not implemented.

## Current code and evidence

- [Project README](outputs/sonic-adventure/README.md)
- [Source access and detailed status](outputs/sonic-adventure/SOURCE_HANDOFF.txt)
- [Recovered C and support code](outputs/sonic-adventure/source/)
- [Verification reports and research drafts](outputs/sonic-adventure/analysis/)
- [Latest angle helper evidence](outputs/sonic-adventure/DRAW_ANGLE_CURVE.txt)
- [Persistent renderer initialization](outputs/sonic-adventure/DRAW_FRAME_SEQUENCE.txt)
- [Build and verification workflow](outputs/sonic-adventure/build_and_verify.ps1)

Research DLLs and import libraries are provided as prerelease assets with SHA256
manifests. They expose test interfaces; there is no game executable to launch.
Qualification varies by component. Consult each original report and its limits.
No disc image, original executable, decoded retail modules, textures, models,
music, proprietary SDK, toolchain bundle or credentials are included.

## Reproducing work

Keep this repository's `outputs/sonic-adventure` layout. Scripts generally resolve
the workspace two levels above that directory and use `work/sonic-adventure` for
locally extracted game inputs and build outputs. Supply your own matching Rev A
game inputs separately. Python, Clang/LLD, PowerShell and, for analysis, Ghidra/JDK
are external dependencies. Some older scripts retain the original workstation's
tool paths; public portability of the complete workflow is still unfinished.
Historical reports pin the original paths and hashes; publication preserves
their bytes instead of silently rewriting their evidence.

`PUBLICATION_MANIFEST.json` records the exact published source/document bytes.
Use `publish_github_snapshot.py --workspace PATH` to prepare a new snapshot, then
add `--publish` to upload using Git Credential Manager. Retail input folders and
toolchain caches are excluded by the publisher's allowlist.

## Attribution

Sonic Adventure and its original materials belong to their respective owners.
This is an independent research project. The numerical dependency in
`outputs/sonic-adventure/third_party/skmp-sh4-fpu` retains its upstream MIT notice.
No project-wide license is assigned by this snapshot.
