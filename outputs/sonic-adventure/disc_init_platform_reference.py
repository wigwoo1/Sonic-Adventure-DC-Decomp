"""Versioned correction to the retained native-disc policy test model.

The native C copies sixteen bytes of its host identity string. The older Python
model assigned seventeen bytes, including NUL, to a sixteen-byte RAM slice.
Preserve that historical file; correct only this newly exercised model path.
This is a host-policy model, not recovered Dreamcast firmware.
"""
from boot_disc_reference import DiscReference as PreviousDiscReference
VERSION=b'SA-NATIVE v 1.00'
assert len(VERSION)==16

class DiscReference(PreviousDiscReference):
 def service(self,m,op,a,b):
  if op==2 and not self.busy:
   index=next(((self.cursor+n)%8 for n in range(8) if self.jobs[(self.cursor+n)%8][1]==1),None)
   if index is not None:
    job=self.jobs[index]
    if job[5]:
     before=len(m);m[job[7]:job[7]+16]=VERSION;assert len(m)==before
     self.cursor=(index+1)%8;job[1]=2;return 0,0
  return super().service(m,op,a,b)
