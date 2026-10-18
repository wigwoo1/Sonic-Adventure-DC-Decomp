param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference='Stop'
$taskClang='C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$taskSources=@('sa_arm_sound_update.c','sa_arm_sound_update_fixture.c','sa_arm_sound_reset_services.c') | ForEach-Object { Join-Path $PSScriptRoot "source/$_" }
& $taskClang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @taskSources -o (Join-Path $Workspace 'work/sonic-adventure/build/arm_sound_update_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native ARM update build failed' }
