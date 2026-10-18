$ErrorActionPreference = 'Stop'
$workspace = Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$env:TMP = Join-Path $workspace 'work/sonic-adventure/build/tmp'
$env:TEMP = $env:TMP
foreach ($stem in @('invalidate', 'purge')) {
  & $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' (Join-Path $PSScriptRoot "source/sa_file_cache_${stem}_io.c") -o (Join-Path $workspace "work/sonic-adventure/build/file_cache_${stem}_io.dll")
  if ($LASTEXITCODE -ne 0) { throw "Cache helper build failed: $stem" }
}
