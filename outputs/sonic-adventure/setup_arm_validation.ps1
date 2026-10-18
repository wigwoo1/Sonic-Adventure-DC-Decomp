param([string]$Workspace = (Split-Path -Parent (Split-Path -Parent $PSScriptRoot)),
      [string]$Python = 'C:\Users\wigwo\.cache\codex-runtimes\codex-primary-runtime\dependencies\python\python.exe')
$ErrorActionPreference='Stop'
$target=Join-Path $Workspace 'work/sonic-adventure/toolchain/arm-validation-python'
& $Python -m pip install --disable-pip-version-check --no-input --target $target -r (Join-Path $PSScriptRoot 'ARM_VALIDATION_REQUIREMENTS.txt')
if ($LASTEXITCODE -ne 0) { throw 'ARM validation dependency installation failed' }
