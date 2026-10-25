"""Prove the logger's four alignment-fallback rows unreachable for aligned ABI."""
import csv,hashlib,json,random
from pathlib import Path
import verify_movie_diagnostics as v
P=v.P

def main():
 listing=P/'analysis/movie-record-constructors-frontier/8c65a2fc_instructions.tsv';rows=list(csv.DictReader(listing.open(),delimiter='\t'));raw_path=v.ROOT/'work/sonic-adventure/extracted/1ST_READ.BIN';raw=raw_path.read_bytes();by={int(r['address'],16):r for r in rows}
 for row in rows:
  pc=int(row['address'],16);assert raw[pc-0x8c010000:pc-0x8c010000+2]==bytes.fromhex(row['bytes'])
 expected={0x8c65a2fe:'sts.l PR,@-r15',0x8c65a30e:'mov r15,r0',0x8c65a310:'add #0x4,r0',0x8c65a312:'mov #0x3,r5',0x8c65a314:'add #0x4,r0',0x8c65a316:'tst r0,r5',0x8c65a318:'bt 0x8c65a322'}
 for pc,text in expected.items():assert by[pc]['instruction'].lstrip('_')==text
 prefix=[r for r in rows if int(r['address'],16)<0x8c65a318];assert [r['instruction'] for r in prefix if ',r15' in r['instruction'] or '@-r15' in r['instruction']]==['sts.l PR,@-r15']
 dead={0x8c65a31a,0x8c65a31c,0x8c65a31e,0x8c65a320};assert not any(int(target,16) in dead for r in rows for target in r['flows'].split())
 # R0 at TST is entry_SP - 4 + 4 + 4 modulo 2^32. Its low bits
 # therefore equal entry_SP's low bits, for every 32-bit entry_SP.
 assert all(((low-4+4+4)&3)==low for low in range(4))
 ram=v.seed_ram();c=next(v.old.old.cases())|dict(entry=v.LOG);memory,state,fpu=v.prepare(ram,c);rng=random.Random(7091);prefixes=0
 for sp in [0x8ce00000,0x8cfffff0]+[0x8ce00000+4*rng.randrange(0x7fffc) for _ in range(62)]:
  m=bytearray(memory);s=v.effects.State.from_buffer_copy(state);f=v.effects.Fpu.from_buffer_copy(fpu);s.r[15]=sp
  v.cpu.reference(m,s,fpu=f,stop_pc=0x8c65a318,instruction_budget=128,special=dict(other=[0]*8,mach=0,macl=0x12345678,gbr=0,vbr=0x8ce50000))
  assert s.pc==0x8c65a318 and s.r[0]==(sp+4)&0xffffffff and s.r[5]==3 and s.r[15]==sp-4 and s.sr&1;prefixes+=1
 source=P/'source/sa_movie_diagnostics.c';assert all(f'L{pc:x}:' in source.read_text() for pc in dead-{0x8c65a320});assert '*stop=0x8c65a320u;' in source.read_text()
 files=[Path(__file__),listing,raw_path,source,P/'movie_diagnostics_reference_cpu.py',Path(v.__file__)]
 report=dict(address=hex(v.LOG),static_rows=33,unreachable_rows=[hex(x) for x in sorted(dead)],unreachable_edges=[['0x8c65a318','0x8c65a31a']],executed_original_prefixes=prefixes,source_rows_retained=True,sha256={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in files},scope='Exact original opcode/data-flow proof: one four-byte PR push, then R0=entry_SP+4, R5=3, so TST is true for every four-byte-aligned entry stack. No branch targets enter the fallback block. Concrete original prefixes supplement the low-bit proof. Preconditions: four-byte-aligned valid stack, immutable code and no asynchronous register/stack corruption. Fallback C rows remain present; arbitrary unaligned stacks and physical exception delivery are not qualified.')
 (P/'analysis/movie_diagnostics_alignment_invariant.json').write_text(json.dumps(report,indent=2)+'\n');print('Proved four unreachable logger rows / one outcome;',prefixes,'original prefixes')
if __name__=='__main__':main()
