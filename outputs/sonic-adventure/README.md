Latest title-fade update: JP_NATIVE_TITLE_FADES.txt. Original black/white fades
and actual Ninja context/header generation now compose the supplied logo through
the shared native owners. 28 fade images / 8,601,600 pixels and 117,322 combined
checks pass, including real frame-wait/Present and Texture Off. A fourteen-member
authored support library is published; SDK/game implementations stay external.
Expanded boot still lacks 43 services. Real task/heap startup and input/exit/save
integration remain necessary before original logo startup or full boot is claimed.
Earlier snapshot notes below retain their historical scope. Japanese baselines
verified; overall about 10%; goal active.

Latest native background update: JP_NATIVE_BACKGROUND_PLANES.txt. Original back-color
packets now render through real SDK/frame/GPU providers. Full gradients, UV16,
offset colors, texture release, Texture Off and the original logo/Present path
pass: 17 images / 978,944 pixels; 116,997 combined checks. Documented SDK
packet defects are repaired in generated native copies. A 13-member authored
support library is available. Expanded original boot has 43 services left;
actual title fades, input/save/exit and full gameplay remain unfinished.
Prior snapshot notes below retain their historical verification boundaries.
Japanese baselines verified; overall about 10%; goal active.

Latest qualified native logo update: JP_NATIVE_LOGO_ASSETS.txt. The unchanged
original six-piece logo now uses real file/PRS/PVM loading and passes full
640x480 image comparisons through the original frame-wait and real Present.
Three logo images / 921,600 pixels and 116,772 combined checks pass. The
four-member authored asset support library is published separately from
game/SDK/codec implementations. Production still lacks 36 game services;
logo startup/fading, full boot and gameplay remain unfinished. Prior
snapshot notes below describe their earlier verification boundaries.
Japanese baselines verified; overall about 10%; goal active.

Current native file/PRS snapshot: JP_NATIVE_ASSET_LOADING.txt. The original
adapted 512-entry game texture wrapper now uses real native file/PRS/PVM
loading and checked ownership transfers. A clean build passes 223 asset
checks, including 27 GPU texture probes / 110,592 pixels, plus 10,519
texture/frame regression checks. Original logo_draw compiles privately;
full logo display and production boot remain unverified. No asset-runtime
DLL or retail content is published. About 10% overall; goal active.

Current native PVM update: JP_NATIVE_PVM_UPLOAD.txt. Bounded archive uploads
retain six original SDK metadata helpers and use actual shared native
texture/palette owners. All 27 supplied-disc logo/title textures pass GPU
probes, plus alpha and retained-palette cases: 31 images / 126,976 pixels.
1,448 integration and 115,070 regression checks pass (116,518 total).
Production still has 36 game services left, with no new unresolved imports;
the real file/PRS/boot path remains to be connected. Retail assets and
game-containing DLLs stay private. Japanese baselines verified; full native
gameplay, English and later ports unfinished. About 10%; goal active.

Current native bitmap upload update: JP_NATIVE_BITMAP_UPLOAD.txt. Original
GCC queue pointer repair now uploads real owned memory textures through the
existing SDK record/handle owner. Bounded sources, palette sharing, retry
and old-frame retention are verified. Two original frame-wait GPU images
match; 722 integration plus 114,346 preserved regression checks pass
(115,068 total). Production supplies 65/79 presentation imports with 36
services left. File/cache/remaining formats and full gameplay remain open.
Japanese baselines verified; English/later platforms unfinished. About 10%;
goal active; incomplete research artifacts, not a playable game.

Current native render/flip update: JP_NATIVE_RENDER_FLIP.txt. kmRender now
captures and submits actual GPU work; flip observes completion, dispatches
real EOR callbacks, waits a logical tick and Presents. Statistics measure
native work and the actual SDK device record is shared. The original frame
wait executes real render/flip. Eight Presents and four GPU images pass;
130 integration plus 132,088 regression checks pass (132,218 combined).
The production audit supplies 63/79 presentation imports; 39 services
remain, without trace fixtures. Japanese baselines verified; full gameplay,
English and later ports unfinished. About 10%; goal active.

Current frame-wait update: JP_NATIVE_FRAME_WAITS.txt. The complete original
frame-pacing source compiles with native serialized waits, without enabling
SH-4 SDK hardware headers. All three spin paths are adapted; original frame,
step and hold contracts pass, including late-frame synchronization and real
WM_CLOSE handling. 6,943 new checks plus 12,873 clock regressions pass.
Four original frame globals are now supplied (60/79 presentation imports).
Expanding the production graph exposes 22 real dependencies: 43 services
remain. Render/flip/backend services and gameplay are unfinished. Japanese
baselines verified; English/later ports unfinished. About 10%; goal active.

Current GPU-jobs update: JP_NATIVE_GPU_JOBS.txt. Four owned GPU targets now
use real completion/timestamp queries and GPU-only copies into the window.
6,330 new checks and 718,229 preserved checks pass (724,559 combined).
115 job images / 38,138,880 backbuffer pixels compare correctly; 117 GPU
job submissions return reliable timing in these tests. Pending output,
capacity, source-owner retirement and deferred shutdown are verified.
The actual production link still needs 25 services. Native game waits,
Kamui render/flip/statistics and gameplay remain unfinished. Japanese source
baselines verified; English/later ports unfinished. About 10%; goal active.

Current direct-window update: JP_NATIVE_DIRECT_WINDOW.txt. The renderer draws
owned frames directly into DXGI backbuffers without a CPU image transfer.
1,878 new checks pass; 111 direct images / 33,932,288 backbuffer pixels match
the prior renderer exactly, including an empty 8K target. Eleven direct
Presents succeeded; frozen GPU/state/window suites pass 716,351 checks.
The actual production link still needs 25 services. Native frame waits,
Kamui render/flip/statistics and gameplay remain unfinished. Japanese source
baselines verified; English/later ports unfinished. About 10%; goal active.

Current state/window update: JP_NATIVE_WINDOW_STATE.txt. Owned frames retain
palette/fog/clip/clear state and can render/present through a real Win32/DXGI
window. New state/window suites pass 103,742 checks and four visible Presents.
All 8,572 ownership and 612,609 GPU regression checks pass on new providers.
The production link has 25 unresolved services (56/79 presentation imports
supplied). Kamui render/flip/statistics, real game frames and gameplay remain
unfinished. About 10% overall; Japanese baseline verified; goal active.

Current native GPU update: see JP_NATIVE_GPU.txt. Actual model/sprite/strip
packets now render to a hardware offscreen target and return RGBA8 pixels.
612,609 checks and 100 image comparisons pass with a documented native-float
pixel oracle. Texture Off has zero shader texture/sampler bindings or uploads.
A real empty 7680x4320 target/readback passes; 8K gameplay is unverified.
No game loop/window/flip/vsync or playable port yet. Console pixel fidelity
and complete effects remain unfinished. About 10% overall; goal active.

Current packet/texture consumer update: JP_NATIVE_PACKET_TEXEL_CONSUMERS.txt.
All nine ordinary packet layouts decode actual owned model/sprite/strip output.
12,651 packet checks, 5,861 texture checks and 17,033 extended checks pass.
More than 14 million independently expected RGBA texels compare exactly.
A three-object authored consumer support library is public research code.
Native GPU drawing, render/flip, full frames and gameplay remain unfinished.
About 10% overall; Japanese baseline verified; native PC integration partial.

Current owned-frame update: see JP_NATIVE_OWNED_FRAMES.txt. Actual committed
packets and encoded texture payloads survive queue/texture reset and release.
8,572 ownership checks, 193 submissions and 128 complete parent draws pass.
With retention enabled, all 200,669 model and 82,489 sprite checks pass.
All legacy-mode primitive/polygon/camera/shared-state regressions pass.
One three-object authored support library is public; game DLLs stay private.
The same 26 real frame services remain; render/flip and pixels are unfinished.
About 10% overall; goal active; no playable native port.

Current complete-model update: see JP_NATIVE_MODEL_RZ.txt. The full pinned
GCC model producer retains all 66 source functions; 152 scalar operations
and vector/point transforms use the existing RZ/DN1 numerical services.
200,669 checks across 3,485 original entries pass, including every clipping
corner mask in all three strip modes and models with up to 2,048 points.
All primitive/sprite/polygon/camera/shared-state regressions pass. The real
production model object is replaced; the same 26 frame services remain.
About 10% overall. No complete native frame or playable PC port.

Current primitive update: see JP_NATIVE_PRIMITIVES.txt. Complete strip/texture,
point-transform and clipped-polygon routines pass 371,722 checks across 4,452
original entry calls. All four writers, selector caches and aliasing are
qualified; 48-point host scratch is checked with original-routine composition.
Presentation supplies 55 of 79 imports; 26 combined frame services remain.
About 10% overall. No complete frame or playable native port.

Current sprite update: see JP_NATIVE_SPRITES.txt. The complete SDK/game sprite
family passes 82,489 checks across 1,536 original draws, including all eight
triangle clipping cases. Intensity/discard contracts pass 36,879 checks;
polygon, camera and shared-state regressions pass. Presentation supplies
52 of 79 imports; 29 combined frame services remain. About 10% overall.
No complete frame or playable port; GCC-wide sprite pictures remain gated.

Current 2D polygon update: see JP_NATIVE_POLYGON2D.txt. Complete plain/textured
fill and outline output passes 332,375 checks, including 592 whole original
draws and 1,024 original color-mode calls. Host queue/camera regressions pass.
Presentation supplies 45 of 79 imports; 36 combined frame services remain.
About 10% overall; no complete frame or playable native port.

Current camera update: see JP_NATIVE_VIEW_ROTATION.txt and
JP_NATIVE_CAMERA.txt. The three view rotations pass 3,072 original calls
and 57,463 checks. Camera setup/translation pass 1,920 original calls and
36,201 checks; camera math executes 3,626 complete original acos chains.
Presentation supplies 42 of 79 raw imports; 40 combined frame services
remain. About 10% overall; no full frame or playable native port.

Current native matrix/view update: see JP_NATIVE_MATRIX_RZ.txt and
JP_NATIVE_VIEW_STACK.txt. RZ shared Ninja/multiplication migration passes;
view initialization, scaling and complete clear pass 3,072 original calls
and 55,360 checks. Presentation supplies 38 of 79 raw imports; the combined
link still needs 44 services. View rotation/setup and renderer numerical
integration remain unfinished. About 10% overall; no playable native port.

FPSCR update: see JP_NATIVE_FPSCR_CONTEXT.txt. Original interrupt contexts
select toward-zero/DN1; the callback wrapper inherits rounding. The new
determinant/inverse build passes 1,944 original calls, 34,725 checks and
961 shared-state checks; 724 context checks qualify bounded code paths.
Other Ninja/renderer arithmetic still needs numerical migration; 46
services remain unresolved. About 10% overall; no playable native port.

Native determinant/inverse update: see JP_NATIVE_INVERSE_MATRIX.txt.
1640 original routine comparisons and 28009 determinant/inverse checks pass.
Multiplication and shared-state regressions pass in the same DLL. The real
production frame/renderer still needs 46 services. No playable port.
Overall estimate remains about 10% weighted milestones; goal active.

Native matrix multiplication update: see JP_NATIVE_MULTI_MATRIX.txt.
2268 original-code comparisons, 33208 contract checks and 961 shared-state
regressions pass. njMultiMatrix is supplied by the real native object; the
complete frame/renderer still needs 47 services. No playable port.
Overall estimate remains about 10% weighted milestones; goal active.

Native replay lifecycle update: see JP_NATIVE_REPLAY_LIFECYCLE.txt.
7601 integration checks pass, including retained ownership, player snapshots,
and a fix for restoration into a deleted/reused task. Full frames remain
unlinked: the production frame/renderer still needs 48 real services.
Overall estimate remains 10% weighted milestones; goal active.

Native presentation/clock update: see JP_NATIVE_PRESENTATION_CLOCK.txt.
Complete presentation source compiles; 12,873 clock checks and 4,494 isolated
actual timer/mark checks pass. Presentation-aware task hooks also compile.
Combined production frame/renderer still needs 48 services; no playable port.
Overall estimate remains 10% weighted milestones; goal active.

Native renderer state update: see JP_NATIVE_RENDER_STATE.txt.
142 state host checks, 12,096 Japanese comparisons and 1,441 model preflight
checks pass. Ten real state imports resolved; seven frame services remain.
Complete rendering and gameplay remain unfinished. Overall estimate 10%.

Native texture ownership update: see JP_NATIVE_TEXTURE_OWNERSHIP.txt.
1,947 host checks and 15,232 Japanese lifecycle comparisons pass. Actual GCC
texture selection runs; three imports resolved, 17 real services remain.
Full loaders, texture decoding and complete rendering remain unfinished.
Overall estimate remains 10% weighted milestones; goal active.

Native graphics queue update: see JP_NATIVE_QUEUE.txt.
6,994 host checks, 11,777 Japanese contract comparisons and 1,024 executor
extension checks pass. Ten renderer imports resolved; 20 real services remain.
Host-owned packets only: complete rendering and playable gameplay unfinished.
Overall estimate remains 10% weighted milestones; goal active.

Shared matrix runtime update: see JP_SHARED_MATRIX_RUNTIME.txt.
Ninja and renderer state are unified: 961 checks in standalone and real task builds.
5,696 Japanese comparisons, 196 independent checks, 2,946 backend checks and
1,610 real task regressions pass. Renderer link audit leaves 30 real services.
Overall estimate remains 10% weighted milestones; no playable native game.

Native renderer boundary update: see JP_NATIVE_RENDERER_BOUNDARY.txt.
Portable matrix backend: 2,946 checks. Renderer data-access helpers: 2,624 checks.
Complete renderer object compiles with 14 entry points; 33 imports remain.
Only the asset-free MIT dependency host probe is a new public research build.
Overall estimate remains 10% weighted milestones; no playable native game.

Native task runtime update: see JP_NATIVE_TASK_RUNTIME.txt.
The pinned task source now links with real lifecycle cleanup and native math.
A documented generated-source adaptation fixes 64-bit heap-header splitting.
All 1,610 host API checks pass; game-containing artifacts remain private.
Overall estimate remains 10% weighted milestones; no playable native game.

Japanese-source native-port direction: see JP_GCC_BASELINE.txt and JP_NATIVE_PORT_PLAN.txt.
Pinned upstream GCC build passed: 247 code images plus seven motion-data images.
All 4,160 manifest hashes and 254 SH-4 ELF headers checked; original matching build intact.
Windows task/heap objects compile; link stops on eleven real dependencies.
English voice research identifies 1,664 unique Japanese clip anchors; English binding unverified.
Overall estimate: 10% revised weighted milestones; goal active; no playable PC port.

Original USA world render-end and queued fixture job: see STAGE_RENDER_END.txt.
Strict 317-C build; 14 CPU comparisons (12 normal, two guards).
Fixture job 5 queues 8,709 packets; both pixel backends reject unsupported commands.
No new completed pixel job; original bank handoff remains unqualified.
Overall estimate: 10% weighted milestones; goal active; ports unfinished.

Original USA stage resources and textures: see STAGE_RESOURCE_LOADING.txt.
Strict 314/315/316-C builds; 27 comparisons (22 normal, five host stops).
Original stage bytes and texture-loading path pass; whole world frame unqualified.
Overall estimate: 10% weighted milestones; goal active; ports unfinished.

Original USA world/land producer research: see MODEL_WORLD_LAND.txt.
Strict 313-C build; 61 comparisons (53 normal, eight expected guard stops).
Prepared native frame 4 passes; actual level resources remain unqualified.
Overall estimate: 10% weighted milestones; goal active; ports unfinished.

Original USA world-reset/candidate writers: see MODEL_WORLD_WRITERS.txt.
Strict 311-C build; 67 comparisons (59 normal, eight expected guard stops);
prepared scene completes native job 3; no real populated level qualified.
Overall estimate: 10% weighted milestones; goal active; ports unfinished.

Japanese matching baseline and source-map research: see CROSS_VERSION_SOURCE_MAP.txt.
247 Japanese images rebuilt byte-for-byte; 354 provisional two-sided unique
USA byte-span pairs, with no new native routine credit or imported game C.
Overall estimate remains 10% weighted milestones; goal active; ports unfinished.

Latest qualified step: camera draw, state-2 update and following native frame
The original camera callbacks and next prepared frame now return normally.
Sixteen bodies contain 1,830 statically recovered rows; runtime checks cover
the reached paths with an empty candidate table. Strict 310-source build,
19 full comparisons (18 normal, one expected guard) and three new native jobs
pass. The original IRQ path acknowledges prior completion; no token is injected.
See CAMERA_DRAW_STATE2.txt. Full game/level coverage remains unfinished.
Approximately 10% weighted milestones; goal active.

Latest qualified step: complete prepared particle frame and native render jobs
The original frame path submits all 194 packets, retires its list and completes
native pixel work. Both texture modes match complete colour/depth images for
62 triangles. Strict 305-source build, eight normal full comparisons and 331
native ownership/failure checks pass. No external completion token is used.
This prepared scene remains separate from full game/frame-loop qualification.
See TA_STREAM_RENDER_JOBS.txt. Approximately 10% weighted milestones; goal active.

Latest qualified step: owned TA pages and complete original particle passes
All six actual particles now complete with 186 packets across three owned
64-packet pages. A second matched normal pass retains 372 packets across six
pages. Strict 301-source build; 14 normal full comparisons and four storage
ownership/failure test groups pass. Large-stream pixel jobs remain unfinished.
See TA_STREAM_OWNERSHIP.txt for evidence, limits and reproduction.
Approximately 10% weighted milestones; no new registry credit. Goal active.

Latest qualified step: original scene and first camera positioning
Original scene setup and first allocated-camera positioning now return normally.
The complete two-head copy dispatch retains all 17 selectors and overlap order.
Strict 299-source build; 99 full comparisons (94 normal and five expected
bounded stops), plus two lower-API checks. The finite six-particle stream reaches
the existing 64-packet collector limit; its full frame remains unfinished.
See MODEL_CAMERA_POSITION.txt for evidence, limitations and reproduction.
Approximately 10% weighted milestones; no new registry credit. Goal active.

Latest verified step: original allocated camera and scheduled star callbacks
The original camera factory and first scheduled update publish the real allocated
work pointer. All six stored star callbacks then complete 8,905 instructions.
Two actual frame scenarios and two retained preview controls pass, including
Texture Off with zero sampling. Total: 44 comparisons and a strict 293-source
build. Later camera updates remain unfinished at 8c019400 and 8c01a100.
See MODEL_CAMERA_OWNER.txt for the verified scope and reproduction workflow.
Approximately 10% weighted milestones; no new registry credit. Goal active.

Latest verified step: allocated star tasks and their camera dependency
Original allocation, six child callbacks and parent removal match. The next
callback correctly stops at the uninitialized camera pointer; startup remains
unfinished. Six callback comparisons and 12 retained rendering comparisons
pass on the guarded backend, with two strict 292-source builds. Texture Off
samples zero textures. See MODEL_CALLBACK_CAMERA_DEPENDENCY.txt for limits.
Approximately 10% weighted milestones; no new routine credit. Goal active.

Latest verified step: original star model produces native pixels and frame jobs
The original model emits 31 floating-colour packets / 10 triangles. Additive
backends render 5,929 model pixels matching an independent rational oracle. Two
original frame scenarios submit 39 packets each and complete actual native jobs.
Texture Off samples zero textures. All 271 platform checks and 14 original/native
calls pass. The visible position is an explicit test input, not a game camera.
See MODEL_FLOAT_RENDER.txt. Hardware accuracy, gameplay and ports remain unfinished.
Approximately 10% weighted milestones; no new routine credit. Goal active.

Latest verified step: original source-record dependency reaches the model path
The guarded geometry copy links and matches 17 cold-pointer cases. Three original
source-record routines pass 59 unit comparisons covering all 223 instruction rows.
With the original caller-selected record, initialization and the retail model
wrapper return with exact original/native state. No model packets are submitted:
the original 11-vertex mesh exceeds the current vertex capacity 0. That capacity
is unchanged. See MODEL_VIEW_PRODUCER.txt for evidence and the next buffer owner.
Approximately10% weighted milestones; no new registry credit. Goal active.

Latest verified step: original TA list-init now owns collector reuse
Twelve original/native frames render twelve jobs and retire 108 packets using
the unchanged 64-packet collector. Original list-init writes trigger reuse; no
harness reset or capacity inflation is used. The scene passes 49 comparisons
plus two controls: 772 checks / 671 exact oracles. Native units pass 12,092
comparisons and 1,002 jobs. See TA_LIST_REUSE.txt and its qualification.
Large level/model streams, hardware accuracy and playable boot remain unfinished.
Approximately 10% weighted milestones; no new routine credit. Goal active.

Latest verified step: corrected VRAM layout and visible retail texture pixels
The new bank-linear-to-texture-view adapter preserves the game VRAM backend.
All seven mip levels match a unique retail archive entry. The native fixture
produces two textured RGB pixels; Texture Off produces four vertex-coloured
RGB pixels with zero samples. The four-frame scene passes 33 comparisons and
two controls: 604 checks / 535 exact oracles. See BANKED_TEXTURE_PIXELS.txt.
Hardware accuracy, playable boot and FPS targets remain unfinished.
Approximately 10% weighted milestones; no new routine credit. Goal active.

Latest verified step: real native pixel work in the complete matched scene
Four original submissions now complete through actual native packet rendering;
no scripted completion acknowledgements are supplied. The scene passes 33 normal
comparisons plus a disabled-completion control: 601 checks / 532 exact oracles.
Complete pixel-job, colour/depth, CPU/memory/device and ordered state match.
The captured texture remains transparent; visible retail pixels and hardware
accuracy are unqualified. See RENDERER_PIXEL_JOBS.txt and its qualification.
Approximately 10% weighted milestones; no new routine credit. Not playable.
Goal active; continue Sol High for texture/address/format investigation.

Latest research step: standalone native pixels (partially verified)
Texture/triangle tests pass 343 checks; immutable pixel jobs pass 13 checks.
Texture Off uses vertex/material colours without sampling. Eight captured-quad
buffer cases match an independent host reference; the textured fixture is fully
transparent and does not qualify visible retail textures. Whole-scene pixel-job
integration and physical GPU pixel matching remain unfinished.
See PVR_PIXELS.txt and analysis/pvr_pixels_qualification.json.
Approximately 10% weighted whole-project milestones; no new routine credit.
Incomplete research artifacts, not a playable port. Goal remains active.

Latest verified step: host render-job completion and original IRQ handling
A 291-source research build preserves 289 previous C inputs. The new job core
passes 12,000 operations / 30,777 checks. The scene passes 33 normal comparisons
plus a matched disabled-completion control: 598 checks / 529 exact oracles.
Declared backend acknowledgements reduce later frames from twelve VBlank
services to two; original IRQ/RTE and wait guards remain intact. Actual pixel
rendering is still unfinished. This is a completion interface, not a playable port.
See RENDER_COMPLETION.txt and analysis/renderer_render_completion_qualification.json.
No new original routine credit: 1,658 entries / 1,655 unique pairs / 702 links.
Approximately 10% weighted whole-project milestones, not measured code coverage.
Goal active; continue Sol High for the native pixel-rendering backend.

Latest verified step: persistent frame clock and original interrupt delivery
A 290-source research build passes 33 normal original/native comparisons and
four typed-stop controls (599 checks / 530 exact oracles). Four consecutive
original draw/frame calls use explicit elapsed time and register-derived VBlank,
without timer/scan/IRQ sample lists during the automatic phase. Reads do not
advance time. Later calls still need twelve VBlank passes; target frame rates,
physical pixels and the playable ports remain unfinished. See RENDERER_FRAME_CLOCK.txt.
No new original routine credit: 1,658 entries / 1,655 unique pairs / 702 links.
Approximately 10% weighted whole-project milestones, not measured code coverage.
Goal active; continue Sol High for original wait/render-completion continuity.

Latest verified step: native elapsed-cycle TMU0 timer foundation
The new platform core passes 90,677 checks; the original timer initializer
and reader pass 68 comparisons without clock sample lists. Register reads
do not advance time. Full scene clock/scheduler/VBlank integration remains
unfinished; the prior declared-profile frame proof is preserved.
No new original routine credit: 1,658 entries / 1,655 unique pairs / 702 links.
See TMU0_CLOCK.txt. Approximately 10% weighted whole-project milestones,
not code coverage. Goal active; continue Sol High for integration.

Latest verified step: original input context and tick/submission callbacks
The real textured particle sequence now completes original frame wrapper
6050b0 under declared external timer/VBlank inputs. Two new original entries;
1,658 registry entries / 1,655 unique pairs / 702 confirmed native links.
263 comparisons match (262 normal, one retained clock-exhaustion stop);
229 rows and all 36 outcomes covered. See RENDERER_INPUT_CONSTRUCTOR.txt.
Approximately 10% weighted whole-project milestones, not code coverage.
Automatic timing, displayed pixels and the playable ports remain unfinished.
Goal active; continue Sol High.

Original filesystem callback and live interrupt observation

Original 5ffcf0 is now software-qualified and runs the actual shared 6005c2 worker. A fresh 287-source research DLL matches 89 original/native comparisons: 80 normal returns and nine expected typed stops. The real 138-texture/player/particle chain completes both IRQ/RTE paths and reaches uninitialized input read 601eb8. A reference-only firmware observer correction records the live IRQ return address; all guest execution and comparisons remain intact.

See RENDERER_FILESYSTEM_CALLBACK.txt and analysis/renderer_filesystem_callback_qualification.json. Inventory: 1,656 entries / 1,653 unique module-address pairs / 700 confirmed native links. Shared worker code earns no duplicate credit. Approximately 10% weighted whole-project milestones, not measured code coverage. These are incomplete research builds, not a playable port. Goal active; continue Sol High for the original input-context producer.

Original TA list callbacks and frame interrupt integration

Three original callbacks are now software-qualified; canonical604546 is reused without duplicate credit. A fresh286-source research DLL passes100 original/native comparisons, including90 normal returns and10 matching typed stops, plus53 TA-event contract cases/357checks. Original empty submission returns aftertwoIRQs without watchdog recovery. The real138-texture/player/particle chain reaches missing filesystem callback5ffcf0; full empty6050b0 reaches uninitialized input601eb8. TA parameter generation, automatic timing anddisplayed pixels remain unfinished.

See RENDERER_LIST_CALLBACKS.txt andanalysis/renderer_list_callbacks_qualification.json. Inventory:1655entries/1652unique module-address pairs/699confirmed exact-name links. Approximately10%weighted whole-project milestones,not measured code coverage. Incomplete research artifacts,not a playable port. Goal active;continue Sol High.

Native TA command-DMA milestone;frame completion pending

A fresh284-source research DLL adds a TA command-transfer endpoint using two additive authored backend sources;282shared C inputs andall original game code remain unchanged.40 DMA contract cases/387checks/185independent oracles pass.27actual original/native comparisons include25normal returns andtwo exact wait frontiers. The original textured-particle frame transfers224bytes,including the exact160-byte generated quad,then waits honestly at64207e. TA list completion/originalIRQ delivery anddisplayed pixels remain unfinished.

See RENDERER_FRAME_TA.txt andanalysis/renderer_frame_ta_qualification.json. Existing source/proof/build bytes andregistry1652entries/1649unique pairs/696exact-name links are preserved;zero new original routine credit. Approximately10%weighted whole-project milestones,not measured code coverage. Incomplete research artifacts,not playable. Goal active;continue Sol High for TA completion events andoriginalIRQ/callback delivery.

Actual renderer/matrix producer integration

Original startup and actual allocators now produce the renderer context andmatrix stack before the real138-texture/player/particle chain.35 comparisons pass:12 producer tests/76checks/40independent oracles and23 scene calls/487checks/438oracles. Actual160-byte quad8cb7fd20lies inside the actual owned allocation andadvances the original context cursor. No polygon pointer,cursor,screen dimensions ormatrix-stack state is supplied.

Existing284-source DLL/C sources andall prior proofs/builds remain unchanged;no new routine credit orfresh native compile. Registry1652entries/1649unique pairs/696exact-name links. GPU frame consumption/pixels andwhole game remain unfinished;external hardware/disc/scene input contracts andthe invalid matrix-stack guard obligation remain explicit. See RENDERER_CONTEXT_PRODUCER.txt andanalysis/renderer_context_producer_qualification.json. Approximately10%weighted whole-project milestones,not measured code coverage. Goal active;continue Sol High for actual frame submission/consumption.

# Renderer release milestone

Four complete original renderer surface/ownership release helpers are now verified and linked:6052e0,6567f2,663920,665660. All102 instruction rows/14 conditional outcomes execute. Fresh284-source build and strict C checks pass;124 original/native comparisons,760 checks and324 independent exact oracles pass. The texture/player/particle regression also passes20 comparisons/472 checks/429 oracles on the new DLL.

Correction:6052e0/6567f2 release allocations;they do not produce the88fcd4 renderer context. The incoming root argument is ignored. Next trace604702 startup callbacks and the genuine indirect writer. Full renderer frame submission/pixels and all ports remain unfinished. Registry:1652 entries/1649 unique module-address pairs/696 exact-name links in the current DLL. See RENDERER_CONTEXT_OWNER.txt and analysis/renderer_context_owner_qualification.json. Incomplete research artifacts,not playable. Approximately10% weighted whole-project milestones,not measured recovered-code coverage. Goal active;continue Sol High. Texture Off remains planned to disable sampling and use vertex/material colours.

# Textured particle command path: verified software integration

Original game code now loads all 138 EFF_REGULAR texture entries and draws an allocated particle into a 160-byte RAM command buffer. Uploaded textures persist across calls in all 8 MiB of simulated video memory. Primary evidence: 23 comparisons, including 20 scene comparisons / 472 checks / 429 independent oracles plus three continuity tests. This verifies software command staging; GPU submission and displayed pixels remain unfinished. The existing 283-source build is preserved, with zero new routine credit: 1,648 registry entries / 1,645 unique module/address pairs / 692 confirmed unique exact-name links. Retail upload trace values remain private. Approximately 10% weighted milestones, not measured code coverage. See [the guide](PLAYER_TEXTURE_DRAW.txt).

# Texture linker recovery and loader diagnosis: verified research update

Three complete helpers (53 rows / 12 outcomes) are software-qualified; 311 fresh original/native comparisons and 1,299 checks pass. The 1,528-row filename linker and its static helper chain are recovered in a 283-source research build, with a verified missing-file error path; full linker behavior remains unqualified. Registry: 1,648 entries / 1,645 unique module/address pairs; 692 confirmed unique exact-name DLL links. Correction: filename API608c82 is not the next call after the archive loader; the static scene list has null filenames. Actual in-memory decoder608134, using original temporary-list producers, returns failure with first-descriptor error6. Next: trace conversion/graphics setup. Approximately 10% weighted milestones, not measured code coverage; full game and rendering unfinished. See [the guide](PLAYER_TEXTURE_LINK.txt).

# Texture resource read and decode: verified research update

Three complete original initializers (64 rows / 6 conditional outcomes) are now software-qualified. 66 fresh original/native comparisons and 357 checks pass, including actual local EFF_REGULAR.PRS read and decompression with exact complete input/output hashes. Four additive builds lead to 282 sources; all final sources pass strict compilation checks. Registry: 1,645 entries / 1,642 unique module/address pairs; 689 confirmed exact-name unique DLL links. The decoder stages numeric texture IDs; original linking reaches missing 0x8c64c374. Visible rendering and full game remain unfinished. Approximately 10% weighted milestones, not measured code coverage. See [the guide](PLAYER_TEXTURE_RESOURCE.txt).

# Particle spawning and draw wrapper: verified research update

Two more complete originals (347 rows / 14 conditional outcomes) are software-qualified in documented domains. 192 fresh original/native comparisons and 1,118 checks pass, with eight real allocated-child lifecycles and genuine exhaustion/reuse. The combined research DLL now has 686 confirmed exact-name unique routine links; registry: 1,642 entries / 1,639 unique module/address pairs. The existing 278-source build is preserved. Visible drawing reaches the original missing EFF_REGULAR texture descriptor; the real loader is identified and remains to be integrated. Approximately 10% weighted milestones, not measured code coverage; full game and ports unfinished. See [the guide](PLAYER_CHILD_PARTICLES.txt).

# Child records and collision extent: verified research update

Four complete original routines (142 rows / 22 conditional outcomes) are now software-qualified in their tested domains. 215 fresh original/native comparisons and 1,128 checks pass, including 16 actual allocated-child collision/setup/update/record lifecycles and genuine heap exhaustion. The combined research DLL has 684 confirmed exact-name unique routine links; registry: 1,640 entries / 1,637 unique module/address pairs. Overall estimate remains approximately 10% weighted milestones, not measured code coverage. Full attachment, effect update/draw, retail startup and ports remain unfinished. See [the guide](PLAYER_CHILD_RECORDS_AND_EXTENT.txt).

Latest verified milestone: original child initializer0b3a2e and player-character
query07ce40are software-qualified.276-source dependency-connected build;143
original/native comparisons/1,346checks and16actual allocated-child lifecycles.
All68new rows/7feasible outcomes execute;one constant-zero outcome is proven
infeasible and retained. Two canonical helpers receive no duplicate credit.
Current counts:1,636registry rows/1,633unique addresses/680confirmedlinks.
Subsequent callbacks and retail startup remain open. See PLAYER_CHILD_CALLBACK.txt.
Overall estimate remains10% weighted milestones,not measured code coverage.
The full game and ports remain unfinished;goal active.

Latest research: genuine allocated constructor -> original movement normal
initialization -> owner collision response verified across eight lifecycles.
50original/native comparisons/311checks;the actual072f58call returns normally
after initialization. A byte-backed contradictory-gate proof identifies31
retained rows around072e84;no instructions are removed. Whole owner/routing
and retail producer/startup invariants remain open. Counts stay at678confirmed
linked routines and1,634registry rows. See MOVEMENT_RESPONSE_OWNER_LIFECYCLE.txt.
Overall estimate remains10% weighted milestones,not measured code coverage.
The full game and ports remain unfinished;goal active.

Latest verified milestone: complete collision child0344dc and debug-line
wrapper043a40 now software-qualified. 126original/native comparisons and752
checks;all526subject instructions/52outcomes exercised. Current DLL confirms
678unique linked registered routines;1,634registry rows/1,631unique addresses.
Native C and the verified274-source DLL are unchanged;strict compile passed.
A real two-call stack-reuse test exposes the larger response caller validity
contract,which remains open. See COLLISION_RESPONSE_QUALIFICATION.txt.
Overall completion remains10% weighted milestones,not measured code coverage.
The full game and ports remain unfinished;goal active.

Latest verified milestone: complete near-plane line clipping at 0x8c64f5e8.
274-source research build;187 original/native comparisons and1,362 checks,
including544 independent exact oracles. 1,632 registered entries;676 confirmed
unique linked routines. Real debug-enabled response now returns normally.
Broader response initialization/alias/numeric contracts and remaining renderer
branches remain open. See PLAYER_RESPONSE_CLIP.txt and its qualification report.
Overall estimate remains10% weighted milestones,not measured code coverage.
The full game and ports remain unfinished;goal active.

Movement path integration andfour verified shape helpers,2026-11-01
Complete point/direction originals02fc12/02fd32/02fe5c/031a84 qualify:425rows/
56outcomes execute.395fresh comparisons/2173checks pass,including148 exact
semantic cases andeight real allocation-to-movement lifecycles.272-source
build/strict checks pass;4063emitted original rows byte-checked.1631registry
entries/1628unique addresses/675confirmedlinks. Full072b44/0730d8 bodies are
connected butremainunregistered:328/629rows+39/92outcomes and251/2041rows+5/234.
Guide:PLAYER_MOVEMENT_PATH.txt. Missing owner/routing branches,constructor
ownership,actual stack/alias/callback/physical/fullgame/port obligations remain.
Overall10%weighted milestones,not measured code coverage. Goalactive;keep
SolHigh. Incomplete research builds,not a playable game.

Task pool initializer09844a qualification,2026-11-01
Complete114-row body:111rows/12outcomes execute;3rows/4outcomes are proven
unreachable for normal immutable synchronous original-entry execution. All
guards retained.33fresh comparisons/263checks pass,includingeight real
allocator-exhaustion paths.268-source strict compiler checks pass;native
code/DLL unchanged.1627registry entries/1624unique addresses/671confirmedlinks.
Guide:PLAYER_TASK_POOL.txt. Caller072b44 is exported for next recovery;the
constructor call sites0b6316/e01d4 have no indexed owner.0777e0caller/stack
aliases,stored callbacks,fullgame and all port/settings goals remain open.
Overall10%weighted milestone estimate,not measured code coverage. Goalactive;
keepSolHigh. Research libraries remain incomplete and nonplayable.

Player-work constructor/helper lifecycle,2026-11-01
Three complete originals06c350/04b440/07d67c:319 rows/42 outcomes execute.
Original fixed heap initialization,4096-task pool,parent creation and full
constructor run on carried state.317 comparisons/12456 independent checks pass.
268-source build and strict unbounded compiler checks pass;710 original rows
byte-checked.1626 registry entries/1623 distinct addresses/670 confirmed links.
Canonical reuse has no duplicate credit.09844a pool qualification,0777e0sort
caller-alias contract and stored0b3a2e callback remain open. See
PLAYER_WORK_LIFECYCLE.txt. Incomplete research DLLs,not playable.
Overall10%weighted milestone estimate,not measured code coverage. Goalactive;
continueSolHigh. All port,display,SUPIR andvertex-colour TextureOff goals remain.

Contact candidate scan/clamp/response075686 and075f8c,2026-10-28
Two complete originals:645 rows/54 branch outcomes exercised.619 fresh
original/native comparisons and4888 checks pass, including344 semantic cases.
264-source build and strict unbounded compiler checks pass; CPU,guards,
observer,ABI andguest limits are unchanged. Original unset-minimum read,
partial writes, return12 and rounding/underflow direction behavior retained.
1623 registry entries/1620 distinct module-address pairs/667 native links.
16411 old immutable published files/1621 registry rows/2761 builds preserved.
Guide:MOVEMENT_CONTACT_CANDIDATES.txt. Incomplete research DLL;not playable.
Next0777e0 special tail,then0730d8;older collision/callback/precision/physical
and all full-game/port/settings obligations remain. TextureOff disables
sampling/usesvertex-materialcolours. NoSARecompimplementation imported.
Overall10%estimatedweightedmilestones;goalactive;continueSolHigh.

Contact commit/heading-speed pair074afc/076640,2026-10-28
Two complete originals:384rows/32outcomes execute;566fresh original/native
comparisons/4394checks,including286semanticcases.263-sourcebuild/strict
unboundedcompiler checks pass. Exactoldbridge/CPU/guards/observer/ABI retained.
1621registryentries/1618unique module-addresspairs/665confirmed native links.
Prior16366immutable publishedfiles/1619registryrows/2759binaries preserved.
Guide:MOVEMENT_CONTACT_COMMIT.txt. ResearchDLL remains nonplayable.
Next075686/075f8c specialtail dependencies;olderparent/collision/precision/
callback/physical/fullgame obligations remain. NoSARecompimplementationcopied.
Overall10%weightedmilestones;goalactive,continueSolHigh. TextureOffdisables
sampling/usesvertex-materialcolours;allfutureports/settingsremainunfinished.

Latest heading query: original06cd22 software-qualified in the262-source library. All106instruction rows and26conditional outcomes execute;586fresh original/native comparisons/5455checks pass. Actualstage0902/0903 fallback,optional outputs,flag overrides,andlive table/actor-index output aliases verified.1619entries/1616unique/663confirmedregisterednative links. Parent/specialtail/world/collision/callback/precision/physical obligations remain open. See MOVEMENT_HEADING_QUERY.txt. Overall10%weightedmilestones;incomplete,nonplayable research builds.

Latest movement surface resolution: original0747d2/0763a0/07256a software-qualified in the261-source library. All647instruction rows and64conditional outcomes execute;680fresh original/native comparisons/4399checks pass. Twentyfinite-input underflow witnesses close the zero-direction branch without injected results or infeasibility assumptions.1618entries/1615unique/662confirmedregisterednative links. Earlier owner/callback/precision/physical obligations remain open. See MOVEMENT_SURFACE_RESOLUTION.txt. Overall10%weightedmilestones;incomplete,nonplayable research builds.

Latest movement record adjustment: original07506e/0751c0 software-qualified in the260-source library. All164instruction rows and8conditional outcomes execute;484fresh original/native comparisons/3790checks pass. Actual callback position restoration,live record mutation,reloaded offsets,modulo16bit angles andimpulse effects verified.1615entries/1612unique/659confirmedregisterednative links. Unknowncallbacks/retailassignments,olderowner/math/precision/physical obligations remain open. See MOVEMENT_RECORD_ADJUSTMENT.txt. Overall10% weightedmilestones;incomplete,nonplayable research builds.

Latest orientation update: original074ee4 is software-qualified for the stated fixed-original-math scalar profile.154of157rows/23of24outcomes execute;3rows/1outcome are accounted by a source-bound arithmetic range analysis. All original code remains; broader precision/math mutation/physical behavior stays open.465fresh original/native comparisons/4299checks and strict259-source compiler checks pass.1613entries/1610unique/657confirmedregisterednative links. Existing DLL reused unchanged. See MOVEMENT_ORIENTATION_FOLD.txt. Overall10%weighted milestones;incomplete,nonplayable research artifacts.

Latest orientation milestone is PARTIALLY VERIFIED: full original074ee4/078e40/055d20Csource compiled in259-source library;only two helpers registered,79newrows. Main074ee4staysUNREGISTERED with3anglefoldrows/1outcomeopen;noownercredit or assumedatan2rangeproof.708fresh comparisons/5982checks pass;1612entries/1609unique/656confirmedregisterednative links. See MOVEMENT_ORIENTATION_RESPONSE.txt and angle-fold obligation evidence. Older owner/math/precision obligations retained. Overall10%weightedmilestones;incomplete,nonplayable research builds.

Latest movement projection group: original075a40/64016c/650614/640080/63a9dc software-qualified in258-source library;5newroutines plus canonical055d5a reuse with zero duplicate credit.1610entries/1607unique/654confirmedlinked.1151fresh comparisons/8765checks;all1005group rows execute,63of64outcomesexecute and1isprovedonlyunderstableRAM PR0/SZ0softwareprofile. Original infinity/heading/rounding edge cases preserved;broaderprecision and older owner/math obligations remain open. See MOVEMENT_PROJECTION_RESPONSE.txt. Overall10%weightedmilestones;incomplete,nonplayable research builds.

Latest movement/model response group: original0742d4/036bc0/036f12 software-qualified in257-source library;3newroutines,1605entries/1602unique/648confirmedlinked.1125freshcomparisons/10817checks;accepted solid/textured/alpha mesh pipeline verified.904of906rows execute;2integerselectorrows provedunreachable. One numeric outcome proved onlyforcurrentPR0/SZ0 profile;broaderprecision remainsopen andoriginalbranch staysinsource. See MOVEMENT_MODEL_RESPONSE.txt. Older parent/world/collision obligations remain. Overall10%weightedmilestones;incomplete,nonplayable research builds.

Latest canonical movement mode query: existing049a4e connected to the256-source full-state library;zero new original routine credit. Registry remains1602entries/1599unique;645 confirmed linked. All6instructions execute;515 fresh original/native comparisons/3977 checks. Preserved standalone source/proof/ABI and entire registry bytes. Actual parameter-produced raw global values verified. See MOVEMENT_MODE_QUERY.txt. Earlier world/collision/movement parent obligations remain open. Overall10% weighted milestones;incomplete,nonplayable research builds.

Latest movement parameter update: original075214 qualified;255 sources;all216 instructions and15conditional outcomes execute;one remaining outcome has a byte-backed stable-RAM proof.667 fresh original/native comparisons/5211 checks;1599 unique routines/644 confirmed linked. Live selector/table overlaps and actual output-produced next mode verified. See MOVEMENT_PARAMETERS.txt. Earlier world/collision/movement parent obligations remain open. Overall10% weighted milestones;incomplete,nonplayable research builds.

Latest alternate movement response: original075860 qualified;254 sources;all202instructions/20outcomes execute,including an actual original-call-produced stack-history zero-length path.598 fresh comparisons/3714 checks;1598 unique routines/643 confirmed linked.32 real world-selection -> collision -> alternate-response pipelines pass. Python host import startup adjusted;guest execution/guards unchanged. See MOVEMENT_ALTERNATE.txt. Older028bfe/029b00/0730d8 owners remain unregistered. Overall10% weighted milestones;incomplete,nonplayable research builds.

Latest contact response: four original routines 075416/055dac/638eac/639b50 qualified;253 sources;373 new instructions/32 outcomes,plus five-row canonical cosine adapter without duplicate credit. All378 rows execute,including an actual zero-length path omitted by the Ghidra draft.777 fresh comparisons/4473 checks;1597 unique routines/642 confirmed linked. Actual world-selection -> collision -> response sequences pass. Older028bfe/029b00/0730d8 owners remain unregistered. See MOVEMENT_RESPONSE.txt. Overall10% weighted milestones;incomplete,nonplayable research builds.

Latest movement world dependencies: two original routines 0522c0/63a8e0 qualified;252 sources;295 instructions/30 branch outcomes. Larger 028bfe owner remains partial/unregistered:245/247 rows,53/56 outcomes execute,one outcome proved,two open.517 fresh comparisons/3073 checks;1593 unique routines/638 confirmed linked. Actual world-selection -> collision sequences verified. See MOVEMENT_WORLD.txt. Overall10% weighted milestones;incomplete,nonplayable research builds.

Latest movement dependencies: three original routines079000/074cf0/639090 recovered;251sources;449instructions/56executed+2provedoutcomes including three canonical adapters;596freshcomparisons/3566checks.1591unique routines/636confirmedlinked.0730d8 and029b00 remain unregistered. See MOVEMENT_VECTORS.txt. Overall10%weightedmilestones; incomplete, nonplayable research builds.

Latest collision initialization: two complete original owners028e60/04e3de recovered;250sources;165instructions/18outcomes including five reused helpers;361fresh comparisons/2439checks.1588unique routines/633confirmedlinked.029b00 remains partial at73of74outcomes. See COLLISION_SETUP.txt. Overall10%weightedmilestones; incomplete, nonplayable research builds.

Latest collision alias evidence: all1121instructions and73of74branch outcomes executed;12freshcomparisons/186checks. One outcome remains open;caller unregistered.1586unique/631confirmedlinked unchanged. See COLLISION_CONTACT_COUNTER_ALIAS.txt. Overall10% weighted milestones; incomplete, nonplayable research build.

Latest collision work: complete029b00body integrated,249sources,all1121instructions/32call sites and72of74branch outcomes exercised.444fresh comparisons/2651checks. Two outcomes remain open; caller is unregistered and receives0newroutinecredit. Registry unchanged1586unique/631confirmedlinked. See COLLISION_CONTACT.txt. Overall10% weighted milestones; incomplete, nonplayable research build.

Latest collision dependencies: original glyph6407b0 and text640922 recovered, using the actual existing string-length callee. 248 sources, all315instructions/4branch outcomes,486fresh comparisons/4392checks. 1586unique registered routines;631confirmed linked. See COLLISION_TEXT.txt. Overall10% by weighted milestones; incomplete, nonplayable research build.

Latest collision dependency:original triangle renderer63d66c recovered with all15actual gamecallees;247sources,332rows/31executed+1provedinfeasibleoutcome,625freshcomparisons/6035checks.1584unique registered/629confirmedlinked. SeeCOLLISION_RENDER.txt. Overall10%weightedmilestones;incomplete/nonplayable.

Latest collision integration:canonicalinverse6390b0/determinant64f3ec now use fullruntimeABI;246sources,1191originalrows/8outcomes,867fresh comparisons/7545checks.1583unique registered/628confirmedlinked;0duplicate routinecredit. SeeCOLLISION_INVERSE.txt. Overall10%weightedmilestones;incomplete/nonplayable.

Latest collision dependency:canonical63a94c normalization now directly routed through proper existing lowercontext.245-source build,757fresh original/native comparisons/6321checks,52finite-vector oracles.0newroutinecredit;1583unique registered/626confirmedlinked. SeeCOLLISION_NORMALIZE.txt. Overall10%weightedmilestones;incomplete/nonplayable.

Latest wrapper return research:scalar bounds checked;SZ1saved-R14 alias directlyverified in48original/native calls/336checks plus7derivationchecks. Both wrappers still unregistered;no routine-count increase. Existing245DLLunchanged. SeeCOLLISION_WRAPPER_RETURN_RESEARCH.txt. Overall10%weightedmilestones;incomplete/nonplayable.

Latest collision-angle dependency milestone:245-source build;2complete new helpers(51rows/6outcomes),canonical114-row range reducer reconnected without duplicate credit;2publicwrappers remain partial/unregistered. 1,385fresh comparisons/11,471checks pass.1,583unique registered routines/626confirmedlinked. SeeCOLLISION_ANGLE.txt. Overall10%weighted milestones;incomplete/nonplayable.

# Sonic Adventure Dreamcast Rev A decompilation and ports

Latest collision-math stage: The 244-source research DLL qualifies two original collision-math helpers: cross product027360 and vector length63a75c. All41 instructions execute. All 1,135 fresh original/native comparisons and 10,155 independent checks pass, including408 finite mathematical-oracle cases. Registry:1,584 entries /1,581 unique pairs; 624 confirmed linked by registered name. Overall remains about10% by weighted milestones, not code coverage. These are incomplete, non-playable research builds. See [COLLISION_MATH.txt](COLLISION_MATH.txt).

Latest command-17 batch-deletion stage: The 243-source research DLL qualifies original batch-deletion handler 649e00. All 123 original instructions and 27 branch outcomes execute; one additional outcome is proved impossible because original preflight returns zero unconditionally. All 1,847 fresh original/native comparisons and 11,167 independent checks pass. Registry: 1,582 entries / 1,579 unique pairs; 622 confirmed linked by registered name. Overall remains about 10% by weighted milestones, not code coverage. This is a non-playable research build. See [MOVIE_FILESYSTEM_COMMAND17.txt](MOVIE_FILESYSTEM_COMMAND17.txt).

Latest command-16 whole-device-write stage: The 242-source research DLL software-qualifies original command-16 whole-device writer 64b60e: all 54 instructions and four branch outcomes execute. All 8,876 fresh original/native comparisons and 68,029 independent checks pass, including all 512 blocks / 262,144 source bytes through 2,560 data/commit packets and first/later/mid-data/commit errors. Registry: 1,581 entries / 1,578 unique pairs; 621 confirmed linked by registered name. Overall remains about 10% by weighted milestones, not measured code coverage. ACK payload evidence is not physical storage durability. Research DLLs are not playable ports. See [MOVIE_FILESYSTEM_COMMAND16.txt](MOVIE_FILESYSTEM_COMMAND16.txt).

Latest command-15 whole-device-read stage: The new 241-source research DLL software-qualifies original command-15 whole-device reader 64b584: all 54 instructions and four branch outcomes execute. All 3,866 fresh original/native comparisons and 24,032 independent checks pass, including a complete 512-block / 262,144-byte normal-device read and first/later/partial errors. Registry: 1,580 entries / 1,577 unique pairs. Overall remains about 10% by weighted milestones, not code coverage. These research DLLs are not playable ports. See [MOVIE_FILESYSTEM_COMMAND15.txt](MOVIE_FILESYSTEM_COMMAND15.txt).

Latest command-14 range-read stage: The new 240-source research DLL software-qualifies command-14 range reader 64bbb2 and its seek helper 64bb4c: all 128 original instructions and 22 branch outcomes execute. All 1,321 fresh original/native comparisons and 7,185 independent checks pass. Original offset reads, EOF handling, ignored seek errors, read-error propagation and cleanup match in the documented input domains. Registry: 1,579 entries / 1,576 unique pairs. Overall remains about 10% by weighted milestones, not code coverage. These research DLLs are not playable ports. See [MOVIE_FILESYSTEM_COMMAND14.txt](MOVIE_FILESYSTEM_COMMAND14.txt).

Latest command-13 type-CC write stage: The new 239-source research DLL software-qualifies command-13 writer 64b91c and its three type-CC write helpers: 483 original instructions and 74 branch outcomes. All 483 instructions and 72 outcomes execute; two outcomes have explicit, scoped exclusion arguments. All 3,374 fresh original/native comparisons and 22,232 independent checks pass. Type-CC creation, sequential writes, readback verification, metadata errors and cleanup match the original in the documented input domains. Registry: 1,577 entries / 1,574 unique pairs. Overall remains about 10% by weighted milestones, not code coverage. These research DLLs are not playable ports. See [MOVIE_FILESYSTEM_COMMAND13.txt](MOVIE_FILESYSTEM_COMMAND13.txt).

Latest command-12 write/close stage: The new 238-source research DLL software-qualifies command-12 writer 64b71c, its ten dependencies and the previously partial close helper 64919e: 969 original instructions / 148 branch outcomes. All 161 close instructions and 24 close outcomes execute. All 4,635 fresh original/native comparisons and 30,143 independent checks pass. Normal writes, write/readback verification, allocation, timestamps, metadata persistence, errors and cleanup match the original within the documented input domains. Two write-open instructions and three outcomes have scoped domain-exclusion arguments; their source remains intact. Registry: 1,573 entries / 1,570 unique pairs. Overall remains about 10% by weighted milestones, not code coverage. Goal active; these research DLLs are not playable ports. See [MOVIE_FILESYSTEM_COMMAND12.txt](MOVIE_FILESYSTEM_COMMAND12.txt).

Latest command-11 file-read stage: The new 237-source research DLL qualifies five original file-read/handle routines (382 instructions / 68 branch outcomes: 377 instructions and 66 outcomes execute; five instructions and two outcomes have local zero-return proofs). All 1,535 fresh original/native comparisons and 8,380 independent checks pass. Sixteen initialized command-11 read/error tasks cover exact data, EOF, missing names, invalid chains and handle-pool exhaustion; two initialized helper sessions verify alias-triggered open failure. Close helper 64919e is translated but its write branches remain partial and unregistered. Registry: 1,561 entries / 1,558 unique pairs. Overall remains about 10% by weighted milestones, not code coverage. Goal active; research DLLs are not playable ports. See [MOVIE_FILESYSTEM_COMMAND11.txt](MOVIE_FILESYSTEM_COMMAND11.txt).

Latest command-10 record-update stage: The new 236-source research DLL qualifies original command-10 record updater 64a244: 115 instructions and 16 branch outcomes (112 instructions/15 outcomes executed; three instructions/one outcome proved impossible under the unchanged zero-return helper). All 1,558 fresh original/native comparisons and 9,587 independent checks pass, including 16 completed update/error tasks and two pending command-11 frontiers. Exact byte/optional 16-bit field updates, directory writes, relocation, errors, callbacks and cleanup match the original in the declared software-input domain. Non-default policy activation remains unproved. Registry: 1,556 entries / 1,553 unique pairs. Overall remains about 10% by weighted milestones, not code coverage. Goal active; research DLLs are not playable ports. See [MOVIE_FILESYSTEM_COMMAND10.txt](MOVIE_FILESYSTEM_COMMAND10.txt).

Latest command-9 relocation stage: The new 235-source research DLL qualifies command-9 storage-block relocation handler 64a828 and seven helpers. All 489 original instructions and 86 branch outcomes execute in verification. All 2,059 fresh original/native comparisons and 14,353 independent checks pass, including ten completed relocation/error tasks and two pending command-10 frontiers. Original data movement, FAT/directory updates, type-CC admission gate, all 256 dirty flags, counters, callbacks and cleanup are preserved. Registry: 1,555 entries / 1,552 unique pairs. Overall remains about 10% by weighted milestones, not code coverage. Goal active; research DLLs are not playable ports. See [MOVIE_FILESYSTEM_COMMAND9.txt](MOVIE_FILESYSTEM_COMMAND9.txt).

Latest command-8 rename stage: The new 234-source research DLL qualifies original command-8 rename handler 649f1e (116 instructions / 18 branch outcomes: 113 instructions and 17 outcomes execute; three instructions and one outcome have a bytechecked constant-return proof). All 1,528 fresh original/native comparisons and 9,319 independent checks pass, including 16 completed rename tasks and two pending command-9 frontier sessions. Exact 12-byte name changes, missing/duplicate rejection, directory writes, relocation, errors, callbacks and cleanup match the original. Registry: 1,547 entries / 1,544 unique pairs. Overall remains about 10% by weighted milestones, not code coverage. Goal active; research DLLs are not playable ports. See [MOVIE_FILESYSTEM_COMMAND8.txt](MOVIE_FILESYSTEM_COMMAND8.txt).

Latest delete-gateway stage: The existing 233-source research DLL now qualifies delete-command gateway 649d20: 89 original instructions and 15 branch outcomes execute, while three instructions and one outcome are proved impossible under its unchanged zero-return helper. All 586 fresh original/native comparisons and 3,138 independent checks pass. Ten initialized task sessions include completed delete/error lifecycles and two pending command-8 frontiers at missing handler 649f1e. Registry: 1,546 entries / 1,543 unique pairs. Native C and builds are unchanged; no fresh native rebuild is claimed. Overall remains about 10% by weighted milestones, not code coverage. Goal active; research DLLs are not playable ports. See [MOVIE_DELETE_GATEWAY.txt](MOVIE_DELETE_GATEWAY.txt).

Latest persistence stage: The 233-source research DLL qualifies persistence writer 64ab36 and six related helpers (447 original instructions / 56 branch outcomes). All 2,466 fresh original/native comparisons / 17,679 independent checks pass, including 17 initialized command-7 task sessions. Directory/FAT writes, bad-block relocation, abort/no-space/removal errors, original callbacks and cleanup execute through real software request/ACK handling. Original ignored FAT/root errors are preserved. Registry: 1,545 entries / 1,542 unique pairs. Overall remains about 10% by weighted milestones, not code coverage. Goal active; research libraries are not playable ports. See [MOVIE_FILESYSTEM_PERSIST.txt](MOVIE_FILESYSTEM_PERSIST.txt).

Latest RAM-removal stage: The 232-source research DLL qualifies three RAM-removal helpers (108 original instructions / eight branch outcomes). All 1,285 fresh original/native comparisons / 9,113 independent checks pass. Eight actual command-7 task sessions verify RAM changes and the real persistence frontier: missing names complete the original error callback; seven other cases reach missing writer 64ab36 with no invented save or completion. Gateway 649d20 remains partial and unregistered. Registry: 1,538 entries / 1,535 unique pairs. Overall remains about 10% by weighted milestones, not code coverage. Goal active; research libraries are not playable ports. See [MOVIE_FILESYSTEM_DELETE.txt](MOVIE_FILESYSTEM_DELETE.txt).

Latest write-removal stage: The unchanged 231-source research DLL passes 820 fresh original/native comparisons and 4,014 independent checks. Twelve completed raw-removal lifecycles verify three additional write-backend outcomes; two initialized command-7 sessions reach the actual next missing handler 649d20. Backend 660bae now has evidence for 196/206 instructions and 31/34 branch outcomes; its optional callback remains unqualified. No new routine credit: registry stays 1,535 entries / 1,532 unique pairs. Overall remains about 10% by weighted milestones, not measured code coverage. Goal active; research libraries are not playable ports. See [MOVIE_WRITE_REMOVAL.txt](MOVIE_WRITE_REMOVAL.txt).

Latest write stage: The 231-source research DLL qualifies eight helper/wrapper entries (333 original instructions / 42 branch outcomes). All 2,352 fresh original/native comparisons / 18,714 independent checks pass, including 23 original task sessions. Command 20 now completes declared raw-ACK writes, retries, error/removal handling, callbacks and cleanup through scheduler 668e60. Backend 660bae remains partial and unregistered because its optional callback and other branches are unfinished. Registry: 1,535 entries / 1,532 unique pairs. Overall remains about 10% by weighted milestones, not measured code coverage. Goal active; builds are incomplete research libraries. See [MOVIE_FILESYSTEM_WRITE.txt](MOVIE_FILESYSTEM_WRITE.txt).

Latest file-chain stage: The 230-source research DLL qualifies original validator 64b300 (72 instructions / 20 branch outcomes); the existing address helper receives zero duplicate credit. All 1,147 fresh original/native comparisons / 7,951 independent checks pass. Eight actual task sessions verify valid chains reaching the next missing helper 6476e8 and malformed chains completing the original error callback. Wrapper 64bcd6 remains partial and unregistered. Registry: 1,527 entries / 1,524 unique pairs. See [MOVIE_FILESYSTEM_CHAIN.txt](MOVIE_FILESYSTEM_CHAIN.txt). Overall remains about 10% by weighted milestones, not code coverage. Goal active; builds are incomplete research libraries.

Latest file-metadata stage: The 229-source research DLL qualifies six new routines (306 original instructions / 38 branch outcomes). All 1,255 fresh original/native comparisons / 8,363 independent checks pass. Eight original task sessions acquire raw filesystem data, complete 24 command-20 error cases, and reach the actual missing chain validator 64b300 after valid file-information decoding. Wrapper 64bcd6 remains partial and unregistered; two existing primitives receive zero duplicate credit. Registry: 1,526 entries / 1,523 unique pairs. See [MOVIE_FILESYSTEM_COMMAND.txt](MOVIE_FILESYSTEM_COMMAND.txt). Overall remains about 10% by weighted milestones, not code coverage. Goal active; research builds are not playable ports.

Latest linked-directory stage: The 228-source research DLL completes original command 1 initialization through linked directory reads and the original callback in the declared device-input domain. Two bodies qualify (71 original instructions/eight branch outcomes). All 2,515 fresh original/native comparisons / 17,983 independent checks pass, including 36 task lifecycles. Registry: 1520 entries / 1517 unique pairs. See [MOVIE_FILESYSTEM_DIRECTORY.txt](MOVIE_FILESYSTEM_DIRECTORY.txt). Overall remains about 10% weighted milestones, not code coverage. Goal active. Physical acquisition, full filesystem, gameplay and ports remain unfinished; builds are incomplete research libraries.

Latest root-validation stage: The 227-source research DLL connects original root validation and positive table reads. Three entries qualify (144 original instructions / 24 branch outcomes): one new 68-instruction root checker and two unchanged, previously unregistered status/table bodies. All 1,811 fresh original/native comparisons / 15,627 independent checks pass, including 34 actual task lifecycles and 576 direct cases. Valid raw roots and zero/one/two table blocks now reach the real missing directory reader 64a9a8. Registry: 1,518 entries / 1,515 unique pairs. See [MOVIE_FILESYSTEM_ROOT.txt](MOVIE_FILESYSTEM_ROOT.txt). Overall remains about 10% by weighted milestones, not code coverage. Goal active; prior source, proofs, builds and requirements are preserved. Builds remain incomplete research libraries.

Latest filesystem-read stage: The 226-source research DLL connects the original root-storage gateway and retrying reader. Two entries qualify (102 original instructions / 14 branch outcomes); the table-reader loop remains partial and unregistered. All 1,043 fresh original/native comparisons / 5,995 independent checks pass, including 36 actual task lifecycles. Successful command-1 reads now assemble 512 bytes and reach the real missing validator 64a4b2; retry, removal, error and callback paths are verified in their declared domains. Registry: 1,515 entries / 1,512 unique pairs. See [MOVIE_FILESYSTEM_STATE.txt](MOVIE_FILESYSTEM_STATE.txt). Overall remains about 10% by weighted milestones, not code coverage. Goal active; earlier source, proofs, builds and requirements are preserved. Builds are incomplete research libraries.

Latest original-interface stage: The 225-source research DLL uses the original interface initializer/selector and resumes actual task-created requests after submission and raw responses. Five new primitives qualify (24 original instructions / two branch outcomes). All 1,354 fresh original/native comparisons / 12,367 independent checks pass, including 38 task lifecycles. Valid command 18 completes two 256-byte reads and its original callback; valid command 1 reaches the real missing filesystem dependency 64ae32. Full header/block readers and physical Maple acquisition remain unfinished. Registry: 1,513 entries / 1,510 unique pairs. See [MOVIE_INPUT_INTERFACE.txt](MOVIE_INPUT_INTERFACE.txt). Overall remains about 10% by weighted milestones, not code coverage. Goal active; prior source/proofs/builds/requirements are preserved. Builds are incomplete research libraries.

Latest submission stage: the 224-source research DLL connects original command preparation, transport-register handoff and queued-to-inflight request IDs. Six entries qualify (351 original instructions / 46 branch outcomes); canonical preflight gets zero duplicate credit. All 1,247 fresh original/native comparisons / 18,295 independent checks pass, including 64 actual API-to-submission-to-response lifecycles with no injected completed IDs. Physical Maple DMA/controller acquisition and successful active task parsing remain unfinished. Registry: 1,508 entries / 1,505 unique pairs. See [MOVIE_INPUT_SUBMISSION.txt](MOVIE_INPUT_SUBMISSION.txt). Overall remains about 10% by weighted milestones, not code coverage. Goal active; prior source/proofs/builds/requirements are preserved. Builds are incomplete research libraries.

Latest input-pump stage: the 223-source research DLL executes the actual response walker, decoding, publication, rearm and callbacks. Nine entries qualify, covering 706 original instructions and 95 executable branch outcomes; six canonical adapters receive zero duplicate credit. All 2,601 fresh original/native comparisons / 36,399 independent checks pass, including all 48 callback slots and real automatic-pump API returns. A byte-checked proof explains one infeasible subdevice branch without removing it. Queue-to-inflight submission and successful active game parsing remain unfinished. Registry: 1,502 entries / 1,499 unique pairs. See [MOVIE_INPUT_PUMP.txt](MOVIE_INPUT_PUMP.txt). Overall remains about 10% by weighted milestones, not code coverage. Goal active; previous source/proofs/builds/requirements remain preserved. Builds are incomplete research libraries.

Latest request stage: four original lock/packet backend bodies qualify in the 222-source research DLL, covering all 89 instructions and four branch outcomes in the coherent-RAM single-guest-executor domain. All 1,771 fresh original/native comparisons / 23,451 independent checks pass. Active task lifecycles now create actual request packets and resume pending completion/device-busy/mutex-busy waits without duplicate packets or task-state repairs. Actual diagnostic formatting and callbacks execute; enabled automatic pumping still reaches a real integration frontier. The existing pump/preflight source is already registered and will be integrated with zero duplicate credit. Registry: 1,493 entries / 1,490 unique pairs. See [MOVIE_CHANNEL_REQUEST.txt](MOVIE_CHANNEL_REQUEST.txt). Overall remains about 10% by weighted milestones, not code coverage. Goal active; prior source/proofs/builds/requirements are preserved. Builds remain incomplete research libraries.

Latest I/O stage: readiness helper 660dec qualifies in the 221-source research DLL, covering all 38 instructions and six branch outcomes. All 1,139 fresh original/native comparisons / 10,295 independent checks pass. Actual task/API lifecycles now yield inside the I/O reader and resume through parser, decoder and worker frames before original failure cleanup, callbacks and idle resumption. Retry and bad-return guards are verified; no parser results or normal task-state repairs are supplied. Active parsing and request backends remain partial. Eight canonical helpers and 82 nonfunction continuation exports receive no duplicate routine credit; six new nonlocal routes are behavior-qualified. Registry: 1,489 entries / 1,486 unique pairs. See [MOVIE_CHANNEL_IO.txt](MOVIE_CHANNEL_IO.txt). Overall remains about 10% by weighted milestones, not code coverage. Goal active; prior source/proofs/builds/requirements are preserved. Builds remain incomplete research libraries.

Latest decoder stage: three local descriptor cleanup, validation and mode-one gateway bodies qualify in the 220-source research DLL, covering all 158 instructions and 40 conditional outcomes in their declared domains. All 771 fresh original/native comparisons / 7,207 independent checks pass. Original admission and submission APIs now reach the inactive decoder return, real sentinel cleanup, completion callback and later idle resumption. The gateway active tail still reaches a real missing downstream dependency; active decoding remains partial. Five other decoder bodies and the full worker stay unregistered. Registry: 1,488 entries / 1,485 unique pairs. See [MOVIE_CHANNEL_DECODER.txt](MOVIE_CHANNEL_DECODER.txt). Overall remains about 10% by weighted milestones, not code coverage. Goal active; prior source/proofs/builds/requirements are preserved. Builds remain incomplete research libraries.

Latest channel-worker stage: the 219-source research DLL implements the eight selectors, full 21-slot dispatch body and real idle yield/resume continuation. All 427 fresh original/native comparisons / 5,903 independent checks pass, including four-cycle single-channel and zero/four/eight-task interleaved lifecycles. Coverage remains partial: 203/272 translated rows execute; 69 worker rows and four outcomes await actual decoder/callback recovery. The canonical yield thunk receives zero duplicate credit. No new registry credit is claimed; registry remains 1,485 entries / 1,482 unique pairs. See [MOVIE_CHANNEL_WORKER.txt](MOVIE_CHANNEL_WORKER.txt). Overall stays approximately 10% by weighted milestones, not code coverage. Goal active; prior source/proofs/builds/requirements are preserved. Builds remain incomplete research libraries.

Latest task-owner milestone: original pool/owner teardown `668cec` and `668d18` qualify in the 218-source research DLL, covering all 53 instructions and eight conditional outcomes. All 209 fresh original/native comparisons / 2,305 independent checks pass, including actual zero-to-eight-channel admission and teardown. The bounded audit confirms that the examined enqueue path leaves owner+20c unchanged; its wider meaning remains unqualified and original wrap is preserved. Real scheduler prefixes identify the next unresolved channel-worker entries. Registry: 1,485 entries / 1,482 unique pairs. See [MOVIE_TASK_OWNER.txt](MOVIE_TASK_OWNER.txt). Worker exports remain research only; gameplay, timing and the game/ports remain unfinished. Overall stays approximately 10% by weighted milestones, not code coverage. Goal active; prior source, proofs, builds and requirements remain preserved.

This workspace is a continuing reverse-engineering project for the user's Dreamcast Rev A copy. The end goal includes a fully decompiled game, a native PC port with 60/120/240 FPS and resolutions up to 8K, later Android and Nintendo 3DS ports with stereoscopic 3D, and adapted N64 and PlayStation demakes.

**Current whole-project estimate: 10%.** This is a weighted milestone score, not a measure of recovered code. Disc inventory, extraction, and PRS decoding have verification evidence. Game-code recovery is still partial. The PC, Android, Nintendo 3DS, N64, and PlayStation ports are not complete or validated.

Public repository: [wigwoo1/Sonic-Adventure-DC-Decomp](https://github.com/wigwoo1/Sonic-Adventure-DC-Decomp). See `GITHUB_PUBLISHING.txt` for source snapshots, research-build prereleases, exclusions and the daily 18:00 America/Indianapolis update check. Builds are test libraries, not a playable game.

## Earlier verified milestone

**The 202-source research DLL connects three original graphics-memory query routines: all 265 instructions and 60 conditional outcomes execute and qualify.** **233 fresh original/native comparisons / 4,261 independent checks** pass: 137 direct accounting cases, 94 retained root/constructor cases replayed in the new DLL, and two uninterrupted configured startup comparisons. Warm startup reaches 4,357 boundaries; cold reaches 585,655 and retains the full 8 MiB clear, two 2 MiB Sound fills, two real transfers/interrupt returns and all 1,065,282 Sound events with zero drops. Every earlier 201-source boundary matches exactly through `0x8c608d88`; original driver initialization now returns and the caller reaches `0x8c6536ec`. Registry: **1,433 entries / 1,430 unique pairs**. The graphics root still has two unqualified outcomes. Earlier source, proofs, builds and requirements remain intact. See [MOVIE_GRAPHICS_HANDOFF.txt](MOVIE_GRAPHICS_HANDOFF.txt). Builds remain incomplete research libraries; overall stays **10% by weighted milestones**, not measured code coverage, with the goal active.

## Earlier verified milestone

**The original memory-manager constructor now qualifies: 159 instructions, with 124 executed and 35 proved infeasible; seven executed and five proved infeasible branch outcomes.** All **28 fresh original/native comparisons / 588 independent checks** pass. Empty and occupied cleanup lists, complete pool contents and one-bank/two-bank metadata are checked against independent expectations. Actual original helpers run; a checked reset/scanner proof accounts for unreachable descriptor failures. A fresh isolated 201-source build matches the published DLL except its timestamp. Registry: **1,430 entries / 1,427 unique pairs**. The graphics root still has two unqualified outcomes and startup remains at `0x8c608d88`. See [MOVIE_GRAPHICS_CONSTRUCTOR.txt](MOVIE_GRAPHICS_CONSTRUCTOR.txt). Prior source, proofs, builds and requirements are preserved; overall stays **10% by weighted milestones**, not measured code coverage, with the goal active.

## Earlier verification extension

**The graphics root remains partially qualified:** 66 fresh full-entry original/native comparisons and 1,334 independent checks pass. They execute 622 of its 629 instructions and 69 of 74 conditional outcomes; a separate checked immutable-option proof accounts for seven instructions and three outcomes. **Two outcomes still require evidence, so no routine recovery credit is added.** Float normalization, unsigned arithmetic, alignment and actual capacity/copy failures are checked. A fresh isolated 201-source build matches the published DLL apart from its timestamp. Registry remains **1,429 entries / 1,426 unique pairs**; earlier source, proofs, builds and requirements are preserved. Read [MOVIE_GRAPHICS_ROOT_PROGRESS.txt](MOVIE_GRAPHICS_ROOT_PROGRESS.txt) for limits and reproduction. Startup remains at `0x8c608d88`; the goal stays active at **10% by weighted milestones**, not measured code coverage.

## Earlier verified milestone

**Six original graphics mode, framebuffer and reset controls now qualify: 1,024 instructions (1,022 executed and two proved infeasible), with 95 executed and three proved infeasible branch outcomes.** All **367 fresh original/native comparisons / 7,571 independent checks** pass. Earlier 3,179 comparisons and full configured startup evidence are retained and hash-checked, giving 3,546 combined evidence comparisons; they were not freshly rerun. The existing 201-source DLL remains unchanged, and a fresh isolated build matches its complete PE image apart from its timestamp. Registry: **1,429 entries / 1,426 unique pairs**. Original callback timeouts, deferred register returns and actual failure prefixes are preserved. The graphics root remains partial and startup still reaches `0x8c608d88`; no pixels or playable port are claimed. See [MOVIE_GRAPHICS_MODES.txt](MOVIE_GRAPHICS_MODES.txt) and its separate qualification, static proof, rebuild and preservation reports. Overall remains **10% by weighted milestones**, not measured code coverage; the goal stays active.

## Earlier verified milestone

**Seven original graphics/list/allocation routines now qualify: 918 instructions, 150 executed branch outcomes and two checked infeasible outcomes.** The existing 201-source DLL remains unchanged. **706 new original/native comparisons / 9,731 independent checks** pass; 2,473 earlier comparisons and startup evidence are retained and hash-checked. A fresh isolated build matches the entire published DLL apart from its timestamp. Header copies, pointer layout, display sizing, exact/split/exhausted pools, cache reuse and descriptor scanning are verified within declared mapped caller domains. Registry: **1,423 entries / 1,420 unique pairs**. Seven larger graphics bodies remain partial; no new startup advancement or rendered pixels are claimed. See [MOVIE_GRAPHICS_BUFFERS.txt](MOVIE_GRAPHICS_BUFFERS.txt) and the separate qualification, ABI, infeasibility, rebuild and preservation reports. Overall stays **10% by weighted milestones**, not measured code coverage; the goal remains active.

## Earlier verified milestone

The **201-source research DLL connects the original graphics initialization family**. Four supporting routines qualify **352 instructions / 74 outcomes**; five existing adapters receive zero duplicate credit. **2,473 fresh original/native comparisons** pass: 226 focused cases with 2,433 independent checks, all 2,245 earlier cases replayed and two full configured startup comparisons. Startup now returns from graphics initialization and reaches `0x8c608d88`; thirteen larger new bodies remain partial and receive no recovery credit. Cold startup retains the full 8 MiB video clear, two 2 MiB Sound fills, two transfers with original interrupt returns, and all 1,065,282 Sound events with zero drops. Registry: **1,416 entries / 1,413 unique pairs**. Read [MOVIE_GRAPHICS_LEAVES.txt](MOVIE_GRAPHICS_LEAVES.txt) and its separate qualification, startup and preservation reports. Earlier source, proofs and builds remain intact. This verifies configured state transitions; pixels, gameplay and a playable port remain unfinished. Overall completion stays **10% by weighted milestones**, not measured code coverage; the goal remains active.

## Earlier verified milestone

The **200-source research DLL now runs all 32 original MIDI initialization sends and the startup buffer handoff**. One newly recovered sender qualifies **43 instructions / six outcomes**; the existing 11-instruction setter receives zero duplicate credit. **2,247 fresh original/native comparisons** pass: 258 focused cases with 5,262 independent checks, all 1,987 earlier cases replayed and two full configured startup comparisons. Both startups leave 39 real pending entries; all 1,092 queue bytes and channel fields are checked independently. Startup reaches the larger initializer `0x8c656940`. Cold startup retains the full 8 MiB video-memory clear, two 2 MiB Sound fills, two transfers with interrupt returns and all 1,065,282 Sound events with zero drops. Registry: **1,412 entries / 1,409 unique pairs**. See `MOVIE_MIDI_COMMANDS.txt` and the qualification, startup and preservation reports. Earlier source, proofs and builds remain intact. Pending packets do not establish ARM processing or audible output. Overall completion remains **10% by weighted milestones**, not measured code coverage; the goal stays active.

## Earlier verified milestone

The **199-source research DLL adds the original stack-argument MIDI packet helper**. Its 162 instructions remain intact: 147 instructions / 17 outcomes execute with a valid aligned stack, and checked pointer arithmetic proves the remaining 15 instructions / five outcomes cannot execute under that convention. Deliberately misaligned research tests are separately labeled; physical alignment exceptions remain unfinished. **1,989 fresh original/native comparisons** pass: 432 focused packet cases with 9,363 independent checks, all 1,555 earlier cases replayed and two full configured startup comparisons. All 256 message bytes, aliases, tracing, mapped Sound memory and partial failures are checked. Startup now reaches `0x8c643b9c`; cold startup retains the full 8 MiB clear, two 2 MiB Sound fills, two transfers with interrupt returns and all 1,065,282 Sound events with zero drops. Registry: **1,411 entries / 1,408 unique pairs**. See `MOVIE_MIDI_PACKET.txt` and separate qualification, alignment, startup and preservation reports. Packet construction does not prove ARM consumption or audible output. Earlier source, proof and build bytes are preserved. Overall completion remains **10% by weighted milestones**, not measured code coverage; the goal stays active.

## Earlier verified milestone

The **198-source research DLL connects three original audio command controls**: **147 instructions / 26 outcomes** now qualify. **1,557 fresh original/native comparisons** pass: 244 focused cases with 5,466 independent checks, all 1,311 earlier mode, queue, packet and transfer cases replayed, and two full configured startup comparisons. Both startup profiles leave seven actual queued commands and reach `0x8c643c18`. Cold startup retains the full 8 MiB video-memory clear, two complete 2 MiB Sound fills, two real transfers and interrupt returns, and all 1,065,282 Sound RAM events with zero drops. Signed-byte fields, version handling, queue limits, aliases and partial failures are verified. Registry: **1,410 entries / 1,407 unique pairs**. See `MOVIE_AUDIO_CONTROLS.txt` and its qualification, startup and preservation reports. Queue readiness does not establish ARM processing or audible output. Earlier source, proof and build bytes remain intact. Overall completion remains **10% by weighted milestones**, not measured code coverage; the goal stays active.

## Earlier verified milestone

The **197-source research DLL connects the original MONO/STRO mode setter and SDK command sender**: two new bodies qualify **120 instructions / 16 outcomes**. **1,313 fresh original/native comparisons** pass: 177 focused cases, all 1,134 earlier cases replayed and two full configured startup comparisons. Both startup profiles queue one actual stereo command and reach `0x8c6437f0`. Cold startup retains the full 8 MiB video-memory clear, two complete 2 MiB Sound fills, two real transfers/IRQ returns and all 1,065,282 Sound RAM events with zero drops. Real command bytes, queue state, interrupt-mask restoration and partial failures are verified; the shared 20-byte copy tail receives zero duplicate credit. Registry: **1,407 entries / 1,404 unique pairs**. See `MOVIE_AUDIO_MODE.txt`, `analysis/movie_audio_mode_qualification.json` and the separate startup report. Queue readiness does not establish ARM processing or audible output. Earlier source/proof/build bytes and requirements are preserved. Overall completion remains **10% by weighted milestones**, not measured code coverage; the goal stays active.

## Earlier verified milestone

The **196-source research DLL connects original flash-packet storage to an explicit native byte-backed service**. Eight new original bodies qualify **610 instructions / 98 branch outcomes**. **1,136 fresh original/native comparisons** pass: 423 direct cases, 711 earlier cases replayed and two complete configured startup comparisons. The authored backend separately passes 338 cases. Both startup profiles reach `0x8c64387c`; cold startup executes the full 8 MiB video-memory clear, two full 2 MiB Sound RAM fills, two actual 33,376-byte transfers/IRQ returns and all 1,065,282 Sound RAM events with zero drops. Raw platform inputs are explicit. All 1,011 integrated rows execute; settings routine `0x8c64bddc` remains partial at 25/28 outcomes and receives no recovery credit. Registry: **1,405 entries / 1,402 unique pairs**. See `MOVIE_PACKET_FLASH.txt`, `analysis/movie_packet_flash_qualification.json` and the separate startup report. Prior canonical source/proof/build bytes and all user requirements are preserved. Overall completion remains **10% by weighted milestones**, not measured code coverage; the goal stays active.

## Earlier verified milestone

The **194-source research DLL now executes the original startup transfer path through two actual transfers and interrupt returns**, reaching the next missing routine at `0x8c64bddc`. **854 fresh original/native comparisons** pass, including two complete earlier-startup comparisons, 123 direct getter cases, 18 failure-control comparisons and 711 replayed earlier cases. Cold startup performs the full 8 MiB video-memory clear, two full 2 MiB Sound RAM fills and two 33,376-byte transfers; all 1,065,282 Sound RAM events compare with zero dropped events. Three existing getters now use native lower reads: all 70 instructions / six outcomes qualify, with **zero duplicate recovery credit**. Registry remains **1,397 entries / 1,394 unique pairs**. See `MOVIE_SOUND_READ_STARTUP.txt` and `analysis/movie_sound_read_qualification.json` for reproduction, explicit platform inputs, the verification-only counter correction and unfinished hardware/game scope. Prior source/proof/build bytes remain intact. Overall completion stays **10% by weighted milestones**, not measured recovered-code coverage; the goal remains active.

## Previous verified milestone

The **original request routine now qualifies all 139 instructions and all 26 branch outcomes**. Twelve new comparisons / 270 independent checks close the remaining queue-full, current-request, queued-start-error and asynchronous-callback paths using real callees. The unchanged **193-source research DLL** and **699 retained comparisons** remain hash-verified: **711 combined comparisons** support this integration. Earlier suites were not rerun for this proof-only extension. The retained runtime proof includes 32 continuous requests completing real data/interrupt/return paths; asynchronous return still leaves completion pending. Read `MOVIE_REQUEST_COMPLETE.txt` and `analysis/movie_request_complete_qualification.json`. One original body becomes complete; reused callees and authored support receive no duplicate credit. Registry: **1,397 entries / 1,394 unique pairs**. Prior source/proof/build bytes and requirements remain intact. Natural startup, broader asynchronous scheduling, the game and ports remain unfinished. Overall estimate stays **10% by weighted milestones**, not measured code coverage.

## Project files

- `source/` contains recovered C and support code.
- `analysis/` contains manifests, instruction exports, audit results, and verification reports.
- `ghidra/` contains Ghidra export and translation helpers.
- `build_*.ps1` and `verify_*.py` contain focused build and comparison workflows.
- `build_and_verify.ps1` runs the broader project test sequence.
- `project_status.json` records requirements and progress evidence.
- `HD_TEXTURE_AND_DISPLAY_PLAN.txt` records the planned SUPIR HD texture pack, HD toggle, texture percentages, render resolution, and aspect-ratio settings. These features are pending.
- `SOURCE_HANDOFF.txt` contains the source-access and handoff notes.
- `MOVIE_STARTUP.txt` qualifies seven original platform/vector/cache bodies (337 rows / 26 outcomes) in a 162-source build. Sixty direct comparisons / 136 checks and extensive register/cache storage checks pass. The original early caller executes 40/109 rows, creates its real one-MiB heap, then stops at driver `0x8c604702`; the caller remains partial. Two stubs and P2 aliases receive no duplicate credit. Registry: 1,321 entries / 1,318 unique pairs. Retained late-boot controls keep their actual null-heap/context faults. Goal stays active at 10%.
- `MOVIE_HEAP.txt` qualifies three new allocator/initializer/release bodies (178 rows / 22 outcomes), reuses the existing entry thunk, and connects them in a 160-source build. All 365 direct comparisons / 2362 checks and six startup profiles / 120 checks pass. Startup executes allocation but retains its actual null-heap fault at `0x8c6021fe`; no heap/result is injected. Original earlier caller `0x8c010180` is exported for the next task. Registry: 1,314 entries / 1,311 unique pairs. Goal remains active at 10%.
- `MOVIE_RENDER_SETUP.txt` connects the existing decrement helper and three callback setters in a new 159-source build, retaining the original shared installer/default reset. All 170 direct comparisons / 504 checks and six startup profiles / 120 checks pass; all 154 integrated rows / 20 outcomes execute. Four initialized profiles advance to allocation thunk `0x8c6021d2`; both cold controls retain their missing-context fault. Zero duplicate credit; registry remains 1,311 entries / 1,308 unique pairs. The next 74-row allocator is exported only. Overall estimate stays 10%, goal active.
- `MOVIE_TIMING_CONSTRUCTORS.txt` closes the three previously partial timing constructors: 867 executed and 12 proven unreachable rows, with 146 executed and 26 unreachable branch outcomes. All 134 new comparisons / 670 checks pass using the unchanged 158-source DLL. Registry: 1,311 entries / 1,308 unique pairs. Run its focused workflow separately after the retained renderer workflow. Startup remains at `0x8c653386`; the goal stays active at 10%.
- `MOVIE_RENDER_CONFIG.txt` qualifies six new renderer bodies (822 rows), with 59 executed branch outcomes and one original branch proven unreachable. The 158-source integration passes 753 direct comparisons and six startup profiles, advancing to `0x8c653386`. Four helpers are reused; three larger timing constructors remain partial and receive no credit. Registry: 1,308 entries / 1,305 unique pairs. Register-state comparisons do not establish pixels, playable output or requested display settings.
- `MOVIE_CONTEXT.txt` connects original context initialization (312 rows) in the 157-source build and adds a native two-stage entry. Seventy direct comparisons, 144 register cases / 432 alias reads, 28 backend checks, three failure-propagation cases and six startup profiles pass. Four initialized profiles advance past the null-context stop to renderer configuration `0x8c6526bc`; two cold controls retain it. Three existing bodies receive zero duplicate credit. This establishes staged integration, not natural boot or playable output.
- `MOVIE_HARDWARE.txt` qualifies four new original control/copy/wait bodies (400 rows / 78 outcomes) and integrates eight existing helpers. The 155-source build passes 400 direct comparisons, 5,000 FIFO samples, 168 control-subword cases, 40 API checks and four startup profiles. Startup retains its actual null-context fault; the original 193-row context initializer is identified for the next task. Registry: 1,302 entries / 1,299 unique pairs. No playable port or full startup credit.
- `MOVIE_TRANSITION.txt` connects the original display wait in the 153-source build: 103 direct comparisons, 5,000 backend samples, 23 backend checks and ten continuous startup profiles. The original mask is `0x01ff`; 12 regression cases expose an overbroad mask in the preserved historical helper. Advancing profiles now reach the unrecovered MMIO control body `0x8c6046c0`. Explicit raw timing observations do not establish actual display timing, pixels or gameplay. Zero new routine credit.
- `MOVIE_STATUS.txt` describes the original A/V status query in the 151-source native integration: 118 direct comparisons, all 65,536 sampled port words, 24 backend checks and 18 continuous boot profiles. Both helpers were already registered; zero new routine credit. Elapsed-clock paths reach the next display wait at `0x8c604594`. Host port samples are explicit inputs; physical cable sensing, natural startup and gameplay remain unverified.
- `MOVIE_ENTRY.txt` records the raw MOVIE image at its observed `0x8ceb0000` base, 536 unverified function candidates, native startup integration, 62 direct comparisons and eight continuous boot profiles. The entry thunk and two closed helpers qualify; MOVIE startup remains partial at 53/364 tested rows, with the elapsed-time path reaching status query `0x8c10d914`.
- `BOOT_CACHE.txt` preserves the original cache-invalidation leaf at both address aliases: 88 direct comparisons, 130 CCR backend cases, 20 additional checks and four earlier boot prefixes reaching `MOVIE.BIN`. One physical body is registered; boot remains partial at 33/51 rows.
- `BOOT_DISC.txt` preserves the original 50-row loader with native disc I/O: 82 direct comparisons, 34 backend/API checks and four earlier boot prefixes loading `MOVIE.BIN` with its verified file hash before the uncached cache frontier.
- `BOOT_FILE.txt` preserves eight qualified file helpers: 403 direct comparisons, all 222 qualified rows / 30 outcomes, six five-stage handle lifecycles and four earlier boot prefixes reaching the `MOVIE.BIN` firmware read boundary.
- `BOOT_EVENT.txt` preserves native integration of nine existing event bodies, with zero duplicate credit: 544 comparisons, 8,192 VBR checks, full 529-row/72-outcome coverage and eight earlier boot prefixes to the file-load path.
- `BOOT_INPUT.txt` preserves six new input bodies and the complete wait caller: 503 direct comparisons, four three-poll lifecycles and 24 earlier connected wait/boot checks.
- `BOOT_INTERRUPT.txt` preserves nine completed interrupt/completion/submission bodies: 589 comparisons, all 2,180 adapter rows, 66 backend checks and sixteen earlier wait/boot prefixes.
- `BOOT_SUBMIT.txt` preserves the earlier transfer-ring/draining milestone and its partial submission evidence, superseded by the qualified software completion paths in `BOOT_INTERRUPT.txt`.
- `BOOT_DEVICE.txt` records the complete 631-row frame command finalizer and five new dependencies, 418 direct comparisons, twelve actual selector/frame sequences and bounded command-packet capture. Wait execution now reaches `0x8c641f00` at 57/91 rows; complete rendering remains unfinished.
- `BOOT_WAIT.txt` records four complete clock/arithmetic/callback helpers, 304 direct comparisons and twelve actual boot prefixes. The wait/setup routine remains partial at 46/91 rows and stops at device callee `0x8c642440`.
- `BOOT_SETUP.txt` records native task cleanup and render-register setup, 106 direct comparisons, and 12 partial boot-caller prefixes that reach the next actual wait/setup dependency. It also documents the corrected callback return-address test and preserved earlier artifacts.
- `STARTUP_CALLBACK_FLAG_RESET.txt` records the complete 14-instruction helper called first by the partial boot caller at `0x8c10b840`, with 48 full-state comparisons. The indirect task-disposal callback remains a modeled boundary in that helper test, so natural boot ordering is still pending.
- `DRAW_FRAME_SEQUENCE.txt` records three packet/context helpers and the complete renderer initializer: 103 context comparisons, six persistent view/initializer/repeated-initializer sequences and two cold-entry guards. Natural boot ordering and visible rendering remain unverified.
- `DRAW_FRAME_SETUP.txt` records three matrix/screen/camera helpers, 92 comparisons, and the preserved earlier partial initializer proof.
- `verify_draw_angle_resume.py` reruns all 60 angle boundary cases while preserving the earlier proof report and checking identical results.
- `OBJECT_DRAW_CLIPPING.txt` records 156 complete original-object caller comparisons, real textured/environment clipping calls and exact queue-trace limits. The outer draw body remains partial at 527/911 rows.
- `DRAW_VIEW_COMPLETE.txt` records the complete original view builder: 28 comparisons, all 166 rows / six outcomes, 142 register-service comparisons and 18,447 host-backend operations. Physical rendering and natural startup remain unfinished.
- `DRAW_VIEW_SETUP.txt` preserves the preceding four complete view dependencies: 182 comparisons, all 217 rows / two outcomes, 158 independent assertions, and eight earlier partial view-builder comparisons.
- `DRAW_MATERIAL_DISPATCH.txt` records the complete material-block dispatcher: 116 comparisons, all 149 rows / 20 outcomes, 336 independent routing/state assertions and real native clipping children. Full rendering remains unfinished.
- `DRAW_CLIPPING_COMPLETE.txt` records seven complete qualified clipping/primitive bodies: 682 comparisons, all 1,739 rows / 138 outcomes, and 13,220 independent geometry/packet assertions. Natural startup, visible rendering and full gameplay remain unfinished.
- `DRAW_CLIPPING_STATE.txt` preserves the earlier integer-bank/MACH foundation: 12,868 kernel checks, 2,816 prefix comparisons and eight helper regressions. Its zero-credit prefix evidence is distinct from the later full-body registrations.
- `DRAW_VECTOR_BLEND.txt` records two complete vector/integer-angle blend helpers, 1,332 focused comparisons, 108 vector-stage and 56 integer-stage caller checks, and 48 newly completed animation callback cases. Full rendering remains unfinished.
- `CLIPPING_CPU_STATE_PLAN.txt` records completed processor-state and local clipping milestones, with broader model/camera/startup/frame verification still pending.
- `DRAW_ANGLE_CURVE.txt` records the complete integer-angle helper, 811 focused and 108 connected comparisons, independent arithmetic and conversion checks, and the next dependency at 8c60a1f6. Full rendering remains unfinished.
- `DRAW_VECTOR_CURVE.txt` records the complete unsigned-label vector helper, 751 focused and 108 targeted connected comparisons, exact staged arithmetic and stack checks, and the next angle continuation. Full rendering remains unfinished.
- `DRAW_PRIMITIVE_STREAM.txt` records three complete packet/scale helpers, 189 focused and 210 connected comparisons, 156 tested callback returns, partial clipping/state requirements, and the next drawing continuation. Full rendering remains unfinished.
- `DRAW_MATERIAL_SETUP.txt` records six new texture/header/colour-cache helpers plus three reused helpers, 503 focused and 210 connected-model comparisons, software queue packets and the next primitive/sampling dependencies. Full rendering remains unfinished.
- `DRAW_GEOMETRY_STREAM.txt` records four new complete projection, angle-sampling, lighting and viewport helpers plus an existing header helper, 912 focused and 210 connected-model comparisons and the next material/primitive dependencies. Full rendering and gameplay remain unfinished.
- `DRAW_GBR_GEOMETRY.txt` records seven complete renderer/GBR/geometry helpers, 742 direct and 210 connected comparisons, real renderer-initialization stages, explicit register-state and relocation checks, and the next geometry/view dependencies. Full rendering and gameplay remain unfinished.
- `DRAW_MOTION_SAMPLING.txt` records 25 newly registered key-search/interpolation/sampler/format helpers, 2,440 direct and 146 affected-caller/regression comparisons, exact source provenance and numerical limits, and the next renderer/GBR dependencies. Fifteen motion proposals and the full renderer remain partial.
- `DRAW_MOTION_VARIANTS.txt` records twelve complete motion/drawing setup, selector and default-pose/buffer helpers, 43 native proposals, focused original-instruction comparisons and the next missing geometry/interpolation services. The renderer remains partial.
- `DRAW_OVERLAY_FADE.txt` records five complete fade/text/quad/state helpers and a guarded existing matrix helper, 556 direct and 420 connected comparisons, accepted object-callback returns and the remaining motion/drawing boundaries. Visible frames and gameplay remain unfinished.
- `DRAW_POLYGON_PRIMITIVES.txt` records seven complete clipping/header/projection helpers, 576 direct and 408 connected comparisons, software queue output and the next overlay fade/draw dependency. Visible frames and gameplay remain unfinished.
- `DRAW_RENDER_HEADERS.txt` records four complete render-state/header/visibility/size helpers, 1,974 direct and 408 connected comparisons, expanded computed branches, and the next polygon clipper boundary. The whole renderer and visible gameplay remain unfinished.
- `DRAW_OBJECT_EFFECTS.txt` records six complete animation/effect profiles (1,492 instructions / 130 outcomes), 1,720 direct and 408 original-model checks, native dispatcher/render-service prefixes, and the next lower texture/render-state dependencies. Incomplete drawing bodies remain unregistered; visible rendering/gameplay is unfinished.
- `DRAW_POINT_TRANSFORM.txt` records the complete original point transform, 435 direct and 408 original-model comparisons, independent overlap/arithmetic checks, the next dispatcher boundary, and read-only analysis of its dependency family. Visible rendering and gameplay remain unfinished.
- `DRAW_HIERARCHY.txt` records five new hierarchy/matrix helpers plus extended bank-save evidence: 595 direct and 408 original-model comparisons, real vector support, exact original-loop/coverage limits, and the next point-transform dependency. The full game and playable rendering remain unfinished.
- `DRAW_MOTION_MESH.txt` records the distinct motion mesh preprocessing/wrapper, 130 full-state and packet checks, native connection to all nine material families, and initialized-model integration. Natural texture/renderer setup and physical GPU output remain unfinished.
- `DRAW_MOTION_FORMATS.txt` records fifteen complete animation format, keyframe and linear/cubic interpolation bodies: 400 original comparisons, all 1,042 rows/52 outcomes, independent curve checks and original-model integration. Motion mesh output remains unfinished.
- `DRAW_MOTION_SETUP.txt` records seven complete motion/control/base-transform bodies, 544 focused original comparisons, independent callback/world-matrix checks and partial original-model integration. Animation handlers and motion mesh output remain explicit boundaries.
- `DRAW_DISTANCE_HELPERS.txt` records complete distance/signed-sqrt bodies: 578 original checks, 2,112 independent FSQRT checks and 408 partial draw-integration checks, with 421/911 draw rows executed and explicit remaining boundaries.
- `OVERLAY_DRAW_STATE.txt` records fourteen reused native services, 244 original-state comparisons and expanded partial draw coverage (273/911 rows), with three explicit continuation boundaries and zero new routine credit.
- `DRAW_PREDICATE_SUPPORT.txt` records three complete draw helpers, 676 focused comparisons, and 100 partial original-draw-entry checks; the compiled 911-row draw candidate remains unregistered with 825 unexecuted rows.
- `DRAW_MATRIX_REGISTERS.txt` records complete register-input scale/translation: 856 focused checks, 64 real transformed-model caller cases and 120 lifetime regressions, with exact vector-proof and reachability limits.
- `OVERLAY_PARAMETER_UPDATE.txt` records the native parameter/event and scale helpers: 1,000 focused comparisons and 32 original-model lifecycles / 512 stages. Includes exact morph-request behavior and the remaining draw/caller dependencies.
- `OVERLAY_INITIALIZER_COMPLETE.txt` records eight new routines, 4,464 focused comparisons and 402 complete-constructor checks with zero game-service fixtures. Documents shared accessory writes and the corrected parameter-update/draw labels.
- `OVERLAY_MODEL_BLEND.txt` records the native 813-row model morph and full 58-row copy dispatcher, 1,626 focused comparisons, 228 shipped-object initializer checks, and exact coverage/guard limitations. Six outer dependencies remain.
- `OVERLAY_OBJECT_INITIALIZER.txt` records three native material variants and allocation-through-selector integration: 2,177 material comparisons, 384 guarded-selector checks and 432 combined tests. Documents seven remaining outer dependencies.
- `OVERLAY_OBJECT_SETUP.txt` covers four native object-preparation routines, 1,552 original-code comparisons, real heap/copy integration, shipped 39-node trees and the exact coverage exclusions. Initializer/appearance-variant integration is next.
- `MAIN_DISPATCH_SPAWN_SETTER.txt` covers the two newly verified helpers, 2,517 full comparisons, native dependency integration, and the remaining cancellation/callback work.
- `MAIN_DISPATCH_CANCELLATION.txt` records the later cancellation/restore recovery, 4,284 comparisons, and a tested native prefix chain with no external game-service fixtures; stored callbacks remain pending.
- `MAIN_DISPATCH_CONTEXT_CALLBACK.txt` covers the subsequently discovered callback at `8c01b358` and 4,908 native creation/scheduling/deletion stage comparisons.
- `MAIN_DISPATCH_SPAWN_CALLBACK.txt` covers spawn callback `8c044680`, its transform/emission helpers, 612 body comparisons and 368 connected lifecycles; particle callback execution remains pending.
- `MAIN_DISPATCH_PARTICLE_CALLBACK.txt` records the subsequent particle callback recovery, five new entries, 471 focused comparisons and 120 complete lifetimes; low-level geometry remains a test fixture.
- `MAIN_DISPATCH_PARTICLE_GEOMETRY.txt` covers the later native geometry preparation/model wrapper: 548 focused comparisons and 120 complete lifetimes passed; recursive model traversal remains a fixture.
- `MAIN_DISPATCH_MODEL_TRAVERSAL.txt` records subsequent native recursive traversal: 1,494 focused comparisons and 120 complete lifetimes passed; the mesh body remains a fixture.
- `MAIN_DISPATCH_MESH.txt` covers the later native mesh/material loop and filter wrapper: 663 focused comparisons and 120 complete lifetimes passed; 27 lower services remain fixtures.
- `MAIN_DISPATCH_MESH_SERVICES.txt` records the next five native filter/material services: 1,416 focused comparisons and 120 lifetimes passed; documents transfer guards and corrected renderer-state semantics.
- `MAIN_DISPATCH_VERTEX_WORKERS.txt` covers four subsequently recovered vertex workers and native store queues: 265 focused comparisons and 120 lifetimes passed, including 9,680 original-mesh vertex transfers.
- `MAIN_DISPATCH_ACQUISITION.txt` covers the next native header-acquisition helper: 152 focused cases and 120 lifetimes passed, including retained queue words and real output-cursor advancement. Also records the cancellation recheck.
- `MAIN_DISPATCH_PRIMITIVES.txt` covers the two subsequent primitive emitters: 546 focused cases and 120 lifetimes passed, with 1,178 reachable rows checked and four rows proven unreachable. The next near-plane clipping dependency remains a fixture.
- `MAIN_DISPATCH_NEAR_CLIP.txt` records the later native near-plane clipper: 408 focused cases, independent clipped geometry, 24 connected mesh cases and 120 particle lifetimes passed. That tested lifetime path now uses no external game-service substitutes; visible rendering remains unverified.
- `MAIN_DISPATCH_ALTERNATE_STRIPS.txt` covers the two subsequent alternate strip emitters: 320 focused cases, including 48 connected mesh cases, plus 120 lifetime regressions passed. Existing stub and clipping dependencies are reused; three acquisition variants are inventoried for the next task.
- `MAIN_DISPATCH_ACQUISITION_VARIANTS.txt` records the subsequent three native header variants: 352 cases / 1,864 stage comparisons, 908 independent packets and 120 lifetime regressions passed. All four material-loop acquisition selectors are now native; sixteen other primitive emitters remain unfinished.
- `MAIN_DISPATCH_LIT_PRIMITIVES.txt` records the later normal-lit family: 606 original-code comparisons, 1,706 reachable rows, independent RGB/alpha/cache/packet checks, 48 connected meshes and 120 lifetime regressions. Three lower lighting helpers and fourteen other primitive emitters remain unfinished.
- `MAIN_DISPATCH_LIT_NEAR_CLIP.txt` covers subsequent native near-plane lighting: 566 focused cases, all 564 rows, 48 fully native clipped meshes with independent colour interpolation, and 120 lifetime regressions. Two alternate lighting helpers and fourteen other primitive emitters remain unfinished.
- `MAIN_DISPATCH_LIT_ALTERNATE_STRIPS.txt` records the later alternate lighting helpers: 392 comparisons, all 760 rows, 96 native connected meshes, independent lit triangles/cache write-back events, and 120 lifetime regressions. Fourteen other primitive emitters remain unfinished.
- `MAIN_DISPATCH_UV_LIT_PRIMITIVES.txt` covers subsequent signed texture-coordinate/lighting writers: 638 comparisons, all 1,771 reachable rows, 48 connected meshes, retained queue words/UV staging, and 120 lifetime regressions. Three lower UV helpers and twelve other primitive emitters remain unfinished.
- `MAIN_DISPATCH_UV_NEAR_CLIP.txt` records subsequent native UV near clipping: 614 comparisons, all 616 rows, exact signed UV quantization, 96 fully native connected meshes, and 120 lifetime regressions. Two alternate UV helpers and twelve other primitive emitters remain unfinished.
- `MAIN_DISPATCH_UV_ALTERNATE_STRIPS.txt` records the subsequent two native UV strip helpers: 512 comparisons, all 839 rows, 192 connected meshes and 120 lifetime regressions. Twelve other primitive emitters remain unfinished; the next pair has 2,153 byte-checked analysis rows.
- `MAIN_DISPATCH_ENVIRONMENT_PRIMITIVES.txt` records subsequent normal-derived UV primitives: 630 comparisons, 2,149 reachable rows, 48 connected meshes and 120 lifetime regressions. Three lower helpers remain explicit fixtures; their 1,337 exported rows are ready for recovery.
- `MAIN_DISPATCH_ENVIRONMENT_NEAR_CLIP.txt` records subsequent native environment clipping: 614 comparisons, all 675 rows, two UV quantization steps, 96 native connected meshes and 120 lifetime regressions. The two alternate helpers remain unfinished.
- `MAIN_DISPATCH_ENVIRONMENT_ALTERNATE_STRIPS.txt` records the subsequent two native environment strip helpers: 544 comparisons, all 662 rows, 224 connected meshes and 120 lifetime regressions. Ten other primitive emitters remain unfinished.
- `MAIN_DISPATCH_FACE_LIT_PRIMITIVES.txt` records the subsequent face-lit pair and numerical normalizer: 870 comparisons, 1,871 reachable rows and 120 lifetime regressions, with explicit strip-register preconditions, host guards and three lower fixtures.
- `MAIN_DISPATCH_FACE_NEAR_CLIP.txt` records subsequent native face-colour clipping: 644 comparisons, all 410 rows, 96 native connected meshes and 120 lifetime regressions, with explicit parent/register and host-guard limits.
- `MAIN_DISPATCH_FACE_ALTERNATE_STRIPS.txt` records subsequent native face-selected strips: 544 comparisons, all 830 rows, 224 connected meshes and 120 lifetime regressions. Ordinary output uses per-vertex lighting; crossings use normalized face colour.
- `MAIN_DISPATCH_UV_FACE_PRIMITIVES.txt` records the subsequent UV/face-factor pair: 702 comparisons, 2,038 reachable rows and 120 lifetime regressions, with explicit strip-register preconditions, host guards and three lower fixtures.
- `MAIN_DISPATCH_UV_FACE_NEAR_CLIP.txt` records native signed-UV clipping with supplied face factors: 716 comparisons, all 550 rows, 96 native crossing meshes and 120 lifetime regressions.
- `MAIN_DISPATCH_UV_FACE_ALTERNATE_STRIPS.txt` records the alternate UV-face helpers: 544 comparisons, all 887 rows, 224 entirely native meshes and 120 lifetime regressions.
- `MAIN_DISPATCH_UV_CONSTANT_PRIMITIVES.txt` records the constant-factor UV pair: 642 comparisons, all 1,419 reachable rows, 64 connected packet/staging meshes and 120 lifetime regressions, with three lower helpers still fixtures.
- `MAIN_DISPATCH_UV_CONSTANT_NEAR_CLIP.txt` records native constant-factor UV clipping: 764 comparisons, all 553 rows, 192 fully native crossing meshes and 120 lifetime regressions.
- `MAIN_DISPATCH_UV_CONSTANT_ALTERNATE_STRIPS.txt` records both alternate helpers: 768 comparisons, all 799 rows, 448 entirely native meshes and 120 lifetime regressions.
- `MAIN_DISPATCH_ENVIRONMENT_FACE_PRIMITIVES.txt` records generated environment UVs with face lighting: 710 comparisons, all 2,541 reachable rows, 48 native packet/staging meshes and 120 lifetime regressions.
- `MAIN_DISPATCH_ENVIRONMENT_FACE_NEAR_CLIP.txt` records native environment-UV clipping with supplied face factors: 716 comparisons, all 612 rows, 96 fully native meshes and 120 lifetime regressions.
- `MAIN_DISPATCH_ENVIRONMENT_FACE_ALTERNATE_STRIPS.txt` records both alternate helpers: 544 comparisons, all 710 rows, 224 entirely native meshes and 120 lifetime regressions.
- `MAIN_DISPATCH_ENVIRONMENT_CONSTANT_PRIMITIVES.txt` records the final mesh primitive parent pair: 666 comparisons, all 1,785 reachable rows, 96 native packet/staging meshes and 120 lifetime regressions; three lower helpers remain fixtures.
- `MAIN_DISPATCH_ENVIRONMENT_CONSTANT_NEAR_CLIP.txt` records native constant-factor environment clipping: 764 comparisons, all 611 rows, 192 fully native crossing meshes and 120 lifetime regressions.
- `MAIN_DISPATCH_ENVIRONMENT_CONSTANT_ALTERNATE_STRIPS.txt` records the final two explicit primitive helpers in this bounded mesh chain: 768 comparisons, all 670 rows, 448 fully native meshes and 120 lifetime regressions; full frame/GPU/gameplay integration is unfinished.
- `MAIN_DISPATCH_MESH_FAMILIES.txt` records 1,040 comparisons across all nine native material families, original live-register strip behavior and 120 linked lifetime regressions; no new routine credit or hardware/gameplay claim.
- `MAIN_MODEL_ASSETS.txt` records 860 structural model candidates in the original executable, a bounded model parser and 192 native comparisons using 24 unchanged original models; external assets and actual gameplay reachability remain open.
- `OVERLAY_MODEL_ASSETS.txt` records 42 mapped overlay scans, 392 native original-model comparisons and one newly recovered AL_MAIN model selector verified in 2,176 cases; scene/gameplay and physical rendering remain unverified.
- `OVERLAY_SELECTOR_INITIALIZER.txt` records the recovered AL_MAIN initialization caller and marker reset: 2,528 full-state comparisons with native selector/no-op integration; eight other dependencies remain explicit services.
- `STREAM_RECORD_LIFECYCLE.txt` explains the connected stream tests and the allocator callback-context correction.
- `STREAM_PENDING_CLEANUP.txt` covers the recovered pending-I/O cleanup caller and its remaining external abort dependency.
- `REQUEST_ABORT_RECOVERY.txt` documents the native abort control flow, including both wait loops and interrupt masking, and the device/finalizer service gaps.
- `STREAM_CLEANUP_ABORT_CONNECTED.txt` covers the linked native cleanup/abort chain and propagation of nested host failures.
- `REQUEST_FINALIZER_RECOVERY.txt` describes native finalization/status, progress-sensitive timeout handling, and the removal of the finalizer fixture from the connected chain.
- `REQUEST_DEVICE_CALLBACKS.txt` maps default device targets and documents native cancellation down to the external runtime service vector.
- The same guide covers the recovered original seek/query caller at `0x8c65bfa4`, its native callees, and the expanded 328-step lifecycle suite.
- `../../work/sonic-adventure/` contains local extracted disc files, analysis data, and generated build products.

## Main executable draft analysis

`analysis/ghidra-all/` contains the full Ghidra draft export for `1ST_READ.BIN`: 3,637 indexed functions, 3,631 draft pseudocode outputs, and six decompilation failures. The report also records 3,777 warnings in the drafts. This is an analysis starting point only; draft pseudocode is not buildable or behavior-verified source, and the index does not finish executable code/data boundaries. `analysis/ghidra/` is a smaller 150-function export and should not be used as the whole-binary draft count. The completion audit reads the full export summary and reports these counts without treating them as recovered code.

The current disc inventory identifies the source directory as `D:\Desktop\Sonic Adventure (USA) (En,Ja,Fr,De,Es) (Rev A)`. Keep the original disc files and generated binaries in the local `work/` area. Review the handoff notes before preparing a source-only GitHub repository.

## Repeating extraction and resource decoding

Use a directory that contains one `.gdi` file and all of its track files. Choose a new or empty output directory so the extraction does not mix with older results. From the project directory, run:

The examples below use the Python 3.12 interpreter bundled on this development workstation. On another computer, replace `$Python` with that computer's Python 3 executable.

```powershell
$Python = 'C:\Users\wigwo\.cache\codex-runtimes\codex-primary-runtime\dependencies\python\python.exe'
& $Python .\extract_disc.py "D:\path\to\GDI-and-track-files" "..\..\work\sonic-adventure\extracted" --all
```

The extraction script writes the boot executable, boot header, disc files, and a hash-bearing inventory. To decode extracted `.PRS` resources and create a decode manifest, run:

```powershell
$Python = 'C:\Users\wigwo\.cache\codex-runtimes\codex-primary-runtime\dependencies\python\python.exe'
& $Python .\decompress_prs.py "..\..\work\sonic-adventure\extracted\files" "..\..\work\sonic-adventure\decoded" ".\analysis\prs_decode_manifest.json"
```

Compare new extraction and decode results with `analysis/extraction_verification.json` and `analysis/prs_native_verification.json` before relying on them. The existing reports cover the current extracted copy; they do not automatically validate a different disc image.

## Rebuilding and checking the latest cleanup recovery

From this project directory, the focused cleanup routine can be rebuilt and checked with:

```powershell
$Python = 'C:\Users\wigwo\.cache\codex-runtimes\codex-primary-runtime\dependencies\python\python.exe'
$Workspace = (Resolve-Path ..\..).Path
.\build_input_service_cleanup_native.ps1 -Workspace $Workspace
& $Python .\verify_input_service_cleanup_8c646654.py
```

The verification report is `analysis/input_service_cleanup_8c646654_verification.json`. It records 456 full-state comparisons against the retail interpreter, including every slot in the 48-entry callback table. The callbacks themselves use explicit test stubs, so these checks verify the cleanup routine and its dispatch paths, not every callback's game-specific implementation.

The caller at `0x8c647198` is also checked with the recovered cleanup routine connected as a native callee. Running `build_and_verify.ps1` rebuilds the two native libraries and checks the caller alone plus four integrated scenarios; the integration report is `analysis/input_service_cleanup_integration_verification.json`. Device service and record-writing behavior remain explicit stubs in those scenarios, and the runtime callback records are empty.

The same project test sequence rebuilds and checks the adjacent input-device wait/reset routine at `0x8c65f2a4`. Its four comparisons cover immediate status release, the first timeout, and status release or timeout in the second wait. The timer helper behavior is a deterministic test fixture; see `analysis/input_device_wait_reset_8c65f2a4_verification.json` for its exact limits.

The busy-semaphore path in input-service entry `0x8c647198` now runs the recovered report-callback body at `0x8c65f348` instead of treating that routine itself as a no-effect callback. The callback invokes the recovered forwarding wrapper at `0x8c10d400`; its downstream formatter at `0x8c10dbc4` and optional runtime callback remain no-effect fixtures. Reports for both recovered routines are in `analysis/input_device_report_callback_8c65f348_verification.json` and `analysis/debug_log_forward_8c10d400_verification.json`.

The 16-byte wrapper at `0x8c038dfc` now has a native recovery. It saves PR for its first indirect callback, restores the stack, then tail-calls a second callback. Its verifier checks both callback targets and order plus complete RAM, GPR, SR, PR, and FPU state over 24 cases; see `analysis/callback_pair_tail_8c038dfc_verification.json`.

The formatter at `0x8c10dbc4` now has partially verified native execution. Its parent routine passed retail comparisons for literals, character/string arguments, integer formatting, width/precision, native emission, output buffering, and mixed conversions. The 514 instruction-byte integer-conversion routine at `0x8c10e304` independently passed 288 full-state comparisons; adjacent output helpers at `0x8c10e566`, `0x8c10e576`, and `0x8c10e50e` passed 8, 40, and 42. Floating-point formats, file mode, and real output I/O are still unverified, so this is not a complete formatter recovery. The adapter at `0x8c10e590` calls the 208-byte conversion helper at `0x8c10e5ac`; no native draft is currently registered or treated as recovered because additional double-precision instructions, conversions, and exception behavior remain unverified. The test interpreter now supports finite double-precision FADD/FSUB, FMUL/FDIV, FLOAT, and FCMP/EQ/GT on DR pairs in PR=1 mode; other double instructions, exceptional operands, and full formatter recovery remain pending. See the `sound_format_engine_*_verification.json` reports, `analysis/format_integer_convert_8c10e304_verification.json`, and the `analysis/fpu_double_*_verification.json` reports.

To run the broader accumulated checks, use:

```powershell
.\build_and_verify.ps1 -Workspace ((Resolve-Path ..\..).Path)
```

That sequence can take a while. Its current top-level settings refer to the Python and Clang installations on the development workstation; adjust those paths when rebuilding elsewhere. The focused cleanup build also uses the workstation's Visual Studio LLVM Clang path if Clang is not available on `PATH`.

## Current verified cleanup milestone

Routine `0x8c646654` now has recovered C, a native dispatcher, and a repeatable build/test/registration sequence. Its indexed body contains 334 instruction bytes within a 412-byte address span that also includes literal/data bytes. The cleanup dependency audit currently reports all 9 direct dependencies as registered. The full game decompilation and every requested port remain outstanding.

## Latest recovered routine

The aligned string-copy helper at `0x8c10ef20` is implemented in `source/sa_string_copy_aligned_8c10ef20.c`. Its test compares all 16 MiB of emulated RAM and every GPR, SR, PR, and PC value against the retail SH-4 interpreter in 64 cases, including empty and varied strings. This helper is intended for aligned source and destination addresses; overlapping buffers are not characterized. Run `build_string_copy_aligned_8c10ef20.ps1`, `verify_string_copy_aligned_8c10ef20.py`, and `register_string_copy_aligned_8c10ef20.py`, or use the integrated `build_and_verify.ps1` workflow. This is one recovered helper, not evidence that the entire text system or game is decompiled.

The general string-copy wrapper at `0x8c10dacc` is also recovered in `source/sa_string_copy_8c10dacc.c`. It dispatches aligned pointers to the optimized helper and handles other alignments with a byte-copy loop. Its 64 differential cases cover aligned, similarly misaligned, and differently aligned pointers and compare complete emulated RAM and CPU state. Buffer overlap remains uncharacterized. Its build, verifier, and registration steps are part of `build_and_verify.ps1`.

The bounds-check helper at `0x8c10c98c` is recovered in `source/sa_bounds_check_8c10c98c.c`. It checks an index against the 256-entry table limit and an item against the selected entry's bound. Its 96 differential cases cover signed comparisons, negative indices, and limit boundaries, comparing complete emulated RAM and CPU state. Negative indices follow the retail table-address arithmetic; caller-side validity requirements are still unknown. Its build, verifier, and registration steps are included in `build_and_verify.ps1`.

The random-number helper at `0x8c10d334` is recovered in `source/sa_random_u15_8c10d334.c`. It updates the global 32-bit seed with the constants embedded beside the retail routine and returns the masked upper 15 bits. Its 128 differential cases compare all emulated RAM, CPU registers/flags, and MACL state. The test interpreter now covers the MACL stack save/restore instructions used by this routine; a focused check for those instructions runs in `build_and_verify.ps1`. See the [Renesas SH-4 Software Manual](https://www.renesas.com/us/en/node/735531) for the processor instruction reference. Whole-game random sequences and gameplay behavior are not yet validated.

The platform-signature helper at `0x8c10ba1e` is recovered in `source/sa_platform_signature_value_8c10ba1e.c`. It checks for the `AGES` signature and, when present, returns the signed 16-bit value at the associated address. Its 128 tests compare complete emulated RAM and CPU state for matching and nonmatching signatures. The meaning and lifetime of that platform value remain unknown.

The request-slot search helper at `0x8c10c4a4` is recovered in `source/sa_request_slot_find_8c10c4a4.c`. It scans sixteen 40-byte records and returns the first record whose leading byte is zero, or zero when all records are occupied. Its 64 tests compare complete emulated RAM and CPU state, including every possible empty slot, randomized marker bytes, and the full-table case. Run its standalone build and verifier or use the integrated `build_and_verify.ps1` workflow. The broader request lifecycle and the consumers of these records remain to be recovered.

The request-slot acquisition routine at `0x8c10c4ca` is recovered in `source/sa_request_slot_acquire_8c10c4ca.c`. It finds the first free record, marks it allocated, clears selected runtime words, and returns the record address; if the table is full, it returns zero. Its 96 tests compare complete emulated RAM and CPU state for all free-slot positions and full-table cases, including stack traffic and untouched record bytes. The meanings of individual record fields and the rest of the request lifecycle remain under investigation.

The stream-position setter at `0x8c65bfc2` is recovered in `source/sa_stream_position_set_8c65bfc2.c`. Nonnegative inputs are stored directly; negative byte positions are rounded into 2 KiB units with the retail wraparound behavior. Its 128 tests compare complete RAM and CPU state, including signed limits and overflow. Ghidra indexes 32 instruction bytes in two discontiguous blocks (`0x8c65bfc2–0x8c65bfca` and `0x8c65c004–0x8c65c01c`); the registry hashes their enclosing address span while retaining the true instruction-byte count and both body ranges. The caller's range guarantees and broader stream-record meaning remain unknown.

The stream-open preparation helper at `0x8c65bf42` and its indirect request-query callback at `0x8c600ec6` are recovered in `source/sa_stream_request_open_prepare_8c65bf42.c`. The helper marks its record as opening, clears its progress field, queries the request object, and stores the result. Its 128 cases and the callback's 64 cases compare all 16 MiB of RAM and every GPR/SR/PR/PC value against the retail SH-4 interpreter. The verifier includes null, unavailable, and ready requests, callback register effects, and stack writes. This covers only the helper and callback; the wider stream lifecycle is still incomplete.

The sound-buffer initializer at `0x8c65b434` is recovered in `source/sa_audio_buffer_initialize_8c65b434.c`. It clears the 256-byte audio buffer and two related global values, reproducing the register and saved-return-address stack effects of the retail call. All 128 randomized comparisons matched complete RAM and CPU state in the SH-4 interpreter. The broader sound driver remains under analysis.

Four sound-work callback tail thunks at `0x8c65bcb4`, `0x8c65bcc0`, `0x8c65bccc`, and `0x8c65bcd8` are recovered in `source/sa_sound_work_callback_thunks.c`. Each saves the incoming `R4` value to the stack, reads its callback address from a different literal slot, passes the work record's `+4` field in `R4`, then tail-jumps while restoring the stack pointer in the jump delay slot. All 512 randomized cases matched complete RAM and CPU state at the callback entry. The Ghidra draft index reports 16 bytes for each entry, but execution confirms each wrapper's body is 12 bytes and the guessed ranges overlap the next entry by four bytes. Callback bodies remain separate recovery work.

The audio work-buffer initializer at `0x8c65bd2c` is recovered in `source/sa_audio_work_buffer_initialize_8c65bd2c.c`. It clears `0xc80` bytes at `0x8c8a3d90` through the retail byte-fill routine and returns success. Its 96 randomized tests matched complete RAM and CPU state, including the fill callback's register effects and the caller's saved-PR stack write. The rest of the audio stream lifecycle remains incomplete.

The sound-stream record allocator at `0x8c65bd46` is recovered in `source/sa_sound_stream_record_allocate_8c65bd46.c`. It searches the 32-entry table at `0x8c8a3d90`, initializes the first free 100-byte record, rounds the initial stream position, invokes the record object's vtable method, and returns the new record; it returns zero if all entries are occupied. Its 64 tests cover each free slot, full-table failure, randomized field bytes, signed position boundaries, callback effects, stack writes, and complete RAM/CPU state. The indirect vtable method is an explicit native callback dependency; the test uses the retail two-argument subtract helper, so other object method behavior remains outside the allocator verifier.

The request-position updater at `0x8c600e7a` is recovered in `source/sa_stream_request_position_update_8c600e7a.c`. It rejects null or inactive requests, supports absolute/current/end-relative positions, checks signed lower and upper bounds, and updates the position only when accepted. All 128 randomized and boundary-focused cases matched complete RAM and CPU state in the retail interpreter, including signed request-status and SR.T behavior. Its callers and the full request lifecycle are still being mapped.
