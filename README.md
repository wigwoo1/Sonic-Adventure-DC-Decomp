# Sonic Adventure Japanese-source native-port research

Work in progress using the complete matching Japanese Dreamcast HDR-0001 v1.007
source as the primary development base. **There is no playable native PC port yet.**
Overall estimate: 10% of weighted milestones, not measured code coverage.

The original Japanese matching build is verified. The isolated GCC baseline also
builds: 247 code images and seven motion-data images, with all 4,160 manifest file
hashes checked. GCC still targets Dreamcast SH-4. A native Windows task/heap
component now links with real cleanup functions and a documented 64-bit allocator
adaptation; 1,610 host API checks pass. There is no native game loop yet. English
localization remains unimplemented; clip identities are candidate anchors only.

The portable matrix backend passes 2,946 checks, and adapted renderer data-access
helpers pass 2,624 checks. The complete model renderer compiles with 14 native
entry points. A shared Ninja/renderer matrix runtime now passes its original
Japanese contracts, backend checks and real task regressions. The actual renderer
link audit leaves 30 graphics/texture/presentation services; it is still unlinked.

The goal is complete native PC gameplay with English localization, 60/120/240 FPS,
up to 8K, display/texture settings and optional SUPIR enhancements, followed by
Android and stereoscopic 3DS ports and eventual N64/PlayStation demakes.
Texture Off must disable sampling and use vertex/material colours.

## Current code and evidence

- [Current goal](outputs/sonic-adventure/JP_BASED_PORT_GOAL.txt)
- [Integration order and limits](outputs/sonic-adventure/JP_NATIVE_PORT_PLAN.txt)
- [Verified GCC baseline and native/localization evidence](outputs/sonic-adventure/JP_GCC_BASELINE.txt)
- [Shared matrix runtime, verified scopes and remaining work](outputs/sonic-adventure/JP_SHARED_MATRIX_RUNTIME.txt)
- [Renderer boundary, verified scopes and remaining work](outputs/sonic-adventure/JP_NATIVE_RENDERER_BOUNDARY.txt)
- [Verified native task runtime and limits](outputs/sonic-adventure/JP_NATIVE_TASK_RUNTIME.txt)
- [Integration workflows](outputs/sonic-adventure/jp_port_tools/)
- [Authored native adapters](outputs/sonic-adventure/native/)
- [Verification reports](outputs/sonic-adventure/analysis/)
- [Preserved USA research and source handoff](outputs/sonic-adventure/SOURCE_HANDOFF.txt)

The pinned upstream dependency is
[reshaped/sonic-adventure at 25a3d315](https://github.com/reshaped/sonic-adventure/tree/25a3d3154dbe657325e97dbe8c5291086f6af5c0).
Its game C remains external while redistribution permission is unresolved.
This repository publishes our authored integration code, metadata and tools;
it does not vendor those game-source bodies or assign them a license.

## Builds and reproduction

Prerelease DLLs/import libraries are preserved historical Windows USA research
artifacts, not the Japanese GCC game build or a playable PC port. Their original
reports define their qualification. No disc images, retail models/textures/audio,
complete game images, toolchain bundles or credentials are published.

Keep the outputs/sonic-adventure layout and provide matching local inputs and the
pinned upstream checkout separately. The current GCC workflow uses an isolated
WSL worktree. Read the baseline guide for required commands, external toolchains,
private paths and fresh-output requirements. Portable installation is unfinished.

PUBLICATION_MANIFEST.json records selected source/document hashes. Each research
archive contains BUILD_MANIFEST.json and has a SHA256SUMS companion. Historical
source/proofs and all previous research builds remain preserved. No project-wide
license is assigned; existing third-party notices remain with their components.
