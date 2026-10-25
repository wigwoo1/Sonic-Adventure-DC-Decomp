/* Ghidra draft only; not buildable/verified source. */

void FUN_8c10fb5c(void)

{
  int in_r0;
  undefined1 *in_r1;
  undefined1 *in_r2;
  undefined1 *puVar1;
  undefined1 *puVar2;
  
  if (in_r0 != 0) {
    puVar2 = in_r2 + in_r0;
    do {
      *in_r1 = *in_r2;
      if (puVar2 <= in_r2 + 1) {
        return;
      }
      in_r1[1] = in_r2[1];
      if (puVar2 <= in_r2 + 2) {
        return;
      }
      puVar1 = in_r2 + 3;
      in_r1[2] = in_r2[2];
      if (puVar2 <= puVar1) {
        return;
      }
      in_r2 = in_r2 + 4;
      in_r1[3] = *puVar1;
      in_r1 = in_r1 + 4;
    } while (in_r2 < puVar2);
  }
  return;
}

