/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Restarted to delay deadcode elimination for space: register */

undefined8 FUN_8c639090(undefined8 *param_1,undefined8 *param_2)

{
  float fVar1;
  undefined8 uVar2;
  ulonglong uVar3;
  float fVar4;
  undefined8 uVar5;
  longlong lVar6;
  undefined8 in_xd2;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  if (in_FPSCR_SZ == '\0') {
    fVar1 = *(float *)((int)param_1 + 4);
    uVar2 = CONCAT44(*(undefined4 *)param_1,fVar1);
    uVar5 = CONCAT44(*(undefined4 *)param_2,*(undefined4 *)((int)param_2 + 4));
    uVar3 = (ulonglong)*(uint *)(param_1 + 1) << 0x20;
    lVar6 = (ulonglong)*(uint *)(param_2 + 1) << 0x20;
  }
  else {
    uVar2 = *param_1;
    uVar5 = *param_2;
    fVar1 = (float)uVar2;
    uVar3 = param_1[1];
    lVar6 = param_2[1];
  }
  uVar3 = uVar3 & 0xffffffff00000000;
                    /* WARNING: Ignoring partial resolution of indirect */
  if (in_FPSCR_PR == '\0') {
    fVar4 = fVar1 * (float)uVar5 +
            (float)((ulonglong)uVar2 >> 0x20) * (float)((ulonglong)uVar5 >> 0x20) +
            (float)lVar6 * 0.0 + (float)(uVar3 >> 0x20) * (float)((ulonglong)lVar6 >> 0x20);
  }
  if (in_FPSCR_SZ == '\0') {
    in_xd2 = CONCAT44((int)uVar3,fVar1);
  }
  return in_xd2;
}

