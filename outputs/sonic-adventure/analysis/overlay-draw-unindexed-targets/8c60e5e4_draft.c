/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c60e602) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void callback_8c60e5e4(float param_1,undefined4 param_2,uint *param_3,undefined4 *param_4)

{
  ushort uVar1;
  undefined *puVar2;
  undefined4 *puVar3;
  undefined *puVar4;
  ushort *puVar5;
  ushort *puVar6;
  ushort *puVar7;
  int iVar8;
  undefined4 *puVar9;
  undefined4 *puVar10;
  float fVar11;
  undefined8 in_dr2;
  double dVar12;
  bool bVar13;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  puVar7 = _DAT_8c60e77c;
  puVar3 = _DAT_8c60e778;
  puVar6 = _DAT_8c60e774;
  puVar5 = _DAT_8c60e770;
  if (param_4 == (undefined4 *)0x0) {
    *_DAT_8c60e770 = 0;
    *puVar6 = 0;
    *puVar7 = 0;
  }
  else {
    fVar11 = (float)(int)param_4[1];
    dVar12 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),fVar11);
    if ((int)param_4[1] < 0) {
      if (in_FPSCR_PR == '\0') {
        dVar12 = (double)CONCAT44(_DAT_8c60e780,fVar11 + _DAT_8c60e780);
      }
      else {
        dVar12 = (double)CONCAT44(_DAT_8c60e780,fVar11) + (double)CONCAT44(_DAT_8c60e780,fVar11);
      }
    }
    if (in_FPSCR_PR == '\0') {
      bVar13 = param_1 < SUB84(dVar12,0);
    }
    else {
      bVar13 = (double)CONCAT44(param_1,param_2) < dVar12;
    }
    if (!bVar13) {
      return;
    }
    *_DAT_8c60e770 = *(ushort *)((int)param_4 + 10) & (ushort)_DAT_8c60e784;
    *puVar6 = (ushort)((*(ushort *)((int)param_4 + 10) & 0xc0) != 0);
    *puVar7 = *(ushort *)(param_4 + 2);
    *puVar3 = *param_4;
    puVar3[4] = param_4[1];
  }
  puVar9 = _DAT_8c60e78c;
  if (in_FPSCR_SZ == '\0') {
    puVar3[3] = param_1;
  }
  else {
    *(ulonglong *)(puVar3 + 3) = CONCAT44(param_1,param_2);
  }
  puVar10 = _DAT_8c60e790;
  iVar8 = _DAT_8c60e788;
  puVar3[5] = 0;
  puVar3[6] = 0;
  *puVar9 = *(undefined4 *)(iVar8 + (uint)*puVar5 * 4);
  if ((*puVar7 & 1) == 0) {
    *puVar10 = PTR_LAB_8c60e734;
  }
  else {
    *puVar10 = _DAT_8c60e794;
  }
  if ((*puVar7 & 2) == 0) {
    if ((*param_3 & 0x20) == 0) {
      *DAT_8c60e738 = PTR_LAB_8c60e748;
    }
    else {
      *DAT_8c60e738 = PTR_LAB_8c60e744;
    }
  }
  else if ((*param_3 & 0x20) == 0) {
    *DAT_8c60e738 = PTR_LAB_8c60e740;
  }
  else {
    *DAT_8c60e738 = PTR_LAB_8c60e73c;
  }
  if ((*puVar7 & 4) == 0) {
    *DAT_8c60e74c = PTR_LAB_8c60e754;
  }
  else {
    *DAT_8c60e74c = PTR_LAB_8c60e750;
  }
  puVar3 = DAT_8c60e764;
  puVar2 = PTR_PTR_8c60e760;
  uVar1 = *puVar6;
  *DAT_8c60e75c = *(undefined4 *)(PTR_PTR_8c60e758 + (uint)uVar1 * 4);
  puVar4 = PTR_FUN_8c60e768;
  *puVar3 = *(undefined4 *)(puVar2 + (uint)uVar1 * 4);
  (*(code *)puVar4)();
  FUN_8c60e31c(param_3);
                    /* WARNING: Could not recover jumptable at 0x8c60e6cc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)PTR_FUN_8c60e76c)();
  return;
}

