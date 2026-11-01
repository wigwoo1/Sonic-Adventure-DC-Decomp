/* Ghidra draft only; not buildable/verified source. */

void FUN_8c10ef20(void)

{
  char cVar1;
  uint *in_r0;
  uint *in_r1;
  uint *puVar2;
  uint uVar3;
  uint uVar4;
  
  uVar3 = *in_r1;
  do {
    puVar2 = in_r1 + 1;
    if (((uVar3 >> 0x18 == 0 || (uVar3 & 0xff0000) == 0) || (uVar3 & 0xff00) == 0) ||
        (uVar3 & 0xff) == 0) {
LAB_8c10ef58:
      uVar3 = puVar2[-1];
      *(char *)in_r0 = (char)uVar3;
      if ((char)uVar3 != '\0') {
        cVar1 = *(char *)((int)puVar2 + -3);
        *(char *)((int)in_r0 + 1) = cVar1;
        if (cVar1 != '\0') {
          cVar1 = *(char *)((int)puVar2 + -2);
          *(char *)((int)in_r0 + 2) = cVar1;
          if (cVar1 != '\0') {
            *(char *)((int)in_r0 + 3) = *(char *)((int)puVar2 + -1);
          }
        }
      }
      return;
    }
    uVar4 = *puVar2;
    puVar2 = in_r1 + 2;
    *in_r0 = uVar3;
    if (((uVar4 >> 0x18 == 0 || (uVar4 & 0xff0000) == 0) || (uVar4 & 0xff00) == 0) ||
        (uVar4 & 0xff) == 0) {
      in_r0 = in_r0 + 1;
      goto LAB_8c10ef58;
    }
    uVar3 = *puVar2;
    puVar2 = in_r1 + 3;
    in_r0[1] = uVar4;
    if (((uVar3 >> 0x18 == 0 || (uVar3 & 0xff0000) == 0) || (uVar3 & 0xff00) == 0) ||
        (uVar3 & 0xff) == 0) {
      in_r0 = in_r0 + 2;
      goto LAB_8c10ef58;
    }
    uVar4 = *puVar2;
    puVar2 = in_r1 + 4;
    in_r0[2] = uVar3;
    if (((uVar4 >> 0x18 == 0 || (uVar4 & 0xff0000) == 0) || (uVar4 & 0xff00) == 0) ||
        (uVar4 & 0xff) == 0) {
      in_r0 = in_r0 + 3;
      goto LAB_8c10ef58;
    }
    uVar3 = *puVar2;
    in_r0[3] = uVar4;
    in_r0 = in_r0 + 4;
    in_r1 = puVar2;
  } while( true );
}

