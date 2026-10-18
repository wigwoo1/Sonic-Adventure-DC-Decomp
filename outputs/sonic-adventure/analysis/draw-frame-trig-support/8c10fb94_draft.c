/* Ghidra draft only; not buildable/verified source. */

double FUN_8c10fb94(float param_1,float param_2,float param_3)

{
  float fVar1;
  int iVar2;
  double *pdVar3;
  double in_dr0;
  double dVar4;
  float in_fr3;
  double dVar5;
  double in_xd2;
  double in_xd4;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  fVar1 = param_2;
  if (in_FPSCR_SZ == '\0') {
    fVar1 = param_3;
  }
  if (in_FPSCR_PR == '\0') {
    dVar5 = (double)CONCAT44(param_1 * param_1,fVar1);
  }
  else {
    dVar5 = (double)CONCAT44(param_1,fVar1) * (double)CONCAT44(param_1,param_2);
  }
  iVar2 = 5;
  if (in_FPSCR_SZ == '\0') {
    param_2 = *(float *)PTR_DAT_8c10fbb4;
    pdVar3 = (double *)(PTR_DAT_8c10fbb4 + 4);
  }
  else {
    pdVar3 = (double *)(PTR_DAT_8c10fbb4 + 8);
    in_xd4 = *(double *)PTR_DAT_8c10fbb4;
  }
  do {
    if (in_FPSCR_SZ == '\0') {
      in_fr3 = *(float *)pdVar3;
      pdVar3 = (double *)((int)pdVar3 + 4);
      in_dr0 = (double)CONCAT44((int)((ulonglong)dVar5 >> 0x20),SUB84(in_dr0,0));
    }
    else {
      in_xd2 = *pdVar3;
      pdVar3 = pdVar3 + 1;
      in_dr0 = dVar5;
    }
    iVar2 = iVar2 + -1;
    if (in_FPSCR_PR == '\0') {
      in_fr3 = (float)((ulonglong)in_dr0 >> 0x20) * param_2 + in_fr3;
    }
    dVar4 = in_xd2;
    if (in_FPSCR_SZ == '\0') {
      param_2 = in_fr3;
      dVar4 = in_xd4;
    }
    in_xd4 = dVar4;
  } while (0 < iVar2);
  fVar1 = param_2;
  if (in_FPSCR_SZ == '\0') {
    fVar1 = SUB84(in_dr0,0);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar4 = (double)CONCAT44(param_2,SUB84(dVar5,0));
  }
  if (in_FPSCR_PR == '\0') {
    dVar4 = (double)CONCAT44(param_1 * (float)((ulonglong)dVar4 >> 0x20),fVar1);
  }
  else {
    dVar4 = (double)CONCAT44(param_1,fVar1) * dVar4;
  }
  return dVar4;
}

