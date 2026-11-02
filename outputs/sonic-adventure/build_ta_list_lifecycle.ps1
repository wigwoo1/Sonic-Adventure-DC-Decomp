param([string]$Workspace=(Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference='Stop'
$taskBuild=Join-Path $Workspace 'work/sonic-adventure/build'
New-Item -ItemType Directory -Force -Path $taskBuild | Out-Null
$taskClang='C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
& $taskClang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot 'source/ta_list_lifecycle.c') -o (Join-Path $taskBuild 'ta_list_lifecycle.dll')
if($LASTEXITCODE -ne 0){throw 'TA list lifecycle build failed'}
