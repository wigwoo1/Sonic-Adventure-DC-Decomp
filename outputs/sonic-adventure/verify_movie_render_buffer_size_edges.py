"""Additional measured size branches, retaining the earlier 216-case evidence."""
import types
from pathlib import Path
import verify_movie_render_buffer_format as suite

def cases():
 for w in (1,2,4,8,16,32,64,128,256,512,1024):
  yield suite.case(suite.SIZE,w=w,h=w,kind=3,flags=0)
 for w,h in ((7,9),(9,7),(32,16),(16,32)):
  for flags in (0,1):yield suite.case(suite.SIZE,w=w,h=h,kind=3,flags=flags)
 for entry in suite.SIZERS:
  for flags in (0,1):yield suite.case(entry,w=32,h=16,flags=flags,pixel=0x30000000)
 yield suite.case(0x8c654334,w=8,h=16,flags=0)

def main():
 namespace=dict(suite.__dict__,cases=cases,__file__=__file__)
 code=Path(suite.__file__).read_text().replace('movie_render_buffer_format_verification.json','movie_render_buffer_size_edges_verification.json')
 # Keep independent field/size calculations identical to the earlier suite.
 namespace['__name__']='render_size_edge_suite';exec(compile(code,str(suite.__file__),'exec'),namespace)
 namespace['cases']=cases;namespace['__file__']=__file__;namespace['main']()

if __name__=='__main__':main()
