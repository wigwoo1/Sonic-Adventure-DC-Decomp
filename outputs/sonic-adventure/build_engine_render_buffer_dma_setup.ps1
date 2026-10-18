param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference = 'Stop'
$env:TMP = Join-Path $Workspace 'work/sonic-adventure/build/tmp'
$env:TEMP = $env:TMP
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$sources = @(
    (Join-Path $PSScriptRoot 'source/sa_engine_render_buffer_dma_setup.c'),
    (Join-Path $PSScriptRoot 'source/sa_engine_word_copy.c'),
    (Join-Path $PSScriptRoot 'source/sa_engine_cache_writeback_range.c')
)
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @sources -o (Join-Path $Workspace 'work/sonic-adventure/build/engine_render_buffer_dma_setup.dll')
if ($LASTEXITCODE -ne 0) { throw 'Render buffer DMA setup build failed' }
