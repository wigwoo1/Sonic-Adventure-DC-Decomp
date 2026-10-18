from pathlib import Path
p=Path(__file__).resolve().parent
generator=p/'create_initialized_repeated_decode_verifier.py'
s=generator.read_text()
start=s.index("  assert struct.unpack_from('<I',native,0xa60034)[0]==288")
end=s.index("s=s[:at]+extra+s[at:]")
s=s[:start]+'''  for (address,length),before in zip(wait_regions,wait_before):assert native[address:address+length]==before
  assert struct.unpack_from('<I',native,0xa60034)[0]==256
  assert struct.unpack_from('<I',native,state+4)[0]==0 and r[15]==0x8cf00000
  assert struct.unpack_from('<2I',native,state+0xa0)==(0,0)
\x27\x27\x27
'''+s[end:]
s=s.replace('Second public update waits because sample availability88 bytes is below initialized minimum32 blocks. Independent complete stream, handle, work, encoded and sample reservation objects, sample buffers and histories remain unchanged; consumed256 and phase0 preserved.', 'Second public update reports zero decoded samples/bytes with88 bytes available per output channel. Independent complete encoded and sample reservation objects, sample buffers, predictor histories and coefficients remain unchanged; consumed256 and phase0 preserved. Request pointers/capacity fields change and are covered by full retail RAM/CPU/FPU comparisons.')
(p/'create_initialized_backpressure_verifier.py').write_text(s)
base=(p/'register_initialized_decode.py').read_text().replace("exec(compile(base, str(__file__), 'exec'))", "base=base.replace('initialized_decode','initialized_backpressure').replace('Persistent original initialization, feeding and decoding:', 'Persistent initialized audio backpressure:')\nexec(compile(base, str(__file__), 'exec'))")
(p/'register_initialized_backpressure.py').write_text(base)
