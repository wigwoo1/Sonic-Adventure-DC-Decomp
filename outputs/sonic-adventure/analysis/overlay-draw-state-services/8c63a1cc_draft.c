/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Restarted to delay deadcode elimination for space: register */

ulonglong FUN_8c63a1cc(ulonglong *param_1,ushort param_2)

{
  float fVar1;
  float fVar2;
  ulonglong in_dr0;
  float fVar5;
  undefined4 uVar6;
  ulonglong uVar4;
  float fVar7;
  float fVar8;
  ulonglong uVar9;
  float fVar10;
  ulonglong uVar11;
  ulonglong uVar12;
  ulonglong in_xd0;
  ulonglong in_xd2;
  float fVar13;
  ulonglong in_xd4;
  float fVar14;
  ulonglong in_xd6;
  float fVar15;
  float in_xf8;
  float in_xf9;
  float in_xf10;
  float in_xf11;
  undefined8 in_xd12;
  float fVar16;
  undefined8 in_xd14;
  float fVar17;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  float fVar3;
  
  fVar1 = (float)in_xd12;
  fVar16 = (float)((ulonglong)in_xd12 >> 0x20);
  fVar17 = (float)((ulonglong)in_xd14 >> 0x20);
  if (param_1 != (ulonglong *)0x0) {
    if (in_FPSCR_PR == '\0') {
      fVar5 = ((float)param_2 * 6.2831855) / 65536.0;
      fVar13 = (float)sin(fVar5);
      cos(fVar5);
      in_dr0 = (ulonglong)(uint)(float)fVar13 << 0x20;
    }
    if (in_FPSCR_SZ == '\0') {
      in_dr0 = in_dr0 & 0xffffffff00000000;
      uVar4 = CONCAT44((int)*param_1,*(undefined4 *)((int)param_1 + 4));
      uVar9 = CONCAT44((int)param_1[1],*(undefined4 *)((int)param_1 + 0xc));
      uVar11 = CONCAT44((int)param_1[2],*(undefined4 *)((int)param_1 + 0x14));
      uVar12 = CONCAT44((int)param_1[3],*(undefined4 *)((int)param_1 + 0x1c));
      param_1 = param_1 + 4;
    }
    else {
      uVar4 = *param_1;
      in_xd0 = param_1[1];
      uVar9 = param_1[2];
      in_xd2 = param_1[3];
      uVar11 = param_1[4];
      in_xd4 = param_1[5];
      uVar12 = param_1[6];
      in_xd6 = param_1[7];
      param_1 = param_1 + 8;
    }
    fVar5 = (float)(in_xd0 >> 0x20);
    fVar13 = (float)(in_xd2 >> 0x20);
    fVar14 = (float)(in_xd4 >> 0x20);
    fVar15 = (float)(in_xd6 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar3 = (float)uVar4;
                    /* WARNING: Ignoring partial resolution of indirect */
      fVar7 = (float)(uVar4 >> 0x20);
      fVar8 = (float)uVar9;
      fVar10 = (float)(uVar9 >> 0x20);
      fVar2 = (float)in_xd0 * fVar3 + (float)in_xd4 * fVar7 + in_xf8 * fVar8 + fVar1 * fVar10;
      fVar2 = fVar5 * fVar3 + fVar14 * fVar7 + in_xf9 * fVar8 + fVar16 * fVar10;
      fVar2 = (float)in_xd2 * fVar3 + (float)in_xd6 * fVar7 + in_xf10 * fVar8 +
              (float)in_xd14 * fVar10;
      fVar2 = fVar13 * fVar3 + fVar15 * fVar7 + in_xf11 * fVar8 + fVar17 * fVar10;
      fVar2 = (float)uVar11;
                    /* WARNING: Ignoring partial resolution of indirect */
      fVar3 = (float)(uVar11 >> 0x20);
      fVar7 = (float)uVar12;
      fVar8 = (float)(uVar12 >> 0x20);
      fVar1 = (float)in_xd0 * fVar2 + (float)in_xd4 * fVar3 + in_xf8 * fVar7 + fVar1 * fVar8;
      fVar1 = fVar5 * fVar2 + fVar14 * fVar3 + in_xf9 * fVar7 + fVar16 * fVar8;
      fVar1 = (float)in_xd2 * fVar2 + (float)in_xd6 * fVar3 + in_xf10 * fVar7 +
              (float)in_xd14 * fVar8;
      fVar1 = fVar13 * fVar2 + fVar15 * fVar3 + in_xf11 * fVar7 + fVar17 * fVar8;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)param_1 + -4) = (int)uVar12;
      *(int *)(param_1 + -1) = (int)(uVar12 >> 0x20);
      *(int *)((int)param_1 + -0xc) = (int)uVar11;
      *(int *)(param_1 + -2) = (int)(uVar11 >> 0x20);
      *(int *)((int)param_1 + -0x14) = (int)uVar9;
      *(int *)(param_1 + -3) = (int)(uVar9 >> 0x20);
      *(int *)((int)param_1 + -0x1c) = (int)uVar4;
      *(int *)(param_1 + -4) = (int)(uVar4 >> 0x20);
      uVar4 = in_dr0 >> 0x20;
    }
    else {
      param_1[-1] = in_xd6;
      param_1[-2] = uVar12;
      param_1[-3] = in_xd4;
      param_1[-4] = uVar11;
      param_1[-5] = in_xd2;
      param_1[-6] = uVar9;
      param_1[-7] = in_xd0;
      param_1[-8] = uVar4;
    }
    return uVar4;
  }
  if (in_FPSCR_PR == '\0') {
    fVar17 = ((float)param_2 * 6.2831855) / 65536.0;
    fVar5 = (float)sin(fVar17);
    fVar17 = (float)cos(fVar17);
    in_dr0 = CONCAT44((float)fVar5,(float)fVar17);
  }
  uVar11 = in_xd0;
  uVar4 = in_xd0;
  uVar9 = in_dr0;
  if (in_FPSCR_SZ == '\0') {
    uVar11 = in_xd4;
    uVar4 = CONCAT44((int)in_dr0,(int)(in_dr0 >> 0x20));
    in_dr0 = in_xd0;
    uVar9 = 0;
  }
  if (in_FPSCR_PR == '\0') {
    fVar17 = (float)uVar4;
                    /* WARNING: Ignoring partial resolution of indirect */
    fVar5 = (float)(uVar4 >> 0x20);
    fVar1 = (float)in_dr0 * fVar17 + (float)uVar11 * fVar5 + in_xf8 * (float)uVar9 + fVar1 * 0.0;
    fVar1 = (float)(in_dr0 >> 0x20) * fVar17 + (float)(uVar11 >> 0x20) * fVar5 +
            in_xf9 * (float)uVar9 + fVar16 * 0.0;
  }
  if (in_FPSCR_SZ == '\0') {
    uVar6 = (undefined4)(uVar4 >> 0x20);
    uVar4 = CONCAT44(uVar6,uVar6);
  }
  return uVar4;
}

