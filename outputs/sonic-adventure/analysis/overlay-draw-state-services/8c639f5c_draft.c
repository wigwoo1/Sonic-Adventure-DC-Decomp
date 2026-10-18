/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Restarted to delay deadcode elimination for space: register */

ulonglong FUN_8c639f5c(ulonglong *param_1,ushort param_2)

{
  float fVar1;
  undefined4 uVar2;
  float fVar3;
  ulonglong in_dr0;
  ulonglong uVar5;
  ulonglong uVar6;
  float fVar7;
  undefined4 uVar8;
  float fVar9;
  float fVar11;
  ulonglong uVar10;
  float fVar12;
  ulonglong in_xd0;
  ulonglong in_xd2;
  float fVar13;
  float in_xf4;
  float in_xf5;
  float in_xf6;
  float in_xf7;
  ulonglong in_xd8;
  float fVar14;
  ulonglong in_xd10;
  float fVar15;
  undefined8 in_xd12;
  float fVar16;
  undefined8 in_xd14;
  float fVar17;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  float fVar4;
  
  fVar1 = (float)in_xd12;
  fVar16 = (float)((ulonglong)in_xd12 >> 0x20);
  fVar17 = (float)((ulonglong)in_xd14 >> 0x20);
  if (param_1 != (ulonglong *)0x0) {
    if (in_FPSCR_PR == '\0') {
      fVar11 = ((float)param_2 * 6.2831855) / 65536.0;
      fVar13 = (float)sin(fVar11);
      fVar11 = (float)cos(fVar11);
      in_dr0 = CONCAT44((float)fVar13,(float)fVar11);
    }
    uVar5 = in_xd0;
    if (in_FPSCR_SZ == '\0') {
      uVar2 = (undefined4)in_dr0;
      in_dr0 = in_dr0 & 0xffffffff00000000;
      uVar5 = CONCAT44(uVar2,uVar2);
    }
    uVar5 = uVar5 & 0xffffffff00000000;
    if (in_FPSCR_SZ == '\0') {
      uVar6 = CONCAT44((int)*param_1,*(undefined4 *)((int)param_1 + 4));
      uVar10 = CONCAT44((int)param_1[1],*(undefined4 *)((int)param_1 + 0xc));
      param_1 = param_1 + 2;
    }
    else {
      uVar6 = *param_1;
      in_xd0 = param_1[1];
      uVar10 = param_1[2];
      in_xd2 = param_1[3];
      param_1 = param_1 + 4;
    }
    if (in_FPSCR_SZ == '\0') {
      uVar2 = (undefined4)param_1[2];
      param_1 = param_1 + 4;
    }
    else {
      uVar2 = (undefined4)(param_1[2] >> 0x20);
      in_xd8 = param_1[3];
      in_xd10 = param_1[5];
      param_1 = param_1 + 6;
    }
    fVar11 = (float)(in_xd0 >> 0x20);
    fVar13 = (float)(in_xd2 >> 0x20);
    fVar14 = (float)(in_xd8 >> 0x20);
    fVar15 = (float)(in_xd10 >> 0x20);
    if (in_FPSCR_PR == '\0') {
      fVar4 = (float)uVar6;
                    /* WARNING: Ignoring partial resolution of indirect */
      fVar7 = (float)(uVar6 >> 0x20);
      fVar9 = (float)uVar10;
      fVar12 = (float)(uVar10 >> 0x20);
      fVar3 = (float)in_xd0 * fVar4 + in_xf4 * fVar7 + (float)in_xd8 * fVar9 + fVar1 * fVar12;
      fVar3 = fVar11 * fVar4 + in_xf5 * fVar7 + fVar14 * fVar9 + fVar16 * fVar12;
      fVar3 = (float)in_xd2 * fVar4 + in_xf6 * fVar7 + (float)in_xd10 * fVar9 +
              (float)in_xd14 * fVar12;
      fVar3 = fVar13 * fVar4 + in_xf7 * fVar7 + fVar15 * fVar9 + fVar17 * fVar12;
      fVar3 = (float)in_dr0;
                    /* WARNING: Ignoring partial resolution of indirect */
      fVar4 = (float)(in_dr0 >> 0x20);
      fVar7 = (float)(uVar5 >> 0x20);
      fVar1 = (float)in_xd0 * fVar3 + in_xf4 * fVar4 + (float)in_xd8 * 0.0 + fVar1 * fVar7;
      fVar1 = fVar11 * fVar3 + in_xf5 * fVar4 + fVar14 * 0.0 + fVar16 * fVar7;
      fVar1 = (float)in_xd2 * fVar3 + in_xf6 * fVar4 + (float)in_xd10 * 0.0 + (float)in_xd14 * fVar7
      ;
      fVar1 = fVar13 * fVar3 + in_xf7 * fVar4 + fVar15 * 0.0 + fVar17 * fVar7;
    }
    uVar8 = (undefined4)(uVar6 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)param_1 + -4) = (int)uVar5;
      *(int *)(param_1 + -1) = (int)(uVar5 >> 0x20);
      *(int *)((int)param_1 + -0xc) = (int)in_dr0;
      *(int *)(param_1 + -2) = (int)(in_dr0 >> 0x20);
      *(int *)((int)param_1 + -0x24) = (int)uVar10;
      *(int *)(param_1 + -5) = (int)(uVar10 >> 0x20);
      *(int *)((int)param_1 + -0x2c) = (int)uVar6;
      *(undefined4 *)(param_1 + -6) = uVar8;
      uVar6 = CONCAT44(uVar8,uVar2);
    }
    else {
      param_1[-1] = CONCAT44(in_xf7,in_xf6);
      param_1[-2] = uVar5;
      param_1[-3] = CONCAT44(in_xf5,in_xf4);
      param_1[-4] = in_dr0;
      param_1[-7] = in_xd2;
      param_1[-8] = uVar10;
      param_1[-9] = in_xd0;
      param_1[-10] = uVar6;
    }
    return uVar6;
  }
  if (in_FPSCR_PR == '\0') {
    fVar17 = ((float)param_2 * 6.2831855) / 65536.0;
    fVar11 = (float)sin(fVar17);
    fVar17 = (float)cos(fVar17);
    in_dr0 = CONCAT44((float)fVar11,(float)fVar17);
  }
  uVar5 = in_xd0;
  if (in_FPSCR_SZ == '\0') {
    uVar5 = CONCAT44((int)in_dr0,(int)in_dr0);
    in_dr0 = in_dr0 & 0xffffffff00000000;
  }
  uVar5 = uVar5 & 0xffffffff00000000;
  if (in_FPSCR_PR == '\0') {
    fVar11 = -(float)(in_dr0 >> 0x20);
                    /* WARNING: Ignoring partial resolution of indirect */
    fVar17 = (float)(uVar5 >> 0x20);
    fVar1 = (float)in_xd0 * 0.0 + in_xf4 * fVar17 + (float)in_xd8 * (float)in_dr0 + fVar1 * fVar11;
    fVar1 = (float)(in_xd0 >> 0x20) * 0.0 + in_xf5 * fVar17 +
            (float)(in_xd8 >> 0x20) * (float)in_dr0 + fVar16 * fVar11;
  }
  if (in_FPSCR_SZ == '\0') {
    uVar2 = (undefined4)(uVar5 >> 0x20);
    uVar5 = CONCAT44(uVar2,uVar2);
  }
  return uVar5;
}

