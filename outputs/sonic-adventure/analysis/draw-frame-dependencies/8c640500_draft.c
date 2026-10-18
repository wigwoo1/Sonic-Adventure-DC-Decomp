/* Ghidra draft only; not buildable/verified source. */

void FUN_8c640500(int param_1)

{
  double dVar1;
  float fVar2;
  double dVar3;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  dVar1 = (double)(*(code *)PTR_FUN_8c64051c)(param_1 >> 1);
  fVar2 = (float)((ulonglong)dVar1 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    dVar3 = (double)CONCAT44(*(undefined4 *)DAT_8c640520,fVar2);
  }
  else {
    dVar3 = *DAT_8c640520;
  }
  if (in_FPSCR_PR == '\0') {
    dVar3 = (double)CONCAT44((float)((ulonglong)dVar3 >> 0x20) / (fVar2 + SUB84(dVar3,0)),
                             SUB84(dVar3,0));
  }
  else {
    dVar3 = dVar3 / (dVar1 + dVar3);
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)DAT_8c640520 + -4) = (int)((ulonglong)dVar3 >> 0x20);
  }
  else {
    *(double *)((int)DAT_8c640520 + -4) = dVar3;
  }
  return;
}

