param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference = 'Stop'
$clang = 'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$sources = @('sa_file_mount.c','sa_interrupt_callback_create.c','sa_interrupt_handler_primary.c','sa_interrupt_handler_set.c','sa_mount_driver_enable.c') | ForEach-Object { Join-Path $PSScriptRoot "source/$_" }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @sources -o (Join-Path $Workspace 'work/sonic-adventure/build/file_mount_registration.dll')
if ($LASTEXITCODE -ne 0) { throw 'Mount callback registration build failed' }
