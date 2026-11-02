"""Compact boundary observations; complete job/image bytes are checked at return."""
import hashlib,struct
import pixel_jobs_native as native
from pixel_jobs_reference import Jobs
from pvr_pixels_native import snapshot as stats_snapshot

class ReferenceJobs(Jobs):
 def __init__(self,seed):
  super().__init__(seed);self.word_digest=hashlib.sha256(struct.pack('<512I',*self.words)).digest()
 def committed(self,*args):
  before=self.job_id
  super().committed(*args)
  if self.job_id!=before:self.word_digest=hashlib.sha256(struct.pack('<512I',*self.words)).digest()
 def observation(self):
  return self.job_id,self.processed,self.last_job,self.cursor,self.count,self.ready,self.word_digest,self.stats

def native_observation(s):
 return s.job_id,s.processed,s.last_job,s.cursor,s.count,s.ready,hashlib.sha256(bytes(s.words)).digest(),stats_snapshot(s.stats)

def observation(jobs):return jobs.observation() if jobs is not None else None
