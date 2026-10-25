"""Explicit alternate media inputs for the unchanged native disc policy."""
import hashlib,json
from pathlib import Path
from disc_init_platform_reference import DiscReference as PreviousDiscReference
P=Path(__file__).resolve().parent

class DiscReference(PreviousDiscReference):
 def __init__(self,root,c):
  super().__init__(root,c)
  if c.get('media_profile'):
   row=json.loads((P/'analysis/movie_disc_paths_media.json').read_text())['profiles'][c['media_profile']]
   path=(root/row['path']).resolve();assert path.is_relative_to((root/'work/sonic-adventure/fixtures/disc-paths').resolve())
   raw=path.read_bytes();assert len(raw)==row['bytes'] and hashlib.sha256(raw).hexdigest()==row['sha256'] and len(raw)%row['stride']==0
   self.path=path;self.offset=0;self.sectors=len(raw)//row['stride'];self.base=row['base_lba'];self.stride=row['stride']
