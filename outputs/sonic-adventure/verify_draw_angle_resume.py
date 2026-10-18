"""Replay the retained angle boundary proof without changing historical reports."""
import json
from pathlib import Path
import verify_draw_angle_boundaries as boundaries

PROJECT = Path(__file__).resolve().parent


def normalized(report):
    result = dict(report)
    result['sha256'] = {str(Path(k).resolve()): v for k, v in report['sha256'].items()}
    return result


def main():
    target = PROJECT / 'analysis/draw_angle_boundaries_verification.json'
    retained = target.read_bytes()
    previous = json.loads(retained)
    original_write = Path.write_text
    captured = []

    def capture(path, data, *args, **kwargs):
        if path.resolve() == target:
            fresh = json.loads(data)
            assert normalized(fresh) == normalized(previous), 'Replay differs from retained proof'
            captured.append(fresh)
            return len(data)
        return original_write(path, data, *args, **kwargs)

    Path.write_text = capture
    try:
        boundaries.main()
    finally:
        Path.write_text = original_write
    assert len(captured) == 1 and target.read_bytes() == retained
    print('Fresh 60-case angle replay matches the retained proof; historical report preserved.')


if __name__ == '__main__':
    main()
