"""Keep external hardware-profile evidence separate from original code recovery."""
import csv,hashlib,json,struct
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1]
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 sha='e88b0ced2243de137bcccac0072f0c1dd00d41bd';source=ROOT/f'work/sonic-adventure/flycast-sb-{sha}.cpp';text=source.read_text()
 assert '//0x005F689C    SB_SBREV    R   System bus revision number' in text and 'SB_SBREV = 0xB;' in text
 binary=ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=binary.read_bytes();word=lambda a:struct.unpack_from('<I',raw,a-0x8c010000)[0]
 assert word(0x8c6049b0)==0xa05f689c and word(0x8c6049bc)==0x8c6734c4 and word(0x8c6734c4)==0x8c000000
 needle=struct.pack('<I',0x8c6734c4);sites=[hex(0x8c010000+i) for i in range(len(raw)-3) if raw[i:i+4]==needle];assert sites==['0x8c6049bc']
 listing=P/'analysis/movie-startup-driver-frontier/8c604702_instructions.tsv';rows={int(r['address'],16):r for r in csv.DictReader(listing.open(),delimiter='\t')}
 expected={0x8c604910:'mov.l 0x8c6049b0,r1',0x8c604912:'mov.l @r1,r2',0x8c604914:'mov #0xa,r3',0x8c604916:'extu.b r2,r2',0x8c604918:'cmp/hs r3,r2',0x8c60491a:'bt 0x8c60493e',0x8c604924:'mov.l 0x8c6049bc,r2',0x8c604926:'mov.l @r2,r3',0x8c60493a:'jmp @r3'}
 for a,instruction in expected.items():assert rows[a]['instruction'].lstrip('_')==instruction and raw[a-0x8c010000:a-0x8c010000+2]==bytes.fromhex(rows[a]['bytes'])
 refs=P/'analysis/movie-startup-platform-references/references.tsv';references=list(csv.DictReader(refs.open(),delimiter='\t'));slot=[r for r in references if r['target']=='8c6734c4']
 assert slot and not any('WRITE' in r['type'] for r in slot)
 files=[Path(__file__),source,binary,listing,refs,P/'ghidra/export_movie_startup_platform_references.ps1',P/'ghidra/ExportOverlaySelectorReferences.java']
 report=dict(callback_word='0x8c6734c4',retail_callback_value='0x8c000000',literal_occurrences=sites,callback_references=slot,revision_register='0xa05f689c',reference_revision=11,
  source_commit=sha,source_url=f'https://github.com/flyinghead/flycast/blob/{sha}/core/hw/holly/sb.cpp',source_lines=dict(register=289,reset=663),source_excerpt=['//0x005F689C    SB_SBREV    R   System bus revision number','SB_SBREV = 0xB;'],
  interpretation='The original driver tests the low revision byte against ten. Values below ten reach the stored fallback callback; ten or higher take the original return path. The explicit Flycast reference profile supplies eleven without rewriting the callback or any game instructions.',
  limitations=['This is an emulator-supported raw hardware profile, not a measurement of the user console or a guarantee covering every Dreamcast revision.','No WRITE reference is present in this seeded Ghidra inventory; this does not prove that no computed or external writer exists.','The historical revision-zero control and its genuine unknown target remain preserved. Current platform defaults are not changed.','The external source is retained privately for reproducibility/hash checking; no emulator implementation is compiled into the recovered game.'],sha256={str(p):digest(p) for p in files})
 (P/'analysis/movie_revision_platform_evidence.json').write_text(json.dumps(report,indent=2)+'\n');print('Original revision branch and fallback word byte-checked; explicit Flycast reference profile pinned to',sha)
if __name__=='__main__':main()
