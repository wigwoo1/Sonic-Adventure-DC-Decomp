/* Ghidra draft only; not buildable/verified source. */

void FUN_8c641bc0(void)

{
  undefined *puVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  undefined *puVar5;
  undefined1 *puVar6;
  
  puVar5 = PTR_DAT_8c641c88;
  puVar6 = &DAT_8c88fe1c;
  uVar2 = *(uint *)*DAT_8c641c84;
  uVar3 = uVar2 >> 0x18;
  *DAT_8c641c8c = uVar3;
  *DAT_8c641c90 = uVar3;
  puVar1 = PTR_FUN_8c641c9c;
  iVar4 = 4;
  uVar2 = DAT_8c641c94 & uVar2 | DAT_8c641c98;
  do {
    (*(code *)puVar1)(uVar2,puVar5,0x20,puVar6);
    iVar4 = iVar4 + -1;
    uVar2 = uVar2 + 0x20;
    puVar5 = puVar5 + 0x20;
  } while (iVar4 != 0);
  *(int *)*DAT_8c641c84 = *(int *)*DAT_8c641c84 + (int)DAT_8c641c80;
  return;
}

