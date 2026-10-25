"""Finish direct timing-constructor paths without rewriting historical proofs."""
import collections, hashlib, itertools, json
from pathlib import Path
import verify_movie_render_config as v

def digest(p): return hashlib.sha256(Path(p).read_bytes()).hexdigest()

def cases():
    # The constructors read mode_flags at 8c8a2d78, not flags at 8c8a2d5c.
    for entry, mode, bits, selector in itertools.product(
            (v.TYPE0, v.TYPE1), (0, 4), (0, 1, 4, 8, 0x10, 0x20, 0x24, 0x28), (0, 6)):
        yield dict(entry=entry, mode=mode, mode_flags=bits, format=selector)
    for mode, bits, interlace in itertools.product(
            (0, 4), (0, 1, 4, 8, 0x20, 0x24, 0x28, 0x2c), (0, 1)):
        yield dict(entry=v.LAYOUT, mode=mode, mode_flags=bits, interlace=interlace,
                   screen_width=320, screen_height=240, pixel_format=2)
    for mode, width, height, pixel in itertools.product(
            (0, 4), (-1, 0, 720), (-1, 576), (0, 4)):
        yield dict(entry=v.LAYOUT, mode=mode, screen_width=width & 0xffffffff,
                   screen_height=height & 0xffffffff, pixel_format=pixel)
    for mode,bits in itertools.product((0,4),(0,1,0x20,0x24,0x28)):
        yield dict(entry=v.LAYOUT,mode=mode,mode_flags=bits,antialias=1)
    # The original has no pixel-index bounds check. Index six reads the next
    # scratch word, before it is initialized as the output table. Exercise this
    # original behavior using RAM contents, never patched code or literal data.
    for mode,depth in itertools.product((0,4),(15,0xfffffff8)):
        yield dict(entry=v.LAYOUT,mode=mode,pixel_format=6,scratch_depth=depth)

def main():
    ram=v.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.previous.seed_ram()
    records=[]; coverage=set(); edges=set()
    for c in cases():
        m,s,f=v.prepare(ram,c)
        if 'scratch_depth' in c:
            allocation=v.load(m,0x8c657ea0,2)
            allocation=allocation-0x10000 if allocation&0x8000 else allocation
            v.store(m,s.r[15]-32+allocation+32,c['scratch_depth'])
        assert v.load(m,0x8c8a2d78)==c.get('mode_flags',0)
        assert v.load(m,0x8c8a2d58)==c.get('format',0)
        before=v.effects.State.from_buffer_copy(s); fp=v.effects.Fpu.from_buffer_copy(f)
        record,path,a,s,f=v.compare(m,s,f,c)
        assert record['status']==0, (c,record['status'],record['stop_pc'])
        record['independent_checks']=v.semantics(m,a,before,fp,s,f,record,c)+2
        records.append(record); coverage.update(path)
        for j,pc in enumerate(path):
            text=v.ROWS.get(pc,''); distance=2 if text.startswith(('bt/s ','bf/s ')) else 1
            if text.startswith(('bt ','bf ','bt/s ','bf/s ')) and j+distance<len(path):
                edges.add((pc,path[j+distance]))
    files=[Path(__file__),Path(v.__file__),v.MP,v.P/'analysis/movie_render_config_connection.json']
    report=dict(cases_passed=len(records),status_counts=dict(collections.Counter(r['status'] for r in records)),
                independent_checks=sum(r['independent_checks'] for r in records),
                covered_addresses=[hex(x) for x in sorted(coverage)],
                covered_conditional_edges=[[hex(a),hex(b)] for a,b in sorted(edges)],
                coverage_by_routine=v.coverage_rows(coverage,edges),dll_sha256=digest(v.effects.DLL),
                sha256={str(p):digest(p) for p in files},cases=records,
                scope='Explicit direct mode-flag, timing-selector, pixel-format, size and integer-register-bank fixtures. Original instructions and native code compare complete machine, RAM and software peripheral state. Negative and zero sizes test arithmetic; they are not supported user-facing display modes. No retail code or literals are changed.')
    (v.P/'analysis/movie_timing_constructors_verification.json').write_text(json.dumps(report,indent=2)+'\n')
    print({k:report[k] for k in ('cases_passed','status_counts','independent_checks')})
    for row in report['coverage_by_routine']:
        if int(row['address'],16) in (v.LAYOUT,v.TYPE0,v.TYPE1): print(row)

if __name__=='__main__': main()
