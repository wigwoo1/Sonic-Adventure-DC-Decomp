/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c0750a0) */

void FUN_8c07506e(int param_1,int param_2,int param_3,double *param_4)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int *piVar5;
  float fVar6;
  double dVar7;
  double dVar8;
  undefined8 in_xd0;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  piVar5 = *(int **)(*(int *)(param_2 + 0x6c) + 0x28);
  if (piVar5 != (int *)0x0) {
    if (*(char *)(param_1 + 8) != '\0') {
      piVar5 = piVar5 + 7;
    }
    dVar7 = (double)(*(code *)PTR_FUN_8c0751a0)(param_2 + 0x38);
    fVar6 = (float)((ulonglong)dVar7 >> 0x20);
    if (fVar6 == 0.0) {
      if (in_FPSCR_SZ == '\0') {
        dVar8 = (double)((ulonglong)*(uint *)param_4 << 0x20);
        dVar7 = (double)CONCAT44(fVar6,*(undefined4 *)(param_1 + 0x20));
      }
      else {
        dVar8 = *param_4;
        in_xd0 = *(undefined8 *)(param_1 + 0x20);
      }
      if (in_FPSCR_PR == '\0') {
        dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),
                                 SUB84(dVar7,0) + (float)((ulonglong)dVar8 >> 0x20));
      }
      else {
        dVar7 = dVar7 + dVar8;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_3 + 4) = SUB84(dVar7,0);
        dVar8 = (double)CONCAT44(*(undefined4 *)((int)param_4 + 4),SUB84(dVar8,0));
        dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),*(undefined4 *)(param_1 + 0x24));
      }
      else {
        *(undefined8 *)(param_3 + 4) = in_xd0;
        dVar8 = *(double *)((int)param_4 + 4);
        in_xd0 = *(undefined8 *)(param_1 + 0x24);
      }
      if (in_FPSCR_PR == '\0') {
        dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),
                                 SUB84(dVar7,0) + (float)((ulonglong)dVar8 >> 0x20));
      }
      else {
        dVar7 = dVar7 + dVar8;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_3 + 8) = SUB84(dVar7,0);
        dVar8 = (double)CONCAT44(*(undefined4 *)(param_4 + 1),SUB84(dVar8,0));
        dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),*(undefined4 *)(param_1 + 0x28));
      }
      else {
        *(undefined8 *)(param_3 + 8) = in_xd0;
        dVar8 = param_4[1];
        in_xd0 = *(undefined8 *)(param_1 + 0x28);
      }
      if (in_FPSCR_PR == '\0') {
        fVar6 = SUB84(dVar7,0) + (float)((ulonglong)dVar8 >> 0x20);
      }
      else {
        fVar6 = SUB84(dVar7 + dVar8,0);
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)(param_3 + 0xc) = fVar6;
      }
      else {
        *(undefined8 *)(param_3 + 0xc) = in_xd0;
      }
    }
    if (*piVar5 != 0) {
      uVar2 = *(undefined4 *)(param_1 + 0x20);
      uVar1 = *(undefined4 *)(param_1 + 0x24);
      uVar3 = *(undefined4 *)(param_1 + 0x28);
      uVar4 = *(undefined4 *)(param_3 + 8);
      *(undefined4 *)(param_1 + 0x20) = *(undefined4 *)(param_3 + 4);
      *(undefined4 *)(param_1 + 0x24) = uVar4;
      *(undefined4 *)(param_1 + 0x28) = *(undefined4 *)(param_3 + 0xc);
      (*(code *)*piVar5)(*(undefined4 *)(DAT_8c0751a4 + (uint)*(byte *)(param_1 + 8) * 4),param_1,
                         piVar5);
      *(undefined4 *)(param_1 + 0x20) = uVar2;
      *(undefined4 *)(param_1 + 0x24) = uVar1;
      *(undefined4 *)(param_1 + 0x28) = uVar3;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar7 = (double)CONCAT44(*(undefined4 *)(param_3 + 4),piVar5[4]);
    }
    else {
      dVar7 = *(double *)(param_3 + 4);
    }
    if (in_FPSCR_PR == '\0') {
      dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) + SUB84(dVar7,0),SUB84(dVar7,0));
    }
    else {
      dVar7 = dVar7 + dVar7;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_3 + 4) = (int)((ulonglong)dVar7 >> 0x20);
      dVar7 = (double)CONCAT44(*(undefined4 *)(param_3 + 8),piVar5[5]);
    }
    else {
      *(double *)(param_3 + 4) = dVar7;
      dVar7 = *(double *)(param_3 + 8);
    }
    if (in_FPSCR_PR == '\0') {
      dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) + SUB84(dVar7,0),SUB84(dVar7,0));
    }
    else {
      dVar7 = dVar7 + dVar7;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_3 + 8) = (int)((ulonglong)dVar7 >> 0x20);
      dVar7 = (double)CONCAT44(*(undefined4 *)(param_3 + 0xc),piVar5[6]);
    }
    else {
      *(double *)(param_3 + 8) = dVar7;
      dVar7 = *(double *)(param_3 + 0xc);
    }
    if (in_FPSCR_PR == '\0') {
      dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) + SUB84(dVar7,0),SUB84(dVar7,0));
    }
    else {
      dVar7 = dVar7 + dVar7;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_3 + 0xc) = (int)((ulonglong)dVar7 >> 0x20);
    }
    else {
      *(double *)(param_3 + 0xc) = dVar7;
    }
    *(uint *)(param_1 + 0x14) = *(int *)(param_1 + 0x14) + piVar5[1] & 0xffff;
    *(uint *)(param_1 + 0x18) = *(int *)(param_1 + 0x18) + piVar5[2] & 0xffff;
    *(uint *)(param_1 + 0x1c) = *(int *)(param_1 + 0x1c) + piVar5[3] & 0xffff;
  }
  return;
}

