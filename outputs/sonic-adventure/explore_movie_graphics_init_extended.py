"""Targeted original/native alignment,weight-normalization and capacity cases."""
import struct
from pathlib import Path
def bits(x):return struct.unpack('<I',struct.pack('<f',x))[0]
def cases():
 for low in (1,3,15,31):yield dict(entry=0x8c656940,kind='vertex_alignment',mode=0,gfx_observed_fpscr=True,gfx_config_words=[(0x24,0xacf5fd00+low)])
 weights=([0]*5,[20]*5,[30]*5,[150,0,0,0,0],[100,0,0,0,0],[99,1,0,0,0],[0,0,100,0,0],[0,0,0,0,100])
 for values in weights:yield dict(entry=0x8c656940,kind='list_weights',mode=0,gfx_observed_fpscr=True,gfx_config_words=[(0x2c+i*4,bits(x)) for i,x in enumerate(values)])
 for available,values in ((0,[0]*5),(8,[0]*5),(40,[0]*5),(160,[100,0,0,0,0])):
  yield dict(entry=0x8c656940,kind='minimum_list_space',mode=0,gfx_observed_fpscr=True,gfx_config_words=[(0x28,available)]+[(0x2c+i*4,bits(x)) for i,x in enumerate(values)],gfx_global_words=[(0x8c8a2d5c,0x40)])
 for available in (0x80000000,0xffffff00,0xfffffff0):yield dict(entry=0x8c656940,kind='unsigned_capacity',mode=0,gfx_observed_fpscr=True,gfx_config_words=[(0x28,available)])
 for reserved in (0x200000,0x400000,0x800000,0x1000000,0x7fffffff):yield dict(entry=0x8c656940,kind='texture_capacity',mode=0,gfx_observed_fpscr=True,gfx_config_words=[(0x1c,reserved)])
def main():
 parent=Path(__file__).with_name('explore_movie_graphics_init.py');code=parent.read_text()
 code=code.replace('movie_graphics_init_exploration.json','movie_graphics_init_extended_exploration.json').replace('files=[Path(__file__),','files=[Path(__file__),Path(parent_script),')
 scope=dict(__name__='graphics_extended_exploration',__file__=__file__,parent_script=str(parent))
 exec(compile(code,str(parent),'exec'),scope);scope['cases']=cases;scope['main']()
if __name__=='__main__':main()
