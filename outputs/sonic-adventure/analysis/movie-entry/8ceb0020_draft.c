/* Ghidra draft only; not buildable/verified source. */

int FUN_8ceb0020(int param_1)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  int *piVar5;
  undefined *puVar6;
  undefined *puVar7;
  undefined *puVar8;
  undefined *puVar9;
  undefined *puVar10;
  undefined4 uVar11;
  undefined *puVar12;
  undefined4 uVar13;
  int iVar14;
  int iVar15;
  undefined8 *puVar16;
  undefined8 in_dr12;
  uint uVar17;
  undefined8 in_dr14;
  undefined8 in_xd14;
  undefined4 in_PR;
  char in_FPSCR_SZ;
  undefined1 local_3c [16];
  undefined8 uStack_2c;
  undefined8 uStack_24;
  ulonglong uVar18;
  
  puVar1 = DAT_8ceb0180;
  if (in_FPSCR_SZ == '\0') {
    uStack_24 = CONCAT44((int)in_dr14,(undefined4)uStack_24);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_24._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    puVar16 = &uStack_2c;
    uStack_2c = CONCAT44((int)in_dr12,(int)((ulonglong)in_dr12 >> 0x20));
  }
  else {
    puVar16 = (undefined8 *)local_3c;
    uStack_24 = in_xd14;
    uStack_2c = in_dr14;
  }
  *(undefined4 *)((int)puVar16 + -4) = in_PR;
  *(undefined4 *)((int)puVar16 + -0x10) = 0;
  iVar15 = 1;
  if (param_1 == 7) {
    *DAT_8ceb0180 = 0;
  }
  else {
    *DAT_8ceb0180 = 5;
  }
  piVar5 = DAT_8ceb0194;
  puVar4 = DAT_8ceb018c;
  puVar3 = DAT_8ceb0188;
  puVar2 = DAT_8ceb0184;
  if (*DAT_8ceb0190 == 1) {
    *DAT_8ceb0184 = 1;
    *puVar4 = 1;
    *puVar3 = 2;
  }
  else {
    *DAT_8ceb0188 = 1;
    if (*piVar5 == 0) {
      *puVar4 = 0x34;
      *puVar2 = 0x38;
    }
    else {
      *puVar4 = 0x3a;
      *puVar2 = 0x3a;
    }
  }
  (*DAT_8ceb019c)(PTR_callback_candidate_8ceb06f4_8ceb0198);
  uVar17 = (uint)in_dr14;
  (*DAT_8ceb01a0)();
  (*DAT_8ceb01a4)(0);
  (*DAT_8ceb01a8)(1);
  (*DAT_8ceb01ac)();
  (*DAT_8ceb01b0)();
  if (param_1 == 7) {
    (*(code *)PTR_callback_candidate_8ceb0a2c_8ceb01b4)();
  }
  (*DAT_8ceb01bc)(PTR_DAT_8ceb01b8,0,0x20);
  puVar6 = PTR_DAT_8ceb01b8;
  *(undefined4 *)PTR_DAT_8ceb01b8 = *puVar4;
  *(undefined4 *)(puVar6 + 4) = *puVar1;
  *(undefined4 *)(puVar6 + 8) = *puVar3;
  puVar7 = PTR_callback_candidate_8cecfce4_8ceb01c0;
  *(undefined4 *)(puVar6 + 0xc) = 2;
  (*(code *)puVar7)();
  puVar9 = PTR_DAT_8ceb01e4;
  puVar8 = PTR_DAT_8ceb01e0;
  puVar7 = PTR_DAT_8ceb01dc;
  puVar6 = PTR_DAT_8ceb01d8;
  *(undefined4 *)((int)puVar16 + -0x14) = *(undefined4 *)(PTR_PTR_8ceb01c4 + param_1 * 4);
  *(undefined4 *)((int)puVar16 + -0xc) = DAT_8ceb01c8;
  *(uint *)((int)puVar16 + -8) = (uint)(param_1 == 0);
  if (in_FPSCR_SZ == '\0') {
    uVar17 = DAT_8ceb01d4;
  }
  uVar18 = (ulonglong)uVar17;
  do {
    iVar14 = *(int *)((int)puVar16 + -0x10);
    *(int *)((int)puVar16 + -0x10) = iVar14 + 1;
    iVar14 = (int)*(char *)(*(int *)((int)puVar16 + -0x14) + iVar14);
    *(int *)puVar7 = iVar14;
    if (iVar14 == -1) break;
    if (*(int *)puVar7 == -2) {
      *(undefined4 *)((int)puVar16 + -0x10) = 1;
      *(int *)puVar7 = (int)**(char **)((int)puVar16 + -0x14);
    }
    (*DAT_8ceb01bc)(PTR_DAT_8ceb01e4,0,0x20);
    *(undefined4 *)puVar9 = 1;
    uVar11 = DAT_8ceb01e8;
    *(undefined4 *)(puVar9 + 0x1c) = 0;
    *(undefined4 *)(puVar9 + 4) = uVar11;
    *(undefined4 *)(puVar9 + 8) = 0x140;
    *(undefined4 *)(puVar9 + 0xc) = 0x1e0;
    *(undefined4 *)(puVar9 + 0x10) = 6;
    *(undefined4 *)((int)puVar16 + -0x18) = 6;
    uVar13 = (*(code *)PTR_callback_candidate_8ced02ba_8ceb01ec)
                       (*(undefined4 *)puVar9,*(undefined4 *)(puVar9 + 4),
                        *(undefined4 *)(puVar9 + 8),*(undefined4 *)(puVar9 + 0xc));
    puVar12 = PTR_callback_candidate_8cecf0f8_8ceb01f4;
    uVar11 = DAT_8ceb01f0;
    puVar10 = PTR_DAT_8ceb01e4;
    *(undefined4 *)(puVar9 + 0x18) = uVar13;
    *(undefined4 *)(puVar9 + 0x14) = uVar11;
    iVar14 = (*(code *)puVar12)(puVar10);
    *(int *)puVar8 = iVar14;
    if (iVar14 == 0) {
      iVar15 = 0;
      break;
    }
    if ((*(int *)((int)puVar16 + -8) != 0) || (param_1 == 7)) {
      (*(code *)PTR_FUN_8ceb0320)(*(undefined4 *)puVar8,1);
    }
    if ((*DAT_8ceb0324 != 1) && (*DAT_8ceb0328 == 1)) {
      (*(code *)PTR_callback_candidate_8cecffd6_8ceb032c)(uVar18,*(undefined4 *)puVar8);
      (*(code *)PTR_callback_candidate_8cecfffc_8ceb0330)(*(undefined4 *)puVar8);
    }
    FUN_8ceb03f2();
    (**(code **)(**(int **)puVar8 + 0x18))
              (*(int **)puVar8,*(undefined4 *)(PTR_PTR_8ceb0334 + *(int *)puVar7 * 0xc));
    *(undefined4 *)puVar6 = 0xffffffff;
    while( true ) {
      (*DAT_8ceb0338)();
      (*(code *)PTR_callback_candidate_8ced55da_8ceb033c)();
      piVar5 = DAT_8ceb0340;
      *(int *)puVar6 = *(int *)puVar6 + 1;
      if (*piVar5 != 0) break;
      (*DAT_8ceb0344)();
      iVar14 = (*DAT_8ceb0348)(0);
      if (iVar14 != 0) {
        if (param_1 != 0) {
          iVar15 = -1;
          goto LAB_8ceb0380;
        }
        (*DAT_8ceb034c)();
        (*DAT_8ceb0350)();
      }
      if ((param_1 == 0) && ((*(uint *)(**(int **)((int)puVar16 + -0xc) + 0x10) & 8) != 0)) {
        iVar14 = *(int *)PTR_DAT_8ceb0354;
        iVar15 = 0;
        if (iVar14 == 0) {
          *(undefined4 *)PTR_DAT_8ceb0358 = 1;
        }
        else {
          if (iVar14 == 3) goto LAB_8ceb0380;
          if (iVar14 == 1) {
            *(undefined4 *)PTR_DAT_8ceb0354 = 4;
          }
        }
      }
      iVar14 = FUN_8ceb0430(param_1);
      if (iVar14 != 0) goto LAB_8ceb0380;
      iVar14 = (**(code **)(**(int **)puVar8 + 0x20))(*(int **)puVar8);
      *(int *)PTR_DAT_8ceb035c = iVar14;
      if ((iVar14 == 3) || (iVar14 == 4)) goto LAB_8ceb0380;
      if (param_1 == 7) {
        (*(code *)PTR_callback_candidate_8ceb0774_8ceb0360)(*(undefined4 *)puVar6);
      }
      (*(code *)PTR_callback_candidate_8ced5464_8ceb0364)();
    }
    iVar15 = 0;
    *DAT_8ceb0340 = 0;
LAB_8ceb0380:
    (**(code **)(**(int **)puVar8 + 0x1c))(*(int **)puVar8);
    (*DAT_8ceb0500)();
    (**(code **)(**(int **)puVar8 + 0x14))(*(int **)puVar8);
  } while (0 < iVar15);
  (*(code *)PTR_callback_candidate_8cecfe2c_8ceb0504)();
  if (param_1 == 7) {
    (*(code *)PTR_callback_candidate_8ceb0b2c_8ceb0508)();
  }
  (*DAT_8ceb0500)();
  (*DAT_8ceb050c)(1);
  (*DAT_8ceb0500)();
  if (-1 < iVar15) {
    if (param_1 == 0) {
      iVar15 = 1;
    }
    else {
      iVar15 = 0;
    }
  }
  return iVar15;
}

