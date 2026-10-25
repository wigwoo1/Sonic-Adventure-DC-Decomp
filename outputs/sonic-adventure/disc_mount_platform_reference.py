"""Select explicit raw media without changing native disc service policy."""
import hashlib,json
from pathlib import Path
from disc_init_platform_reference import DiscReference as PreviousDiscReference
P=Path(__file__).resolve().parent
class DiscReference(PreviousDiscReference):
 def __init__(self,root,c):
  super().__init__(root,c)
  if c.get('mount_profile'):
   row=json.loads((P/'analysis/movie_mount_media.json').read_text())['profiles'][c['mount_profile']]
   self.path=(root/row['path']).resolve();assert self.path.is_relative_to((root/'work/sonic-adventure/fixtures/disc-mount').resolve())
   data=self.path.read_bytes();assert len(data)==row['bytes'] and hashlib.sha256(data).hexdigest()==row['sha256'] and len(data)%row['stride']==0
   self.offset=0;self.sectors=len(data)//row['stride'];self.base=row['base_lba'];self.stride=row['stride']
