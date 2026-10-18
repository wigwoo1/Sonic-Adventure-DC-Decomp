from pathlib import Path
p=Path(__file__).resolve().parent
s=(p/'register_initialized_backpressure.py').read_text()
s=s.replace("base=base.replace('initialized_decode','initialized_backpressure').replace('Persistent original initialization, feeding and decoding:', 'Persistent initialized audio backpressure:')", "base=base.replace('initialized_decode','initialized_two_chunks').replace('Persistent original initialization, feeding and decoding:', 'Persistent original initialization and two decoded chunks:')")
(p/'register_initialized_two_chunks.py').write_text(s)
