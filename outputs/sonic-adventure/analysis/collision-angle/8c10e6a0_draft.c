/* Ghidra draft only; not buildable/verified source. */

double FUN_8c10e6a0(double param_1)

{
  float fVar1;
  double dVar2;
  float in_fr3;
  undefined4 extraout_fr3;
  undefined4 uVar4;
  undefined4 in_fr2;
  undefined4 extraout_fr2;
  float fVar6;
  double dVar5;
  undefined8 in_xd4;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  float local_c;
  float fStack_8;
  float fVar3;
  
  fVar3 = SUB84(param_1,0);
  fVar6 = (float)((ulonglong)param_1 >> 0x20);
  fVar1 = fVar3;
  local_c = fVar3;
  fStack_8 = fVar6;
  if (in_FPSCR_SZ != '\0') {
    local_c = (float)in_xd4;
    fStack_8 = (float)((ulonglong)in_xd4 >> 0x20);
    param_1 = (double)CONCAT44(fVar6,fStack_8);
    fVar3 = in_fr3;
    fVar1 = fStack_8;
  }
  if (in_FPSCR_PR == '\0') {
    param_1 = (double)CONCAT44((float)((ulonglong)param_1 >> 0x20) / fVar3,fVar1);
  }
  else {
    param_1 = param_1 / (double)CONCAT44(in_fr2,fVar3);
  }
  dVar2 = (double)(*(code *)PTR_FUN_8c10e6e8)(param_1);
  dVar5 = dVar2;
  if (in_FPSCR_SZ == '\0') {
    dVar5 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),SUB84(param_1,0));
  }
  if (((uint)local_c & DAT_8c10e6ec) != 0) {
    uVar4 = (undefined4)((ulonglong)dVar5 >> 0x20);
    if ((DAT_8c10e6ec & (uint)fStack_8) == 0) {
      if (in_FPSCR_SZ == '\0') {
        dVar5 = (double)CONCAT44(*(undefined4 *)PTR_DAT_8c10e6f4,SUB84(dVar5,0));
      }
      else {
        dVar5 = *(double *)PTR_DAT_8c10e6f4;
        uVar4 = extraout_fr3;
      }
    }
    else if (in_FPSCR_SZ == '\0') {
      dVar5 = (double)CONCAT44(*(undefined4 *)PTR_DAT_8c10e6f0,SUB84(dVar5,0));
    }
    else {
      dVar5 = *(double *)PTR_DAT_8c10e6f0;
      uVar4 = extraout_fr3;
    }
    if (in_FPSCR_PR == '\0') {
      dVar5 = (double)((ulonglong)dVar5 & 0xffffffff);
    }
    else {
      dVar5 = dVar5 + (double)CONCAT44(extraout_fr2,uVar4);
    }
  }
  if (in_FPSCR_SZ == '\0') {
    dVar5 = (double)((ulonglong)dVar2 & 0xffffffff);
  }
  return dVar5;
}

