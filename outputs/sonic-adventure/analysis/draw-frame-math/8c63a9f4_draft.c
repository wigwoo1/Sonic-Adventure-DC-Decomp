/* Ghidra draft only; not buildable/verified source. */

double FUN_8c63a9f4(ushort param_1)

{
  float fVar1;
  float fVar2;
  double in_dr0;
  double dVar3;
  char in_FPSCR_PR;
  
  if (in_FPSCR_PR == '\0') {
    fVar1 = ((float)param_1 * 6.2831855) / 65536.0;
    fVar2 = (float)sin(fVar1);
    fVar1 = (float)cos(fVar1);
    dVar3 = (double)CONCAT44((float)fVar2 / (float)fVar1,(float)fVar1);
  }
  else {
    dVar3 = in_dr0 / in_dr0;
  }
  return dVar3;
}

