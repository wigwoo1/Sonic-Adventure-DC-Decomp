/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Restarted to delay deadcode elimination for space: register */

void FUN_8c638ecc(undefined8 *param_1,undefined8 *param_2,undefined8 *param_3)

{
  float fVar1;
  float fVar2;
  undefined8 uVar3;
  float fVar4;
  float fVar5;
  undefined4 uVar6;
  undefined8 uVar7;
  undefined4 uVar8;
  undefined8 in_xd0;
  float in_xf2;
  float in_xf3;
  undefined8 in_xd4;
  undefined8 in_xd6;
  undefined8 in_xd8;
  undefined8 in_xd10;
  float in_xf12;
  float in_xf13;
  float in_xf14;
  float in_xf15;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  if (in_FPSCR_SZ == '\0') {
    uVar3 = CONCAT44(*(undefined4 *)param_2,*(undefined4 *)((int)param_2 + 4));
    fVar5 = *(float *)(param_2 + 1);
  }
  else {
    uVar3 = *param_2;
    in_xd0 = param_2[1];
    fVar5 = (float)((ulonglong)param_2[2] >> 0x20);
  }
  uVar7 = CONCAT44(fVar5,0x3f800000);
  fVar1 = (float)uVar3;
  fVar4 = (float)((ulonglong)uVar3 >> 0x20);
  if (param_1 != (undefined8 *)0x0) {
    if (in_FPSCR_SZ == '\0') {
      uVar3 = CONCAT44(*(undefined4 *)param_1,*(undefined4 *)(param_1 + 2));
      uVar7 = CONCAT44(*(undefined4 *)(param_1 + 4),*(undefined4 *)(param_1 + 6));
    }
    else {
      uVar3 = *param_1;
      uVar7 = param_1[4];
      in_xd6 = param_1[6];
    }
                    /* WARNING: Ignoring partial resolution of indirect */
    if (in_FPSCR_PR == '\0') {
      fVar5 = (float)uVar3 * fVar1 + (float)((ulonglong)uVar3 >> 0x20) * fVar4 + (float)uVar7 * 1.0
              + (float)((ulonglong)uVar7 >> 0x20) * fVar5;
    }
    if (in_FPSCR_SZ == '\0') {
      uVar8 = *(undefined4 *)((int)param_1 + 0x34);
      *(int *)param_3 = (int)uVar7;
      uVar6 = *(undefined4 *)(param_1 + 7);
      *(undefined4 *)((int)param_3 + 4) = uVar8;
    }
    else {
      uVar3 = *(undefined8 *)((int)param_1 + 0x34);
      *param_3 = in_xd6;
      uVar6 = (undefined4)param_1[5];
      in_xd6 = param_1[7];
      *(undefined8 *)((int)param_3 + 4) = uVar3;
    }
                    /* WARNING: Ignoring partial resolution of indirect */
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_3 + 1) = uVar6;
    }
    else {
      param_3[1] = in_xd6;
    }
    return;
  }
                    /* WARNING: Ignoring partial resolution of indirect */
  if (in_FPSCR_PR == '\0') {
    fVar2 = (float)in_xd0 * fVar1 + (float)in_xd4 * fVar4 + (float)in_xd8 * 1.0 + in_xf12 * fVar5;
    fVar2 = (float)((ulonglong)in_xd0 >> 0x20) * fVar1 + (float)((ulonglong)in_xd4 >> 0x20) * fVar4
            + (float)((ulonglong)in_xd8 >> 0x20) * 1.0 + in_xf13 * fVar5;
    fVar2 = in_xf2 * fVar1 + (float)in_xd6 * fVar4 + (float)in_xd10 * 1.0 + in_xf14 * fVar5;
    fVar5 = in_xf3 * fVar1 + (float)((ulonglong)in_xd6 >> 0x20) * fVar4 +
            (float)((ulonglong)in_xd10 >> 0x20) * 1.0 + in_xf15 * fVar5;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(param_3 + 1) = (int)((ulonglong)uVar7 >> 0x20);
    *(int *)((int)param_3 + 4) = (int)uVar3;
    *(int *)param_3 = (int)((ulonglong)uVar3 >> 0x20);
  }
  else {
    *(undefined8 *)((int)param_3 + 4) = uVar7;
    *(undefined8 *)((int)param_3 + -4) = in_xd0;
    *(undefined8 *)((int)param_3 + -0xc) = uVar3;
  }
  return;
}

