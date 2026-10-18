/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c63fdd6) */
/* WARNING: Removing unreachable block (ram,0x8c63fdf2) */

void FUN_8c63fdc0(int param_1)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  float fVar4;
  float fVar6;
  double dVar5;
  float fVar7;
  undefined8 in_xd4;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  fVar7 = DAT_8c63fe00._4_4_;
  if (in_FPSCR_SZ == '\0') {
    fVar7 = (float)DAT_8c63fe00;
  }
  uVar3 = 0;
  do {
    uVar2 = (int)uVar3 >> 4;
    if ((int)uVar2 < 0) {
      iVar1 = 1 >> (~uVar2 & 0x1f) + 1;
    }
    else {
      iVar1 = 1 << (uVar2 & 0x1f);
    }
    fVar6 = (float)((ulonglong)in_xd4 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      fVar6 = 1.0;
    }
    uVar2 = uVar3 + 1;
    fVar4 = (float)(int)(iVar1 * ((uVar3 & 0xf) + 0x10));
    if (in_FPSCR_PR == '\0') {
      fVar4 = fVar4 / fVar7;
      dVar5 = (double)CONCAT44(fVar6 / fVar4,fVar4);
    }
    else {
      dVar5 = (double)CONCAT44(fVar6,fVar4) / (double)CONCAT44(fVar7,0x3f800000);
      dVar5 = dVar5 / dVar5;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(uVar3 * 4 + param_1) = (int)((ulonglong)dVar5 >> 0x20);
    }
    else {
      *(double *)(uVar3 * 4 + param_1) = dVar5;
    }
    uVar3 = uVar2;
  } while ((int)uVar2 < 0x80);
  return;
}

