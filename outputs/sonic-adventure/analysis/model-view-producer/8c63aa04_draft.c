/* Ghidra draft only; not buildable/verified source. */

double FUN_8c63aa04(double param_1,undefined4 param_2)

{
  uint in_fr1;
  double dVar1;
  float fVar2;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  fVar2 = (float)((ulonglong)param_1 >> 0x20);
  switch(param_2) {
  case 0:
    return (double)CONCAT44(0x3f800000,in_fr1);
  case 1:
    if (in_FPSCR_SZ == '\0') {
      param_1 = (double)CONCAT44(fVar2,in_fr1);
    }
    if (in_FPSCR_PR == '\0') {
      param_1 = (double)((ulonglong)param_1 & 0xffffffff);
    }
    else {
      param_1 = SQRT(param_1);
    }
    return param_1;
  case 3:
    dVar1 = param_1;
    if (in_FPSCR_SZ == '\0') {
      dVar1 = (double)CONCAT44(fVar2,in_fr1);
    }
    if (in_FPSCR_PR == '\0') {
      dVar1 = (double)((ulonglong)dVar1 & 0xffffffff);
    }
    else {
      dVar1 = dVar1 * SQRT(param_1);
    }
    return dVar1;
  case 0xf:
    if (in_FPSCR_PR == '\0') {
      param_1 = (double)CONCAT44(fVar2 * fVar2,SUB84(param_1,0));
    }
    else {
      param_1 = param_1 * param_1;
    }
  case 0xd:
    if (in_FPSCR_PR == '\0') {
      fVar2 = (float)((ulonglong)param_1 >> 0x20);
      param_1 = (double)CONCAT44(fVar2 * fVar2,SUB84(param_1,0));
    }
    else {
      param_1 = param_1 * param_1;
    }
  case 0xb:
    if (in_FPSCR_PR == '\0') {
      fVar2 = (float)((ulonglong)param_1 >> 0x20);
      param_1 = (double)CONCAT44(fVar2 * fVar2,SUB84(param_1,0));
    }
    else {
      param_1 = param_1 * param_1;
    }
  case 9:
    if (in_FPSCR_PR == '\0') {
      fVar2 = (float)((ulonglong)param_1 >> 0x20);
      param_1 = (double)CONCAT44(fVar2 * fVar2,SUB84(param_1,0));
    }
    else {
      param_1 = param_1 * param_1;
    }
  case 7:
    if (in_FPSCR_PR == '\0') {
      fVar2 = (float)((ulonglong)param_1 >> 0x20);
      param_1 = (double)CONCAT44(fVar2 * fVar2,SUB84(param_1,0));
    }
    else {
      param_1 = param_1 * param_1;
    }
  case 5:
    dVar1 = param_1;
    if (in_FPSCR_SZ == '\0') {
      dVar1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),in_fr1);
    }
    if (in_FPSCR_PR == '\0') {
      param_1 = (double)((ulonglong)dVar1 & 0xffffffff);
    }
    else {
      param_1 = dVar1 * param_1 * param_1;
    }
    return param_1;
  default:
    if (in_FPSCR_PR == '\0') {
      param_1 = (double)CONCAT44(fVar2 * fVar2,SUB84(param_1,0));
    }
    else {
      param_1 = param_1 * param_1;
    }
  case 0xe:
    if (in_FPSCR_PR == '\0') {
      fVar2 = (float)((ulonglong)param_1 >> 0x20);
      param_1 = (double)CONCAT44(fVar2 * fVar2,SUB84(param_1,0));
    }
    else {
      param_1 = param_1 * param_1;
    }
  case 0xc:
    if (in_FPSCR_PR == '\0') {
      fVar2 = (float)((ulonglong)param_1 >> 0x20);
      param_1 = (double)CONCAT44(fVar2 * fVar2,SUB84(param_1,0));
    }
    else {
      param_1 = param_1 * param_1;
    }
  case 10:
    if (in_FPSCR_PR == '\0') {
      fVar2 = (float)((ulonglong)param_1 >> 0x20);
      param_1 = (double)CONCAT44(fVar2 * fVar2,SUB84(param_1,0));
    }
    else {
      param_1 = param_1 * param_1;
    }
  case 8:
    if (in_FPSCR_PR == '\0') {
      fVar2 = (float)((ulonglong)param_1 >> 0x20);
      param_1 = (double)CONCAT44(fVar2 * fVar2,SUB84(param_1,0));
    }
    else {
      param_1 = param_1 * param_1;
    }
  case 6:
    if (in_FPSCR_PR == '\0') {
      fVar2 = (float)((ulonglong)param_1 >> 0x20);
      param_1 = (double)CONCAT44(fVar2 * fVar2,SUB84(param_1,0));
    }
    else {
      param_1 = param_1 * param_1;
    }
  case 4:
    if (in_FPSCR_PR == '\0') {
      param_1 = (double)((ulonglong)param_1 & 0xffffffff);
    }
    else {
      param_1 = param_1 * param_1;
    }
  case 2:
    if (in_FPSCR_SZ == '\0') {
      param_1 = (double)(ulonglong)in_fr1;
    }
    return param_1;
  }
}

