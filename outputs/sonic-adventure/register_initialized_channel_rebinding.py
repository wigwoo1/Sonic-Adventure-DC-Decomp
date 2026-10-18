import ast
from pathlib import Path

root = Path(__file__).resolve().parent
base = (root / 'register_initialize_then_feed.py').read_text()
provider = ast.parse((root / 'register_tick_native_provider.py').read_text())
names = next(ast.literal_eval(node.value) for node in ast.walk(provider)
             if isinstance(node, ast.Assign) and any(isinstance(t, ast.Name) and t.id == 'names' for t in node.targets))
names.add('sa_sound_work_channel_source_set')
base = base.replace("path = root / 'analysis/recovered_routines.json'", "names.update(" + repr(names) + ")\npath = root / 'analysis/recovered_routines.json'")
base = base.replace('initialize_then_feed', 'initialized_channel_rebinding')
base = base.replace('Persistent original initialization and feeding:', 'Persistent initialized decoding with channel source replacement:')
base = base.replace('Native input provider source query integration verification failed', 'Persistent initialized audio decoding verification failed')
exec(compile(base, str(__file__), 'exec'))
