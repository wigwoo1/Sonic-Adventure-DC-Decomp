import ast
from pathlib import Path

root = Path(__file__).resolve().parent
base = (root / 'register_initialize_then_feed.py').read_text()
provider = ast.parse((root / 'register_tick_native_provider.py').read_text())
names = next(ast.literal_eval(node.value) for node in ast.walk(provider)
             if isinstance(node, ast.Assign) and any(isinstance(t, ast.Name) and t.id == 'names' for t in node.targets))
names.update({'sa_sound_source_pool_initialize','sa_sound_source_create','sa_sound_source_reset','sa_sound_work_pool_initialize','sa_sound_stream_pool_initialize','sa_file_decode_pool_initialize','sa_sound_work_create','sa_sound_work_reset','sa_sound_source_ring_size_get','sa_sound_source_mirror_size_get','sa_sound_stream_create','sa_file_decode_handle_allocate','sa_sound_work_start','sa_sound_work_sample_limit_set','sa_sound_work_remaining_limit_set'})
base = base.replace("path = root / 'analysis/recovered_routines.json'", "names.update(" + repr(names) + ")\npath = root / 'analysis/recovered_routines.json'")
base = base.replace('initialize_then_feed', 'native_audio_refill')
base = base.replace('Persistent original initialization and feeding:', 'Native audio producer refill with output consumption and decoding:')
base = base.replace('Native input provider source query integration verification failed', 'Persistent initialized audio decoding verification failed')
exec(compile(base, str(__file__), 'exec'))
