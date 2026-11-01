/* Ghidra draft only; not buildable/verified source. */

void FUN_8c098a10(void)

{
  int *piVar1;
  uint uVar2;
  undefined4 *puVar3;
  undefined1 *puVar4;
  uint uVar5;
  undefined4 *puVar6;
  
  uVar2 = DAT_8c098a70;
  piVar1 = DAT_8c098a6c;
  puVar4 = DAT_8c098a68;
  *DAT_8c098a6c = (int)DAT_8c098a68;
  puVar6 = (undefined4 *)(puVar4 + uVar2);
  uVar5 = 0;
  do {
    *puVar4 = 0xff;
    puVar4[1] = 0xff;
    uVar5 = uVar5 + 3;
    puVar4[2] = 0xff;
    puVar4 = puVar4 + 3;
  } while (uVar5 < uVar2);
  *(undefined4 **)*piVar1 = puVar6;
  *(undefined4 *)(*piVar1 + 4) = 0;
  *(undefined4 **)(*piVar1 + 8) = puVar6;
  *(undefined4 *)(*piVar1 + 0xc) = 0;
  *puVar6 = 0;
  puVar6[1] = *piVar1;
  puVar3 = DAT_8c098a74;
  puVar6[2] = 0;
  puVar6[3] = *piVar1;
  *puVar3 = 0;
  return;
}

