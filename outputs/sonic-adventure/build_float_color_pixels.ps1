param([string]$Workspace=(Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference='Stop'
$taskBuild=Join-Path $Workspace 'work/sonic-adventure/build'
New-Item -ItemType Directory -Force -Path $taskBuild | Out-Null
$taskClang='C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
& $taskClang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/pvr_pixels_float.c') (Join-Path $PSScriptRoot 'source/render_completion.c') (Join-Path $PSScriptRoot 'source/pixel_jobs.c') (Join-Path $PSScriptRoot 'source/banked_pixel_jobs.c') -o (Join-Path $taskBuild 'float_color_pixels.dll')
if($LASTEXITCODE -ne 0){throw 'Bank-linear VRAM pixel-job build failed'}
