SH-4 numerical instruction dependency

Author: Stefanos Kornilios Mitsis Poiitidis (skmp)
License: MIT; the complete copyright and permission notice is preserved at
the beginning of sh4_fpu_approx.h. Keep that notice with redistributed code.

Source: https://gist.github.com/skmp/4bd7b38149fc455978b5104201573236
Pinned revision: 29f17b480213f063c6db998ecba5745d3f4ad4b3
Raw source:
https://gist.githubusercontent.com/skmp/4bd7b38149fc455978b5104201573236/raw/29f17b480213f063c6db998ecba5745d3f4ad4b3/sh4_fpu_approx.h
SHA256: cc95d6a3ede9ab57b6f332fcec5dacc97cda16515699dcdcd4bcf4978c869d2f
Downloaded: 2026-10-15
Local modifications: none.

Research: https://skmp.dev/blog/sh4-fpu/
Specification reference: https://www.renesas.com/en/document/mas/sh-4a-software-manual
The SH-4A manual is a related architectural reference. The author's measured
Dreamcast SH7091 semantics, including differences from that manual, are
implemented by this dependency. Upstream hardware-validation claims are the
author's; the original captures were not found in the public repository tree
inspected for this checkpoint and have not been replayed locally.

This header implements FIPR, FTRV, FSCA and FSRRA with integer arithmetic,
coefficient tables, FPSCR effects and trap statuses. It is support code for
the native port, not an additional recovered Sonic Adventure routine.
Our adapter and tests live in source/sh4_vector_numeric.c and the associated
build/verify scripts two directory levels above this folder.
