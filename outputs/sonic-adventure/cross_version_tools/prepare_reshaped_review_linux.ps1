$ErrorActionPreference = 'Stop'
$taskRoot = (Resolve-Path (Join-Path $PSScriptRoot '..\..')).Path
$taskDownloads = Join-Path $PSScriptRoot 'toolchain/reshaped-review-linux'
$taskInstall = Join-Path $PSScriptRoot 'wsl/sa-source-review-ubuntu2404'
$taskName = 'sa-dc-source-review'
if (Test-Path -LiteralPath $taskInstall) { throw 'New distribution directory already exists; inspect instead of replacing it.' }
$taskNames = (& wsl.exe --list --quiet) -replace "`0", ''
if ($taskNames -contains $taskName) { throw 'Distribution already registered; inspect it instead of replacing it.' }
New-Item -ItemType Directory -Path $taskDownloads -Force | Out-Null
$taskManifest = Join-Path $taskDownloads 'DistributionInfo.json'
if (Test-Path -LiteralPath $taskManifest) { throw 'Download manifest already exists; preserve it for review.' }
& curl.exe --fail --location --connect-timeout 30 --output $taskManifest 'https://raw.githubusercontent.com/microsoft/WSL/master/distributions/DistributionInfo.json'
if ($LASTEXITCODE -ne 0) { throw 'Official distribution manifest download failed.' }
$taskEntry = (Get-Content -LiteralPath $taskManifest -Raw | ConvertFrom-Json).ModernDistributions.Ubuntu | Where-Object Name -eq 'Ubuntu-24.04'
$taskUrl = $taskEntry.Amd64Url.Url
$taskHash = $taskEntry.Amd64Url.Sha256
if ($taskUrl -notmatch '^https://releases\.ubuntu\.com/24\.04[^/]*/ubuntu-24\.04[^/]*-wsl-amd64\.wsl$' -or $taskHash -notmatch '^[a-f0-9]{64}$') { throw 'Unexpected official image entry.' }
$taskImage = Join-Path $taskDownloads ([IO.Path]::GetFileName($taskUrl))
if (Test-Path -LiteralPath $taskImage) { throw 'Image already exists; inspect its hash instead of overwriting it.' }
Write-Output ('Downloading official Ubuntu image: ' + $taskUrl)
& curl.exe --fail --location --retry 2 --connect-timeout 30 --output $taskImage $taskUrl
if ($LASTEXITCODE -ne 0) { throw 'Ubuntu image download failed.' }
if ((Get-FileHash -LiteralPath $taskImage -Algorithm SHA256).Hash.ToLowerInvariant() -ne $taskHash) { throw 'Image checksum mismatch; import refused.' }
Write-Output ('Verified image SHA256: ' + $taskHash)
New-Item -ItemType Directory -Path $taskInstall | Out-Null
& wsl.exe --import $taskName $taskInstall $taskImage --version 2
if ($LASTEXITCODE -ne 0) { throw 'New distribution import failed; old distributions were not changed.' }
& wsl.exe -d $taskName --exec sh -lc 'cat /etc/os-release; uname -m; command -v python3; command -v make; command -v cc'
if ($LASTEXITCODE -ne 0) { throw 'Initial Linux tool inventory incomplete; inspect installed tools next.' }
