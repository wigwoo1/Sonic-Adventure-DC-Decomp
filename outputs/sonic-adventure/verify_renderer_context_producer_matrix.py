"""Use the canonical original matrix initializer with an actual allocated buffer."""
from pathlib import Path
P=Path(__file__).resolve().parent
disc_builder=P/'verify_renderer_context_producer_disc.py'
source=disc_builder.read_text();ending="exec(compile(code, str(parent), 'exec'), globals())";assert source.rstrip().endswith(ending)
exec(compile(source.rsplit(ending,1)[0],str(disc_builder),'exec'),globals())
code=code.replace('renderer_context_producer_disc_verification.json','renderer_context_producer_matrix_verification.json')
code=code.replace('renderer_context_producer_disc_frontier.json','renderer_context_producer_matrix_frontier.json')
marker=" phase(0x8c09844a,[],'original task-pool initialization');"
assert code.count(marker)==1
code=code.replace(marker,""" phase(0x8c098b0c,[17,64],'original matrix-stack buffer allocation')
 matrix_buffer=base[1].r[0];exact(matrix_buffer!=0,'actual allocated matrix-stack buffer')
 phase(0x8c638fe4,[matrix_buffer,16,0],'original matrix-stack and floating-matrix initializer')
 exact(q.get(base[0],0x8c88f5f8)==matrix_buffer and q.get(base[0],0x8c88f69c)==1 and q.get(base[0],0x8c88f698)==16,'independent real matrix-stack cursor/depth/capacity')
"""+marker)
marker="P/'verify_renderer_context_producer.py'";assert code.count(marker)==1
code=code.replace(marker,marker+",P/'verify_renderer_context_producer_disc.py',P/'source/sa_draw_frame_dependencies.c',P/'analysis/draw_frame_setup_verification.json'")
code=code.replace('renderer_context_from_original_startup=True,','renderer_context_from_original_startup=True,matrix_stack_from_original_initializer=True,')
code=code.replace('caller arena/scene/matrix inputs remain declared','caller arena/scene inputs remain declared;the actual calloc buffer andcanonical638fe4 produce matrix-stack metadata andfloating-matrix state')
exec(compile(code,str(parent),'exec'),globals())
