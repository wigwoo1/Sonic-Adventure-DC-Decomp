/* Ghidra draft only; not buildable/verified source. */

void FUN_8c65796e(void)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  int iVar4;
  
  puVar1 = DAT_8c657a04;
  *DAT_8c657a04 = 0;
  puVar2 = DAT_8c657a08;
  *DAT_8c657a08 = 0xffffffff;
  puVar3 = DAT_8c657a0c;
  *DAT_8c657a0c = 0xffffffff;
  puVar1[3] = 0xffffffff;
  puVar2[3] = 0;
  puVar3[3] = 0;
  puVar1[6] = 0;
  *DAT_8c657a10 = 0;
  iVar4 = *DAT_8c657a14;
  while ((iVar4 != 0 && ((*(code *)*DAT_8c657a18)(DAT_8c657a18[1]), *DAT_8c657a10 < 0xb))) {
    iVar4 = *DAT_8c657a14;
  }
  if (*DAT_8c657a1c == 0) {
    *DAT_8c657a10 = 0;
    iVar4 = *DAT_8c657a20;
    while ((iVar4 != 0 && ((*(code *)*DAT_8c657a18)(DAT_8c657a18[1]), *DAT_8c657a10 < 0xb))) {
      iVar4 = *DAT_8c657a20;
    }
  }
  *DAT_8c657a20 = 0;
  *DAT_8c657a14 = 0;
  return;
}

