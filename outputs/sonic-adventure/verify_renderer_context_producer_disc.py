"""Retain the raw-vector negative control; add the declared host-disc contract."""
from pathlib import Path
P = Path(__file__).resolve().parent
builder = P / 'verify_renderer_context_producer.py'
source = builder.read_text()
ending = "exec(compile(code, str(parent), 'exec'), globals())"
assert source.count(ending) == 1
exec(compile(source.replace(ending, ''), str(builder), 'exec'), globals())
code = code.replace('import renderer_context_producer_fixtures as q', 'import renderer_context_producer_disc_fixtures as q')
code = code.replace('renderer_context_producer_verification.json', 'renderer_context_producer_disc_verification.json')
code = code.replace('renderer_context_producer_frontier.json', 'renderer_context_producer_disc_frontier.json')
marker = "P/'renderer_context_producer_fixtures.py'"
assert code.count(marker) == 1
code = code.replace(marker, marker + ",P/'renderer_context_producer_disc_fixtures.py',P/'verify_renderer_context_producer.py'")
code = code.replace('Raw original RAM with null polygon/view roots.', 'Original game RAM with null polygon/view roots and an explicit external host-disc vector at8c0000bc;the raw unbound-vector control stops honestly at null and is retained separately.')
exec(compile(code, str(parent), 'exec'), globals())
