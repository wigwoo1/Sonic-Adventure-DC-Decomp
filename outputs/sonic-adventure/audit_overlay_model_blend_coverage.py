"""Explain exact uncovered morph paths without claiming hardware-wide coverage."""
import hashlib,json
from pathlib import Path
PROJECT=Path(__file__).resolve().parent
def main():
 path=PROJECT/'analysis/overlay_model_blend_verification.json';report=json.loads(path.read_text())
 rows=['0xc904fc0','0xc904fc2','0xc90539e','0xc9053a0','0xc9053a2','0xc9053aa','0xc9053ac','0xc9053b2','0xc9053b4','0xc9053bc','0xc9053c6','0xc9053c8','0xc9053ca','0xc9053d2','0xc9053d4']
 assert report['uncovered_rows']==rows and report['rows_covered']==856
 assert report['conditional_edges_covered']==67 and report['conditional_edges_total']==74
 for p,h in report['sha256'].items():assert hashlib.sha256(Path(p).read_bytes()).hexdigest()==h,p
 notes=dict(verification='analysis/overlay_model_blend_verification.json',verification_sha256=hashlib.sha256(path.read_bytes()).hexdigest(),uncovered_rows=rows,uncovered_edges=report['uncovered_edges'],explanations=[dict(rows=rows[:2],scope='Initial loop-count branch',reason='The original code assigns 12 for appearance type 0 and 14 otherwise. Both are positive at the initial CMP/PL/BT/S. The fall-through BRA/NOP is not reached with the unchanged instructions and bounded valid RAM inputs.'),dict(rows=rows[2:],scope='Final six channel-clamp outcomes after byte conversion',reason='For completed ordinary PR=0/SZ=0 arithmetic, each value was truncated, masked by EXTU.B and converted back with FLOAT. It is therefore exactly in 0..255 at these clamps. Extrapolation tests exercise the earlier, necessary clamps; these later clamp outcomes remain unexecuted.',qualification='This explains the ordinary narrow finite contract. It is not a proof that every wide-transfer, exceptional or hardware-reentrant state makes these paths unreachable. Such states are guarded or remain outside the verified contract.')],newly_recovered_entries=0,scope='Coverage-accounting notes, no added dynamic coverage or routine credit.',sha256={str(Path(__file__)):hashlib.sha256(Path(__file__).read_bytes()).hexdigest()})
 (PROJECT/'analysis/overlay_model_blend_coverage_notes.json').write_text(json.dumps(notes,indent=2)+'\n')
 print('15 unexecuted rows / seven outcomes recorded with qualified scope; no extra coverage credit.')
if __name__=='__main__':main()
