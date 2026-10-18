from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'build_sound_linear_source_capacity.ps1').read_text().replace('sound_linear_source_capacity','signed_division').replace('Sound source query build failed','Signed division build failed')
(p/'build_signed_division.ps1').write_text(s)
source=p/'source/sa_signed_division.c';s=source.read_text();a=s.index('/* Static C recovery');z=s.index(' */',a)+3
source.write_text(s[:a]+'/* Retail signed32 quotient helper 8c10ca5c..8c10cb06.\n * DIV0S/ROTCL/DIV1/SUBC/ADDC instruction behavior preserved.\n * Zero divisor returns zero and records error0x44e. No external calls. */'+s[z:])
