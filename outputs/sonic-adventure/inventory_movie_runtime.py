"""Pin the observed raw MOVIE image/base and its nonexhaustive Ghidra index."""
import collections,csv,hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[1];INPUT=ROOT/'work/sonic-adventure/extracted/files/SONICADV/MOVIE.BIN';SHA='84489968b607f6f80a7ad04ad402224ddd951a65e3ae8e18429ecfa17525614b'
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 assert INPUT.stat().st_size==184592 and digest(INPUT)==SHA
 prefix=P/'analysis/boot_cache_prefix_verification.json';proof=json.loads(prefix.read_text());assert proof['cases_passed']==4 and proof['next_dependencies']==['0x8ceb0000']
 for r in proof['cases']:assert r['loaded_movie_sha256']==SHA and r['load_destination_offset']=='0xeb0000'
 index=P/'analysis/movie-runtime/function_index.tsv';rows=list(csv.DictReader(index.open(),delimiter='\t'));assert rows and rows[0]['address']=='8ceb0000'
 files=[Path(__file__),INPUT,prefix,index,P/'analysis/movie-runtime/initial_pseudocode.c',P/'ghidra/analyze_movie_runtime.ps1',P/'ghidra/SeedMovieRuntime.java',P/'ghidra/SeedOverlay.java',P/'ghidra/ExportInitialDecompilation.java',P/'ghidra/export_movie_entry.ps1']
 report=dict(module='MOVIE.BIN',retail_binary=INPUT.relative_to(ROOT).as_posix(),retail_binary_sha256=SHA,load_address='0x8ceb0000',entry='0x8ceb0000',retail_bytes=184592,input_transform='identity/raw executable, observed directly loaded and called by original boot',indexed_candidates=len(rows),draft_statuses=dict(collections.Counter(r['decompilation_status'] for r in rows)),function_index=index.relative_to(P).as_posix(),whole_module_code_data_boundaries_complete=False,newly_registered_entries=0,sha256={str(p):digest(p) for p in files},limitations=['Ghidra roots/index/pseudocode are analysis candidates, not verified recovered game code.','The index is nonexhaustive and does not establish total game routine count or a recovered-code percentage.','Runtime base/raw input come from complete-state boot load/call evidence; other modules have different load bases.'])
 (P/'analysis/movie_runtime_inventory.json').write_text(json.dumps(report,indent=2)+'\n');print('Pinned raw MOVIE identity and',len(rows),'candidate functions at observed 8ceb0000')
if __name__=='__main__':main()
