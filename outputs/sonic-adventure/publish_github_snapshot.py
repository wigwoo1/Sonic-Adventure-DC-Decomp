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
    manifest = dict(project='Sonic Adventure Dreamcast Rev A research',
                    files=entries, excluded_local_extensions=excluded,
                    retail_game_files_included=False, playable_game=False,
                    qualification='Historical proofs describe their specific domains; drafts are not verified code.')
    (destination / 'PUBLICATION_MANIFEST.json').write_text(json.dumps(manifest, indent=2)+'\n', encoding='utf-8')
    (destination / '.gitignore').write_text('work/\npublication/\n__pycache__/\n*.pyc\n.env*\n*.pem\n*.key\n*.bin\n*.BIN\n*.gdi\n*.cdi\n*.iso\n*.chd\n*.dll\n*.lib\n*.exe\n', encoding='utf-8')
    (destination / '.gitattributes').write_text('* -text\n', encoding='utf-8')
    (destination / 'README.md').write_text('''# Sonic Adventure Dreamcast decompilation research

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
