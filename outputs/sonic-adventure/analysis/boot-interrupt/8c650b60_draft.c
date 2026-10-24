/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c650b60(void)

{
  int *piVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  uint uVar9;
  uint uVar10;
  
  piVar1 = DAT_8c650c28;
  *DAT_8c650c28 = 0;
  puVar2 = DAT_8c650c2c;
  *DAT_8c650c2c = 1;
  *puVar2 = 0;
  do {
  } while (*piVar1 != 0);
  *DAT_8c650c30 = *DAT_8c650c30 & 0xfffffffe;
  *puVar2 = 1;
  *puVar2 = 0;
  puVar3 = DAT_8c650c3c;
  puVar2 = DAT_8c650c34;
  *DAT_8c650c34 = 0;
  puVar2[1] = 0;
  iVar6 = DAT_8c650c48;
  puVar5 = DAT_8c650c44;
  puVar4 = DAT_8c650c40;
  puVar2 = DAT_8c650c38;
  *DAT_8c650c38 = 0;
  puVar2[1] = 0;
  uVar10 = (uint)DAT_8c650c20;
  *puVar3 = 0;
  *puVar4 = 7;
  *puVar5 = 7;
  uVar9 = 0;
  do {
    iVar7 = iVar6 + uVar10 + uVar9;
    iVar8 = iVar6 + uVar9;
    uVar9 = uVar9 + 0x20;
    *(undefined4 *)(iVar8 + 0x18) = 0;
    *(undefined4 *)(iVar7 + 0x18) = 8;
  } while (uVar9 < uVar10);
  return 0;
}

