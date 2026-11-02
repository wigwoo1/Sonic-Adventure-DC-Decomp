/* Ghidra draft only; not buildable/verified source. */

void FUN_8c01c29a(void)

{
  int *piVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  
  piVar1 = DAT_8c01c344;
  puVar5 = DAT_8c01c340 + 0x96;
  for (puVar4 = DAT_8c01c340; puVar4 < puVar5; puVar4 = puVar4 + 6) {
    iVar3 = *piVar1;
    uVar2 = *(undefined4 *)(iVar3 + 0x24);
    *puVar4 = *(undefined4 *)(iVar3 + 0x20);
    puVar4[1] = uVar2;
    puVar4[2] = *(undefined4 *)(iVar3 + 0x28);
    iVar3 = *piVar1;
    uVar2 = *(undefined4 *)(iVar3 + 0x24);
    puVar4[3] = *(undefined4 *)(iVar3 + 0x20);
    puVar4[4] = uVar2;
    puVar4[5] = *(undefined4 *)(iVar3 + 0x28);
  }
  *DAT_8c01c348 = 0;
  return;
}

