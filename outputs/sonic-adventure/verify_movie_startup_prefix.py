"""Check retained late-boot controls without substituting earlier startup."""
from pathlib import Path
P=Path(__file__).resolve().parent;parent=P/'verify_movie_render_config_prefix.py';code=parent.read_text()
code=code.replace('import verify_movie_render_config as v','import verify_movie_startup as v')
code=code.replace('import verify_movie_context_prefix as prior_prefix','import verify_movie_heap_prefix as prior_prefix')
code=code.replace('movie_render_config_connection.json','movie_startup_connection.json').replace('movie_render_config_prefix_verification.json','movie_startup_prefix_verification.json')
exec(compile(code,str(parent),'exec'),globals())
