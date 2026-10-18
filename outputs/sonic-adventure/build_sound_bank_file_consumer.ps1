param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference = 'Stop'
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$sources = @('sa_sound_bank_load.c','sa_file_size_get.c') | ForEach-Object { Join-Path $PSScriptRoot "source/$_" }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @sources -o (Join-Path $Workspace 'work/sonic-adventure/build/sound_bank_file_consumer.dll')
if ($LASTEXITCODE -ne 0) { throw 'Sound bank file consumer build failed' }
