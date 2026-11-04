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
link audit now resolves ten further services through an authored host-owned
queue, qualified by 6,994 host checks and 11,777 Japanese contract comparisons.
Native texture records and real owned payload release now pass 1,947 host checks
and 15,232 Japanese lifecycle comparisons. The actual GCC texture selector runs.
Native renderer state now passes 12,096 Japanese comparisons, with 142 state
host checks and 1,441 model preflight checks. The complete presentation source now compiles natively. Its broader scope has
79 references: 55 are supplied, while the combined production frame/renderer
link still needs 26 services. The clock passes 12,873 checks; actual timer/mark
boundaries pass 4,494 isolated checks. Task/replay integration passes 7,601
checks, including a native guard against restoring into deleted/reused tasks.
Native njMultiMatrix passes 2,268 original-code contract comparisons and 961
shared-state regressions, preserving the distinct FIPR/FTRV paths.
Native determinant/inverse helpers pass 1,640 original-code calls and 28,009
checks, retaining the exact singular fallback and ordered scalar arithmetic.
Bounded original FPSCR checks now establish explicit interrupt toward-zero/DN1
and callback mode preservation. A toward-zero determinant/inverse build passes
1,944 original calls, 34,725 checks and 961 shared-state regressions.
The shared Ninja matrix family now passes 6,786 toward-zero original calls
and 196 independent checks; multiplication retains 2,268 original comparisons.
View initialization, scaling and complete stack clearing pass 3,072 original
calls and 55,360 checks. Three view rotations pass 3,072 original calls and
57,463 checks. Camera setup/translation pass 1,920 original calls and 36,201
checks; camera math executes 3,626 full original acos dependency chains.
Complete 2D polygon output passes 332,375 checks, with 592 whole original
draws and 1,024 original color setter calls. Queue and camera regressions pass.
The component library contains three authored production objects. The diagnostic
DLL contains external SDK bodies and stays private.
The complete native sprite family passes 82,489 checks across 1,536 original
draws, reaching all eight triangle clipping cases. Intensity/discard contracts
pass 36,879 checks. Its 146 scalar operations use integer RZ services.
The authored queue DLL/import library is public; game-containing sprite
diagnostic artifacts remain private. GCC-wide sprite pictures, complete replay,
wide-picture numerical integration and full frames remain unfinished; BIOS
handoff is unmeasured. See [the FPSCR guide](outputs/sonic-adventure/JP_NATIVE_FPSCR_CONTEXT.txt).
Complete strip, point and clipped-polygon providers pass 371,722 checks
across 4,452 original entries. All four writers, selector caches and point-output
aliases are qualified. A 48-point host clipping case is checked by original-routine
composition without executing the original parent's unsafe small scratch.
The complete GCC model producer now routes 152 scalar operators and its vector
math through the same RZ/DN1 services. Its 66 source functions remain; 200,669
checks across 3,485 original entries pass. Every near-plane corner mask, all
three strip modes, models up to 2,048 points and shared-queue runs are covered.
GCC odd-count, frame/skip and extended-unlit policies are checked separately.
The real production model object is replaced; 26 frame services still remain.
Owned packet/resource submission storage now passes 8,572 checks across 193
submissions and 128 actual parent draws. Retained textures survive release before
submission, owner replacement and out-of-order frame completion. With retention
enabled, all 200,669 model and 82,489 sprite checks pass. The three-object authored
support library is public; diagnostic/game bodies stay private. Real render/flip
and pixel consumption remain unfinished; the same 26 frame services remain.
Ordinary packet decoding now passes 12,651 checks, including 32 actual owned
frames from 128 complete model/sprite/strip draws. Encoded texture conversion
passes 5,861 checks and 14,067,264 exact synthetic RGBA comparisons; extended
bounds/palette/mutated-stream tests pass 17,033 checks. The three authored
consumer objects are public; game-containing diagnostics stay private. GPU
drawing, render/flip and the same 26 frame services remain unfinished.
A hardware D3D11 offscreen backend now consumes these real owned frames.
The GPU suites pass 612,609 checks over 100 images and 611,671 compared interior
pixels, including 32 complete actual model/sprite/strip parent calls. Texture
Off skips decoding/upload and its compiled shader has no texture/sampler
bindings. Empty 7680x4320 target/readback works; gameplay performance is unverified.
A one-object authored GPU support library is public research code. Window/flip/
vsync, actual game frames and console pixel fidelity remain unfinished.
Owned frames now capture palette/fog/clip/clear metadata, and a native Win32/
DXGI window presents their bound images. New suites pass 103,742 checks, including
real backbuffer comparisons and four successful visible Presents. All 8,572
ownership and 612,609 GPU checks pass on the new providers. The coherent authored
nine-object support library is public. The real device-state owner resolves one
production import: 56/79 presentation imports are supplied, and 25 combined
services remain. Kamui scheduling/statistics and actual game frames remain open.
The native direct window path now renders into DXGI backbuffers without CPU
image transfer. It passes 1,878 new checks: 111 direct images / 33,932,288 pixels
match the legacy renderer exactly, including an empty 8K target; eleven direct
Presents succeed. Frozen GPU/state/window suites pass 716,351 checks. The new
coherent authored support library replaces overlapping older graphics libraries.
Three game-thread waits and real Kamui scheduling/statistics remain open.
Bounded GPU jobs now retain four independent targets with actual event and
timestamp queries. They pass 6,330 new checks plus 718,229 preserved checks.
115 job images / 38,138,880 pixels compare correctly, and 117 submissions yield
reliable GPU timings on this device. Completed targets copy to a window on the
GPU; pending output and deferred shutdown are verified. Native frame waits and
Kamui scheduling/statistics are the next integration work.
The complete original frame-pacing source now compiles with serialized native
waits and without SH-4 SDK hardware inlines. Its frame/step/hold behavior passes
6,943 checks plus all 12,873 clock regressions, including overdue tick counts,
real QPC/Win32 waits and close handling. Four original frame globals are supplied
(60/79 presentation imports). Adding the frame source exposes 22 more genuine
dependencies; the expanded graph has 43 missing services. No trace providers are
linked into production. Real game render/flip integration remains open.
Bounded PVM archive loading now retains six original SDK metadata helpers and
loads all 27 textures from supplied-disc logo/title archives into real native
owners. 31 GPU images / 126,976 pixels and 116,518 combined checks pass.
The production graph gains real PVM services without new unresolved imports;
36 existing game services and real file/PRS/boot integration remain open.
Retail assets and their probe images stay private.

The original repaired bitmap queue now uploads bounded memory textures into
real shared native records. Two original frame-wait images verify old/new
bitmap pixels across refresh; 115,068 integration/regression checks pass.
Production supplies 65/79 presentation imports with 36 services remaining.
File/cache formats, assets and full gameplay remain unfinished.

Native kmRender/flip/reset and typed measured statistics now drive actual GPU
jobs and real window Presents. EOR callbacks are delivered once on the game
thread. The original frame_wait executes this backend; eight Presents and four
image comparisons pass, with 132,218 checks including preserved regressions.
The actual typed SDK device owner is shared. Production supplies 63/79 original
presentation imports with 39 services left; audio/bitmap/loader traces remain
diagnostic-only. Full game execution and hardware slot latency remain open.
Full-frame execution and gameplay are unfinished. English integration
begins as the needed asset/text/audio systems become usable. Game-containing
runtime artifacts remain private.

The goal is complete native PC gameplay with English localization, 60/120/240 FPS,
up to 8K, display/texture settings and optional SUPIR enhancements, followed by
Android and stereoscopic 3DS ports and eventual N64/PlayStation demakes.
Texture Off must disable sampling and use vertex/material colours.

## Current code and evidence

- [Current goal](outputs/sonic-adventure/JP_BASED_PORT_GOAL.txt)
- [Integration order and limits](outputs/sonic-adventure/JP_NATIVE_PORT_PLAN.txt)
- [Verified GCC baseline and native/localization evidence](outputs/sonic-adventure/JP_GCC_BASELINE.txt)
- [Bounded PVM archives and supplied-disc native texture probes](outputs/sonic-adventure/JP_NATIVE_PVM_UPLOAD.txt)
- [Real bitmap queue and bounded texture uploads](outputs/sonic-adventure/JP_NATIVE_BITMAP_UPLOAD.txt)
- [Real native render/flip and measured statistics](outputs/sonic-adventure/JP_NATIVE_RENDER_FLIP.txt)
- [Native frame waits and original callback contracts](outputs/sonic-adventure/JP_NATIVE_FRAME_WAITS.txt)
- [GPU completion jobs and timing limits](outputs/sonic-adventure/JP_NATIVE_GPU_JOBS.txt)
- [Direct native window rendering and limits](outputs/sonic-adventure/JP_NATIVE_DIRECT_WINDOW.txt)
- [Captured rendering state and native window](outputs/sonic-adventure/JP_NATIVE_WINDOW_STATE.txt)
- [Native offscreen GPU consumption and Texture Off](outputs/sonic-adventure/JP_NATIVE_GPU.txt)
- [Native packet and encoded-texture consumers](outputs/sonic-adventure/JP_NATIVE_PACKET_TEXEL_CONSUMERS.txt)
- [Owned frame commands and texture lifetimes](outputs/sonic-adventure/JP_NATIVE_OWNED_FRAMES.txt)
- [Complete model producer numerical integration](outputs/sonic-adventure/JP_NATIVE_MODEL_RZ.txt)
- [Native strip/texture and clipped polygon providers](outputs/sonic-adventure/JP_NATIVE_PRIMITIVES.txt)
- [Native sprites, intensity headers and discarded batches](outputs/sonic-adventure/JP_NATIVE_SPRITES.txt)
- [Native 2D polygons, bounded queue and picture aspect](outputs/sonic-adventure/JP_NATIVE_POLYGON2D.txt)
- [Native camera setup, translation and SDK math](outputs/sonic-adventure/JP_NATIVE_CAMERA.txt)
- [Native view direction/roll helpers](outputs/sonic-adventure/JP_NATIVE_VIEW_ROTATION.txt)
- [Native toward-zero matrix family](outputs/sonic-adventure/JP_NATIVE_MATRIX_RZ.txt)
- [View initialization, scaling and matrix-stack clearing](outputs/sonic-adventure/JP_NATIVE_VIEW_STACK.txt)
- [Native determinant/inverse and scalar qualification](outputs/sonic-adventure/JP_NATIVE_INVERSE_MATRIX.txt)
- [Native matrix multiplication and original-code comparisons](outputs/sonic-adventure/JP_NATIVE_MULTI_MATRIX.txt)
- [Task deletion, retained ownership and replay qualification](outputs/sonic-adventure/JP_NATIVE_REPLAY_LIFECYCLE.txt)
- [Native presentation, independent clocks and remaining dependencies](outputs/sonic-adventure/JP_NATIVE_PRESENTATION_CLOCK.txt)
- [Renderer state, model bounds and remaining frame services](outputs/sonic-adventure/JP_NATIVE_RENDER_STATE.txt)
- [Native texture ownership, verified scope and remaining integration](outputs/sonic-adventure/JP_NATIVE_TEXTURE_OWNERSHIP.txt)
- [Native graphics queue, verified scope and integration requirements](outputs/sonic-adventure/JP_NATIVE_QUEUE.txt)
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
