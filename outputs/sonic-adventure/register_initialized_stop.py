import ast
from pathlib import Path

root = Path(__file__).resolve().parent
base = (root / 'register_initialize_then_feed.py').read_text()
provider = ast.parse((root / 'register_tick_native_provider.py').read_text())
names = next(ast.literal_eval(node.value) for node in ast.walk(provider)
             if isinstance(node, ast.Assign) and any(isinstance(t, ast.Name) and t.id == 'names' for t in node.targets))
base = base.replace("path = root / 'analysis/recovered_routines.json'", "names.update(" + repr(names) + ")\npath = root / 'analysis/recovered_routines.json'")
base = base.replace('initialize_then_feed', 'initialized_decode')
base = base.replace('Persistent original initialization and feeding:', 'Persistent original initialization, feeding and decoding:')
base = base.replace('Native input provider source query integration verification failed', 'Persistent initialized audio decoding verification failed')
base=base.replace('initialized_decode','initialized_two_chunks').replace('Persistent original initialization, feeding and decoding:', 'Persistent original initialization and two decoded chunks:')
base=base.replace('initialized_two_chunks','initialized_stop').replace('Persistent original initialization and two decoded chunks:', 'Persistent original initialization, decoding and native stop:')
base=base.replace("path = root / 'analysis/recovered_routines.json'","names.update({'sa_sound_work_stop','sa_sound_stream_decode_reset','sa_file_decode_handle_reset'})\npath = root / 'analysis/recovered_routines.json'")
exec(compile(base, str(__file__), 'exec'))
