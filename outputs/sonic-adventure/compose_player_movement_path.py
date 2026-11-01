"""Compose routing/shape metadata without altering the original per-family proofs."""
import hashlib,json
from pathlib import Path
P=Path(__file__).resolve().parent
files=[P/'analysis/player_movement_route_translation.json',P/'analysis/player_movement_shapes_translation.json']
parts=[json.loads(path.read_text()) for path in files]
manifest=dict(routines=[dict(r,source=t['source']) for t in parts for r in t['routines']],
              resume_routes=[r for t in parts for r in t['resume_routes']],
              return_manifest=parts[-1]['return_manifest'],source_paths=[t['source'] for t in parts],
              canonical_reused_entries=[],newly_registered_entries=0,instruction_rows=sum(t['instruction_rows'] for t in parts),
              scope='Complete original routing and shape bodies together;no qualification credit from generation.',
              sha256={str(path):hashlib.sha256(path.read_bytes()).hexdigest() for path in files})
text=json.dumps(manifest,indent=2)+'\n';target=P/'analysis/player_movement_path_translation.json'
if target.exists():assert target.read_text()==text,'Preserve prior metadata;write an additive revision.'
else:target.write_text(text)
print('Combined routing/shape metadata matches complete original contributors')
