/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Restarted to delay deadcode elimination for space: register */

void FUN_8c638f28(undefined8 *param_1,double *param_2,undefined4 *param_3)

{
  undefined4 in_fr1;
  float fVar1;
  double dVar2;
  float fVar3;
  double dVar4;
  float fVar5;
  float fVar6;
  undefined8 uVar7;
  ulonglong uVar8;
  ulonglong uVar9;
  double in_xd0;
  float in_xf2;
  float in_xf3;
  undefined8 in_xd4;
  float in_xf6;
  float in_xf7;
  undefined8 in_xd8;
  float in_xf10;
  float in_xf11;
  float in_xf12;
  float in_xf13;
  float in_xf14;
  float in_xf15;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  if (in_FPSCR_SZ == '\0') {
    dVar2 = (double)CONCAT44(*(undefined4 *)param_2,in_fr1);
    param_2 = (double *)((int)param_2 + 4);
  }
  else {
    dVar2 = *param_2;
    param_2 = param_2 + 1;
  }
  if (in_FPSCR_SZ == '\0') {
    dVar2 = (double)CONCAT44((int)((ulonglong)dVar2 >> 0x20),*(undefined4 *)param_2);
    dVar4 = (double)((ulonglong)*(uint *)((int)param_2 + 4) << 0x20);
  }
  else {
    in_xd0 = *param_2;
    dVar4 = param_2[1];
  }
  uVar8 = (ulonglong)dVar4 & 0xffffffff00000000;
  fVar5 = SUB84(dVar2,0);
  fVar3 = (float)((ulonglong)dVar2 >> 0x20);
  fVar6 = (float)(uVar8 >> 0x20);
  if (param_1 == (undefined8 *)0x0) {
                    /* WARNING: Ignoring partial resolution of indirect */
    if (in_FPSCR_PR == '\0') {
      fVar1 = SUB84(in_xd0,0) * fVar5 + (float)in_xd4 * fVar3 + (float)in_xd8 * 0.0 +
              in_xf12 * fVar6;
      fVar1 = (float)((ulonglong)in_xd0 >> 0x20) * fVar5 +
              (float)((ulonglong)in_xd4 >> 0x20) * fVar3 + (float)((ulonglong)in_xd8 >> 0x20) * 0.0
              + in_xf13 * fVar6;
      fVar1 = in_xf2 * fVar5 + in_xf6 * fVar3 + in_xf10 * 0.0 + in_xf14 * fVar6;
      fVar5 = in_xf3 * fVar5 + in_xf7 * fVar3 + in_xf11 * 0.0 + in_xf15 * fVar6;
    }
  }
  else {
    if (in_FPSCR_SZ == '\0') {
      uVar7 = CONCAT44(*(undefined4 *)param_1,*(undefined4 *)(param_1 + 2));
      uVar8 = (ulonglong)*(uint *)(param_1 + 4) << 0x20;
    }
    else {
      uVar7 = *param_1;
      uVar8 = param_1[4];
    }
    uVar8 = uVar8 & 0xffffffff00000000;
                    /* WARNING: Ignoring partial resolution of indirect */
    if (in_FPSCR_PR == '\0') {
      fVar1 = (float)uVar7 * fVar5 + (float)((ulonglong)uVar7 >> 0x20) * fVar3 + 0.0 +
              (float)(uVar8 >> 0x20) * fVar6;
    }
    if (in_FPSCR_SZ == '\0') {
      uVar7 = CONCAT44(*(undefined4 *)((int)param_1 + 4),*(undefined4 *)((int)param_1 + 0x14));
      uVar9 = (ulonglong)*(uint *)((int)param_1 + 0x24) << 0x20;
    }
    else {
      uVar7 = *(undefined8 *)((int)param_1 + 4);
      uVar9 = *(ulonglong *)((int)param_1 + 0x24);
    }
    uVar9 = uVar9 & 0xffffffff00000000;
                    /* WARNING: Ignoring partial resolution of indirect */
    if (in_FPSCR_PR == '\0') {
      fVar1 = (float)uVar7 * fVar5 + (float)((ulonglong)uVar7 >> 0x20) * fVar3 + 0.0 +
              (float)(uVar9 >> 0x20) * fVar6;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar2 = (double)CONCAT44((int)uVar8,(int)uVar9);
      uVar7 = CONCAT44(*(undefined4 *)(param_1 + 1),*(undefined4 *)(param_1 + 3));
      uVar8 = CONCAT44(*(undefined4 *)(param_1 + 5),(int)uVar8);
    }
    else {
      dVar2 = (double)CONCAT44(in_xf7,in_xf6);
      uVar7 = param_1[1];
      uVar8 = param_1[5];
    }
    uVar8 = uVar8 & 0xffffffff00000000;
                    /* WARNING: Ignoring partial resolution of indirect */
    if (in_FPSCR_PR == '\0') {
      fVar5 = (float)uVar7 * fVar5 + (float)((ulonglong)uVar7 >> 0x20) * fVar3 + 0.0 +
              (float)(uVar8 >> 0x20) * fVar6;
    }
    if (in_FPSCR_SZ == '\0') {
      uVar8 = uVar8 << 0x20;
    }
    else {
      in_xd0 = (double)CONCAT44(in_xf11,in_xf10);
      uVar8 = (ulonglong)(uint)in_xf7 << 0x20;
    }
  }
  dVar4 = (double)(uVar8 & 0xffffffff00000000);
  if (*DAT_8c638f8c != 0) {
                    /* WARNING: Ignoring partial resolution of indirect */
    fVar5 = SUB84(dVar2,0);
    fVar3 = (float)((ulonglong)dVar2 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar6 = (float)((ulonglong)dVar4 >> 0x20);
      fVar6 = fVar5 * fVar5 + fVar3 * fVar3 + 0.0 + fVar6 * fVar6;
      fVar6 = 1.0 / SQRT(SUB84(dVar4,0));
      dVar4 = (double)CONCAT44((float)((ulonglong)dVar4 >> 0x20) * fVar6,fVar6);
      dVar2 = (double)CONCAT44(fVar3 * fVar6,fVar5 * fVar6);
    }
    else {
      dVar4 = dVar4 * dVar4;
      dVar2 = dVar2 * dVar4 * dVar4;
    }
  }
  if (in_FPSCR_SZ == '\0') {
    param_3[2] = (int)((ulonglong)dVar4 >> 0x20);
    param_3[1] = SUB84(dVar2,0);
    *param_3 = (int)((ulonglong)dVar2 >> 0x20);
  }
  else {
    *(double *)(param_3 + 1) = dVar4;
    *(double *)(param_3 + -1) = in_xd0;
    *(double *)(param_3 + -3) = dVar2;
  }
  return;
}

