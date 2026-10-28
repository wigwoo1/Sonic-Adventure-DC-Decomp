/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Restarted to delay deadcode elimination for space: register */

undefined8 FUN_8c63a75c(undefined8 *param_1)

{
  undefined4 in_fr1;
  undefined8 uVar1;
  double dVar2;
  float fVar3;
  undefined8 in_xd6;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  float fVar4;
  
  if (in_FPSCR_SZ == '\0') {
    uVar1 = CONCAT44(*(undefined4 *)param_1,*(undefined4 *)((int)param_1 + 4));
    dVar2 = (double)((ulonglong)*(uint *)(param_1 + 1) << 0x20);
  }
  else {
    uVar1 = *param_1;
    dVar2 = (double)param_1[2];
  }
                    /* WARNING: Ignoring partial resolution of indirect */
  if (in_FPSCR_PR == '\0') {
    fVar3 = (float)((ulonglong)uVar1 >> 0x20);
    fVar4 = (float)((ulonglong)dVar2 >> 0x20);
    fVar3 = (float)uVar1 * (float)uVar1 + fVar3 * fVar3 + SUB84(dVar2,0) * SUB84(dVar2,0) +
            fVar4 * fVar4;
    fVar3 = SQRT(SUB84(dVar2,0));
  }
  else {
    fVar3 = SUB84(SQRT(dVar2),0);
  }
  if (in_FPSCR_SZ == '\0') {
    in_xd6 = CONCAT44(fVar3,in_fr1);
  }
  return in_xd6;
}

