/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Restarted to delay deadcode elimination for space: register */

undefined8 FUN_8c63a804(undefined4 param_1,undefined8 *param_2,undefined8 *param_3)

{
  undefined8 in_dr0;
  undefined8 uVar1;
  undefined8 uVar2;
  float fVar3;
  float fVar4;
  undefined8 uVar5;
  float fVar6;
  float fVar7;
  undefined8 in_xd2;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  if (in_FPSCR_SZ == '\0') {
    uVar5 = CONCAT44(*(undefined4 *)param_3,param_1);
    param_3 = (undefined8 *)((int)param_3 + 4);
  }
  else {
    uVar5 = *param_3;
    param_3 = param_3 + 1;
  }
  if (in_FPSCR_SZ == '\0') {
    uVar5 = CONCAT44((int)((ulonglong)uVar5 >> 0x20),*(undefined4 *)param_3);
    fVar7 = *(float *)((int)param_3 + 4);
  }
  else {
    fVar7 = (float)((ulonglong)param_3[1] >> 0x20);
  }
  if (param_2 != (undefined8 *)0x0) {
    if (in_FPSCR_SZ == '\0') {
      uVar1 = CONCAT44(*(undefined4 *)param_2,*(undefined4 *)(param_2 + 2));
      uVar2 = CONCAT44(*(undefined4 *)(param_2 + 4),*(undefined4 *)(param_2 + 6));
    }
    else {
      uVar1 = *param_2;
      uVar2 = param_2[4];
      in_xd2 = param_2[6];
    }
                    /* WARNING: Ignoring partial resolution of indirect */
    fVar4 = (float)uVar5;
    fVar6 = (float)((ulonglong)uVar5 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar3 = (float)uVar1 * fVar4 + (float)((ulonglong)uVar1 >> 0x20) * fVar6 + (float)uVar2 * 1.0
              + (float)((ulonglong)uVar2 >> 0x20) * fVar7;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_2 + 6) = (int)uVar2;
      uVar5 = CONCAT44(*(undefined4 *)((int)param_2 + 4),*(undefined4 *)((int)param_2 + 0x14));
      uVar1 = CONCAT44(*(undefined4 *)((int)param_2 + 0x24),*(undefined4 *)((int)param_2 + 0x34));
    }
    else {
      param_2[6] = in_xd2;
      uVar5 = *(undefined8 *)((int)param_2 + 4);
      uVar1 = *(undefined8 *)((int)param_2 + 0x24);
      in_xd2 = *(undefined8 *)((int)param_2 + 0x34);
    }
                    /* WARNING: Ignoring partial resolution of indirect */
    if (in_FPSCR_PR == '\0') {
      fVar3 = (float)uVar5 * fVar4 + (float)((ulonglong)uVar5 >> 0x20) * fVar6 + (float)uVar1 * 1.0
              + (float)((ulonglong)uVar1 >> 0x20) * fVar7;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)param_2 + 0x34) = (int)uVar1;
      uVar5 = CONCAT44(*(undefined4 *)(param_2 + 1),*(undefined4 *)(param_2 + 3));
      uVar1 = CONCAT44(*(undefined4 *)(param_2 + 5),*(undefined4 *)(param_2 + 7));
    }
    else {
      *(undefined8 *)((int)param_2 + 0x34) = in_xd2;
      uVar5 = param_2[1];
      uVar1 = param_2[5];
      in_xd2 = param_2[7];
    }
                    /* WARNING: Ignoring partial resolution of indirect */
    if (in_FPSCR_PR == '\0') {
      fVar3 = (float)uVar5 * fVar4 + (float)((ulonglong)uVar5 >> 0x20) * fVar6 + (float)uVar1 * 1.0
              + (float)((ulonglong)uVar1 >> 0x20) * fVar7;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_2 + 7) = (int)uVar1;
      uVar5 = CONCAT44(*(undefined4 *)((int)param_2 + 0xc),*(undefined4 *)((int)param_2 + 0x1c));
      uVar1 = CONCAT44(*(undefined4 *)((int)param_2 + 0x2c),*(undefined4 *)((int)param_2 + 0x3c));
    }
    else {
      param_2[7] = in_xd2;
      uVar5 = *(undefined8 *)((int)param_2 + 0xc);
      uVar1 = *(undefined8 *)((int)param_2 + 0x2c);
      in_xd2 = *(undefined8 *)((int)param_2 + 0x3c);
    }
                    /* WARNING: Ignoring partial resolution of indirect */
    if (in_FPSCR_PR == '\0') {
      fVar7 = (float)uVar5 * fVar4 + (float)((ulonglong)uVar5 >> 0x20) * fVar6 + (float)uVar1 * 1.0
              + (float)((ulonglong)uVar1 >> 0x20) * fVar7;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)param_2 + 0x3c) = (int)uVar1;
    }
    else {
      *(undefined8 *)((int)param_2 + 0x3c) = in_xd2;
    }
    return uVar5;
  }
  return in_dr0;
}

