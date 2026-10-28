/* Ghidra draft only; not buildable/verified source. */

int FUN_8c055dac(undefined4 param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  undefined4 uVar1;
  float fVar2;
  double dVar4;
  uint uVar6;
  bool bVar7;
  int iVar8;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_14 [16];
  undefined8 uVar3;
  float fVar5;
  
  uVar1 = (*(code *)PTR_FUN_8c055e28)(param_2,param_3);
  if (in_FPSCR_SZ == '\0') {
    uVar1 = param_1;
  }
  uVar3 = (*(code *)PTR_FUN_8c055e2c)(uVar1);
  fVar5 = (float)((ulonglong)uVar3 >> 0x20);
  fVar2 = (float)uVar3;
  if (in_FPSCR_SZ == '\0') {
    fVar2 = DAT_8c055e30;
  }
  dVar4 = (double)CONCAT44(fVar5,fVar2);
  if (in_FPSCR_PR == '\0') {
    fVar5 = fVar5 * fVar2;
    dVar4 = (double)CONCAT44(fVar5,fVar2);
    iVar8 = (int)fVar5;
  }
  else {
    dVar4 = dVar4 * dVar4;
    iVar8 = (int)dVar4;
  }
  uVar6 = (uint)((ulonglong)dVar4 >> 0x20);
  (*(code *)PTR_FUN_8c055e34)(param_2,param_3,auStack_14);
  if (param_4 != 0) {
    dVar4 = (double)(*(code *)PTR_FUN_8c055e28)(param_4,auStack_14);
    if (in_FPSCR_PR == '\0') {
      bVar7 = (float)((ulonglong)dVar4 >> 0x20) < 0.0;
    }
    else {
      bVar7 = dVar4 < (double)((ulonglong)uVar6 << 0x20);
    }
    if (bVar7) {
      iVar8 = DAT_8c055e24 - iVar8;
    }
  }
  return iVar8;
}

