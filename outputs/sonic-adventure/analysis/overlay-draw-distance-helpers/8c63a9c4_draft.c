/* Ghidra draft only; not buildable/verified source. */

double FUN_8c63a9c4(float param_1,uint param_2)

{
  uint in_fr1;
  double dVar1;
  bool bVar2;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  if (in_FPSCR_PR == '\0') {
    bVar2 = param_1 < 0.0;
  }
  else {
    bVar2 = (double)CONCAT44(param_1,param_2) < (double)(ulonglong)in_fr1;
  }
  if (in_FPSCR_SZ == '\0') {
    param_2 = in_fr1;
  }
  if (bVar2) {
    if (in_FPSCR_PR == '\0') {
      dVar1 = (double)CONCAT44(-SQRT(ABS(param_1)),param_2);
    }
    else {
      dVar1 = -SQRT(ABS((double)CONCAT44(param_1,param_2)));
    }
    return dVar1;
  }
  if (in_FPSCR_PR == '\0') {
    dVar1 = (double)CONCAT44(SQRT(param_1),param_2);
  }
  else {
    dVar1 = SQRT((double)CONCAT44(param_1,param_2));
  }
  return dVar1;
}

