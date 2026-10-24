param([string]$Workspace=(Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path)
$ErrorActionPreference='Stop'
$taskToolRoot=Join-Path $Workspace 'work/sonic-adventure/toolchain'
$taskGhidra=Join-Path $taskToolRoot 'ghidra_12.1.4_PUBLIC'
$taskJdk=Get-ChildItem -LiteralPath $taskToolRoot -Directory -Filter 'jdk-*' | Select-Object -First 1
$env:JAVA_HOME=$taskJdk.FullName
$env:GHIDRA_USER_HOME=Join-Path $taskToolRoot 'arm-sound-driver-user-home'
$env:_JAVA_OPTIONS='-Duser.home="'+$env:GHIDRA_USER_HOME+'"'
$taskInput=Join-Path $Workspace 'work/sonic-adventure/extracted/files/SONICADV/MOVIE.BIN'
if((Get-FileHash -LiteralPath $taskInput -Algorithm SHA256).Hash.ToLowerInvariant() -ne '84489968b607f6f80a7ad04ad402224ddd951a65e3ae8e18429ecfa17525614b'){throw 'MOVIE raw input hash mismatch'}
$taskProjects=Join-Path $Workspace 'work/sonic-adventure/ghidra-projects'
$taskOutput=Join-Path $Workspace 'outputs/sonic-adventure/analysis/movie-runtime'
$taskArguments=@($taskProjects,'SonicAdventureMovieRuntime')
if(Test-Path -LiteralPath (Join-Path $taskProjects 'SonicAdventureMovieRuntime.gpr')){$taskArguments+=@('-process','MOVIE.BIN')}
else{$taskArguments+=@('-import',$taskInput,'-loader','BinaryLoader','-loader-baseAddr','0x8ceb0000','-processor','SuperH4:LE:32:default','-cspec','default')}
$taskArguments+=@('-preScript','SeedMovieRuntime.java','-preScript','SeedOverlay.java','-scriptPath',$PSScriptRoot,'-analysisTimeoutPerFile','300','-postScript','ExportInitialDecompilation.java',$taskOutput,'0','-postScript','ExportFailedFunctionEvidence.java',(Join-Path $taskOutput 'entry'),'8ceb0000','-max-cpu','4')
& (Join-Path $taskGhidra 'support/analyzeHeadless.bat') @taskArguments
if($LASTEXITCODE -ne 0){throw 'MOVIE runtime import/analysis failed'}
if(!(Test-Path -LiteralPath (Join-Path $taskOutput 'function_index.tsv'))){throw 'MOVIE runtime index missing'}
