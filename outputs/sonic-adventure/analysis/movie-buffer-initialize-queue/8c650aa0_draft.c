/* Ghidra draft only; not buildable/verified source. */

void FUN_8c650aa0(void)

{
  int *piVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  uint uVar9;
  
  piVar1 = DAT_8c650b24;
  *DAT_8c650b28 = 0;
  *DAT_8c650b2c = 0;
  *DAT_8c650b30 = 0;
  *piVar1 = (int)DAT_8c650b20;
  *DAT_8c650b38 = DAT_8c650b34;
  *DAT_8c650b3c = 0;
  *DAT_8c650b40 = 0x10;
  *DAT_8c650b44 = 0x10;
  iVar5 = DAT_8c650b5c;
  puVar4 = DAT_8c650b54;
  puVar2 = DAT_8c650b48;
  *DAT_8c650b48 = 0;
  puVar2[1] = 0;
  puVar3 = DAT_8c650b50;
  puVar2 = DAT_8c650b4c;
  uVar9 = (uint)DAT_8c650b22;
  *DAT_8c650b4c = 0;
  puVar2[1] = 0;
  uVar8 = 0;
  *puVar3 = 0;
  *puVar4 = 7;
  *DAT_8c650b58 = 1;
  do {
    iVar6 = iVar5 + uVar9 + uVar8;
    iVar7 = iVar5 + uVar8;
    uVar8 = uVar8 + 0x20;
    *(undefined4 *)(iVar7 + 0x18) = 0;
    *(undefined4 *)(iVar6 + 0x18) = 8;
  } while (uVar8 < uVar9);
  return;
}

