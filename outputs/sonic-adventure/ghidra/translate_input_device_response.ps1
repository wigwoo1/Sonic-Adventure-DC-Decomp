param([Parameter(Mandatory=$true)][string]$Python)
$ErrorActionPreference = 'Stop'
$ranges = @(
 @('response_complete','8c65fe14','8c65ff12'),
 @('presence_decode','8c65ff12','8c65ff5a'),
 @('response_header','8c65ff5a','8c65ffb8'),
 @('primary_response','8c65ffb8','8c66004c'),
 @('sub_response','8c66005c','8c66018e'),
 @('info_decode','8c66018e','8c6602d2')
)
foreach ($range in $ranges) {
 & $Python "$PSScriptRoot/translate_tikal_helper.py" "sa_input_device_$($range[0])" $range[1] $range[2] input_device_completion_postqueue.tsv
 if ($LASTEXITCODE -ne 0) { throw "Translation failed: $($range[0])" }
}

& $Python "$PSScriptRoot/translate_tikal_helper.py" sa_input_response_word_swap 8c668b24 8c668b38 input_work_initialization.tsv
if ($LASTEXITCODE -ne 0) { throw 'Response word swap translation failed' }
