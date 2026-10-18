param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference = 'Stop'
$env:TMP = Join-Path $Workspace 'work/sonic-adventure/build/tmp'
$env:TEMP = $env:TMP
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$python = 'C:\Users\wigwo\.cache\codex-runtimes\codex-primary-runtime\dependencies\python\python.exe'
$sources = @('sa_sound_request_commit','sa_sound_request_copy20','sa_engine_record_copy_loaded_dispatch','sa_engine_error_record') | ForEach-Object { Join-Path $PSScriptRoot ('source/' + $_ + '.c') }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @sources -o (Join-Path $Workspace 'work/sonic-adventure/build/sound_request_commit.dll')
if ($LASTEXITCODE -ne 0) { throw 'Sound request producer build failed' }
& $python (Join-Path $PSScriptRoot 'verify_sound_request_commit.py')
if ($LASTEXITCODE -ne 0) { throw 'Sound request producer verification failed' }
