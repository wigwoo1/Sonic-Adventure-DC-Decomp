param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)))
$ErrorActionPreference='Stop'
$clang='C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe'
$sources=@('sa_arm_sound_boot_native.c','sa_arm_sound_dispatcher.c','sa_arm_sound_input.c','sa_arm_sound_input_fixture.c','sa_arm_sound_exception.c','sa_arm_sound_exception_fixture.c','sa_arm_sound_reset_services.c','sa_arm_sound_commands.c','sa_arm_sound_bank_select.c','sa_arm_sound_controls.c','sa_arm_sound_notes.c','sa_arm_sound_status.c','sa_arm_sound_update.c','sa_arm_sound_snapshot.c','sa_arm_sound_ramps.c','sa_arm_sound_effects.c','sa_arm_sound_stream_update.c','sa_arm_sound_sequencer.c') | ForEach-Object { Join-Path $PSScriptRoot "source/$_" }
& $clang -shared -O2 -Wall -Wextra -Werror -fuse-ld=lld -nostdlib '-Wl,/noentry' @sources -o (Join-Path $Workspace 'work/sonic-adventure/build/arm_sound_boot_native.dll')
if ($LASTEXITCODE -ne 0) { throw 'Native ARM sound boot build failed' }
