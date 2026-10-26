"""Fresh isolated constructor-evidence build preserving published artifacts."""
from pathlib import Path
import verify_movie_graphics_root_rebuild as previous
def main():
 parent=Path(previous.__file__);source=parent.read_text().replace('graphics-root-build-check','graphics-constructor-build-check').replace('movie_graphics_root_rebuild_verification','movie_graphics_constructor_rebuild_verification').replace('Root evidence fresh201-source','Constructor evidence fresh201-source').replace('root arithmetic/option evidence;root still partial','constructor evidence;graphics root still partial')
 namespace=dict(__name__='constructor_isolated_rebuild',__file__=str(Path(__file__)));exec(compile(source,str(parent),'exec'),namespace);namespace['main']()
if __name__=='__main__':main()
