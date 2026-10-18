param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference='Stop'
$taskClang='C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$taskSources=@('sa_arm_sound_snapshot.c','sa_arm_sound_snapshot_fixture.c') | ForEach-Object { Join-Path $PSScriptRoot "source/$_" }
& $taskClang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @taskSources -o (Join-Path $Workspace 'work/sonic-adventure/build/arm_sound_snapshot_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native ARM snapshot build failed' }
