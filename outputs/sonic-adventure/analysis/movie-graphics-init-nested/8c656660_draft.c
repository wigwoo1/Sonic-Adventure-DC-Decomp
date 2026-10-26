/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c656760) */
/* WARNING: Removing unreachable block (ram,0x8c656754) */
/* WARNING: Removing unreachable block (ram,0x8c65675a) */
/* WARNING: Removing unreachable block (ram,0x8c656766) */

int FUN_8c656660(int param_1,int param_2)

{
  int *piVar1;
  undefined *puVar2;
  int *piVar3;
  int *piVar4;
  undefined *puVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  uint uVar9;
  int iVar10;
  uint uVar11;
  uint uVar12;
  
  iVar10 = DAT_8c65678c;
  piVar1 = DAT_8c656784;
  iVar6 = 0;
  DAT_8c656784[0x15] = 2;
  *piVar1 = param_1;
  piVar1[0x16] = param_2;
  piVar3 = DAT_8c656794;
  puVar2 = PTR_DAT_8c656788;
  iVar7 = *(int *)PTR_DAT_8c656788;
  if (iVar7 == iVar10) {
    *(undefined4 *)PTR_DAT_8c656790 = 1;
    piVar4 = DAT_8c656798;
    *DAT_8c656798 = *piVar3;
    piVar4[*piVar3 + -6] = 2;
    *DAT_8c65679c = 0;
    piVar3[6] = 1;
    uVar8 = *(uint *)(param_1 + 0x10) & 1;
    uVar12 = *(uint *)(param_1 + 0x10) & 2;
    uVar9 = *(uint *)(param_1 + 0x10) & 4;
    uVar11 = *(uint *)(param_1 + 0x10) & 8;
    FUN_8c656f50(uVar8,uVar12,uVar9,uVar11,0,iVar7,0);
    uVar9 = FUN_8c657096(uVar8,uVar12,uVar9,uVar11,0,1);
    FUN_8c6573c4(uVar9);
    puVar2 = PTR_FUN_8c6567a0;
    iVar10 = (int)DAT_8c656780;
    piVar1[0x16] = (uint)piVar1[0x16] >> 1;
    (*(code *)puVar2)(iVar10,piVar1[6],piVar1[9]);
    if ((uint)(DAT_8c6567a4 - piVar1[0x18]) < (uint)piVar1[0x16]) {
      piVar1[0x16] = DAT_8c6567a4 - piVar1[0x18];
    }
    iVar6 = (*(code *)PTR_FUN_8c6567ac)(piVar1[0x16],DAT_8c6567a8 + -4);
    if (iVar6 == 0) {
      *DAT_8c6567b0 = piVar1[0x16];
      (*(code *)PTR_FUN_8c6567b8)
                (*DAT_8c6567b4 >> 7,DAT_8c6567b4[1] >> 7,DAT_8c6567b4[2] >> 7,DAT_8c6567b4[3] >> 7,
                 uVar9 & ~piVar1[6],0);
      *DAT_8c65679c = 1;
    }
  }
  else {
    uVar9 = *(uint *)PTR_DAT_8c656788;
    uVar11 = (uint)DAT_8c656840;
    *(uint *)PTR_DAT_8c656788 = uVar9 | uVar11;
    puVar5 = PTR_FUN_8c656844;
    if ((uVar9 | uVar11) == (int)DAT_8c656842) {
      *(int *)puVar2 = iVar10;
      iVar6 = (*(code *)puVar5)(piVar1[0x15]);
    }
  }
  return iVar6;
}

