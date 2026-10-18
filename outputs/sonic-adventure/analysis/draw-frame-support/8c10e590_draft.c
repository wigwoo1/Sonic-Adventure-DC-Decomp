/* Ghidra draft only; not buildable/verified source. */

double FUN_8c10e590(void)

{
  undefined8 uVar1;
  float fVar3;
  double dVar2;
  float extraout_fr3;
  undefined4 extraout_fr2;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  uVar1 = (*(code *)PTR_FUN_8c10e5a4)();
  if (in_FPSCR_SZ == '\0') {
    fVar3 = (float)((ulonglong)uVar1 >> 0x20);
    dVar2 = (double)CONCAT44(*(undefined4 *)PTR_DAT_8c10e5a8,(int)uVar1);
  }
  else {
    dVar2 = *(double *)PTR_DAT_8c10e5a8;
    fVar3 = extraout_fr3;
  }
  if (in_FPSCR_PR == '\0') {
    dVar2 = (double)CONCAT44((float)((ulonglong)dVar2 >> 0x20) - fVar3,SUB84(dVar2,0));
  }
  else {
    dVar2 = dVar2 - (double)CONCAT44(extraout_fr2,fVar3);
  }
  return dVar2;
}

