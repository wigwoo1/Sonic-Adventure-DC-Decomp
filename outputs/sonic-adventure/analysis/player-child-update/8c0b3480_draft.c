/* Ghidra draft only; not buildable/verified source. */

void FUN_8c0b3480(uint param_1,int param_2,double *param_3,double *param_4,ulonglong *param_5)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  double *pdVar4;
  undefined4 unaff_r13;
  undefined4 unaff_r14;
  float fVar5;
  double in_dr2;
  double dVar6;
  double in_xd2;
  undefined4 in_PR;
  char cVar7;
  int iVar8;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(param_2 + 0x34));
  }
  else {
    in_xd2 = *(double *)(param_2 + 0x34);
  }
  if (in_FPSCR_PR == '\0') {
    iVar8 = (int)SUB84(in_dr2,0);
  }
  else {
    iVar8 = (int)in_dr2;
  }
  iVar8 = (*(code *)PTR_FUN_8c0b3514)(iVar8);
  iVar1 = *(int *)((uint)*(byte *)(param_2 + 3) * 0x48 + *(int *)(param_2 + 8) + 0x18);
  uVar2 = SUB84(in_dr2,0);
  if (iVar1 == 0) {
    uVar3 = *(undefined4 *)(iVar8 + 0x1c8);
    *(undefined4 *)param_4 = *(undefined4 *)(iVar8 + 0x1c4);
    *(undefined4 *)((int)param_4 + 4) = uVar3;
    *(undefined4 *)(param_4 + 1) = *(undefined4 *)(iVar8 + 0x1cc);
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44(*(undefined4 *)param_4,uVar2);
    }
    else {
      dVar6 = *param_4;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),
                               *(undefined4 *)
                                ((uint)*(byte *)(param_2 + 3) * 0x48 + *(int *)(param_2 + 8) + 0x14)
                              );
    }
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) * SUB84(dVar6,0),SUB84(dVar6,0));
    }
    else {
      dVar6 = dVar6 * dVar6;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)param_4 = (int)((ulonglong)dVar6 >> 0x20);
    }
    else {
      *param_4 = dVar6;
    }
    pdVar4 = (double *)((int)param_4 + 4);
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44(*(undefined4 *)pdVar4,SUB84(dVar6,0));
    }
    else {
      dVar6 = *pdVar4;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),
                               *(undefined4 *)
                                ((uint)*(byte *)(param_2 + 3) * 0x48 + *(int *)(param_2 + 8) + 0x14)
                              );
    }
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) * SUB84(dVar6,0),SUB84(dVar6,0));
    }
    else {
      dVar6 = dVar6 * dVar6;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)pdVar4 = (int)((ulonglong)dVar6 >> 0x20);
    }
    else {
      *pdVar4 = dVar6;
    }
    pdVar4 = param_4 + 1;
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44(*(undefined4 *)pdVar4,SUB84(dVar6,0));
    }
    else {
      dVar6 = *pdVar4;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),
                               *(undefined4 *)
                                ((uint)*(byte *)(param_2 + 3) * 0x48 + *(int *)(param_2 + 8) + 0x14)
                              );
    }
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) * SUB84(dVar6,0),SUB84(dVar6,0));
    }
    else {
      dVar6 = dVar6 * dVar6;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)pdVar4 = (int)((ulonglong)dVar6 >> 0x20);
    }
    else {
      *pdVar4 = dVar6;
    }
    iVar1 = *(int *)(param_2 + 8) + (uint)*(byte *)(param_2 + 3) * 0x48;
    if (in_FPSCR_SZ == '\0') {
      fVar5 = *(float *)(iVar8 + 0x1c4);
      dVar6 = (double)CONCAT44(*(undefined4 *)(iVar1 + 4),*(undefined4 *)(iVar1 + 0x10));
    }
    else {
      fVar5 = (float)((ulonglong)*(undefined8 *)(iVar8 + 0x1c4) >> 0x20);
      dVar6 = *(double *)(iVar1 + 4);
    }
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44(fVar5 * SUB84(dVar6,0) + (float)((ulonglong)dVar6 >> 0x20),
                               SUB84(dVar6,0));
    }
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),*(undefined4 *)(iVar8 + 0x17c));
    }
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) + SUB84(dVar6,0),SUB84(dVar6,0));
    }
    else {
      dVar6 = dVar6 + dVar6;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)param_3 = (int)((ulonglong)dVar6 >> 0x20);
    }
    else {
      *param_3 = dVar6;
    }
    iVar1 = *(int *)(param_2 + 8) + (uint)*(byte *)(param_2 + 3) * 0x48;
    if (in_FPSCR_SZ == '\0') {
      fVar5 = *(float *)(iVar8 + 0x1c8);
      dVar6 = (double)CONCAT44(*(undefined4 *)(iVar1 + 8),*(undefined4 *)(iVar1 + 0x10));
    }
    else {
      fVar5 = (float)((ulonglong)*(undefined8 *)(iVar8 + 0x1c8) >> 0x20);
      dVar6 = *(double *)(iVar1 + 8);
    }
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44(fVar5 * SUB84(dVar6,0) + (float)((ulonglong)dVar6 >> 0x20),
                               SUB84(dVar6,0));
    }
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),*(undefined4 *)(iVar8 + 0x180));
    }
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) + SUB84(dVar6,0),SUB84(dVar6,0));
    }
    else {
      dVar6 = dVar6 + dVar6;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)param_3 + 4) = (int)((ulonglong)dVar6 >> 0x20);
    }
    else {
      *(double *)((int)param_3 + 4) = dVar6;
    }
    iVar1 = *(int *)(param_2 + 8) + (uint)*(byte *)(param_2 + 3) * 0x48;
    if (in_FPSCR_SZ == '\0') {
      fVar5 = *(float *)(iVar8 + 0x1cc);
      dVar6 = (double)CONCAT44(*(undefined4 *)(iVar1 + 0xc),*(undefined4 *)(iVar1 + 0x10));
    }
    else {
      fVar5 = (float)((ulonglong)*(undefined8 *)(iVar8 + 0x1cc) >> 0x20);
      dVar6 = *(double *)(iVar1 + 0xc);
    }
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44(fVar5 * SUB84(dVar6,0) + (float)((ulonglong)dVar6 >> 0x20),
                               SUB84(dVar6,0));
    }
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),*(undefined4 *)(iVar8 + 0x184));
    }
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) + SUB84(dVar6,0),SUB84(dVar6,0));
    }
    else {
      dVar6 = dVar6 + dVar6;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_3 + 1) = (int)((ulonglong)dVar6 >> 0x20);
    }
    else {
      param_3[1] = dVar6;
    }
    if (param_5 != (ulonglong *)0x0) {
      uVar2 = *(undefined4 *)(iVar8 + 0x180);
      *(undefined4 *)param_5 = *(undefined4 *)(iVar8 + 0x17c);
      *(undefined4 *)((int)param_5 + 4) = uVar2;
      *(undefined4 *)(param_5 + 1) = *(undefined4 *)(iVar8 + 0x184);
    }
  }
  else if (iVar1 == 1) {
    uVar3 = *(undefined4 *)(iVar8 + 0x1d4);
    *(undefined4 *)param_4 = *(undefined4 *)(iVar8 + 0x1d0);
    *(undefined4 *)((int)param_4 + 4) = uVar3;
    *(undefined4 *)(param_4 + 1) = *(undefined4 *)(iVar8 + 0x1d8);
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44(*(undefined4 *)param_4,uVar2);
    }
    else {
      dVar6 = *param_4;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),
                               *(undefined4 *)
                                ((uint)*(byte *)(param_2 + 3) * 0x48 + *(int *)(param_2 + 8) + 0x14)
                              );
    }
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) * SUB84(dVar6,0),SUB84(dVar6,0));
    }
    else {
      dVar6 = dVar6 * dVar6;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)param_4 = (int)((ulonglong)dVar6 >> 0x20);
    }
    else {
      *param_4 = dVar6;
    }
    pdVar4 = (double *)((int)param_4 + 4);
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44(*(undefined4 *)pdVar4,SUB84(dVar6,0));
    }
    else {
      dVar6 = *pdVar4;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),
                               *(undefined4 *)
                                ((uint)*(byte *)(param_2 + 3) * 0x48 + *(int *)(param_2 + 8) + 0x14)
                              );
    }
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) * SUB84(dVar6,0),SUB84(dVar6,0));
    }
    else {
      dVar6 = dVar6 * dVar6;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)pdVar4 = (int)((ulonglong)dVar6 >> 0x20);
    }
    else {
      *pdVar4 = dVar6;
    }
    pdVar4 = param_4 + 1;
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44(*(undefined4 *)pdVar4,SUB84(dVar6,0));
    }
    else {
      dVar6 = *pdVar4;
    }
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),
                               *(undefined4 *)
                                ((uint)*(byte *)(param_2 + 3) * 0x48 + *(int *)(param_2 + 8) + 0x14)
                              );
    }
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) * SUB84(dVar6,0),SUB84(dVar6,0));
    }
    else {
      dVar6 = dVar6 * dVar6;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)pdVar4 = (int)((ulonglong)dVar6 >> 0x20);
    }
    else {
      *pdVar4 = dVar6;
    }
    iVar1 = *(int *)(param_2 + 8) + (uint)*(byte *)(param_2 + 3) * 0x48;
    if (in_FPSCR_SZ == '\0') {
      fVar5 = *(float *)(iVar8 + 0x1d0);
      dVar6 = (double)CONCAT44(*(undefined4 *)(iVar1 + 4),*(undefined4 *)(iVar1 + 0x10));
    }
    else {
      fVar5 = (float)((ulonglong)*(undefined8 *)(iVar8 + 0x1d0) >> 0x20);
      dVar6 = *(double *)(iVar1 + 4);
    }
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44(fVar5 * SUB84(dVar6,0) + (float)((ulonglong)dVar6 >> 0x20),
                               SUB84(dVar6,0));
    }
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),*(undefined4 *)(iVar8 + 0x188));
    }
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) + SUB84(dVar6,0),SUB84(dVar6,0));
    }
    else {
      dVar6 = dVar6 + dVar6;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)param_3 = (int)((ulonglong)dVar6 >> 0x20);
    }
    else {
      *param_3 = dVar6;
    }
    iVar1 = *(int *)(param_2 + 8) + (uint)*(byte *)(param_2 + 3) * 0x48;
    if (in_FPSCR_SZ == '\0') {
      fVar5 = *(float *)(iVar8 + 0x1d4);
      dVar6 = (double)CONCAT44(*(undefined4 *)(iVar1 + 8),*(undefined4 *)(iVar1 + 0x10));
    }
    else {
      fVar5 = (float)((ulonglong)*(undefined8 *)(iVar8 + 0x1d4) >> 0x20);
      dVar6 = *(double *)(iVar1 + 8);
    }
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44(fVar5 * SUB84(dVar6,0) + (float)((ulonglong)dVar6 >> 0x20),
                               SUB84(dVar6,0));
    }
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),*(undefined4 *)(iVar8 + 0x18c));
    }
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) + SUB84(dVar6,0),SUB84(dVar6,0));
    }
    else {
      dVar6 = dVar6 + dVar6;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)param_3 + 4) = (int)((ulonglong)dVar6 >> 0x20);
    }
    else {
      *(double *)((int)param_3 + 4) = dVar6;
    }
    iVar1 = *(int *)(param_2 + 8) + (uint)*(byte *)(param_2 + 3) * 0x48;
    if (in_FPSCR_SZ == '\0') {
      fVar5 = *(float *)(iVar8 + 0x1d8);
      dVar6 = (double)CONCAT44(*(undefined4 *)(iVar1 + 0xc),*(undefined4 *)(iVar1 + 0x10));
    }
    else {
      fVar5 = (float)((ulonglong)*(undefined8 *)(iVar8 + 0x1d8) >> 0x20);
      dVar6 = *(double *)(iVar1 + 0xc);
    }
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44(fVar5 * SUB84(dVar6,0) + (float)((ulonglong)dVar6 >> 0x20),
                               SUB84(dVar6,0));
    }
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),*(undefined4 *)(iVar8 + 400));
    }
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) + SUB84(dVar6,0),SUB84(dVar6,0));
    }
    else {
      dVar6 = dVar6 + dVar6;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_3 + 1) = (int)((ulonglong)dVar6 >> 0x20);
    }
    else {
      param_3[1] = dVar6;
    }
    if (param_5 != (ulonglong *)0x0) {
      uVar2 = *(undefined4 *)(iVar8 + 0x18c);
      *(undefined4 *)param_5 = *(undefined4 *)(iVar8 + 0x188);
      *(undefined4 *)((int)param_5 + 4) = uVar2;
      *(undefined4 *)(param_5 + 1) = *(undefined4 *)(iVar8 + 400);
    }
  }
  else {
    iVar1 = (uint)*(byte *)(param_2 + 3) * 0x48 + *(int *)(param_2 + 8);
    uVar3 = *(undefined4 *)(iVar1 + 8);
    *(undefined4 *)param_3 = *(undefined4 *)(iVar1 + 4);
    *(undefined4 *)((int)param_3 + 4) = uVar3;
    *(undefined4 *)(param_3 + 1) = *(undefined4 *)(iVar1 + 0xc);
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44(*(undefined4 *)param_3,uVar2);
    }
    else {
      dVar6 = *param_3;
    }
    iVar1 = (uint)*(byte *)(param_2 + 3) * 0x48 + *(int *)(param_2 + 8);
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),*(undefined4 *)(iVar1 + 0x10));
    }
    else {
      in_xd2 = *(double *)(iVar1 + 0x10);
    }
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) + SUB84(dVar6,0),SUB84(dVar6,0));
    }
    else {
      dVar6 = dVar6 + dVar6;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)param_3 = (int)((ulonglong)dVar6 >> 0x20);
      dVar6 = (double)CONCAT44(*(undefined4 *)((int)param_3 + 4),*(undefined4 *)(iVar8 + 0x118));
    }
    else {
      *param_3 = dVar6;
      dVar6 = *(double *)((int)param_3 + 4);
      in_xd2 = *(double *)(iVar8 + 0x118);
    }
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) + SUB84(dVar6,0),SUB84(dVar6,0));
    }
    else {
      dVar6 = dVar6 + dVar6;
    }
    uVar2 = (undefined4)((ulonglong)dVar6 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)((int)param_3 + 4) = uVar2;
    }
    else {
      *(double *)((int)param_3 + 4) = dVar6;
    }
    iVar8 = (uint)*(byte *)(param_2 + 3) * 0x48 + *(int *)(param_2 + 8);
    if (in_FPSCR_SZ == '\0') {
      dVar6 = (double)CONCAT44(uVar2,*(undefined4 *)(iVar8 + 0x14));
    }
    else {
      in_xd2 = *(double *)(iVar8 + 0x14);
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)param_4 = SUB84(dVar6,0);
      *(undefined4 *)((int)param_4 + 4) = 0;
      *(undefined4 *)(param_4 + 1) = 0;
    }
    else {
      *param_4 = in_xd2;
      *(ulonglong *)((int)param_4 + 4) = (ulonglong)param_1;
      param_4[1] = (double)(ulonglong)param_1;
    }
    if (param_5 != (ulonglong *)0x0) {
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)param_5 = 0;
        dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),*(undefined4 *)((int)param_3 + 4));
        *(undefined4 *)((int)param_5 + 4) = *(undefined4 *)((int)param_3 + 4);
        *(undefined4 *)(param_5 + 1) = 0;
      }
      else {
        *param_5 = (ulonglong)param_1;
        *(undefined8 *)((int)param_5 + 4) = *(undefined8 *)((int)param_3 + 4);
        param_5[1] = (ulonglong)param_1;
      }
    }
  }
  if (in_FPSCR_SZ == '\0') {
    dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),*(undefined4 *)(param_2 + 0x34));
  }
  if (in_FPSCR_PR == '\0') {
    cVar7 = (char)(int)SUB84(dVar6,0);
  }
  else {
    cVar7 = (char)(int)dVar6;
  }
  iVar1 = *(int *)(DAT_8c0b34e8 + cVar7 * 4);
  iVar8 = *(int *)(*(int *)(iVar1 + 0x38) + 0xc);
  (*(code *)PTR_FUN_8c0b34f0)(PTR_DAT_8c0b34ec,in_PR,unaff_r13,unaff_r14);
  (*(code *)PTR_FUN_8c0b34f4)(0,iVar8 + 8);
  if (*(int *)(iVar1 + 0x1c) != 0) {
    (*(code *)PTR_FUN_8c0b34f8)(0,*(int *)(iVar1 + 0x1c));
  }
  if (*(int *)(iVar1 + 0x14) != 0) {
    (*(code *)PTR_FUN_8c0b34fc)(0,*(int *)(iVar1 + 0x14));
  }
  iVar8 = DAT_8c0b3500 - *(int *)(iVar1 + 0x18);
  if (iVar8 != 0) {
    (*(code *)PTR_FUN_8c0b3504)(0,iVar8,iVar8);
  }
  (*(code *)PTR_FUN_8c0b3508)(0,param_3);
  (*(code *)PTR_FUN_8c0b350c)(0,param_4);
  if (param_5 != (ulonglong *)0x0) {
    (*(code *)PTR_FUN_8c0b3508)(0,param_5,param_5);
  }
  (*(code *)PTR_FUN_8c0b3510)(1);
  return;
}

