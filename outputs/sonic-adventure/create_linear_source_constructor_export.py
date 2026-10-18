from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'ghidra/export_sound_linear_source.ps1').read_text()
s=s.replace("Copy-Item (Join-Path $PSScriptRoot 'SeedOwnerHelperBranches.java') $scriptRoot","Copy-Item (Join-Path $PSScriptRoot 'SeedLinearSource.java') $scriptRoot")
s=s.replace('-postScript ExportInstructionRange.java','-postScript SeedLinearSource.java -postScript ExportInstructionRange.java')
s=s.replace('sound_linear_source_65e000.tsv','sound_linear_source_constructor_65e0dc.tsv').replace('8c65e000 8c65e368','8c65e0dc 8c65e150')
s=s.replace('input-status-scripts','linear-source-constructor-scripts')
(p/'ghidra/export_sound_linear_source_constructor.ps1').write_text(s)
