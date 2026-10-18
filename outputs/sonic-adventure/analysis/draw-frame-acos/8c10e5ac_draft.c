/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c10e618) */
/* WARNING: Removing unreachable block (ram,0x8c10e5e6) */
/* WARNING: Removing unreachable block (ram,0x8c10e5c4) */
/* WARNING: Removing unreachable block (ram,0x8c10e5e2) */
/* WARNING: Removing unreachable block (ram,0x8c10e606) */
/* WARNING: Removing unreachable block (ram,0x8c10e626) */
/* WARNING: Removing unreachable block (ram,0x8c10e5ee) */
/* WARNING: Removing unreachable block (ram,0x8c10e5f4) */

double FUN_8c10e5ac(double param_1)

{
  undefined1 *puVar1;
  undefined4 uVar2;
  double in_dr0;
  float fVar3;
  ulonglong in_dr2;
  double dVar4;
  undefined8 uVar5;
  undefined4 uVar6;
  float fVar7;
  undefined8 in_xd2;
  double in_xd4;
  double in_xd14;
  bool bVar8;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_8 [4];
  undefined1 auStack_4 [4];
  
  if (in_FPSCR_SZ == '\0') {
    puVar1 = auStack_4;
  }
  else {
    puVar1 = local_8;
  }
  *(undefined4 *)(puVar1 + -4) = in_PR;
  if (in_FPSCR_SZ == '\0') {
    fVar7 = (float)((ulonglong)param_1 >> 0x20);
    param_1 = (double)CONCAT44(fVar7,fVar7);
    *(float *)(puVar1 + -8) = fVar7;
  }
  else {
    fVar7 = SUB84(param_1,0);
    *(double *)(puVar1 + -8) = param_1;
    in_xd4 = param_1;
  }
  dVar4 = (double)(in_dr2 & 0xffffffff00000000);
  uVar2 = (undefined4)((ulonglong)param_1 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(puVar1 + -0xc) = uVar2;
  }
  else {
    *(double *)(puVar1 + -0xc) = param_1;
  }
  if ((*(uint *)(puVar1 + -0xc) & DAT_8c10e688) != 0) {
    if (in_FPSCR_SZ == '\0') {
      dVar4 = (double)CONCAT44((int)(in_dr2 >> 0x20),*(undefined4 *)(puVar1 + -0xc));
    }
    else {
      in_xd2 = *(undefined8 *)(puVar1 + -0xc);
    }
    fVar3 = SUB84(dVar4,0);
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),-fVar3);
    if (in_FPSCR_SZ == '\0') {
      *(float *)(puVar1 + -0xc) = -fVar3;
    }
    else {
      *(undefined8 *)(puVar1 + -0xc) = in_xd2;
    }
  }
  if (in_FPSCR_SZ == '\0') {
    dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),*(undefined4 *)(puVar1 + -0xc));
    param_1 = (double)CONCAT44(uVar2,*(undefined4 *)PTR_DAT_8c10e68c);
  }
  else {
    in_xd4 = *(double *)PTR_DAT_8c10e68c;
  }
  if (in_FPSCR_PR == '\0') {
    bVar8 = SUB84(param_1,0) < SUB84(dVar4,0);
  }
  else {
    bVar8 = param_1 < dVar4;
  }
  if (bVar8) {
    param_1 = in_xd4;
    if (in_FPSCR_SZ != '\0') {
      param_1 = *(double *)PTR_DAT_8c10e684;
    }
  }
  else if (SUB84(dVar4,0) == SUB84(param_1,0)) {
    param_1 = in_xd4;
    if (in_FPSCR_SZ != '\0') {
      param_1 = *(double *)PTR_DAT_8c10e690;
    }
  }
  else if (fVar7 == 0.0) {
    if (in_FPSCR_SZ == '\0') {
      param_1 = in_xd4;
    }
  }
  else {
    if (in_FPSCR_SZ == '\0') {
      uVar5 = CONCAT44(*(undefined4 *)PTR_DAT_8c10e694,*(undefined4 *)(puVar1 + -0xc));
    }
    else {
      uVar5 = *(undefined8 *)PTR_DAT_8c10e694;
    }
    if (in_FPSCR_PR != '\0' || (float)((ulonglong)uVar5 >> 0x20) <= (float)uVar5) {
      dVar4 = in_xd4;
      if (in_FPSCR_SZ == '\0') {
        dVar4 = in_xd14;
      }
    }
    else {
      dVar4 = param_1;
      if (in_FPSCR_SZ == '\0') {
        dVar4 = in_xd14;
      }
    }
    if (in_FPSCR_SZ == '\0') {
      dVar4 = (double)((ulonglong)param_1 & 0xffffffff);
    }
    uVar6 = SUB84(dVar4,0);
    uVar2 = (*(code *)PTR_FUN_8c10e698)(dVar4);
    if (in_FPSCR_SZ == '\0') {
      uVar2 = uVar6;
    }
    in_dr0 = (double)(*(code *)PTR_FUN_8c10e69c)(uVar2,1);
    param_1 = in_dr0;
    if (in_FPSCR_SZ == '\0') {
      param_1 = in_xd4;
    }
  }
  if (in_FPSCR_SZ == '\0') {
    param_1 = (double)((ulonglong)in_dr0 & 0xffffffff);
  }
  return param_1;
}

