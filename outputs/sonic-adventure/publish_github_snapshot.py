"""Stage and publish source plus research builds without packaging retail inputs.

Credentials are obtained only through Git's configured credential manager and
stay in memory. The default action prepares local files; --publish uploads them.
"""
import argparse
import collections
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import urllib.error
import urllib.parse
import urllib.request
import zipfile

OWNER = 'wigwoo1'
REPO = 'Sonic-Adventure-DC-Decomp'
TEXT = {'.c', '.h', '.py', '.ps1', '.java', '.json', '.tsv', '.txt', '.md',
        '.gradle', '.properties', '.manifest', '.xml', '.yml', '.yaml'}
SKIP = {'__pycache__', '.git', '.gradle', 'build', 'dist', 'work', 'node_modules'}
SECRET = re.compile(rb'(?:gh[pousr]_[A-Za-z0-9]{30,}|github_pat_[A-Za-z0-9_]{50,}|'
                    rb'AKIA[A-Z0-9]{16}|-----BEGIN (?:RSA |EC |OPENSSH )?PRIVATE KEY-----)')


def digest(data):
    return hashlib.sha256(data).hexdigest()


def git(folder, *args):
    result = subprocess.run(['git', '-C', str(folder), *args], text=True, capture_output=True)
    if result.returncode:
        raise RuntimeError('Git operation failed: '+result.stderr.strip())
    return result.stdout.strip()


def select_files(workspace):
    project = workspace / 'outputs/sonic-adventure'
    selected = []
    excluded = collections.Counter()
    for path in sorted(project.rglob('*')):
        if not path.is_file():
            continue
        rel = path.relative_to(project)
        if any(part in SKIP for part in rel.parts[:-1]) or path.suffix.lower() not in TEXT:
            excluded[path.suffix.lower() or '<no suffix>'] += 1
            continue
        if path.name.startswith('.env') or path.name.lower() in {'credentials.json', 'secrets.json'}:
            raise RuntimeError('Credential-shaped filename in publication selection')
        raw = path.read_bytes()
        if SECRET.search(raw):
            raise RuntimeError(f'Credential-shaped content in {rel}; publication stopped')
        raw.decode('utf-8-sig')
        selected.append((path, path.relative_to(workspace), raw))
    for name in ('register_conditional_byte_clear_8c10e566.py',
                 'register_global_word_store_8c043998.py',
                 'verify_conditional_byte_clear_8c10e566.py'):
        path = workspace / name
        if path.exists():
            raw = path.read_bytes()
            assert not SECRET.search(raw)
            selected.append((path, Path(name), raw))
    assert selected and any(rel.as_posix().endswith('source/sa_draw_angle_curve.c')
                            for _, rel, _ in selected)
    return selected, dict(excluded)


def prepare(workspace):
    destination = workspace / 'publication/sonic-adventure'
    destination.mkdir(parents=True, exist_ok=True)
    selected, excluded = select_files(workspace)
    entries = []
    for _, rel, raw in selected:
        target = destination / rel
        target.parent.mkdir(parents=True, exist_ok=True)
        if not target.exists() or target.read_bytes() != raw:
            target.write_bytes(raw)
        entries.append(dict(path=rel.as_posix(), bytes=len(raw), sha256=digest(raw)))
    # Never delete historical copies automatically; unexpected stale files must
    # be reviewed before a later publication, rather than silently disappearing.
    allowed = {r['path'] for r in entries}
    for base in (destination / 'outputs',):
        for path in base.rglob('*'):
            if path.is_file() and path.relative_to(destination).as_posix() not in allowed:
                raise RuntimeError(f'Stale staged file requires review: {path}')
    manifest = dict(project='Sonic Adventure Japanese-source native-port research',
                    files=entries, excluded_local_extensions=excluded,
                    retail_game_files_included=False, playable_game=False,
                    qualification='Historical proofs describe their specific domains; drafts are not verified code.')
    (destination / 'PUBLICATION_MANIFEST.json').write_text(json.dumps(manifest, indent=2)+'\n', encoding='utf-8')
    (destination / '.gitignore').write_text('work/\npublication/\n__pycache__/\n*.pyc\n.env*\n*.pem\n*.key\n*.bin\n*.BIN\n*.gdi\n*.cdi\n*.iso\n*.chd\n*.dll\n*.lib\n*.exe\n', encoding='utf-8')
    (destination / '.gitattributes').write_text('* -text\n', encoding='utf-8')
    (destination / 'README.md').write_text('''# Sonic Adventure Japanese-source native-port research

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
''', encoding='utf-8')
    builds = []
    for folder in (workspace/'work/sonic-adventure/build', workspace/'outputs/sonic-adventure/work/sonic-adventure/build'):
        for path in sorted(folder.glob('*')):
            if path.is_file() and path.suffix.lower() in {'.dll', '.lib', '.exe'}:
                raw = path.read_bytes()
                assert raw.startswith(b'MZ') if path.suffix.lower() != '.lib' else raw.startswith(b'!<arch>')
                builds.append((path, path.relative_to(workspace).as_posix(), raw))
    assert builds
    build_manifest = dict(playable_game=False, kind='Existing Windows research DLLs and import libraries',
                          files=[dict(path=rel, bytes=len(raw), sha256=digest(raw)) for _,rel,raw in builds],
                          source_manifest_sha256=digest((destination/'PUBLICATION_MANIFEST.json').read_bytes()))
    releases = workspace/'publication/release-assets'
    releases.mkdir(parents=True, exist_ok=True)
    archive = releases/'sonic-adventure-windows-research-builds.zip'
    with zipfile.ZipFile(archive, 'w', compression=zipfile.ZIP_DEFLATED, compresslevel=6) as out:
        for _, rel, raw in builds:
            info = zipfile.ZipInfo(rel, date_time=(2026, 1, 1, 0, 0, 0))
            info.compress_type = zipfile.ZIP_DEFLATED
            out.writestr(info, raw)
        info = zipfile.ZipInfo('BUILD_MANIFEST.json', date_time=(2026,1,1,0,0,0))
        info.compress_type = zipfile.ZIP_DEFLATED
        out.writestr(info, json.dumps(build_manifest, indent=2)+'\n')
    with zipfile.ZipFile(archive) as check:
        assert check.testzip() is None
        for _, rel, raw in builds:
            assert digest(check.read(rel)) == digest(raw)
    checksums = releases/'SHA256SUMS.txt'
    checksums.write_text(digest(archive.read_bytes())+'  '+archive.name+'\n', encoding='utf-8')
    print(json.dumps(dict(source_files=len(entries), source_bytes=sum(e['bytes'] for e in entries),
                          build_files=len(builds), build_archive_bytes=archive.stat().st_size,
                          destination=str(destination), archive=str(archive)), indent=2), flush=True)
    return destination, archive, checksums


def credential():
    result = subprocess.run(['git','-c','credential.interactive=false','credential','fill'],
                            input='protocol=https\nhost=github.com\nusername='+OWNER+'\n\n', text=True,
                            capture_output=True, timeout=40,
                            env=dict(os.environ,GIT_TERMINAL_PROMPT='0',GCM_INTERACTIVE='Never'))
    if result.returncode:
        raise RuntimeError('Git Credential Manager is not authenticated or cannot access its credential store')
    values = dict(line.split('=',1) for line in result.stdout.splitlines() if '=' in line)
    if not values.get('password'):
        raise RuntimeError('Git Credential Manager returned no publishing token')
    return values['password']


def publish(destination, archive, checksums):
    token = credential()

    def request(path, data=None, method=None, binary=False):
        raw = data if binary else (None if data is None else json.dumps(data).encode())
        url = path if path.startswith('https://uploads.github.com/') else 'https://api.github.com'+path
        req = urllib.request.Request(url, data=raw, method=method,
                headers={'Authorization':'Bearer '+token, 'Accept':'application/vnd.github+json',
                         'User-Agent':'sonic-adventure-publisher',
                         'Content-Type':'application/octet-stream' if binary else 'application/json'})
        with urllib.request.urlopen(req, timeout=180) as response:
            return json.load(response)

    assert request('/user')['login'] == OWNER, 'Unexpected GitHub account; refusing publication'
    route = '/repos/'+OWNER+'/'+REPO
    try:
        repo = request(route)
    except urllib.error.HTTPError as error:
        if error.code != 404:
            raise
        repo = request('/user/repos', dict(name=REPO, private=False, auto_init=False,
                       description='Sonic Adventure Dreamcast Rev A decompilation and native-port research. Work in progress; not playable.'))
    assert not repo['private'] and repo['owner']['login'] == OWNER
    if not (destination/'.git').exists():
        subprocess.run(['git','init','--initial-branch=main',str(destination)],check=True,capture_output=True)
    git(destination,'config','user.name',OWNER)
    git(destination,'config','user.email','63259866+'+OWNER+'@users.noreply.github.com')
    git(destination,'config','credential.username',OWNER)
    remote = 'https://github.com/'+OWNER+'/'+REPO+'.git'
    remotes = git(destination,'remote').splitlines()
    if 'origin' not in remotes:
        git(destination,'remote','add','origin',remote)
    assert git(destination,'remote','get-url','origin') == remote
    git(destination,'add','--all')
    if git(destination,'diff','--cached','--name-only'):
        git(destination,'commit','-m','Publish current recovered source, documentation and verification evidence')
    # Ordinary fast-forward push; never replace remote history.
    git(destination,'push','--set-upstream','origin','main')
    head = git(destination,'rev-parse','HEAD')
    assert request(route+'/commits/main')['sha'] == head
    tag = 'research-'+head[:12]+'-'+digest(archive.read_bytes())[:8]
    notes = ('Research snapshot of the current recovered C, scripts, documentation and evidence. '
             'Not a playable game or complete decompilation. Overall milestone estimate remains 10%. '
             'Archive contains existing Windows research DLLs and import libraries, with per-file hashes. '
             'Verification scope varies: consult the matching source reports. '
             'Original game inputs and assets are not included. Source commit: '+head)
    try:
        release = request(route+'/releases/tags/'+tag)
    except urllib.error.HTTPError as error:
        if error.code != 404:
            raise
        release = request(route+'/releases',dict(tag_name=tag,target_commitish=head,name='Research snapshot '+head[:12],body=notes,prerelease=True,draft=False))
    existing = {a['name']:a for a in request(route+'/releases/'+str(release['id'])+'/assets')}
    for path in (archive,checksums):
        if path.name in existing:
            assert existing[path.name]['size']==path.stat().st_size, 'Existing immutable release asset differs'
            continue
        uploaded = request(release['upload_url'].split('{')[0]+'?name='+urllib.parse.quote(path.name),path.read_bytes(),method='POST',binary=True)
        assert uploaded['state']=='uploaded' and uploaded['size']==path.stat().st_size
    assets = request(route+'/releases/'+str(release['id'])+'/assets')
    for path in (archive, checksums):
        asset = next(a for a in assets if a['name'] == path.name)
        assert asset['state'] == 'uploaded' and asset['size'] == path.stat().st_size
        if asset.get('digest'):
            assert asset['digest'] == 'sha256:'+digest(path.read_bytes())
        else:
            # Public download; do not forward the publishing token to a CDN.
            with urllib.request.urlopen(asset['browser_download_url'], timeout=180) as response:
                assert digest(response.read()) == digest(path.read_bytes())
    result = dict(repository=repo['html_url'], visibility='public', commit=head,
                  release=release['html_url'], assets_verified=2,
                  archive_sha256=digest(archive.read_bytes()))
    (destination.parent/'last_publish_result.json').write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8')
    print(json.dumps(result,indent=2))


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--workspace',type=Path,required=True)
    parser.add_argument('--publish',action='store_true')
    args=parser.parse_args()
    workspace=args.workspace.resolve()
    assert (workspace/'outputs/sonic-adventure/source').is_dir()
    prepared=prepare(workspace)
    if args.publish:
        publish(*prepared)


if __name__=='__main__':
    main()
