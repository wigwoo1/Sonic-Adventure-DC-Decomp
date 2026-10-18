/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c639f02) */
/* WARNING: Removing unreachable block (ram,0x8c639ede) */
/* WARNING: Restarted to delay deadcode elimination for space: register */

float FUN_8c639ec8(int param_1,ushort param_2)

{
  float fVar1;
  longlong lVar2;
  float in_fr1;
  float in_fr0;
  float fVar3;
  float fVar4;
  float fVar5;
  longlong lVar6;
  float fVar7;
  undefined8 uVar9;
  float fVar10;
  float fVar11;
  undefined8 uVar12;
  float fVar13;
  float in_xf0;
  float in_xf1;
  float in_xf2;
  float in_xf3;
  longlong in_xd4;
  float fVar14;
  undefined8 in_xd6;
  float fVar15;
  undefined8 in_xd8;
  float fVar16;
  undefined8 in_xd10;
  float fVar17;
  undefined8 in_xd12;
  undefined8 in_xd14;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  float fVar8;
  
  fVar10 = (float)((ulonglong)in_xd12 >> 0x20);
  fVar3 = (float)((ulonglong)in_xd14 >> 0x20);
  if (param_1 != 0) {
    if (in_FPSCR_PR == '\0') {
      fVar4 = ((float)param_2 * 6.2831855) / 65536.0;
      fVar1 = (float)sin(fVar4);
      fVar4 = (float)cos(fVar4);
      in_fr0 = (float)fVar1;
      in_fr1 = (float)fVar4;
    }
    if (in_FPSCR_SZ == '\0') {
      lVar6 = (ulonglong)(uint)in_fr0 << 0x20;
    }
    else {
      lVar6 = CONCAT44(in_fr0,in_fr1);
    }
    fVar4 = 0.0;
    if (in_FPSCR_SZ == '\0') {
      uVar9 = CONCAT44(*(undefined4 *)(param_1 + 0x10),*(undefined4 *)(param_1 + 0x14));
      uVar12 = CONCAT44(*(undefined4 *)(param_1 + 0x18),*(undefined4 *)(param_1 + 0x1c));
      param_1 = param_1 + 0x30;
    }
    else {
      uVar9 = *(undefined8 *)(param_1 + 0x10);
      in_xd4 = *(longlong *)(param_1 + 0x18);
      uVar12 = *(undefined8 *)(param_1 + 0x20);
      in_xd6 = *(undefined8 *)(param_1 + 0x28);
      in_xd8 = *(undefined8 *)(param_1 + 0x38);
      in_xd10 = *(undefined8 *)(param_1 + 0x48);
      param_1 = param_1 + 0x50;
    }
    fVar14 = (float)((ulonglong)in_xd4 >> 0x20);
    fVar15 = (float)((ulonglong)in_xd6 >> 0x20);
    fVar16 = (float)((ulonglong)in_xd8 >> 0x20);
    fVar17 = (float)((ulonglong)in_xd10 >> 0x20);
    fVar1 = in_fr1;
    if (in_FPSCR_PR == '\0') {
                    /* WARNING: Ignoring partial resolution of indirect */
      fVar7 = (float)lVar6;
      fVar8 = (float)((ulonglong)lVar6 >> 0x20);
      fVar1 = in_xf0 * in_fr1 + (float)in_xd4 * 0.0 + (float)in_xd8 * fVar7 + (float)in_xd12 * fVar8
      ;
      fVar4 = in_xf1 * in_fr1 + fVar14 * 0.0 + fVar16 * fVar7 + fVar10 * fVar8;
      fVar5 = in_xf2 * in_fr1 + (float)in_xd6 * 0.0 + (float)in_xd10 * fVar7 +
              (float)in_xd14 * fVar8;
      fVar7 = in_xf3 * in_fr1 + fVar15 * 0.0 + fVar17 * fVar7 + fVar3 * fVar8;
      fVar5 = (float)uVar9;
                    /* WARNING: Ignoring partial resolution of indirect */
      fVar8 = (float)((ulonglong)uVar9 >> 0x20);
      fVar11 = (float)uVar12;
      fVar13 = (float)((ulonglong)uVar12 >> 0x20);
      fVar7 = in_xf0 * fVar5 + (float)in_xd4 * fVar8 + (float)in_xd8 * fVar11 +
              (float)in_xd12 * fVar13;
      fVar10 = in_xf1 * fVar5 + fVar14 * fVar8 + fVar16 * fVar11 + fVar10 * fVar13;
      fVar10 = in_xf2 * fVar5 + (float)in_xd6 * fVar8 + (float)in_xd10 * fVar11 +
               (float)in_xd14 * fVar13;
      fVar10 = in_xf3 * fVar5 + fVar15 * fVar8 + fVar17 * fVar11 + fVar3 * fVar13;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_1 + -4) = (int)uVar12;
      *(int *)(param_1 + -8) = (int)((ulonglong)uVar12 >> 0x20);
      *(int *)(param_1 + -0xc) = (int)uVar9;
      *(int *)(param_1 + -0x10) = (int)((ulonglong)uVar9 >> 0x20);
      *(int *)(param_1 + -0x14) = (int)lVar6;
      *(int *)(param_1 + -0x18) = (int)((ulonglong)lVar6 >> 0x20);
      *(float *)(param_1 + -0x1c) = fVar1;
      *(float *)(param_1 + -0x20) = fVar4;
    }
    else {
      *(undefined8 *)(param_1 + -8) = in_xd6;
      *(undefined8 *)(param_1 + -0x10) = uVar12;
      *(longlong *)(param_1 + -0x18) = in_xd4;
      *(undefined8 *)(param_1 + -0x20) = uVar9;
      *(ulonglong *)(param_1 + -0x28) = CONCAT44(in_xf3,in_xf2);
      *(longlong *)(param_1 + -0x30) = lVar6;
      *(ulonglong *)(param_1 + -0x38) = CONCAT44(in_xf1,in_xf0);
      *(ulonglong *)(param_1 + -0x40) = CONCAT44(fVar4,fVar1);
    }
    return fVar4;
  }
  if (in_FPSCR_PR == '\0') {
    fVar3 = ((float)param_2 * 6.2831855) / 65536.0;
    fVar4 = (float)sin(fVar3);
    fVar3 = (float)cos(fVar3);
    in_fr0 = (float)fVar4;
    in_fr1 = (float)fVar3;
  }
  if (in_FPSCR_SZ == '\0') {
    lVar6 = (ulonglong)(uint)in_fr0 << 0x20;
  }
  else {
    lVar6 = CONCAT44(in_fr0,in_fr1);
  }
  fVar3 = 0.0;
  lVar2 = lVar6;
  if (in_FPSCR_SZ == '\0') {
    lVar2 = in_xd4;
  }
  if (in_FPSCR_PR == '\0') {
    fVar3 = in_xf1 * in_fr1 + (float)((ulonglong)lVar2 >> 0x20) * 0.0 +
            (float)((ulonglong)in_xd8 >> 0x20) * (float)lVar6 +
            fVar10 * (float)((ulonglong)lVar6 >> 0x20);
  }
  return fVar3;
}

