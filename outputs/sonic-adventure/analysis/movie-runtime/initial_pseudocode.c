/* Ghidra-generated draft. Not buildable or behavior-verified. */
/* Address: 8ceb0000 */

int thunk_FUN_8ceb0020(int param_1)

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
  undefined1 auStack_3c [16];
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
    puVar16 = (undefined8 *)auStack_3c;
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


/* Address: 8ceb0020 */

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


/* Address: 8ceb03f2 */

void FUN_8ceb03f2(void)

{
  undefined *puVar1;
  undefined *puVar2;
  
  puVar2 = PTR_DAT_8ceb0518;
  puVar1 = PTR_DAT_8ceb0514;
  *(undefined4 *)PTR_DAT_8ceb0510 = 0;
  *(undefined4 *)puVar1 = 0;
  if (*(int *)(PTR_DAT_8ceb051c + *(int *)puVar2 * 0xc) == 0) {
    *(undefined4 *)PTR_DAT_8ceb0528 = 1;
  }
  else {
    (*(code *)PTR_callback_candidate_8ced0022_8ceb0524)(*(undefined4 *)PTR_DAT_8ceb0520,0);
    *(undefined4 *)PTR_DAT_8ceb0528 = 0;
  }
  return;
}


/* Address: 8ceb0430 */

undefined4 FUN_8ceb0430(int param_1)

{
  undefined *puVar1;
  undefined *puVar2;
  undefined *puVar3;
  undefined *puVar4;
  undefined *puVar5;
  undefined *puVar6;
  undefined *puVar7;
  undefined4 uVar8;
  int iVar9;
  int iVar10;
  undefined4 *puVar11;
  
  puVar3 = PTR_DAT_8ceb0520;
  (**(code **)(**(int **)PTR_DAT_8ceb0520 + 0x24))
            (*(int **)PTR_DAT_8ceb0520,PTR_DAT_8ceb0530,PTR_DAT_8ceb052c,0);
  puVar11 = (undefined4 *)PTR_DAT_8ceb0534;
  uVar8 = (*DAT_8ceb0538)(100);
  puVar2 = PTR_DAT_8ceb0518;
  *puVar11 = uVar8;
  puVar7 = PTR_DAT_8ceb0540;
  puVar6 = PTR_DAT_8ceb053c;
  puVar5 = PTR_DAT_8ceb0528;
  puVar4 = PTR_callback_candidate_8ced0022_8ceb0524;
  puVar1 = PTR_DAT_8ceb0510;
  iVar9 = *(int *)PTR_DAT_8ceb0528;
  iVar10 = *(int *)PTR_DAT_8ceb053c;
  if (iVar9 == 0) {
    iVar9 = (*DAT_8ceb0538)();
    puVar1 = PTR_DAT_8ceb0514;
    if (0xbf < iVar9) {
      *(int *)puVar5 = *(int *)puVar5 + 1;
      iVar9 = 0xc0;
      if (*(int *)puVar1 != 0) {
        *(undefined4 *)puVar5 = 4;
      }
    }
  }
  else {
    if (iVar9 == 1) {
      if ((*DAT_8ceb06c0 != 1) && (*DAT_8ceb06c4 == 1)) {
        if (iVar10 < *(int *)(PTR_PTR_8ceb0544 + *(int *)puVar2 * 0xc + 8) << 1) {
          return 0;
        }
        *(int *)PTR_DAT_8ceb0540 = iVar10;
        *(int *)puVar5 = *(int *)puVar5 + 1;
        return 0;
      }
      if (iVar10 < *(int *)(PTR_PTR_8ceb0544 + *(int *)puVar2 * 0xc + 8)) {
        return 0;
      }
      *(int *)PTR_DAT_8ceb0540 = iVar10;
      *(int *)puVar5 = *(int *)puVar5 + 1;
      return 0;
    }
    if (iVar9 != 2) {
      if (iVar9 == 4) {
        *(int *)PTR_DAT_8ceb0540 = iVar10;
        *(int *)puVar5 = *(int *)puVar5 + 1;
      }
      else if (iVar9 != 5) {
        return 0;
      }
      iVar9 = (*(int *)puVar7 - *(int *)puVar6) * 3;
      if (iVar9 < -999) {
        iVar9 = -999;
      }
      (**(code **)(**(int **)puVar3 + 0x2c))(*(int **)puVar3,iVar9);
      uVar8 = 3;
      iVar10 = *(int *)puVar7;
      iVar9 = *(int *)puVar6;
      if (param_1 != 7) {
        iVar9 = (*DAT_8ceb06c8)(iVar10 - iVar9);
        if (iVar9 < 0) {
          *(undefined4 *)puVar5 = uVar8;
          iVar9 = 0;
        }
        (*(code *)puVar4)(*(undefined4 *)puVar3,iVar9);
        if (iVar9 != 0) {
          return 0;
        }
        return 1;
      }
      iVar9 = (*DAT_8ceb06c8)(iVar9 - iVar10);
      *(int *)puVar1 = iVar9;
      if (0xfe < iVar9) {
        *(undefined4 *)puVar1 = 0xffffffff;
        *(undefined4 *)puVar5 = uVar8;
      }
      if (-1 < *(int *)puVar1) {
        return 0;
      }
      return 1;
    }
    if (param_1 == 7) {
      iVar9 = (*DAT_8ceb06c8)(iVar10 - *(int *)PTR_DAT_8ceb0540);
      *(int *)puVar1 = iVar9;
      if (iVar9 < 0xff) {
        return 0;
      }
      *(undefined4 *)puVar1 = 0xffffffff;
      *(int *)puVar5 = *(int *)puVar5 + 1;
      return 0;
    }
    iVar9 = (*DAT_8ceb06c8)();
    if (iVar9 < 0) {
      iVar9 = 0;
      *(int *)puVar5 = *(int *)puVar5 + 1;
    }
  }
  (*(code *)puVar4)(*(undefined4 *)puVar3,iVar9);
  return 0;
}


/* Address: 8ceb06f4 */

void callback_candidate_8ceb06f4(void)

{
  (*DAT_8ceb0720)();
  (*DAT_8ceb0724)();
  (*DAT_8ceb0728)();
                    /* WARNING: Could not recover jumptable at 0x8ceb070a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*DAT_8ceb072c)();
  return;
}


/* Address: 8ceb0740 */

undefined4 * FUN_8ceb0740(uint param_1,int param_2)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  char in_FPSCR_SZ;
  
  puVar1 = (undefined4 *)(*DAT_8ceb08e0)(param_2 * 0xc);
  puVar2 = puVar1;
  while (param_2 != 0) {
    *puVar2 = 0;
    param_2 = param_2 + -1;
    if (in_FPSCR_SZ == '\0') {
      puVar2[1] = 0;
    }
    else {
      *(ulonglong *)(puVar2 + 1) = (ulonglong)param_1;
    }
    puVar2 = puVar2 + 3;
  }
  return puVar1;
}


/* Address: 8ceb0774 */

/* WARNING: Removing unreachable block (ram,0x8ceb0878) */
/* WARNING: Removing unreachable block (ram,0x8ceb08b6) */
/* WARNING: Removing unreachable block (ram,0x8ceb0832) */
/* WARNING: Removing unreachable block (ram,0x8ceb07e8) */
/* WARNING: Removing unreachable block (ram,0x8ceb0856) */
/* WARNING: Removing unreachable block (ram,0x8ceb096c) */
/* WARNING: Removing unreachable block (ram,0x8ceb098c) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void callback_candidate_8ceb0774(ulonglong param_1,ulonglong param_2,int param_3)

{
  code *pcVar1;
  undefined *puVar2;
  int iVar3;
  char *pcVar4;
  undefined4 *puVar5;
  int iVar6;
  int *piVar7;
  int *piVar8;
  double *pdVar9;
  float fVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  double dVar14;
  undefined4 uVar15;
  double dVar16;
  uint uVar18;
  float fVar19;
  double dVar17;
  double in_dr12;
  undefined4 uVar20;
  double in_dr14;
  double dVar21;
  double in_xd2;
  double in_xd4;
  double dVar22;
  double in_xd6;
  ulonglong in_xd14;
  bool bVar23;
  undefined4 in_PR;
  int iVar24;
  int iVar25;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 auStack_3c [16];
  double dStack_2c;
  undefined8 uStack_24;
  
  if (in_FPSCR_SZ == '\0') {
    uStack_24 = CONCAT44(SUB84(in_dr14,0),(undefined4)uStack_24);
                    /* WARNING: Ignoring partial resolution of indirect */
    uStack_24._0_4_ = (int)((ulonglong)in_dr14 >> 0x20);
    pdVar9 = &dStack_2c;
    dStack_2c = (double)CONCAT44(SUB84(in_dr12,0),(int)((ulonglong)in_dr12 >> 0x20));
  }
  else {
    pdVar9 = (double *)auStack_3c;
    uStack_24 = in_xd14;
    dStack_2c = in_dr14;
  }
  *(undefined4 *)((int)pdVar9 + -4) = in_PR;
  *(int *)((int)pdVar9 + -0x30) = param_3;
  if (-1 < param_3) {
    piVar7 = *(int **)PTR_DAT_8ceb08ec;
    piVar8 = *(int **)PTR_DAT_8ceb08e8;
    for (iVar3 = *(int *)PTR_DAT_8ceb08e4; puVar2 = PTR_DAT_8ceb0aa4, iVar3 != 0; iVar3 = iVar3 + -1
        ) {
      if (piVar8[5] == *(int *)((int)pdVar9 + -0x30)) {
        param_1 = param_1 & 0xffffffff;
        *piVar7 = 1;
        if (in_FPSCR_SZ == '\0') {
          piVar7[1] = 0;
        }
        else {
          *(ulonglong *)(piVar7 + 1) = param_1;
        }
      }
      if (*piVar7 != 0) {
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)pdVar9 + -0x14) = piVar8[4];
        }
        else {
          in_xd2 = *(double *)(piVar8 + 4);
          *(double *)((int)pdVar9 + -0x14) = in_xd2;
        }
        puVar5 = (undefined4 *)piVar8[3];
        *(undefined4 *)((int)pdVar9 + -0x18) = *puVar5;
        if (in_FPSCR_SZ == '\0') {
          dVar21 = (double)CONCAT44(piVar7[1],SUB84(in_dr14,0));
        }
        else {
          dVar21 = *(double *)(piVar7 + 1);
        }
        fVar13 = (float)((ulonglong)dVar21 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          iVar24 = (int)fVar13;
        }
        else {
          iVar24 = (int)dVar21;
        }
        dVar16 = dVar21;
        if (in_FPSCR_SZ == '\0') {
          dVar16 = (double)CONCAT44(fVar13,(int)param_1);
        }
        pcVar4 = (char *)(iVar24 * 0x10 + puVar5[1]);
        if (in_FPSCR_SZ == '\0') {
          uVar15 = *(undefined4 *)(pcVar4 + 4);
          dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),uVar15);
          dVar22 = (double)CONCAT44(uVar15,(float)iVar24);
        }
        else {
          dVar22 = *(double *)(pcVar4 + 4);
          uVar15 = SUB84(dVar16,0);
          in_xd4 = dVar22;
        }
        if (in_FPSCR_PR == '\0') {
          dVar16 = (double)CONCAT44((float)((ulonglong)dVar16 >> 0x20) - SUB84(dVar22,0),uVar15);
        }
        else {
          dVar16 = dVar16 - dVar22;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),*(undefined4 *)(pcVar4 + 0x14))
          ;
        }
        else {
          in_xd2 = *(double *)(pcVar4 + 0x14);
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),
                                    SUB84(dVar22,0) - SUB84(dVar16,0));
        }
        else {
          dVar22 = dVar22 - dVar16;
        }
        uVar18 = (uint)((ulonglong)dVar16 >> 0x20);
        dVar14 = dVar16;
        if (in_FPSCR_SZ == '\0') {
          dVar14 = (double)((ulonglong)uVar18 << 0x20);
        }
        fVar10 = (float)((ulonglong)dVar14 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44(fVar10 * SUB84(dVar22,0) + (float)((ulonglong)dVar22 >> 0x20),
                                    SUB84(dVar22,0));
        }
        uVar15 = (undefined4)((ulonglong)dVar22 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)((int)pdVar9 + -0x20) = uVar15;
          dVar22 = (double)CONCAT44(uVar15,*(undefined4 *)(pcVar4 + 0x18));
          dVar16 = (double)CONCAT44(uVar18,*(undefined4 *)(pcVar4 + 8));
        }
        else {
          *(double *)((int)pdVar9 + -0x20) = dVar22;
          in_xd2 = *(double *)(pcVar4 + 0x18);
          in_xd4 = *(double *)(pcVar4 + 8);
        }
        if (in_FPSCR_PR == '\0') {
          fVar12 = SUB84(dVar22,0) - SUB84(dVar16,0);
        }
        else {
          fVar12 = SUB84(dVar22 - dVar16,0);
        }
        dVar22 = in_xd4;
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44(SUB84(dVar16,0),fVar12);
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44(fVar10 * SUB84(dVar22,0) + (float)((ulonglong)dVar22 >> 0x20),
                                    SUB84(dVar22,0));
        }
        uVar15 = (undefined4)((ulonglong)dVar22 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)((int)pdVar9 + -0x24) = uVar15;
          dVar22 = (double)CONCAT44(uVar15,*(undefined4 *)(pcVar4 + 0x1c));
          dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),*(undefined4 *)(pcVar4 + 0xc));
        }
        else {
          *(double *)((int)pdVar9 + -0x24) = dVar22;
          in_xd2 = *(double *)(pcVar4 + 0x1c);
          in_xd4 = *(double *)(pcVar4 + 0xc);
        }
        if (in_FPSCR_PR == '\0') {
          fVar12 = SUB84(dVar22,0) - SUB84(dVar16,0);
        }
        else {
          fVar12 = SUB84(dVar22 - dVar16,0);
        }
        dVar22 = in_xd4;
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44(SUB84(dVar16,0),fVar12);
        }
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44(fVar10 * SUB84(dVar22,0) + (float)((ulonglong)dVar22 >> 0x20),
                                    SUB84(dVar22,0));
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)pdVar9 + -0x28) = (int)((ulonglong)dVar22 >> 0x20);
        }
        else {
          *(double *)((int)pdVar9 + -0x28) = dVar22;
        }
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44(DAT_8ceb08f0,(int)param_2);
        }
        else {
          dVar22 = (double)CONCAT44(DAT_8ceb08f4,DAT_8ceb08f0);
        }
        iVar24 = (int)(char)(pcVar4[0x10] - *pcVar4);
        if (in_FPSCR_SZ == '\0') {
          dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),DAT_8ceb08f4);
        }
        else {
          in_xd4 = (double)CONCAT44(_DAT_8ceb08f8,DAT_8ceb08f4);
        }
        if (in_FPSCR_PR == '\0') {
          fVar12 = (float)iVar24;
          dVar14 = (double)CONCAT44(fVar12,fVar10 * fVar12 + (float)(int)*pcVar4);
        }
        else {
          dVar14 = (double)iVar24;
        }
        dVar17 = in_xd2;
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),SUB84(dVar14,0));
          dVar17 = in_xd6;
        }
        fVar10 = SUB84(dVar16,0);
        if (in_FPSCR_PR == '\0') {
          fVar11 = (float)((ulonglong)dVar22 >> 0x20);
          fVar12 = SUB84(dVar22,0) * fVar11 + fVar10;
          dVar22 = (double)CONCAT44(fVar11,fVar12);
          iVar24 = (int)fVar12;
        }
        else {
          dVar22 = dVar22 * dVar22 + dVar16;
          iVar24 = (int)dVar22;
        }
        *(int *)((int)pdVar9 + -0x2c) = iVar24;
        iVar24 = (int)(char)(pcVar4[0x11] - pcVar4[1]);
        uVar15 = (undefined4)((ulonglong)dVar14 >> 0x20);
        fVar12 = (float)(int)pcVar4[1];
        if (in_FPSCR_PR == '\0') {
          fVar11 = (float)iVar24;
        }
        else {
          fVar11 = (float)((ulonglong)(double)iVar24 >> 0x20);
        }
        fVar19 = (float)((ulonglong)dVar16 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          fVar12 = fVar11 * fVar19 + fVar12;
        }
        in_xd6 = in_xd2;
        if (in_FPSCR_SZ == '\0') {
          dVar22 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),fVar12);
          in_xd6 = dVar17;
        }
        if (in_FPSCR_PR == '\0') {
          fVar11 = (float)((ulonglong)dVar22 >> 0x20);
          dVar22 = (double)CONCAT44(fVar11,SUB84(dVar22,0) * fVar11);
        }
        else {
          dVar22 = dVar22 * dVar22;
        }
        dVar14 = in_xd6;
        if (in_FPSCR_SZ == '\0') {
          fVar12 = SUB84(dVar22,0);
          dVar14 = in_xd2;
        }
        if (in_FPSCR_PR == '\0') {
          dVar16 = (double)CONCAT44(uVar15,fVar12 + fVar10);
          iVar24 = (int)(fVar12 + fVar10);
        }
        else {
          dVar16 = (double)CONCAT44(uVar15,fVar12) + dVar16;
          iVar24 = (int)dVar16;
        }
        fVar12 = (float)(int)pcVar4[2];
        if (in_FPSCR_PR == '\0') {
          fVar12 = (float)(int)(char)(pcVar4[0x12] - pcVar4[2]) * fVar19 + fVar12;
        }
        dVar17 = dVar14;
        if (in_FPSCR_SZ == '\0') {
          dVar17 = (double)CONCAT44(fVar12,fVar10);
        }
        if (in_FPSCR_PR == '\0') {
          dVar17 = (double)CONCAT44((float)((ulonglong)dVar17 >> 0x20) *
                                    (float)((ulonglong)dVar22 >> 0x20),SUB84(dVar17,0));
        }
        else {
          dVar17 = dVar17 * dVar22;
        }
        in_xd2 = dVar17;
        if (in_FPSCR_SZ == '\0') {
          fVar12 = (float)((ulonglong)dVar17 >> 0x20);
          in_xd2 = dVar14;
        }
        if (in_FPSCR_PR == '\0') {
          fVar12 = fVar12 + SUB84(dVar17,0);
        }
        else {
          fVar12 = SUB84((double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),fVar12) + dVar17,0);
        }
        if (in_FPSCR_PR == '\0') {
          iVar25 = (int)fVar12;
        }
        else {
          iVar25 = (int)(double)(ulonglong)(uint)fVar12;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)((int)pdVar9 + -0xc) = piVar8[6];
          in_dr12 = (double)CONCAT44(piVar8[7],SUB84(in_dr12,0));
          *(int *)((int)pdVar9 + -0x1c) = piVar8[8];
          dVar21 = (double)CONCAT44(fVar13,piVar7[2]);
          *(int *)((int)pdVar9 + -0x10) = piVar8[1];
        }
        else {
          *(undefined8 *)((int)pdVar9 + -0xc) = *(undefined8 *)(piVar8 + 6);
          in_dr12 = *(double *)(piVar8 + 7);
          *(undefined8 *)((int)pdVar9 + -0x1c) = *(undefined8 *)(piVar8 + 8);
          in_xd14 = *(ulonglong *)(piVar7 + 2);
          in_xd2 = *(double *)(piVar8 + 1);
          *(double *)((int)pdVar9 + -0x10) = in_xd2;
        }
        iVar6 = *piVar8;
        fVar13 = (float)*(int *)(*(int *)(iVar6 + 4) + 4);
        dVar16 = (double)(ulonglong)(uint)fVar13;
        dVar14 = in_xd2;
        if (in_FPSCR_SZ == '\0') {
          dVar14 = (double)CONCAT44(fVar13,SUB84(dVar17,0));
        }
        uVar15 = (undefined4)((ulonglong)dVar14 >> 0x20);
        uVar20 = (undefined4)((ulonglong)in_dr12 >> 0x20);
        if (in_FPSCR_PR != '\0' || 0.0 <= fVar13) {
          if (in_FPSCR_SZ == '\0') {
            in_dr12 = (double)CONCAT44(uVar20,uVar15);
          }
        }
        else {
          if (in_FPSCR_SZ == '\0') {
            in_dr12 = (double)CONCAT44(uVar20,uVar15);
            dVar16 = (double)(ulonglong)_DAT_8ceb08f8;
          }
          else {
            in_xd2 = (double)CONCAT44(uRam8ceb08fc,_DAT_8ceb08f8);
          }
          if (in_FPSCR_PR == '\0') {
            in_dr12 = (double)CONCAT44((int)((ulonglong)in_dr12 >> 0x20),
                                       SUB84(in_dr12,0) + SUB84(dVar16,0));
          }
          else {
            in_dr12 = in_dr12 + dVar16;
          }
        }
        *(int *)((int)pdVar9 + -8) = piVar8[2];
        (*DAT_8ceb0a80)(0);
        pcVar1 = DAT_8ceb0a84;
        if (in_FPSCR_SZ == '\0') {
          uVar18 = SUB84(dVar22,0);
        }
        else {
          uVar18 = (uint)*(undefined8 *)((int)pdVar9 + -0x28);
          in_xd4 = *(double *)((int)pdVar9 + -0x24);
        }
        (*DAT_8ceb0a84)(0);
        (*DAT_8ceb0a88)(0,*(undefined4 *)((int)pdVar9 + -0x2c),iVar24,iVar25);
        if (in_FPSCR_SZ == '\0') {
          param_2 = (ulonglong)uVar18;
          uVar18 = (uint)((ulonglong)in_dr12 >> 0x20);
        }
        else {
          param_2 = *(ulonglong *)((int)pdVar9 + -0x1c);
          uVar18 = (uint)*(undefined8 *)((int)pdVar9 + -0xc);
          in_xd4 = in_dr12;
        }
        (*pcVar1)(0);
        (*DAT_8ceb0a8c)(*(undefined4 *)((int)pdVar9 + -8));
        param_1 = in_xd14;
        if (in_FPSCR_SZ == '\0') {
          param_1 = (ulonglong)uVar18;
        }
        (*DAT_8ceb0a90)(iVar6);
        (*DAT_8ceb0a94)(1);
        if (in_FPSCR_SZ == '\0') {
          dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),
                                    *(undefined4 *)((int)pdVar9 + -0x10));
        }
        else {
          in_xd2 = *(double *)((int)pdVar9 + -0x10);
        }
        if (in_FPSCR_PR == '\0') {
          fVar13 = SUB84(dVar21,0) + SUB84(dVar16,0);
          dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),fVar13);
          bVar23 = fVar13 < SUB84(in_dr12,0);
        }
        else {
          dVar21 = dVar21 + dVar16;
          fVar13 = SUB84(dVar21,0);
          bVar23 = dVar21 < in_dr12;
        }
        if (!bVar23) {
          if (in_FPSCR_PR == '\0') {
            dVar21 = (double)CONCAT44((int)((ulonglong)dVar21 >> 0x20),fVar13 - SUB84(in_dr12,0));
          }
          else {
            dVar21 = dVar21 - in_dr12;
          }
        }
        if (in_FPSCR_SZ == '\0') {
          dVar16 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),
                                    *(undefined4 *)((int)pdVar9 + -0x14));
          piVar7[2] = SUB84(dVar21,0);
        }
        else {
          in_xd2 = *(double *)((int)pdVar9 + -0x14);
          *(ulonglong *)(piVar7 + 2) = in_xd14;
        }
        if (in_FPSCR_PR == '\0') {
          in_dr14 = (double)CONCAT44((float)((ulonglong)dVar21 >> 0x20) + SUB84(dVar16,0),
                                     SUB84(dVar21,0));
        }
        else {
          in_dr14 = dVar21 + dVar16;
        }
        fVar13 = (float)((ulonglong)in_dr14 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          piVar7[1] = (int)fVar13;
        }
        else {
          *(double *)(piVar7 + 1) = in_dr14;
        }
        if (in_FPSCR_PR == '\0') {
          bVar23 = fVar13 < (float)(*(int *)((int)pdVar9 + -0x18) + -1);
        }
        else {
          bVar23 = in_dr14 < (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),
                                              (float)(*(int *)((int)pdVar9 + -0x18) + -1));
        }
        if (!bVar23) {
          *piVar7 = 0;
        }
      }
      piVar7 = piVar7 + 3;
      piVar8 = piVar8 + 9;
    }
    iVar3 = *(int *)PTR_DAT_8ceb0a98;
    if (iVar3 < 0) {
      if (in_FPSCR_SZ == '\0') {
        in_xd14 = CONCAT44(0xbf800000,(int)param_1);
      }
                    /* WARNING: Could not recover jumptable at 0x8ceb09b2. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (*DAT_8ceb0aa0)(in_xd14,PTR_PTR_8ceb0a9c,4,0x20);
      return;
    }
    if (iVar3 != 0) {
      if (in_FPSCR_SZ == '\0') {
        in_xd14 = CONCAT44(0xbf800000,(int)param_1);
      }
      iVar3 = iVar3 << 0x18;
      *(int *)(PTR_DAT_8ceb0aa4 + 0xc) = iVar3;
      *(int *)(puVar2 + 8) = iVar3;
      *(int *)(puVar2 + 4) = iVar3;
      *(int *)puVar2 = iVar3;
                    /* WARNING: Could not recover jumptable at 0x8ceb09ec. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (*DAT_8ceb0aa0)(in_xd14,PTR_PTR_8ceb0a9c,4,0x60);
      return;
    }
  }
  return;
}


/* Address: 8ceb0a0c */

void FUN_8ceb0a0c(undefined4 *param_1)

{
  undefined *puVar1;
  undefined4 uVar2;
  
  puVar1 = PTR_DAT_8ceb0aac;
  *(undefined4 *)PTR_DAT_8ceb0aa8 = *param_1;
  *(undefined4 *)puVar1 = param_1[1];
  uVar2 = FUN_8ceb0740(param_1[1]);
  *(undefined4 *)PTR_DAT_8ceb0ab0 = uVar2;
  return;
}


/* Address: 8ceb0a2c */

void callback_candidate_8ceb0a2c(undefined4 param_1)

{
  code *pcVar1;
  int iVar2;
  undefined *puVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  char in_FPSCR_SZ;
  
  (*DAT_8ceb0ab4)(0x31c7);
  (*DAT_8ceb0ac0)(DAT_8ceb0abc,DAT_8ceb0ab8);
  if (in_FPSCR_SZ == '\0') {
    uVar5 = CONCAT44((undefined4)DAT_8ceb0ac4,param_1);
    uVar4 = CONCAT44(DAT_8ceb0acc,DAT_8ceb0ac4._4_4_);
  }
  else {
    uVar5 = CONCAT44(DAT_8ceb0ac4._4_4_,(undefined4)DAT_8ceb0ac4);
    uVar4 = CONCAT44(DAT_8ceb0ad0,DAT_8ceb0acc);
  }
  (*DAT_8ceb0ad0)(uVar4,uVar5,DAT_8ceb0abc);
  (*DAT_8ceb0ad4)(DAT_8ceb0abc);
  iVar2 = (*DAT_8ceb0ad8)();
  pcVar1 = DAT_8ceb0adc;
  if (iVar2 == 3) {
    (*DAT_8ceb0adc)(PTR_s_KNUCKLES_PRS_8ceb0ae4,DAT_8ceb0ae0);
    FUN_8ceb0a0c(PTR_PTR_8ceb0ae8);
    return;
  }
  if (iVar2 == 7) {
    (*DAT_8ceb0adc)(PTR_s_EV_TR2_BIG_PRS_8ceb0b44,PTR_PTR_8ceb0b40);
    puVar3 = PTR_PTR_8ceb0b48;
  }
  else {
    (*DAT_8ceb0adc)(PTR_s_AMY_PRS_8ceb0b50,DAT_8ceb0b4c);
    (*pcVar1)(PTR_s_MILES_PRS_8ceb0b58,DAT_8ceb0b54);
    puVar3 = PTR_PTR_8ceb0b5c;
  }
  FUN_8ceb0a0c(puVar3);
  return;
}


/* Address: 8ceb0b2c */

void callback_candidate_8ceb0b2c(void)

{
  (*DAT_8ceb0b64)(*(undefined4 *)PTR_DAT_8ceb0b60);
  return;
}


/* Address: 8ceb0b80 */

void callback_candidate_8ceb0b80(char *param_1)

{
  undefined *puVar1;
  uint uVar2;
  uint uVar3;
  char *pcVar4;
  undefined4 *puVar5;
  int iVar6;
  int iVar7;
  
  iVar6 = 6;
  uVar3 = 0;
  puVar5 = *(undefined4 **)(param_1 + 0x30);
  uVar2 = *(uint *)(param_1 + 0x10);
  iVar7 = *(int *)(param_1 + 0x2c);
  pcVar4 = param_1;
  do {
    if (*pcVar4 == '\0') {
      (**(code **)(param_1 + 8))(iVar7,*puVar5);
    }
    else {
      uVar3 = uVar3 | uVar2;
    }
    uVar2 = uVar2 >> 1;
    iVar6 = iVar6 + -1;
    iVar7 = iVar7 + *(int *)(param_1 + 0x24);
    puVar5 = puVar5 + 1;
    pcVar4 = pcVar4 + 1;
  } while (iVar6 != 0);
  if (uVar3 != 0) {
    *(uint *)(param_1 + 0x28) = uVar3;
    puVar1 = PTR_callback_candidate_8ceb1504_8ceb0c70;
    *(undefined4 *)(param_1 + 0x34) = *(undefined4 *)(param_1 + 0x30);
    (*(code *)puVar1)(param_1);
    return;
  }
  return;
}


/* Address: 8ceb0bf2 */

void callback_candidate_8ceb0bf2(char *param_1)

{
  undefined *puVar1;
  int iVar2;
  char *pcVar3;
  undefined4 *puVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  
  iVar5 = 6;
  iVar2 = *(int *)(param_1 + 0x28);
  uVar6 = *(uint *)(param_1 + 0x14);
  puVar4 = *(undefined4 **)(param_1 + 0x30);
  iVar7 = *(int *)(param_1 + 0x2c);
  pcVar3 = param_1;
  do {
    if ((iVar2 < 0) && (*pcVar3 == '\0')) {
      (**(code **)(param_1 + 8))(iVar7,*puVar4);
      *(uint *)(param_1 + 0x28) = *(uint *)(param_1 + 0x28) & uVar6;
    }
    puVar1 = PTR_callback_candidate_8ceb1504_8ceb0c70;
    puVar4 = puVar4 + 1;
    iVar2 = iVar2 << 1;
    iVar5 = iVar5 + -1;
    uVar6 = *(uint *)(param_1 + 0x10) | uVar6 >> 1;
    iVar7 = iVar7 + *(int *)(param_1 + 0x24);
    pcVar3 = pcVar3 + 1;
  } while (iVar5 != 0);
  if (*(int *)(param_1 + 0x28) != 0) {
    *(undefined4 *)(param_1 + 0x34) = *(undefined4 *)(param_1 + 0x30);
    (*(code *)puVar1)(param_1);
    return;
  }
  return;
}


/* Address: 8ceb0c74 */

undefined4 callback_candidate_8ceb0c74(int param_1)

{
  undefined4 *puVar1;
  undefined *puVar2;
  undefined1 uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  (*(code *)PTR_FUN_8ceb0dc4)(DAT_8ceb0da8 + param_1);
  puVar1 = DAT_8ceb0dc8;
  iVar4 = (int)DAT_8ceb0dac;
  iVar6 = param_1 + 0x3c;
  *(undefined4 *)(param_1 + 0x60) = *(undefined4 *)(DAT_8ceb0daa + param_1);
  *(int *)(param_1 + 0x5c) = iVar4 + param_1;
  *(undefined4 *)(param_1 + 0x6c) = 0;
  iVar4 = (int)DAT_8ceb0dae;
  *(undefined4 *)(param_1 + 0x68) = *puVar1;
  puVar2 = PTR_callback_candidate_8ceb17e8_8ceb0dcc;
  iVar5 = (int)DAT_8ceb0da8;
  *(int *)(param_1 + 100) = iVar4 + param_1;
  *(int *)(param_1 + 0x58) = iVar5 + param_1;
  uVar3 = (*(code *)puVar2)(param_1,iVar6);
  iVar4 = (int)DAT_8ceb0db0;
  *(undefined1 *)(param_1 + 0x70) = uVar3;
  *(int *)(param_1 + 0x58) = iVar4 + param_1;
  uVar3 = (*(code *)puVar2)(param_1,iVar6);
  iVar4 = (int)DAT_8ceb0db2;
  *(undefined1 *)(param_1 + 0x71) = uVar3;
  *(int *)(param_1 + 0x58) = iVar4 + param_1;
  uVar3 = (*(code *)puVar2)(param_1,iVar6);
  iVar4 = (int)DAT_8ceb0db4;
  *(undefined1 *)(param_1 + 0x72) = uVar3;
  *(int *)(param_1 + 0x58) = iVar4 + param_1;
  uVar3 = (*(code *)puVar2)(param_1,iVar6);
  puVar1 = DAT_8ceb0dd0;
  iVar4 = (int)DAT_8ceb0db6;
  *(undefined1 *)(param_1 + 0x73) = uVar3;
  *(undefined4 *)(param_1 + 0x68) = *puVar1;
  iVar5 = (int)DAT_8ceb0db8;
  *(int *)(param_1 + 100) = iVar4 + param_1;
  *(int *)(param_1 + 0x58) = iVar5 + param_1;
  uVar3 = (*(code *)puVar2)(param_1,iVar6);
  iVar5 = (int)DAT_8ceb0dbc;
  iVar4 = (int)DAT_8ceb0dba;
  *(undefined1 *)(param_1 + 0x74) = uVar3;
  *(int *)(param_1 + 100) = iVar4 + param_1;
  *(int *)(param_1 + 0x58) = iVar5 + param_1;
  uVar3 = (*(code *)puVar2)(param_1,iVar6);
  puVar2 = PTR_callback_candidate_8ceb0b80_8ceb0dd4;
  *(undefined1 *)(param_1 + 0x75) = uVar3;
  (*(code *)puVar2)((undefined1 *)(param_1 + 0x70));
  return 0;
}


/* Address: 8ceb0d2e */

undefined4 callback_candidate_8ceb0d2e(int param_1)

{
  undefined *puVar1;
  undefined1 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined1 *puVar6;
  uint uVar7;
  
  iVar3 = (int)DAT_8ceb0daa;
  iVar4 = (int)DAT_8ceb0dbe;
  iVar5 = (int)DAT_8ceb0dc0;
  uVar7 = DAT_8ceb0da8 + param_1;
  *(undefined4 *)(param_1 + 0x60) = *(undefined4 *)(iVar3 + param_1);
  *(int *)(param_1 + 0x5c) = iVar4 + param_1;
  *(undefined4 *)(param_1 + 0x6c) = 1;
  iVar3 = *(int *)(iVar3 + 0x58 + param_1) << 2;
  *(int *)(param_1 + 0x98) = iVar3;
  puVar6 = (undefined1 *)(param_1 + 0x70);
  do {
    puVar1 = PTR_FUN_8ceb0dd8;
    if (iVar3 < 0) {
      *(uint *)(param_1 + 0x58) = uVar7;
      uVar2 = (*(code *)puVar1)(param_1,param_1 + 0x3c);
      *puVar6 = uVar2;
    }
    uVar7 = uVar7 + iVar5;
    iVar3 = iVar3 << 1;
    puVar6 = puVar6 + 1;
  } while (uVar7 < (uint)(DAT_8ceb0dc2 * 2 + -0x20 + param_1));
  (*(code *)PTR_callback_candidate_8ceb0bf2_8ceb0ddc)((undefined1 *)(param_1 + 0x70));
  return 0;
}


/* Address: 8ceb0de0 */

uint callback_candidate_8ceb0de0(int param_1)

{
  short sVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  byte in_MD;
  byte bVar6;
  byte in_RB;
  byte bVar7;
  byte in_BL;
  byte bVar8;
  byte in_FD;
  byte bVar9;
  byte in_M;
  byte extraout_M;
  byte in_Q;
  byte extraout_Q;
  byte in_IMASK;
  byte bVar10;
  byte in_S;
  byte extraout_S;
  byte in_T;
  byte extraout_T;
  int local_1c;
  int local_18;
  
  bVar8 = (byte)(((uint)in_BL << 0x1c) >> 0x18);
  bVar6 = bVar8 | (byte)(((uint)in_RB << 0x1d) >> 0x18);
  bVar10 = (byte)((((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4) & 0xf0) >> 4);
  bVar9 = (byte)((((uint)in_Q << 8 | (uint)in_M << 9 | (uint)in_FD << 0xf) & 0x8000) >> 0xf);
  bVar8 = (bVar8 & 0x10) >> 4;
  bVar7 = (bVar6 & 0x20) >> 5;
  bVar6 = ((bVar6 | (byte)(((uint)in_MD << 0x1e) >> 0x18)) & 0x40) >> 6;
  iVar3 = DAT_8ceb0f28 + param_1;
  uVar2 = *(uint *)(iVar3 + 0x20);
  iVar5 = DAT_8ceb0f2a + param_1;
  *(uint *)(iVar3 + 0x20) = uVar2 + (int)DAT_8ceb0f2c;
  uVar4 = uVar2 >> 0x18 & 0x1c;
  *DAT_8ceb0f34 = uVar4;
  *DAT_8ceb0f38 = uVar4;
  uVar2 = uVar2 & DAT_8ceb0f3c | DAT_8ceb0f40;
  FUN_8ceb1452(param_1,*(undefined4 *)(DAT_8ceb0f2e + param_1),&local_1c,(int *)(iVar3 + 0x10));
  sVar1 = *(short *)(iVar3 + 0x1e);
  *(int *)(iVar5 + 4) = *(int *)(iVar3 + 0x10) + local_1c;
  *(int *)(iVar5 + 0xc) = *(int *)(iVar3 + 0x14) + local_1c;
  local_18 = *(int *)(iVar3 + 0x18) + local_18;
  *(int *)(iVar5 + 0x14) = local_18;
  *(int *)(iVar5 + 0x1c) = local_18 + 8;
  iVar3 = sVar1 * 8 + *(int *)(iVar5 + 0x14);
  *(int *)(iVar5 + 0x24) = iVar3;
  *(int *)(iVar5 + 0x2c) = iVar3 + 8;
  (*(code *)PTR_callback_candidate_8ceb1c10_8ceb0f44)
            (DAT_8ceb0f30 + param_1,uVar2,iVar5,*(undefined4 *)(param_1 + 0x38));
  return ((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4) & 0xf0 |
         ((uint)extraout_T | (uint)extraout_S << 1 | (uint)bVar10 << 4 | (uint)extraout_Q << 8 |
          (uint)extraout_M << 9 | (uint)bVar9 << 0xf | (uint)bVar8 << 0x1c | (uint)bVar7 << 0x1d |
         (uint)bVar6 << 0x1e) & (int)DAT_8ceb0f26;
}


/* Address: 8ceb0eb2 */

uint callback_candidate_8ceb0eb2(int param_1)

{
  uint uVar1;
  uint uVar2;
  byte in_MD;
  byte bVar3;
  byte in_RB;
  byte bVar4;
  byte in_BL;
  byte bVar5;
  byte in_FD;
  byte bVar6;
  byte in_M;
  byte extraout_M;
  byte in_Q;
  byte extraout_Q;
  byte in_IMASK;
  byte bVar7;
  byte in_S;
  byte extraout_S;
  byte in_T;
  byte extraout_T;
  
  bVar5 = (byte)(((uint)in_BL << 0x1c) >> 0x18);
  bVar3 = bVar5 | (byte)(((uint)in_RB << 0x1d) >> 0x18);
  bVar7 = (byte)((((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4) & 0xf0) >> 4);
  bVar6 = (byte)((((uint)in_Q << 8 | (uint)in_M << 9 | (uint)in_FD << 0xf) & 0x8000) >> 0xf);
  bVar5 = (bVar5 & 0x10) >> 4;
  bVar4 = (bVar3 & 0x20) >> 5;
  bVar3 = ((bVar3 | (byte)(((uint)in_MD << 0x1e) >> 0x18)) & 0x40) >> 6;
  uVar1 = *(uint *)(DAT_8ceb0f28 + param_1 + 0x20);
  *(uint *)(DAT_8ceb0f28 + param_1 + 0x20) = uVar1 + (int)DAT_8ceb0f2c;
  uVar2 = uVar1 >> 0x18 & 0x1c;
  *DAT_8ceb0f34 = uVar2;
  *DAT_8ceb0f38 = uVar2;
  uVar1 = uVar1 & DAT_8ceb0f3c | DAT_8ceb0f40;
  (*(code *)PTR_callback_candidate_8ceb1c10_8ceb0f44)
            (DAT_8ceb0f30 + param_1,uVar1,DAT_8ceb0f32 + param_1,*(undefined4 *)(param_1 + 0x38),
             uVar1);
  return ((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4) & 0xf0 |
         ((uint)extraout_T | (uint)extraout_S << 1 | (uint)bVar7 << 4 | (uint)extraout_Q << 8 |
          (uint)extraout_M << 9 | (uint)bVar6 << 0xf | (uint)bVar5 << 0x1c | (uint)bVar4 << 0x1d |
         (uint)bVar3 << 0x1e) & (int)DAT_8ceb0f26;
}


/* Address: 8ceb0f48 */

void callback_candidate_8ceb0f48(int param_1)

{
  int iVar1;
  int iVar2;
  undefined1 auStack_18 [12];
  
  iVar1 = (int)DAT_8ceb104a;
  iVar2 = DAT_8ceb1048 + param_1;
  FUN_8ceb1054(param_1,*(undefined4 *)(iVar2 + 8),auStack_18,iVar1 + param_1,DAT_8ceb104c + param_1)
  ;
  FUN_8ceb11fc(param_1,iVar2);
  (*(code *)PTR_FUN_8ceb1050)(*(undefined4 *)(iVar2 + 8),auStack_18,iVar1 + param_1 + 0x10);
  return;
}


/* Address: 8ceb0f8e */

void callback_candidate_8ceb0f8e(int param_1)

{
  int iVar1;
  undefined1 auStack_14 [12];
  
  iVar1 = (int)DAT_8ceb1048;
  FUN_8ceb1054(param_1,*(undefined4 *)(iVar1 + param_1 + 8),auStack_14,DAT_8ceb104a + param_1,
               DAT_8ceb104c + param_1);
  FUN_8ceb11fc(param_1,iVar1 + param_1);
  return;
}


/* Address: 8ceb0fc4 */

void callback_candidate_8ceb0fc4(int param_1)

{
  int iVar1;
  undefined1 auStack_14 [12];
  
  iVar1 = (int)DAT_8ceb1048;
  FUN_8ceb1054(param_1,*(undefined4 *)(iVar1 + param_1 + 8),auStack_14,DAT_8ceb104a + param_1 + 0x10
               ,DAT_8ceb104e + param_1);
  FUN_8ceb11fc(param_1,iVar1 + param_1);
  return;
}


/* Address: 8ceb0ffc */

void callback_candidate_8ceb0ffc(int param_1)

{
  int iVar1;
  int iVar2;
  undefined1 auStack_18 [12];
  
  iVar1 = (int)DAT_8ceb104a;
  iVar2 = DAT_8ceb1048 + param_1;
  FUN_8ceb1054(param_1,*(undefined4 *)(iVar2 + 8),auStack_18,iVar1 + param_1,DAT_8ceb104c + param_1)
  ;
  FUN_8ceb1054(param_1,*(undefined4 *)(iVar2 + 0xc),auStack_18,iVar1 + param_1 + 0x10,
               DAT_8ceb104e + param_1);
  FUN_8ceb126c(param_1,iVar2);
  return;
}


/* Address: 8ceb1054 */

/* WARNING: Restarted to delay deadcode elimination for space: stack */

void FUN_8ceb1054(int param_1,int param_2,int *param_3,int *param_4,int param_5)

{
  int iVar1;
  int iVar2;
  int iVar3;
  code *pcVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  code *pcVar9;
  int local_40;
  int local_3c;
  int local_38;
  int local_34;
  int local_30;
  int local_2c;
  int local_28;
  int local_24;
  
  iVar8 = *(int *)(DAT_8ceb1180 + param_1);
  iVar1 = (int)*(short *)((int)param_4 + 0xe);
  iVar2 = (int)(short)param_4[3];
  FUN_8ceb1452(param_1,*(undefined4 *)(DAT_8ceb1182 + param_1),param_3,param_4);
  iVar5 = *(int *)(param_5 + 0x18);
  iVar6 = *(int *)(param_5 + 0x1c);
  FUN_8ceb14a0((int)(iVar5 + (uint)(iVar5 < 0)) >> 1,(int)(iVar6 + (uint)(iVar6 < 0)) >> 1,&local_30
              );
  iVar3 = *param_3 + local_30 + local_2c * iVar2;
  FUN_8ceb14a0(iVar5,iVar6,&local_40);
  iVar6 = param_3[1];
  pcVar4 = *(code **)(PTR_PTR_8ceb1188 + (local_24 * 2 + local_28) * 4 + iVar8 * 0x10);
  param_1 = DAT_8ceb1184 + param_1;
  pcVar9 = *(code **)(PTR_PTR_8ceb1188 + (local_34 * 2 + local_38) * 4 + iVar8 * 0x10);
  *(int *)(param_1 + 0x20) = iVar2;
  *(int *)(param_1 + 0x18) = param_2;
  iVar7 = *param_4 + iVar3;
  *(int *)(param_1 + 0x24) = iVar7;
  iVar5 = local_28;
  if (iVar8 == 0) {
    iVar5 = 0;
  }
  *(int *)(param_1 + 0x28) = iVar7 + iVar5 + iVar2;
  (*pcVar4)(param_1);
  *(int *)(param_1 + 0x18) = param_2 + 0x40;
  iVar3 = param_4[1] + iVar3;
  *(int *)(param_1 + 0x24) = iVar3;
  if (iVar8 == 0) {
    local_28 = 0;
  }
  *(int *)(param_1 + 0x28) = iVar3 + local_28 + iVar2;
  (*pcVar4)(param_1);
  iVar5 = (int)DAT_8ceb1186;
  *(int *)(param_1 + 0x20) = iVar1;
  *(int *)(param_1 + 0x18) = iVar5 + param_2;
  iVar5 = param_4[2] + iVar6 + local_40 + local_3c * iVar1;
  *(int *)(param_1 + 0x24) = iVar5;
  if (iVar8 == 0) {
    local_38 = 0;
  }
  *(int *)(param_1 + 0x28) = iVar5 + local_38 + iVar1;
  (*pcVar9)(param_1);
  *(int *)(param_1 + 0x18) = DAT_8ceb12dc + param_2;
  *(int *)(param_1 + 0x24) = *(int *)(param_1 + 0x24) + 8;
  *(int *)(param_1 + 0x28) = *(int *)(param_1 + 0x28) + 8;
  (*pcVar9)(param_1);
  *(int *)(param_1 + 0x18) = DAT_8ceb12de + param_2;
  *(int *)(param_1 + 0x24) = *(int *)(param_1 + 0x24) + (iVar1 + -1) * 8;
  *(int *)(param_1 + 0x28) = *(int *)(param_1 + 0x28) + (iVar1 + -1) * 8;
  (*pcVar9)(param_1);
  *(int *)(param_1 + 0x18) = param_2 + DAT_8ceb12e0;
  *(int *)(param_1 + 0x24) = *(int *)(param_1 + 0x24) + 8;
  *(int *)(param_1 + 0x28) = *(int *)(param_1 + 0x28) + 8;
  (*pcVar9)(param_1);
  return;
}


/* Address: 8ceb11fc */

uint FUN_8ceb11fc(int param_1)

{
  uint uVar1;
  uint uVar2;
  byte in_MD;
  byte bVar3;
  byte in_RB;
  byte bVar4;
  byte in_BL;
  byte bVar5;
  byte in_FD;
  byte bVar6;
  byte in_M;
  byte extraout_M;
  byte in_Q;
  byte extraout_Q;
  byte in_IMASK;
  byte bVar7;
  byte in_S;
  byte extraout_S;
  byte in_T;
  byte extraout_T;
  
  bVar5 = (byte)(((uint)in_BL << 0x1c) >> 0x18);
  bVar3 = bVar5 | (byte)(((uint)in_RB << 0x1d) >> 0x18);
  bVar7 = (byte)((((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4) & 0xf0) >> 4);
  bVar6 = (byte)((((uint)in_Q << 8 | (uint)in_M << 9 | (uint)in_FD << 0xf) & 0x8000) >> 0xf);
  bVar5 = (bVar5 & 0x10) >> 4;
  bVar4 = (bVar3 & 0x20) >> 5;
  bVar3 = ((bVar3 | (byte)(((uint)in_MD << 0x1e) >> 0x18)) & 0x40) >> 6;
  uVar1 = *(uint *)(param_1 + DAT_8ceb12e4 + 0x20);
  *(uint *)(param_1 + DAT_8ceb12e4 + 0x20) = uVar1 + (int)DAT_8ceb12e6;
  uVar2 = uVar1 >> 0x18 & 0x1c;
  *DAT_8ceb12ec = uVar2;
  *DAT_8ceb12f0 = uVar2;
  (*(code *)PTR_FUN_8ceb12fc)
            (uVar1 & DAT_8ceb12f4 | DAT_8ceb12f8,*(undefined4 *)(DAT_8ceb12e8 + param_1));
  return ((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4) & 0xf0 |
         ((uint)extraout_T | (uint)extraout_S << 1 | (uint)bVar7 << 4 | (uint)extraout_Q << 8 |
          (uint)extraout_M << 9 | (uint)bVar6 << 0xf | (uint)bVar5 << 0x1c | (uint)bVar4 << 0x1d |
         (uint)bVar3 << 0x1e) & (int)DAT_8ceb12e2;
}


/* Address: 8ceb126c */

uint FUN_8ceb126c(int param_1)

{
  uint uVar1;
  uint uVar2;
  byte in_MD;
  byte bVar3;
  byte in_RB;
  byte bVar4;
  byte in_BL;
  byte bVar5;
  byte in_FD;
  byte bVar6;
  byte in_M;
  byte extraout_M;
  byte in_Q;
  byte extraout_Q;
  byte in_IMASK;
  byte bVar7;
  byte in_S;
  byte extraout_S;
  byte in_T;
  byte extraout_T;
  
  bVar5 = (byte)(((uint)in_BL << 0x1c) >> 0x18);
  bVar3 = bVar5 | (byte)(((uint)in_RB << 0x1d) >> 0x18);
  bVar7 = (byte)((((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4) & 0xf0) >> 4);
  bVar6 = (byte)((((uint)in_Q << 8 | (uint)in_M << 9 | (uint)in_FD << 0xf) & 0x8000) >> 0xf);
  bVar5 = (bVar5 & 0x10) >> 4;
  bVar4 = (bVar3 & 0x20) >> 5;
  bVar3 = ((bVar3 | (byte)(((uint)in_MD << 0x1e) >> 0x18)) & 0x40) >> 6;
  uVar1 = *(uint *)(param_1 + DAT_8ceb12e4 + 0x20);
  *(uint *)(param_1 + DAT_8ceb12e4 + 0x20) = uVar1 + (int)DAT_8ceb12e6;
  uVar2 = uVar1 >> 0x18 & 0x1c;
  *DAT_8ceb12ec = uVar2;
  *DAT_8ceb12f0 = uVar2;
  (*(code *)PTR_callback_candidate_8ceb2028_8ceb1300)
            (uVar1 & DAT_8ceb12f4 | DAT_8ceb12f8,*(undefined4 *)(DAT_8ceb12e8 + param_1));
  return ((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4) & 0xf0 |
         ((uint)extraout_T | (uint)extraout_S << 1 | (uint)bVar7 << 4 | (uint)extraout_Q << 8 |
          (uint)extraout_M << 9 | (uint)bVar6 << 0xf | (uint)bVar5 << 0x1c | (uint)bVar4 << 0x1d |
         (uint)bVar3 << 0x1e) & (int)DAT_8ceb12e2;
}


/* Address: 8ceb1304 */

void callback_candidate_8ceb1304(int param_1,int param_2)

{
  short sVar1;
  int iVar2;
  undefined *puVar3;
  undefined4 uVar4;
  int *piVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int local_28;
  int local_24;
  
  puVar3 = PTR_FUN_8ceb1498;
  piVar5 = (int *)(DAT_8ceb148e + param_1);
  sVar1 = *(short *)((int)piVar5 + 0xe);
  iVar2 = piVar5[3];
  iVar7 = DAT_8ceb148c + param_1;
  iVar8 = DAT_8ceb1490 + param_1;
  iVar6 = piVar5[8];
  piVar5[8] = param_2 * 0x180 + iVar6 + (int)DAT_8ceb1492;
  while (param_2 = param_2 + -1, param_2 != 0) {
    FUN_8ceb1452(param_1,*(int *)(DAT_8ceb1494 + param_1) - param_2,&local_28,piVar5);
    *(int *)(iVar8 + 0x1c) = (int)(short)iVar2;
    *(int *)(iVar8 + 0x14) = iVar6;
    *(int *)(iVar8 + 0x24) = *piVar5 + local_28;
    uVar4 = (*(code *)puVar3)(iVar8,iVar7);
    *(undefined4 *)(iVar8 + 0x14) = uVar4;
    *(int *)(iVar8 + 0x24) = piVar5[1] + local_28;
    uVar4 = (*(code *)puVar3)(iVar8,iVar7 + 0x40);
    *(int *)(iVar8 + 0x1c) = (int)sVar1;
    *(undefined4 *)(iVar8 + 0x14) = uVar4;
    *(int *)(iVar8 + 0x24) = piVar5[2] + local_24;
    uVar4 = (*(code *)puVar3)(iVar8,iVar7 + 0x80);
    *(undefined4 *)(iVar8 + 0x14) = uVar4;
    *(int *)(iVar8 + 0x24) = *(int *)(iVar8 + 0x24) + 8;
    uVar4 = (*(code *)puVar3)(iVar8,iVar7 + 0xc0);
    *(undefined4 *)(iVar8 + 0x14) = uVar4;
    *(int *)(iVar8 + 0x24) = *(int *)(iVar8 + 0x24) + (sVar1 + -1) * 8;
    uVar4 = (*(code *)puVar3)(iVar8,iVar7 + 0x100);
    *(undefined4 *)(iVar8 + 0x14) = uVar4;
    *(int *)(iVar8 + 0x24) = *(int *)(iVar8 + 0x24) + 8;
    iVar6 = (*(code *)puVar3)(iVar8,iVar7 + 0x140);
    iVar7 = DAT_8ceb148c + param_1;
    (*(code *)PTR_FUN_8ceb149c)(iVar7,&local_28,piVar5 + 4);
  }
  return;
}


/* Address: 8ceb1410 */

void callback_candidate_8ceb1410(int param_1,int param_2)

{
  int iVar1;
  code *pcVar2;
  int iVar3;
  
  iVar1 = (int)DAT_8ceb1494;
  iVar3 = *(int *)(iVar1 + param_1);
  *(undefined4 *)(iVar1 + 0xc + param_1) = 0;
  param_2 = iVar3 - param_2;
  pcVar2 = *(code **)(iVar1 + -0x5c + param_1);
  while (param_2 = param_2 + 1, param_2 < iVar3) {
    *(int *)(DAT_8ceb1494 + param_1) = param_2;
    (*pcVar2)(param_1);
  }
  *(int *)(DAT_8ceb1494 + param_1) = iVar3;
  return;
}


/* Address: 8ceb1452 */

void FUN_8ceb1452(int param_1,int param_2,int *param_3,int param_4)

{
  short sVar1;
  int iVar2;
  int iVar3;
  char *pcVar4;
  
  pcVar4 = (char *)(param_2 * 2 + DAT_8ceb1496 + param_1);
  iVar3 = (int)*pcVar4;
  sVar1 = *(short *)(param_4 + 0xe);
  iVar2 = (int)pcVar4[1];
  *param_3 = iVar3 * 8 * (int)*(short *)(param_4 + 0xc) + iVar2 * 8;
  param_3[1] = iVar3 * 0x10 * (int)sVar1 + iVar2 * 0x10;
  return;
}


/* Address: 8ceb14a0 */

void FUN_8ceb14a0(uint param_1,uint param_2,int *param_3)

{
  *param_3 = (int)param_1 >> 1;
  param_3[1] = (int)param_2 >> 1;
  param_3[2] = param_1 & 1;
  param_3[3] = param_2 & 1;
  return;
}


/* Address: 8ceb1504 */

/* WARNING: Restarted to delay deadcode elimination for space: register */
/* WARNING: Restarted to delay deadcode elimination for space: stack */

void callback_candidate_8ceb1504
               (double param_1,double param_2,double param_3,double param_4,int param_5)

{
  uint uVar1;
  short sVar2;
  double *pdVar3;
  double *pdVar4;
  short *psVar5;
  double *pdVar6;
  double *pdVar7;
  double *pdVar8;
  int iVar9;
  uint uVar10;
  int iVar11;
  float fVar12;
  double in_dr0;
  double dVar13;
  float fVar14;
  float fVar15;
  double in_dr2;
  double dVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  float fVar20;
  double dVar21;
  double dVar22;
  float fVar23;
  float fVar24;
  float fVar25;
  double dVar26;
  float fVar27;
  float fVar28;
  float fVar29;
  double dVar30;
  float fVar31;
  float fVar32;
  float fVar33;
  double in_dr12;
  double dVar34;
  double in_dr14;
  double dVar35;
  double in_xd0;
  double in_xd2;
  double in_xd4;
  float fVar36;
  double in_xd6;
  float fVar37;
  double in_xd8;
  double in_xd10;
  double in_xd12;
  float fVar38;
  float fVar39;
  double in_xd14;
  float fVar40;
  short sVar41;
  short sVar42;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  pdVar3 = *(double **)(param_5 + 0x40);
  if (in_FPSCR_SZ == '\0') {
    in_dr0 = (double)(ulonglong)*(uint *)pdVar3;
    in_dr2 = (double)(ulonglong)*(uint *)((int)pdVar3 + 4);
    param_1 = (double)(ulonglong)*(uint *)(pdVar3 + 1);
    param_2 = (double)(ulonglong)*(uint *)((int)pdVar3 + 0xc);
    param_3 = (double)(ulonglong)*(uint *)(pdVar3 + 2);
    param_4 = (double)(ulonglong)*(uint *)((int)pdVar3 + 0x14);
    in_dr12 = (double)(ulonglong)*(uint *)(pdVar3 + 3);
    in_dr14 = (double)(ulonglong)*(uint *)((int)pdVar3 + 0x1c);
  }
  else {
    in_xd0 = *pdVar3;
    in_xd2 = pdVar3[1];
    in_xd4 = pdVar3[2];
    in_xd6 = pdVar3[3];
    in_xd8 = pdVar3[4];
    in_xd10 = pdVar3[5];
    in_xd12 = pdVar3[6];
    in_xd14 = pdVar3[7];
  }
  pdVar6 = *(double **)(param_5 + 0x2c);
  iVar11 = 6;
  uVar10 = *(uint *)(param_5 + 0x28);
  pdVar3 = pdVar6 + 2;
  iVar9 = *(int *)(param_5 + 0x24);
  pdVar7 = pdVar6 + 4;
  pdVar4 = *(double **)(param_5 + 0x44);
  pdVar8 = pdVar6 + 6;
  do {
    uVar1 = uVar10 & 0x80000000;
    uVar10 = uVar10 << 1;
    if (uVar1 != 0) {
      if (in_FPSCR_SZ == '\0') {
        dVar13 = (double)CONCAT44(*(undefined4 *)pdVar6,SUB84(in_dr0,0));
        dVar16 = (double)CONCAT44(*(undefined4 *)((int)pdVar6 + 4),SUB84(in_dr2,0));
        pdVar6 = pdVar6 + 1;
      }
      else {
        dVar13 = *pdVar6;
        dVar16 = pdVar6[1];
        pdVar6 = pdVar6 + 2;
      }
                    /* WARNING: Ignoring partial resolution of indirect */
      fVar33 = SUB84(in_xd0,0);
      fVar19 = (float)((ulonglong)in_xd0 >> 0x20);
      fVar25 = SUB84(in_xd2,0);
      fVar28 = (float)((ulonglong)in_xd2 >> 0x20);
      fVar29 = SUB84(in_xd4,0);
      fVar32 = (float)((ulonglong)in_xd4 >> 0x20);
      fVar36 = SUB84(in_xd6,0);
      fVar37 = (float)((ulonglong)in_xd6 >> 0x20);
      fVar23 = SUB84(in_xd8,0);
      fVar18 = (float)((ulonglong)in_xd8 >> 0x20);
      fVar24 = SUB84(in_xd10,0);
      fVar27 = (float)((ulonglong)in_xd10 >> 0x20);
      fVar31 = SUB84(in_xd12,0);
      fVar38 = (float)((ulonglong)in_xd12 >> 0x20);
      fVar39 = SUB84(in_xd14,0);
      fVar40 = (float)((ulonglong)in_xd14 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar20 = SUB84(dVar13,0);
        fVar14 = (float)((ulonglong)dVar13 >> 0x20);
        fVar15 = SUB84(dVar16,0);
        fVar17 = (float)((ulonglong)dVar16 >> 0x20);
        fVar12 = fVar33 * fVar20 + fVar29 * fVar14 + fVar23 * fVar15 + fVar31 * fVar17;
        fVar12 = fVar19 * fVar20 + fVar32 * fVar14 + fVar18 * fVar15 + fVar38 * fVar17;
        fVar12 = fVar25 * fVar20 + fVar36 * fVar14 + fVar24 * fVar15 + fVar39 * fVar17;
        fVar12 = fVar28 * fVar20 + fVar37 * fVar14 + fVar27 * fVar15 + fVar40 * fVar17;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar21 = (double)CONCAT44(*(undefined4 *)pdVar3,SUB84(param_1,0));
        dVar22 = (double)CONCAT44(*(undefined4 *)((int)pdVar3 + 4),SUB84(param_2,0));
        pdVar3 = pdVar3 + 1;
        dVar26 = (double)CONCAT44(*(undefined4 *)pdVar7,SUB84(param_3,0));
        dVar30 = (double)CONCAT44(*(undefined4 *)((int)pdVar7 + 4),SUB84(param_4,0));
        pdVar7 = pdVar7 + 1;
      }
      else {
        dVar21 = *pdVar3;
        dVar22 = pdVar3[1];
        pdVar3 = pdVar3 + 2;
        dVar26 = *pdVar7;
        dVar30 = pdVar7[1];
        pdVar7 = pdVar7 + 2;
      }
                    /* WARNING: Ignoring partial resolution of indirect */
      if (in_FPSCR_PR == '\0') {
        fVar20 = SUB84(dVar21,0);
        fVar14 = (float)((ulonglong)dVar21 >> 0x20);
        fVar15 = SUB84(dVar22,0);
        fVar17 = (float)((ulonglong)dVar22 >> 0x20);
        fVar12 = fVar33 * fVar20 + fVar29 * fVar14 + fVar23 * fVar15 + fVar31 * fVar17;
        fVar12 = fVar19 * fVar20 + fVar32 * fVar14 + fVar18 * fVar15 + fVar38 * fVar17;
        fVar12 = fVar25 * fVar20 + fVar36 * fVar14 + fVar24 * fVar15 + fVar39 * fVar17;
        fVar12 = fVar28 * fVar20 + fVar37 * fVar14 + fVar27 * fVar15 + fVar40 * fVar17;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar6 + -4) = (int)((ulonglong)dVar13 >> 0x20);
        dVar34 = (double)CONCAT44(*(undefined4 *)pdVar8,SUB84(in_dr12,0));
        pdVar6 = pdVar6 + -1;
        *(int *)pdVar6 = (int)((ulonglong)dVar16 >> 0x20);
        dVar35 = (double)CONCAT44(*(undefined4 *)((int)pdVar8 + 4),SUB84(in_dr14,0));
        pdVar8 = pdVar8 + 1;
      }
      else {
        pdVar6[-1] = dVar13;
        dVar34 = *pdVar8;
        pdVar6 = pdVar6 + -2;
        *pdVar6 = dVar16;
        dVar35 = pdVar8[1];
        pdVar8 = pdVar8 + 2;
      }
                    /* WARNING: Ignoring partial resolution of indirect */
      if (in_FPSCR_PR == '\0') {
        fVar20 = SUB84(dVar26,0);
        fVar14 = (float)((ulonglong)dVar26 >> 0x20);
        fVar15 = SUB84(dVar30,0);
        fVar17 = (float)((ulonglong)dVar30 >> 0x20);
        fVar12 = fVar33 * fVar20 + fVar29 * fVar14 + fVar23 * fVar15 + fVar31 * fVar17;
        fVar12 = fVar19 * fVar20 + fVar32 * fVar14 + fVar18 * fVar15 + fVar38 * fVar17;
        fVar12 = fVar25 * fVar20 + fVar36 * fVar14 + fVar24 * fVar15 + fVar39 * fVar17;
        fVar12 = fVar28 * fVar20 + fVar37 * fVar14 + fVar27 * fVar15 + fVar40 * fVar17;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar3 + -4) = (int)((ulonglong)dVar21 >> 0x20);
        in_dr0 = (double)CONCAT44(*(undefined4 *)(pdVar6 + 8),SUB84(dVar13,0));
        pdVar3 = pdVar3 + -1;
        *(int *)pdVar3 = (int)((ulonglong)dVar22 >> 0x20);
        in_dr2 = (double)CONCAT44(*(undefined4 *)((int)pdVar6 + 0x44),SUB84(dVar16,0));
        pdVar6 = pdVar6 + 9;
      }
      else {
        pdVar3[-1] = dVar21;
        in_dr0 = pdVar6[8];
        pdVar3 = pdVar3 + -2;
        *pdVar3 = dVar22;
        in_dr2 = pdVar6[9];
        pdVar6 = pdVar6 + 10;
      }
                    /* WARNING: Ignoring partial resolution of indirect */
      if (in_FPSCR_PR == '\0') {
        fVar20 = SUB84(dVar34,0);
        fVar14 = (float)((ulonglong)dVar34 >> 0x20);
        fVar15 = SUB84(dVar35,0);
        fVar17 = (float)((ulonglong)dVar35 >> 0x20);
        fVar12 = fVar33 * fVar20 + fVar29 * fVar14 + fVar23 * fVar15 + fVar31 * fVar17;
        fVar12 = fVar19 * fVar20 + fVar32 * fVar14 + fVar18 * fVar15 + fVar38 * fVar17;
        fVar12 = fVar25 * fVar20 + fVar36 * fVar14 + fVar24 * fVar15 + fVar39 * fVar17;
        fVar12 = fVar28 * fVar20 + fVar37 * fVar14 + fVar27 * fVar15 + fVar40 * fVar17;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar7 + -4) = (int)((ulonglong)dVar26 >> 0x20);
        param_1 = (double)CONCAT44(*(undefined4 *)(pdVar3 + 8),SUB84(dVar21,0));
        pdVar7 = pdVar7 + -1;
        *(int *)pdVar7 = (int)((ulonglong)dVar30 >> 0x20);
        param_2 = (double)CONCAT44(*(undefined4 *)((int)pdVar3 + 0x44),SUB84(dVar22,0));
        pdVar3 = pdVar3 + 9;
      }
      else {
        pdVar7[-1] = dVar26;
        param_1 = pdVar3[8];
        pdVar7 = pdVar7 + -2;
        *pdVar7 = dVar30;
        param_2 = pdVar3[9];
        pdVar3 = pdVar3 + 10;
      }
                    /* WARNING: Ignoring partial resolution of indirect */
      if (in_FPSCR_PR == '\0') {
        fVar20 = SUB84(in_dr0,0);
        fVar14 = (float)((ulonglong)in_dr0 >> 0x20);
        fVar15 = SUB84(in_dr2,0);
        fVar17 = (float)((ulonglong)in_dr2 >> 0x20);
        fVar12 = fVar33 * fVar20 + fVar29 * fVar14 + fVar23 * fVar15 + fVar31 * fVar17;
        fVar12 = fVar19 * fVar20 + fVar32 * fVar14 + fVar18 * fVar15 + fVar38 * fVar17;
        fVar12 = fVar25 * fVar20 + fVar36 * fVar14 + fVar24 * fVar15 + fVar39 * fVar17;
        fVar12 = fVar28 * fVar20 + fVar37 * fVar14 + fVar27 * fVar15 + fVar40 * fVar17;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar8 + -4) = (int)((ulonglong)dVar34 >> 0x20);
        param_3 = (double)CONCAT44(*(undefined4 *)(pdVar7 + 8),SUB84(dVar26,0));
        pdVar8 = pdVar8 + -1;
        *(int *)pdVar8 = (int)((ulonglong)dVar35 >> 0x20);
        param_4 = (double)CONCAT44(*(undefined4 *)((int)pdVar7 + 0x44),SUB84(dVar30,0));
        pdVar7 = pdVar7 + 9;
      }
      else {
        pdVar8[-1] = dVar34;
        param_3 = pdVar7[8];
        pdVar8 = pdVar8 + -2;
        *pdVar8 = dVar35;
        param_4 = pdVar7[9];
        pdVar7 = pdVar7 + 10;
      }
                    /* WARNING: Ignoring partial resolution of indirect */
      if (in_FPSCR_PR == '\0') {
        fVar12 = SUB84(param_1,0);
        fVar14 = (float)((ulonglong)param_1 >> 0x20);
        fVar15 = SUB84(param_2,0);
        fVar17 = (float)((ulonglong)param_2 >> 0x20);
        fVar20 = fVar33 * fVar12 + fVar29 * fVar14 + fVar23 * fVar15 + fVar31 * fVar17;
        fVar20 = fVar19 * fVar12 + fVar32 * fVar14 + fVar18 * fVar15 + fVar38 * fVar17;
        fVar20 = fVar25 * fVar12 + fVar36 * fVar14 + fVar24 * fVar15 + fVar39 * fVar17;
        fVar12 = fVar28 * fVar12 + fVar37 * fVar14 + fVar27 * fVar15 + fVar40 * fVar17;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar6 + -4) = (int)((ulonglong)in_dr0 >> 0x20);
        in_dr12 = (double)CONCAT44(*(undefined4 *)(pdVar8 + 8),SUB84(dVar34,0));
        pdVar6 = pdVar6 + -1;
        *(int *)pdVar6 = (int)((ulonglong)in_dr2 >> 0x20);
        in_dr14 = (double)CONCAT44(*(undefined4 *)((int)pdVar8 + 0x44),SUB84(dVar35,0));
        pdVar8 = pdVar8 + 9;
      }
      else {
        pdVar6[-1] = in_dr0;
        in_dr12 = pdVar8[8];
        pdVar6 = pdVar6 + -2;
        *pdVar6 = in_dr2;
        in_dr14 = pdVar8[9];
        pdVar8 = pdVar8 + 10;
      }
      pdVar6 = pdVar6 + -8;
                    /* WARNING: Ignoring partial resolution of indirect */
      if (in_FPSCR_PR == '\0') {
        fVar20 = SUB84(param_3,0);
        fVar14 = (float)((ulonglong)param_3 >> 0x20);
        fVar15 = SUB84(param_4,0);
        fVar17 = (float)((ulonglong)param_4 >> 0x20);
        fVar12 = fVar33 * fVar20 + fVar29 * fVar14 + fVar23 * fVar15 + fVar31 * fVar17;
        fVar12 = fVar19 * fVar20 + fVar32 * fVar14 + fVar18 * fVar15 + fVar38 * fVar17;
        fVar12 = fVar25 * fVar20 + fVar36 * fVar14 + fVar24 * fVar15 + fVar39 * fVar17;
        fVar12 = fVar28 * fVar20 + fVar37 * fVar14 + fVar27 * fVar15 + fVar40 * fVar17;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar3 + -4) = (int)((ulonglong)param_1 >> 0x20);
        pdVar3 = pdVar3 + -1;
        *(int *)pdVar3 = (int)((ulonglong)param_2 >> 0x20);
      }
      else {
        pdVar3[-1] = param_1;
        pdVar3 = pdVar3 + -2;
        *pdVar3 = param_2;
      }
      pdVar3 = pdVar3 + -8;
                    /* WARNING: Ignoring partial resolution of indirect */
      if (in_FPSCR_PR == '\0') {
        fVar12 = SUB84(in_dr12,0);
        fVar20 = (float)((ulonglong)in_dr12 >> 0x20);
        fVar14 = SUB84(in_dr14,0);
        fVar15 = (float)((ulonglong)in_dr14 >> 0x20);
        fVar33 = fVar33 * fVar12 + fVar29 * fVar20 + fVar23 * fVar14 + fVar31 * fVar15;
        fVar33 = fVar19 * fVar12 + fVar32 * fVar20 + fVar18 * fVar14 + fVar38 * fVar15;
        fVar33 = fVar25 * fVar12 + fVar36 * fVar20 + fVar24 * fVar14 + fVar39 * fVar15;
        fVar33 = fVar28 * fVar12 + fVar37 * fVar20 + fVar27 * fVar14 + fVar40 * fVar15;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar7 + -4) = (int)((ulonglong)param_3 >> 0x20);
        pdVar7 = pdVar7 + -1;
        *(int *)pdVar7 = (int)((ulonglong)param_4 >> 0x20);
      }
      else {
        pdVar7[-1] = param_3;
        pdVar7 = pdVar7 + -2;
        *pdVar7 = param_4;
      }
      pdVar7 = pdVar7 + -8;
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar8 + -4) = (int)((ulonglong)in_dr12 >> 0x20);
        pdVar8 = pdVar8 + -1;
        *(int *)pdVar8 = (int)((ulonglong)in_dr14 >> 0x20);
      }
      else {
        pdVar8[-1] = in_dr12;
        pdVar8 = pdVar8 + -2;
        *pdVar8 = in_dr14;
      }
      pdVar8 = pdVar8 + -8;
    }
    iVar11 = iVar11 + -1;
    pdVar6 = (double *)((int)pdVar6 + iVar9);
    pdVar3 = (double *)((int)pdVar3 + iVar9);
    pdVar7 = (double *)((int)pdVar7 + iVar9);
    pdVar8 = (double *)((int)pdVar8 + iVar9);
  } while (iVar11 != 0);
  if (in_FPSCR_SZ == '\0') {
    in_dr0 = (double)(ulonglong)*(uint *)pdVar4;
    in_dr2 = (double)(ulonglong)*(uint *)((int)pdVar4 + 4);
    param_1 = (double)(ulonglong)*(uint *)(pdVar4 + 1);
    param_2 = (double)(ulonglong)*(uint *)((int)pdVar4 + 0xc);
    param_3 = (double)(ulonglong)*(uint *)(pdVar4 + 2);
    param_4 = (double)(ulonglong)*(uint *)((int)pdVar4 + 0x14);
    in_dr12 = (double)(ulonglong)*(uint *)(pdVar4 + 3);
    in_dr14 = (double)(ulonglong)*(uint *)((int)pdVar4 + 0x1c);
  }
  else {
    in_xd0 = *pdVar4;
    in_xd2 = pdVar4[1];
    in_xd4 = pdVar4[2];
    in_xd6 = pdVar4[3];
    in_xd8 = pdVar4[4];
    in_xd10 = pdVar4[5];
    in_xd12 = pdVar4[6];
    in_xd14 = pdVar4[7];
  }
  uVar10 = *(uint *)(param_5 + 0x28);
  iVar9 = 6;
  pdVar3 = *(double **)(param_5 + 0x2c);
  pdVar7 = pdVar3 + 0x10;
  pdVar6 = *(double **)(param_5 + 0x48);
  do {
    uVar1 = uVar10 & 0x80000000;
    uVar10 = uVar10 << 1;
    if (uVar1 != 0) {
      iVar11 = 4;
      if (in_FPSCR_SZ == '\0') {
        param_3 = (double)CONCAT44(*(undefined4 *)pdVar7,SUB84(param_3,0));
        param_4 = (double)CONCAT44(*(undefined4 *)((int)pdVar7 + 4),SUB84(param_4,0));
        pdVar7 = pdVar7 + 1;
      }
      else {
        param_3 = *pdVar7;
        param_4 = pdVar7[1];
        pdVar7 = pdVar7 + 2;
      }
      do {
        pdVar8 = pdVar6;
                    /* WARNING: Ignoring partial resolution of indirect */
        fVar33 = (float)((ulonglong)in_xd0 >> 0x20);
        fVar19 = (float)((ulonglong)in_xd2 >> 0x20);
        fVar25 = (float)((ulonglong)in_xd4 >> 0x20);
        fVar28 = (float)((ulonglong)in_xd6 >> 0x20);
        fVar29 = (float)((ulonglong)in_xd8 >> 0x20);
        fVar32 = (float)((ulonglong)in_xd10 >> 0x20);
        fVar36 = (float)((ulonglong)in_xd12 >> 0x20);
        fVar37 = (float)((ulonglong)in_xd14 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          fVar18 = SUB84(param_3,0);
          fVar24 = (float)((ulonglong)param_3 >> 0x20);
          fVar27 = SUB84(param_4,0);
          fVar31 = (float)((ulonglong)param_4 >> 0x20);
          fVar23 = SUB84(in_xd0,0) * fVar18 + SUB84(in_xd4,0) * fVar24 + SUB84(in_xd8,0) * fVar27 +
                   SUB84(in_xd12,0) * fVar31;
          fVar23 = fVar33 * fVar18 + fVar25 * fVar24 + fVar29 * fVar27 + fVar36 * fVar31;
          fVar23 = SUB84(in_xd2,0) * fVar18 + SUB84(in_xd6,0) * fVar24 + SUB84(in_xd10,0) * fVar27 +
                   SUB84(in_xd14,0) * fVar31;
          fVar23 = fVar19 * fVar18 + fVar28 * fVar24 + fVar32 * fVar27 + fVar37 * fVar31;
        }
        if (in_FPSCR_SZ == '\0') {
          fVar18 = *(float *)pdVar3;
          fVar23 = *(float *)((int)pdVar3 + 4);
          pdVar3 = pdVar3 + 1;
          param_2 = (double)CONCAT44(fVar18,SUB84(param_2,0));
          param_1 = (double)CONCAT44(fVar23,SUB84(param_1,0));
          dVar13 = (double)CONCAT44(fVar23,SUB84(in_dr0,0));
          dVar16 = (double)CONCAT44(fVar18,SUB84(in_dr2,0));
        }
        else {
          param_2 = *pdVar3;
          param_1 = pdVar3[1];
          pdVar3 = pdVar3 + 2;
          fVar18 = (float)((ulonglong)param_2 >> 0x20);
          fVar23 = (float)((ulonglong)param_1 >> 0x20);
          dVar13 = param_1;
          dVar16 = param_2;
        }
        fVar27 = (float)((ulonglong)param_3 >> 0x20);
        fVar24 = SUB84(param_3,0);
        fVar31 = (float)((ulonglong)param_4 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          in_dr0 = (double)CONCAT44(fVar23 + fVar27,SUB84(dVar13,0) + fVar24);
          dVar16 = (double)CONCAT44(fVar18 + fVar31,SUB84(dVar16,0));
        }
        else {
          in_dr0 = dVar13 + param_3 + param_3;
          dVar16 = dVar16 + param_4;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)pdVar8 = (int)((ulonglong)in_dr0 >> 0x20);
        }
        else {
          *pdVar8 = in_dr0;
        }
        fVar23 = SUB84(param_4,0);
        if (in_FPSCR_PR == '\0') {
          in_dr2 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),SUB84(dVar16,0) + fVar23);
        }
        else {
          in_dr2 = dVar16 + param_4;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(pdVar8 + 4) = SUB84(in_dr0,0);
        }
        else {
          pdVar8[4] = in_xd0;
        }
        if (in_FPSCR_PR == '\0') {
          param_1 = (double)CONCAT44((float)((ulonglong)param_1 >> 0x20) - fVar27,SUB84(param_1,0));
        }
        else {
          param_1 = param_1 - param_3;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(pdVar8 + 8) = (int)((ulonglong)in_dr2 >> 0x20);
        }
        else {
          pdVar8[8] = in_dr2;
        }
        if (in_FPSCR_PR == '\0') {
          param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),SUB84(param_1,0) - fVar24);
        }
        else {
          param_1 = param_1 - param_3;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(pdVar8 + 0xc) = SUB84(in_dr2,0);
        }
        else {
          pdVar8[0xc] = in_xd2;
        }
        if (in_FPSCR_PR == '\0') {
          param_2 = (double)CONCAT44((float)((ulonglong)param_2 >> 0x20) - fVar31,SUB84(param_2,0));
        }
        else {
          param_2 = param_2 - param_4;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(pdVar8 + 0xe) = (int)((ulonglong)param_1 >> 0x20);
        }
        else {
          pdVar8[0xe] = param_1;
        }
        if (in_FPSCR_PR == '\0') {
          param_2 = (double)CONCAT44((int)((ulonglong)param_2 >> 0x20),SUB84(param_2,0) - fVar23);
        }
        else {
          param_2 = param_2 - param_4;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(pdVar8 + 10) = SUB84(param_1,0);
          *(int *)(pdVar8 + 6) = (int)((ulonglong)param_2 >> 0x20);
        }
        else {
          pdVar8[10] = in_xd4;
          pdVar8[6] = param_2;
        }
        iVar11 = iVar11 + -1;
        if (in_FPSCR_SZ == '\0') {
          *(int *)(pdVar8 + 2) = SUB84(param_2,0);
          param_3 = (double)CONCAT44(*(undefined4 *)pdVar7,fVar24);
          param_4 = (double)CONCAT44(*(undefined4 *)((int)pdVar7 + 4),fVar23);
          pdVar7 = pdVar7 + 1;
        }
        else {
          pdVar8[2] = in_xd6;
          param_3 = *pdVar7;
          param_4 = pdVar7[1];
          pdVar7 = pdVar7 + 2;
        }
        pdVar6 = (double *)((int)pdVar8 + 4);
      } while (iVar11 != 0);
      iVar11 = 4;
      pdVar8 = (double *)((int)pdVar8 + 0x74);
      do {
        pdVar6 = pdVar8;
                    /* WARNING: Ignoring partial resolution of indirect */
        if (in_FPSCR_PR == '\0') {
          fVar23 = SUB84(param_3,0);
          fVar24 = (float)((ulonglong)param_3 >> 0x20);
          fVar27 = SUB84(param_4,0);
          fVar31 = (float)((ulonglong)param_4 >> 0x20);
          fVar18 = SUB84(in_xd0,0) * fVar23 + SUB84(in_xd4,0) * fVar24 + SUB84(in_xd8,0) * fVar27 +
                   SUB84(in_xd12,0) * fVar31;
          fVar18 = fVar33 * fVar23 + fVar25 * fVar24 + fVar29 * fVar27 + fVar36 * fVar31;
          fVar18 = SUB84(in_xd2,0) * fVar23 + SUB84(in_xd6,0) * fVar24 + SUB84(in_xd10,0) * fVar27 +
                   SUB84(in_xd14,0) * fVar31;
          fVar23 = fVar19 * fVar23 + fVar28 * fVar24 + fVar32 * fVar27 + fVar37 * fVar31;
        }
        if (in_FPSCR_SZ == '\0') {
          fVar18 = *(float *)pdVar3;
          fVar23 = *(float *)((int)pdVar3 + 4);
          pdVar3 = pdVar3 + 1;
          param_2 = (double)CONCAT44(fVar18,SUB84(param_2,0));
          param_1 = (double)CONCAT44(fVar23,SUB84(param_1,0));
          dVar13 = (double)CONCAT44(fVar23,SUB84(in_dr0,0));
          dVar16 = (double)CONCAT44(fVar18,SUB84(in_dr2,0));
        }
        else {
          param_2 = *pdVar3;
          param_1 = pdVar3[1];
          pdVar3 = pdVar3 + 2;
          fVar18 = (float)((ulonglong)param_2 >> 0x20);
          fVar23 = (float)((ulonglong)param_1 >> 0x20);
          dVar13 = param_1;
          dVar16 = param_2;
        }
        fVar27 = (float)((ulonglong)param_3 >> 0x20);
        fVar24 = SUB84(param_3,0);
        fVar31 = (float)((ulonglong)param_4 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          in_dr0 = (double)CONCAT44(fVar23 + fVar27,SUB84(dVar13,0) + fVar24);
          dVar16 = (double)CONCAT44(fVar18 + fVar31,SUB84(dVar16,0));
        }
        else {
          in_dr0 = dVar13 + param_3 + param_3;
          dVar16 = dVar16 + param_4;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)pdVar6 = (int)((ulonglong)in_dr0 >> 0x20);
        }
        else {
          *pdVar6 = in_dr0;
        }
        fVar23 = SUB84(param_4,0);
        if (in_FPSCR_PR == '\0') {
          in_dr2 = (double)CONCAT44((int)((ulonglong)dVar16 >> 0x20),SUB84(dVar16,0) + fVar23);
        }
        else {
          in_dr2 = dVar16 + param_4;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(pdVar6 + 4) = SUB84(in_dr0,0);
        }
        else {
          pdVar6[4] = in_xd0;
        }
        if (in_FPSCR_PR == '\0') {
          param_1 = (double)CONCAT44((float)((ulonglong)param_1 >> 0x20) - fVar27,SUB84(param_1,0));
        }
        else {
          param_1 = param_1 - param_3;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(pdVar6 + 8) = (int)((ulonglong)in_dr2 >> 0x20);
        }
        else {
          pdVar6[8] = in_dr2;
        }
        if (in_FPSCR_PR == '\0') {
          param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),SUB84(param_1,0) - fVar24);
        }
        else {
          param_1 = param_1 - param_3;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(pdVar6 + 0xc) = SUB84(in_dr2,0);
        }
        else {
          pdVar6[0xc] = in_xd2;
        }
        if (in_FPSCR_PR == '\0') {
          param_2 = (double)CONCAT44((float)((ulonglong)param_2 >> 0x20) - fVar31,SUB84(param_2,0));
        }
        else {
          param_2 = param_2 - param_4;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(pdVar6 + 0xe) = (int)((ulonglong)param_1 >> 0x20);
        }
        else {
          pdVar6[0xe] = param_1;
        }
        if (in_FPSCR_PR == '\0') {
          param_2 = (double)CONCAT44((int)((ulonglong)param_2 >> 0x20),SUB84(param_2,0) - fVar23);
        }
        else {
          param_2 = param_2 - param_4;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(pdVar6 + 10) = SUB84(param_1,0);
          *(int *)(pdVar6 + 6) = (int)((ulonglong)param_2 >> 0x20);
        }
        else {
          pdVar6[10] = in_xd4;
          pdVar6[6] = param_2;
        }
        iVar11 = iVar11 + -1;
        if (in_FPSCR_SZ == '\0') {
          *(int *)(pdVar6 + 2) = SUB84(param_2,0);
          param_3 = (double)CONCAT44(*(undefined4 *)pdVar7,fVar24);
          param_4 = (double)CONCAT44(*(undefined4 *)((int)pdVar7 + 4),fVar23);
          pdVar7 = pdVar7 + 1;
        }
        else {
          pdVar6[2] = in_xd6;
          param_3 = *pdVar7;
          param_4 = pdVar7[1];
          pdVar7 = pdVar7 + 2;
        }
        pdVar8 = (double *)((int)pdVar6 + 4);
      } while (iVar11 != 0);
      pdVar7 = pdVar7 + -0x12;
      pdVar6 = (double *)((int)pdVar6 + -0x8c);
      pdVar3 = pdVar3 + -0x10;
    }
    iVar9 = iVar9 + -1;
    pdVar6 = pdVar6 + 0x20;
    pdVar3 = pdVar3 + 0x20;
    pdVar7 = pdVar7 + 0x20;
  } while (iVar9 != 0);
  pdVar3 = *(double **)(param_5 + 0x40);
  if (in_FPSCR_SZ == '\0') {
    in_dr0 = (double)(ulonglong)*(uint *)pdVar3;
    in_dr2 = (double)(ulonglong)*(uint *)((int)pdVar3 + 4);
    param_1 = (double)(ulonglong)*(uint *)(pdVar3 + 1);
    param_2 = (double)(ulonglong)*(uint *)((int)pdVar3 + 0xc);
    param_3 = (double)(ulonglong)*(uint *)(pdVar3 + 2);
    param_4 = (double)(ulonglong)*(uint *)((int)pdVar3 + 0x14);
    in_dr12 = (double)(ulonglong)*(uint *)(pdVar3 + 3);
    in_dr14 = (double)(ulonglong)*(uint *)((int)pdVar3 + 0x1c);
  }
  else {
    in_xd0 = *pdVar3;
    in_xd2 = pdVar3[1];
    in_xd4 = pdVar3[2];
    in_xd6 = pdVar3[3];
    in_xd8 = pdVar3[4];
    in_xd10 = pdVar3[5];
    in_xd12 = pdVar3[6];
    in_xd14 = pdVar3[7];
  }
  pdVar6 = *(double **)(param_5 + 0x48);
  iVar11 = 6;
  uVar10 = *(uint *)(param_5 + 0x28);
  pdVar3 = pdVar6 + 2;
  iVar9 = *(int *)(param_5 + 0x24);
  pdVar7 = pdVar6 + 4;
  pdVar4 = *(double **)(param_5 + 0x44);
  pdVar8 = pdVar6 + 6;
  do {
    uVar1 = uVar10 & 0x80000000;
    uVar10 = uVar10 << 1;
    if (uVar1 != 0) {
      if (in_FPSCR_SZ == '\0') {
        dVar13 = (double)CONCAT44(*(undefined4 *)pdVar6,SUB84(in_dr0,0));
        dVar16 = (double)CONCAT44(*(undefined4 *)((int)pdVar6 + 4),SUB84(in_dr2,0));
        pdVar6 = pdVar6 + 1;
      }
      else {
        dVar13 = *pdVar6;
        dVar16 = pdVar6[1];
        pdVar6 = pdVar6 + 2;
      }
                    /* WARNING: Ignoring partial resolution of indirect */
      fVar33 = SUB84(in_xd0,0);
      fVar19 = (float)((ulonglong)in_xd0 >> 0x20);
      fVar25 = SUB84(in_xd2,0);
      fVar28 = (float)((ulonglong)in_xd2 >> 0x20);
      fVar29 = SUB84(in_xd4,0);
      fVar32 = (float)((ulonglong)in_xd4 >> 0x20);
      fVar36 = SUB84(in_xd6,0);
      fVar37 = (float)((ulonglong)in_xd6 >> 0x20);
      fVar23 = SUB84(in_xd8,0);
      fVar18 = (float)((ulonglong)in_xd8 >> 0x20);
      fVar24 = SUB84(in_xd10,0);
      fVar27 = (float)((ulonglong)in_xd10 >> 0x20);
      fVar31 = SUB84(in_xd12,0);
      fVar38 = (float)((ulonglong)in_xd12 >> 0x20);
      fVar39 = SUB84(in_xd14,0);
      fVar40 = (float)((ulonglong)in_xd14 >> 0x20);
      if (in_FPSCR_PR == '\0') {
        fVar20 = SUB84(dVar13,0);
        fVar14 = (float)((ulonglong)dVar13 >> 0x20);
        fVar15 = SUB84(dVar16,0);
        fVar17 = (float)((ulonglong)dVar16 >> 0x20);
        fVar12 = fVar33 * fVar20 + fVar29 * fVar14 + fVar23 * fVar15 + fVar31 * fVar17;
        fVar12 = fVar19 * fVar20 + fVar32 * fVar14 + fVar18 * fVar15 + fVar38 * fVar17;
        fVar12 = fVar25 * fVar20 + fVar36 * fVar14 + fVar24 * fVar15 + fVar39 * fVar17;
        fVar12 = fVar28 * fVar20 + fVar37 * fVar14 + fVar27 * fVar15 + fVar40 * fVar17;
      }
      if (in_FPSCR_SZ == '\0') {
        dVar21 = (double)CONCAT44(*(undefined4 *)pdVar3,SUB84(param_1,0));
        dVar22 = (double)CONCAT44(*(undefined4 *)((int)pdVar3 + 4),SUB84(param_2,0));
        pdVar3 = pdVar3 + 1;
        dVar26 = (double)CONCAT44(*(undefined4 *)pdVar7,SUB84(param_3,0));
        dVar30 = (double)CONCAT44(*(undefined4 *)((int)pdVar7 + 4),SUB84(param_4,0));
        pdVar7 = pdVar7 + 1;
      }
      else {
        dVar21 = *pdVar3;
        dVar22 = pdVar3[1];
        pdVar3 = pdVar3 + 2;
        dVar26 = *pdVar7;
        dVar30 = pdVar7[1];
        pdVar7 = pdVar7 + 2;
      }
                    /* WARNING: Ignoring partial resolution of indirect */
      if (in_FPSCR_PR == '\0') {
        fVar20 = SUB84(dVar21,0);
        fVar14 = (float)((ulonglong)dVar21 >> 0x20);
        fVar15 = SUB84(dVar22,0);
        fVar17 = (float)((ulonglong)dVar22 >> 0x20);
        fVar12 = fVar33 * fVar20 + fVar29 * fVar14 + fVar23 * fVar15 + fVar31 * fVar17;
        fVar12 = fVar19 * fVar20 + fVar32 * fVar14 + fVar18 * fVar15 + fVar38 * fVar17;
        fVar12 = fVar25 * fVar20 + fVar36 * fVar14 + fVar24 * fVar15 + fVar39 * fVar17;
        fVar12 = fVar28 * fVar20 + fVar37 * fVar14 + fVar27 * fVar15 + fVar40 * fVar17;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar6 + -4) = (int)((ulonglong)dVar13 >> 0x20);
        dVar34 = (double)CONCAT44(*(undefined4 *)pdVar8,SUB84(in_dr12,0));
        pdVar6 = pdVar6 + -1;
        *(int *)pdVar6 = (int)((ulonglong)dVar16 >> 0x20);
        dVar35 = (double)CONCAT44(*(undefined4 *)((int)pdVar8 + 4),SUB84(in_dr14,0));
        pdVar8 = pdVar8 + 1;
      }
      else {
        pdVar6[-1] = dVar13;
        dVar34 = *pdVar8;
        pdVar6 = pdVar6 + -2;
        *pdVar6 = dVar16;
        dVar35 = pdVar8[1];
        pdVar8 = pdVar8 + 2;
      }
                    /* WARNING: Ignoring partial resolution of indirect */
      if (in_FPSCR_PR == '\0') {
        fVar20 = SUB84(dVar26,0);
        fVar14 = (float)((ulonglong)dVar26 >> 0x20);
        fVar15 = SUB84(dVar30,0);
        fVar17 = (float)((ulonglong)dVar30 >> 0x20);
        fVar12 = fVar33 * fVar20 + fVar29 * fVar14 + fVar23 * fVar15 + fVar31 * fVar17;
        fVar12 = fVar19 * fVar20 + fVar32 * fVar14 + fVar18 * fVar15 + fVar38 * fVar17;
        fVar12 = fVar25 * fVar20 + fVar36 * fVar14 + fVar24 * fVar15 + fVar39 * fVar17;
        fVar12 = fVar28 * fVar20 + fVar37 * fVar14 + fVar27 * fVar15 + fVar40 * fVar17;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar3 + -4) = (int)((ulonglong)dVar21 >> 0x20);
        in_dr0 = (double)CONCAT44(*(undefined4 *)(pdVar6 + 8),SUB84(dVar13,0));
        pdVar3 = pdVar3 + -1;
        *(int *)pdVar3 = (int)((ulonglong)dVar22 >> 0x20);
        in_dr2 = (double)CONCAT44(*(undefined4 *)((int)pdVar6 + 0x44),SUB84(dVar16,0));
        pdVar6 = pdVar6 + 9;
      }
      else {
        pdVar3[-1] = dVar21;
        in_dr0 = pdVar6[8];
        pdVar3 = pdVar3 + -2;
        *pdVar3 = dVar22;
        in_dr2 = pdVar6[9];
        pdVar6 = pdVar6 + 10;
      }
                    /* WARNING: Ignoring partial resolution of indirect */
      if (in_FPSCR_PR == '\0') {
        fVar20 = SUB84(dVar34,0);
        fVar14 = (float)((ulonglong)dVar34 >> 0x20);
        fVar15 = SUB84(dVar35,0);
        fVar17 = (float)((ulonglong)dVar35 >> 0x20);
        fVar12 = fVar33 * fVar20 + fVar29 * fVar14 + fVar23 * fVar15 + fVar31 * fVar17;
        fVar12 = fVar19 * fVar20 + fVar32 * fVar14 + fVar18 * fVar15 + fVar38 * fVar17;
        fVar12 = fVar25 * fVar20 + fVar36 * fVar14 + fVar24 * fVar15 + fVar39 * fVar17;
        fVar12 = fVar28 * fVar20 + fVar37 * fVar14 + fVar27 * fVar15 + fVar40 * fVar17;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar7 + -4) = (int)((ulonglong)dVar26 >> 0x20);
        param_1 = (double)CONCAT44(*(undefined4 *)(pdVar3 + 8),SUB84(dVar21,0));
        pdVar7 = pdVar7 + -1;
        *(int *)pdVar7 = (int)((ulonglong)dVar30 >> 0x20);
        param_2 = (double)CONCAT44(*(undefined4 *)((int)pdVar3 + 0x44),SUB84(dVar22,0));
        pdVar3 = pdVar3 + 9;
      }
      else {
        pdVar7[-1] = dVar26;
        param_1 = pdVar3[8];
        pdVar7 = pdVar7 + -2;
        *pdVar7 = dVar30;
        param_2 = pdVar3[9];
        pdVar3 = pdVar3 + 10;
      }
                    /* WARNING: Ignoring partial resolution of indirect */
      if (in_FPSCR_PR == '\0') {
        fVar12 = SUB84(in_dr0,0);
        fVar14 = (float)((ulonglong)in_dr0 >> 0x20);
        fVar15 = SUB84(in_dr2,0);
        fVar17 = (float)((ulonglong)in_dr2 >> 0x20);
        fVar20 = fVar33 * fVar12 + fVar29 * fVar14 + fVar23 * fVar15 + fVar31 * fVar17;
        fVar20 = fVar19 * fVar12 + fVar32 * fVar14 + fVar18 * fVar15 + fVar38 * fVar17;
        fVar20 = fVar25 * fVar12 + fVar36 * fVar14 + fVar24 * fVar15 + fVar39 * fVar17;
        fVar12 = fVar28 * fVar12 + fVar37 * fVar14 + fVar27 * fVar15 + fVar40 * fVar17;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar8 + -4) = (int)((ulonglong)dVar34 >> 0x20);
        param_3 = (double)CONCAT44(*(undefined4 *)(pdVar7 + 8),SUB84(dVar26,0));
        pdVar8 = pdVar8 + -1;
        *(int *)pdVar8 = (int)((ulonglong)dVar35 >> 0x20);
        param_4 = (double)CONCAT44(*(undefined4 *)((int)pdVar7 + 0x44),SUB84(dVar30,0));
        pdVar7 = pdVar7 + 9;
      }
      else {
        pdVar8[-1] = dVar34;
        param_3 = pdVar7[8];
        pdVar8 = pdVar8 + -2;
        *pdVar8 = dVar35;
        param_4 = pdVar7[9];
        pdVar7 = pdVar7 + 10;
      }
                    /* WARNING: Ignoring partial resolution of indirect */
      if (in_FPSCR_PR == '\0') {
        fVar20 = SUB84(param_1,0);
        fVar14 = (float)((ulonglong)param_1 >> 0x20);
        fVar15 = SUB84(param_2,0);
        fVar17 = (float)((ulonglong)param_2 >> 0x20);
        fVar12 = fVar33 * fVar20 + fVar29 * fVar14 + fVar23 * fVar15 + fVar31 * fVar17;
        fVar12 = fVar19 * fVar20 + fVar32 * fVar14 + fVar18 * fVar15 + fVar38 * fVar17;
        fVar12 = fVar25 * fVar20 + fVar36 * fVar14 + fVar24 * fVar15 + fVar39 * fVar17;
        fVar12 = fVar28 * fVar20 + fVar37 * fVar14 + fVar27 * fVar15 + fVar40 * fVar17;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar6 + -4) = (int)((ulonglong)in_dr0 >> 0x20);
        in_dr12 = (double)CONCAT44(*(undefined4 *)(pdVar8 + 8),SUB84(dVar34,0));
        pdVar6 = pdVar6 + -1;
        *(int *)pdVar6 = (int)((ulonglong)in_dr2 >> 0x20);
        in_dr14 = (double)CONCAT44(*(undefined4 *)((int)pdVar8 + 0x44),SUB84(dVar35,0));
        pdVar8 = pdVar8 + 9;
      }
      else {
        pdVar6[-1] = in_dr0;
        in_dr12 = pdVar8[8];
        pdVar6 = pdVar6 + -2;
        *pdVar6 = in_dr2;
        in_dr14 = pdVar8[9];
        pdVar8 = pdVar8 + 10;
      }
      pdVar6 = pdVar6 + -8;
                    /* WARNING: Ignoring partial resolution of indirect */
      if (in_FPSCR_PR == '\0') {
        fVar20 = SUB84(param_3,0);
        fVar14 = (float)((ulonglong)param_3 >> 0x20);
        fVar15 = SUB84(param_4,0);
        fVar17 = (float)((ulonglong)param_4 >> 0x20);
        fVar12 = fVar33 * fVar20 + fVar29 * fVar14 + fVar23 * fVar15 + fVar31 * fVar17;
        fVar12 = fVar19 * fVar20 + fVar32 * fVar14 + fVar18 * fVar15 + fVar38 * fVar17;
        fVar12 = fVar25 * fVar20 + fVar36 * fVar14 + fVar24 * fVar15 + fVar39 * fVar17;
        fVar12 = fVar28 * fVar20 + fVar37 * fVar14 + fVar27 * fVar15 + fVar40 * fVar17;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar3 + -4) = (int)((ulonglong)param_1 >> 0x20);
        pdVar3 = pdVar3 + -1;
        *(int *)pdVar3 = (int)((ulonglong)param_2 >> 0x20);
      }
      else {
        pdVar3[-1] = param_1;
        pdVar3 = pdVar3 + -2;
        *pdVar3 = param_2;
      }
      pdVar3 = pdVar3 + -8;
                    /* WARNING: Ignoring partial resolution of indirect */
      if (in_FPSCR_PR == '\0') {
        fVar12 = SUB84(in_dr12,0);
        fVar20 = (float)((ulonglong)in_dr12 >> 0x20);
        fVar14 = SUB84(in_dr14,0);
        fVar15 = (float)((ulonglong)in_dr14 >> 0x20);
        fVar33 = fVar33 * fVar12 + fVar29 * fVar20 + fVar23 * fVar14 + fVar31 * fVar15;
        fVar33 = fVar19 * fVar12 + fVar32 * fVar20 + fVar18 * fVar14 + fVar38 * fVar15;
        fVar33 = fVar25 * fVar12 + fVar36 * fVar20 + fVar24 * fVar14 + fVar39 * fVar15;
        fVar33 = fVar28 * fVar12 + fVar37 * fVar20 + fVar27 * fVar14 + fVar40 * fVar15;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar7 + -4) = (int)((ulonglong)param_3 >> 0x20);
        pdVar7 = pdVar7 + -1;
        *(int *)pdVar7 = (int)((ulonglong)param_4 >> 0x20);
      }
      else {
        pdVar7[-1] = param_3;
        pdVar7 = pdVar7 + -2;
        *pdVar7 = param_4;
      }
      pdVar7 = pdVar7 + -8;
      if (in_FPSCR_SZ == '\0') {
        *(int *)((int)pdVar8 + -4) = (int)((ulonglong)in_dr12 >> 0x20);
        pdVar8 = pdVar8 + -1;
        *(int *)pdVar8 = (int)((ulonglong)in_dr14 >> 0x20);
      }
      else {
        pdVar8[-1] = in_dr12;
        pdVar8 = pdVar8 + -2;
        *pdVar8 = in_dr14;
      }
      pdVar8 = pdVar8 + -8;
    }
    iVar11 = iVar11 + -1;
    pdVar6 = (double *)((int)pdVar6 + iVar9);
    pdVar3 = (double *)((int)pdVar3 + iVar9);
    pdVar7 = (double *)((int)pdVar7 + iVar9);
    pdVar8 = (double *)((int)pdVar8 + iVar9);
  } while (iVar11 != 0);
  if (in_FPSCR_SZ == '\0') {
    in_dr0 = (double)(ulonglong)*(uint *)pdVar4;
    in_dr2 = (double)(ulonglong)*(uint *)((int)pdVar4 + 4);
    pdVar4 = pdVar4 + 1;
  }
  else {
    in_xd0 = *pdVar4;
    in_xd2 = pdVar4[1];
    pdVar4 = pdVar4 + 2;
  }
  *(undefined4 *)(param_5 + 0x4c) = 6;
  if (in_FPSCR_SZ == '\0') {
    param_1 = (double)(ulonglong)*(uint *)pdVar4;
    param_2 = (double)(ulonglong)*(uint *)((int)pdVar4 + 4);
    param_3 = (double)(ulonglong)*(uint *)(pdVar4 + 1);
    pdVar4 = (double *)((int)pdVar4 + 0xc);
  }
  else {
    in_xd4 = *pdVar4;
    in_xd6 = pdVar4[1];
    in_xd8 = pdVar4[2];
    pdVar4 = pdVar4 + 3;
  }
  pdVar3 = *(double **)(param_5 + 0x48);
  if (in_FPSCR_SZ == '\0') {
    param_4 = (double)(ulonglong)*(uint *)pdVar4;
    in_dr12 = (double)(ulonglong)*(uint *)((int)pdVar4 + 4);
    pdVar4 = pdVar4 + 1;
  }
  else {
    in_xd10 = *pdVar4;
    in_xd12 = pdVar4[1];
    pdVar4 = pdVar4 + 2;
  }
  uVar10 = *(uint *)(param_5 + 0x28);
  pdVar6 = pdVar3 + 0x10;
  if (in_FPSCR_SZ != '\0') {
    in_xd14 = *pdVar4;
  }
  iVar9 = 0x100;
  do {
    uVar1 = uVar10 & 0x80000000;
    uVar10 = uVar10 << 1;
    psVar5 = (short *)**(int **)(param_5 + 0x34);
    *(int **)(param_5 + 0x34) = *(int **)(param_5 + 0x34) + 1;
    if (uVar1 != 0) {
      iVar9 = 8;
      if (in_FPSCR_SZ == '\0') {
        param_3 = (double)CONCAT44(*(undefined4 *)pdVar6,SUB84(param_3,0));
        param_4 = (double)CONCAT44(*(undefined4 *)((int)pdVar6 + 4),SUB84(param_4,0));
        pdVar6 = pdVar6 + 1;
      }
      else {
        param_3 = *pdVar6;
        param_4 = pdVar6[1];
        pdVar6 = pdVar6 + 2;
      }
      do {
                    /* WARNING: Ignoring partial resolution of indirect */
        if (in_FPSCR_PR == '\0') {
          fVar19 = SUB84(param_3,0);
          fVar25 = (float)((ulonglong)param_3 >> 0x20);
          fVar28 = SUB84(param_4,0);
          fVar29 = (float)((ulonglong)param_4 >> 0x20);
          fVar33 = SUB84(in_xd0,0) * fVar19 + SUB84(in_xd4,0) * fVar25 + SUB84(in_xd8,0) * fVar28 +
                   SUB84(in_xd12,0) * fVar29;
          fVar33 = (float)((ulonglong)in_xd0 >> 0x20) * fVar19 +
                   (float)((ulonglong)in_xd4 >> 0x20) * fVar25 +
                   (float)((ulonglong)in_xd8 >> 0x20) * fVar28 +
                   (float)((ulonglong)in_xd12 >> 0x20) * fVar29;
          fVar33 = SUB84(in_xd2,0) * fVar19 + SUB84(in_xd6,0) * fVar25 + SUB84(in_xd10,0) * fVar28 +
                   SUB84(in_xd14,0) * fVar29;
          fVar33 = (float)((ulonglong)in_xd2 >> 0x20) * fVar19 +
                   (float)((ulonglong)in_xd6 >> 0x20) * fVar25 +
                   (float)((ulonglong)in_xd10 >> 0x20) * fVar28 +
                   (float)((ulonglong)in_xd14 >> 0x20) * fVar29;
        }
        if (in_FPSCR_SZ == '\0') {
          fVar19 = *(float *)pdVar3;
          fVar33 = *(float *)((int)pdVar3 + 4);
          pdVar3 = pdVar3 + 1;
          dVar13 = (double)CONCAT44(fVar19,SUB84(param_2,0));
          dVar21 = (double)CONCAT44(fVar33,SUB84(param_1,0));
          dVar16 = (double)CONCAT44(fVar33,SUB84(in_dr0,0));
          dVar26 = (double)CONCAT44(fVar19,SUB84(in_dr2,0));
        }
        else {
          dVar13 = *pdVar3;
          dVar21 = pdVar3[1];
          pdVar3 = pdVar3 + 2;
          fVar19 = (float)((ulonglong)dVar13 >> 0x20);
          fVar33 = (float)((ulonglong)dVar21 >> 0x20);
          dVar16 = dVar21;
          dVar26 = dVar13;
        }
        fVar28 = (float)((ulonglong)param_3 >> 0x20);
        fVar25 = SUB84(param_3,0);
        fVar32 = (float)((ulonglong)param_4 >> 0x20);
        fVar29 = SUB84(param_4,0);
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44(fVar33 + fVar28,SUB84(dVar16,0) + fVar25);
          dVar16 = (double)CONCAT44(fVar19 + fVar32,SUB84(dVar26,0) + fVar29);
          dVar26 = (double)CONCAT44((float)((ulonglong)dVar21 >> 0x20) - fVar28,
                                    SUB84(dVar21,0) - fVar25);
        }
        else {
          dVar22 = dVar16 + param_3 + param_3;
          dVar16 = dVar26 + param_4 + param_4;
          dVar26 = (dVar21 - param_3) - param_3;
        }
        if (in_FPSCR_SZ == '\0') {
          in_dr12 = (double)CONCAT44(*(undefined4 *)(param_5 + 0x20),SUB84(in_dr12,0));
        }
        else {
          in_dr12 = *(double *)(param_5 + 0x20);
        }
        if (in_FPSCR_PR == '\0') {
          dVar13 = (double)CONCAT44((float)((ulonglong)dVar13 >> 0x20) - fVar32,SUB84(dVar13,0));
        }
        else {
          dVar13 = dVar13 - param_4;
        }
        if (in_FPSCR_SZ == '\0') {
          param_3 = (double)CONCAT44(*(undefined4 *)pdVar6,fVar25);
          pdVar6 = (double *)((int)pdVar6 + 4);
        }
        else {
          param_3 = *pdVar6;
          pdVar6 = pdVar6 + 1;
        }
        if (in_FPSCR_PR == '\0') {
          dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),SUB84(dVar13,0) - fVar29);
        }
        else {
          dVar13 = dVar13 - param_4;
        }
        if (in_FPSCR_SZ == '\0') {
          param_4 = (double)CONCAT44(*(undefined4 *)pdVar6,fVar29);
          pdVar6 = (double *)((int)pdVar6 + 4);
        }
        else {
          param_4 = *pdVar6;
          pdVar6 = pdVar6 + 1;
        }
        fVar33 = (float)((ulonglong)in_dr12 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          dVar22 = (double)CONCAT44((float)((ulonglong)dVar22 >> 0x20) + fVar33,SUB84(dVar22,0));
        }
        else {
          dVar22 = dVar22 + in_dr12;
        }
        *(double **)(param_5 + 0x50) = pdVar3;
        if (in_FPSCR_PR == '\0') {
          in_dr0 = (double)CONCAT44((int)((ulonglong)dVar22 >> 0x20),SUB84(dVar22,0) + fVar33);
        }
        else {
          in_dr0 = dVar22 + in_dr12;
        }
        if (in_FPSCR_PR == '\0') {
          in_dr2 = (double)CONCAT44((float)((ulonglong)dVar16 >> 0x20) + fVar33,
                                    SUB84(dVar16,0) + fVar33);
          param_1 = (double)CONCAT44((float)((ulonglong)dVar26 >> 0x20) + fVar33,
                                     SUB84(dVar26,0) + fVar33);
          param_2 = (double)CONCAT44((float)((ulonglong)dVar13 >> 0x20) + fVar33,
                                     SUB84(dVar13,0) + fVar33);
          sVar41 = (short)(int)(float)((ulonglong)in_dr0 >> 0x20);
          sVar42 = (short)(int)SUB84(in_dr0,0);
        }
        else {
          in_dr2 = dVar16 + in_dr12 + in_dr12;
          param_1 = dVar26 + in_dr12 + in_dr12;
          param_2 = dVar13 + in_dr12 + in_dr12;
          sVar41 = (short)(int)in_dr0;
          sVar42 = (short)(int)in_dr0;
        }
        sVar2 = (short)*(undefined4 *)(param_5 + 0x38);
        *psVar5 = sVar41 - sVar2;
        if (in_FPSCR_PR == '\0') {
          sVar41 = (short)(int)(float)((ulonglong)in_dr2 >> 0x20);
        }
        else {
          sVar41 = (short)(int)in_dr2;
        }
        psVar5[0x10] = sVar42 - sVar2;
        if (in_FPSCR_PR == '\0') {
          sVar42 = (short)(int)SUB84(in_dr2,0);
        }
        else {
          sVar42 = (short)(int)in_dr2;
        }
        psVar5[0x20] = sVar41 - sVar2;
        if (in_FPSCR_PR == '\0') {
          sVar41 = (short)(int)(float)((ulonglong)param_1 >> 0x20);
        }
        else {
          sVar41 = (short)(int)param_1;
        }
        psVar5[0x30] = sVar42 - sVar2;
        if (in_FPSCR_PR == '\0') {
          sVar42 = (short)(int)SUB84(param_1,0);
        }
        else {
          sVar42 = (short)(int)param_1;
        }
        psVar5[0x38] = sVar41 - sVar2;
        if (in_FPSCR_PR == '\0') {
          sVar41 = (short)(int)(float)((ulonglong)param_2 >> 0x20);
        }
        else {
          sVar41 = (short)(int)param_2;
        }
        psVar5[0x28] = sVar42 - sVar2;
        if (in_FPSCR_PR == '\0') {
          sVar42 = (short)(int)SUB84(param_2,0);
        }
        else {
          sVar42 = (short)(int)param_2;
        }
        psVar5[0x18] = sVar41 - sVar2;
        iVar9 = iVar9 + -1;
        psVar5[8] = sVar42 - sVar2;
        psVar5 = psVar5 + 1;
        pdVar3 = *(double **)(param_5 + 0x50);
      } while (iVar9 != 0);
      iVar9 = *(int *)(param_5 + 0x24);
      pdVar3 = pdVar3 + -0x10;
      pdVar6 = pdVar6 + -0x12;
    }
    pdVar3 = (double *)((int)pdVar3 + iVar9);
    iVar11 = *(int *)(param_5 + 0x4c) + -1;
    *(int *)(param_5 + 0x4c) = iVar11;
    pdVar6 = (double *)((int)pdVar6 + iVar9);
  } while (iVar11 != 0);
  return;
}


/* Address: 8ceb17e8 */

/* WARNING: Removing unreachable block (ram,0x8ceb186c) */

void callback_candidate_8ceb17e8(undefined4 param_1,uint *param_2,int param_3)

{
  byte bVar1;
  uint uVar2;
  double *pdVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  uint uVar8;
  double dVar9;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  uVar4 = *param_2 >> 0x10;
  if (0x10 < (int)param_2[2]) {
    uVar2 = param_2[2] - 0x30;
    if ((int)uVar2 < 0) {
      uVar2 = param_2[1] >> (~uVar2 & 0x1f) + 1;
    }
    else {
      uVar2 = param_2[1] << (uVar2 & 0x1f);
    }
    uVar4 = uVar4 | uVar2;
  }
  bVar1 = *(byte *)(*(int *)(param_3 + 0x2c) + (uVar4 >> 9));
  uVar2 = (uint)(bVar1 >> 4);
  uVar8 = bVar1 & 0xf;
  iVar7 = 0;
  if (uVar2 != 0) {
    uVar6 = (uint)*(short *)(param_2[0xc] + uVar8 * 2);
    uVar4 = uVar4 & uVar6;
    uVar6 = ~(uVar6 >> 1);
    uVar2 = (uVar8 + uVar2) - 0x10;
    iVar7 = uVar6 + (uVar6 & uVar4);
    if ((int)uVar2 < 0) {
      iVar5 = (int)uVar4 >> (~uVar2 & 0x1f) + 1;
      uVar4 = iVar7 >> (~uVar2 & 0x1f) + 1;
    }
    else {
      iVar5 = uVar4 << (uVar2 & 0x1f);
      uVar4 = iVar7 << (uVar2 & 0x1f);
    }
    iVar7 = ((uVar4 << 1 | (uint)((uVar4 & 0x80000000) != 0)) + iVar5) * 8;
  }
  pdVar3 = *(double **)(param_3 + 0x1c);
  iVar7 = iVar7 + **(int **)(param_3 + 0x28);
  if (in_FPSCR_PR == '\0') {
    dVar9 = (double)CONCAT44((float)iVar7,param_1);
  }
  else {
    dVar9 = (double)iVar7;
  }
  **(int **)(param_3 + 0x28) = iVar7;
  if (in_FPSCR_SZ == '\0') {
    *(int *)pdVar3 = (int)((ulonglong)dVar9 >> 0x20);
  }
  else {
    *pdVar3 = dVar9;
  }
  *(undefined4 *)(param_3 + 0x10) = 0;
  FUN_8ceb1a3c();
  return;
}


/* Address: 8ceb1888 */

/* WARNING: Removing unreachable block (ram,0x8ceb1a02) */
/* WARNING: Removing unreachable block (ram,0x8ceb1956) */
/* WARNING: Removing unreachable block (ram,0x8ceb1940) */
/* WARNING: Removing unreachable block (ram,0x8ceb198a) */
/* WARNING: Removing unreachable block (ram,0x8ceb19b4) */
/* WARNING: Removing unreachable block (ram,0x8ceb19c6) */
/* WARNING: Removing unreachable block (ram,0x8ceb199e) */
/* WARNING: Removing unreachable block (ram,0x8ceb1976) */
/* WARNING: Removing unreachable block (ram,0x8ceb19d6) */
/* WARNING: Removing unreachable block (ram,0x8ceb1a20) */

void FUN_8ceb1888(uint *param_1,int param_2)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  undefined8 *puVar6;
  double dVar7;
  bool bVar8;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  puVar6 = *(undefined8 **)(param_2 + 0x1c);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)((int)puVar6 + 0xfc) = 0;
    *(undefined4 *)(puVar6 + 0x1f) = 0;
    *(undefined4 *)((int)puVar6 + 0xf4) = 0;
    *(undefined4 *)(puVar6 + 0x1e) = 0;
    *(undefined4 *)((int)puVar6 + 0xec) = 0;
    *(undefined4 *)(puVar6 + 0x1d) = 0;
    *(undefined4 *)((int)puVar6 + 0xe4) = 0;
    *(undefined4 *)(puVar6 + 0x1c) = 0;
    *(undefined4 *)((int)puVar6 + 0xdc) = 0;
    *(undefined4 *)(puVar6 + 0x1b) = 0;
    *(undefined4 *)((int)puVar6 + 0xd4) = 0;
    *(undefined4 *)(puVar6 + 0x1a) = 0;
    *(undefined4 *)((int)puVar6 + 0xcc) = 0;
    *(undefined4 *)(puVar6 + 0x19) = 0;
    *(undefined4 *)((int)puVar6 + 0xc4) = 0;
    *(undefined4 *)(puVar6 + 0x18) = 0;
    *(undefined4 *)((int)puVar6 + 0xbc) = 0;
    *(undefined4 *)(puVar6 + 0x17) = 0;
    *(undefined4 *)((int)puVar6 + 0xb4) = 0;
    *(undefined4 *)(puVar6 + 0x16) = 0;
    *(undefined4 *)((int)puVar6 + 0xac) = 0;
    *(undefined4 *)(puVar6 + 0x15) = 0;
    *(undefined4 *)((int)puVar6 + 0xa4) = 0;
    *(undefined4 *)(puVar6 + 0x14) = 0;
    *(undefined4 *)((int)puVar6 + 0x9c) = 0;
    *(undefined4 *)(puVar6 + 0x13) = 0;
    *(undefined4 *)((int)puVar6 + 0x94) = 0;
    *(undefined4 *)(puVar6 + 0x12) = 0;
    *(undefined4 *)((int)puVar6 + 0x8c) = 0;
    *(undefined4 *)(puVar6 + 0x11) = 0;
    *(undefined4 *)((int)puVar6 + 0x84) = 0;
    *(undefined4 *)(puVar6 + 0x10) = 0;
  }
  else {
    puVar6[0x1f] = 0;
    puVar6[0x1e] = 0;
    puVar6[0x1d] = 0;
    puVar6[0x1c] = 0;
    puVar6[0x1b] = 0;
    puVar6[0x1a] = 0;
    puVar6[0x19] = 0;
    puVar6[0x18] = 0;
    puVar6[0x17] = 0;
    puVar6[0x16] = 0;
    puVar6[0x15] = 0;
    puVar6[0x14] = 0;
    puVar6[0x13] = 0;
    puVar6[0x12] = 0;
    puVar6[0x11] = 0;
    puVar6[0x10] = 0;
    puVar6[0xf] = 0;
    puVar6[0xe] = 0;
    puVar6[0xd] = 0;
    puVar6[0xc] = 0;
    puVar6[0xb] = 0;
    puVar6[10] = 0;
    puVar6[9] = 0;
    puVar6[8] = 0;
    puVar6[7] = 0;
    puVar6[6] = 0;
    puVar6[5] = 0;
    puVar6[4] = 0;
    puVar6[3] = 0;
    puVar6[2] = 0;
    puVar6[1] = 0;
    *puVar6 = 0;
  }
  uVar5 = *param_1;
  if (param_1[2] != 0) {
    uVar1 = param_1[2] - 0x20;
    if ((int)uVar1 < 0) {
      uVar1 = param_1[1] >> (~uVar1 & 0x1f) + 1;
    }
    else {
      uVar1 = param_1[1] << (uVar1 & 0x1f);
    }
    uVar5 = uVar5 | uVar1;
  }
  uVar1 = uVar5 << 1;
  if ((uVar5 & 0x80000000) == 0) {
    uVar3 = (uVar5 & 0x7fffffff) >> 0x17;
    if (uVar3 < 8) {
      if (((uVar5 & 0x7fffffff) >> 0x19 & 1) == 0) {
        if (((uVar5 & 0x7fffffff) >> 0x18 & 1) == 0) {
          if ((uVar3 & 1) == 0) {
            if ((uVar5 & 0x400000) == 0) {
              if ((uVar5 & 0x200000) == 0) {
                uVar1 = (uVar5 & 0xfffff) >> 0xf;
                uVar3 = (uint)*(short *)(param_1[10] + (uVar1 & 0xfffffffe));
              }
              else {
                uVar1 = (uVar5 & 0x1fffff) >> 0x10;
                uVar3 = (uint)*(short *)(param_1[9] + (uVar1 & 0xfffffffe));
              }
            }
            else {
              uVar1 = (uVar5 & 0x3fffff) >> 0x11;
              uVar3 = (uint)*(short *)(param_1[8] + (uVar1 & 0xfffffffe));
            }
          }
          else {
            uVar1 = (uVar5 & 0x7fffffff) >> 0x12;
            uVar3 = (uint)*(short *)(param_1[7] + (uVar1 & 0xfffffffe));
          }
        }
        else {
          uVar1 = (uVar5 & 0x7fffffff) >> 0x13;
          uVar3 = (uint)*(short *)(param_1[6] + (uVar1 & 0xfffffffe));
        }
      }
      else {
        uVar1 = (uVar5 & 0x7fffffff) >> 0x15;
        uVar3 = (uint)*(short *)(param_1[5] + (uVar1 & 0xfffffffe));
      }
      uVar4 = uVar3 & 0xff;
      uVar3 = uVar3 >> 8;
    }
    else {
      uVar3 = *(uint *)(param_1[4] + ((uVar5 & 0x7fffffff) >> 0x18) * 4);
      uVar4 = uVar3 & 0xff;
      if (uVar4 == 0x40) {
        uVar1 = (uVar5 & 0x7fffffff) >> 10 & 0xffff;
        uVar4 = uVar1 >> 10;
        uVar3 = (uint)(char)(uVar1 >> 2);
        if ((uVar3 & 0x7f) == 0) {
          uVar3 = uVar3 << 1 | (uVar5 & 0x7fffffff) >> 4 & 0xff;
        }
        bVar8 = (int)uVar3 < 0;
        if (bVar8) {
          uVar3 = -uVar3;
        }
        uVar1 = (uint)bVar8;
        goto LAB_8ceb19e8;
      }
      uVar5 = (uVar3 >> 0x10) - 0x21;
      uVar3 = (uint)(char)(uVar3 >> 8);
      if ((int)uVar5 < 0) {
        uVar1 = uVar1 >> (~uVar5 & 0x1f) + 1;
      }
      else {
        uVar1 = uVar1 << (uVar5 & 0x1f);
      }
    }
    uVar1 = uVar1 & 1;
  }
  else {
    uVar4 = 0;
    uVar3 = 1;
    uVar1 = (uint)((uVar1 & 0x80000000) != 0);
  }
LAB_8ceb19e8:
  iVar2 = (int)*(char *)(param_1[0xb] + uVar4);
  *(int *)(param_2 + 0x10) = iVar2;
  uVar5 = ((int)((uVar3 * 2 + 1) * *(int *)(param_2 + 0x24) *
                (uint)*(byte *)(*(int *)(param_2 + 0x20) + iVar2)) >> 4) - 1U | 1;
  if (uVar1 != 0) {
    uVar5 = -uVar5;
  }
  if (in_FPSCR_PR == '\0') {
    dVar7 = (double)((ulonglong)(uint)(float)(int)uVar5 << 0x20);
  }
  else {
    dVar7 = (double)(int)uVar5;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(*(int *)(param_2 + 0x1c) + iVar2 * 4) = (int)((ulonglong)dVar7 >> 0x20);
  }
  else {
    *(double *)(*(int *)(param_2 + 0x1c) + iVar2 * 4) = dVar7;
  }
  FUN_8ceb1a3c();
  return;
}


/* Address: 8ceb1a3c */

/* WARNING: Removing unreachable block (ram,0x8ceb1ace) */
/* WARNING: Removing unreachable block (ram,0x8ceb1af6) */
/* WARNING: Removing unreachable block (ram,0x8ceb1b1e) */
/* WARNING: Removing unreachable block (ram,0x8ceb1b2e) */
/* WARNING: Removing unreachable block (ram,0x8ceb1b7a) */
/* WARNING: Removing unreachable block (ram,0x8ceb1b0c) */
/* WARNING: Removing unreachable block (ram,0x8ceb1ae2) */
/* WARNING: Removing unreachable block (ram,0x8ceb1a98) */
/* WARNING: Removing unreachable block (ram,0x8ceb1aae) */
/* WARNING: Removing unreachable block (ram,0x8ceb1b58) */

uint FUN_8ceb1a3c(double param_1,uint *param_2,int param_3,uint param_4)

{
  uint uVar1;
  int in_r2;
  uint in_r3;
  uint uVar2;
  uint uVar3;
  char *unaff_r8;
  uint uVar4;
  uint uVar5;
  uint unaff_r11;
  uint unaff_r12;
  uint unaff_r13;
  uint *unaff_r14;
  bool bVar6;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  do {
    uVar1 = unaff_r11;
    if (unaff_r13 != 0) {
      uVar1 = unaff_r13 - 0x20;
      if ((int)uVar1 < 0) {
        uVar1 = unaff_r12 >> (~uVar1 & 0x1f) + 1;
      }
      else {
        uVar1 = unaff_r12 << (uVar1 & 0x1f);
      }
      uVar1 = unaff_r11 | uVar1;
    }
    uVar2 = uVar1 << 1;
    if ((uVar1 & 0x80000000) == 0) {
      uVar5 = (uVar1 & 0x7fffffff) >> 0x17;
      if (uVar5 < 8) {
        if (((uVar1 & 0x7fffffff) >> 0x19 & 1) == 0) {
          if (((uVar1 & 0x7fffffff) >> 0x18 & 1) == 0) {
            if ((uVar5 & 1) == 0) {
              if ((uVar1 & 0x400000) == 0) {
                if ((uVar1 & 0x200000) == 0) {
                  uVar5 = 0x11;
                  uVar2 = (uVar1 & 0xfffff) >> 0xf;
                  param_4 = (uint)*(short *)(param_2[10] + (uVar2 & 0xfffffffe));
                }
                else {
                  uVar5 = 0x10;
                  uVar2 = (uVar1 & 0x1fffff) >> 0x10;
                  param_4 = (uint)*(short *)(param_2[9] + (uVar2 & 0xfffffffe));
                }
              }
              else {
                uVar5 = 0xf;
                uVar2 = (uVar1 & 0x3fffff) >> 0x11;
                param_4 = (uint)*(short *)(param_2[8] + (uVar2 & 0xfffffffe));
              }
            }
            else {
              uVar5 = 0xe;
              uVar2 = (uVar1 & 0x7fffffff) >> 0x12;
              param_4 = (uint)*(short *)(param_2[7] + (uVar2 & 0xfffffffe));
            }
          }
          else {
            uVar5 = 0xd;
            uVar2 = (uVar1 & 0x7fffffff) >> 0x13;
            param_4 = (uint)*(short *)(param_2[6] + (uVar2 & 0xfffffffe));
          }
        }
        else {
          uVar5 = 0xb;
          uVar2 = (uVar1 & 0x7fffffff) >> 0x15;
          param_4 = (uint)*(short *)(param_2[5] + (uVar2 & 0xfffffffe));
        }
        uVar4 = param_4 & 0xff;
        param_4 = param_4 >> 8;
      }
      else {
        uVar3 = *(uint *)(param_2[4] + ((uVar1 & 0x7fffffff) >> 0x18) * 4);
        uVar4 = uVar3 & 0xff;
        if (uVar4 == 0x40) {
          uVar5 = 0x14;
          uVar2 = (uVar1 & 0x7fffffff) >> 10 & 0xffff;
          uVar4 = uVar2 >> 10;
          param_4 = (uint)(char)(uVar2 >> 2);
          if ((param_4 & 0x7f) == 0) {
            uVar5 = 0x1c;
            param_4 = param_4 << 1 | (uVar1 & 0x7fffffff) >> 4 & 0xff;
          }
          bVar6 = (int)param_4 < 0;
          if (bVar6) {
            param_4 = -param_4;
          }
          uVar1 = (uint)bVar6;
          goto LAB_8ceb1b40;
        }
        uVar5 = uVar3 >> 0x10;
        uVar1 = uVar5 - 0x21;
        param_4 = (uint)(char)(uVar3 >> 8);
        if ((int)uVar1 < 0) {
          uVar2 = uVar2 >> (~uVar1 & 0x1f) + 1;
        }
        else {
          uVar2 = uVar2 << (uVar1 & 0x1f);
        }
      }
      uVar1 = uVar2 & 1;
    }
    else {
      uVar1 = uVar1 << 2;
      if ((uVar2 & 0x80000000) == 0) {
        uVar4 = 0xffffffff;
        uVar5 = 2;
      }
      else {
        uVar5 = 3;
        uVar4 = 0;
        param_4 = 1;
        uVar1 = (uint)((uVar1 & 0x80000000) != 0);
      }
    }
LAB_8ceb1b40:
    unaff_r13 = unaff_r13 + uVar5;
    if ((int)unaff_r13 < 0x20) {
      unaff_r11 = unaff_r11 << (uVar5 & 0x1f);
    }
    else {
      unaff_r13 = unaff_r13 - 0x20;
      uVar2 = *unaff_r14;
      unaff_r14 = unaff_r14 + 1;
      if ((int)unaff_r13 < 0) {
        unaff_r11 = unaff_r12 >> (~unaff_r13 & 0x1f) + 1;
      }
      else {
        unaff_r11 = unaff_r12 << (unaff_r13 & 0x1f);
      }
      unaff_r12 = ((uVar2 & 0xff) << 8 | (uVar2 & 0xff00) >> 8) << 0x10 |
                  ((uVar2 & 0xff0000) >> 0x10) << 8 | uVar2 >> 0x18;
    }
    unaff_r8 = unaff_r8 + uVar4 + 1;
    if (uVar4 + 1 == 0) {
      *param_2 = unaff_r11;
      param_2[1] = unaff_r12;
      param_2[2] = unaff_r13;
      param_2[3] = (uint)unaff_r14;
      if (in_r3 != *(uint *)(param_3 + 0x10)) {
        in_r3 = -in_r3;
      }
      return in_r3;
    }
    uVar5 = (uint)*unaff_r8;
    param_4 = (param_4 * 2 + in_r2) * *(int *)(param_3 + 0x24);
    uVar2 = ((int)(param_4 * *(byte *)(*(int *)(param_3 + 0x20) + uVar5)) >> 4) - 1U | 1;
    if (uVar1 != 0) {
      uVar2 = -uVar2;
    }
    if (in_FPSCR_PR == '\0') {
      param_1 = (double)CONCAT44((float)(int)uVar2,SUB84(param_1,0));
    }
    else {
      param_1 = (double)(int)uVar2;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(*(int *)(param_3 + 0x1c) + uVar5 * 4) = (int)((ulonglong)param_1 >> 0x20);
    }
    else {
      *(double *)(*(int *)(param_3 + 0x1c) + uVar5 * 4) = param_1;
    }
    in_r3 = uVar5 & 0x3fffffff;
  } while( true );
}


/* Address: 8ceb1bb8 */

void FUN_8ceb1bb8(int param_1)

{
  undefined8 *puVar1;
  int iVar2;
  char in_FPSCR_SZ;
  
  iVar2 = 6;
  puVar1 = (undefined8 *)(param_1 + 0x600);
  do {
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)((int)puVar1 + -4) = 0;
      *(undefined4 *)(puVar1 + -1) = 0;
      *(undefined4 *)((int)puVar1 + -0xc) = 0;
      *(undefined4 *)(puVar1 + -2) = 0;
      *(undefined4 *)((int)puVar1 + -0x14) = 0;
      *(undefined4 *)(puVar1 + -3) = 0;
      *(undefined4 *)((int)puVar1 + -0x1c) = 0;
      *(undefined4 *)(puVar1 + -4) = 0;
      *(undefined4 *)((int)puVar1 + -0x24) = 0;
      *(undefined4 *)(puVar1 + -5) = 0;
      *(undefined4 *)((int)puVar1 + -0x2c) = 0;
      *(undefined4 *)(puVar1 + -6) = 0;
      *(undefined4 *)((int)puVar1 + -0x34) = 0;
      *(undefined4 *)(puVar1 + -7) = 0;
      *(undefined4 *)((int)puVar1 + -0x3c) = 0;
      *(undefined4 *)(puVar1 + -8) = 0;
      *(undefined4 *)((int)puVar1 + -0x44) = 0;
      *(undefined4 *)(puVar1 + -9) = 0;
      *(undefined4 *)((int)puVar1 + -0x4c) = 0;
      *(undefined4 *)(puVar1 + -10) = 0;
      *(undefined4 *)((int)puVar1 + -0x54) = 0;
      *(undefined4 *)(puVar1 + -0xb) = 0;
      *(undefined4 *)((int)puVar1 + -0x5c) = 0;
      *(undefined4 *)(puVar1 + -0xc) = 0;
      *(undefined4 *)((int)puVar1 + -100) = 0;
      *(undefined4 *)(puVar1 + -0xd) = 0;
      *(undefined4 *)((int)puVar1 + -0x6c) = 0;
      *(undefined4 *)(puVar1 + -0xe) = 0;
      puVar1 = (undefined8 *)((int)puVar1 + -0x74);
      *(undefined4 *)puVar1 = 0;
    }
    else {
      puVar1[-1] = 0;
      puVar1[-2] = 0;
      puVar1[-3] = 0;
      puVar1[-4] = 0;
      puVar1[-5] = 0;
      puVar1[-6] = 0;
      puVar1[-7] = 0;
      puVar1[-8] = 0;
      puVar1[-9] = 0;
      puVar1[-10] = 0;
      puVar1[-0xb] = 0;
      puVar1[-0xc] = 0;
      puVar1[-0xd] = 0;
      puVar1[-0xe] = 0;
      puVar1[-0xf] = 0;
      puVar1[-0x10] = 0;
      puVar1[-0x11] = 0;
      puVar1[-0x12] = 0;
      puVar1[-0x13] = 0;
      puVar1[-0x14] = 0;
      puVar1[-0x15] = 0;
      puVar1[-0x16] = 0;
      puVar1[-0x17] = 0;
      puVar1[-0x18] = 0;
      puVar1[-0x19] = 0;
      puVar1[-0x1a] = 0;
      puVar1[-0x1b] = 0;
      puVar1[-0x1c] = 0;
      puVar1 = puVar1 + -0x1d;
      *puVar1 = 0;
    }
    iVar2 = iVar2 + -1;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)((int)puVar1 + -4) = 0;
      *(undefined4 *)(puVar1 + -1) = 0;
      puVar1 = (undefined8 *)((int)puVar1 + -0xc);
      *(undefined4 *)puVar1 = 0;
    }
    else {
      puVar1[-1] = 0;
      puVar1[-2] = 0;
      puVar1 = puVar1 + -3;
      *puVar1 = 0;
    }
  } while (iVar2 != 0);
  return;
}


/* Address: 8ceb1c10 */

void callback_candidate_8ceb1c10
               (undefined8 param_1,undefined8 param_2,short *param_3,undefined8 *param_4,
               int *param_5,int param_6)

{
  short sVar1;
  short sVar2;
  undefined1 uVar3;
  int iVar4;
  short *psVar5;
  short *psVar6;
  int iVar7;
  int iVar8;
  int *piVar9;
  int *piVar10;
  int iVar11;
  undefined8 *puVar12;
  undefined8 in_dr0;
  undefined8 uVar13;
  undefined8 in_dr2;
  undefined8 uVar14;
  undefined8 uVar15;
  undefined8 uVar16;
  undefined4 uVar17;
  char in_FPSCR_SZ;
  
  iVar7 = 6;
  iVar8 = *param_5;
  do {
    piVar9 = (int *)param_5[1];
    param_5 = param_5 + 2;
    iVar11 = *param_5;
    if (iVar8 == iVar7) {
      param_6 = param_6 + -0x10;
    }
    iVar4 = 8;
    piVar10 = piVar9;
    do {
      sVar1 = *param_3;
      sVar2 = param_3[1];
      if (((uint)piVar10 & 0x1f) == 0) {
        *piVar10 = param_6;
      }
      uVar3 = *(undefined1 *)(param_6 + sVar2);
      *(undefined1 *)piVar10 = *(undefined1 *)(param_6 + sVar1);
      *(undefined1 *)((int)piVar10 + 1) = uVar3;
      iVar4 = iVar4 + -1;
      uVar3 = *(undefined1 *)(param_6 + param_3[3]);
      *(undefined1 *)((int)piVar10 + 2) = *(undefined1 *)(param_6 + param_3[2]);
      *(undefined1 *)((int)piVar10 + 3) = uVar3;
      psVar5 = param_3 + 6;
      uVar3 = *(undefined1 *)(param_6 + param_3[5]);
      *(undefined1 *)(piVar10 + 1) = *(undefined1 *)(param_6 + param_3[4]);
      *(undefined1 *)((int)piVar10 + 5) = uVar3;
      psVar6 = param_3 + 7;
      param_3 = param_3 + 8;
      uVar3 = *(undefined1 *)(param_6 + *psVar6);
      *(undefined1 *)((int)piVar10 + 6) = *(undefined1 *)(param_6 + *psVar5);
      *(undefined1 *)((int)piVar10 + 7) = uVar3;
      piVar10 = (int *)((int)piVar10 + iVar11);
    } while (iVar4 != 0);
    if (in_FPSCR_SZ == '\0') {
      uVar13 = CONCAT44(*piVar9,(int)in_dr0);
    }
    else {
      uVar13 = *(undefined8 *)piVar9;
    }
    puVar12 = (undefined8 *)((int)piVar9 + iVar11);
    if (in_FPSCR_SZ == '\0') {
      uVar14 = CONCAT44(*(undefined4 *)puVar12,(int)in_dr2);
    }
    else {
      uVar14 = *puVar12;
    }
    puVar12 = (undefined8 *)((int)puVar12 + iVar11);
    if (in_FPSCR_SZ == '\0') {
      uVar15 = CONCAT44(*(undefined4 *)puVar12,(int)param_1);
    }
    else {
      uVar15 = *puVar12;
    }
    puVar12 = (undefined8 *)((int)puVar12 + iVar11);
    if (in_FPSCR_SZ == '\0') {
      uVar16 = CONCAT44(*(undefined4 *)puVar12,(int)param_2);
    }
    else {
      uVar16 = *puVar12;
    }
    puVar12 = (undefined8 *)((int)puVar12 + iVar11);
    if (in_FPSCR_SZ == '\0') {
      *(int *)param_4 = (int)((ulonglong)uVar13 >> 0x20);
      *(int *)(param_4 + 1) = (int)((ulonglong)uVar14 >> 0x20);
      *(int *)(param_4 + 2) = (int)((ulonglong)uVar15 >> 0x20);
      *(int *)(param_4 + 3) = (int)((ulonglong)uVar16 >> 0x20);
      in_dr0 = CONCAT44(*(undefined4 *)puVar12,(int)uVar13);
    }
    else {
      *param_4 = uVar13;
      param_4[1] = uVar14;
      param_4[2] = uVar15;
      param_4[3] = uVar16;
      in_dr0 = *puVar12;
    }
    puVar12 = (undefined8 *)((int)puVar12 + iVar11);
    if (in_FPSCR_SZ == '\0') {
      in_dr2 = CONCAT44(*(undefined4 *)puVar12,(int)uVar14);
    }
    else {
      in_dr2 = *puVar12;
    }
    puVar12 = (undefined8 *)((int)puVar12 + iVar11);
    if (in_FPSCR_SZ == '\0') {
      param_1 = CONCAT44(*(undefined4 *)puVar12,(int)uVar15);
    }
    else {
      param_1 = *puVar12;
    }
    if (in_FPSCR_SZ == '\0') {
      uVar17 = *(undefined4 *)((int)puVar12 + iVar11);
      param_2 = CONCAT44(uVar17,(int)uVar16);
      *(int *)(param_4 + 4) = (int)((ulonglong)in_dr0 >> 0x20);
      *(int *)(param_4 + 5) = (int)((ulonglong)in_dr2 >> 0x20);
      *(int *)(param_4 + 6) = (int)((ulonglong)param_1 >> 0x20);
      *(undefined4 *)(param_4 + 7) = uVar17;
    }
    else {
      param_2 = *(undefined8 *)((int)puVar12 + iVar11);
      param_4[4] = in_dr0;
      param_4[5] = in_dr2;
      param_4[6] = param_1;
      param_4[7] = param_2;
    }
    iVar7 = iVar7 + -1;
    param_4 = param_4 + 8;
  } while (iVar7 != 0);
  return;
}


/* Address: 8ceb1cd4 */

void FUN_8ceb1cd4(undefined8 *param_1,int *param_2,uint param_3)

{
  byte bVar1;
  short sVar2;
  byte bVar3;
  uint uVar4;
  short *psVar5;
  int iVar6;
  byte bVar7;
  byte *pbVar8;
  byte *pbVar9;
  byte *pbVar10;
  short *psVar11;
  int iVar12;
  int iVar13;
  undefined8 uVar14;
  undefined8 uVar16;
  undefined8 uVar18;
  char in_FPSCR_SZ;
  undefined4 uVar15;
  undefined4 uVar17;
  undefined4 uVar19;
  
  iVar6 = *param_2;
  psVar11 = (short *)param_2[1];
  pbVar8 = (byte *)param_2[2];
  iVar12 = 6;
  do {
    uVar4 = param_3 & 0x80000000;
    param_3 = param_3 << 1;
    if (uVar4 == 0) {
      psVar11 = psVar11 + 0x40;
    }
    else {
      bVar7 = *pbVar8;
      iVar13 = 8;
      sVar2 = *psVar11;
      pbVar10 = pbVar8 + 1;
      psVar5 = psVar11 + 1;
      do {
        psVar11 = psVar5;
        pbVar9 = pbVar10;
        bVar1 = pbVar9[1];
        bVar3 = *(byte *)(iVar6 + (uint)*pbVar9 + (int)*psVar11);
        *pbVar8 = *(byte *)(iVar6 + (uint)bVar7 + (int)sVar2);
        pbVar8[1] = bVar3;
        bVar7 = pbVar9[3];
        bVar3 = *(byte *)(iVar6 + (uint)pbVar9[2] + (int)psVar11[2]);
        pbVar8[2] = *(byte *)(iVar6 + (uint)bVar1 + (int)psVar11[1]);
        pbVar8[3] = bVar3;
        bVar1 = pbVar9[5];
        bVar3 = *(byte *)(iVar6 + (uint)pbVar9[4] + (int)psVar11[4]);
        pbVar8[4] = *(byte *)(iVar6 + (uint)bVar7 + (int)psVar11[3]);
        pbVar8[5] = bVar3;
        bVar7 = pbVar9[7];
        bVar3 = *(byte *)(iVar6 + (uint)pbVar9[6] + (int)psVar11[6]);
        iVar13 = iVar13 + -1;
        pbVar8[6] = *(byte *)(iVar6 + (uint)bVar1 + (int)psVar11[5]);
        pbVar8[7] = bVar3;
        pbVar8 = pbVar8 + 8;
        sVar2 = psVar11[7];
        pbVar10 = pbVar9 + 8;
        psVar5 = psVar11 + 8;
      } while (iVar13 != 0);
      pbVar8 = pbVar9 + -0x39;
      psVar11 = psVar11 + 7;
    }
    if (in_FPSCR_SZ == '\0') {
      uVar15 = *(undefined4 *)(pbVar8 + 4);
      uVar17 = *(undefined4 *)(pbVar8 + 8);
      uVar19 = *(undefined4 *)(pbVar8 + 0xc);
      pbVar10 = pbVar8 + 0x10;
      *(undefined4 *)param_1 = *(undefined4 *)pbVar8;
      *(undefined4 *)(param_1 + 1) = uVar15;
      *(undefined4 *)(param_1 + 2) = uVar17;
      *(undefined4 *)(param_1 + 3) = uVar19;
      uVar15 = *(undefined4 *)(pbVar8 + 0x14);
      uVar17 = *(undefined4 *)(pbVar8 + 0x18);
      uVar19 = *(undefined4 *)(pbVar8 + 0x1c);
      pbVar8 = pbVar8 + 0x20;
      *(undefined4 *)(param_1 + 4) = *(undefined4 *)pbVar10;
      *(undefined4 *)(param_1 + 5) = uVar15;
      *(undefined4 *)(param_1 + 6) = uVar17;
      *(undefined4 *)(param_1 + 7) = uVar19;
    }
    else {
      uVar14 = *(undefined8 *)(pbVar8 + 8);
      uVar16 = *(undefined8 *)(pbVar8 + 0x10);
      uVar18 = *(undefined8 *)(pbVar8 + 0x18);
      pbVar10 = pbVar8 + 0x20;
      *param_1 = *(undefined8 *)pbVar8;
      param_1[1] = uVar14;
      param_1[2] = uVar16;
      param_1[3] = uVar18;
      uVar14 = *(undefined8 *)(pbVar8 + 0x28);
      uVar16 = *(undefined8 *)(pbVar8 + 0x30);
      uVar18 = *(undefined8 *)(pbVar8 + 0x38);
      pbVar8 = pbVar8 + 0x40;
      param_1[4] = *(undefined8 *)pbVar10;
      param_1[5] = uVar14;
      param_1[6] = uVar16;
      param_1[7] = uVar18;
    }
    iVar12 = iVar12 + -1;
    param_1 = param_1 + 8;
  } while (iVar12 != 0);
  return;
}


/* Address: 8ceb1dc0 */

void FUN_8ceb1dc0(undefined4 param_1,undefined4 param_2,undefined8 *param_3,int *param_4,
                 int *param_5)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  int *piVar4;
  int *piVar5;
  undefined8 *puVar6;
  int *piVar7;
  undefined4 in_fr1;
  undefined8 uVar8;
  int iVar10;
  undefined8 uVar9;
  undefined4 in_fr3;
  undefined8 uVar11;
  int iVar12;
  undefined4 uVar13;
  undefined8 uVar14;
  int iVar15;
  undefined8 uVar16;
  char in_FPSCR_SZ;
  int iVar17;
  
  iVar2 = *param_4;
  iVar1 = (int)(short)param_5[3];
  piVar5 = (int *)(*param_5 + iVar2);
  if (in_FPSCR_SZ == '\0') {
    uVar8 = CONCAT44(*(undefined4 *)param_3,in_fr1);
    param_3 = (undefined8 *)((int)param_3 + 4);
  }
  else {
    uVar8 = *param_3;
    param_3 = param_3 + 1;
  }
  if (in_FPSCR_SZ == '\0') {
    uVar11 = CONCAT44(*(undefined4 *)param_3,in_fr3);
    uVar14 = CONCAT44(*(undefined4 *)((int)param_3 + 4),param_1);
    uVar16 = CONCAT44(*(undefined4 *)(param_3 + 1),param_2);
    piVar4 = (int *)((int)param_3 + 0xc);
  }
  else {
    uVar11 = *param_3;
    uVar14 = param_3[1];
    uVar16 = param_3[2];
    piVar4 = (int *)(param_3 + 3);
  }
  iVar10 = (int)((ulonglong)uVar8 >> 0x20);
  iVar12 = (int)((ulonglong)uVar11 >> 0x20);
  iVar15 = (int)((ulonglong)uVar14 >> 0x20);
  iVar17 = (int)((ulonglong)uVar16 >> 0x20);
  if (((uint)piVar5 & 0x1f) == 0) {
    *piVar5 = iVar1;
    if (in_FPSCR_SZ == '\0') {
      *piVar5 = iVar10;
    }
    else {
      *(undefined8 *)piVar5 = uVar8;
    }
    piVar5 = (int *)((int)piVar5 + iVar1);
    *piVar5 = iVar1;
    if (in_FPSCR_SZ == '\0') {
      *piVar5 = iVar12;
    }
    else {
      *(undefined8 *)piVar5 = uVar11;
    }
    piVar5 = (int *)((int)piVar5 + iVar1);
    *piVar5 = iVar1;
    if (in_FPSCR_SZ == '\0') {
      *piVar5 = iVar15;
    }
    else {
      *(undefined8 *)piVar5 = uVar14;
    }
    piVar5 = (int *)((int)piVar5 + iVar1);
    *piVar5 = iVar1;
    if (in_FPSCR_SZ == '\0') {
      *piVar5 = iVar17;
    }
    else {
      *(undefined8 *)piVar5 = uVar16;
    }
    piVar5 = (int *)((int)piVar5 + iVar1);
    *piVar5 = iVar1;
    if (in_FPSCR_SZ == '\0') {
      uVar9 = CONCAT44(*piVar4,(int)uVar8);
      uVar11 = CONCAT44(piVar4[1],(int)uVar11);
      uVar14 = CONCAT44(piVar4[2],(int)uVar14);
      uVar8 = CONCAT44(piVar4[3],(int)uVar16);
      piVar7 = piVar4 + 4;
      *piVar5 = *piVar4;
    }
    else {
      uVar9 = *(undefined8 *)piVar4;
      uVar11 = *(undefined8 *)(piVar4 + 2);
      uVar14 = *(undefined8 *)(piVar4 + 4);
      uVar8 = *(undefined8 *)(piVar4 + 6);
      piVar7 = piVar4 + 8;
      *(undefined8 *)piVar5 = uVar9;
    }
    piVar5 = (int *)((int)piVar5 + iVar1);
    *piVar5 = iVar1;
    if (in_FPSCR_SZ == '\0') {
      *piVar5 = (int)((ulonglong)uVar11 >> 0x20);
    }
    else {
      *(undefined8 *)piVar5 = uVar11;
    }
    piVar5 = (int *)((int)piVar5 + iVar1);
    *piVar5 = iVar1;
    if (in_FPSCR_SZ == '\0') {
      *piVar5 = (int)((ulonglong)uVar14 >> 0x20);
    }
    else {
      *(undefined8 *)piVar5 = uVar14;
    }
    piVar5 = (int *)((int)piVar5 + iVar1);
    *piVar5 = iVar1;
    if (in_FPSCR_SZ == '\0') {
      *piVar5 = (int)((ulonglong)uVar8 >> 0x20);
    }
    else {
      *(undefined8 *)piVar5 = uVar8;
    }
    if (in_FPSCR_SZ == '\0') {
      uVar16 = CONCAT44(*piVar7,(int)uVar9);
      uVar11 = CONCAT44(piVar7[1],(int)uVar11);
      piVar7 = piVar7 + 2;
    }
    else {
      uVar16 = *(undefined8 *)piVar7;
      uVar11 = *(undefined8 *)(piVar7 + 2);
      piVar7 = piVar7 + 4;
    }
    piVar5 = (int *)(param_5[1] + iVar2);
    *piVar5 = iVar1;
    if (in_FPSCR_SZ == '\0') {
      uVar14 = CONCAT44(*piVar7,(int)uVar14);
      uVar8 = CONCAT44(piVar7[1],(int)uVar8);
      piVar7 = piVar7 + 2;
      *piVar5 = (int)((ulonglong)uVar16 >> 0x20);
    }
    else {
      uVar14 = *(undefined8 *)piVar7;
      uVar8 = *(undefined8 *)(piVar7 + 2);
      piVar7 = piVar7 + 4;
      *(undefined8 *)piVar5 = uVar16;
    }
    piVar5 = (int *)((int)piVar5 + iVar1);
    *piVar5 = iVar1;
    if (in_FPSCR_SZ == '\0') {
      *piVar5 = (int)((ulonglong)uVar11 >> 0x20);
    }
    else {
      *(undefined8 *)piVar5 = uVar11;
    }
    piVar5 = (int *)((int)piVar5 + iVar1);
    *piVar5 = iVar1;
    if (in_FPSCR_SZ == '\0') {
      *piVar5 = (int)((ulonglong)uVar14 >> 0x20);
    }
    else {
      *(undefined8 *)piVar5 = uVar14;
    }
    piVar5 = (int *)((int)piVar5 + iVar1);
    *piVar5 = iVar1;
    if (in_FPSCR_SZ == '\0') {
      *piVar5 = (int)((ulonglong)uVar8 >> 0x20);
    }
    else {
      *(undefined8 *)piVar5 = uVar8;
    }
    piVar5 = (int *)((int)piVar5 + iVar1);
    *piVar5 = iVar1;
    if (in_FPSCR_SZ == '\0') {
      uVar16 = CONCAT44(*piVar7,(int)uVar16);
      uVar11 = CONCAT44(piVar7[1],(int)uVar11);
      uVar14 = CONCAT44(piVar7[2],(int)uVar14);
      uVar8 = CONCAT44(piVar7[3],(int)uVar8);
      piVar4 = piVar7 + 4;
      *piVar5 = *piVar7;
    }
    else {
      uVar16 = *(undefined8 *)piVar7;
      uVar11 = *(undefined8 *)(piVar7 + 2);
      uVar14 = *(undefined8 *)(piVar7 + 4);
      uVar8 = *(undefined8 *)(piVar7 + 6);
      piVar4 = piVar7 + 8;
      *(undefined8 *)piVar5 = uVar16;
    }
    uVar13 = (undefined4)uVar16;
    piVar5 = (int *)((int)piVar5 + iVar1);
    *piVar5 = iVar1;
    if (in_FPSCR_SZ == '\0') {
      *piVar5 = (int)((ulonglong)uVar11 >> 0x20);
    }
    else {
      *(undefined8 *)piVar5 = uVar11;
    }
    piVar5 = (int *)((int)piVar5 + iVar1);
    *piVar5 = iVar1;
    if (in_FPSCR_SZ == '\0') {
      *piVar5 = (int)((ulonglong)uVar14 >> 0x20);
    }
    else {
      *(undefined8 *)piVar5 = uVar14;
    }
    piVar5 = (int *)((int)piVar5 + iVar1);
    *piVar5 = iVar1;
    if (in_FPSCR_SZ == '\0') {
      *piVar5 = (int)((ulonglong)uVar8 >> 0x20);
    }
    else {
      *(undefined8 *)piVar5 = uVar8;
    }
  }
  else {
    if (in_FPSCR_SZ == '\0') {
      *piVar5 = iVar10;
    }
    else {
      *(undefined8 *)piVar5 = uVar8;
    }
    piVar5 = (int *)((int)piVar5 + iVar1);
    if (in_FPSCR_SZ == '\0') {
      *piVar5 = iVar12;
    }
    else {
      *(undefined8 *)piVar5 = uVar11;
    }
    piVar5 = (int *)((int)piVar5 + iVar1);
    if (in_FPSCR_SZ == '\0') {
      *piVar5 = iVar15;
    }
    else {
      *(undefined8 *)piVar5 = uVar14;
    }
    piVar5 = (int *)((int)piVar5 + iVar1);
    if (in_FPSCR_SZ == '\0') {
      *piVar5 = iVar17;
    }
    else {
      *(undefined8 *)piVar5 = uVar16;
    }
    piVar5 = (int *)((int)piVar5 + iVar1);
    if (in_FPSCR_SZ == '\0') {
      uVar9 = CONCAT44(*piVar4,(int)uVar8);
      uVar11 = CONCAT44(piVar4[1],(int)uVar11);
      uVar14 = CONCAT44(piVar4[2],(int)uVar14);
      uVar8 = CONCAT44(piVar4[3],(int)uVar16);
      piVar7 = piVar4 + 4;
      *piVar5 = *piVar4;
    }
    else {
      uVar9 = *(undefined8 *)piVar4;
      uVar11 = *(undefined8 *)(piVar4 + 2);
      uVar14 = *(undefined8 *)(piVar4 + 4);
      uVar8 = *(undefined8 *)(piVar4 + 6);
      piVar7 = piVar4 + 8;
      *(undefined8 *)piVar5 = uVar9;
    }
    puVar6 = (undefined8 *)((int)piVar5 + iVar1);
    if (in_FPSCR_SZ == '\0') {
      *(int *)puVar6 = (int)((ulonglong)uVar11 >> 0x20);
    }
    else {
      *puVar6 = uVar11;
    }
    puVar6 = (undefined8 *)((int)puVar6 + iVar1);
    if (in_FPSCR_SZ == '\0') {
      *(int *)puVar6 = (int)((ulonglong)uVar14 >> 0x20);
    }
    else {
      *puVar6 = uVar14;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)puVar6 + iVar1) = (int)((ulonglong)uVar8 >> 0x20);
    }
    else {
      *(undefined8 *)((int)puVar6 + iVar1) = uVar8;
    }
    if (in_FPSCR_SZ == '\0') {
      uVar16 = CONCAT44(*piVar7,(int)uVar9);
      uVar11 = CONCAT44(piVar7[1],(int)uVar11);
      piVar7 = piVar7 + 2;
    }
    else {
      uVar16 = *(undefined8 *)piVar7;
      uVar11 = *(undefined8 *)(piVar7 + 2);
      piVar7 = piVar7 + 4;
    }
    puVar6 = (undefined8 *)(param_5[1] + iVar2);
    if (in_FPSCR_SZ == '\0') {
      uVar14 = CONCAT44(*piVar7,(int)uVar14);
      uVar8 = CONCAT44(piVar7[1],(int)uVar8);
      piVar7 = piVar7 + 2;
      *(int *)puVar6 = (int)((ulonglong)uVar16 >> 0x20);
    }
    else {
      uVar14 = *(undefined8 *)piVar7;
      uVar8 = *(undefined8 *)(piVar7 + 2);
      piVar7 = piVar7 + 4;
      *puVar6 = uVar16;
    }
    puVar6 = (undefined8 *)((int)puVar6 + iVar1);
    if (in_FPSCR_SZ == '\0') {
      *(int *)puVar6 = (int)((ulonglong)uVar11 >> 0x20);
    }
    else {
      *puVar6 = uVar11;
    }
    puVar6 = (undefined8 *)((int)puVar6 + iVar1);
    if (in_FPSCR_SZ == '\0') {
      *(int *)puVar6 = (int)((ulonglong)uVar14 >> 0x20);
    }
    else {
      *puVar6 = uVar14;
    }
    puVar6 = (undefined8 *)((int)puVar6 + iVar1);
    if (in_FPSCR_SZ == '\0') {
      *(int *)puVar6 = (int)((ulonglong)uVar8 >> 0x20);
    }
    else {
      *puVar6 = uVar8;
    }
    piVar5 = (int *)((int)puVar6 + iVar1);
    if (in_FPSCR_SZ == '\0') {
      uVar16 = CONCAT44(*piVar7,(int)uVar16);
      uVar11 = CONCAT44(piVar7[1],(int)uVar11);
      uVar14 = CONCAT44(piVar7[2],(int)uVar14);
      uVar8 = CONCAT44(piVar7[3],(int)uVar8);
      piVar4 = piVar7 + 4;
      *piVar5 = *piVar7;
    }
    else {
      uVar16 = *(undefined8 *)piVar7;
      uVar11 = *(undefined8 *)(piVar7 + 2);
      uVar14 = *(undefined8 *)(piVar7 + 4);
      uVar8 = *(undefined8 *)(piVar7 + 6);
      piVar4 = piVar7 + 8;
      *(undefined8 *)piVar5 = uVar16;
    }
    uVar13 = (undefined4)uVar16;
    puVar6 = (undefined8 *)((int)piVar5 + iVar1);
    if (in_FPSCR_SZ == '\0') {
      *(int *)puVar6 = (int)((ulonglong)uVar11 >> 0x20);
    }
    else {
      *puVar6 = uVar11;
    }
    puVar6 = (undefined8 *)((int)puVar6 + iVar1);
    if (in_FPSCR_SZ == '\0') {
      *(int *)puVar6 = (int)((ulonglong)uVar14 >> 0x20);
    }
    else {
      *puVar6 = uVar14;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)puVar6 + iVar1) = (int)((ulonglong)uVar8 >> 0x20);
    }
    else {
      *(undefined8 *)((int)puVar6 + iVar1) = uVar8;
    }
  }
  iVar1 = (int)*(short *)((int)param_5 + 0xe);
  puVar6 = (undefined8 *)(param_5[2] + param_4[1]);
  if (in_FPSCR_SZ == '\0') {
    uVar8 = CONCAT44(piVar4[0x10],uVar13);
    piVar5 = piVar4 + 0x11;
  }
  else {
    uVar8 = *(undefined8 *)(piVar4 + 0x10);
    piVar5 = piVar4 + 0x12;
  }
  if (in_FPSCR_SZ == '\0') {
    uVar11 = CONCAT44(*piVar4,(int)uVar11);
    piVar4 = piVar4 + 1;
  }
  else {
    uVar11 = *(undefined8 *)piVar4;
    piVar4 = piVar4 + 2;
  }
  piVar7 = (int *)(puVar6 + 1);
  iVar2 = (int)((ulonglong)uVar8 >> 0x20);
  uVar13 = (undefined4)((ulonglong)uVar11 >> 0x20);
  if (((uint)puVar6 & 0x1f) == 0) {
    *piVar7 = iVar1;
    if (in_FPSCR_SZ == '\0') {
      *piVar7 = iVar2;
      puVar6 = (undefined8 *)((int)puVar6 + 4);
      *(undefined4 *)puVar6 = uVar13;
    }
    else {
      *(undefined8 *)piVar7 = uVar8;
      *puVar6 = uVar11;
    }
    iVar1 = iVar1 + 8;
    piVar7 = (int *)((int)puVar6 + iVar1);
    if (in_FPSCR_SZ == '\0') {
      uVar14 = CONCAT44(*piVar5,(int)uVar14);
      piVar5 = piVar5 + 1;
    }
    else {
      uVar14 = *(undefined8 *)piVar5;
      piVar5 = piVar5 + 2;
    }
    *piVar7 = iVar1;
    if (in_FPSCR_SZ == '\0') {
      iVar2 = *piVar4;
      piVar4 = piVar4 + 1;
      *piVar7 = (int)((ulonglong)uVar14 >> 0x20);
      piVar7 = piVar7 + -1;
      *piVar7 = iVar2;
    }
    else {
      uVar11 = *(undefined8 *)piVar4;
      piVar4 = piVar4 + 2;
      *(undefined8 *)piVar7 = uVar14;
      piVar7 = piVar7 + -2;
      *(undefined8 *)piVar7 = uVar11;
    }
    piVar7 = (int *)((int)piVar7 + iVar1);
    if (in_FPSCR_SZ == '\0') {
      uVar8 = CONCAT44(*piVar5,(int)uVar8);
      piVar5 = piVar5 + 1;
    }
    else {
      uVar8 = *(undefined8 *)piVar5;
      piVar5 = piVar5 + 2;
    }
    *piVar7 = iVar1;
    if (in_FPSCR_SZ == '\0') {
      iVar2 = *piVar4;
      piVar4 = piVar4 + 1;
      *piVar7 = (int)((ulonglong)uVar8 >> 0x20);
      piVar7 = piVar7 + -1;
      *piVar7 = iVar2;
    }
    else {
      uVar11 = *(undefined8 *)piVar4;
      piVar4 = piVar4 + 2;
      *(undefined8 *)piVar7 = uVar8;
      piVar7 = piVar7 + -2;
      *(undefined8 *)piVar7 = uVar11;
    }
    piVar7 = (int *)((int)piVar7 + iVar1);
    if (in_FPSCR_SZ == '\0') {
      uVar14 = CONCAT44(*piVar5,(int)uVar14);
      piVar5 = piVar5 + 1;
    }
    else {
      uVar14 = *(undefined8 *)piVar5;
      piVar5 = piVar5 + 2;
    }
    *piVar7 = iVar1;
    if (in_FPSCR_SZ == '\0') {
      iVar2 = *piVar4;
      piVar4 = piVar4 + 1;
      *piVar7 = (int)((ulonglong)uVar14 >> 0x20);
      piVar7 = piVar7 + -1;
      *piVar7 = iVar2;
    }
    else {
      uVar11 = *(undefined8 *)piVar4;
      piVar4 = piVar4 + 2;
      *(undefined8 *)piVar7 = uVar14;
      piVar7 = piVar7 + -2;
      *(undefined8 *)piVar7 = uVar11;
    }
    piVar7 = (int *)((int)piVar7 + iVar1);
    if (in_FPSCR_SZ == '\0') {
      uVar8 = CONCAT44(*piVar5,(int)uVar8);
      piVar5 = piVar5 + 1;
    }
    else {
      uVar8 = *(undefined8 *)piVar5;
      piVar5 = piVar5 + 2;
    }
    *piVar7 = iVar1;
    if (in_FPSCR_SZ == '\0') {
      iVar2 = *piVar4;
      piVar4 = piVar4 + 1;
      *piVar7 = (int)((ulonglong)uVar8 >> 0x20);
      piVar7 = piVar7 + -1;
      *piVar7 = iVar2;
    }
    else {
      uVar11 = *(undefined8 *)piVar4;
      piVar4 = piVar4 + 2;
      *(undefined8 *)piVar7 = uVar8;
      piVar7 = piVar7 + -2;
      *(undefined8 *)piVar7 = uVar11;
    }
    piVar7 = (int *)((int)piVar7 + iVar1);
    if (in_FPSCR_SZ == '\0') {
      uVar14 = CONCAT44(*piVar5,(int)uVar14);
      piVar5 = piVar5 + 1;
    }
    else {
      uVar14 = *(undefined8 *)piVar5;
      piVar5 = piVar5 + 2;
    }
    *piVar7 = iVar1;
    if (in_FPSCR_SZ == '\0') {
      iVar2 = *piVar4;
      piVar4 = piVar4 + 1;
      *piVar7 = (int)((ulonglong)uVar14 >> 0x20);
      piVar7 = piVar7 + -1;
      *piVar7 = iVar2;
    }
    else {
      uVar11 = *(undefined8 *)piVar4;
      piVar4 = piVar4 + 2;
      *(undefined8 *)piVar7 = uVar14;
      piVar7 = piVar7 + -2;
      *(undefined8 *)piVar7 = uVar11;
    }
    piVar7 = (int *)((int)piVar7 + iVar1);
    if (in_FPSCR_SZ == '\0') {
      uVar8 = CONCAT44(*piVar5,(int)uVar8);
      piVar5 = piVar5 + 1;
    }
    else {
      uVar8 = *(undefined8 *)piVar5;
      piVar5 = piVar5 + 2;
    }
    *piVar7 = iVar1;
    if (in_FPSCR_SZ == '\0') {
      iVar2 = *piVar4;
      piVar4 = piVar4 + 1;
      *piVar7 = (int)((ulonglong)uVar8 >> 0x20);
      piVar7 = piVar7 + -1;
      *piVar7 = iVar2;
    }
    else {
      uVar11 = *(undefined8 *)piVar4;
      piVar4 = piVar4 + 2;
      *(undefined8 *)piVar7 = uVar8;
      piVar7 = piVar7 + -2;
      *(undefined8 *)piVar7 = uVar11;
    }
    piVar7 = (int *)((int)piVar7 + iVar1);
    if (in_FPSCR_SZ == '\0') {
      uVar14 = CONCAT44(*piVar5,(int)uVar14);
    }
    else {
      uVar14 = *(undefined8 *)piVar5;
    }
    *piVar7 = iVar1;
    if (in_FPSCR_SZ == '\0') {
      iVar2 = *piVar4;
      *piVar7 = (int)((ulonglong)uVar14 >> 0x20);
      piVar7 = piVar7 + -1;
      *piVar7 = iVar2;
    }
    else {
      uVar11 = *(undefined8 *)piVar4;
      *(undefined8 *)piVar7 = uVar14;
      piVar7 = piVar7 + -2;
      *(undefined8 *)piVar7 = uVar11;
    }
    piVar7 = (int *)((int)piVar7 + iVar1);
    if (in_FPSCR_SZ == '\0') {
      uVar8 = CONCAT44(piVar5[0x12],(int)uVar8);
      piVar5 = piVar5 + 0x13;
    }
    else {
      uVar8 = *(undefined8 *)(piVar5 + 0x12);
      piVar5 = piVar5 + 0x14;
    }
    *piVar7 = iVar1;
    if (in_FPSCR_SZ == '\0') {
      iVar2 = piVar4[0x12];
      piVar4 = piVar4 + 0x13;
      *piVar7 = (int)((ulonglong)uVar8 >> 0x20);
      piVar7 = piVar7 + -1;
      *piVar7 = iVar2;
    }
    else {
      uVar11 = *(undefined8 *)(piVar4 + 0x12);
      piVar4 = piVar4 + 0x14;
      *(undefined8 *)piVar7 = uVar8;
      piVar7 = piVar7 + -2;
      *(undefined8 *)piVar7 = uVar11;
    }
    piVar7 = (int *)((int)piVar7 + iVar1);
    if (in_FPSCR_SZ == '\0') {
      uVar14 = CONCAT44(*piVar5,(int)uVar14);
      piVar5 = piVar5 + 1;
    }
    else {
      uVar14 = *(undefined8 *)piVar5;
      piVar5 = piVar5 + 2;
    }
    *piVar7 = iVar1;
    if (in_FPSCR_SZ == '\0') {
      iVar2 = *piVar4;
      piVar4 = piVar4 + 1;
      *piVar7 = (int)((ulonglong)uVar14 >> 0x20);
      piVar7 = piVar7 + -1;
      *piVar7 = iVar2;
    }
    else {
      uVar11 = *(undefined8 *)piVar4;
      piVar4 = piVar4 + 2;
      *(undefined8 *)piVar7 = uVar14;
      piVar7 = piVar7 + -2;
      *(undefined8 *)piVar7 = uVar11;
    }
    piVar7 = (int *)((int)piVar7 + iVar1);
    if (in_FPSCR_SZ == '\0') {
      uVar8 = CONCAT44(*piVar5,(int)uVar8);
      piVar5 = piVar5 + 1;
    }
    else {
      uVar8 = *(undefined8 *)piVar5;
      piVar5 = piVar5 + 2;
    }
    *piVar7 = iVar1;
    if (in_FPSCR_SZ == '\0') {
      iVar2 = *piVar4;
      piVar4 = piVar4 + 1;
      *piVar7 = (int)((ulonglong)uVar8 >> 0x20);
      piVar7 = piVar7 + -1;
      *piVar7 = iVar2;
    }
    else {
      uVar11 = *(undefined8 *)piVar4;
      piVar4 = piVar4 + 2;
      *(undefined8 *)piVar7 = uVar8;
      piVar7 = piVar7 + -2;
      *(undefined8 *)piVar7 = uVar11;
    }
    piVar7 = (int *)((int)piVar7 + iVar1);
    if (in_FPSCR_SZ == '\0') {
      uVar14 = CONCAT44(*piVar5,(int)uVar14);
      piVar5 = piVar5 + 1;
    }
    else {
      uVar14 = *(undefined8 *)piVar5;
      piVar5 = piVar5 + 2;
    }
    *piVar7 = iVar1;
    if (in_FPSCR_SZ == '\0') {
      iVar2 = *piVar4;
      piVar4 = piVar4 + 1;
      *piVar7 = (int)((ulonglong)uVar14 >> 0x20);
      piVar7 = piVar7 + -1;
      *piVar7 = iVar2;
    }
    else {
      uVar11 = *(undefined8 *)piVar4;
      piVar4 = piVar4 + 2;
      *(undefined8 *)piVar7 = uVar14;
      piVar7 = piVar7 + -2;
      *(undefined8 *)piVar7 = uVar11;
    }
    piVar7 = (int *)((int)piVar7 + iVar1);
    if (in_FPSCR_SZ == '\0') {
      uVar8 = CONCAT44(*piVar5,(int)uVar8);
      piVar5 = piVar5 + 1;
    }
    else {
      uVar8 = *(undefined8 *)piVar5;
      piVar5 = piVar5 + 2;
    }
    *piVar7 = iVar1;
    if (in_FPSCR_SZ == '\0') {
      iVar2 = *piVar4;
      piVar4 = piVar4 + 1;
      *piVar7 = (int)((ulonglong)uVar8 >> 0x20);
      piVar7 = piVar7 + -1;
      *piVar7 = iVar2;
    }
    else {
      uVar11 = *(undefined8 *)piVar4;
      piVar4 = piVar4 + 2;
      *(undefined8 *)piVar7 = uVar8;
      piVar7 = piVar7 + -2;
      *(undefined8 *)piVar7 = uVar11;
    }
    piVar7 = (int *)((int)piVar7 + iVar1);
    if (in_FPSCR_SZ == '\0') {
      uVar14 = CONCAT44(*piVar5,(int)uVar14);
      piVar5 = piVar5 + 1;
    }
    else {
      uVar14 = *(undefined8 *)piVar5;
      piVar5 = piVar5 + 2;
    }
    *piVar7 = iVar1;
    if (in_FPSCR_SZ == '\0') {
      iVar2 = *piVar4;
      piVar4 = piVar4 + 1;
      *piVar7 = (int)((ulonglong)uVar14 >> 0x20);
      piVar7 = piVar7 + -1;
      *piVar7 = iVar2;
    }
    else {
      uVar11 = *(undefined8 *)piVar4;
      piVar4 = piVar4 + 2;
      *(undefined8 *)piVar7 = uVar14;
      piVar7 = piVar7 + -2;
      *(undefined8 *)piVar7 = uVar11;
    }
    piVar7 = (int *)((int)piVar7 + iVar1);
    if (in_FPSCR_SZ == '\0') {
      uVar8 = CONCAT44(*piVar5,(int)uVar8);
      piVar5 = piVar5 + 1;
    }
    else {
      uVar8 = *(undefined8 *)piVar5;
      piVar5 = piVar5 + 2;
    }
    *piVar7 = iVar1;
    if (in_FPSCR_SZ == '\0') {
      iVar2 = *piVar4;
      piVar4 = piVar4 + 1;
      *piVar7 = (int)((ulonglong)uVar8 >> 0x20);
      piVar7 = piVar7 + -1;
      *piVar7 = iVar2;
    }
    else {
      uVar11 = *(undefined8 *)piVar4;
      piVar4 = piVar4 + 2;
      *(undefined8 *)piVar7 = uVar8;
      piVar7 = piVar7 + -2;
      *(undefined8 *)piVar7 = uVar11;
    }
    piVar7 = (int *)((int)piVar7 + iVar1);
    if (in_FPSCR_SZ == '\0') {
      uVar8 = CONCAT44(*piVar5,(int)uVar14);
    }
    else {
      uVar8 = *(undefined8 *)piVar5;
    }
    *piVar7 = iVar1;
    if (in_FPSCR_SZ == '\0') {
      iVar1 = *piVar4;
      *piVar7 = (int)((ulonglong)uVar8 >> 0x20);
      piVar7[-1] = iVar1;
    }
    else {
      uVar14 = *(undefined8 *)piVar4;
      *(undefined8 *)piVar7 = uVar8;
      *(undefined8 *)(piVar7 + -2) = uVar14;
    }
    return;
  }
  if (in_FPSCR_SZ == '\0') {
    *piVar7 = iVar2;
    puVar6 = (undefined8 *)((int)puVar6 + 4);
    *(undefined4 *)puVar6 = uVar13;
  }
  else {
    *(undefined8 *)piVar7 = uVar8;
    *puVar6 = uVar11;
  }
  iVar1 = iVar1 + 8;
  piVar7 = (int *)((int)puVar6 + iVar1);
  if (in_FPSCR_SZ == '\0') {
    piVar3 = piVar5 + 1;
    iVar2 = *piVar4;
    piVar4 = piVar4 + 1;
    *piVar7 = *piVar5;
    piVar7 = piVar7 + -1;
    *piVar7 = iVar2;
  }
  else {
    piVar3 = piVar5 + 2;
    uVar8 = *(undefined8 *)piVar4;
    piVar4 = piVar4 + 2;
    *(undefined8 *)piVar7 = *(undefined8 *)piVar5;
    piVar7 = piVar7 + -2;
    *(undefined8 *)piVar7 = uVar8;
  }
  piVar7 = (int *)((int)piVar7 + iVar1);
  if (in_FPSCR_SZ == '\0') {
    piVar5 = piVar3 + 1;
    iVar2 = *piVar4;
    piVar4 = piVar4 + 1;
    *piVar7 = *piVar3;
    piVar7 = piVar7 + -1;
    *piVar7 = iVar2;
  }
  else {
    piVar5 = piVar3 + 2;
    uVar8 = *(undefined8 *)piVar4;
    piVar4 = piVar4 + 2;
    *(undefined8 *)piVar7 = *(undefined8 *)piVar3;
    piVar7 = piVar7 + -2;
    *(undefined8 *)piVar7 = uVar8;
  }
  piVar7 = (int *)((int)piVar7 + iVar1);
  if (in_FPSCR_SZ == '\0') {
    piVar3 = piVar5 + 1;
    iVar2 = *piVar4;
    piVar4 = piVar4 + 1;
    *piVar7 = *piVar5;
    piVar7 = piVar7 + -1;
    *piVar7 = iVar2;
  }
  else {
    piVar3 = piVar5 + 2;
    uVar8 = *(undefined8 *)piVar4;
    piVar4 = piVar4 + 2;
    *(undefined8 *)piVar7 = *(undefined8 *)piVar5;
    piVar7 = piVar7 + -2;
    *(undefined8 *)piVar7 = uVar8;
  }
  piVar7 = (int *)((int)piVar7 + iVar1);
  if (in_FPSCR_SZ == '\0') {
    piVar5 = piVar3 + 1;
    iVar2 = *piVar4;
    piVar4 = piVar4 + 1;
    *piVar7 = *piVar3;
    piVar7 = piVar7 + -1;
    *piVar7 = iVar2;
  }
  else {
    piVar5 = piVar3 + 2;
    uVar8 = *(undefined8 *)piVar4;
    piVar4 = piVar4 + 2;
    *(undefined8 *)piVar7 = *(undefined8 *)piVar3;
    piVar7 = piVar7 + -2;
    *(undefined8 *)piVar7 = uVar8;
  }
  piVar7 = (int *)((int)piVar7 + iVar1);
  if (in_FPSCR_SZ == '\0') {
    piVar3 = piVar5 + 1;
    iVar2 = *piVar4;
    piVar4 = piVar4 + 1;
    *piVar7 = *piVar5;
    piVar7 = piVar7 + -1;
    *piVar7 = iVar2;
  }
  else {
    piVar3 = piVar5 + 2;
    uVar8 = *(undefined8 *)piVar4;
    piVar4 = piVar4 + 2;
    *(undefined8 *)piVar7 = *(undefined8 *)piVar5;
    piVar7 = piVar7 + -2;
    *(undefined8 *)piVar7 = uVar8;
  }
  piVar7 = (int *)((int)piVar7 + iVar1);
  if (in_FPSCR_SZ == '\0') {
    piVar5 = piVar3 + 1;
    iVar2 = *piVar4;
    piVar4 = piVar4 + 1;
    *piVar7 = *piVar3;
    piVar7 = piVar7 + -1;
    *piVar7 = iVar2;
  }
  else {
    piVar5 = piVar3 + 2;
    uVar8 = *(undefined8 *)piVar4;
    piVar4 = piVar4 + 2;
    *(undefined8 *)piVar7 = *(undefined8 *)piVar3;
    piVar7 = piVar7 + -2;
    *(undefined8 *)piVar7 = uVar8;
  }
  piVar7 = (int *)((int)piVar7 + iVar1);
  if (in_FPSCR_SZ == '\0') {
    iVar2 = *piVar4;
    *piVar7 = *piVar5;
    piVar7 = piVar7 + -1;
    *piVar7 = iVar2;
  }
  else {
    uVar8 = *(undefined8 *)piVar4;
    *(undefined8 *)piVar7 = *(undefined8 *)piVar5;
    piVar7 = piVar7 + -2;
    *(undefined8 *)piVar7 = uVar8;
  }
  piVar7 = (int *)((int)piVar7 + iVar1);
  if (in_FPSCR_SZ == '\0') {
    piVar3 = piVar5 + 0x13;
    iVar2 = piVar4[0x12];
    piVar4 = piVar4 + 0x13;
    *piVar7 = piVar5[0x12];
    piVar7 = piVar7 + -1;
    *piVar7 = iVar2;
  }
  else {
    piVar3 = piVar5 + 0x14;
    uVar8 = *(undefined8 *)(piVar4 + 0x12);
    piVar4 = piVar4 + 0x14;
    *(undefined8 *)piVar7 = *(undefined8 *)(piVar5 + 0x12);
    piVar7 = piVar7 + -2;
    *(undefined8 *)piVar7 = uVar8;
  }
  piVar7 = (int *)((int)piVar7 + iVar1);
  if (in_FPSCR_SZ == '\0') {
    piVar5 = piVar3 + 1;
    iVar2 = *piVar4;
    piVar4 = piVar4 + 1;
    *piVar7 = *piVar3;
    piVar7 = piVar7 + -1;
    *piVar7 = iVar2;
  }
  else {
    piVar5 = piVar3 + 2;
    uVar8 = *(undefined8 *)piVar4;
    piVar4 = piVar4 + 2;
    *(undefined8 *)piVar7 = *(undefined8 *)piVar3;
    piVar7 = piVar7 + -2;
    *(undefined8 *)piVar7 = uVar8;
  }
  piVar7 = (int *)((int)piVar7 + iVar1);
  if (in_FPSCR_SZ == '\0') {
    piVar3 = piVar5 + 1;
    iVar2 = *piVar4;
    piVar4 = piVar4 + 1;
    *piVar7 = *piVar5;
    piVar7 = piVar7 + -1;
    *piVar7 = iVar2;
  }
  else {
    piVar3 = piVar5 + 2;
    uVar8 = *(undefined8 *)piVar4;
    piVar4 = piVar4 + 2;
    *(undefined8 *)piVar7 = *(undefined8 *)piVar5;
    piVar7 = piVar7 + -2;
    *(undefined8 *)piVar7 = uVar8;
  }
  piVar7 = (int *)((int)piVar7 + iVar1);
  if (in_FPSCR_SZ == '\0') {
    piVar5 = piVar3 + 1;
    iVar2 = *piVar4;
    piVar4 = piVar4 + 1;
    *piVar7 = *piVar3;
    piVar7 = piVar7 + -1;
    *piVar7 = iVar2;
  }
  else {
    piVar5 = piVar3 + 2;
    uVar8 = *(undefined8 *)piVar4;
    piVar4 = piVar4 + 2;
    *(undefined8 *)piVar7 = *(undefined8 *)piVar3;
    piVar7 = piVar7 + -2;
    *(undefined8 *)piVar7 = uVar8;
  }
  piVar7 = (int *)((int)piVar7 + iVar1);
  if (in_FPSCR_SZ == '\0') {
    piVar3 = piVar5 + 1;
    iVar2 = *piVar4;
    piVar4 = piVar4 + 1;
    *piVar7 = *piVar5;
    piVar7 = piVar7 + -1;
    *piVar7 = iVar2;
  }
  else {
    piVar3 = piVar5 + 2;
    uVar8 = *(undefined8 *)piVar4;
    piVar4 = piVar4 + 2;
    *(undefined8 *)piVar7 = *(undefined8 *)piVar5;
    piVar7 = piVar7 + -2;
    *(undefined8 *)piVar7 = uVar8;
  }
  piVar7 = (int *)((int)piVar7 + iVar1);
  if (in_FPSCR_SZ == '\0') {
    piVar5 = piVar3 + 1;
    iVar2 = *piVar4;
    piVar4 = piVar4 + 1;
    *piVar7 = *piVar3;
    piVar7 = piVar7 + -1;
    *piVar7 = iVar2;
  }
  else {
    piVar5 = piVar3 + 2;
    uVar8 = *(undefined8 *)piVar4;
    piVar4 = piVar4 + 2;
    *(undefined8 *)piVar7 = *(undefined8 *)piVar3;
    piVar7 = piVar7 + -2;
    *(undefined8 *)piVar7 = uVar8;
  }
  piVar7 = (int *)((int)piVar7 + iVar1);
  if (in_FPSCR_SZ == '\0') {
    piVar3 = piVar5 + 1;
    iVar2 = *piVar4;
    piVar4 = piVar4 + 1;
    *piVar7 = *piVar5;
    piVar7 = piVar7 + -1;
    *piVar7 = iVar2;
  }
  else {
    piVar3 = piVar5 + 2;
    uVar8 = *(undefined8 *)piVar4;
    piVar4 = piVar4 + 2;
    *(undefined8 *)piVar7 = *(undefined8 *)piVar5;
    piVar7 = piVar7 + -2;
    *(undefined8 *)piVar7 = uVar8;
  }
  piVar7 = (int *)((int)piVar7 + iVar1);
  if (in_FPSCR_SZ == '\0') {
    iVar1 = *piVar4;
    *piVar7 = *piVar3;
    piVar7[-1] = iVar1;
  }
  else {
    uVar8 = *(undefined8 *)piVar4;
    *(undefined8 *)piVar7 = *(undefined8 *)piVar3;
    *(undefined8 *)(piVar7 + -2) = uVar8;
  }
  return;
}


/* Address: 8ceb2028 */

void callback_candidate_8ceb2028(undefined8 *param_1,int *param_2,uint param_3)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  byte bVar4;
  uint uVar5;
  int iVar6;
  byte *pbVar7;
  byte *pbVar8;
  byte *pbVar9;
  byte *pbVar10;
  short *psVar11;
  short *psVar12;
  int iVar13;
  int iVar14;
  undefined8 uVar15;
  undefined8 uVar17;
  undefined8 uVar19;
  char in_FPSCR_SZ;
  undefined4 uVar16;
  undefined4 uVar18;
  undefined4 uVar20;
  
  iVar6 = *param_2;
  psVar11 = (short *)param_2[1];
  pbVar10 = (byte *)param_2[3];
  pbVar7 = (byte *)param_2[2];
  iVar13 = 6;
  pbVar8 = pbVar7;
  do {
    uVar5 = param_3 & 0x80000000;
    param_3 = param_3 << 1;
    iVar14 = 8;
    if (uVar5 == 0) {
      psVar11 = psVar11 + 0x40;
      do {
        bVar1 = pbVar10[1];
        bVar2 = pbVar8[1];
        *pbVar7 = (byte)((int)((uint)*pbVar8 + (uint)*pbVar10 + 1) >> 1);
        bVar3 = pbVar10[2];
        bVar4 = pbVar8[2];
        pbVar7[1] = (byte)((int)((uint)bVar2 + (uint)bVar1 + 1) >> 1);
        bVar1 = pbVar10[3];
        bVar2 = pbVar8[3];
        pbVar7[2] = (byte)((int)((uint)bVar4 + (uint)bVar3 + 1) >> 1);
        bVar3 = pbVar10[4];
        bVar4 = pbVar8[4];
        pbVar7[3] = (byte)((int)((uint)bVar2 + (uint)bVar1 + 1) >> 1);
        bVar1 = pbVar10[5];
        bVar2 = pbVar8[5];
        pbVar7[4] = (byte)((int)((uint)bVar4 + (uint)bVar3 + 1) >> 1);
        bVar3 = pbVar10[6];
        bVar4 = pbVar8[6];
        pbVar7[5] = (byte)((int)((uint)bVar2 + (uint)bVar1 + 1) >> 1);
        bVar1 = pbVar10[7];
        pbVar10 = pbVar10 + 8;
        bVar2 = pbVar8[7];
        pbVar8 = pbVar8 + 8;
        pbVar7[6] = (byte)((int)((uint)bVar4 + (uint)bVar3 + 1) >> 1);
        iVar14 = iVar14 + -1;
        pbVar7[7] = (byte)((int)((uint)bVar2 + (uint)bVar1 + 1) >> 1);
        pbVar7 = pbVar7 + 8;
      } while (iVar14 != 0);
    }
    else {
      do {
        bVar1 = pbVar10[1];
        bVar2 = pbVar8[1];
        *pbVar7 = *(byte *)(iVar6 + ((int)((uint)*pbVar8 + (uint)*pbVar10 + 1) >> 1) + (int)*psVar11
                           );
        bVar3 = pbVar10[2];
        bVar4 = pbVar8[2];
        pbVar7[1] = *(byte *)(iVar6 + ((int)((uint)bVar2 + (uint)bVar1 + 1) >> 1) + (int)psVar11[1])
        ;
        bVar1 = pbVar10[3];
        bVar2 = pbVar8[3];
        pbVar7[2] = *(byte *)(iVar6 + ((int)((uint)bVar4 + (uint)bVar3 + 1) >> 1) + (int)psVar11[2])
        ;
        bVar3 = pbVar10[4];
        bVar4 = pbVar8[4];
        pbVar7[3] = *(byte *)(iVar6 + ((int)((uint)bVar2 + (uint)bVar1 + 1) >> 1) + (int)psVar11[3])
        ;
        bVar1 = pbVar10[5];
        bVar2 = pbVar8[5];
        pbVar7[4] = *(byte *)(iVar6 + ((int)((uint)bVar4 + (uint)bVar3 + 1) >> 1) + (int)psVar11[4])
        ;
        bVar3 = pbVar10[6];
        bVar4 = pbVar8[6];
        pbVar7[5] = *(byte *)(iVar6 + ((int)((uint)bVar2 + (uint)bVar1 + 1) >> 1) + (int)psVar11[5])
        ;
        psVar12 = psVar11 + 7;
        bVar1 = pbVar10[7];
        pbVar10 = pbVar10 + 8;
        bVar2 = pbVar8[7];
        pbVar8 = pbVar8 + 8;
        pbVar7[6] = *(byte *)(iVar6 + ((int)((uint)bVar4 + (uint)bVar3 + 1) >> 1) + (int)psVar11[6])
        ;
        psVar11 = psVar11 + 8;
        iVar14 = iVar14 + -1;
        pbVar7[7] = *(byte *)(iVar6 + ((int)((uint)bVar2 + (uint)bVar1 + 1) >> 1) + (int)*psVar12);
        pbVar7 = pbVar7 + 8;
      } while (iVar14 != 0);
    }
    if (in_FPSCR_SZ == '\0') {
      uVar16 = *(undefined4 *)(pbVar8 + -0x3c);
      uVar18 = *(undefined4 *)(pbVar8 + -0x38);
      uVar20 = *(undefined4 *)(pbVar8 + -0x34);
      pbVar9 = pbVar8 + -0x30;
      *(undefined4 *)param_1 = *(undefined4 *)(pbVar8 + -0x40);
      *(undefined4 *)(param_1 + 1) = uVar16;
      *(undefined4 *)(param_1 + 2) = uVar18;
      *(undefined4 *)(param_1 + 3) = uVar20;
      uVar16 = *(undefined4 *)(pbVar8 + -0x2c);
      uVar18 = *(undefined4 *)(pbVar8 + -0x28);
      uVar20 = *(undefined4 *)(pbVar8 + -0x24);
      pbVar8 = pbVar8 + -0x20;
      *(undefined4 *)(param_1 + 4) = *(undefined4 *)pbVar9;
      *(undefined4 *)(param_1 + 5) = uVar16;
      *(undefined4 *)(param_1 + 6) = uVar18;
      *(undefined4 *)(param_1 + 7) = uVar20;
    }
    else {
      uVar15 = *(undefined8 *)(pbVar8 + -0x38);
      uVar17 = *(undefined8 *)(pbVar8 + -0x30);
      uVar19 = *(undefined8 *)(pbVar8 + -0x28);
      *param_1 = *(undefined8 *)(pbVar8 + -0x40);
      param_1[1] = uVar15;
      param_1[2] = uVar17;
      param_1[3] = uVar19;
      uVar15 = *(undefined8 *)(pbVar8 + -0x18);
      uVar17 = *(undefined8 *)(pbVar8 + -0x10);
      uVar19 = *(undefined8 *)(pbVar8 + -8);
      param_1[4] = *(undefined8 *)(pbVar8 + -0x20);
      param_1[5] = uVar15;
      param_1[6] = uVar17;
      param_1[7] = uVar19;
    }
    iVar13 = iVar13 + -1;
    param_1 = param_1 + 8;
  } while (iVar13 != 0);
  return;
}


/* Address: 8ceb21fc */

undefined8 * FUN_8ceb21fc(undefined4 param_1,undefined4 param_2,int param_3,undefined8 *param_4)

{
  undefined8 *puVar1;
  undefined8 *puVar2;
  int iVar3;
  undefined4 in_fr1;
  undefined8 uVar4;
  undefined4 uVar5;
  undefined4 in_fr3;
  undefined8 uVar6;
  undefined4 uVar7;
  undefined8 uVar8;
  undefined4 uVar9;
  undefined8 uVar10;
  undefined4 uVar11;
  char in_FPSCR_SZ;
  
  puVar2 = *(undefined8 **)(param_3 + 0x24);
  iVar3 = *(int *)(param_3 + 0x1c);
  puVar1 = *(undefined8 **)(param_3 + 0x14);
  if (in_FPSCR_SZ == '\0') {
    uVar4 = CONCAT44(*(undefined4 *)puVar2,in_fr1);
  }
  else {
    uVar4 = *puVar2;
  }
  puVar2 = (undefined8 *)((int)puVar2 + iVar3);
  if (in_FPSCR_SZ == '\0') {
    uVar6 = CONCAT44(*(undefined4 *)puVar2,in_fr3);
  }
  else {
    uVar6 = *puVar2;
  }
  puVar2 = (undefined8 *)((int)puVar2 + iVar3);
  if (in_FPSCR_SZ == '\0') {
    uVar8 = CONCAT44(*(undefined4 *)puVar2,param_1);
  }
  else {
    uVar8 = *puVar2;
  }
  puVar2 = (undefined8 *)((int)puVar2 + iVar3);
  if (in_FPSCR_SZ == '\0') {
    uVar10 = CONCAT44(*(undefined4 *)puVar2,param_2);
  }
  else {
    uVar10 = *puVar2;
  }
  puVar2 = (undefined8 *)((int)puVar2 + iVar3);
  uVar5 = (undefined4)((ulonglong)uVar4 >> 0x20);
  uVar7 = (undefined4)((ulonglong)uVar6 >> 0x20);
  uVar9 = (undefined4)((ulonglong)uVar8 >> 0x20);
  uVar11 = (undefined4)((ulonglong)uVar10 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)puVar1 = uVar5;
    *(undefined4 *)(puVar1 + 1) = uVar7;
    *(undefined4 *)(puVar1 + 2) = uVar9;
    *(undefined4 *)(puVar1 + 3) = uVar11;
    *(undefined4 *)(param_4 + 3) = uVar11;
    *(undefined4 *)((int)param_4 + 0x14) = uVar9;
    *(undefined4 *)(param_4 + 2) = uVar7;
    param_4 = (undefined8 *)((int)param_4 + 0xc);
    *(undefined4 *)param_4 = uVar5;
    uVar4 = CONCAT44(*(undefined4 *)puVar2,(int)uVar4);
  }
  else {
    *puVar1 = uVar4;
    puVar1[1] = uVar6;
    puVar1[2] = uVar8;
    puVar1[3] = uVar10;
    param_4[3] = uVar10;
    param_4[2] = uVar8;
    param_4[1] = uVar6;
    *param_4 = uVar4;
    uVar4 = *puVar2;
  }
  puVar2 = (undefined8 *)((int)puVar2 + iVar3);
  if (in_FPSCR_SZ == '\0') {
    uVar6 = CONCAT44(*(undefined4 *)puVar2,(int)uVar6);
  }
  else {
    uVar6 = *puVar2;
  }
  puVar2 = (undefined8 *)((int)puVar2 + iVar3);
  if (in_FPSCR_SZ == '\0') {
    uVar8 = CONCAT44(*(undefined4 *)puVar2,(int)uVar8);
  }
  else {
    uVar8 = *puVar2;
  }
  uVar5 = (undefined4)((ulonglong)uVar4 >> 0x20);
  uVar7 = (undefined4)((ulonglong)uVar6 >> 0x20);
  uVar9 = (undefined4)((ulonglong)uVar8 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    uVar11 = *(undefined4 *)((int)puVar2 + iVar3);
    *(undefined4 *)(puVar1 + 4) = uVar5;
    *(undefined4 *)(puVar1 + 5) = uVar7;
    *(undefined4 *)(puVar1 + 6) = uVar9;
    *(undefined4 *)(puVar1 + 7) = uVar11;
    *(undefined4 *)(param_4 + 7) = uVar11;
    *(undefined4 *)((int)param_4 + 0x34) = uVar9;
    *(undefined4 *)(param_4 + 6) = uVar7;
    *(undefined4 *)((int)param_4 + 0x2c) = uVar5;
  }
  else {
    uVar10 = *(undefined8 *)((int)puVar2 + iVar3);
    puVar1[4] = uVar4;
    puVar1[5] = uVar6;
    puVar1[6] = uVar8;
    puVar1[7] = uVar10;
    param_4[7] = uVar10;
    param_4[6] = uVar8;
    param_4[5] = uVar6;
    param_4[4] = uVar4;
  }
  return puVar1 + 8;
}


/* Address: 8ceb2274 */

void callback_candidate_8ceb2274(int param_1,undefined1 *param_2,int param_3)

{
  undefined1 uVar1;
  undefined1 uVar2;
  undefined1 uVar3;
  undefined2 uVar4;
  undefined2 uVar5;
  undefined1 uVar6;
  undefined4 *puVar7;
  undefined1 *puVar8;
  int iVar9;
  
  puVar7 = (undefined4 *)(param_1 + -8);
  iVar9 = 8;
  uVar6 = *param_2;
  do {
    uVar5 = *(undefined2 *)(param_2 + 2);
    uVar1 = param_2[4];
    uVar4 = CONCAT11(param_2[1],uVar6);
    uVar2 = param_2[5];
    uVar3 = param_2[6];
    puVar8 = param_2 + 7;
    param_2 = puVar8 + param_3 + -7;
    uVar6 = *param_2;
    *(undefined1 *)((int)puVar7 + 0xf) = *puVar8;
    *(undefined1 *)((int)puVar7 + 0xe) = uVar3;
    *(undefined1 *)((int)puVar7 + 0xd) = uVar2;
    iVar9 = iVar9 + -1;
    *(undefined1 *)(puVar7 + 3) = uVar1;
    puVar7 = puVar7 + 2;
    *puVar7 = CONCAT22(uVar5,uVar4);
  } while (iVar9 != 0);
  return;
}


/* Address: 8ceb2340 */

void callback_candidate_8ceb2340(int param_1)

{
  byte bVar1;
  byte bVar2;
  int iVar3;
  byte bVar4;
  uint uVar5;
  byte bVar6;
  byte *pbVar7;
  byte *pbVar8;
  undefined1 *puVar9;
  int iVar10;
  
  pbVar7 = *(byte **)(param_1 + 0x24);
  iVar3 = 8;
  pbVar8 = *(byte **)(param_1 + 0x28);
  puVar9 = *(undefined1 **)(param_1 + 0x18);
  iVar10 = *(int *)(param_1 + 0x20) + -7;
  bVar4 = *pbVar7;
  bVar6 = *pbVar8;
  do {
    bVar1 = pbVar7[1];
    bVar2 = pbVar8[1];
    uVar5 = (uint)bVar4 + (uint)bVar6 + 1;
    *puVar9 = (char)((int)uVar5 >> 1);
    bVar4 = pbVar7[2];
    bVar6 = pbVar8[2];
    uVar5 = (uint)bVar2 + (uint)bVar1 + (uint)((uVar5 & 1) != 0);
    puVar9[1] = (char)((int)uVar5 >> 1);
    bVar1 = pbVar7[3];
    bVar2 = pbVar8[3];
    uVar5 = (uint)bVar6 + (uint)bVar4 + (uint)((uVar5 & 1) != 0);
    puVar9[2] = (char)((int)uVar5 >> 1);
    bVar4 = pbVar7[4];
    bVar6 = pbVar8[4];
    uVar5 = (uint)bVar2 + (uint)bVar1 + (uint)((uVar5 & 1) != 0);
    puVar9[3] = (char)((int)uVar5 >> 1);
    bVar1 = pbVar7[5];
    bVar2 = pbVar8[5];
    uVar5 = (uint)bVar6 + (uint)bVar4 + (uint)((uVar5 & 1) != 0);
    puVar9[4] = (char)((int)uVar5 >> 1);
    bVar4 = pbVar7[6];
    bVar6 = pbVar8[6];
    uVar5 = (uint)bVar2 + (uint)bVar1 + (uint)((uVar5 & 1) != 0);
    puVar9[5] = (char)((int)uVar5 >> 1);
    bVar1 = pbVar7[7];
    bVar2 = pbVar8[7];
    uVar5 = (uint)bVar6 + (uint)bVar4 + (uint)((uVar5 & 1) != 0);
    puVar9[6] = (char)((int)uVar5 >> 1);
    pbVar7 = pbVar7 + 7 + iVar10;
    pbVar8 = pbVar8 + 7 + iVar10;
    bVar4 = *pbVar7;
    bVar6 = *pbVar8;
    iVar3 = iVar3 + -1;
    puVar9[7] = (char)((int)((uint)bVar2 + (uint)bVar1 + (uint)((uVar5 & 1) != 0)) >> 1);
    puVar9 = puVar9 + 8;
  } while (iVar3 != 0);
  return;
}


/* Address: 8ceb23e4 */

void callback_candidate_8ceb23e4(int param_1)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  byte bVar4;
  byte bVar5;
  undefined1 *puVar6;
  int iVar7;
  uint uVar8;
  byte *pbVar9;
  int iVar10;
  
  iVar10 = *(int *)(param_1 + 0x20);
  iVar7 = 8;
  pbVar9 = *(byte **)(param_1 + 0x24);
  puVar6 = (undefined1 *)(*(int *)(param_1 + 0x18) + -8);
  do {
    bVar1 = pbVar9[1];
    bVar2 = pbVar9[2];
    bVar3 = pbVar9[3];
    uVar8 = (uint)*pbVar9 + (uint)bVar1 + 1;
    puVar6[8] = (char)((int)uVar8 >> 1);
    bVar4 = pbVar9[4];
    uVar8 = (uint)bVar1 + (uint)bVar2 + (uint)((uVar8 & 1) != 0);
    puVar6[9] = (char)((int)uVar8 >> 1);
    bVar5 = pbVar9[5];
    uVar8 = (uint)bVar2 + (uint)bVar3 + (uint)((uVar8 & 1) != 0);
    puVar6[10] = (char)((int)uVar8 >> 1);
    bVar2 = pbVar9[6];
    uVar8 = (uint)bVar3 + (uint)bVar4 + (uint)((uVar8 & 1) != 0);
    puVar6[0xb] = (char)((int)uVar8 >> 1);
    bVar3 = pbVar9[7];
    uVar8 = (uint)bVar4 + (uint)bVar5 + (uint)((uVar8 & 1) != 0);
    puVar6[0xc] = (char)((int)uVar8 >> 1);
    bVar1 = pbVar9[8];
    uVar8 = (uint)bVar5 + (uint)bVar2 + (uint)((uVar8 & 1) != 0);
    puVar6[0xd] = (char)((int)uVar8 >> 1);
    uVar8 = (uint)bVar2 + (uint)bVar3 + (uint)((uVar8 & 1) != 0);
    puVar6[0xe] = (char)((int)uVar8 >> 1);
    puVar6[0xf] = (char)((int)((uint)bVar3 + (uint)bVar1 + (uint)((uVar8 & 1) != 0)) >> 1);
    iVar7 = iVar7 + -1;
    pbVar9 = pbVar9 + 8 + iVar10 + -8;
    puVar6 = puVar6 + 8;
  } while (iVar7 != 0);
  return;
}


/* Address: 8ceb2488 */

void callback_candidate_8ceb2488(int param_1)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  byte bVar4;
  byte bVar5;
  byte bVar6;
  int iVar7;
  byte bVar8;
  byte bVar9;
  byte *pbVar10;
  byte *pbVar11;
  undefined1 *puVar12;
  int iVar13;
  
  iVar7 = 8;
  iVar13 = *(int *)(param_1 + 0x20) + -8;
  pbVar10 = *(byte **)(param_1 + 0x24);
  pbVar11 = *(byte **)(param_1 + 0x28);
  puVar12 = *(undefined1 **)(param_1 + 0x18);
  bVar8 = *pbVar10;
  bVar9 = *pbVar11;
  do {
    bVar1 = pbVar10[1];
    bVar2 = pbVar11[1];
    bVar3 = pbVar10[2];
    bVar4 = pbVar11[2];
    bVar5 = pbVar11[3];
    bVar6 = pbVar10[3];
    *puVar12 = (char)((uint)bVar8 + (uint)bVar9 + (uint)bVar1 + (uint)bVar2 + 2 >> 2);
    bVar8 = pbVar11[4];
    bVar9 = pbVar10[4];
    puVar12[1] = (char)((uint)bVar1 + (uint)bVar2 + (uint)bVar3 + (uint)bVar4 + 2 >> 2);
    bVar1 = pbVar11[5];
    bVar2 = pbVar10[5];
    puVar12[2] = (char)((uint)bVar3 + (uint)bVar4 + (uint)bVar6 + (uint)bVar5 + 2 >> 2);
    bVar3 = pbVar11[6];
    bVar4 = pbVar10[6];
    puVar12[3] = (char)((uint)bVar6 + (uint)bVar5 + (uint)bVar9 + (uint)bVar8 + 2 >> 2);
    bVar5 = pbVar11[7];
    bVar6 = pbVar10[7];
    puVar12[4] = (char)((uint)bVar9 + (uint)bVar8 + (uint)bVar2 + (uint)bVar1 + 2 >> 2);
    bVar8 = pbVar11[8];
    bVar9 = pbVar10[8];
    puVar12[5] = (char)((uint)bVar2 + (uint)bVar1 + (uint)bVar4 + (uint)bVar3 + 2 >> 2);
    puVar12[6] = (char)((uint)bVar4 + (uint)bVar3 + (uint)bVar6 + (uint)bVar5 + 2 >> 2);
    puVar12[7] = (char)((uint)bVar6 + (uint)bVar5 + (uint)bVar9 + (uint)bVar8 + 2 >> 2);
    pbVar10 = pbVar10 + 8 + iVar13;
    bVar8 = *pbVar10;
    pbVar11 = pbVar11 + 8 + iVar13;
    bVar9 = *pbVar11;
    iVar7 = iVar7 + -1;
    puVar12 = puVar12 + 8;
  } while (iVar7 != 0);
  return;
}


/* Address: 8ceb499c */

/* WARNING: Removing unreachable block (ram,0x8ceb4acc) */
/* WARNING: Removing unreachable block (ram,0x8ceb4a4c) */
/* WARNING: Removing unreachable block (ram,0x8ceb4b06) */
/* WARNING: Removing unreachable block (ram,0x8ceb4ada) */

undefined4 callback_candidate_8ceb499c(uint *param_1,int *param_2,int *param_3,int *param_4)

{
  undefined1 uVar1;
  undefined *puVar2;
  int iVar3;
  uint uVar4;
  int *piVar5;
  uint uVar6;
  int iVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  undefined1 *puVar11;
  undefined1 *puVar12;
  undefined1 *puVar13;
  undefined1 *puVar14;
  
  uVar10 = param_1[1];
  uVar9 = param_1[2];
  puVar11 = (undefined1 *)param_1[3];
  uVar8 = *param_1;
  while( true ) {
    uVar6 = uVar8 >> 9;
    if (9 < (int)uVar9) {
      uVar4 = -(0x29 - uVar9);
      if ((int)uVar4 < 0) {
        uVar4 = uVar10 >> (~uVar4 & 0x1f) + 1;
      }
      else {
        uVar4 = uVar10 << (uVar4 & 0x1f);
      }
      uVar6 = uVar6 | uVar4;
    }
    if (uVar6 == 0) break;
    iVar7 = *(int *)((int)DAT_8ceb4a90 + (int)param_1);
    while( true ) {
      do {
        uVar6 = uVar8 >> 0x14;
        if (0x14 < (int)uVar9) {
          uVar4 = -(0x34 - uVar9);
          if ((int)uVar4 < 0) {
            uVar4 = uVar10 >> (~uVar4 & 0x1f) + 1;
          }
          else {
            uVar4 = uVar10 << (uVar4 & 0x1f);
          }
          uVar6 = uVar6 | uVar4;
        }
        piVar5 = DAT_8ceb4a94;
        if (uVar6 >> 8 != 0) {
          uVar6 = uVar6 >> 6;
          piVar5 = DAT_8ceb4a98;
        }
        uVar6 = (uint)*(short *)(*piVar5 + uVar6 * 2);
        uVar9 = uVar9 + (uVar6 & 0xf);
        if ((int)uVar9 < 0x20) {
          uVar8 = uVar8 << (uVar6 & 0xf);
        }
        else {
          uVar1 = *puVar11;
          puVar12 = puVar11 + 1;
          uVar9 = uVar9 - 0x20;
          puVar13 = puVar11 + 2;
          if ((int)uVar9 < 0) {
            uVar8 = uVar10 >> (~uVar9 & 0x1f) + 1;
          }
          else {
            uVar8 = uVar10 << (uVar9 & 0x1f);
          }
          puVar14 = puVar11 + 3;
          puVar11 = puVar11 + 4;
          uVar10 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar12),*puVar13),*puVar14);
        }
        uVar4 = (uVar6 >> 2 & 0xff) >> 2;
      } while (uVar4 == 0x22);
      if (uVar4 != 0x23) break;
      *(int *)((int)DAT_8ceb4a90 + (int)param_1) = *(int *)((int)DAT_8ceb4a90 + (int)param_1) + 0x21
      ;
    }
    if (uVar4 == 0x24) {
LAB_8ceb4a8c:
      iVar7 = -2;
    }
    else {
      iVar3 = (int)DAT_8ceb4a90;
      *(uint *)(iVar3 + (int)param_1) = *(int *)(iVar3 + (int)param_1) + uVar4;
      *(uint *)(iVar3 + 8 + (int)param_1) = uVar6 >> 10;
      if (*(int *)(iVar3 + 4 + (int)param_1) < *(int *)(iVar3 + (int)param_1)) goto LAB_8ceb4a8c;
      iVar7 = *(int *)((int)DAT_8ceb4b76 + (int)param_1) - iVar7;
    }
    if (iVar7 == -2) break;
    uVar6 = uVar8;
    if ((*(uint *)((int)DAT_8ceb4b78 + (int)param_1) & 0x10) != 0) {
      if ((int)uVar9 < 0x1b) {
        uVar6 = uVar8 << 5;
        *(uint *)((int)DAT_8ceb4b7a + (int)param_1) = uVar8 >> 0x1b;
        uVar9 = uVar9 + 5;
      }
      else {
        uVar9 = uVar9 - 0x1b;
        if (uVar9 == 0) {
          *(uint *)((int)DAT_8ceb4b7a + (int)param_1) = uVar8 >> 0x1b;
        }
        else {
          uVar6 = -(5 - uVar9);
          if ((int)uVar6 < 0) {
            uVar6 = uVar10 >> (~uVar6 & 0x1f) + 1;
          }
          else {
            uVar6 = uVar10 << (uVar6 & 0x1f);
          }
          *(uint *)((int)DAT_8ceb4b7a + (int)param_1) = (uVar8 | uVar6) >> 0x1b;
          if ((int)uVar9 < 0) {
            uVar10 = uVar10 >> (~uVar9 & 0x1f) + 1;
          }
          else {
            uVar10 = uVar10 << (uVar9 & 0x1f);
          }
        }
        uVar1 = *puVar11;
        puVar12 = puVar11 + 1;
        puVar13 = puVar11 + 2;
        puVar14 = puVar11 + 3;
        puVar11 = puVar11 + 4;
        uVar6 = uVar10;
        uVar10 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar12),*puVar13),*puVar14);
      }
    }
    iVar7 = (int)DAT_8ceb4b7c;
    *param_1 = uVar6;
    param_1[1] = uVar10;
    param_1[2] = uVar9;
    param_1[3] = (uint)puVar11;
    (**(code **)(iVar7 + (int)param_1))(param_1);
    (**(code **)((int)DAT_8ceb4b7e + (int)param_1))(param_1);
    uVar9 = param_1[2];
    uVar10 = param_1[1];
    uVar8 = *param_1;
    puVar11 = (undefined1 *)param_1[3];
  }
  iVar7 = (int)(puVar11 + ((int)(uVar9 + 7) >> 3) + -8) - *param_2;
  *param_4 = *param_4 + iVar7;
  *param_3 = *param_3 - iVar7;
  puVar2 = PTR_callback_candidate_8ced92e2_8ceb4b80;
  *param_2 = (int)(puVar11 + ((int)(uVar9 + 7) >> 3) + -8);
  (*(code *)puVar2)(param_2,param_3,param_4);
  return 0;
}


/* Address: 8ceb67f8 */

/* WARNING: Removing unreachable block (ram,0x8ceb685c) */
/* WARNING: Removing unreachable block (ram,0x8ceb69fe) */
/* WARNING: Removing unreachable block (ram,0x8ceb68a8) */
/* WARNING: Removing unreachable block (ram,0x8ceb6848) */
/* WARNING: Removing unreachable block (ram,0x8ceb693a) */
/* WARNING: Removing unreachable block (ram,0x8ceb6ab6) */
/* WARNING: Removing unreachable block (ram,0x8ceb6986) */
/* WARNING: Removing unreachable block (ram,0x8ceb69ba) */
/* WARNING: Removing unreachable block (ram,0x8ceb69ac) */
/* WARNING: Removing unreachable block (ram,0x8ceb6a68) */

undefined4 callback_candidate_8ceb67f8(uint *param_1,int *param_2,int *param_3,int *param_4)

{
  undefined1 uVar1;
  bool bVar2;
  undefined *puVar3;
  int iVar4;
  uint uVar5;
  int *piVar6;
  int iVar7;
  int iVar8;
  uint uVar9;
  uint uVar10;
  uint uVar11;
  undefined1 *puVar12;
  undefined1 *puVar13;
  undefined1 *puVar14;
  undefined1 *puVar15;
  uint uVar16;
  
  bVar2 = true;
  uVar11 = param_1[1];
  uVar9 = *param_1;
  puVar12 = (undefined1 *)param_1[3];
  uVar10 = param_1[2];
  while( true ) {
    uVar16 = uVar9 >> 9;
    if (9 < (int)uVar10) {
      uVar5 = -(0x29 - uVar10);
      if ((int)uVar5 < 0) {
        uVar5 = uVar11 >> (~uVar5 & 0x1f) + 1;
      }
      else {
        uVar5 = uVar11 << (uVar5 & 0x1f);
      }
      uVar16 = uVar16 | uVar5;
    }
    if (uVar16 == 0) break;
    iVar7 = *(int *)((int)DAT_8ceb68ec + (int)param_1);
    while( true ) {
      do {
        uVar16 = uVar9 >> 0x15;
        if (0x15 < (int)uVar10) {
          uVar5 = -(0x35 - uVar10);
          if ((int)uVar5 < 0) {
            uVar5 = uVar11 >> (~uVar5 & 0x1f) + 1;
          }
          else {
            uVar5 = uVar11 << (uVar5 & 0x1f);
          }
          uVar16 = uVar16 | uVar5;
        }
        piVar6 = DAT_8ceb68f0;
        if (uVar16 >> 7 != 0) {
          uVar16 = uVar16 >> 6;
          piVar6 = DAT_8ceb68f4;
        }
        uVar16 = (uint)*(short *)(*piVar6 + uVar16 * 2);
        uVar10 = uVar10 + (uVar16 & 0xf);
        if ((int)uVar10 < 0x20) {
          uVar9 = uVar9 << (uVar16 & 0xf);
        }
        else {
          uVar1 = *puVar12;
          puVar13 = puVar12 + 1;
          uVar10 = uVar10 - 0x20;
          puVar14 = puVar12 + 2;
          if ((int)uVar10 < 0) {
            uVar9 = uVar11 >> (~uVar10 & 0x1f) + 1;
          }
          else {
            uVar9 = uVar11 << (uVar10 & 0x1f);
          }
          puVar15 = puVar12 + 3;
          puVar12 = puVar12 + 4;
          uVar11 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar13),*puVar14),*puVar15);
        }
        uVar5 = (uVar16 >> 2 & 0xff) >> 2;
      } while (uVar5 == 0x22);
      if (uVar5 != 0x23) break;
      *(int *)((int)DAT_8ceb68ec + (int)param_1) = *(int *)((int)DAT_8ceb68ec + (int)param_1) + 0x21
      ;
    }
    if (uVar5 == 0x24) {
LAB_8ceb68e8:
      uVar16 = 0xfffffffe;
    }
    else {
      iVar4 = (int)DAT_8ceb68ec;
      *(uint *)(iVar4 + (int)param_1) = *(int *)(iVar4 + (int)param_1) + uVar5;
      *(uint *)(iVar4 + 8 + (int)param_1) = uVar16 >> 10;
      if (*(int *)(iVar4 + 4 + (int)param_1) < *(int *)(iVar4 + (int)param_1)) goto LAB_8ceb68e8;
      uVar16 = *(int *)((int)DAT_8ceb69dc + (int)param_1) - iVar7;
    }
    if (uVar16 == 0xfffffffe) break;
    if ((!bVar2) && (1 < uVar16)) {
      (**(code **)((int)DAT_8ceb69de + (int)param_1))(param_1,uVar16);
      (*(code *)PTR_FUN_8ceb69e8)((int)DAT_8ceb69e0 + (int)param_1);
      (*(code *)PTR_FUN_8ceb69ec)(param_1);
    }
    if ((*(uint *)((int)DAT_8ceb69e2 + (int)param_1) & 0x20) == 0) {
      uVar16 = uVar9 >> 0x1b;
      if (0x1b < (int)uVar10) {
        uVar5 = -(0x3b - uVar10);
        if ((int)uVar5 < 0) {
          uVar5 = uVar11 >> (~uVar5 & 0x1f) + 1;
        }
        else {
          uVar5 = uVar11 << (uVar5 & 0x1f);
        }
        uVar16 = uVar16 | uVar5;
      }
      uVar16 = (uint)*(short *)(*DAT_8ceb69f0 + uVar16 * 2);
      uVar10 = uVar10 + (uVar16 & 0xff);
      *(uint *)((int)DAT_8ceb69e2 + (int)param_1) = uVar16 >> 8;
      if ((int)uVar10 < 0x20) {
        uVar9 = uVar9 << (uVar16 & 0x1f);
      }
      else {
        uVar1 = *puVar12;
        puVar13 = puVar12 + 1;
        uVar10 = uVar10 - 0x20;
        puVar14 = puVar12 + 2;
        if ((int)uVar10 < 0) {
          uVar9 = uVar11 >> (~uVar10 & 0x1f) + 1;
        }
        else {
          uVar9 = uVar11 << (uVar10 & 0x1f);
        }
        puVar15 = puVar12 + 3;
        puVar12 = puVar12 + 4;
        uVar11 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar13),*puVar14),*puVar15);
      }
    }
    uVar16 = uVar9;
    if ((*(uint *)((int)DAT_8ceb69e2 + (int)param_1) & 0x10) != 0) {
      if ((int)uVar10 < 0x1b) {
        uVar16 = uVar9 << 5;
        *(uint *)((int)DAT_8ceb6b0e + (int)param_1) = uVar9 >> 0x1b;
        uVar10 = uVar10 + 5;
      }
      else {
        uVar10 = uVar10 - 0x1b;
        if (uVar10 == 0) {
          *(uint *)((int)DAT_8ceb69e4 + (int)param_1) = uVar9 >> 0x1b;
        }
        else {
          uVar16 = -(5 - uVar10);
          if ((int)uVar16 < 0) {
            uVar16 = uVar11 >> (~uVar16 & 0x1f) + 1;
          }
          else {
            uVar16 = uVar11 << (uVar16 & 0x1f);
          }
          *(uint *)((int)DAT_8ceb69e4 + (int)param_1) = (uVar9 | uVar16) >> 0x1b;
          if ((int)uVar10 < 0) {
            uVar11 = uVar11 >> (~uVar10 & 0x1f) + 1;
          }
          else {
            uVar11 = uVar11 << (uVar10 & 0x1f);
          }
        }
        uVar1 = *puVar12;
        puVar13 = puVar12 + 1;
        puVar14 = puVar12 + 2;
        puVar15 = puVar12 + 3;
        puVar12 = puVar12 + 4;
        uVar16 = uVar11;
        uVar11 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar13),*puVar14),*puVar15);
      }
    }
    puVar3 = PTR_callback_candidate_8ceb865c_8ceb6b28;
    if ((*(uint *)((int)DAT_8ceb6b10 + (int)param_1) & 8) == 0) {
      (*(code *)PTR_FUN_8ceb6b2c)((int)DAT_8ceb6b16 + (int)param_1);
    }
    else {
      iVar8 = (int)DAT_8ceb6b12;
      iVar4 = (int)DAT_8ceb6b14;
      iVar7 = (int)DAT_8ceb6b16;
      *param_1 = uVar16;
      param_1[1] = uVar11;
      param_1[2] = uVar10;
      param_1[3] = (uint)puVar12;
      iVar7 = (*(code *)puVar3)(param_1,iVar7 + (int)param_1,iVar4 + (int)param_1,
                                iVar8 + (int)param_1);
      iVar4 = (*(code *)PTR_callback_candidate_8ceb865c_8ceb6b28)
                        (param_1,(int)DAT_8ceb6b16 + (int)param_1,(int)DAT_8ceb6b1a + (int)param_1,
                         (int)DAT_8ceb6b18 + (int)param_1);
      uVar10 = param_1[2];
      uVar16 = *param_1;
      uVar11 = param_1[1];
      puVar12 = (undefined1 *)param_1[3];
      if (iVar7 != 0 || iVar4 != 0) break;
    }
    if ((*(uint *)((int)DAT_8ceb6b10 + (int)param_1) & 2) == 0) {
      *(undefined4 *)((int)DAT_8ceb6b1c + (int)param_1) = 0;
    }
    else {
      uVar9 = uVar16 >> 0x17;
      if (0x17 < (int)uVar10) {
        uVar5 = -(0x37 - uVar10);
        if ((int)uVar5 < 0) {
          uVar5 = uVar11 >> (~uVar5 & 0x1f) + 1;
        }
        else {
          uVar5 = uVar11 << (uVar5 & 0x1f);
        }
        uVar9 = uVar9 | uVar5;
      }
      uVar9 = (uint)*(short *)(*DAT_8ceb6b30 + uVar9 * 2);
      uVar10 = uVar10 + (uVar9 & 0xff);
      *(uint *)((int)DAT_8ceb6b1c + (int)param_1) = (uVar9 & 0xfffffff0) << 0x10;
      if ((int)uVar10 < 0x20) {
        uVar16 = uVar16 << (uVar9 & 0x1f);
      }
      else {
        uVar1 = *puVar12;
        puVar13 = puVar12 + 1;
        uVar10 = uVar10 - 0x20;
        puVar14 = puVar12 + 2;
        if ((int)uVar10 < 0) {
          uVar16 = uVar11 >> (~uVar10 & 0x1f) + 1;
        }
        else {
          uVar16 = uVar11 << (uVar10 & 0x1f);
        }
        puVar15 = puVar12 + 3;
        puVar12 = puVar12 + 4;
        uVar11 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar13),*puVar14),*puVar15);
      }
    }
    iVar7 = (int)DAT_8ceb6b10;
    *param_1 = uVar16;
    param_1[1] = uVar11;
    param_1[2] = uVar10;
    param_1[3] = (uint)puVar12;
    if ((*(uint *)(iVar7 + (int)param_1) & 1) == 0) {
      if (*(int *)((int)DAT_8ceb6b1c + (int)param_1) != 0) {
        (**(code **)((int)DAT_8ceb6b22 + (int)param_1))(param_1);
      }
      (**(code **)((int)DAT_8ceb6b24 + (int)param_1))(param_1);
      (*(code *)PTR_FUN_8ceb6b34)(param_1);
    }
    else {
      (**(code **)((int)DAT_8ceb6b1e + (int)param_1))(param_1);
      (**(code **)((int)DAT_8ceb6b20 + (int)param_1))(param_1);
    }
    uVar10 = param_1[2];
    puVar12 = (undefined1 *)param_1[3];
    uVar9 = *param_1;
    uVar11 = param_1[1];
    bVar2 = false;
  }
  iVar7 = (int)(puVar12 + ((int)(uVar10 + 7) >> 3) + -8) - *param_2;
  *param_4 = *param_4 + iVar7;
  *param_3 = *param_3 - iVar7;
  *param_2 = (int)(puVar12 + ((int)(uVar10 + 7) >> 3) + -8);
  (*(code *)PTR_callback_candidate_8ced92e2_8ceb6b80)(param_2,param_3,param_4);
  return 0;
}


/* Address: 8ceb8640 */

void FUN_8ceb8640(int param_1)

{
  *(undefined4 *)(param_1 + 0x10) = 0;
  *(undefined4 *)(param_1 + 0x14) = 0;
  *(undefined4 *)(param_1 + 0x18) = 0;
  *(undefined4 *)(param_1 + 0x1c) = 0;
  return;
}


/* Address: 8ceb864c */

void FUN_8ceb864c(int param_1)

{
  int iVar1;
  int iVar2;
  
  iVar1 = (int)DAT_8ceb876e;
  iVar2 = (int)DAT_8ceb876c;
  *(int *)(iVar1 + param_1) = iVar2;
  *(int *)(iVar1 + 8 + param_1) = iVar2;
  *(int *)(iVar1 + 4 + param_1) = iVar2;
  return;
}


/* Address: 8ceb865c */

/* WARNING: Removing unreachable block (ram,0x8ceb869a) */
/* WARNING: Removing unreachable block (ram,0x8ceb8686) */
/* WARNING: Removing unreachable block (ram,0x8ceb86f4) */

undefined4 callback_candidate_8ceb865c(uint *param_1,int *param_2,int *param_3,int *param_4)

{
  undefined1 uVar1;
  short sVar2;
  uint uVar3;
  uint uVar4;
  int *piVar5;
  uint uVar6;
  uint uVar7;
  int iVar8;
  uint uVar9;
  uint uVar10;
  uint uVar11;
  undefined1 *puVar12;
  undefined1 *puVar13;
  undefined1 *puVar14;
  undefined1 *puVar15;
  int iVar16;
  undefined4 local_20;
  
  uVar9 = *param_1;
  uVar10 = param_1[2];
  uVar11 = param_1[1];
  puVar12 = (undefined1 *)param_1[3];
  local_20 = 0;
  uVar4 = param_2[2];
  uVar6 = param_2[1];
  uVar7 = uVar9 >> 0x15;
  if (0x15 < (int)uVar10) {
    uVar3 = -(0x35 - uVar10);
    if ((int)uVar3 < 0) {
      uVar3 = uVar11 >> (~uVar3 & 0x1f) + 1;
    }
    else {
      uVar3 = uVar11 << (uVar3 & 0x1f);
    }
    uVar7 = uVar7 | uVar3;
  }
  piVar5 = DAT_8ceb8770;
  if (uVar7 >> 7 != 0) {
    uVar7 = uVar7 >> 6;
    piVar5 = DAT_8ceb8774;
  }
  sVar2 = *(short *)(*piVar5 + uVar7 * 2);
  iVar8 = (int)(char)sVar2;
  if (iVar8 == 0x7f) {
    local_20 = 0xffffffff;
  }
  else {
    uVar7 = (uint)(int)sVar2 >> 8;
    uVar10 = uVar10 + (uVar7 & 0xff);
    if ((int)uVar10 < 0x20) {
      uVar7 = uVar9 << (uVar7 & 0x1f);
    }
    else {
      uVar1 = *puVar12;
      puVar13 = puVar12 + 1;
      uVar10 = uVar10 - 0x20;
      puVar14 = puVar12 + 2;
      if ((int)uVar10 < 0) {
        uVar7 = uVar11 >> (~uVar10 & 0x1f) + 1;
      }
      else {
        uVar7 = uVar11 << (uVar10 & 0x1f);
      }
      puVar15 = puVar12 + 3;
      puVar12 = puVar12 + 4;
      uVar11 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar13),*puVar14),*puVar15);
    }
    uVar9 = uVar7;
    if (iVar8 == 0) {
      *param_3 = *param_4;
    }
    else {
      if (uVar6 != 0) {
        iVar16 = 0x20 - uVar6;
        if ((int)uVar10 < iVar16) {
          uVar3 = -iVar16;
          if ((int)uVar6 < 0) {
            uVar9 = uVar7 >> (~uVar6 & 0x1f) + 1;
          }
          else {
            uVar9 = uVar7 << (uVar6 & 0x1f);
          }
          if ((int)uVar3 < 0) {
            uVar7 = uVar7 >> (~uVar3 & 0x1f) + 1;
          }
          else {
            uVar7 = uVar7 << (uVar3 & 0x1f);
          }
          uVar10 = uVar10 + uVar6;
        }
        else {
          uVar10 = uVar10 - iVar16;
          if (uVar10 == 0) {
            uVar9 = -iVar16;
            if ((int)uVar9 < 0) {
              uVar7 = uVar7 >> (~uVar9 & 0x1f) + 1;
            }
            else {
              uVar7 = uVar7 << (uVar9 & 0x1f);
            }
          }
          else {
            uVar9 = -(uVar6 - uVar10);
            if ((int)uVar9 < 0) {
              uVar9 = uVar11 >> (~uVar9 & 0x1f) + 1;
            }
            else {
              uVar9 = uVar11 << (uVar9 & 0x1f);
            }
            uVar3 = -iVar16;
            if ((int)uVar3 < 0) {
              uVar7 = (uVar7 | uVar9) >> (~uVar3 & 0x1f) + 1;
            }
            else {
              uVar7 = (uVar7 | uVar9) << (uVar3 & 0x1f);
            }
            if ((int)uVar10 < 0) {
              uVar11 = uVar11 >> (~uVar10 & 0x1f) + 1;
            }
            else {
              uVar11 = uVar11 << (uVar10 & 0x1f);
            }
          }
          uVar1 = *puVar12;
          puVar13 = puVar12 + 1;
          puVar14 = puVar12 + 2;
          puVar15 = puVar12 + 3;
          puVar12 = puVar12 + 4;
          uVar9 = uVar11;
          uVar11 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar13),*puVar14),*puVar15);
        }
        if ((int)uVar6 < 0) {
          iVar8 = iVar8 >> (~uVar6 & 0x1f) + 1;
        }
        else {
          iVar8 = iVar8 << (uVar6 & 0x1f);
        }
        iVar16 = (param_2[3] + -1) - uVar7;
        if (iVar8 < 1) {
          iVar8 = iVar8 + iVar16;
        }
        else {
          iVar8 = iVar8 - iVar16;
        }
      }
      if ((int)uVar4 < 0) {
        iVar8 = iVar8 + *param_4 >> (~uVar4 & 0x1f) + 1;
      }
      else {
        iVar8 = iVar8 + *param_4 << (uVar4 & 0x1f);
      }
      uVar4 = -uVar4;
      if ((int)uVar4 < 0) {
        iVar8 = iVar8 >> (~uVar4 & 0x1f) + 1;
      }
      else {
        iVar8 = iVar8 << (uVar4 & 0x1f);
      }
      *param_3 = iVar8;
      *param_4 = iVar8;
    }
    if (*param_2 != 0) {
      *param_3 = *param_3 << 1;
    }
  }
  *param_1 = uVar9;
  param_1[1] = uVar11;
  param_1[2] = uVar10;
  param_1[3] = (uint)puVar12;
  return local_20;
}


/* Address: 8ceb87b4 */

/* WARNING: Removing unreachable block (ram,0x8ceb896e) */
/* WARNING: Removing unreachable block (ram,0x8ceb8818) */
/* WARNING: Removing unreachable block (ram,0x8ceb893a) */
/* WARNING: Removing unreachable block (ram,0x8ceb8804) */
/* WARNING: Removing unreachable block (ram,0x8ceb8864) */
/* WARNING: Removing unreachable block (ram,0x8ceb8a46) */
/* WARNING: Removing unreachable block (ram,0x8ceb89a6) */
/* WARNING: Removing unreachable block (ram,0x8ceb8960) */
/* WARNING: Removing unreachable block (ram,0x8ceb8a94) */

undefined4 callback_candidate_8ceb87b4(uint *param_1,int *param_2,int *param_3,int *param_4)

{
  undefined1 uVar1;
  bool bVar2;
  int iVar3;
  uint uVar4;
  int *piVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  undefined1 *puVar11;
  undefined1 *puVar12;
  undefined1 *puVar13;
  undefined1 *puVar14;
  uint uVar15;
  
  bVar2 = true;
  uVar10 = param_1[1];
  uVar8 = *param_1;
  puVar11 = (undefined1 *)param_1[3];
  uVar9 = param_1[2];
  while( true ) {
    uVar15 = uVar8 >> 9;
    if (9 < (int)uVar9) {
      uVar4 = -(0x29 - uVar9);
      if ((int)uVar4 < 0) {
        uVar4 = uVar10 >> (~uVar4 & 0x1f) + 1;
      }
      else {
        uVar4 = uVar10 << (uVar4 & 0x1f);
      }
      uVar15 = uVar15 | uVar4;
    }
    if (uVar15 == 0) break;
    iVar6 = *(int *)((int)DAT_8ceb88a8 + (int)param_1);
    while( true ) {
      do {
        uVar15 = uVar8 >> 0x15;
        if (0x15 < (int)uVar9) {
          uVar4 = -(0x35 - uVar9);
          if ((int)uVar4 < 0) {
            uVar4 = uVar10 >> (~uVar4 & 0x1f) + 1;
          }
          else {
            uVar4 = uVar10 << (uVar4 & 0x1f);
          }
          uVar15 = uVar15 | uVar4;
        }
        piVar5 = DAT_8ceb88ac;
        if (uVar15 >> 7 != 0) {
          uVar15 = uVar15 >> 6;
          piVar5 = DAT_8ceb88b0;
        }
        uVar15 = (uint)*(short *)(*piVar5 + uVar15 * 2);
        uVar9 = uVar9 + (uVar15 & 0xf);
        if ((int)uVar9 < 0x20) {
          uVar8 = uVar8 << (uVar15 & 0xf);
        }
        else {
          uVar1 = *puVar11;
          puVar12 = puVar11 + 1;
          uVar9 = uVar9 - 0x20;
          puVar13 = puVar11 + 2;
          if ((int)uVar9 < 0) {
            uVar8 = uVar10 >> (~uVar9 & 0x1f) + 1;
          }
          else {
            uVar8 = uVar10 << (uVar9 & 0x1f);
          }
          puVar14 = puVar11 + 3;
          puVar11 = puVar11 + 4;
          uVar10 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar12),*puVar13),*puVar14);
        }
        uVar4 = (uVar15 >> 2 & 0xff) >> 2;
      } while (uVar4 == 0x22);
      if (uVar4 != 0x23) break;
      *(int *)((int)DAT_8ceb88a8 + (int)param_1) = *(int *)((int)DAT_8ceb88a8 + (int)param_1) + 0x21
      ;
    }
    if (uVar4 == 0x24) {
LAB_8ceb88a4:
      uVar15 = 0xfffffffe;
    }
    else {
      iVar3 = (int)DAT_8ceb88a8;
      *(uint *)(iVar3 + (int)param_1) = *(int *)(iVar3 + (int)param_1) + uVar4;
      *(uint *)(iVar3 + 8 + (int)param_1) = uVar15 >> 10;
      if (*(int *)(iVar3 + 4 + (int)param_1) < *(int *)(iVar3 + (int)param_1)) goto LAB_8ceb88a4;
      uVar15 = *(int *)((int)DAT_8ceb8990 + (int)param_1) - iVar6;
    }
    if (uVar15 == 0xfffffffe) break;
    if ((!bVar2) && (1 < uVar15)) {
      (**(code **)((int)DAT_8ceb8992 + (int)param_1))(param_1,uVar15);
      FUN_8ceb864c(param_1);
    }
    if ((*(uint *)((int)DAT_8ceb8994 + (int)param_1) & 0x20) == 0) {
      uVar15 = uVar8 >> 0x1a;
      if (0x1a < (int)uVar9) {
        uVar4 = -(0x3a - uVar9);
        if ((int)uVar4 < 0) {
          uVar4 = uVar10 >> (~uVar4 & 0x1f) + 1;
        }
        else {
          uVar4 = uVar10 << (uVar4 & 0x1f);
        }
        uVar15 = uVar15 | uVar4;
      }
      uVar15 = (uint)*(short *)(*DAT_8ceb8998 + uVar15 * 2);
      uVar9 = uVar9 + (uVar15 & 0xff);
      *(uint *)((int)DAT_8ceb8994 + (int)param_1) = uVar15 >> 8;
      if ((int)uVar9 < 0x20) {
        uVar8 = uVar8 << (uVar15 & 0x1f);
      }
      else {
        uVar1 = *puVar11;
        puVar12 = puVar11 + 1;
        uVar9 = uVar9 - 0x20;
        puVar13 = puVar11 + 2;
        if ((int)uVar9 < 0) {
          uVar8 = uVar10 >> (~uVar9 & 0x1f) + 1;
        }
        else {
          uVar8 = uVar10 << (uVar9 & 0x1f);
        }
        puVar14 = puVar11 + 3;
        puVar11 = puVar11 + 4;
        uVar10 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar12),*puVar13),*puVar14);
      }
    }
    uVar15 = uVar8;
    if ((*(uint *)((int)DAT_8ceb8994 + (int)param_1) & 0x10) != 0) {
      if ((int)uVar9 < 0x1b) {
        uVar15 = uVar8 << 5;
        *(uint *)((int)DAT_8ceb8a98 + (int)param_1) = uVar8 >> 0x1b;
        uVar9 = uVar9 + 5;
      }
      else {
        uVar9 = uVar9 - 0x1b;
        if (uVar9 == 0) {
          *(uint *)((int)DAT_8ceb8996 + (int)param_1) = uVar8 >> 0x1b;
        }
        else {
          uVar15 = -(5 - uVar9);
          if ((int)uVar15 < 0) {
            uVar15 = uVar10 >> (~uVar15 & 0x1f) + 1;
          }
          else {
            uVar15 = uVar10 << (uVar15 & 0x1f);
          }
          *(uint *)((int)DAT_8ceb8996 + (int)param_1) = (uVar8 | uVar15) >> 0x1b;
          if ((int)uVar9 < 0) {
            uVar10 = uVar10 >> (~uVar9 & 0x1f) + 1;
          }
          else {
            uVar10 = uVar10 << (uVar9 & 0x1f);
          }
        }
        uVar1 = *puVar11;
        puVar12 = puVar11 + 1;
        puVar13 = puVar11 + 2;
        puVar14 = puVar11 + 3;
        puVar11 = puVar11 + 4;
        uVar15 = uVar10;
        uVar10 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar12),*puVar13),*puVar14);
      }
    }
    if ((*(uint *)((int)DAT_8ceb8a9a + (int)param_1) & 8) != 0) {
      iVar7 = (int)DAT_8ceb8a9c;
      iVar3 = (int)DAT_8ceb8a9e;
      iVar6 = (int)DAT_8ceb8aa0;
      *param_1 = uVar15;
      param_1[1] = uVar10;
      param_1[2] = uVar9;
      param_1[3] = (uint)puVar11;
      iVar6 = callback_candidate_8ceb865c
                        (param_1,iVar6 + (int)param_1,iVar3 + (int)param_1,iVar7 + (int)param_1);
      iVar3 = callback_candidate_8ceb865c
                        (param_1,(int)DAT_8ceb8aa0 + (int)param_1,(int)DAT_8ceb8aa4 + (int)param_1,
                         (int)DAT_8ceb8aa2 + (int)param_1);
      uVar9 = param_1[2];
      uVar15 = *param_1;
      uVar10 = param_1[1];
      puVar11 = (undefined1 *)param_1[3];
      if (iVar6 != 0 || iVar3 != 0) break;
    }
    if ((*(uint *)((int)DAT_8ceb8a9a + (int)param_1) & 4) != 0) {
      iVar7 = (int)DAT_8ceb8aa6;
      iVar3 = (int)DAT_8ceb8aa8;
      iVar6 = (int)DAT_8ceb8aaa;
      *param_1 = uVar15;
      param_1[1] = uVar10;
      param_1[2] = uVar9;
      param_1[3] = (uint)puVar11;
      iVar6 = callback_candidate_8ceb865c
                        (param_1,iVar6 + (int)param_1,iVar3 + (int)param_1,iVar7 + (int)param_1);
      iVar3 = callback_candidate_8ceb865c
                        (param_1,(int)DAT_8ceb8aaa + (int)param_1,(int)DAT_8ceb8aae + (int)param_1,
                         (int)DAT_8ceb8aac + (int)param_1);
      uVar9 = param_1[2];
      uVar15 = *param_1;
      uVar10 = param_1[1];
      puVar11 = (undefined1 *)param_1[3];
      if (iVar6 != 0 || iVar3 != 0) break;
    }
    if ((*(uint *)((int)DAT_8ceb8a9a + (int)param_1) & 2) == 0) {
      *(undefined4 *)((int)DAT_8ceb8b70 + (int)param_1) = 0;
    }
    else {
      uVar8 = uVar15 >> 0x17;
      if (0x17 < (int)uVar9) {
        uVar4 = -(0x37 - uVar9);
        if ((int)uVar4 < 0) {
          uVar4 = uVar10 >> (~uVar4 & 0x1f) + 1;
        }
        else {
          uVar4 = uVar10 << (uVar4 & 0x1f);
        }
        uVar8 = uVar8 | uVar4;
      }
      uVar8 = (uint)*(short *)(*DAT_8ceb8ab4 + uVar8 * 2);
      uVar9 = uVar9 + (uVar8 & 0xff);
      *(uint *)((int)DAT_8ceb8ab0 + (int)param_1) = (uVar8 & 0xfffffff0) << 0x10;
      if ((int)uVar9 < 0x20) {
        uVar15 = uVar15 << (uVar8 & 0x1f);
      }
      else {
        uVar1 = *puVar11;
        puVar12 = puVar11 + 1;
        uVar9 = uVar9 - 0x20;
        puVar13 = puVar11 + 2;
        if ((int)uVar9 < 0) {
          uVar15 = uVar10 >> (~uVar9 & 0x1f) + 1;
        }
        else {
          uVar15 = uVar10 << (uVar9 & 0x1f);
        }
        puVar14 = puVar11 + 3;
        puVar11 = puVar11 + 4;
        uVar10 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar12),*puVar13),*puVar14);
      }
    }
    iVar6 = (int)DAT_8ceb8b72;
    *param_1 = uVar15;
    param_1[1] = uVar10;
    param_1[2] = uVar9;
    param_1[3] = (uint)puVar11;
    if ((*(uint *)(iVar6 + (int)param_1) & 1) == 0) {
      if (*(int *)((int)DAT_8ceb8b70 + (int)param_1) != 0) {
        (**(code **)((int)DAT_8ceb8b7c + (int)param_1))(param_1);
      }
      iVar6 = (int)DAT_8ceb8b7e;
      *(undefined4 *)((int)DAT_8ceb8b7e + (int)param_1) =
           ((undefined4 *)((int)DAT_8ceb8b7e + (int)param_1))
           [(int)(*(uint *)((int)DAT_8ceb8b72 + (int)param_1) & 0xc) >> 2];
      (**(code **)(iVar6 + (int)param_1))(param_1);
      FUN_8ceb864c(param_1);
    }
    else {
      (**(code **)((int)DAT_8ceb8b74 + (int)param_1))(param_1);
      (**(code **)((int)DAT_8ceb8b76 + (int)param_1))(param_1);
      FUN_8ceb8640((int)DAT_8ceb8b78 + (int)param_1);
      FUN_8ceb8640((int)DAT_8ceb8b7a + (int)param_1);
    }
    uVar9 = param_1[2];
    puVar11 = (undefined1 *)param_1[3];
    uVar8 = *param_1;
    uVar10 = param_1[1];
    bVar2 = false;
  }
  iVar6 = (int)(puVar11 + ((int)(uVar9 + 7) >> 3) + -8) - *param_2;
  *param_4 = *param_4 + iVar6;
  *param_3 = *param_3 - iVar6;
  *param_2 = (int)(puVar11 + ((int)(uVar9 + 7) >> 3) + -8);
  (*(code *)PTR_callback_candidate_8ced92e2_8ceb8b80)(param_2,param_3,param_4);
  return 0;
}


/* Address: 8cecf068 */

uint FUN_8cecf068(void)

{
  byte in_IMASK;
  byte in_S;
  byte in_T;
  
  return (((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4) & 0xf0) >> 4;
}


/* Address: 8cecf080 */

uint FUN_8cecf080(uint param_1)

{
  byte in_MD;
  byte in_RB;
  byte in_BL;
  byte in_FD;
  byte in_M;
  byte in_Q;
  byte in_IMASK;
  byte in_S;
  byte in_T;
  
  return (param_1 & 0xf) << 4 |
         ((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4 | (uint)in_Q << 8 | (uint)in_M << 9 |
          (uint)in_FD << 0xf | (uint)in_BL << 0x1c | (uint)in_RB << 0x1d | (uint)in_MD << 0x1e) &
         (int)DAT_8cecf142;
}


/* Address: 8cecf096 */

void FUN_8cecf096(uint param_1,undefined4 param_2,int param_3,int param_4)

{
  undefined *puVar1;
  undefined *puVar2;
  undefined *puVar3;
  undefined4 uVar4;
  
  puVar2 = PTR_DAT_8cecf14c;
  puVar1 = PTR_DAT_8cecf148;
  *(uint *)PTR_DAT_8cecf144 = param_1;
  *(undefined4 *)puVar1 = param_2;
  puVar3 = PTR_DAT_8cecf154;
  puVar1 = PTR_DAT_8cecf150;
  *(int *)puVar2 = param_3;
  *(int *)puVar1 = param_4;
  puVar1 = PTR_DAT_8cecf158;
  if ((param_1 & 4) != 0) {
    param_3 = param_3 << 1;
  }
  *(int *)puVar3 = param_3;
  if ((param_1 & 2) == 0) {
    *(undefined4 *)puVar1 = DAT_8cecf160;
  }
  else {
    *(undefined4 *)puVar1 = DAT_8cecf15c;
  }
  uVar4 = (*DAT_8cecf164)();
  *(undefined4 *)PTR_DAT_8cecf168 = uVar4;
  if (param_4 == 2) {
    (*(code *)PTR_FUN_8cecf16c)(2);
    return;
  }
  if (param_4 == 3) {
    (*(code *)PTR_FUN_8cecf16c)(3);
    return;
  }
  return;
}


/* Address: 8cecf0f8 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 * callback_candidate_8cecf0f8(undefined4 *param_1)

{
  code *pcVar1;
  undefined *puVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  int iVar5;
  int iVar6;
  undefined4 *puVar7;
  undefined4 *puVar8;
  undefined4 *puVar9;
  char in_FPSCR_SZ;
  undefined4 local_30;
  undefined1 auStack_2c [4];
  undefined1 auStack_28 [4];
  undefined1 auStack_24 [4];
  undefined1 auStack_20 [4];
  undefined1 auStack_1c [8];
  
  iVar6 = 0;
  do {
    puVar9 = (undefined4 *)(iVar6 * 0x80 + DAT_8cecf170 + 4);
    if (puVar9[1] == 0) break;
    iVar6 = iVar6 + 1;
  } while (iVar6 < 0x20);
  if (iVar6 == 0x20) {
    FUN_8cecf512(0xfffffff5);
    (*(code *)PTR_callback_candidate_8ced5374_8cecf178)
              (PTR_s_W2000_mwPlyCreateSofdec_can_t_cr_8cecf174);
  }
  else {
    iVar6 = (*(code *)PTR_callback_candidate_8ced0350_8cecf2b0)
                      (puVar9,*param_1,*(undefined4 *)PTR_DAT_8cecf2ac);
    if (iVar6 == 0) {
      *(undefined4 *)PTR_DAT_8cecf2b4 = 1;
      iVar6 = (*(code *)PTR_callback_candidate_8ced03f8_8cecf2b8)
                        (puVar9,*param_1,param_1[1],param_1[2],param_1[3],param_1[4],param_1[5],
                         param_1[6]);
      puVar9[0xc] = iVar6;
      if (iVar6 != 0) {
        (*(code *)PTR_callback_candidate_8ced01bc_8cecf2bc)
                  (*param_1,param_1[1],&local_30,auStack_2c,auStack_28,auStack_24,auStack_20,
                   auStack_1c);
        (*(code *)PTR_callback_candidate_8ced0620_8cecf2c8)
                  (puVar9,param_1,*(undefined4 *)PTR_DAT_8cecf2c4,*(undefined4 *)PTR_DAT_8cecf2c0);
        iVar5 = (*(code *)PTR_FUN_8cecf2cc)();
        puVar9[0xe] = iVar5;
        if (iVar5 != 0) {
          *puVar9 = PTR_DAT_8cecf2d0;
          pcVar1 = DAT_8cecf2d4;
          puVar9[0x1f] = local_30;
          (*pcVar1)();
          iVar5 = (int)DAT_8cecf2a8;
          puVar9[0xb] = 1;
          puVar9[0x13] = param_1[7];
          puVar9[0x10] = 0;
          puVar9[0x12] = 0;
          puVar9[0x11] = 0;
          puVar9[0x19] = 0;
          puVar9[0x17] = iVar5;
          puVar2 = PTR_callback_candidate_8ced45c8_8cecf2dc;
          puVar9[0x18] = 6;
          if (in_FPSCR_SZ == '\0') {
            puVar9[0x1c] = _DAT_8cecf2d8;
          }
          else {
            *(ulonglong *)(puVar9 + 0x1c) =
                 CONCAT44(PTR_callback_candidate_8ced45c8_8cecf2dc,_DAT_8cecf2d8);
          }
          puVar9[2] = 0;
          iVar5 = (*(code *)puVar2)(iVar6,7,puVar9 + 0xf);
          if (iVar5 != 0) {
            puVar9[0xf] = 0;
          }
          iVar6 = (*(code *)PTR_callback_candidate_8ced45c8_8cecf2dc)(iVar6,3,puVar9 + 0x16);
          if (iVar6 != 0) {
            puVar9[0x16] = 0;
          }
          puVar7 = DAT_8cecf2ec;
          puVar4 = DAT_8cecf2e8;
          puVar3 = DAT_8cecf2e4;
          puVar8 = DAT_8cecf2e0;
          puVar9[1] = 1;
          *puVar8 = 0;
          *puVar3 = 0;
          *puVar4 = 0;
          puVar8 = puVar7 + 0x10;
          for (; puVar7 < puVar8; puVar7 = puVar7 + 1) {
            *puVar7 = 0;
          }
          return puVar9;
        }
      }
    }
  }
  return (undefined4 *)0x0;
}


/* Address: 8cecf2f0 */

void callback_candidate_8cecf2f0(int param_1)

{
  int *piVar1;
  
  if (param_1 != 0) {
    callback_candidate_8cecf450(param_1);
    *(undefined4 *)(param_1 + 4) = 0;
    if (*(int *)(param_1 + 0x34) != 0) {
      (*(code *)PTR_callback_candidate_8ced0952_8cecf398)(*(undefined4 *)(param_1 + 0x34));
    }
    if (*(int *)(param_1 + 0x38) != 0) {
      piVar1 = *(int **)(param_1 + 0x38);
      (**(code **)(*piVar1 + 0xc))(piVar1,piVar1);
    }
    if (*(int *)(param_1 + 0x30) != 0) {
      (*(code *)PTR_callback_candidate_8ced0604_8cecf39c)(*(undefined4 *)(param_1 + 0x30));
    }
    if (*(int *)(param_1 + 0x40) != 0) {
      (*(code *)PTR_callback_candidate_8ced583c_8cecf3a0)(*(undefined4 *)(param_1 + 0x40));
      *(undefined4 *)(param_1 + 0x40) = 0;
    }
    *(undefined4 *)PTR_DAT_8cecf3a4 = 0;
    (*(code *)PTR_callback_candidate_8ced03c4_8cecf3a8)(param_1);
    return;
  }
  return;
}


/* Address: 8cecf35e */

/* WARNING: Removing unreachable block (ram,0x8cecf3c6) */
/* WARNING: Removing unreachable block (ram,0x8cecf3ec) */

void callback_candidate_8cecf35e(int param_1,undefined4 param_2)

{
  undefined *puVar1;
  int iVar2;
  undefined4 uVar3;
  
  uVar3 = *(undefined4 *)(param_1 + 0x38);
  callback_candidate_8cecf450(param_1);
  iVar2 = (*(code *)PTR_thunk_EXT_FUN_8c65bdd2_8cecf3ac)(param_2,uVar3);
  puVar1 = PTR_callback_candidate_8ced5374_8cecf3b0;
  *(int *)(param_1 + 0x34) = iVar2;
  if (iVar2 == 0) {
    FUN_8cecf512(0xffffff9a);
    (*(code *)puVar1)(PTR_s_W2001_mwPlyStartFname_can_t_open_8cecf3b4,param_2);
  }
  else {
    iVar2 = *(int *)(param_1 + 0x7c);
    if (iVar2 < 0) {
      iVar2 = iVar2 + DAT_8cecf4bc;
    }
    iVar2 = (*(code *)PTR_callback_candidate_8ced0960_8cecf4c0)
                      (*(undefined4 *)(param_1 + 0x34),iVar2 >> 0xb);
    if (iVar2 != 0) {
      FUN_8cecf512(0xfffffffc);
      (*(code *)puVar1)(PTR_s_W2005_mwPlyStartFname__gsc_param_8cecf4c4);
    }
    iVar2 = *(int *)(param_1 + 0x7c);
    if (iVar2 < 0) {
      iVar2 = iVar2 + DAT_8cecf4bc;
    }
    iVar2 = (*(code *)PTR_FUN_8cecf4c8)(*(undefined4 *)(param_1 + 0x34),iVar2 >> 0xb);
    if (iVar2 != 0) {
      FUN_8cecf512(0xfffffffc);
      (*(code *)puVar1)(PTR_s_W2006_mwPlyStartFname__gsc_prame_8cecf4cc);
    }
    iVar2 = (*(code *)PTR_callback_candidate_8ced0976_8cecf4d0)(*(undefined4 *)(param_1 + 0x34));
    if (iVar2 != 0) {
      FUN_8cecf512(0xffffff99);
      return;
    }
    (*(code *)PTR_callback_candidate_8ced0700_8cecf4d4)(param_1);
    (*(code *)PTR_callback_candidate_8ced071c_8cecf4d8)(param_1);
    if (*(int *)(param_1 + 0x40) != 0) {
      (*(code *)PTR_callback_candidate_8ced583c_8cecf4dc)(*(undefined4 *)(param_1 + 0x40));
      *(undefined4 *)(param_1 + 0x40) = 0;
    }
    *(undefined4 *)(param_1 + 0x44) = 0;
    *(undefined4 *)(param_1 + 8) = 1;
  }
  return;
}


/* Address: 8cecf450 */

void callback_candidate_8cecf450(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 uVar3;
  
  uVar3 = *(undefined4 *)(param_1 + 0x30);
  uVar1 = FUN_8cecf068();
  FUN_8cecfaae();
  *(undefined4 *)(param_1 + 8) = 0;
  FUN_8cecf080(uVar1);
  iVar2 = (*(code *)PTR_callback_candidate_8ced4084_8cecf4e0)(uVar3);
  if (iVar2 != 0) {
    FUN_8cecf512((int)DAT_8cecf4be);
    (*(code *)PTR_callback_candidate_8ced5374_8cecf4e8)
              (PTR_s_E2003_mwSfdStop_can_t_stop_SFD_8cecf4e4);
  }
  if (*(int *)(param_1 + 0x34) != 0) {
    (*(code *)PTR_callback_candidate_8ced0952_8cecf4ec)(*(undefined4 *)(param_1 + 0x34));
    *(undefined4 *)(param_1 + 0x34) = 0;
  }
  if (*(int *)(param_1 + 0x40) != 0) {
    (*(code *)PTR_callback_candidate_8ced583c_8cecf4dc)(*(undefined4 *)(param_1 + 0x40));
    *(undefined4 *)(param_1 + 0x40) = 0;
  }
                    /* WARNING: Could not recover jumptable at 0x8cecf4b8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(**(int **)(param_1 + 0x38) + 0x14))(*(int **)(param_1 + 0x38));
  return;
}


/* Address: 8cecf4f0 */

undefined4 callback_candidate_8cecf4f0(int param_1)

{
  if (param_1 == 0) {
    FUN_8cecf512(0xfffffff4);
    (*(code *)PTR_callback_candidate_8ced5374_8cecf6f4)
              (PTR_s_W2004_mwSfdGetStat__handle_is_NU_8cecf6f0);
    return 0;
  }
  return *(undefined4 *)(param_1 + 8);
}


/* Address: 8cecf512 */

int FUN_8cecf512(int param_1)

{
  undefined4 *puVar1;
  code *pcVar2;
  
  puVar1 = DAT_8cecf6f8;
  DAT_8cecf6f8[2] = param_1;
  if (param_1 == 0) {
    param_1 = 0;
  }
  else {
    pcVar2 = (code *)*puVar1;
    if (pcVar2 != (code *)0x0) {
      (*pcVar2)(puVar1[1],param_1,0);
    }
  }
  return param_1;
}


/* Address: 8cecf540 */

int FUN_8cecf540(int param_1,int param_2)

{
  if ((param_2 == 0) && (param_1 == 1)) {
    param_1 = 0;
  }
  return param_1;
}


/* Address: 8cecf550 */

int callback_candidate_8cecf550(int param_1,undefined4 param_2)

{
  undefined *puVar1;
  int iVar2;
  undefined4 uVar3;
  int local_18;
  int local_14;
  undefined4 local_10;
  
  puVar1 = PTR_callback_candidate_8ced4d6c_8cecf700;
  uVar3 = *(undefined4 *)(param_1 + 0x30);
  *(undefined4 *)PTR_DAT_8cecf6fc = param_2;
  local_10 = param_2;
  (*(code *)puVar1)(uVar3,&local_14);
  iVar2 = FUN_8cecf540(local_14 == 0,local_10);
  (*(code *)PTR_callback_candidate_8ced4f66_8cecf704)(uVar3,&local_18);
  if ((*(int *)PTR_DAT_8cecf708 <= local_18) && (iVar2 == 0)) {
    iVar2 = 2;
  }
  return iVar2;
}


/* Address: 8cecf59c */

void FUN_8cecf59c(int param_1)

{
  undefined *puVar1;
  
  puVar1 = PTR_DAT_8cecf70c;
  if ((param_1 != 0) && (param_1 - *(int *)PTR_DAT_8cecf70c != 1)) {
    *(int *)PTR_DAT_8cecf710 = *(int *)PTR_DAT_8cecf710 + 1;
  }
  *(int *)puVar1 = param_1;
  return;
}


/* Address: 8cecf5b8 */

/* WARNING: Removing unreachable block (ram,0x8cecf5dc) */
/* WARNING: Removing unreachable block (ram,0x8cecf5f8) */

void FUN_8cecf5b8(undefined4 param_1,int param_2,int param_3,int param_4)

{
  int *piVar1;
  int *piVar2;
  int *piVar3;
  code *pcVar4;
  undefined4 uVar5;
  double dVar6;
  float fVar7;
  int iVar8;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  int local_24;
  int local_20;
  int local_1c;
  int local_18;
  int local_14;
  
  local_1c = param_4;
  local_18 = param_2;
  local_14 = param_3;
  callback_candidate_8cecfec8(&local_24,&local_20);
  piVar3 = DAT_8cecf720;
  piVar2 = DAT_8cecf71c;
  piVar1 = DAT_8cecf714._4_4_;
  if (in_FPSCR_SZ == '\0') {
    dVar6 = (double)CONCAT44((undefined4)DAT_8cecf714,param_1);
  }
  else {
    dVar6 = (double)CONCAT44(DAT_8cecf714._4_4_,(undefined4)DAT_8cecf714);
  }
  fVar7 = (float)((ulonglong)dVar6 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    iVar8 = (int)(((float)local_24 * fVar7) / (float)local_20);
  }
  else {
    iVar8 = (int)(((double)local_20 * dVar6) / ((double)local_20 * dVar6));
  }
  *DAT_8cecf714._4_4_ = iVar8;
  pcVar4 = DAT_8cecf724;
  if (in_FPSCR_PR == '\0') {
    iVar8 = (int)(((float)local_18 * fVar7) / (float)local_14);
  }
  else {
    iVar8 = (int)(((double)local_14 * dVar6) / ((double)local_14 * dVar6));
  }
  *piVar2 = iVar8;
  uVar5 = (*pcVar4)();
  (*DAT_8cecf728)(uVar5);
  iVar8 = (*DAT_8cecf72c)();
  *piVar3 = iVar8 - *(int *)PTR_DAT_8cecf730;
  *(int *)PTR_DAT_8cecf734 = *piVar1 - *piVar2;
  *(int *)PTR_DAT_8cecf738 = *piVar3 - *piVar2;
  *(int *)PTR_DAT_8cecf73c = *piVar3 - *piVar1;
  if (local_1c == 0) {
    uVar5 = (*DAT_8cecf724)();
    (*DAT_8cecf728)(uVar5);
    uVar5 = (*DAT_8cecf72c)();
    *(undefined4 *)PTR_DAT_8cecf730 = uVar5;
  }
  return;
}


/* Address: 8cecf668 */

void FUN_8cecf668(undefined4 param_1,int param_2,undefined4 param_3,undefined4 param_4)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  
  *(undefined4 *)PTR_DAT_8cecf740 = param_4;
  FUN_8cecf59c(param_4);
  FUN_8cecf5b8(param_1,param_2,param_3,param_4);
  piVar1 = DAT_8cecf74c;
  iVar4 = *DAT_8cecf744;
  if (*DAT_8cecf748 != param_2) {
    *DAT_8cecf748 = param_2;
    piVar2 = DAT_8cecf750;
    iVar3 = iVar4 - *piVar1;
    *DAT_8cecf750 = iVar3;
    if (iVar3 < 0xf) {
      iVar3 = *piVar2;
    }
    else {
      iVar3 = 0xf;
    }
    *(int *)(DAT_8cecf754 + iVar3 * 4) = *(int *)(DAT_8cecf754 + iVar3 * 4) + 1;
  }
  *piVar1 = iVar4;
  return;
}


/* Address: 8cecf6c6 */

void callback_candidate_8cecf6c6(int param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 uVar1;
  
  uVar1 = *(undefined4 *)(param_1 + 0x30);
  FUN_8cecf668(param_1,param_2,param_3);
  (*(code *)PTR_callback_candidate_8ced4d2a_8cecf758)(uVar1,param_2,param_3);
  return;
}


/* Address: 8cecf75c */

void FUN_8cecf75c(undefined4 param_1,int param_2)

{
  undefined *puVar1;
  undefined *puVar2;
  undefined4 uVar3;
  int iVar4;
  undefined4 uVar5;
  undefined8 uVar6;
  char in_FPSCR_SZ;
  int local_58 [2];
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  
  uVar5 = *(undefined4 *)(param_2 + 0x30);
  (*(code *)PTR_callback_candidate_8ced4508_8cecf8d8)(uVar5,local_58);
  if ((0 < local_58[0]) && (*(int *)(param_2 + 0x40) == 0)) {
    uStack_18 = local_50;
    uStack_14 = local_4c;
    uStack_10 = *(undefined4 *)(param_2 + 0x4c);
    uVar3 = (*(code *)PTR_callback_candidate_8ced572e_8cecf8dc)(0,&uStack_18);
    puVar2 = PTR_FUN_8cecf8e4;
    puVar1 = PTR_callback_candidate_8cecf550_8cecf8e0;
    *(undefined4 *)(param_2 + 0x40) = uVar3;
    (*(code *)puVar2)(*(undefined4 *)(param_2 + 0x40),puVar1,param_2);
    (*(code *)PTR_FUN_8cecf8ec)
              (*(undefined4 *)(param_2 + 0x40),PTR_callback_candidate_8cecf6c6_8cecf8e8,param_2);
    if (*(int *)(param_2 + 100) != 0) {
      if (in_FPSCR_SZ == '\0') {
        uVar6 = CONCAT44(*(undefined4 *)(param_2 + 0x68),*(undefined4 *)(param_2 + 0x6c));
      }
      else {
        uVar6 = *(undefined8 *)(param_2 + 0x68);
      }
      (*(code *)PTR_FUN_8cecf8f0)(uVar6,*(undefined4 *)(param_2 + 0x40));
      if (in_FPSCR_SZ == '\0') {
        uVar6 = CONCAT44(*(undefined4 *)(param_2 + 0x74),*(undefined4 *)(param_2 + 0x78));
      }
      else {
        uVar6 = *(undefined8 *)(param_2 + 0x74);
      }
      (*(code *)PTR_FUN_8cecf8f4)(uVar6,*(undefined4 *)(param_2 + 0x40));
      param_1 = (undefined4)uVar6;
    }
    (*(code *)PTR_FUN_8cecf8f8)(*(undefined4 *)(param_2 + 0x40),*(undefined4 *)(param_2 + 0x5c));
    (*(code *)PTR_FUN_8cecf8fc)(*(undefined4 *)(param_2 + 0x40),*(undefined4 *)(param_2 + 0x60));
    if (in_FPSCR_SZ == '\0') {
      uVar6 = CONCAT44(*(undefined4 *)(param_2 + 0x70),param_1);
    }
    else {
      uVar6 = *(undefined8 *)(param_2 + 0x70);
    }
    (*(code *)PTR_FUN_8cecf900)(uVar6,*(undefined4 *)(param_2 + 0x40));
    *(undefined4 *)(param_2 + 8) = 2;
  }
  iVar4 = (*(code *)PTR_callback_candidate_8ced4342_8cecf904)(uVar5);
  if ((iVar4 == 4) || (iVar4 == 6)) {
    *(undefined4 *)(param_2 + 8) = 2;
  }
  iVar4 = (*(code *)PTR_callback_candidate_8ced4334_8cecf908)();
  if (iVar4 != 1) {
    *(undefined4 *)(param_2 + 8) = 3;
  }
  (*(code *)PTR_callback_candidate_8ced0870_8cecf90c)(param_2);
  return;
}


/* Address: 8cecf840 */

void FUN_8cecf840(undefined4 param_1)

{
  *(undefined4 *)PTR_DAT_8cecf910 = param_1;
  return;
}


/* Address: 8cecf846 */

void FUN_8cecf846(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  uVar2 = *(undefined4 *)(param_1 + 0x30);
  (*(code *)PTR_callback_candidate_8ced0870_8cecf90c)(param_1);
  iVar1 = (*(code *)PTR_callback_candidate_8ced4334_8cecf908)();
  if (iVar1 != 1) {
    FUN_8cecf840(uVar2);
    *(undefined4 *)(param_1 + 8) = 3;
  }
  return;
}


/* Address: 8cecf874 */

undefined4 FUN_8cecf874(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = *(int *)(param_1 + 8);
  if (iVar1 != 0) {
    if (iVar1 == 1) {
      FUN_8cecf75c(param_1);
    }
    else if (iVar1 == 2) {
      FUN_8cecf846(param_1);
    }
  }
  if ((*(int *)(param_1 + 0x34) != 0) &&
     (iVar1 = (*(code *)PTR_callback_candidate_8ced09ac_8cecf914)(*(undefined4 *)(param_1 + 0x34)),
     iVar1 != 0)) {
    iVar1 = *(int *)(param_1 + 0x34);
    uVar2 = FUN_8cecf068();
    *(undefined4 *)(param_1 + 8) = 4;
    *(undefined4 *)(param_1 + 0x34) = 0;
    FUN_8cecf080(uVar2);
    if (iVar1 != 0) {
      (*(code *)PTR_callback_candidate_8ced0952_8cecf918)(iVar1);
    }
  }
  return 0;
}


/* Address: 8cecf91c */

void FUN_8cecf91c(void)

{
  code *pcVar1;
  undefined *puVar2;
  undefined *puVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  int iVar7;
  int iVar8;
  uint uVar9;
  uint uVar10;
  
  pcVar1 = DAT_8cecfa1c;
  uVar4 = (*DAT_8cecfa1c)();
  *(undefined4 *)PTR_DAT_8cecfa20 = uVar4;
  uVar4 = (*pcVar1)();
  if (*(int *)PTR_DAT_8cecfa24 == 1) {
    (*(code *)PTR_callback_candidate_8ced38c4_8cecfa28)();
  }
  uVar5 = (*pcVar1)();
  uVar6 = (*DAT_8cecfa2c)(uVar4,uVar5);
  iVar7 = (*DAT_8cecfa30)(uVar6);
  puVar3 = PTR_DAT_8cecfa3c;
  puVar2 = PTR_DAT_8cecfa38;
  if (DAT_8cecfa16 <= iVar7) {
    *(undefined4 *)PTR_DAT_8cecfa34 = uVar4;
    *(undefined4 *)puVar2 = uVar5;
    *(int *)puVar3 = *(int *)puVar3 + iVar7;
  }
  uVar4 = (*pcVar1)();
  uVar9 = (uint)DAT_8cecfa1a;
  iVar7 = (int)DAT_8cecfa18;
  uVar10 = 0;
  do {
    iVar8 = DAT_8cecfa40 + uVar10;
    if (*(int *)(iVar8 + 4) != 0) {
      if (*(int *)(iVar8 + 8) == 0) {
        *(undefined4 *)(iVar8 + 0x48) = 0;
      }
      else {
        *(undefined4 *)(iVar8 + 0x48) = 1;
        FUN_8cecf874();
      }
    }
    uVar10 = uVar10 + iVar7;
  } while (uVar10 < uVar9);
  uVar5 = (*pcVar1)();
  uVar6 = (*DAT_8cecfa2c)(uVar4,uVar5);
  iVar7 = (*DAT_8cecfa30)(uVar6);
  puVar3 = PTR_DAT_8cecfa4c;
  puVar2 = PTR_DAT_8cecfa48;
  if (0x31 < iVar7) {
    *(undefined4 *)PTR_DAT_8cecfa44 = uVar4;
    *(undefined4 *)puVar2 = uVar5;
    *(int *)puVar3 = *(int *)puVar3 + iVar7;
  }
  *(undefined4 *)PTR_DAT_8cecfa50 = *(undefined4 *)PTR_DAT_8cecfa20;
  uVar4 = (*pcVar1)();
  *(undefined4 *)PTR_DAT_8cecfa54 = uVar4;
  return;
}


/* Address: 8cecfaae */

void FUN_8cecfaae(void)

{
  undefined4 *puVar1;
  code *pcVar2;
  
  if (*(int *)PTR_DAT_8cecfb8c == 1) {
    *(undefined4 *)PTR_DAT_8cecfb90 = 1;
    pcVar2 = DAT_8cecfb84;
    puVar1 = DAT_8cecfb80;
    while (*DAT_8cecfb94 == 1) {
      (*pcVar2)(*puVar1);
    }
    *(undefined4 *)PTR_DAT_8cecfb90 = 0;
  }
  return;
}


/* Address: 8cecfae4 */

void FUN_8cecfae4(int param_1)

{
  undefined *puVar1;
  undefined *puVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  int local_24 [2];
  
  uVar4 = *(undefined4 *)(param_1 + 0x30);
  if (*(int *)(param_1 + 0x40) != 0) {
    *(undefined4 *)(param_1 + 0x44) = 0;
    iVar3 = (*(code *)PTR_callback_candidate_8ced6508_8cecfb98)(*(undefined4 *)(param_1 + 0x40));
    puVar2 = PTR_callback_candidate_8ced42d8_8cecfba0;
    puVar1 = PTR_callback_candidate_8ced429c_8cecfb9c;
    iVar5 = 0;
    if (0 < iVar3) {
      do {
        (*(code *)puVar1)(uVar4,local_24);
        if (local_24[0] == 0) {
          return;
        }
        (*(code *)PTR_callback_candidate_8ced652a_8cecfba4)
                  (*(undefined4 *)(param_1 + 0x40),*(undefined4 *)(local_24[0] + 0x20),
                   *(undefined4 *)(local_24[0] + 0x14),*(undefined4 *)(local_24[0] + 0x18));
        (*(code *)puVar2)(uVar4,local_24[0]);
        iVar5 = iVar5 + 1;
        *(undefined4 *)(param_1 + 0x44) = 1;
      } while (iVar5 < iVar3);
    }
  }
  return;
}


/* Address: 8cecfba8 */

void FUN_8cecfba8(void)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  
  iVar1 = DAT_8cecfca0;
  uVar4 = 0;
  uVar3 = (uint)DAT_8cecfc9c;
  iVar2 = (int)DAT_8cecfc9a;
  do {
    if (*(int *)(iVar1 + uVar4 + 8) != 0) {
      FUN_8cecfae4();
    }
    uVar4 = uVar4 + iVar2;
  } while (uVar4 < uVar3);
  return;
}


/* Address: 8cecfbdc */

uint callback_candidate_8cecfbdc(void)

{
  undefined *puVar1;
  undefined *puVar2;
  int *piVar3;
  undefined4 uVar4;
  byte in_MD;
  byte bVar5;
  byte in_RB;
  byte bVar6;
  byte in_BL;
  byte bVar7;
  byte in_FD;
  byte bVar8;
  byte in_M;
  byte extraout_M;
  byte extraout_M_00;
  byte extraout_M_01;
  byte bVar9;
  byte in_Q;
  byte extraout_Q;
  byte extraout_Q_00;
  byte extraout_Q_01;
  byte bVar10;
  byte in_IMASK;
  byte bVar11;
  byte in_S;
  byte extraout_S;
  byte extraout_S_00;
  byte extraout_S_01;
  byte bVar12;
  byte in_T;
  byte extraout_T;
  byte bVar13;
  
  bVar7 = (byte)(((uint)in_BL << 0x1c) >> 0x18);
  bVar5 = bVar7 | (byte)(((uint)in_RB << 0x1d) >> 0x18);
  bVar11 = (byte)((((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4) & 0xf0) >> 4);
  bVar8 = (byte)((((uint)in_Q << 8 | (uint)in_M << 9 | (uint)in_FD << 0xf) & 0x8000) >> 0xf);
  bVar7 = (bVar7 & 0x10) >> 4;
  bVar6 = (bVar5 & 0x20) >> 5;
  bVar5 = ((bVar5 | (byte)(((uint)in_MD << 0x1e) >> 0x18)) & 0x40) >> 6;
  uVar4 = (*DAT_8cecfca4)();
  puVar2 = PTR_DAT_8cecfcb0;
  puVar1 = PTR_DAT_8cecfcac;
  *(undefined4 *)PTR_DAT_8cecfca8 = uVar4;
  *(undefined4 *)puVar1 = 1;
  piVar3 = DAT_8cecfcb4;
  *(int *)puVar2 = *(int *)puVar2 + 1;
  *piVar3 = *piVar3 + 1;
  bVar9 = extraout_M;
  bVar10 = extraout_Q;
  bVar12 = extraout_S;
  if (*(int *)PTR_DAT_8cecfcb8 == 1) {
    (*(code *)PTR_callback_candidate_8ced478e_8cecfcbc)();
    bVar9 = extraout_M_00;
    bVar10 = extraout_Q_00;
    bVar12 = extraout_S_00;
  }
  bVar13 = 0;
  if ((*(int *)PTR_DAT_8cecfcc0 == 1) && (bVar13 = 0, *(int *)PTR_DAT_8cecfcc4 <= *(int *)puVar2)) {
    (*DAT_8cecfccc)(*DAT_8cecfcc8);
    bVar9 = extraout_M_01;
    bVar10 = extraout_Q_01;
    bVar12 = extraout_S_01;
    bVar13 = extraout_T;
  }
  return ((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4) & 0xf0 |
         ((uint)bVar13 | (uint)bVar12 << 1 | (uint)bVar11 << 4 | (uint)bVar10 << 8 |
          (uint)bVar9 << 9 | (uint)bVar8 << 0xf | (uint)bVar7 << 0x1c | (uint)bVar6 << 0x1d |
         (uint)bVar5 << 0x1e) & (int)DAT_8cecfc9e;
}


/* Address: 8cecfce4 */

void callback_candidate_8cecfce4(undefined4 *param_1)

{
  undefined *puVar1;
  code *pcVar2;
  int iVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  
  (*(code *)PTR_callback_candidate_8ced5614_8cecfdac)();
  (*DAT_8cecfdb0)();
  (*(code *)PTR_FUN_8cecfdb4)();
  (*(code *)PTR_callback_candidate_8ced5712_8cecfdb8)();
  (*DAT_8cecfdbc)();
  (*DAT_8cecfdc0)();
  (*(code *)PTR_FUN_8cecfdc4)();
  iVar3 = (*(code *)PTR_FUN_8cecfdc8)();
  if (iVar3 != 0) {
    FUN_8cecf512(0xffffff9b);
    (*(code *)PTR_callback_candidate_8ced5374_8cecfdd0)
              (PTR_s_E2005_mwPlyInitSofdec__can_t_ini_8cecfdcc);
  }
  (*DAT_8cecfdd8)(DAT_8cecfdd4,0,(int)DAT_8cecfda6);
  (*(code *)(DAT_8cecfde0 | DAT_8cecfddc))((int)DAT_8cecfda8);
  if (*(int *)PTR_DAT_8cecfde4 == 1) {
    (*DAT_8cecfdec)(DAT_8cecfde8,5);
    uVar4 = (*DAT_8cecfdf0)(0);
    pcVar2 = DAT_8cecfe00;
    uVar5 = DAT_8cecfdfc;
    puVar1 = PTR_DAT_8cecfdf8;
    iVar3 = (int)DAT_8cecfdaa;
    *DAT_8cecfdf4 = uVar4;
    uVar5 = (*pcVar2)(uVar5,iVar3,puVar1,0);
    pcVar2 = DAT_8cecfe0c;
    puVar1 = PTR_LAB_8cecfe08;
    *DAT_8cecfe04 = uVar5;
    (*pcVar2)(puVar1,0);
  }
  if (param_1 != (undefined4 *)0x0) {
    FUN_8cecf096(*param_1,param_1[1],param_1[2],param_1[3]);
  }
  *DAT_8cecfe10 = 0;
  (*(code *)PTR_callback_candidate_8ced53c2_8cecfe18)(PTR_LAB_8cecfe14,0);
  (*(code *)PTR_callback_candidate_8ced5494_8cecfe20)(PTR_callback_candidate_8cecfbdc_8cecfe1c,0);
  (*(code *)PTR_callback_candidate_8ced5556_8cecfe28)(PTR_LAB_8cecfe24,0);
  return;
}


/* Address: 8cecfe2c */

void callback_candidate_8cecfe2c(void)

{
  undefined *puVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  undefined4 uVar6;
  
  iVar2 = DAT_8cecff18;
  puVar1 = PTR_callback_candidate_8ced19c6_8cecff14;
  uVar5 = 0;
  uVar6 = 0;
  iVar3 = (int)DAT_8cecff0c;
  uVar4 = (uint)DAT_8cecff0e;
  do {
    if (*(int *)(iVar2 + uVar5 + 8) == 1) {
      (*(code *)puVar1)(uVar6);
    }
    uVar5 = uVar5 + iVar3;
  } while (uVar5 < uVar4);
  if (*(int *)PTR_DAT_8cecff1c == 1) {
    (*DAT_8cecff20)(0);
    (*DAT_8cecff28)(*DAT_8cecff24);
  }
  (*(code *)PTR_callback_candidate_8ced55a4_8cecff30)(PTR_LAB_8cecff2c);
  (*(code *)PTR_callback_candidate_8ced54e2_8cecff38)(PTR_callback_candidate_8cecfbdc_8cecff34);
  (*(code *)PTR_callback_candidate_8ced5410_8cecff40)(PTR_LAB_8cecff3c);
  (*DAT_8cecff44)();
  (*(code *)PTR_FUN_8cecff48)();
  (*(code *)PTR_FUN_8cecff4c)();
  (*(code *)PTR_FUN_8cecff50)();
  (*DAT_8cecff54)();
  (*DAT_8cecff58)();
  (*(code *)(DAT_8cecff60 | DAT_8cecff5c))((int)DAT_8cecff10);
  (*(code *)PTR_FUN_8cecff64)();
  return;
}


/* Address: 8cecfec8 */

void callback_candidate_8cecfec8(int param_1,int *param_2,undefined4 *param_3)

{
  int iVar1;
  
  iVar1 = (*(code *)PTR_callback_candidate_8ced4846_8cecff68)(*(undefined4 *)(param_1 + 0x30));
  if (iVar1 != 0) {
    FUN_8cecf512((int)DAT_8cecff12);
    (*(code *)PTR_callback_candidate_8ced5374_8cecff70)
              (PTR_s_E2006_mwSfdGetTime__can_t_get_ti_8cecff6c);
    *param_2 = 0;
    *param_3 = 1;
  }
  if (*param_2 < 0) {
    *param_2 = 0;
    *param_3 = 1;
  }
  return;
}


/* Address: 8cecff74 */

void callback_candidate_8cecff74(int param_1,int param_2)

{
  int iVar1;
  undefined4 uVar2;
  undefined *puVar3;
  
  uVar2 = *(undefined4 *)(param_1 + 0x30);
  FUN_8cecfaae();
  iVar1 = (*(code *)PTR_callback_candidate_8ced379c_8ced0090)(uVar2,param_2);
  if (iVar1 != 0) {
    FUN_8cecf512((int)DAT_8ced008e);
    puVar3 = PTR_DAT_8ced0098;
    if (param_2 == 1) {
      puVar3 = PTR_DAT_8ced0094;
    }
    (*(code *)PTR_callback_candidate_8ced5374_8ced00a0)
              (PTR_s_E2007_mwSfdPause__can_t_pause____8ced009c,puVar3);
  }
  return;
}


/* Address: 8cecffb8 */

void FUN_8cecffb8(int param_1,undefined4 param_2)

{
  (*(code *)PTR_callback_candidate_8ced2ec0_8ced00a4)(*(undefined4 *)(param_1 + 0x30),3,param_2);
  return;
}


/* Address: 8cecffd6 */

void callback_candidate_8cecffd6(undefined4 param_1,undefined4 param_2,int param_3)

{
  undefined8 in_xd4;
  char in_FPSCR_SZ;
  
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(param_3 + 0x68) = param_1;
    *(undefined4 *)(param_3 + 0x6c) = param_2;
  }
  else {
    *(ulonglong *)(param_3 + 0x68) = CONCAT44(param_1,param_2);
    *(undefined8 *)(param_3 + 0x6c) = in_xd4;
  }
  *(undefined4 *)(param_3 + 100) = 1;
  if (*(int *)(param_3 + 0x40) == 0) {
    return;
  }
  (*(code *)PTR_FUN_8ced00a8)(*(undefined4 *)(param_3 + 0x40));
  return;
}


/* Address: 8cecfffc */

void callback_candidate_8cecfffc(undefined4 param_1,undefined4 param_2,int param_3)

{
  undefined8 in_xd4;
  char in_FPSCR_SZ;
  
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(param_3 + 0x74) = param_1;
    *(undefined4 *)(param_3 + 0x78) = param_2;
  }
  else {
    *(ulonglong *)(param_3 + 0x74) = CONCAT44(param_1,param_2);
    *(undefined8 *)(param_3 + 0x78) = in_xd4;
  }
  *(undefined4 *)(param_3 + 100) = 1;
  if (*(int *)(param_3 + 0x40) == 0) {
    return;
  }
  (*(code *)PTR_FUN_8ced00ac)(*(undefined4 *)(param_3 + 0x40));
  return;
}


/* Address: 8ced0022 */

void callback_candidate_8ced0022(int param_1,undefined4 param_2)

{
  *(undefined4 *)(param_1 + 0x5c) = param_2;
  if (*(int *)(param_1 + 0x40) != 0) {
    (*(code *)PTR_FUN_8ced00b0)(*(undefined4 *)(param_1 + 0x40));
    return;
  }
  return;
}


/* Address: 8ced00d6 */

undefined4 callback_candidate_8ced00d6(int param_1)

{
  undefined4 uVar1;
  
  uVar1 = 0;
  if (*(int *)(param_1 + 0x3c) != 0) {
    uVar1 = (*DAT_8ced019c)(*(undefined4 *)(param_1 + 0x3c));
  }
  return uVar1;
}


/* Address: 8ced0112 */

undefined4 callback_candidate_8ced0112(int param_1)

{
  undefined4 uVar1;
  
  uVar1 = 0;
  if (*(int *)(param_1 + 0x3c) != 0) {
    uVar1 = (*DAT_8ced01a4)(*(undefined4 *)(param_1 + 0x3c));
  }
  return uVar1;
}


/* Address: 8ced01bc */

void callback_candidate_8ced01bc
               (int param_1,int param_2,int *param_3,undefined4 *param_4,int *param_5,
               undefined4 *param_6,undefined4 *param_7,undefined4 *param_8)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  
  if (param_2 < 0) {
    param_2 = param_2 + 7;
  }
  if (param_1 == 2) {
    *param_3 = param_2 >> 3;
    *param_4 = 0;
    *param_5 = param_2 >> 3;
    *param_6 = 0;
    *param_7 = 0;
  }
  else {
    iVar3 = (*DAT_8ced0320)((int)DAT_8ced031e);
    uVar2 = DAT_8ced032c;
    uVar1 = DAT_8ced0324;
    *param_3 = iVar3 << 0xb;
    *param_4 = uVar1;
    *param_5 = *param_3 + DAT_8ced0328;
    *param_6 = uVar2;
    *param_7 = uVar2;
  }
  *param_8 = DAT_8ced0330;
  return;
}


/* Address: 8ced0222 */

/* WARNING: Removing unreachable block (ram,0x8ced0238) */

void FUN_8ced0222(int param_1,int param_2,int *param_3,int *param_4,int *param_5,int *param_6)

{
  int iVar1;
  int iVar2;
  
  iVar1 = (int)(param_1 + (uint)(param_1 < 0)) >> 1;
  iVar2 = iVar1 + 0x1f;
  if (iVar2 < 0) {
    iVar2 = iVar1 + 0x3e;
  }
  *param_5 = (iVar2 >> 5) << 5;
  *param_6 = (int)(param_2 + (uint)(param_2 < 0)) >> 1;
  *param_3 = param_1;
  *param_4 = param_2;
  return;
}


/* Address: 8ced0256 */

void FUN_8ced0256(int param_1,int *param_2,int *param_3)

{
  int iVar1;
  int local_1c;
  int local_18;
  int local_14;
  int local_10;
  int local_c;
  int *local_8;
  
  local_c = param_1;
  local_8 = param_2;
  FUN_8ced0222(&local_1c,&local_18,&local_14,&local_10);
  *local_8 = (local_14 * local_10 * 2 + local_1c * local_18) * 2 + 0x20;
  iVar1 = local_1c * local_18 * 3;
  *param_3 = ((int)(iVar1 + (uint)(iVar1 < 0)) >> 1) * local_c + 0x20;
  return;
}


/* Address: 8ced02ba */

int callback_candidate_8ced02ba(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  int local_2c;
  int local_28;
  int local_24;
  int local_20;
  int local_1c;
  int local_18;
  int local_14;
  int local_10;
  undefined4 local_c;
  undefined4 local_8;
  
  local_c = param_1;
  local_8 = param_2;
  callback_candidate_8ced01bc(&local_24,&local_20,&local_1c,&local_18,&local_14,&local_10);
  FUN_8ced0256(local_c,local_8,param_3,&local_2c,&local_28);
  return local_24 + local_20 + local_1c + local_18 + local_2c + local_28 + local_14 + local_10 +
         0x20;
}


/* Address: 8ced0350 */

undefined4 callback_candidate_8ced0350(int param_1,undefined4 param_2)

{
  undefined *puVar1;
  undefined4 uVar2;
  int iVar3;
  undefined1 auStack_18 [12];
  undefined4 local_c;
  
  puVar1 = PTR_FUN_8ced03d8;
  if (param_1 == 1) {
    (*DAT_8ced03e0)();
  }
  else {
    if (param_1 != 2) {
      iVar3 = (int)DAT_8ced03d2;
      goto LAB_8ced03ae;
    }
    (*DAT_8ced03e0)();
  }
  local_c = param_2;
  iVar3 = (*(code *)PTR_callback_candidate_8ced18e4_8ced03e8)(auStack_18);
  if (iVar3 == 0) {
    iVar3 = (*(code *)PTR_callback_candidate_8ced1afa_8ced03f0)(0,PTR_LAB_8ced03ec);
    if (iVar3 == 0) {
      return 0;
    }
    iVar3 = (int)DAT_8ced03d6;
  }
  else {
    iVar3 = (int)DAT_8ced03d4;
  }
LAB_8ced03ae:
  uVar2 = (*(code *)puVar1)(iVar3);
  return uVar2;
}


/* Address: 8ced03c4 */

undefined4 callback_candidate_8ced03c4(void)

{
  (*(code *)PTR_callback_candidate_8ced19c6_8ced03f4)();
  return 0;
}


/* Address: 8ced03f8 */

/* WARNING: Type propagation algorithm not settling */

int callback_candidate_8ced03f8
              (int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int *piVar1;
  int *piVar2;
  undefined *puVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  undefined4 uVar7;
  int local_7c [4];
  int local_6c;
  int local_68;
  int local_64;
  int local_60;
  int local_5c;
  int local_58;
  undefined1 auStack_54 [4];
  uint local_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  
  local_6c = param_1;
  local_68 = param_2;
  uVar7 = *(undefined4 *)((int)local_7c + (int)DAT_8ced0530);
  callback_candidate_8ced01bc
            (param_1,param_2,DAT_8ced054c,DAT_8ced0548,DAT_8ced0544,DAT_8ced0540,DAT_8ced053c,
             DAT_8ced0538);
  FUN_8ced0256(param_3,param_4,uVar7,DAT_8ced0554,DAT_8ced0550);
  piVar2 = DAT_8ced055c;
  piVar1 = DAT_8ced0558;
  iVar4 = (int)DAT_8ced0532;
  *DAT_8ced0558 = *DAT_8ced0548 + *DAT_8ced0544 + *DAT_8ced0540;
  uVar5 = *(int *)((int)local_7c + iVar4) + 0x1fU & 0xffffffe0;
  *piVar2 = uVar5 - *(int *)((int)local_7c + iVar4);
  local_60 = uVar5 + *piVar1;
  local_64 = uVar5 + *piVar1 + *DAT_8ced054c;
  local_68 = local_64 + *DAT_8ced0554;
  local_58 = local_68 + *DAT_8ced0550;
  iVar4 = (int)DAT_8ced0534;
  local_5c = local_58 + *DAT_8ced053c;
  iVar6 = ((local_5c + *DAT_8ced0538) - uVar5) + *piVar2;
  *(int *)PTR_DAT_8ced0560 = iVar6;
  if (*(int *)((int)local_7c + iVar4) < iVar6) {
    (*(code *)PTR_callback_candidate_8ced5374_8ced0568)
              (PTR_s_E2011__not_enough_work___d_byte__8ced0564,
               iVar6 - *(int *)((int)local_7c + (int)DAT_8ced0534));
  }
  else {
    FUN_8ced0222(param_3,param_4,local_7c,local_7c + 1,local_7c + 2,local_7c + 3);
    piVar1 = DAT_8ced0574;
    puVar3 = PTR_DAT_8ced056c;
    *(int *)PTR_DAT_8ced056c = local_7c[2];
    *(int *)(puVar3 + 4) = local_7c[3];
    *(int *)(puVar3 + 8) = local_7c[0];
    *(int *)(puVar3 + 0xc) = local_7c[1];
    *(int *)(puVar3 + 0x10) = local_64;
    *(int *)(puVar3 + 0x14) = local_7c[0];
    *(int *)(puVar3 + 0x18) = local_7c[1];
    *(undefined4 *)(puVar3 + 0x1c) = uVar7;
    *(int *)(puVar3 + 0x20) = local_68;
    puVar3 = PTR_DAT_8ced0570;
    *(int *)(PTR_DAT_8ced0570 + 8) = local_58;
    *(int *)(puVar3 + 0x18) = local_5c;
    *piVar1 = local_60;
    if (local_6c == 1) {
      (*DAT_8ced057c)();
    }
    else if (local_6c == 2) {
      (*DAT_8ced06c0)();
    }
    local_4c = *DAT_8ced06c4;
    local_48 = *DAT_8ced06c8;
    local_44 = *DAT_8ced06cc;
    uStack_20 = param_4;
    local_50 = uVar5;
    uStack_28 = uVar7;
    uStack_24 = param_3;
    (*(code *)PTR_FUN_8ced06d4)(PTR_DAT_8ced06d0);
    (*(code *)PTR_FUN_8ced06dc)(PTR_DAT_8ced06d8);
    iVar4 = (*(code *)PTR_callback_candidate_8ced3dc2_8ced06e0)(auStack_54,0);
    if (iVar4 == 0) {
      (*(code *)PTR_FUN_8ced06e4)((int)DAT_8ced06b4);
    }
    else {
      iVar6 = (*(code *)PTR_callback_candidate_8ced1afa_8ced06ec)(iVar4,PTR_LAB_8ced06e8,0);
      if (iVar6 == 0) {
        return iVar4;
      }
      (*(code *)PTR_FUN_8ced06e4)((int)DAT_8ced06b6);
    }
  }
  return 0;
}


/* Address: 8ced0604 */

void callback_candidate_8ced0604(void)

{
  int iVar1;
  
  iVar1 = (*(code *)PTR_callback_candidate_8ced3ff0_8ced06f0)();
  if (iVar1 != 0) {
    (*(code *)PTR_FUN_8ced06e4)((int)DAT_8ced06b8);
    return;
  }
  return;
}


/* Address: 8ced0620 */

/* WARNING: Removing unreachable block (ram,0x8ced0666) */
/* WARNING: Removing unreachable block (ram,0x8ced0672) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void callback_candidate_8ced0620(undefined4 param_1,int param_2,int param_3,int param_4)

{
  undefined *puVar1;
  undefined4 uVar2;
  float fVar3;
  undefined *puVar4;
  double in_xd2;
  bool bVar5;
  int iVar6;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  puVar1 = PTR_callback_candidate_8ced436a_8ced06f4;
  uVar2 = *(undefined4 *)(param_2 + 0x30);
  (*(code *)PTR_callback_candidate_8ced436a_8ced06f4)(uVar2,8,0);
  (*(code *)puVar1)(uVar2,1);
  (*(code *)puVar1)(uVar2,0);
  (*(code *)puVar1)(uVar2,0x17,4);
  puVar4 = PTR_callback_candidate_8ced2ec0_8ced06fc;
  if (in_FPSCR_SZ == '\0') {
    puVar4 = _DAT_8ced06f8;
  }
  fVar3 = (float)(param_3 * param_4 * (int)DAT_8ced06ba);
  if (in_FPSCR_SZ == '\0') {
    in_xd2 = (double)CONCAT44(fVar3,param_1);
  }
  if (in_FPSCR_PR == '\0') {
    fVar3 = (float)((ulonglong)in_xd2 >> 0x20) + (float)puVar4;
    in_xd2 = (double)CONCAT44(fVar3,SUB84(in_xd2,0));
    iVar6 = (int)fVar3;
  }
  else {
    in_xd2 = in_xd2 + (double)CONCAT44(puVar4,fVar3);
    fVar3 = (float)((ulonglong)in_xd2 >> 0x20);
    iVar6 = (int)in_xd2;
  }
  if (in_FPSCR_PR == '\0') {
    bVar5 = fVar3 < (float)iVar6;
  }
  else {
    bVar5 = in_xd2 < (double)CONCAT44(puVar4,(float)iVar6);
  }
  if (bVar5) {
    iVar6 = iVar6 + -1;
  }
  (*(code *)puVar1)(uVar2,0x2d,iVar6);
  (*(code *)puVar1)(uVar2,0x2c,iVar6);
  (*(code *)puVar1)(uVar2,0x2a,iVar6);
  (*(code *)puVar1)(uVar2,0xe,1);
  (*(code *)PTR_callback_candidate_8ced2ec0_8ced06fc)(uVar2,5,0);
  return;
}


/* Address: 8ced0700 */

undefined4 callback_candidate_8ced0700(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = (*(code *)PTR_callback_candidate_8ced3858_8ced0824)(*(undefined4 *)(param_1 + 0x30));
  if (iVar1 != 0) {
    uVar2 = (*(code *)PTR_FUN_8ced0828)((int)DAT_8ced081c);
    return uVar2;
  }
  return 0;
}


/* Address: 8ced071c */

undefined4 callback_candidate_8ced071c(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = (*(code *)PTR_callback_candidate_8ced4032_8ced082c)(*(undefined4 *)(param_1 + 0x30));
  if (iVar1 != 0) {
    uVar2 = (*(code *)PTR_FUN_8ced0828)((int)DAT_8ced081e);
    return uVar2;
  }
  return 0;
}


/* Address: 8ced0754 */

void FUN_8ced0754(int param_1,undefined4 *param_2,undefined4 *param_3,undefined4 *param_4)

{
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_c;
  
  (*(code *)PTR_callback_candidate_8ced41fc_8ced0834)(*(undefined4 *)(param_1 + 0x30),&local_20);
  *param_2 = local_20;
  *param_3 = local_1c;
  *param_4 = local_c;
  return;
}


/* Address: 8ced0782 */

void FUN_8ced0782(int param_1,int param_2)

{
  if (-1 < param_2) {
    (*(code *)PTR_callback_candidate_8ced4232_8ced0838)(*(undefined4 *)(param_1 + 0x30));
    return;
  }
  return;
}


/* Address: 8ced07e0 */

undefined4 FUN_8ced07e0(void)

{
  int iVar1;
  
  iVar1 = (*(code *)PTR_callback_candidate_8ced4334_8ced0848)();
  if (iVar1 != 1) {
    return 1;
  }
  return 0;
}


/* Address: 8ced0854 */

void FUN_8ced0854(void)

{
                    /* WARNING: Could not recover jumptable at 0x8ced085e. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*DAT_8ced08fc)(*DAT_8ced08f8,*DAT_8ced08f4,0);
  return;
}


/* Address: 8ced0870 */

/* WARNING: Removing unreachable block (ram,0x8ced089a) */

void callback_candidate_8ced0870(int param_1)

{
  int *piVar1;
  int local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  
  piVar1 = *(int **)(param_1 + 0x38);
  if (*(int *)(param_1 + 0x30) != 0) {
    FUN_8ced0754(param_1,&local_1c,&local_20,&local_18);
    if (local_20 < 0) {
      local_20 = local_20 + 0x1f;
    }
    local_20 = (local_20 >> 5) << 5;
    if (0 < local_20) {
      (**(code **)(*piVar1 + 0x18))(piVar1,1,local_20,&local_14);
      (*(code *)PTR_callback_candidate_8ced5240_8ced0900)(local_1c,local_14,local_10);
      (**(code **)(*piVar1 + 0x20))(piVar1,0,&local_14);
      FUN_8ced0782(param_1,local_10,local_18);
    }
  }
  return;
}


/* Address: 8ced0908 */

void callback_candidate_8ced55da(void)

{
  (*DAT_8ced0928)();
  return;
}


/* Address: 8ced0914 */

void callback_candidate_8ced5464(void)

{
  (*(code *)PTR_callback_candidate_8ced5464_8ced0930)();
  return;
}


/* Address: 8ced0940 */

undefined4 FUN_8ced0940(void)

{
  return 0;
}


/* Address: 8ced0948 */

undefined4 FUN_8ced0948(void)

{
  return 0;
}


/* Address: 8ced094c */

void thunk_EXT_FUN_8c65bdd2(void)

{
                    /* WARNING: Could not recover jumptable at 0x8ced094e. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*DAT_8ced09c4)();
  return;
}


/* Address: 8ced0952 */

undefined4 callback_candidate_8ced0952(void)

{
  (*DAT_8ced09c8)();
  return 0;
}


/* Address: 8ced0960 */

undefined4 callback_candidate_8ced0960(undefined4 param_1)

{
  (*DAT_8ced09cc)(param_1);
  return 0;
}


/* Address: 8ced0972 */

undefined4 FUN_8ced0972(void)

{
  return 0;
}


/* Address: 8ced0976 */

undefined4 callback_candidate_8ced0976(void)

{
  (*DAT_8ced09d0)();
  return 0;
}


/* Address: 8ced09ac */

bool callback_candidate_8ced09ac(void)

{
  int iVar1;
  
  iVar1 = (*DAT_8ced09d8)();
  return iVar1 == 4;
}


/* Address: 8ced09be */

void thunk_EXT_FUN_8c65c19e(void)

{
                    /* WARNING: Could not recover jumptable at 0x8ced09c0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*DAT_8ced09dc)();
  return;
}


/* Address: 8ced09e0 */

void FUN_8ced09e0(void)

{
  *DAT_8ced09f4 = *DAT_8ced09f4 + 1;
  return;
}


/* Address: 8ced09f8 */

uint FUN_8ced09f8(void)

{
  byte in_IMASK;
  byte in_S;
  byte in_T;
  
  return (((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4) & 0xf0) >> 4;
}


/* Address: 8ced0a10 */

uint FUN_8ced0a10(uint param_1)

{
  byte in_MD;
  byte in_RB;
  byte in_BL;
  byte in_FD;
  byte in_M;
  byte in_Q;
  byte in_IMASK;
  byte in_S;
  byte in_T;
  
  return (param_1 & 0xf) << 4 |
         ((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4 | (uint)in_Q << 8 | (uint)in_M << 9 |
          (uint)in_FD << 0xf | (uint)in_BL << 0x1c | (uint)in_RB << 0x1d | (uint)in_MD << 0x1e) &
         (int)DAT_8ced0afa;
}


/* Address: 8ced0a2c */

void FUN_8ced0a2c(void)

{
  code *UNRECOVERED_JUMPTABLE;
  int iVar1;
  
  UNRECOVERED_JUMPTABLE = DAT_8ced0b08;
  if (*DAT_8ced0b00 < 1) {
    iVar1 = (int)DAT_8ced0afc;
    *DAT_8ced0b00 = *DAT_8ced0b00 + 1;
                    /* WARNING: Could not recover jumptable at 0x8ced0a40. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*UNRECOVERED_JUMPTABLE)(DAT_8ced0b04,0,iVar1);
    return;
  }
  return;
}


/* Address: 8ced0a48 */

void FUN_8ced0a48(void)

{
  *DAT_8ced0b00 = *DAT_8ced0b00 + -1;
  return;
}


/* Address: 8ced0ac6 */

void callback_candidate_8ced0ac6(int param_1)

{
  if (param_1 != 0) {
    (*DAT_8ced0b08)(param_1,0,0x30);
    *(undefined1 *)(param_1 + 4) = 0;
  }
  return;
}


/* Address: 8ced0b60 */

int callback_candidate_8ced0b60(int param_1,int param_2)

{
  int *piVar1;
  int iVar2;
  
  if ((-1 < param_2) && (param_2 < 4)) {
    iVar2 = 0;
    piVar1 = (int *)(param_2 * 4 + param_1 + 0x18);
    while (piVar1 = (int *)*piVar1, piVar1 != (int *)0x0) {
      iVar2 = iVar2 + piVar1[3];
    }
    return iVar2;
  }
  if (*(int *)(param_1 + 0x28) != 0) {
    (**(code **)(param_1 + 0x28))(*(undefined4 *)(param_1 + 0x2c),0xfffffffd);
  }
  return 0;
}


/* Address: 8ced0ba8 */

void callback_candidate_8ced0ba8(int param_1,int param_2,int param_3,undefined4 *param_4)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  undefined1 auStack_24 [8];
  undefined1 auStack_1c [4];
  int local_18;
  
  if ((param_2 < 0) || (3 < param_2)) {
    if (*(int *)(param_1 + 0x28) != 0) {
      (**(code **)(param_1 + 0x28))(*(undefined4 *)(param_1 + 0x2c),0xfffffffd);
    }
    *param_4 = 0;
    param_4[1] = 0;
    return;
  }
  uVar1 = FUN_8ced09f8(param_1);
  param_2 = param_2 * 4;
  puVar2 = *(undefined4 **)(param_1 + 0x18 + param_2);
  if (puVar2 != (undefined4 *)0x0) {
    (*DAT_8ced0d70)();
    if (local_18 <= param_3) {
      (*DAT_8ced0d70)();
      *(undefined4 *)(param_2 + param_1 + 0x18) = *puVar2;
      *puVar2 = *(undefined4 *)(param_1 + 0x14);
      *(undefined4 **)(param_1 + 0x14) = puVar2;
      goto LAB_8ced0c60;
    }
    if (*(char *)(param_1 + 5) == '\x01') {
      (*DAT_8ced0d74)(auStack_1c,param_3,auStack_1c,auStack_24);
      (*DAT_8ced0d70)();
      (*DAT_8ced0d70)();
      goto LAB_8ced0c60;
    }
  }
  *param_4 = 0;
  param_4[1] = 0;
LAB_8ced0c60:
  FUN_8ced0a10(param_1,uVar1);
  return;
}


/* Address: 8ced0c74 */

void callback_candidate_8ced0c74(int param_1,int param_2,int *param_3)

{
  code *pcVar1;
  undefined4 uVar2;
  int *piVar3;
  int *piVar4;
  undefined4 *puVar5;
  
  if ((param_2 < 0) || (3 < param_2)) {
    if (*(int *)(param_1 + 0x28) != 0) {
                    /* WARNING: Could not recover jumptable at 0x8ced0ca2. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (**(code **)(param_1 + 0x28))(*(undefined4 *)(param_1 + 0x2c),0xfffffffd);
      return;
    }
  }
  else if ((0 < param_3[1]) && (*param_3 != 0)) {
    uVar2 = FUN_8ced09f8(param_1);
    piVar3 = (int *)(param_2 * 4 + param_1 + 0x18);
    piVar4 = (int *)0x0;
    while (*piVar3 != 0) {
      piVar3 = (int *)*piVar3;
      piVar4 = piVar3;
    }
    if (((*(char *)(param_1 + 5) == '\x01') && (piVar4 != (int *)0x0)) &&
       (piVar4[2] + piVar4[3] == *param_3)) {
      piVar4[3] = piVar4[3] + param_3[1];
    }
    else {
      puVar5 = *(undefined4 **)(param_1 + 0x14);
      if (puVar5 == (undefined4 *)0x0) {
        if (*(int *)(param_1 + 0x28) != 0) {
          (**(code **)(param_1 + 0x28))(*(undefined4 *)(param_1 + 0x2c),0xfffffffd);
        }
      }
      else {
        *(undefined4 *)(param_1 + 0x14) = *puVar5;
        pcVar1 = DAT_8ced0d70;
        *puVar5 = 0;
        (*pcVar1)();
        *piVar3 = (int)puVar5;
      }
    }
    FUN_8ced0a10(param_1,uVar2);
    return;
  }
  return;
}


/* Address: 8ced0d3c */

void callback_candidate_8ced0d3c(int param_1,int param_2,int *param_3)

{
  code *pcVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 *puVar4;
  
  if ((param_2 < 0) || (3 < param_2)) {
    if (*(int *)(param_1 + 0x28) != 0) {
                    /* WARNING: Could not recover jumptable at 0x8ced0d6a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (**(code **)(param_1 + 0x28))(*(undefined4 *)(param_1 + 0x2c),0xfffffffd);
      return;
    }
  }
  else if ((0 < param_3[1]) && (*param_3 != 0)) {
    uVar2 = FUN_8ced09f8(param_1);
    param_2 = param_2 * 4;
    iVar3 = *(int *)(param_2 + param_1 + 0x18);
    if (((*(char *)(param_1 + 5) == '\x01') && (iVar3 != 0)) &&
       (*param_3 + param_3[1] == *(int *)(iVar3 + 8))) {
      *(int *)(iVar3 + 8) = *param_3;
      *(int *)(iVar3 + 0xc) = *(int *)(iVar3 + 0xc) + param_3[1];
    }
    else {
      puVar4 = *(undefined4 **)(param_1 + 0x14);
      if (puVar4 == (undefined4 *)0x0) {
        if (*(int *)(param_1 + 0x28) != 0) {
          (**(code **)(param_1 + 0x28))(*(undefined4 *)(param_1 + 0x2c),0xfffffffd);
        }
      }
      else {
        *(undefined4 *)(param_1 + 0x14) = *puVar4;
        pcVar1 = DAT_8ced0ec0;
        *puVar4 = 0;
        (*pcVar1)();
        *puVar4 = *(undefined4 *)(param_1 + 0x18 + param_2);
        *(undefined4 **)(param_1 + 0x18 + param_2) = puVar4;
      }
    }
    FUN_8ced0a10(param_1,uVar2);
    return;
  }
  return;
}


/* Address: 8ced0e1a */

undefined4 callback_candidate_8ced0e1a(int param_1,int param_2,int param_3,undefined4 *param_4)

{
  undefined1 *puVar1;
  undefined1 auStack_10 [12];
  
  puVar1 = auStack_10;
  *param_4 = 0;
  if ((param_2 < 0) || (3 < param_2)) {
    if (*(int *)(param_1 + 0x28) != 0) {
      (**(code **)(param_1 + 0x28))(*(undefined4 *)(param_1 + 0x2c),0xfffffffd);
    }
  }
  else if (*(int *)(param_2 * 4 + param_1 + 0x18) != 0) {
    (*DAT_8ced0ec0)();
    *param_4 = *(undefined4 *)(puVar1 + 4);
    if (*(char *)(param_1 + 5) == '\x01') {
      if (param_3 <= *(int *)(puVar1 + 4)) {
        return 1;
      }
    }
    else if (*(int *)(puVar1 + 4) == param_3) {
      return 1;
    }
  }
  return 0;
}


/* Address: 8ced0ec4 */

void FUN_8ced0ec4(undefined4 *param_1)

{
  undefined4 *puVar1;
  
  puVar1 = DAT_8ced0fdc;
  *DAT_8ced0fdc = *param_1;
  puVar1[1] = param_1[1];
  puVar1[2] = param_1[2] + 0x1f & 0xffffffe0;
  puVar1[3] = param_1[3];
  puVar1[4] = param_1[4];
  puVar1[5] = param_1[5];
  puVar1[6] = param_1[6] + 0x1f & 0xffffffe0;
  return;
}


/* Address: 8ced0eee */

undefined4 callback_candidate_8ced0eee(void)

{
  (*(code *)PTR_FUN_8ced0fe0)(DAT_8ced0fdc,0,7);
  return 0;
}


/* Address: 8ced0f04 */

undefined4 callback_candidate_8ced0f04(undefined4 param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = (*(code *)PTR_FUN_8ced0fe4)(6);
  if ((iVar1 != 0) && (iVar1 = FUN_8ced12f2(param_1), iVar1 != 1)) {
    uVar2 = FUN_8ced0f40(param_1);
    FUN_8ced102c(param_1);
    FUN_8ced109e(param_1);
    return uVar2;
  }
  return 0;
}


/* Address: 8ced0f40 */

int FUN_8ced0f40(undefined4 param_1)

{
  int iVar1;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c [2];
  
  iVar1 = FUN_8ced1158(&local_10,local_c);
  if (iVar1 == 0) {
    FUN_8ced0f76(param_1,local_10,local_c[0],&local_14);
    iVar1 = FUN_8ced12a6(param_1,local_14);
  }
  return iVar1;
}


/* Address: 8ced0f76 */

void FUN_8ced0f76(int param_1,undefined4 param_2,int param_3,int *param_4)

{
  code *pcVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  undefined4 auStack_1c [2];
  undefined4 local_14;
  int local_10;
  
  iVar2 = param_1 + DAT_8ced0fd8 + (int)DAT_8ced0fd6;
  piVar4 = *(int **)(iVar2 + 0xc);
  (**(code **)(*piVar4 + 0x18))(piVar4,0,*(undefined4 *)(iVar2 + 0x10),&local_14);
  if (param_3 < local_10) {
    local_10 = param_3;
  }
  iVar2 = DAT_8ced0fe8;
  if (local_10 < DAT_8ced0fe8) {
    iVar2 = local_10;
  }
  (*(code *)PTR_FUN_8ced0fec)(local_14,param_2,iVar2);
  if (iVar2 == 0) {
    pcVar1 = *(code **)(*piVar4 + 0x1c);
    puVar3 = &local_14;
  }
  else {
    (*DAT_8ced1104)(&local_14,iVar2,&local_14,auStack_1c);
    (**(code **)(*piVar4 + 0x20))(piVar4,1,&local_14);
    puVar3 = auStack_1c;
    pcVar1 = *(code **)(*piVar4 + 0x1c);
  }
  (*pcVar1)(piVar4,0,puVar3);
  *param_4 = iVar2;
  return;
}


/* Address: 8ced102c */

void FUN_8ced102c(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 uVar3;
  
  iVar2 = DAT_8ced10fc + param_1 + (int)DAT_8ced10fa;
  uVar1 = *(undefined4 *)(DAT_8ced10fe + iVar2);
  uVar3 = *(undefined4 *)(DAT_8ced1100 + iVar2);
  iVar2 = (*(code *)PTR_FUN_8ced1108)(param_1,uVar3);
  if (((iVar2 != 1) && (iVar2 = (*(code *)PTR_FUN_8ced1108)(param_1,uVar1), iVar2 == 1)) &&
     (iVar2 = FUN_8ced1084(param_1), iVar2 != 0)) {
    (*(code *)PTR_FUN_8ced110c)(param_1,uVar3,1);
    return;
  }
  return;
}


/* Address: 8ced1084 */

bool FUN_8ced1084(int param_1)

{
  int iVar1;
  
  iVar1 = (*DAT_8ced1110)(*(undefined4 *)(param_1 + DAT_8ced10fc + (int)DAT_8ced10fa + 8));
  return iVar1 == 3;
}


/* Address: 8ced109e */

void FUN_8ced109e(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  
  uVar4 = *(undefined4 *)(DAT_8ced10fc + param_1 + (int)DAT_8ced10fa + 8);
  iVar1 = (*DAT_8ced1110)(uVar4);
  iVar2 = (*DAT_8ced1114)(uVar4);
  if (iVar2 != 0) {
    *DAT_8ced1118 = iVar2;
  }
  iVar3 = (*(code *)PTR_FUN_8ced111c)(param_1,0x1a);
  if (iVar3 == 0) {
    iVar2 = 0;
  }
  if (iVar2 != 0) {
    uVar4 = DAT_8ced1124;
    if ((iVar2 != -1) && (uVar4 = DAT_8ced1128, iVar2 != -2)) {
      uVar4 = DAT_8ced1254;
    }
    (*(code *)PTR_FUN_8ced1120)(param_1,uVar4);
  }
  if ((iVar1 != 5) && (iVar2 == 0)) {
    return;
  }
  FUN_8ced130a(param_1,1);
  return;
}


/* Address: 8ced1158 */

int FUN_8ced1158(int param_1,int *param_2,int *param_3)

{
  int iVar1;
  undefined4 uVar2;
  int local_44;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  int *local_30;
  int *local_2c;
  int local_28;
  int local_24;
  
  uVar2 = *(undefined4 *)((int)DAT_8ced1250 + DAT_8ced124e + param_1 + (int)DAT_8ced124c);
  local_30 = param_3;
  local_2c = param_2;
  iVar1 = (*(code *)PTR_callback_candidate_8ced9ba4_8ced1258)(param_1,uVar2,&local_28);
  if (iVar1 == 0) {
    (*(code *)PTR_callback_candidate_8ced9d2e_8ced125c)
              (param_1,uVar2,&local_3c,&local_38,&local_40,&local_44,&local_34);
    iVar1 = FUN_8ced1210(param_1,&local_28,local_34,&local_40,&local_44);
    if (iVar1 == 0) {
      if (local_44 == 0) {
        *local_30 = local_24;
      }
      else {
        *local_30 = local_44 - local_28;
      }
      *local_2c = local_28;
      (*(code *)PTR_callback_candidate_8ced9d7e_8ced1264)
                (param_1,uVar2,local_3c,local_38,local_40,local_44);
      iVar1 = 0;
    }
    else {
      iVar1 = (*(code *)PTR_FUN_8ced1260)(param_1);
    }
  }
  return iVar1;
}


/* Address: 8ced1210 */

undefined4 FUN_8ced1210(undefined4 param_1,int *param_2,int param_3,int *param_4,int *param_5)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  if (*param_5 == 0) {
    if (param_2[3] != 0) {
      iVar1 = *param_4;
      iVar2 = param_2[3] - iVar1;
      iVar3 = param_3 - iVar1;
      if (iVar2 < iVar3) {
        iVar3 = iVar2;
      }
      iVar2 = param_2[1] + iVar1 + *param_2;
      (*(code *)PTR_FUN_8ced138c)(iVar2,iVar1 + param_2[2],iVar3);
      iVar1 = *param_4;
      *param_4 = iVar1 + iVar3;
      if ((iVar1 + iVar3 == param_3) || (iVar1 = FUN_8ced12da(param_1), iVar1 == 1)) {
        *param_5 = iVar3 + iVar2;
      }
    }
  }
  return 0;
}


/* Address: 8ced12a6 */

int FUN_8ced12a6(int param_1,undefined4 param_2)

{
  int iVar1;
  undefined4 uVar2;
  
  uVar2 = *(undefined4 *)((int)DAT_8ced1386 + DAT_8ced1384 + param_1 + (int)DAT_8ced1382);
  iVar1 = (*(code *)PTR_callback_candidate_8ced9c48_8ced1390)(uVar2,param_2,uVar2);
  if (iVar1 != 0) {
    return iVar1;
  }
  return 0;
}


/* Address: 8ced12da */

void FUN_8ced12da(int param_1)

{
  (*(code *)PTR_FUN_8ced1394)
            (*(undefined4 *)((int)DAT_8ced1386 + DAT_8ced1384 + param_1 + (int)DAT_8ced1382));
  return;
}


/* Address: 8ced12f2 */

void FUN_8ced12f2(int param_1)

{
  (*(code *)PTR_FUN_8ced1394)
            (*(undefined4 *)((int)DAT_8ced1388 + DAT_8ced1384 + param_1 + (int)DAT_8ced1382));
  return;
}


/* Address: 8ced130a */

void FUN_8ced130a(int param_1,undefined4 param_2)

{
  (*(code *)PTR_FUN_8ced1398)
            (*(undefined4 *)((int)DAT_8ced1388 + DAT_8ced1384 + param_1 + (int)DAT_8ced1382),param_2
            );
  return;
}


/* Address: 8ced1326 */

int callback_candidate_8ced1326(int param_1)

{
  code *pcVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  iVar2 = (*(code *)PTR_FUN_8ced139c)(6);
  if (iVar2 != 0) {
    iVar6 = DAT_8ced1384 + param_1 + (int)DAT_8ced1382;
    iVar2 = FUN_8ced13fc(param_1,(int *)(iVar6 + 8));
    if (iVar2 != 0) {
      return iVar2;
    }
    uVar3 = (*(code *)PTR_FUN_8ced139c)(param_1,0x1b);
    iVar2 = FUN_8ced1438((undefined4 *)(iVar6 + 0x10),uVar3);
    if (iVar2 == 0) {
      iVar2 = (*(code *)PTR_FUN_8ced13a4)(param_1,DAT_8ced13a0);
      return iVar2;
    }
    iVar4 = (*DAT_8ced1494)(*(undefined4 *)(iVar6 + 0x18),*(undefined4 *)(iVar6 + 0x10),
                            *(undefined4 *)(iVar6 + 0x14));
    pcVar1 = DAT_8ced14a0;
    if (iVar4 == 0) {
      iVar2 = (*(code *)PTR_FUN_8ced149c)(param_1,DAT_8ced1498);
      return iVar2;
    }
    iVar5 = (int)DAT_8ced1492;
    *(int *)(iVar6 + 8) = iVar2;
    *(int *)(iVar6 + 0xc) = iVar4;
    *(undefined4 *)(iVar5 * 2 + param_1 + -0x74) = *(undefined4 *)(iVar2 + 0xc);
    (*pcVar1)(iVar2);
    FUN_8ced161a(param_1,1);
    FUN_8ced1476(param_1);
  }
  return 0;
}


/* Address: 8ced13fc */

undefined4 FUN_8ced13fc(undefined4 *param_1)

{
  undefined4 uVar1;
  
  if ((*(int *)(DAT_8ced14a4 + 8) != 0) && (*(int *)(DAT_8ced14a4 + 0x18) != 0)) {
    (*DAT_8ced14ac)();
    *param_1 = 0;
    param_1[1] = 0;
    param_1[9] = 0xffffffff;
    param_1[10] = 1;
    param_1[0xb] = 0;
    param_1[0xc] = 0;
    return 0;
  }
  uVar1 = (*(code *)PTR_FUN_8ced149c)(DAT_8ced14a8);
  return uVar1;
}


/* Address: 8ced1438 */

int FUN_8ced1438(int param_1,undefined4 param_2)

{
  int iVar1;
  
  iVar1 = (*DAT_8ced14b0)(*(undefined4 *)(param_1 + 0xc),*(undefined4 *)(param_1 + 0x18),
                          *(undefined4 *)(param_1 + 0x14));
  if (iVar1 == 0) {
    return 0;
  }
  (*DAT_8ced14b4)(iVar1,param_2);
  (*DAT_8ced14b8)(iVar1,0);
  return iVar1;
}


/* Address: 8ced1476 */

void FUN_8ced1476(undefined4 param_1)

{
  (*(code *)PTR_callback_candidate_8ced43f8_8ced14bc)(0xf,2);
  (*(code *)PTR_FUN_8ced14c4)(param_1,PTR_callback_candidate_8ced14c8_8ced14c0,2);
  return;
}


/* Address: 8ced14c8 */

undefined4 callback_candidate_8ced14c8(int param_1,undefined4 *param_2,undefined4 *param_3)

{
  int iVar1;
  int iVar2;
  int local_20;
  undefined4 local_1c;
  undefined4 *local_18;
  undefined4 *local_14;
  undefined4 local_10;
  
  iVar2 = DAT_8ced15de + param_1 + (int)DAT_8ced15dc;
  local_10 = *(undefined4 *)(iVar2 + 8);
  local_18 = param_2;
  local_14 = param_3;
  iVar1 = (*(code *)PTR_FUN_8ced15e0)(param_1,param_2,param_3);
  if (iVar1 != 0) {
    if ((*(int *)(param_1 + 0x40) == 4) &&
       ((*DAT_8ced15e4)(local_10,&local_20,&local_1c), *(int *)(iVar2 + 0x2c) < local_20)) {
      *(int *)(iVar2 + 0x2c) = local_20;
      *(undefined4 *)(iVar2 + 0x30) = local_1c;
    }
    *local_18 = *(undefined4 *)(iVar2 + 0x2c);
    *local_14 = *(undefined4 *)(iVar2 + 0x30);
  }
  return 0;
}


/* Address: 8ced1560 */

undefined4 FUN_8ced1560(void)

{
  (*DAT_8ced15e8)();
  return 0;
}


/* Address: 8ced159e */

undefined4 callback_candidate_8ced159e(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int local_18;
  undefined4 local_14;
  undefined4 local_10;
  
  iVar2 = DAT_8ced15de + param_1 + (int)DAT_8ced15dc;
  local_10 = *(undefined4 *)(iVar2 + 8);
  if (param_2 == 0) {
    *(undefined4 *)(iVar2 + 0x38) = 0;
    FUN_8ced1642(0);
  }
  else if (param_2 == 1) {
    FUN_8ced1642(1);
  }
  else if (param_2 == 2) {
    local_14 = *(undefined4 *)(iVar2 + 0x30);
    FUN_8ced166c(&local_18,local_14);
    iVar3 = local_18 + *(int *)(iVar2 + 0x38);
    iVar1 = (*DAT_8ced16bc)(local_10,iVar3);
    *(int *)(iVar2 + 0x38) = iVar3 - iVar1;
  }
  return 0;
}


/* Address: 8ced161a */

void FUN_8ced161a(int param_1,int param_2)

{
  int iVar1;
  
  iVar1 = DAT_8ced16b8 + param_1 + (int)DAT_8ced16b6;
  *(int *)(iVar1 + 0x34) = param_2;
  if ((param_2 == 0) && (*(int *)(param_1 + 0x48) == 1)) {
    return;
  }
  thunk_EXT_FUN_8c644808(*(undefined4 *)(iVar1 + 8));
  return;
}


/* Address: 8ced1642 */

void FUN_8ced1642(int param_1,int param_2)

{
  int iVar1;
  
  iVar1 = param_1 + DAT_8ced16b8 + (int)DAT_8ced16b6;
  if ((param_2 == 0) && (*(int *)(iVar1 + 0x34) == 1)) {
    return;
  }
  thunk_EXT_FUN_8c644808(*(undefined4 *)(iVar1 + 8));
  return;
}


/* Address: 8ced1666 */

void thunk_EXT_FUN_8c644808(void)

{
                    /* WARNING: Could not recover jumptable at 0x8ced1668. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*DAT_8ced16c0)();
  return;
}


/* Address: 8ced166c */

void FUN_8ced166c(undefined4 *param_1,undefined4 param_2)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 *local_c;
  
  local_10 = param_2;
  local_c = param_1;
  (*(code *)PTR_FUN_8ced16c4)(&local_18,&local_14);
  puVar1 = local_c;
  uVar2 = (*(code *)PTR_callback_candidate_8ced9708_8ced16c8)(local_10,local_18,local_14);
  *puVar1 = uVar2;
  return;
}


/* Address: 8ced16dc */

undefined4 callback_candidate_8ced16dc(undefined4 param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = (*(code *)PTR_FUN_8ced17e0)(6);
  if (iVar1 == 0) {
    return 0;
  }
  uVar2 = FUN_8ced1700(param_1);
  return uVar2;
}


/* Address: 8ced1700 */

undefined4 FUN_8ced1700(undefined4 param_1)

{
  FUN_8ced1714();
  FUN_8ced1750(param_1);
  return 0;
}


/* Address: 8ced1714 */

void FUN_8ced1714(int param_1)

{
  int iVar1;
  
  iVar1 = (*(code *)PTR_FUN_8ced17e4)(7);
  if ((iVar1 != 1) &&
     (iVar1 = (*(code *)PTR_FUN_8ced17e8)
                        (param_1,*(undefined4 *)
                                  ((int)DAT_8ced17dc + DAT_8ced17da * 2 + -0x7c + param_1)),
     iVar1 == 1)) {
    (*(code *)PTR_FUN_8ced17ec)(param_1,7,1);
    return;
  }
  return;
}


/* Address: 8ced1750 */

void FUN_8ced1750(int param_1)

{
  int iVar1;
  
  iVar1 = (*(code *)PTR_FUN_8ced17f0)(7);
  if ((iVar1 != 1) &&
     (iVar1 = (*(code *)PTR_FUN_8ced17f4)
                        (param_1,*(undefined4 *)
                                  ((int)DAT_8ced17dc + DAT_8ced17da * 2 + -0x7c + param_1)),
     iVar1 == 1)) {
    (*(code *)PTR_FUN_8ced17f8)(param_1,7,1);
    return;
  }
  return;
}


/* Address: 8ced178c */

undefined4 callback_candidate_8ced178c(void)

{
  (*(code *)PTR_FUN_8ced17e0)(6);
  return 0;
}


/* Address: 8ced17a0 */

int callback_candidate_8ced17a0(undefined4 param_1)

{
  int iVar1;
  
  iVar1 = (*(code *)PTR_FUN_8ced17e0)(6);
  if (iVar1 != 0) {
    iVar1 = (*(code *)PTR_callback_candidate_8ceda5fc_8ced17fc)(param_1,3,5,0,0);
    if (iVar1 != 0) {
      return iVar1;
    }
  }
  return 0;
}


/* Address: 8ced1800 */

int callback_candidate_8ced1800(undefined4 param_1)

{
  int iVar1;
  
  iVar1 = (*(code *)PTR_FUN_8ced18d4)(6);
  if (iVar1 != 0) {
    iVar1 = (*(code *)PTR_callback_candidate_8ceda5fc_8ced18d8)(param_1,3,6,0,0);
    if (iVar1 != 0) {
      return iVar1;
    }
  }
  return 0;
}


/* Address: 8ced183a */

int callback_candidate_8ced183a(undefined4 param_1)

{
  int iVar1;
  
  iVar1 = (*(code *)PTR_FUN_8ced18d4)(6);
  if (iVar1 != 0) {
    iVar1 = (*(code *)PTR_callback_candidate_8ceda5fc_8ced18d8)(param_1,3,7,0,0);
    if (iVar1 != 0) {
      return iVar1;
    }
  }
  return 0;
}


/* Address: 8ced1874 */

int callback_candidate_8ced1874(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  
  iVar1 = (*(code *)PTR_FUN_8ced18d4)(param_1,6);
  if (iVar1 != 0) {
    iVar1 = (*(code *)PTR_callback_candidate_8ceda5fc_8ced18d8)(param_1,3,8,param_2,0);
    if (iVar1 != 0) {
      return iVar1;
    }
  }
  return 0;
}


/* Address: 8ced18e4 */

int callback_candidate_8ced18e4(void)

{
  int iVar1;
  
  iVar1 = FUN_8ced1908();
  if (iVar1 != 0) {
    return iVar1;
  }
  (*(code *)PTR_FUN_8ced1a04)();
  FUN_8ced1bc4();
  return 0;
}


/* Address: 8ced1908 */

int FUN_8ced1908(undefined4 *param_1)

{
  undefined *puVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  (*(code *)PTR_FUN_8ced1a0c)(DAT_8ced1a08,0,0x5e);
  iVar2 = DAT_8ced1a08;
  (*DAT_8ced1a14)(DAT_8ced1a08,PTR_s_SFD_Version_1_12_1999_03_08_8ced1a10,0x20);
  (*DAT_8ced1a14)(iVar2 + 0x20,PTR_s_Copyright__c__1997_1999_CRI_8ced1a18,0x20);
  (*(code *)PTR_FUN_8ced1a20)(iVar2 + 0x40,PTR_DAT_8ced1a1c,0x2f);
  (*DAT_8ced1a24)();
  iVar3 = (int)DAT_8ced19f6;
  *(undefined4 *)(DAT_8ced19f4 + iVar2) = 0;
  FUN_8ced1a90(iVar3 + iVar2);
  (*(code *)PTR_FUN_8ced1a28)(DAT_8ced19f8 + iVar2,param_1[3]);
  (*(code *)PTR_FUN_8ced1a2c)(DAT_8ced19fa + iVar2);
  iVar5 = (int)DAT_8ced19fe;
  iVar6 = param_1[2];
  iVar3 = (int)DAT_8ced19fc;
  iVar4 = param_1[1];
  *(int *)(iVar3 + iVar2) = iVar6;
  puVar1 = PTR_FUN_8ced1a0c;
  *(int *)(iVar3 + 4 + iVar2) = iVar4;
  (*(code *)puVar1)(iVar4,0,(uint)(iVar6 * iVar5) >> 2);
  iVar3 = 0;
  if (0 < iVar6) {
    do {
      iVar3 = iVar3 + 1;
      *(undefined4 *)(iVar4 + 0x40) = 0;
      iVar4 = iVar4 + iVar5;
    } while (iVar3 < iVar6);
  }
  iVar2 = (*(code *)PTR_callback_candidate_8ceda288_8ced1a30)(DAT_8ced1a00 + iVar2,*param_1);
  if (iVar2 == 0) {
    iVar2 = 0;
  }
  return iVar2;
}


/* Address: 8ced19c6 */

int callback_candidate_8ced19c6(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  undefined4 uVar8;
  
  iVar1 = DAT_8ced1a08;
  uVar8 = 0x80;
  iVar5 = 0;
  iVar3 = *(int *)(DAT_8ced19fc + DAT_8ced1a08);
  iVar2 = *(int *)(DAT_8ced19fc + 4 + DAT_8ced1a08);
  iVar7 = (int)DAT_8ced19fe;
  iVar6 = iVar2;
  for (iVar4 = 0; iVar4 < iVar3; iVar4 = iVar4 + 1) {
    if (*(int *)(iVar6 + 0x40) != 0) {
      iVar5 = (*(code *)PTR_callback_candidate_8ced3ff0_8ced1b30)(iVar2,uVar8);
    }
    iVar6 = iVar6 + iVar7;
    iVar2 = iVar2 + iVar7;
  }
  (*(code *)PTR_FUN_8ced1b34)(DAT_8ced1b26 + iVar1);
  (*(code *)PTR_FUN_8ced1b38)(DAT_8ced1b28 + iVar1);
  iVar1 = (*(code *)PTR_callback_candidate_8ceda2a6_8ced1b3c)(DAT_8ced1b2a + iVar1);
  if ((iVar1 == 0) && (iVar1 = iVar5, iVar5 == 0)) {
    FUN_8ced1bc8();
    iVar1 = 0;
  }
  return iVar1;
}


/* Address: 8ced1a90 */

void FUN_8ced1a90(undefined4 *param_1)

{
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  param_1[3] = 0;
  param_1[4] = 0;
  return;
}


/* Address: 8ced1a94 */

void FUN_8ced1a94(undefined4 *param_1)

{
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  param_1[3] = 0;
  param_1[4] = 0;
  return;
}


/* Address: 8ced1aa2 */

undefined4 FUN_8ced1aa2(int param_1,undefined4 param_2)

{
  if (param_1 == 0) {
    FUN_8ced1ada(DAT_8ced1b40,param_2);
  }
  else {
    FUN_8ced1ada(DAT_8ced1b2c + param_1,param_2);
    if (0 < *(int *)(param_1 + 0x40)) {
      *(int *)(param_1 + 0x40) = -*(int *)(param_1 + 0x40);
    }
  }
  return param_2;
}


/* Address: 8ced1ada */

void FUN_8ced1ada(undefined4 *param_1,int param_2)

{
  param_1[2] = param_2;
  if ((param_2 != 0) && ((code *)*param_1 != (code *)0x0)) {
    (*(code *)*param_1)(param_1[1]);
  }
  return;
}


/* Address: 8ced1afa */

undefined4 callback_candidate_8ced1afa(int param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = DAT_8ced1b40;
  if (param_1 != 0) {
    iVar1 = FUN_8ced1baa(param_1);
    if (iVar1 != 0) {
      uVar2 = FUN_8ced1aa2(0,DAT_8ced1b44);
      return uVar2;
    }
    iVar1 = DAT_8ced1bfa + param_1;
  }
  FUN_8ced1b5e(iVar1,param_2,param_3);
  return 0;
}


/* Address: 8ced1b5e */

void FUN_8ced1b5e(undefined4 *param_1,undefined4 param_2,undefined4 param_3)

{
  *param_1 = param_2;
  param_1[1] = param_3;
  return;
}


/* Address: 8ced1baa */

undefined4 FUN_8ced1baa(int param_1)

{
  *DAT_8ced1c0c = param_1;
  if ((param_1 != 0) && (*(int *)(param_1 + 0x40) != 0)) {
    return 0;
  }
  return 0xffffffff;
}


/* Address: 8ced1bc4 */

void FUN_8ced1bc4(void)

{
  return;
}


/* Address: 8ced1bc8 */

void FUN_8ced1bc8(void)

{
  return;
}


/* Address: 8ced1bcc */

uint FUN_8ced1bcc(uint *param_1)

{
  uint uVar1;
  byte in_MD;
  byte in_RB;
  byte in_BL;
  byte in_FD;
  byte in_M;
  byte in_Q;
  byte in_IMASK;
  byte in_S;
  byte in_T;
  
  uVar1 = (uint)DAT_8ced1bfc;
  *param_1 = (((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4) & 0xf0) >> 4;
  return ((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4 | (uint)in_Q << 8 | (uint)in_M << 9 |
          (uint)in_FD << 0xf | (uint)in_BL << 0x1c | (uint)in_RB << 0x1d | (uint)in_MD << 0x1e) &
         uVar1 | 0xf0;
}


/* Address: 8ced1be4 */

uint FUN_8ced1be4(uint *param_1)

{
  byte in_MD;
  byte in_RB;
  byte in_BL;
  byte in_FD;
  byte in_M;
  byte in_Q;
  byte in_IMASK;
  byte in_S;
  byte in_T;
  
  return (*param_1 & 0xf) << 4 |
         ((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4 | (uint)in_Q << 8 | (uint)in_M << 9 |
          (uint)in_FD << 0xf | (uint)in_BL << 0x1c | (uint)in_RB << 0x1d | (uint)in_MD << 0x1e) &
         (int)DAT_8ced1bfc;
}


/* Address: 8ced1c18 */

undefined4 callback_candidate_8ced1c18(int param_1)

{
  undefined4 uVar1;
  
  uVar1 = *(undefined4 *)((int)DAT_8ced1c94 + DAT_8ced1c92 + param_1);
  (*(code *)PTR_FUN_8ced1c98)(uVar1,1,uVar1);
  return 0;
}


/* Address: 8ced1cac */

undefined4 callback_candidate_8ced1cac(void)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = (*(code *)PTR_callback_candidate_8ced78c4_8ced1db8)(4,DAT_8ced1db4);
  if (iVar1 != 0) {
    uVar2 = (*(code *)PTR_FUN_8ced1dc0)(0,DAT_8ced1dbc);
    return uVar2;
  }
  return 0;
}


/* Address: 8ced1ccc */

undefined4 callback_candidate_8ced1ccc(void)

{
  (*(code *)PTR_callback_candidate_8ced7954_8ced1dc4)();
  return 0;
}


/* Address: 8ced1cda */

undefined4 callback_candidate_8ced1cda(undefined4 param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = FUN_8ced2164();
  if (iVar1 == 1) {
    return 0;
  }
  uVar2 = FUN_8ced1d0a(param_1);
  FUN_8ced21e8(param_1);
  FUN_8ced2334(param_1);
  return uVar2;
}


/* Address: 8ced1d0a */

int FUN_8ced1d0a(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int local_1c;
  undefined4 local_18;
  undefined4 local_14 [2];
  
  iVar1 = DAT_8ced1dc8;
  iVar3 = 0;
  do {
    iVar2 = FUN_8ced1d68(param_1,&local_18,local_14);
    if (((iVar2 != 0) || (iVar2 = FUN_8ced1e04(param_1,local_18,local_14[0],&local_1c), iVar2 != 0))
       || (local_1c == 0)) {
      return iVar2;
    }
    local_1c = *(int *)(param_1 + 0x28);
    iVar2 = FUN_8ced1dd0(param_1,local_1c);
    if (iVar2 != 0) {
      return iVar2;
    }
    iVar3 = iVar3 + local_1c;
  } while (iVar3 < iVar1);
  return 0;
}


/* Address: 8ced1d68 */

int FUN_8ced1d68(int param_1,undefined4 *param_2,undefined4 *param_3)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 local_20;
  undefined4 local_1c;
  
  uVar2 = *(undefined4 *)((int)DAT_8ced1db0 + DAT_8ced1dae + param_1 + (int)DAT_8ced1dac);
  iVar1 = (*(code *)PTR_callback_candidate_8ced9ba4_8ced1dcc)(uVar2,&local_20,uVar2);
  if (iVar1 != 0) {
    return iVar1;
  }
  *param_2 = local_20;
  *param_3 = local_1c;
  return 0;
}


/* Address: 8ced1dd0 */

int FUN_8ced1dd0(int param_1,undefined4 param_2)

{
  int iVar1;
  undefined4 uVar2;
  
  uVar2 = *(undefined4 *)((int)DAT_8ced1edc + DAT_8ced1eda + param_1 + (int)DAT_8ced1ed8);
  iVar1 = (*(code *)PTR_callback_candidate_8ced9c48_8ced1ee0)(uVar2,param_2,uVar2);
  if (iVar1 != 0) {
    return iVar1;
  }
  return 0;
}


/* Address: 8ced1e04 */

undefined4 FUN_8ced1e04(int param_1,int param_2,int param_3,int *param_4)

{
  int iVar1;
  undefined4 uVar2;
  int local_28;
  uint local_24;
  int local_20;
  int local_1c [2];
  
  iVar1 = (int)DAT_8ced1ed8;
  uVar2 = 0;
  *param_4 = 0;
  iVar1 = (*(code *)PTR_callback_candidate_8ced6848_8ced1ee4)
                    (*(undefined4 *)(DAT_8ced1eda + param_1 + iVar1 + 8),param_2,param_3,&local_28,
                     &local_24);
  if (iVar1 != 0) {
    uVar2 = (*(code *)PTR_FUN_8ced1eec)(param_1,DAT_8ced1ee8);
  }
  iVar1 = FUN_8ced21aa(param_1,param_3,local_24);
  if (iVar1 == 0) {
    if (local_24 == 0) {
      FUN_8ced1ec6(param_1,param_2,param_3,local_1c);
      *param_4 = local_1c[0];
    }
    else if ((local_24 & DAT_8ced1ef0) == 0) {
      uVar2 = (*(code *)PTR_FUN_8ced1eec)(param_1,DAT_8ced1ef4);
    }
    else {
      uVar2 = FUN_8ced1ef8(param_1,param_2 + local_28,param_3 - local_28,&local_20);
      if (local_20 != 0) {
        *param_4 = local_28 + local_20;
      }
    }
  }
  else {
    FUN_8ced2102(param_1,1);
  }
  return uVar2;
}


/* Address: 8ced1ec6 */

void FUN_8ced1ec6(int param_1,int param_2,int *param_3)

{
  if (*(int *)(param_1 + 0x28) <= param_2) {
    *param_3 = *(int *)(param_1 + 0x28);
    return;
  }
  *param_3 = 0;
  return;
}


/* Address: 8ced1ef8 */

int FUN_8ced1ef8(int param_1,undefined4 param_2,int param_3,int *param_4)

{
  undefined *puVar1;
  int iVar2;
  int iVar3;
  undefined1 auStack_30 [4];
  int local_2c;
  undefined4 local_28;
  int local_14;
  
  puVar1 = PTR_callback_candidate_8ced7878_8ced202c;
  iVar3 = 0;
  *param_4 = 0;
  iVar2 = (*(code *)puVar1)(*(undefined4 *)(DAT_8ced2024 + param_1 + (int)DAT_8ced2022 + 8),
                            auStack_30);
  if (iVar2 != 0) {
    iVar3 = (*(code *)PTR_FUN_8ced2034)(param_1,DAT_8ced2030);
  }
  if (param_3 < local_14) {
    iVar3 = 0;
  }
  else {
    iVar2 = (**(code **)(PTR_PTR_8ced2038 + local_2c * 4))(param_1,local_28,param_2,local_14);
    if (iVar2 == 1) {
      *param_4 = local_14;
    }
    else if (iVar2 != 0) {
      iVar3 = iVar2;
    }
  }
  return iVar3;
}


/* Address: 8ced1f7a */

undefined4 callback_candidate_8ced1f7a(int param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = (*(code *)PTR_FUN_8ced203c)(param_1,6);
  if (iVar1 == 0) {
    return 1;
  }
  uVar2 = FUN_8ced2048(param_1,*(undefined4 *)
                                ((int)DAT_8ced2026 + DAT_8ced2024 + param_1 + (int)DAT_8ced2022),
                       param_2,param_3);
  return uVar2;
}


/* Address: 8ced1fbe */

undefined4 callback_candidate_8ced1fbe(int param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = (*(code *)PTR_FUN_8ced203c)(param_1,5);
  if (iVar1 == 0) {
    return 1;
  }
  uVar2 = FUN_8ced2048(param_1,*(undefined4 *)
                                ((int)DAT_8ced2028 + DAT_8ced2024 + param_1 + (int)DAT_8ced2022),
                       param_2,param_3);
  return uVar2;
}


/* Address: 8ced2048 */

int FUN_8ced2048(undefined4 param_1,undefined4 param_2,int param_3,int param_4)

{
  int iVar1;
  undefined4 local_2c;
  int local_28;
  undefined4 local_24;
  int local_20;
  undefined4 local_18;
  
  iVar1 = (*(code *)PTR_callback_candidate_8ced9a2e_8ced214c)(param_1,&local_2c);
  if (iVar1 == 0) {
    if (local_20 + local_28 < param_4) {
      iVar1 = 0;
    }
    else {
      iVar1 = param_4;
      if (local_28 < param_4) {
        FUN_8ced20c4(local_2c,param_3,local_28);
        param_3 = local_28 + param_3;
        local_2c = local_24;
        iVar1 = param_4 - local_28;
      }
      FUN_8ced20c4(local_2c,param_3,iVar1);
      iVar1 = (*(code *)PTR_callback_candidate_8ced9aea_8ced2150)(param_1,param_2,param_4,local_18);
      if (iVar1 == 0) {
        iVar1 = 1;
      }
    }
  }
  return iVar1;
}


/* Address: 8ced20c4 */

void FUN_8ced20c4(uint param_1,uint param_2,uint param_3)

{
  uint uVar1;
  
  if (param_3 == 0) {
    return;
  }
  uVar1 = param_1 | param_2 | param_3;
  if ((uVar1 & 1) == 0) {
    if ((uVar1 & 3) == 0) {
      (*(code *)PTR_FUN_8ced2154)((int)param_3 >> 2);
      return;
    }
    (*(code *)PTR_FUN_8ced2158)((int)param_3 >> 1);
    return;
  }
  (*(code *)PTR_FUN_8ced215c)(param_3);
  return;
}


/* Address: 8ced2102 */

void FUN_8ced2102(int param_1,undefined4 param_2)

{
  undefined *puVar1;
  int iVar2;
  
  puVar1 = PTR_FUN_8ced2160;
  iVar2 = DAT_8ced2144 + param_1 + (int)DAT_8ced2142;
  (*(code *)PTR_FUN_8ced2160)(*(undefined4 *)(DAT_8ced2146 + iVar2),param_2);
  (*(code *)puVar1)(param_1,*(undefined4 *)(DAT_8ced2148 + iVar2),param_2);
  (*(code *)puVar1)(param_1,*(undefined4 *)(DAT_8ced214a + iVar2),param_2);
  return;
}


/* Address: 8ced2164 */

uint FUN_8ced2164(int param_1)

{
  undefined *puVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  
  puVar1 = PTR_FUN_8ced228c;
  iVar5 = DAT_8ced2280 + param_1 + (int)DAT_8ced227e;
  uVar2 = (*(code *)PTR_FUN_8ced228c)(*(undefined4 *)(DAT_8ced2282 + iVar5));
  uVar3 = (*(code *)puVar1)(param_1,*(undefined4 *)(DAT_8ced2284 + iVar5));
  uVar4 = (*(code *)puVar1)(param_1,*(undefined4 *)(DAT_8ced2286 + iVar5));
  return uVar2 & uVar3 & uVar4;
}


/* Address: 8ced21aa */

undefined4 FUN_8ced21aa(int param_1,int param_2,int param_3)

{
  int iVar1;
  undefined4 uVar2;
  
  if (param_3 == DAT_8ced2290) {
    return 1;
  }
  if ((param_2 < *(int *)(param_1 + 0x28)) &&
     (uVar2 = *(undefined4 *)((int)DAT_8ced2288 + DAT_8ced2280 + param_1 + (int)DAT_8ced227e),
     iVar1 = (*(code *)PTR_FUN_8ced228c)(uVar2,uVar2), iVar1 == 1)) {
    return 1;
  }
  return 0;
}


/* Address: 8ced21e8 */

void FUN_8ced21e8(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  
  iVar1 = FUN_8ced2238();
  if (iVar1 != 1) {
    iVar3 = DAT_8ced2280 + param_1 + (int)DAT_8ced227e;
    uVar2 = *(undefined4 *)(DAT_8ced2288 + iVar3);
    iVar1 = (*(code *)PTR_FUN_8ced2294)(param_1,uVar2);
    if ((iVar1 == 1) && (iVar1 = FUN_8ced22d8(param_1,uVar2), iVar1 != 0)) {
      FUN_8ced2298(param_1,1);
    }
    FUN_8ced23e6(param_1,iVar3);
    return;
  }
  return;
}


/* Address: 8ced2238 */

uint FUN_8ced2238(int param_1)

{
  undefined *puVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  
  puVar1 = PTR_FUN_8ced2294;
  iVar5 = DAT_8ced2280 + param_1 + (int)DAT_8ced227e;
  uVar2 = (*(code *)PTR_FUN_8ced2294)(*(undefined4 *)(DAT_8ced2282 + iVar5));
  uVar3 = (*(code *)puVar1)(param_1,*(undefined4 *)(DAT_8ced2284 + iVar5));
  uVar4 = (*(code *)puVar1)(param_1,*(undefined4 *)(DAT_8ced2286 + iVar5));
  return uVar2 | uVar3 | uVar4;
}


/* Address: 8ced2298 */

void FUN_8ced2298(int param_1,undefined4 param_2)

{
  undefined *puVar1;
  int iVar2;
  
  puVar1 = PTR_FUN_8ced232c;
  iVar2 = DAT_8ced2320 + param_1 + (int)DAT_8ced231e;
  (*(code *)PTR_FUN_8ced232c)(*(undefined4 *)(DAT_8ced2322 + iVar2),param_2);
  (*(code *)puVar1)(param_1,*(undefined4 *)(DAT_8ced2324 + iVar2),param_2);
  (*(code *)puVar1)(param_1,*(undefined4 *)(DAT_8ced2326 + iVar2),param_2);
  return;
}


/* Address: 8ced22d8 */

undefined4 FUN_8ced22d8(int param_1)

{
  int iVar1;
  int iVar2;
  undefined1 auStack_20 [20];
  int local_c;
  
  iVar2 = *(int *)(DAT_8ced2328 + param_1 + 0x58);
  if (*(int *)(param_1 + 8) < iVar2) {
    iVar2 = *(int *)(param_1 + 8);
  }
  iVar1 = (*(code *)PTR_callback_candidate_8ced9ba4_8ced2330)(auStack_20);
  if ((iVar1 == 0) && (iVar2 <= local_c)) {
    return 1;
  }
  return 0;
}


/* Address: 8ced2334 */

void FUN_8ced2334(int param_1)

{
  undefined *puVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined1 auStack_40 [8];
  int local_38;
  int local_34;
  
  puVar1 = PTR_callback_candidate_8ced783a_8ced2430;
  uVar2 = *(undefined4 *)(DAT_8ced242e + param_1 + (int)DAT_8ced242c + 8);
  if (*(int *)(param_1 + 0x40) == 2) {
    iVar3 = 0;
    iVar5 = 0;
    iVar4 = 0;
    do {
      (*(code *)puVar1)(uVar2,auStack_40,iVar3);
      if (iVar5 <= local_38) {
        iVar5 = local_38;
      }
      iVar3 = iVar3 + 1;
      if (iVar4 <= local_34) {
        iVar4 = local_34;
      }
    } while (iVar3 < 3);
    iVar3 = (*(code *)PTR_FUN_8ced2434)(param_1,6);
    if (((iVar3 == 1) && (iVar5 == 0)) &&
       (iVar5 = (*(code *)PTR_FUN_8ced2438)(param_1,6), iVar5 != 0)) {
      (*(code *)PTR_callback_candidate_8ced43f8_8ced243c)(param_1,6,0);
    }
    iVar5 = (*(code *)PTR_FUN_8ced2434)(param_1,6);
    if (((iVar5 == 1) && (iVar4 == 0)) &&
       (iVar4 = (*(code *)PTR_FUN_8ced2438)(param_1,7), iVar4 != 0)) {
      (*(code *)PTR_callback_candidate_8ced43f8_8ced243c)(param_1,5,0);
    }
  }
  return;
}


/* Address: 8ced23e6 */

void FUN_8ced23e6(int param_1,int param_2)

{
  undefined4 uVar1;
  undefined1 auStack_38 [16];
  int local_28;
  undefined1 auStack_18 [8];
  int local_10;
  
  uVar1 = *(undefined4 *)(param_2 + 8);
  (*(code *)PTR_callback_candidate_8ced7808_8ced2440)(uVar1,auStack_18);
  if ((local_10 != -1) && (0 < local_10)) {
    *(int *)(param_1 + 100) = local_10;
  }
  (*(code *)PTR_callback_candidate_8ced783a_8ced2430)(uVar1,auStack_38,1);
  if (local_28 != -1) {
    *(int *)(param_1 + 0x68) = local_28;
  }
  return;
}


/* Address: 8ced2444 */

undefined4 callback_candidate_8ced2444(int param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  int *piVar4;
  
  piVar4 = (int *)(DAT_8ced2512 + param_1 + (int)DAT_8ced2510 + 8);
  FUN_8ced24a2(piVar4);
  iVar1 = (*(code *)PTR_callback_candidate_8ced7adc_8ced2514)();
  uVar3 = DAT_8ced2518;
  if (iVar1 != 0) {
    iVar2 = (*(code *)PTR_callback_candidate_8ced7a2c_8ced2520)(iVar1,PTR_LAB_8ced251c,param_1);
    if (iVar2 == 0) {
      *piVar4 = iVar1;
      return 0;
    }
    callback_candidate_8ced7bb4(iVar1);
    uVar3 = DAT_8ced2524;
  }
  uVar3 = (*(code *)PTR_FUN_8ced2528)(param_1,uVar3);
  return uVar3;
}


/* Address: 8ced24a2 */

void FUN_8ced24a2(undefined4 *param_1)

{
  *param_1 = 0;
  return;
}


/* Address: 8ced24ae */

undefined4 callback_candidate_8ced24ae(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = callback_candidate_8ced7bb4
                    (*(undefined4 *)(param_1 + DAT_8ced2512 + (int)DAT_8ced2510 + 8));
  if (iVar1 != 0) {
    uVar2 = (*(code *)PTR_FUN_8ced2528)(param_1,DAT_8ced252c);
    return uVar2;
  }
  return 0;
}


/* Address: 8ced24da */

void callback_candidate_8ced7bb4(void)

{
  (*(code *)PTR_callback_candidate_8ced7bb4_8ced2530)();
  return;
}


/* Address: 8ced2538 */

void FUN_8ced2538(undefined4 *param_1)

{
  undefined4 *puVar1;
  
  puVar1 = DAT_8ced2638;
  *DAT_8ced2638 = *param_1;
  puVar1[1] = param_1[1];
  puVar1[2] = param_1[2];
  puVar1[3] = param_1[3];
  puVar1[4] = param_1[4] + 0x1f & 0xffffffe0;
  puVar1[5] = param_1[5];
  puVar1[6] = param_1[6];
  puVar1[7] = param_1[7];
  puVar1[8] = param_1[8] + 0x1f & 0xffffffe0;
  return;
}


/* Address: 8ced256a */

undefined4 callback_candidate_8ced256a(void)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = (*(code *)PTR_callback_candidate_8ced80ac_8ced2640)(8,DAT_8ced263c);
  if (iVar1 == 0) {
    (*(code *)PTR_FUN_8ced2654)(DAT_8ced2638,0,9);
    return 0;
  }
  uVar2 = DAT_8ced264c;
  if (iVar1 == DAT_8ced2644) {
    uVar2 = DAT_8ced2648;
  }
  uVar2 = (*(code *)PTR_FUN_8ced2650)(0,uVar2);
  return uVar2;
}


/* Address: 8ced25a0 */

undefined4 callback_candidate_8ced25a0(void)

{
  (*(code *)PTR_FUN_8ced2658)();
  return 0;
}


/* Address: 8ced25ae */

undefined4 callback_candidate_8ced25ae(undefined4 param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = (*(code *)PTR_FUN_8ced265c)(5);
  if ((iVar1 != 0) && (iVar1 = FUN_8ced313e(param_1), iVar1 != 1)) {
    uVar2 = FUN_8ced25e6(param_1);
    FUN_8ced3194(param_1);
    return uVar2;
  }
  return 0;
}


/* Address: 8ced25e6 */

int FUN_8ced25e6(undefined4 param_1)

{
  int iVar1;
  undefined4 local_18;
  int local_14;
  undefined4 local_10;
  undefined4 local_c [2];
  
  do {
    iVar1 = FUN_8ced26ae(param_1,&local_10,local_c);
    if (iVar1 != 0) {
      return iVar1;
    }
    iVar1 = FUN_8ced29ba(param_1,local_10,local_c[0],&local_18,&local_14);
    if (iVar1 != 0) {
      return iVar1;
    }
    iVar1 = FUN_8ced2986(param_1,local_18);
    if (iVar1 != 0) {
      return iVar1;
    }
  } while (local_14 != 0);
  return 0;
}


/* Address: 8ced2660 */

undefined4 FUN_8ced2660(int param_1)

{
  int iVar1;
  int iVar2;
  
  iVar1 = *(int *)(DAT_8ced2762 + param_1 + 0x5c);
  iVar2 = *(int *)(param_1 + 0x2c);
  if (iVar1 == -1) {
    iVar1 = iVar2;
  }
  if (iVar2 < iVar1) {
    iVar1 = iVar2;
  }
  iVar2 = FUN_8ced353c();
  if ((iVar2 != -1) && (iVar2 < iVar1)) {
    return 0;
  }
  return 1;
}


/* Address: 8ced26ae */

int FUN_8ced26ae(int param_1,int *param_2,int *param_3)

{
  int iVar1;
  undefined4 uVar2;
  int local_48;
  int local_44;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  int *local_34;
  int local_30;
  int local_2c;
  
  uVar2 = *(undefined4 *)((int)DAT_8ced2764 + DAT_8ced2760 + param_1 + (int)DAT_8ced275e);
  local_34 = param_2;
  iVar1 = (*(code *)PTR_callback_candidate_8ced9ba4_8ced2768)(param_1,uVar2,&local_30);
  if (iVar1 == 0) {
    (*(code *)PTR_callback_candidate_8ced9d2e_8ced276c)
              (param_1,uVar2,&local_48,&local_3c,&local_40,&local_44,&local_38);
    iVar1 = FUN_8ced27b4(&local_30,local_38,&local_40,&local_44);
    if (iVar1 == 0) {
      if (local_2c < DAT_8ced2774) {
        if (local_44 == 0) {
          iVar1 = FUN_8ced28c0(param_1,&local_30,&local_48,&local_3c,local_40);
          if (iVar1 != 0) {
            return iVar1;
          }
          if (local_48 == 0) {
            *param_3 = 0;
          }
          else {
            *param_3 = local_48 - local_30;
          }
        }
        else {
          *param_3 = local_44 - local_30;
        }
      }
      else {
        *param_3 = local_2c;
      }
      *local_34 = local_30;
      (*(code *)PTR_callback_candidate_8ced9d7e_8ced2868)
                (param_1,uVar2,local_48,local_3c,local_40,local_44);
      iVar1 = 0;
    }
    else {
      iVar1 = (*(code *)PTR_FUN_8ced2770)(param_1);
    }
  }
  return iVar1;
}


/* Address: 8ced27b4 */

undefined4 FUN_8ced27b4(int *param_1,int param_2,int *param_3,int *param_4)

{
  undefined *puVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  
  if (*param_4 == 0) {
    iVar8 = *param_1;
    iVar3 = param_1[2];
    if (*param_3 < param_1[3]) {
      iVar4 = *param_3;
      iVar2 = iVar4;
      if (3 < iVar4) {
        iVar2 = 3;
      }
      iVar6 = 3 - iVar4;
      if (iVar6 < 0) {
        iVar6 = 0;
      }
      iVar7 = param_1[3] - iVar4;
      iVar5 = param_2 - iVar4;
      if (iVar7 < iVar5) {
        iVar5 = iVar7;
      }
      iVar7 = param_1[1] + iVar4 + iVar8;
      iVar2 = (*(code *)PTR_callback_candidate_8ced7da8_8ced286c)
                        (iVar7 - iVar2,(iVar3 + iVar4) - iVar2,iVar5 + iVar2,(int)DAT_8ced2864,iVar5
                        );
      puVar1 = PTR_FUN_8ced2870;
      iVar4 = iVar2 - (iVar3 + iVar4);
      *param_3 = *param_3 + iVar4;
      iVar3 = (*(code *)puVar1)(iVar2 + -4);
      iVar4 = iVar4 + iVar7;
      if ((iVar3 == 0x40) || (iVar3 == DAT_8ced2866)) {
        *param_4 = iVar4;
      }
      else if ((iVar3 == 4) || (iVar3 == 8)) {
        iVar8 = iVar7 - iVar8;
        iVar3 = (*(code *)PTR_callback_candidate_8ced7c62_8ced294c)
                          (iVar6 + iVar7,iVar8 + iVar6,(int)DAT_8ced2946,iVar8);
        if ((iVar3 != 0) && (iVar3 = (*(code *)PTR_FUN_8ced2950)(iVar3), iVar3 == 4)) {
          *param_4 = iVar4;
        }
      }
      else if (param_2 <= *param_3) {
        return DAT_8ced2954;
      }
    }
  }
  return 0;
}


/* Address: 8ced28c0 */

undefined4 FUN_8ced28c0(int *param_1,int *param_2,int *param_3,int param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  
  iVar6 = *param_1;
  if ((*param_2 == 0) || (*param_2 - iVar6 < 5)) {
    if (3 < param_4) {
      param_4 = 3;
    }
    iVar4 = param_4 + param_1[1] + iVar6;
    if (iVar4 != *param_3) {
      iVar7 = iVar4 - iVar6;
      *param_3 = iVar4;
      iVar1 = 0;
      while ((3 < iVar7 &&
             (iVar2 = (*(code *)PTR_callback_candidate_8ced7c62_8ced294c)
                                (iVar4,iVar7,(int)DAT_8ced2946), iVar2 != 0))) {
        iVar3 = (*(code *)PTR_FUN_8ced2950)(iVar2);
        iVar5 = iVar2 + 4;
        if ((iVar3 == 0x40) || (iVar3 == DAT_8ced2948)) {
          *param_2 = iVar5;
          return 0;
        }
        iVar8 = iVar5;
        if (((iVar3 != 8) && (iVar8 = iVar1, iVar3 == 4)) && (iVar8 = iVar5, iVar1 != 0))
        goto LAB_8ced2970;
        iVar7 = iVar7 - (iVar2 - iVar4);
        iVar4 = iVar4 + (iVar2 - iVar4);
        iVar1 = iVar8;
      }
      if (iVar1 == iVar6 + 4) {
LAB_8ced2970:
        *param_2 = iVar1;
      }
    }
  }
  return 0;
}


/* Address: 8ced2986 */

int FUN_8ced2986(int param_1,undefined4 param_2)

{
  int iVar1;
  undefined4 uVar2;
  
  uVar2 = *(undefined4 *)((int)DAT_8ced2a4c + DAT_8ced2a4a + param_1 + (int)DAT_8ced2a48);
  iVar1 = (*(code *)PTR_callback_candidate_8ced9c48_8ced2a50)(uVar2,param_2,uVar2);
  if (iVar1 != 0) {
    return iVar1;
  }
  return 0;
}


/* Address: 8ced29ba */

int FUN_8ced29ba(int param_1,int param_2,int param_3,int *param_4)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  undefined4 uVar4;
  int local_94 [2];
  undefined4 *local_8c;
  undefined1 auStack_88 [48];
  int local_58;
  int iStack_54;
  undefined1 auStack_50 [24];
  undefined4 uStack_38;
  
  iVar1 = (int)DAT_8ced2a4e;
  iVar2 = (int)DAT_8ced2a4a;
  local_94[1] = param_2;
  *param_4 = 0;
  **(undefined4 **)((int)local_94 + iVar1) = 0;
  local_8c = (undefined4 *)(iVar2 + param_1 + (int)DAT_8ced2a48 + 8);
  uVar4 = *local_8c;
  if (param_3 < 4) {
    uVar3 = 0;
  }
  else {
    uVar3 = (*(code *)PTR_FUN_8ced2a54)(param_2);
  }
  local_58 = local_94[1];
  iStack_54 = param_3;
  iVar1 = FUN_8ced2bae(param_1,param_3,uVar3);
  if (iVar1 != 0) {
    FUN_8ced352c(param_1);
    return 0;
  }
  if (param_3 < 5) {
    return 0;
  }
  if ((uVar3 & 0x4c) == 0) {
    if ((uVar3 & 2) != 0) {
      iVar1 = (*(code *)PTR_callback_candidate_8ced9338_8ced2b40)(uVar4,auStack_50);
      if (iVar1 != 0) {
        (*(code *)PTR_FUN_8ced2b48)(param_1,DAT_8ced2b44);
      }
      iVar1 = FUN_8ced2bd2(param_1,uVar4,auStack_50);
      if (iVar1 != 0) {
        return iVar1;
      }
      iVar1 = FUN_8ced2ccc(param_1,auStack_50);
      if (iVar1 == 0) {
        iVar1 = FUN_8ced3492(param_1,auStack_50,auStack_88,local_94);
        if (iVar1 != 0) {
          return 0;
        }
        iVar1 = (*(code *)PTR_callback_candidate_8ced7f2c_8ced2b58)
                          (uVar4,&local_58,param_4,auStack_88);
        if (iVar1 == 0) {
          if (0 < *param_4) {
            FUN_8ced362e(local_94[0]);
            (*(code *)PTR_FUN_8ced2b60)
                      (uVar4,DAT_8ced2b3e + param_1 + 8,DAT_8ced2b3e + param_1 + 0xc);
            (*(code *)PTR_callback_candidate_8ced3f42_8ced2b64)(param_1,1,uStack_38);
            local_8c[0xd] = 0;
            return 0;
          }
          FUN_8ced3628(local_94[0]);
          return 0;
        }
        FUN_8ced3628(local_94[0]);
        uVar4 = DAT_8ced2b5c;
      }
      else {
        iVar1 = (*(code *)PTR_callback_candidate_8ced801c_8ced2b4c)(uVar4,&local_58,param_4);
        uVar4 = DAT_8ced2b50;
        if (iVar1 == 0) {
          **(undefined4 **)((int)local_94 + (int)DAT_8ced2b3c) = 1;
          (*(code *)PTR_callback_candidate_8ced3fb0_8ced2b54)(param_1,1,uStack_38);
          return 0;
        }
      }
      goto LAB_8ced2afc;
    }
    if (uVar3 != 0) {
      return 0;
    }
    iVar1 = (*(code *)PTR_callback_candidate_8ced7d0e_8ced2c6c)
                      (local_94[1],param_3,(int)DAT_8ced2c62);
    if (iVar1 == 0) {
      iVar1 = (int)DAT_8ced2c64;
      *param_4 = param_3 + -4;
      **(undefined4 **)((int)local_94 + iVar1) = 0;
      return 0;
    }
    *param_4 = iVar1 - local_94[1];
  }
  else {
    iVar1 = (*(code *)PTR_callback_candidate_8ced86d8_8ced2a58)(uVar4,&local_58,param_4);
    uVar4 = DAT_8ced2a5c;
    if (iVar1 != 0) {
LAB_8ced2afc:
      iVar1 = (*(code *)PTR_FUN_8ced2b48)(param_1,uVar4);
      return iVar1;
    }
  }
  **(undefined4 **)((int)local_94 + (int)DAT_8ced2c64) = 1;
  return 0;
}


/* Address: 8ced2bae */

undefined4 FUN_8ced2bae(int param_1,int param_2)

{
  int iVar1;
  
  if ((param_2 != DAT_8ced2c66) && ((4 < param_1 || (iVar1 = FUN_8ced3126(), iVar1 != 1)))) {
    return 0;
  }
  return 1;
}


/* Address: 8ced2bd2 */

undefined4 FUN_8ced2bd2(int param_1,undefined4 param_2,undefined4 *param_3)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 local_14;
  undefined4 local_10;
  
  if (*(int *)(param_1 + 0x5c) == 0) {
    *(undefined4 *)(param_1 + 0x4c) = *param_3;
    *(undefined4 *)(param_1 + 0x50) = param_3[1];
    *(undefined4 *)(param_1 + 0x54) = param_3[2];
    *(undefined4 *)(param_1 + 0x58) = param_3[3];
    *(undefined4 *)(param_1 + 0x60) = param_3[4];
    local_10 = param_2;
    iVar2 = (*(code *)PTR_callback_candidate_8ced93b0_8ced2c70)(param_2,&local_14);
    if (iVar2 == 0) {
      *(undefined4 *)(param_1 + 0x5c) = local_14;
      uVar1 = FUN_8ced2c2c(param_1,(undefined4 *)(param_1 + 0x4c));
    }
    else {
      uVar1 = (*(code *)PTR_FUN_8ced2c78)(param_1,DAT_8ced2c74);
    }
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}


/* Address: 8ced2c2c */

undefined4 FUN_8ced2c2c(int param_1,int *param_2)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar3 = DAT_8ced2c6a + param_1 + (int)DAT_8ced2c68;
  iVar4 = *param_2;
  iVar2 = param_2[1];
  uVar1 = DAT_8ced2c7c;
  if ((((iVar4 <= *(int *)(iVar3 + 0x14)) && (iVar4 <= *(int *)(iVar3 + 0xc) << 1)) &&
      (iVar2 <= *(int *)(iVar3 + 0x18))) &&
     (((iVar2 <= *(int *)(iVar3 + 0x10) << 1 &&
       (iVar2 = iVar4 * iVar2 * 3, iVar4 = *(int *)(iVar3 + 0x20) * *(int *)(iVar3 + 0x24) * 3,
       uVar1 = DAT_8ced2d68,
       (int)(iVar2 + (uint)(iVar2 < 0)) >> 1 <= (int)(iVar4 + (uint)(iVar4 < 0)) >> 1)) &&
      (uVar1 = DAT_8ced2d6c, 0 < *(int *)(iVar3 + 0x28))))) {
    return 0;
  }
  uVar1 = (*(code *)PTR_FUN_8ced2c78)(uVar1);
  return uVar1;
}


/* Address: 8ced2ccc */

undefined4 FUN_8ced2ccc(int param_1,int param_2)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 uVar3;
  
  if (*(int *)(DAT_8ced2d62 + param_1) == 0) {
    uVar3 = *(undefined4 *)(param_2 + 0x18);
    iVar2 = FUN_8ced2da8(param_1,uVar3);
    if (((iVar2 == 0) && (iVar2 = FUN_8ced2d26(param_1,uVar3), iVar2 == 0)) &&
       (iVar2 = FUN_8ced2dd6(param_1,param_2,uVar3), iVar2 == 0)) {
      uVar1 = 0;
    }
    else {
      uVar1 = 1;
    }
    FUN_8ced2d70(param_1,uVar3,uVar1);
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}


/* Address: 8ced2d26 */

undefined4 FUN_8ced2d26(int param_1,int param_2)

{
  int iVar1;
  
  iVar1 = *(int *)(param_1 + DAT_8ced2d66 + (int)DAT_8ced2d64 + 0x34);
  if (iVar1 == 2) {
    if (param_2 == 2) {
      return 1;
    }
  }
  else if (iVar1 != 3) {
    return 0;
  }
  if (param_2 == 3) {
    return 1;
  }
  return 0;
}


/* Address: 8ced2d70 */

void FUN_8ced2d70(int param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  
  iVar1 = param_1 + DAT_8ced2e18 + (int)DAT_8ced2e16;
  iVar2 = *(int *)(iVar1 + 0x34);
  if (param_3 == 1) {
    if ((param_2 == 1) || (param_2 == 2)) {
      iVar2 = 2;
    }
  }
  else if (iVar2 == 2) {
    iVar2 = 3;
  }
  else if (iVar2 == 3) {
    iVar2 = 5;
  }
  *(int *)(iVar1 + 0x34) = iVar2;
  return;
}


/* Address: 8ced2da8 */

undefined4 FUN_8ced2da8(int param_1,int param_2)

{
  int iVar1;
  
  param_1 = param_1 + DAT_8ced2e1a;
  if (param_2 == 1) {
    iVar1 = *(int *)(param_1 + 8);
  }
  else if (param_2 == 2) {
    iVar1 = *(int *)(param_1 + 0xc);
  }
  else {
    if (param_2 != 3) {
      return 1;
    }
    iVar1 = *(int *)(param_1 + 0x10);
  }
  if (iVar1 == 0) {
    return 1;
  }
  return 0;
}


/* Address: 8ced2dd6 */

undefined4 FUN_8ced2dd6(int param_1,int param_2)

{
  undefined4 uVar1;
  undefined4 extraout_r2;
  int iVar2;
  int iVar3;
  int local_30;
  undefined4 local_2c;
  int local_28;
  undefined1 local_24 [4];
  undefined1 local_20 [4];
  undefined4 local_1c [2];
  
  iVar2 = DAT_8ced2e18 + param_1 + (int)DAT_8ced2e16;
  iVar3 = DAT_8ced2e1c + param_1;
  FUN_8ced3320(param_1,&local_30,&local_2c);
  if (*(code **)(iVar3 + 0x18) != (code *)0x0) {
    uVar1 = (**(code **)(iVar3 + 0x18))(param_1,param_2,local_30,local_2c);
    return uVar1;
  }
  if (param_2 == 1) {
    (*(code *)PTR_FUN_8ced2f24)(iVar3,local_30);
  }
  else if (param_2 != 2) goto LAB_8ced2e42;
  local_30 = (*(code *)PTR_FUN_8ced2f28)(iVar3,local_30);
LAB_8ced2e42:
  if ((*(int *)(iVar2 + 0x3c) < *(int *)(DAT_8ced2f18 + param_1)) &&
     ((*(code *)PTR_FUN_8ced2f2c)(param_1,&local_28,local_1c), -1 < local_28)) {
    FUN_8ced2eae(param_1,param_2,local_24,local_20);
    iVar3 = (*DAT_8ced2f30)();
    local_30 = local_30 - iVar3;
    iVar3 = (*(code *)PTR_callback_candidate_8ced65f0_8ced2f34)
                      (local_28,local_1c[0],local_30,extraout_r2);
    if (iVar3 == 0) {
      *(int *)(iVar2 + 0x3c) = *(int *)(iVar2 + 0x3c) + 1;
      return 1;
    }
  }
  return 0;
}


/* Address: 8ced2eae */

void FUN_8ced2eae(int param_1,undefined4 *param_2,undefined4 *param_3)

{
  int iVar1;
  int iVar2;
  
  iVar2 = (int)DAT_8ced2f1a;
  iVar1 = (int)DAT_8ced2f1c;
  *param_3 = *(undefined4 *)(iVar1 + param_1 + iVar2);
  *param_2 = *(undefined4 *)(iVar1 + -4 + param_1 + iVar2);
  return;
}


/* Address: 8ced2ec0 */

undefined4 callback_candidate_8ced2ec0(int param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  undefined4 uVar2;
  
  if (param_1 == 0) {
    uVar2 = 0;
  }
  else {
    iVar1 = (*(code *)PTR_FUN_8ced2f38)(param_1);
    if (iVar1 != 0) {
      param_1 = 0;
      uVar2 = DAT_8ced2f3c;
      goto LAB_8ced2f04;
    }
    uVar2 = *(undefined4 *)(DAT_8ced2f20 + param_1 + (int)DAT_8ced2f1e + 8);
  }
  iVar1 = (*(code *)PTR_callback_candidate_8ced8604_8ced2f40)(uVar2,param_2,param_3);
  uVar2 = DAT_8ced2f44;
  if (iVar1 == 0) {
    return 0;
  }
LAB_8ced2f04:
  uVar2 = (*(code *)PTR_FUN_8ced2f48)(param_1,uVar2);
  return uVar2;
}


/* Address: 8ced2f4c */

int callback_candidate_8ced2f4c(int param_1)

{
  undefined *puVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  int *piVar5;
  
  iVar2 = (*(code *)PTR_FUN_8ced302c)(5);
  if (iVar2 == 0) {
    return 0;
  }
  piVar5 = (int *)(DAT_8ced3028 + param_1 + (int)DAT_8ced3026 + 8);
  iVar2 = FUN_8ced2fda(param_1,piVar5);
  if (iVar2 == 0) {
    iVar2 = (*(code *)PTR_callback_candidate_8ced8536_8ced3030)();
    uVar4 = DAT_8ced3034;
    if (iVar2 != 0) {
      iVar3 = (*(code *)PTR_callback_candidate_8ced7e54_8ced303c)(iVar2,PTR_LAB_8ced3038,param_1);
      puVar1 = PTR_callback_candidate_8ced8604_8ced3048;
      if (iVar3 == 0) {
        (*(code *)PTR_callback_candidate_8ced8604_8ced3048)(iVar2,2,1);
        (*(code *)puVar1)(iVar2,1,0);
        (*(code *)puVar1)(iVar2,3,0);
        *piVar5 = iVar2;
        return 0;
      }
      callback_candidate_8ced85de(iVar2);
      uVar4 = DAT_8ced3040;
    }
    iVar2 = (*(code *)PTR_FUN_8ced3044)(param_1,uVar4);
    return iVar2;
  }
  return iVar2;
}


/* Address: 8ced2fda */

undefined4 FUN_8ced2fda(undefined4 *param_1)

{
  undefined4 uVar1;
  
  if ((*(int *)(DAT_8ced304c + 0x10) != 0) && (*(int *)(DAT_8ced304c + 0x20) != 0)) {
    (*DAT_8ced3054)();
    *param_1 = 0;
    param_1[10] = 0;
    param_1[0xb] = 5;
    FUN_8ced3390();
    param_1[0xd] = 0;
    return 0;
  }
  uVar1 = (*(code *)PTR_FUN_8ced3044)(DAT_8ced3050);
  return uVar1;
}


/* Address: 8ced3058 */

undefined4 callback_candidate_8ced3058(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  int *piVar3;
  
  piVar3 = (int *)(param_1 + DAT_8ced3174 + (int)DAT_8ced3172 + 8);
  if (*piVar3 != 0) {
    iVar1 = callback_candidate_8ced85de();
    if (iVar1 != 0) {
      uVar2 = (*(code *)PTR_FUN_8ced3180)(param_1,DAT_8ced317c);
      return uVar2;
    }
    *piVar3 = 0;
  }
  return 0;
}


/* Address: 8ced3098 */

void callback_candidate_8ced85de(void)

{
  (*(code *)PTR_callback_candidate_8ced85de_8ced3184)();
  return;
}


/* Address: 8ced30be */

undefined4 callback_candidate_8ced30be(int param_1,int *param_2)

{
  int iVar1;
  int local_10 [2];
  
  iVar1 = FUN_8ced353c(param_1);
  if (iVar1 == -1) {
    FUN_8ced3156(param_1,1);
  }
  else {
    iVar1 = FUN_8ced3634(param_1,local_10);
    if (iVar1 != 0) {
      FUN_8ced31ec(param_1,iVar1,param_2);
      if (local_10[0] == 0) {
        return 0;
      }
      iVar1 = (int)DAT_8ced3176;
      *(undefined4 *)(iVar1 + param_1 + 0x40) = *(undefined4 *)(*param_2 + 0x14);
      *(undefined4 *)(iVar1 + param_1 + 0x44) = *(undefined4 *)(*param_2 + 0x18);
      return 0;
    }
  }
  *param_2 = 0;
  return 0;
}


/* Address: 8ced3126 */

void FUN_8ced3126(int param_1)

{
  (*(code *)PTR_FUN_8ced318c)
            (*(undefined4 *)((int)DAT_8ced3178 + DAT_8ced3174 + param_1 + (int)DAT_8ced3172));
  return;
}


/* Address: 8ced313e */

void FUN_8ced313e(int param_1)

{
  (*(code *)PTR_FUN_8ced318c)
            (*(undefined4 *)((int)DAT_8ced317a + DAT_8ced3174 + param_1 + (int)DAT_8ced3172));
  return;
}


/* Address: 8ced3156 */

void FUN_8ced3156(int param_1,undefined4 param_2)

{
  (*(code *)PTR_FUN_8ced3190)
            (*(undefined4 *)((int)DAT_8ced317a + DAT_8ced3174 + param_1 + (int)DAT_8ced3172),param_2
            );
  return;
}


/* Address: 8ced3194 */

void FUN_8ced3194(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 uVar3;
  
  iVar2 = DAT_8ced32ce + param_1 + (int)DAT_8ced32cc;
  uVar1 = *(undefined4 *)(DAT_8ced32d0 + iVar2);
  uVar3 = *(undefined4 *)(DAT_8ced32d2 + iVar2);
  iVar2 = (*(code *)PTR_FUN_8ced32d8)(param_1,uVar3);
  if (((iVar2 != 1) && (iVar2 = (*(code *)PTR_FUN_8ced32d8)(param_1,uVar1), iVar2 == 1)) &&
     (iVar2 = FUN_8ced2660(param_1), iVar2 != 0)) {
    (*(code *)PTR_FUN_8ced32dc)(param_1,uVar3,1);
    return;
  }
  return;
}


/* Address: 8ced31ec */

void FUN_8ced31ec(int param_1,int param_2,int *param_3)

{
  int iVar1;
  int iVar2;
  undefined4 local_1c;
  undefined4 local_18 [2];
  
  iVar1 = DAT_8ced32ce + param_1 + (int)DAT_8ced32cc;
  iVar2 = *(int *)(DAT_8ced32d2 + iVar1) * (int)DAT_8ced32d6 + DAT_8ced32d4 + param_1;
  *param_3 = iVar2 + 0x24;
  *(undefined4 *)(iVar2 + 0x20) = 1;
  *(int *)(iVar1 + 0x30) = param_2;
  FUN_8ced3320(param_1,(undefined4 *)(param_2 + 8),&local_1c,local_18);
  *(undefined4 *)*param_3 = *(undefined4 *)(param_2 + 8);
  *(undefined4 *)(*param_3 + 4) = *(undefined4 *)(param_2 + 0xc);
  *(undefined4 *)(*param_3 + 8) = *(undefined4 *)(param_2 + 0x10);
  *(undefined4 *)(*param_3 + 0xc) = *(undefined4 *)(param_2 + 0x14);
  *(undefined4 *)(*param_3 + 0x10) = *(undefined4 *)(param_2 + 0x20);
  *(undefined4 *)(*param_3 + 0x14) = local_1c;
  *(undefined4 *)(*param_3 + 0x18) = local_18[0];
  *(undefined4 *)(*param_3 + 0x1c) = *(undefined4 *)(param_1 + 0x38);
  *(undefined4 *)(*param_3 + 0x20) = *(undefined4 *)(param_2 + 4);
  *(undefined4 *)(*param_3 + 0x24) = 0;
  *(undefined4 *)(*param_3 + 0x28) = 0;
  return;
}


/* Address: 8ced3288 */

undefined4 callback_candidate_8ced3288(int param_1,int param_2)

{
  undefined *puVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  undefined1 auStack_18 [4];
  int local_14;
  
  local_14 = param_2;
  (*(code *)PTR_FUN_8ced32e0)(auStack_18);
  puVar1 = PTR_FUN_8ced32e4;
  iVar3 = DAT_8ced32ce + param_1 + (int)DAT_8ced32cc;
  iVar4 = DAT_8ced32d4 + param_1 + *(int *)(DAT_8ced32d2 + iVar3) * (int)DAT_8ced32d6;
  piVar5 = (int *)(iVar4 + 0x20);
  if (*piVar5 == 1) {
    if (iVar4 + 0x24 == local_14) {
      *piVar5 = 0;
      FUN_8ced3628(*(undefined4 *)(iVar3 + 0x30));
      (*(code *)puVar1)(auStack_18);
      return 0;
    }
    (*(code *)PTR_FUN_8ced32e4)(auStack_18);
    uVar2 = DAT_8ced343c;
  }
  else {
    (*(code *)PTR_FUN_8ced32e4)(auStack_18);
    uVar2 = DAT_8ced32e8;
  }
  uVar2 = (*(code *)PTR_FUN_8ced3440)(param_1,uVar2);
  return uVar2;
}


/* Address: 8ced3320 */

void FUN_8ced3320(int param_1,int param_2,int *param_3,undefined4 *param_4)

{
  code *pcVar1;
  int iVar2;
  int local_34;
  undefined4 local_30;
  undefined4 *local_2c;
  int *local_28;
  int local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  
  local_20 = *(undefined4 *)(param_2 + 0x10);
  local_1c = *(undefined4 *)(param_2 + 0x1c);
  local_18 = *(undefined4 *)(param_2 + 0x20);
  local_14 = *(undefined4 *)(param_2 + 0x24);
  local_10 = *(undefined4 *)(param_2 + 0x28);
  local_c = *(undefined4 *)(param_2 + 0x2c);
  local_8 = *(undefined4 *)(param_2 + 0x14);
  local_2c = param_4;
  local_28 = param_3;
  local_24 = param_1;
  (*(code *)PTR_callback_candidate_8ced4a7a_8ced3444)(&local_20,&local_34,&local_30);
  pcVar1 = DAT_8ced3448;
  iVar2 = local_24 + DAT_8ced3434;
  if (local_34 <= *(int *)(iVar2 + 0x38)) {
    *(int *)(iVar2 + 0x38) = local_34;
    *(undefined4 *)(iVar2 + 0x3c) = local_30;
    (*pcVar1)();
  }
  *local_28 = local_34 - *(int *)(iVar2 + 0x38);
  *local_2c = local_30;
  return;
}


/* Address: 8ced3390 */

void FUN_8ced3390(int param_1)

{
  int iVar1;
  undefined *puVar2;
  undefined2 uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  undefined1 auStack_20 [4];
  int local_1c;
  
  (*(code *)PTR_FUN_8ced344c)(auStack_20);
  iVar5 = param_1 + DAT_8ced3438 + (int)DAT_8ced3436;
  iVar7 = *(int *)(iVar5 + 0xc) * *(int *)(iVar5 + 0x10);
  iVar8 = *(int *)(iVar5 + 0x14) * *(int *)(iVar5 + 0x18);
  iVar1 = *(int *)(iVar5 + 0x20) * *(int *)(iVar5 + 0x24) * 3;
  iVar6 = *(int *)(iVar5 + 0x1c);
  local_1c = iVar7 * 2;
  uVar3 = (undefined2)*(int *)(iVar5 + 0xc);
  *(undefined2 *)(iVar5 + 0x54) = uVar3;
  *(undefined2 *)(iVar5 + 100) = uVar3;
  uVar3 = (undefined2)*(int *)(iVar5 + 0x14);
  *(undefined2 *)(iVar5 + 0x56) = uVar3;
  *(undefined2 *)(iVar5 + 0x66) = uVar3;
  *(int *)(iVar5 + 0x48) = iVar6;
  *(int *)(iVar5 + 0x4c) = iVar7 + iVar6;
  *(int *)(iVar5 + 0x50) = local_1c + iVar6;
  iVar4 = local_1c + iVar8;
  *(int *)(iVar5 + 0x60) = iVar4 + iVar6;
  iVar4 = iVar4 + iVar8;
  *(int *)(iVar5 + 0x58) = iVar4 + iVar6;
  *(int *)(iVar5 + 0x5c) = iVar7 + iVar4 + iVar6;
  iVar6 = *(int *)(iVar5 + 0x2c);
  for (iVar4 = 0; puVar2 = PTR_FUN_8ced3570, iVar4 < *(int *)(iVar5 + 0x28); iVar4 = iVar4 + 1) {
    *(undefined4 *)(iVar5 + 0x68 + iVar4 * 0x38) = 0;
    *(int *)(iVar4 * 0x38 + iVar5 + 0x6c) = iVar4 * ((int)(iVar1 + (uint)(iVar1 < 0)) >> 1) + iVar6;
  }
  *(undefined4 *)(iVar5 + 0x40) = 0;
  *(undefined4 *)(iVar5 + 0x44) = 1;
  *(undefined4 *)(iVar5 + 0x38) = 0;
  (*(code *)puVar2)(auStack_20);
  return;
}


/* Address: 8ced3492 */

undefined4 FUN_8ced3492(int param_1,int param_2,int param_3,int *param_4)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  
  iVar3 = DAT_8ced356e + param_1 + (int)DAT_8ced356c;
  iVar1 = FUN_8ced35c4();
  *param_4 = iVar1;
  if (iVar1 == 0) {
    uVar2 = 0xffffffff;
  }
  else {
    (*DAT_8ced3574)();
    if ((*(int *)(param_2 + 0x18) == 1) || (*(int *)(param_2 + 0x18) == 2)) {
      *(uint *)(iVar3 + 0x40) = *(uint *)(iVar3 + 0x40) ^ 1;
      *(uint *)(iVar3 + 0x44) = *(uint *)(iVar3 + 0x44) ^ 1;
    }
    (*DAT_8ced3574)(iVar3 + 0x48);
    (*DAT_8ced3574)();
    uVar2 = 0;
    *(undefined4 *)(param_3 + 0x20) = *(undefined4 *)(*param_4 + 4);
    *(int *)(param_3 + 0x24) = *param_4 + 8;
    *(undefined4 *)(param_3 + 0x28) = 0;
    *(undefined4 *)(param_3 + 0x2c) = 0;
  }
  return uVar2;
}


/* Address: 8ced352c */

void FUN_8ced352c(int param_1)

{
  *(undefined4 *)(param_1 + DAT_8ced356e + (int)DAT_8ced356c + 0x38) = 1;
  return;
}


/* Address: 8ced353c */

int FUN_8ced353c(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined1 auStack_18 [4];
  int local_14;
  
  local_14 = param_1;
  (*(code *)PTR_FUN_8ced3578)(auStack_18);
  iVar4 = 0;
  iVar3 = 0;
  iVar1 = local_14 + DAT_8ced356e + (int)DAT_8ced356c;
  iVar2 = 0;
  while (iVar2 < *(int *)(iVar1 + 0x28)) {
    iVar5 = iVar2 * 0x38;
    if (*(int *)(iVar1 + 0x68 + iVar5) == 2) {
      iVar4 = iVar4 + 1;
    }
    iVar2 = iVar2 + 1;
    if (*(int *)(iVar5 + iVar1 + 0x68) == 0) {
      iVar3 = iVar3 + 1;
    }
  }
  if ((*(int *)(iVar1 + 0x38) == 1) && (iVar3 == *(int *)(iVar1 + 0x28))) {
    iVar4 = -1;
  }
  (*(code *)PTR_FUN_8ced367c)(auStack_18);
  return iVar4;
}


/* Address: 8ced35c4 */

int FUN_8ced35c4(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined1 auStack_14 [4];
  int local_10;
  
  local_10 = param_1;
  (*(code *)PTR_FUN_8ced3680)(auStack_14);
  iVar3 = 0;
  iVar1 = local_10 + DAT_8ced3678 + (int)DAT_8ced3676;
  iVar2 = 0;
  do {
    if (*(int *)(iVar1 + 0x28) <= iVar2) {
LAB_8ced3616:
      (*(code *)PTR_FUN_8ced367c)(auStack_14);
      return iVar3;
    }
    iVar4 = iVar2 * 0x38;
    if (*(int *)(iVar1 + 0x68 + iVar4) == 0) {
      *(undefined4 *)(iVar1 + 0x68 + iVar4) = 1;
      iVar3 = iVar1 + 0x68 + iVar4;
      goto LAB_8ced3616;
    }
    iVar2 = iVar2 + 1;
  } while( true );
}


/* Address: 8ced3628 */

void FUN_8ced3628(undefined4 *param_1)

{
  *param_1 = 0;
  return;
}


/* Address: 8ced362e */

void FUN_8ced362e(undefined4 *param_1)

{
  *param_1 = 2;
  return;
}


/* Address: 8ced3634 */

int FUN_8ced3634(int param_1,undefined4 *param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  undefined1 auStack_28 [4];
  undefined4 *local_24;
  
  local_24 = param_2;
  (*(code *)PTR_FUN_8ced3680)(auStack_28);
  iVar3 = 0;
  iVar4 = 0;
  iVar2 = param_1 + DAT_8ced3678 + (int)DAT_8ced3676;
  *local_24 = 0;
  for (iVar5 = 0; iVar5 < *(int *)(iVar2 + 0x28); iVar5 = iVar5 + 1) {
    iVar6 = iVar5 * 0x38;
    if (*(int *)(iVar2 + 0x68 + iVar6) == 2) {
      iVar3 = iVar3 + 1;
      iVar1 = FUN_8ced36f4(iVar4,iVar2 + 0x68 + iVar6);
      if (iVar1 != 0) {
        iVar4 = iVar2 + 0x68 + iVar6;
      }
    }
  }
  if ((iVar3 < 2) && ((iVar3 != 1 || (*(int *)(iVar2 + 0x38) != 1)))) {
    iVar4 = 0;
  }
  if (iVar3 == 1) {
    *local_24 = 1;
  }
  (*(code *)PTR_FUN_8ced3770)(auStack_28);
  return iVar4;
}


/* Address: 8ced36f4 */

undefined4 FUN_8ced36f4(int param_1,int param_2)

{
  int iVar1;
  
  if (param_1 != 0) {
    iVar1 = FUN_8ced371e(param_1 + 8,param_2 + 8,param_1 + 8);
    if (iVar1 == 0) {
      return 0;
    }
  }
  return 1;
}


/* Address: 8ced371e */

undefined4 FUN_8ced371e(int param_1,int param_2)

{
  if ((*(int *)(param_1 + 0x20) <= *(int *)(param_2 + 0x20)) &&
     ((*(int *)(param_1 + 0x20) < *(int *)(param_2 + 0x20) ||
      ((*(int *)(param_1 + 0x24) <= *(int *)(param_2 + 0x24) &&
       ((*(int *)(param_1 + 0x24) < *(int *)(param_2 + 0x24) ||
        ((*(int *)(param_1 + 0x28) <= *(int *)(param_2 + 0x28) &&
         ((*(int *)(param_1 + 0x28) < *(int *)(param_2 + 0x28) ||
          ((*(int *)(param_1 + 0x2c) <= *(int *)(param_2 + 0x2c) &&
           ((*(int *)(param_1 + 0x2c) < *(int *)(param_2 + 0x2c) ||
            (*(int *)(param_1 + 0x14) <= *(int *)(param_2 + 0x14))))))))))))))))) {
    return 0;
  }
  return 1;
}


/* Address: 8ced379c */

void callback_candidate_8ced379c(int param_1,int param_2)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = (*(code *)PTR_FUN_8ced3884)(param_1);
  if (iVar1 != 0) {
    (*(code *)PTR_FUN_8ced388c)(0,DAT_8ced3890);
    return;
  }
  if (param_2 == 0) {
    uVar2 = 0;
  }
  else if (*(int *)(param_1 + 0x48) == 0) {
    uVar2 = 1;
  }
  else {
    uVar2 = 2;
  }
  *(int *)(param_1 + 0x48) = param_2;
  FUN_8ced37e8(param_1,uVar2);
  return;
}


/* Address: 8ced37e8 */

undefined4 FUN_8ced37e8(int param_1,int param_2)

{
  undefined4 uVar1;
  
  if (((param_2 == 2) && (*(int *)(param_1 + 0x40) != 4)) ||
     ((*(int *)(param_1 + 0x44) != 3 && (*(int *)(param_1 + 0x44) != 4)))) {
    uVar1 = 0;
  }
  else {
    (*(code *)PTR_callback_candidate_8ced4ca0_8ced3894)(param_1,param_2);
    uVar1 = FUN_8ced382a(param_1,param_2);
  }
  return uVar1;
}


/* Address: 8ced382a */

int FUN_8ced382a(undefined4 param_1)

{
  int iVar1;
  
  iVar1 = (*(code *)PTR_callback_candidate_8ceda5fc_8ced3898)(7,8,param_1,0);
  if (iVar1 != 0) {
    return iVar1;
  }
  return 0;
}


/* Address: 8ced3858 */

undefined4 callback_candidate_8ced3858(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = (*(code *)PTR_FUN_8ced3884)();
  if (iVar1 != 0) {
    uVar2 = (*(code *)PTR_FUN_8ced388c)(0,DAT_8ced389c);
    return uVar2;
  }
  *(undefined4 *)(param_1 + 0x44) = 3;
  return 0;
}


/* Address: 8ced38a0 */

void FUN_8ced38a0(void)

{
  if (*(int *)PTR_DAT_8ced3970 != DAT_8ced3974) {
    (*(code *)PTR_FUN_8ced397c)(0,DAT_8ced3978);
    return;
  }
  return;
}


/* Address: 8ced38ba */

void callback_candidate_8ced478e(void)

{
  (*DAT_8ced3980)();
  return;
}


/* Address: 8ced38c4 */

void callback_candidate_8ced38c4(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar4 = 0;
  iVar3 = *(int *)(DAT_8ced396a + DAT_8ced3984);
  iVar2 = (int)DAT_8ced396c;
  iVar1 = *(int *)(DAT_8ced396a + 4 + DAT_8ced3984);
  if (0 < iVar3) {
    do {
      FUN_8ced3900(iVar1);
      iVar4 = iVar4 + 1;
      iVar1 = iVar1 + iVar2;
    } while (iVar4 < iVar3);
  }
  callback_candidate_8ced3d10();
  return;
}


/* Address: 8ced3900 */

void FUN_8ced3900(int param_1)

{
  int iVar1;
  
  iVar1 = *(int *)(param_1 + 0x40);
  if (iVar1 == 1) {
LAB_8ced3922:
    if (((iVar1 != 2) && (iVar1 != 3)) && (iVar1 != 4)) goto LAB_8ced3938;
  }
  else if ((iVar1 != 2) && (iVar1 != 3)) {
    if (iVar1 != 4) {
      return;
    }
    goto LAB_8ced3922;
  }
  FUN_8ced39a8(param_1);
LAB_8ced3938:
  iVar1 = *(int *)(param_1 + 0x40);
  if (iVar1 == 1) {
    iVar1 = FUN_8ced39ae(param_1);
  }
  else if (iVar1 == 2) {
    iVar1 = FUN_8ced39d2(param_1);
  }
  else if (iVar1 == 3) {
    iVar1 = FUN_8ced3b40(param_1);
  }
  else if (iVar1 == 4) {
    iVar1 = FUN_8ced3b90(param_1);
  }
  else if (iVar1 == 6) {
    iVar1 = FUN_8ced3bca(param_1);
  }
  *(int *)(param_1 + 0x40) = iVar1;
  return;
}


/* Address: 8ced39a8 */

void FUN_8ced39a8(void)

{
  (*(code *)PTR_callback_candidate_8ceda59e_8ced3a90)(2);
  return;
}


/* Address: 8ced39ae */

undefined4 FUN_8ced39ae(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  uVar2 = *(undefined4 *)(param_1 + 0x40);
  iVar1 = *(int *)(param_1 + 0x44);
  if ((((iVar1 == 2) || (iVar1 == 3)) || (iVar1 == 4)) || (iVar1 == 6)) {
    uVar2 = 2;
  }
  return uVar2;
}


/* Address: 8ced39d2 */

undefined4 FUN_8ced39d2(int param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  
  uVar3 = *(undefined4 *)(param_1 + 0x40);
  iVar2 = *(int *)(param_1 + 0x44);
  iVar1 = FUN_8ced3a2e(param_1);
  if (iVar1 != 0) {
    FUN_8ced3a7e(param_1);
    if (iVar2 == 2) {
      uVar3 = 2;
    }
    else if (iVar2 == 3) {
      uVar3 = 3;
    }
    else if ((iVar2 == 4) || (iVar2 == 6)) {
      iVar1 = FUN_8ced3bd0(param_1);
      if (iVar1 == 0) {
        uVar3 = 3;
      }
      else {
        FUN_8ced405e(param_1);
        uVar3 = 4;
      }
    }
  }
  return uVar3;
}


/* Address: 8ced3a2e */

undefined4 FUN_8ced3a2e(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  
  iVar1 = (*(code *)PTR_FUN_8ced3a94)(5);
  if (iVar1 == 0) {
    iVar1 = 1;
  }
  else {
    iVar1 = (*(code *)PTR_FUN_8ced3a98)(param_1,6);
  }
  iVar2 = (*(code *)PTR_FUN_8ced3a94)(param_1,6);
  if (iVar2 == 0) {
    iVar2 = 1;
  }
  else {
    iVar2 = (*(code *)PTR_FUN_8ced3a98)(param_1,7);
  }
  if ((iVar1 == 0) || (iVar2 == 0)) {
    uVar3 = 0;
  }
  else {
    uVar3 = 1;
  }
  return uVar3;
}


/* Address: 8ced3a7e */

void FUN_8ced3a7e(undefined4 param_1)

{
  FUN_8ced3a9c();
  FUN_8ced3aec(param_1);
  return;
}


/* Address: 8ced3a9c */

void FUN_8ced3a9c(int param_1)

{
  int iVar1;
  
  iVar1 = DAT_8ced3b86 + param_1;
  if ((*(int *)(iVar1 + 0x18) == 0) && (*(int *)(iVar1 + 0x3c) == 2)) {
    (*(code *)PTR_callback_candidate_8ced43f8_8ced3b88)(param_1,0xf,1);
  }
  if ((*(int *)(iVar1 + 0x14) == 0) && (*(int *)(iVar1 + 0x3c) == 1)) {
    (*(code *)PTR_callback_candidate_8ced43f8_8ced3b88)(param_1,0xf,2);
    return;
  }
  return;
}


/* Address: 8ced3aec */

void FUN_8ced3aec(int param_1)

{
  undefined4 uVar1;
  byte bVar2;
  
  bVar2 = *(int *)(DAT_8ced3b86 + param_1 + 0x18) == 1;
  if (*(int *)(DAT_8ced3b86 + param_1 + 0x14) == 1) {
    bVar2 = bVar2 | 2;
  }
  if (bVar2 == 1) {
    uVar1 = 1;
  }
  else if (bVar2 == 2) {
    uVar1 = 2;
  }
  else if (bVar2 == 3) {
    uVar1 = (*(code *)PTR_FUN_8ced3b8c)(param_1,0x19);
  }
  else {
    uVar1 = 3;
  }
  (*(code *)PTR_callback_candidate_8ced43f8_8ced3b88)(param_1,0x19,uVar1);
  return;
}


/* Address: 8ced3b40 */

undefined4 FUN_8ced3b40(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  uVar2 = *(undefined4 *)(param_1 + 0x40);
  iVar1 = *(int *)(param_1 + 0x44);
  if (iVar1 == 2) {
    uVar2 = 2;
  }
  else if (iVar1 == 3) {
    uVar2 = 3;
  }
  else if (((iVar1 == 4) || (iVar1 == 6)) && (iVar1 = FUN_8ced3bd0(param_1), iVar1 != 0)) {
    FUN_8ced405e(param_1);
    uVar2 = 4;
  }
  return uVar2;
}


/* Address: 8ced3b90 */

undefined4 FUN_8ced3b90(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = FUN_8ced3bf6();
  if (iVar1 != 0) {
    return *(undefined4 *)(param_1 + 0x40);
  }
  uVar2 = *(undefined4 *)(param_1 + 0x40);
  iVar1 = *(int *)(param_1 + 0x44);
  if ((((iVar1 != 2) && (iVar1 != 3)) && (iVar1 != 4)) && (iVar1 == 6)) {
    uVar2 = 6;
  }
  return uVar2;
}


/* Address: 8ced3bca */

undefined4 FUN_8ced3bca(int param_1)

{
  return *(undefined4 *)(param_1 + 0x40);
}


/* Address: 8ced3bd0 */

undefined4 FUN_8ced3bd0(int param_1)

{
  if (((*(int *)(DAT_8ced3c88 + param_1 + 0x38) != 0) &&
      (*(int *)(DAT_8ced3c88 + param_1 + 0x14) != 0)) &&
     (*(int *)(param_1 + DAT_8ced3c8a + 0x6c) == 0)) {
    return 0;
  }
  return 1;
}


/* Address: 8ced3bf6 */

undefined4 FUN_8ced3bf6(undefined4 param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = FUN_8ced3c1a();
  if ((iVar1 == 0) && (iVar1 = FUN_8ced3c68(param_1), iVar1 == 0)) {
    return 0;
  }
  uVar2 = FUN_8ced3ce8(param_1);
  return uVar2;
}


/* Address: 8ced3c1a */

undefined4 FUN_8ced3c1a(int param_1)

{
  int iVar1;
  int local_14;
  undefined4 local_10;
  undefined4 local_c;
  
  iVar1 = *(int *)(DAT_8ced3c88 + param_1 + 0x50);
  local_c = *(undefined4 *)(DAT_8ced3c88 + param_1 + 0x54);
  if (((iVar1 != -4) && ((*(code *)PTR_FUN_8ced3c8c)(&local_14,&local_10), -1 < local_14)) &&
     (iVar1 = (*(code *)PTR_callback_candidate_8ced65f0_8ced3c90)(local_14,local_10,iVar1,local_c),
     iVar1 == 0)) {
    return 1;
  }
  return 0;
}


/* Address: 8ced3c68 */

uint FUN_8ced3c68(int param_1)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  
  if ((*(int *)(DAT_8ced3c88 + param_1 + 0x18) == 0) &&
     (*(int *)(DAT_8ced3c88 + param_1 + 0x14) == 0)) {
    uVar4 = 1;
  }
  else {
    uVar1 = (*(code *)PTR_FUN_8ced3d8c)(param_1,6);
    uVar2 = (*(code *)PTR_FUN_8ced3d8c)(param_1,7);
    iVar3 = (*(code *)PTR_FUN_8ced3d90)(param_1,0x19);
    uVar4 = uVar2;
    if ((iVar3 != 1) && (uVar4 = uVar1, iVar3 != 2)) {
      if (iVar3 == 3) {
        uVar4 = uVar1 | uVar2;
      }
      else {
        uVar4 = 0;
        if (iVar3 == 0) {
          uVar4 = uVar1 & uVar2;
        }
      }
    }
  }
  return uVar4;
}


/* Address: 8ced3ce8 */

int FUN_8ced3ce8(int param_1)

{
  int iVar1;
  
  iVar1 = FUN_8ced40e4();
  if (iVar1 != 0) {
    return iVar1;
  }
  *(undefined4 *)(param_1 + 0x44) = 6;
  return 0;
}


/* Address: 8ced3d10 */

/* WARNING: Type propagation algorithm not settling */
/* WARNING: Restarted to delay deadcode elimination for space: stack */

undefined4 callback_candidate_8ced3d10(void)

{
  undefined *puVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  undefined1 auStack_28 [4];
  int *local_24;
  int *local_20;
  int local_1c [4];
  
  (*(code *)PTR_FUN_8ced3d94)(auStack_28);
  puVar1 = PTR_FUN_8ced3e44;
  local_20 = local_1c + 2;
  local_24 = local_1c + 1;
  iVar5 = 0;
  local_1c[0] = 0;
  local_1c[1] = 0;
  local_1c[2] = 0;
  iVar4 = *(int *)(DAT_8ced3d86 + DAT_8ced3d98);
  iVar2 = *(int *)(DAT_8ced3d86 + 4 + DAT_8ced3d98);
  if (0 < iVar4) {
    do {
      iVar3 = *(int *)(iVar2 + 0x40);
      if (iVar3 == 0) {
        iVar3 = 0;
      }
      else if ((iVar3 == 6) || (iVar3 < 0)) {
        iVar3 = 2;
      }
      else {
        iVar3 = 1;
      }
      iVar5 = iVar5 + 1;
      local_1c[iVar3] = local_1c[iVar3] + 1;
      iVar2 = iVar2 + DAT_8ced3d88;
    } while (iVar5 < iVar4);
  }
  if (local_1c[1] == 0) {
    if (local_1c[2] == 0) {
      uVar6 = 0;
    }
    else {
      uVar6 = 2;
    }
  }
  else {
    uVar6 = 1;
  }
  *(undefined4 *)(DAT_8ced3e3c + DAT_8ced3d98) = uVar6;
  (*(code *)puVar1)(auStack_28);
  return uVar6;
}


/* Address: 8ced3dc2 */

undefined4 callback_candidate_8ced3dc2(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = FUN_8ced3dfe(param_1);
  if (iVar1 == 0) {
    iVar1 = FUN_8ced3e0e();
    if (iVar1 != 0) {
      uVar2 = FUN_8ced3e54(iVar1,param_1,param_2);
      return uVar2;
    }
  }
  else {
    (*(code *)PTR_FUN_8ced3e4c)(0,DAT_8ced3e48);
  }
  return 0;
}


/* Address: 8ced3dfe */

undefined4 FUN_8ced3dfe(int param_1)

{
  if (*(int *)(param_1 + 4) == 0) {
    return 0xffffffff;
  }
  return 0;
}


/* Address: 8ced3e0e */

int FUN_8ced3e0e(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = *(int *)(DAT_8ced3e3e + DAT_8ced3e50);
  iVar1 = *(int *)(DAT_8ced3e3e + 4 + DAT_8ced3e50);
  iVar2 = 0;
  if (0 < iVar3) {
    do {
      if (*(int *)(iVar1 + 0x40) == 0) {
        return iVar1;
      }
      iVar2 = iVar2 + 1;
      iVar1 = iVar1 + DAT_8ced3e40;
    } while (iVar2 < iVar3);
  }
  return 0;
}


/* Address: 8ced3e54 */

int FUN_8ced3e54(int param_1,int param_2,undefined4 param_3)

{
  code *pcVar1;
  int iVar2;
  
  (*(code *)PTR_FUN_8ced3f8c)(param_1,0,(int)DAT_8ced3f7c);
  pcVar1 = DAT_8ced3f90;
  *(uint *)(param_2 + 4) = *(int *)(param_2 + 4) + 0x1fU & 0xffffffe0;
  (*pcVar1)();
  *(undefined4 *)(param_1 + 0x3c) = param_3;
  *(undefined4 *)(param_1 + 0x40) = 1;
  *(undefined4 *)(param_1 + 0x44) = 1;
  *(undefined4 *)(param_1 + 0x48) = 0;
  FUN_8ced3ef8(param_1 + 0x4c);
  FUN_8ced3f22(DAT_8ced3f7e + param_1);
  (*(code *)PTR_FUN_8ced3f94)(DAT_8ced3f80 + param_1);
  (*(code *)PTR_FUN_8ced3f9c)(DAT_8ced3f82 + param_1,DAT_8ced3f98,0x2f);
  (*(code *)PTR_callback_candidate_8ced462e_8ced3fa0)(param_1,DAT_8ced3f84 + param_1);
  (*(code *)PTR_callback_candidate_8ced97b0_8ced3fa4)(param_1,DAT_8ced3f86 + param_1,param_2);
  (*(code *)PTR_callback_candidate_8ceda304_8ced3fa8)
            (param_1,DAT_8ced3f88 + param_1,param_2,param_3);
  iVar2 = FUN_8ced3fea(param_1);
  if (iVar2 != 0) {
    *(undefined4 *)(param_1 + 0x40) = 0;
    *(undefined4 *)(param_1 + 0x44) = 0;
    param_1 = 0;
  }
  return param_1;
}


/* Address: 8ced3ef8 */

void FUN_8ced3ef8(undefined4 *param_1)

{
  (*(code *)PTR_FUN_8ced3f8c)(0,0x10);
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  param_1[3] = 0;
  param_1[4] = 0;
  param_1[5] = 0;
  param_1[6] = 0;
  param_1[7] = 1;
  return;
}


/* Address: 8ced3f22 */

void FUN_8ced3f22(undefined4 *param_1)

{
  (*(code *)PTR_FUN_8ced3f8c)(0,0x10);
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  param_1[3] = 0;
  return;
}


/* Address: 8ced3f42 */

void callback_candidate_8ced3f42(int param_1,int param_2,undefined4 param_3)

{
  undefined *puVar1;
  code *pcVar2;
  int *piVar3;
  
  puVar1 = PTR_FUN_8ced3fac;
  piVar3 = (int *)(DAT_8ced3f7e + param_1);
  *piVar3 = *piVar3 + param_2;
  pcVar2 = (code *)(*(code *)puVar1)(param_1,0x24);
  if (pcVar2 != (code *)0x0) {
    (*pcVar2)(param_1,param_3,piVar3);
  }
  return;
}


/* Address: 8ced3fb0 */

void callback_candidate_8ced3fb0(int param_1,int param_2,undefined4 param_3)

{
  undefined *puVar1;
  code *pcVar2;
  int iVar3;
  
  puVar1 = PTR_FUN_8ced40c4;
  iVar3 = DAT_8ced40c0 + param_1;
  *(int *)(iVar3 + 4) = *(int *)(iVar3 + 4) + param_2;
  pcVar2 = (code *)(*(code *)puVar1)(param_1,0x25);
  if (pcVar2 != (code *)0x0) {
    (*pcVar2)(param_1,param_3,iVar3);
  }
  return;
}


/* Address: 8ced3fea */

void FUN_8ced3fea(void)

{
  (*(code *)PTR_callback_candidate_8ceda59e_8ced40c8)(3);
  return;
}


/* Address: 8ced3ff0 */

undefined4 callback_candidate_8ced3ff0(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 in_PR;
  
  iVar1 = (*(code *)PTR_FUN_8ced40cc)();
  if (iVar1 != 0) {
    uVar2 = (*(code *)PTR_FUN_8ced40d4)(0,DAT_8ced40d0);
    return uVar2;
  }
  uVar2 = (*(code *)PTR_callback_candidate_8ceda59e_8ced40c8)(4,in_PR);
  *(undefined4 *)(param_1 + 0x40) = 0;
  *(undefined4 *)(param_1 + 0x44) = 0;
  return uVar2;
}


/* Address: 8ced4032 */

undefined4 callback_candidate_8ced4032(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = (*(code *)PTR_FUN_8ced40cc)();
  if (iVar1 != 0) {
    uVar2 = (*(code *)PTR_FUN_8ced40d4)(0,DAT_8ced40d8);
    return uVar2;
  }
  *(undefined4 *)(param_1 + 0x44) = 4;
  return 0;
}


/* Address: 8ced405e */

int FUN_8ced405e(void)

{
  int iVar1;
  
  iVar1 = (*(code *)PTR_callback_candidate_8ceda5fc_8ced40dc)(7,6,0,0);
  if (iVar1 != 0) {
    return iVar1;
  }
  return 0;
}


/* Address: 8ced4084 */

int callback_candidate_8ced4084(int param_1)

{
  int iVar1;
  
  iVar1 = (*(code *)PTR_FUN_8ced40cc)();
  if (iVar1 != 0) {
    iVar1 = (*(code *)PTR_FUN_8ced40d4)(0,DAT_8ced40e0);
    return iVar1;
  }
  if ((*(int *)(param_1 + 0x40) != 1) && (iVar1 = FUN_8ced40e4(param_1), iVar1 != 0)) {
    return iVar1;
  }
  return 0;
}


/* Address: 8ced40e4 */

int FUN_8ced40e4(int param_1)

{
  int iVar1;
  
  if (*(int *)(param_1 + 0x40) == 4) {
    iVar1 = (*(code *)PTR_callback_candidate_8ceda5fc_8ced41e8)(param_1,7,0,0);
    if (iVar1 != 0) {
      return iVar1;
    }
  }
  *(undefined4 *)(param_1 + 0x40) = 1;
  *(undefined4 *)(param_1 + 0x44) = 1;
  return 0;
}


/* Address: 8ced41fc */

void callback_candidate_8ced41fc(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  
  iVar1 = (*(code *)PTR_FUN_8ced4314)(param_1);
  if (iVar1 != 0) {
    (*(code *)PTR_FUN_8ced431c)(0,DAT_8ced4318);
    return;
  }
  (*(code *)PTR_callback_candidate_8ceda5fc_8ced4320)(param_1,0,9,param_2,0);
  return;
}


/* Address: 8ced4232 */

void callback_candidate_8ced4232(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  
  iVar1 = (*(code *)PTR_FUN_8ced4314)(param_1,param_3);
  if (iVar1 != 0) {
    (*(code *)PTR_FUN_8ced431c)(0,DAT_8ced4324);
    return;
  }
  (*(code *)PTR_callback_candidate_8ceda5fc_8ced4320)(param_1,0,10,param_2,param_3);
  return;
}


/* Address: 8ced429c */

void callback_candidate_8ced429c(undefined4 param_1,undefined4 *param_2)

{
  int iVar1;
  
  *param_2 = 0;
  iVar1 = (*(code *)PTR_FUN_8ced4314)(param_1);
  if (iVar1 != 0) {
    (*(code *)PTR_FUN_8ced431c)(0,DAT_8ced432c);
    return;
  }
  (*(code *)PTR_callback_candidate_8ceda5fc_8ced4320)(param_1,6,0xb,param_2,0);
  return;
}


/* Address: 8ced42d8 */

void callback_candidate_8ced42d8(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  
  iVar1 = (*(code *)PTR_FUN_8ced4314)(param_1);
  if (iVar1 != 0) {
    (*(code *)PTR_FUN_8ced431c)(0,DAT_8ced4330);
    return;
  }
  (*(code *)PTR_callback_candidate_8ceda5fc_8ced4320)(param_1,6,0xc,param_2,0);
  return;
}


/* Address: 8ced4334 */

void callback_candidate_8ced4334(void)

{
  (*(code *)PTR_callback_candidate_8ced3d10_8ced4440)();
  return;
}


/* Address: 8ced4342 */

undefined4 callback_candidate_8ced4342(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = (*(code *)PTR_FUN_8ced4444)();
  if (iVar1 != 0) {
    uVar2 = (*(code *)PTR_FUN_8ced444c)(0,DAT_8ced4448);
    return uVar2;
  }
  return *(undefined4 *)(param_1 + 0x40);
}


/* Address: 8ced436a */

undefined4 callback_candidate_8ced436a(int param_1,int param_2,undefined4 param_3)

{
  int iVar1;
  undefined4 uVar2;
  
  if (param_1 == 0) {
    FUN_8ced43b8(param_2,param_3);
    *(undefined4 *)(DAT_8ced4450 + param_2 * 4) = param_3;
  }
  else {
    iVar1 = (*(code *)PTR_FUN_8ced4444)(param_1);
    if (iVar1 != 0) {
      uVar2 = (*(code *)PTR_FUN_8ced444c)(0,DAT_8ced4454);
      return uVar2;
    }
    callback_candidate_8ced43f8(param_1,param_2,param_3);
  }
  return 0;
}


/* Address: 8ced43b8 */

void FUN_8ced43b8(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar1 = *(int *)(DAT_8ced443c + DAT_8ced4458);
  iVar2 = *(int *)(DAT_8ced443c + 4 + DAT_8ced4458);
  iVar3 = 0;
  if (0 < iVar1) {
    do {
      if (0 < *(int *)(iVar2 + 0x40)) {
        callback_candidate_8ced43f8(iVar2,param_1,param_2);
      }
      iVar3 = iVar3 + 1;
    } while (iVar3 < iVar1);
  }
  return;
}


/* Address: 8ced43f8 */

void callback_candidate_8ced43f8(int param_1,int param_2,undefined4 param_3)

{
  int iVar1;
  
  iVar1 = FUN_8ced4460(param_1,param_2);
  if ((iVar1 != 0) &&
     (*(undefined4 *)(param_2 * 4 + DAT_8ced443e + param_1) = param_3, param_2 == 0xf)) {
    (*(code *)PTR_FUN_8ced445c)(param_1);
    return;
  }
  return;
}


/* Address: 8ced4460 */

undefined4 FUN_8ced4460(undefined4 param_1,int param_2,int param_3)

{
  int iVar1;
  
  if ((((param_2 != 6) || (param_3 != 1)) ||
      (iVar1 = (*(code *)PTR_FUN_8ced4574)(param_1,3), iVar1 != 0)) &&
     (((param_2 != 5 || (param_3 != 1)) ||
      (iVar1 = (*(code *)PTR_FUN_8ced4574)(param_1,2), iVar1 != 0)))) {
    return 1;
  }
  return 0;
}


/* Address: 8ced44fc */

undefined4 FUN_8ced44fc(int param_1,int param_2)

{
  return *(undefined4 *)(param_2 * 4 + DAT_8ced456e + param_1);
}


/* Address: 8ced4508 */

undefined4 callback_candidate_8ced4508(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = (*(code *)PTR_FUN_8ced457c)(param_1,param_2);
  if (iVar1 != 0) {
    uVar2 = (*(code *)PTR_FUN_8ced4584)(0,DAT_8ced4588);
    return uVar2;
  }
  (*DAT_8ced458c)();
  return 0;
}


/* Address: 8ced45c8 */

undefined4 callback_candidate_8ced45c8(int param_1,int param_2,undefined4 *param_3)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = (*(code *)PTR_FUN_8ced460c)(param_1);
  if (iVar1 != 0) {
    uVar2 = (*(code *)PTR_FUN_8ced4614)(0,DAT_8ced461c);
    return uVar2;
  }
  *param_3 = *(undefined4 *)(param_2 * DAT_8ced4606 + param_1 + DAT_8ced4608 + 8);
  return 0;
}


/* Address: 8ced4620 */

void FUN_8ced4620(undefined4 *param_1,undefined4 param_2)

{
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = param_2;
  return;
}


/* Address: 8ced462a */

void FUN_8ced462a(void)

{
  return;
}


/* Address: 8ced462e */

void callback_candidate_8ced462e(undefined4 param_1,undefined4 param_2,int param_3)

{
  double dVar1;
  undefined4 uVar2;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  FUN_8ced4a54(PTR_callback_candidate_8ced48ee_8ced4758,0);
  FUN_8ced4a54(param_2,PTR_callback_candidate_8ced4912_8ced475c,1);
  FUN_8ced4a54(param_2,0,2);
  FUN_8ced4a54(param_2,PTR_callback_candidate_8ced4944_8ced4760,3);
  FUN_8ced4a54(param_2,0,4);
  *(undefined4 *)(param_3 + 0x18) = 0;
  FUN_8ced46de(param_3 + 0x1c);
  uVar2 = DAT_8ced4764;
  *(undefined4 *)(param_3 + 0x38) = DAT_8ced4764;
  *(undefined4 *)(param_3 + 0x3c) = 1;
  *(undefined4 *)(param_3 + 0x40) = 0xfffffffb;
  *(undefined4 *)(param_3 + 0x44) = 1;
  *(undefined4 *)(param_3 + 0x48) = 0xfffffffb;
  *(undefined4 *)(param_3 + 0x4c) = 1;
  *(undefined4 *)(param_3 + 0x50) = 0xfffffffb;
  *(undefined4 *)(param_3 + 0x54) = uVar2;
  *(undefined4 *)(param_3 + 0x58) = 0;
  *(undefined4 *)(param_3 + 0x5c) = uVar2;
  *(undefined4 *)(param_3 + 0x60) = 0;
  *(undefined4 *)(param_3 + 100) = 0;
  *(int *)(param_3 + 0x68) = (int)DAT_8ced4756;
  *(undefined4 *)(param_3 + 0x6c) = 0;
  *(undefined4 *)(param_3 + 0x70) = 0;
  if (in_FPSCR_PR == '\0') {
    dVar1 = (double)CONCAT44(0xbf800000,param_1);
  }
  else {
    dVar1 = -(double)CONCAT44(0x3f800000,param_1);
  }
  *(undefined4 *)(param_3 + 0x74) = 1;
  *(undefined4 *)(param_3 + 0x78) = 100;
  uVar2 = (undefined4)((ulonglong)dVar1 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(param_3 + 0x7c) = uVar2;
  }
  else {
    *(double *)(param_3 + 0x7c) = dVar1;
  }
  *(undefined4 *)(param_3 + 0x80) = 0;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(param_3 + 0x84) = uVar2;
  }
  else {
    *(double *)(param_3 + 0x84) = dVar1;
  }
  *(undefined4 *)(param_3 + 0x88) = 0xffffffff;
  return;
}


/* Address: 8ced46de */

void FUN_8ced46de(undefined4 *param_1)

{
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  param_1[3] = 0;
  param_1[4] = 0;
  param_1[5] = 0;
  param_1[6] = 0;
  return;
}


/* Address: 8ced46f0 */

void FUN_8ced46f0(int param_1,int param_2)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  
  if (*(int *)(param_1 + 0x50) == -5) {
    *(int *)(param_1 + 0x50) = param_2;
    return;
  }
  iVar3 = param_2 - *(int *)(param_1 + 0x50);
  if (iVar3 != 0) {
    *(int *)(param_1 + 0x50) = param_2;
    iVar1 = iVar3;
    if (iVar3 < *(int *)(param_1 + 0x58)) {
      iVar1 = *(int *)(param_1 + 0x58);
    }
    *(int *)(param_1 + 0x58) = iVar1;
    iVar1 = iVar3;
    if (*(int *)(param_1 + 0x5c) < iVar3) {
      iVar1 = *(int *)(param_1 + 0x5c);
    }
    *(int *)(param_1 + 0x5c) = iVar1;
    if (((*(int *)(param_1 + 0x54) != DAT_8ced4764) && (iVar3 < *(int *)(param_1 + 0x54))) &&
       (uVar2 = (uint)(*(int *)(param_1 + 0x54) - iVar3) >> 3, uVar2 != 0)) {
      *(uint *)(param_1 + 0x54) = *(int *)(param_1 + 0x54) - uVar2;
      return;
    }
    *(int *)(param_1 + 0x54) = iVar3;
  }
  return;
}


/* Address: 8ced4768 */

int FUN_8ced4768(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  
  iVar1 = *(int *)(param_1 + 0x54) + *(int *)(param_1 + 0x50);
  iVar2 = *(int *)(param_1 + 0x50) + *(int *)(param_1 + 0x58);
  if ((iVar1 <= param_2) && (iVar1 = DAT_8ced487c, param_2 < iVar2)) {
    iVar1 = iVar2;
  }
  return iVar1;
}


/* Address: 8ced478e */

void callback_candidate_8ced478e(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar1 = DAT_8ced4880;
  *(int *)(DAT_8ced4872 + DAT_8ced4880) = *(int *)(DAT_8ced4872 + DAT_8ced4880) + 1;
  iVar3 = (int)DAT_8ced4876;
  iVar2 = *(int *)(DAT_8ced4874 + iVar1);
  iVar1 = *(int *)(DAT_8ced4874 + 4 + iVar1);
  for (iVar4 = 0; iVar4 < iVar2; iVar4 = iVar4 + 1) {
    if (*(int *)(iVar1 + 0x40) != 0) {
      FUN_8ced47d8();
    }
    iVar1 = iVar1 + iVar3;
  }
  return;
}


/* Address: 8ced47d8 */

void FUN_8ced47d8(int param_1)

{
  FUN_8ced47f8();
  FUN_8ced48e0(param_1,DAT_8ced4878 + param_1 + 0x48,DAT_8ced4878 + param_1 + 0x4c);
  return;
}


/* Address: 8ced47f8 */

void FUN_8ced47f8(int param_1)

{
  int iVar1;
  undefined1 auStack_c [4];
  int local_8;
  
  local_8 = param_1;
  iVar1 = FUN_8ced482c();
  if (iVar1 != 0) {
    (*(code *)PTR_FUN_8ced4884)(auStack_c);
    iVar1 = local_8 + DAT_8ced4878;
    *(int *)(iVar1 + 100) = *(int *)(iVar1 + 100) + *(int *)(iVar1 + 0x68);
    (*(code *)PTR_FUN_8ced4888)(auStack_c);
  }
  return;
}


/* Address: 8ced482c */

undefined4 FUN_8ced482c(int param_1)

{
  if ((*(int *)(param_1 + 0x40) == 4) && (*(int *)(param_1 + 0x48) == 0)) {
    return 1;
  }
  return 0;
}


/* Address: 8ced4846 */

void callback_candidate_8ced4846(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  
  iVar1 = (*(code *)PTR_FUN_8ced488c)(param_1);
  if (iVar1 != 0) {
    (*(code *)PTR_FUN_8ced4894)(0,DAT_8ced4890);
    return;
  }
  FUN_8ced4898(param_1,param_2,param_3);
  return;
}


/* Address: 8ced4898 */

undefined4 FUN_8ced4898(int param_1,undefined4 *param_2,undefined4 *param_3)

{
  undefined4 uVar1;
  int iVar2;
  
  iVar2 = DAT_8ced49b4 + param_1;
  if (*(int *)(iVar2 + 0x48) != -5) {
    *param_2 = *(undefined4 *)(iVar2 + 0x48);
    *param_3 = *(undefined4 *)(iVar2 + 0x4c);
    return 0;
  }
                    /* WARNING: Could not recover jumptable at 0x8ced48ea. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  uVar1 = (**(code **)(DAT_8ced49b4 + param_1))();
  return uVar1;
}


/* Address: 8ced48e0 */

void FUN_8ced48e0(int param_1)

{
                    /* WARNING: Could not recover jumptable at 0x8ced48ea. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(DAT_8ced49b4 + param_1))();
  return;
}


/* Address: 8ced48ee */

undefined4 callback_candidate_8ced48ee(undefined4 *param_1,undefined4 *param_2)

{
  int iVar1;
  
  iVar1 = FUN_8ced4956(param_1);
  if (iVar1 != 0) {
    *param_1 = 0xfffffffe;
    *param_2 = 1;
  }
  return 0;
}


/* Address: 8ced4912 */

undefined4 callback_candidate_8ced4912(int param_1,undefined4 *param_2,undefined4 *param_3)

{
  int iVar1;
  int iVar2;
  
  iVar2 = FUN_8ced4956(param_1,param_2);
  iVar1 = DAT_8ced49c4;
  if (iVar2 != 0) {
    *param_2 = *(undefined4 *)(param_1 + DAT_8ced49b4 + 100);
    *param_3 = *(undefined4 *)(iVar1 + 8);
  }
  return 0;
}


/* Address: 8ced4944 */

undefined4 callback_candidate_8ced4944(undefined4 param_1)

{
  FUN_8ced4956(param_1);
  return 0;
}


/* Address: 8ced4956 */

undefined4 FUN_8ced4956(int param_1,undefined4 *param_2,undefined4 *param_3)

{
  int iVar1;
  
  iVar1 = *(int *)(param_1 + 0x40);
  if ((((iVar1 != 4) && (iVar1 != -4)) && (iVar1 != 6)) && (iVar1 != -6)) {
    *param_2 = 0xffffffff;
    *param_3 = 1;
    return 0;
  }
  return 1;
}


/* Address: 8ced4984 */

undefined4 FUN_8ced4984(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = (*(code *)PTR_FUN_8ced49b8)();
  if (iVar1 != 0) {
    uVar2 = (*(code *)PTR_FUN_8ced49c0)(0,DAT_8ced49c8);
    return uVar2;
  }
  *(int *)(param_1 + DAT_8ced49b4 + 0x60) = *(int *)(param_1 + DAT_8ced49b4 + 0x60) + 1;
  return 0;
}


/* Address: 8ced4a54 */

void FUN_8ced4a54(int param_1,undefined4 param_2,int param_3)

{
  *(undefined4 *)(param_3 * 4 + DAT_8ced4b14 + param_1 + 4) = param_2;
  FUN_8ced4a64();
  return;
}


/* Address: 8ced4a64 */

void FUN_8ced4a64(int param_1)

{
  *(undefined4 *)(DAT_8ced4b14 + param_1) =
       ((undefined4 *)(DAT_8ced4b14 + param_1))[*(int *)(DAT_8ced4b16 + param_1) + 1];
  return;
}


/* Address: 8ced4a7a */

void callback_candidate_8ced4a7a(int *param_1,undefined4 param_2,undefined4 param_3)

{
                    /* WARNING: Could not recover jumptable at 0x8ced4aac. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(PTR_DAT_8ced4b3c + param_1[1] * 4 + *param_1 * 8))
            (*(undefined4 *)(PTR_DAT_8ced4b38 + *param_1 * 4),param_1,param_2,param_3);
  return;
}


/* Address: 8ced4ab0 */

void FUN_8ced4ab0(int param_1,int param_2,int *param_3,int *param_4)

{
  *param_3 = param_1 * *(int *)(param_2 + 8) * (int)DAT_8ced4b18 +
             param_1 * *(int *)(param_2 + 0xc) * 0x3c + param_1 * *(int *)(param_2 + 0x10) +
             (*(int *)(param_2 + 0x14) + *(int *)(param_2 + 0x18)) * (int)DAT_8ced4b1a;
  *param_4 = param_1;
  return;
}


/* Address: 8ced4afa */

void callback_candidate_8ced4afa(undefined4 param_1,undefined4 *param_2)

{
  FUN_8ced4ab0((int)DAT_8ced4b1c);
  *param_2 = param_1;
  return;
}


/* Address: 8ced4b40 */

void callback_candidate_8ced4b40(undefined4 param_1,undefined4 *param_2)

{
  FUN_8ced4ab0((int)DAT_8ced4c80);
  *param_2 = param_1;
  return;
}


/* Address: 8ced4b5a */

void callback_candidate_8ced4b5a(undefined4 param_1,undefined4 *param_2)

{
  FUN_8ced4ab0(DAT_8ced4c8c);
  *param_2 = param_1;
  return;
}


/* Address: 8ced4b74 */

void callback_candidate_8ced4b74(undefined4 param_1,int param_2,int *param_3,undefined4 *param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  iVar3 = *(int *)(param_2 + 8);
  iVar5 = *(int *)(param_2 + 0xc);
  iVar4 = *(int *)(param_2 + 0x10);
  iVar2 = *(int *)(param_2 + 0x14);
  iVar6 = *(int *)(param_2 + 0x18);
  iVar1 = (*DAT_8ced4c90)();
  *param_3 = (iVar1 * 2 + iVar3 * DAT_8ced4c94 + iVar5 * DAT_8ced4c82 + iVar4 * 0x18 + iVar2 + iVar6
             ) * (int)DAT_8ced4c84;
  *param_4 = param_1;
  return;
}


/* Address: 8ced4bd0 */

void callback_candidate_8ced4bd0(undefined4 param_1,int param_2,int *param_3,undefined4 *param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  iVar4 = *(int *)(param_2 + 8);
  iVar2 = *(int *)(param_2 + 0xc);
  iVar5 = *(int *)(param_2 + 0x10);
  iVar3 = *(int *)(param_2 + 0x14);
  iVar6 = *(int *)(param_2 + 0x18);
  iVar1 = (*DAT_8ced4c90)();
  *param_3 = (iVar1 * 2 + iVar4 * DAT_8ced4c98 + iVar2 * DAT_8ced4c86 + iVar5 * 0x1e + iVar3 + iVar6
             ) * (int)DAT_8ced4c84;
  *param_4 = param_1;
  return;
}


/* Address: 8ced4c28 */

void callback_candidate_8ced4c28(undefined4 param_1,int param_2,int *param_3,undefined4 *param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  iVar4 = *(int *)(param_2 + 8);
  iVar2 = *(int *)(param_2 + 0xc);
  iVar5 = *(int *)(param_2 + 0x10);
  iVar3 = *(int *)(param_2 + 0x14);
  iVar6 = *(int *)(param_2 + 0x18);
  iVar1 = (*DAT_8ced4c90)();
  *param_3 = (iVar1 * 2 + iVar4 * DAT_8ced4c9c + iVar2 * DAT_8ced4c88 + iVar5 * 0x3c + iVar3 + iVar6
             ) * (int)DAT_8ced4c84;
  *param_4 = param_1;
  return;
}


/* Address: 8ced4ca0 */

void callback_candidate_8ced4ca0(undefined4 param_1,int param_2)

{
  undefined4 local_18;
  undefined4 local_14;
  undefined1 auStack_10 [4];
  undefined4 local_c;
  int local_8;
  
  local_c = param_1;
  local_8 = param_2;
  (*(code *)PTR_FUN_8ced4dc4)(auStack_10);
  if (((local_8 != 0) && (local_8 != 1)) && (local_8 == 2)) {
    FUN_8ced4d0a(local_c,&local_18,&local_14);
    FUN_8ced4ce0(local_c,local_18,local_14);
  }
  (*(code *)PTR_FUN_8ced4dc8)(auStack_10);
  return;
}


/* Address: 8ced4ce0 */

void FUN_8ced4ce0(int param_1)

{
  int iVar1;
  int iVar2;
  
  iVar2 = (int)DAT_8ced4dbc;
  iVar1 = (*(code *)PTR_callback_candidate_8ced9708_8ced4dd0)(*(undefined4 *)(DAT_8ced4dcc + 8));
  *(int *)(iVar2 + param_1 + 100) = *(int *)(iVar2 + param_1 + 100) + iVar1;
  return;
}


/* Address: 8ced4d0a */

void FUN_8ced4d0a(int param_1,int *param_2,int *param_3)

{
  undefined *puVar1;
  int iVar2;
  
  puVar1 = PTR_DAT_8ced4dd4;
  iVar2 = *(int *)(param_1 + 0x60);
  if (iVar2 == 0) {
    iVar2 = (int)DAT_8ced4dbe;
    *param_2 = 0;
  }
  else {
    *param_2 = (int)DAT_8ced4dc0;
    iVar2 = *(int *)(puVar1 + iVar2 * 4);
  }
  *param_3 = iVar2;
  return;
}


/* Address: 8ced4d2a */

undefined4 callback_candidate_8ced4d2a(int param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = (*(code *)PTR_FUN_8ced4dd8)(param_1);
  if (iVar1 != 0) {
    uVar2 = (*(code *)PTR_FUN_8ced4de0)(0,DAT_8ced4ddc);
    return uVar2;
  }
  param_1 = param_1 + DAT_8ced4dbc;
  *(int *)(param_1 + 0x6c) = *(int *)(param_1 + 0x6c) + 1;
  *(undefined4 *)(param_1 + 0x70) = param_2;
  *(undefined4 *)(param_1 + 0x74) = param_3;
  return 0;
}


/* Address: 8ced4d6c */

undefined4
callback_candidate_8ced4d6c(int param_1,undefined4 param_2,undefined4 param_3,undefined4 *param_4)

{
  undefined *puVar1;
  int iVar2;
  undefined4 uVar3;
  
  puVar1 = PTR_FUN_8ced4dd8;
  *param_4 = 0;
  iVar2 = (*(code *)puVar1)(param_1);
  if (iVar2 != 0) {
    uVar3 = (*(code *)PTR_FUN_8ced4de0)(0,DAT_8ced4de4);
    return uVar3;
  }
  if (*(int *)(DAT_8ced4dc2 + param_1 + 0x38) == 0) {
    *param_4 = 1;
  }
  else if (*(int *)(param_1 + 0x44) == 4) {
    callback_candidate_8ced4e08
              (param_1,param_2,param_3,param_4,
               *(undefined4 *)((int)DAT_8ced4ef8 + DAT_8ced4dc2 + param_1));
  }
  else {
    *param_4 = 0;
  }
  return 0;
}


/* Address: 8ced4e08 */

/* WARNING: Removing unreachable block (ram,0x8ced4e68) */
/* WARNING: Removing unreachable block (ram,0x8ced4e7c) */

void callback_candidate_8ced4e08
               (double param_1,int param_2,int param_3,int param_4,undefined4 *param_5)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  double dVar4;
  double in_xd2;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  int local_24;
  int local_20;
  int local_1c;
  int local_18;
  
  piVar1 = DAT_8ced4f08;
  iVar3 = (int)DAT_8ced4efa;
  local_1c = param_4;
  local_18 = param_3;
  FUN_8ced4898(param_2,&local_20,&local_24);
  if (local_24 == 1) {
    if (local_20 != -2) {
      iVar2 = *piVar1;
      if (*(int *)((int)DAT_8ced4efc + iVar3 + param_2) == iVar2) goto LAB_8ced4e9e;
      *(int *)((int)DAT_8ced4efc + iVar3 + param_2) = iVar2;
    }
  }
  else {
    iVar3 = (*DAT_8ced4f0c)(piVar1[2]);
    local_20 = local_20 + iVar3;
    if (in_FPSCR_SZ == '\0') {
      param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),DAT_8ced4f10);
    }
    if (in_FPSCR_PR == '\0') {
      dVar4 = (double)CONCAT44((float)local_1c,(float)local_18);
    }
    else {
      dVar4 = (double)local_1c;
    }
    if (in_FPSCR_PR == '\0') {
      dVar4 = (double)CONCAT44((int)((ulonglong)dVar4 >> 0x20),SUB84(dVar4,0) * SUB84(param_1,0));
    }
    else {
      dVar4 = dVar4 * param_1;
    }
    if (in_FPSCR_SZ == '\0') {
      in_xd2 = (double)CONCAT44(SUB84(dVar4,0),SUB84(param_1,0));
    }
    if (in_FPSCR_PR == '\0') {
      in_xd2 = (double)CONCAT44((float)((ulonglong)in_xd2 >> 0x20) /
                                (float)((ulonglong)dVar4 >> 0x20),SUB84(in_xd2,0));
    }
    else {
      in_xd2 = in_xd2 / dVar4;
    }
    if (in_FPSCR_PR == '\0') {
      dVar4 = (double)CONCAT44((float)local_24,(float)local_20 * SUB84(in_xd2,0));
    }
    else {
      dVar4 = (double)local_24 * in_xd2;
    }
    if (in_FPSCR_SZ == '\0') {
      in_xd2 = (double)CONCAT44((int)((ulonglong)in_xd2 >> 0x20),SUB84(dVar4,0));
    }
    if (in_FPSCR_PR == '\0') {
      in_xd2 = (double)CONCAT44((int)((ulonglong)in_xd2 >> 0x20),
                                SUB84(in_xd2,0) / (float)((ulonglong)dVar4 >> 0x20));
    }
    else {
      in_xd2 = in_xd2 / dVar4;
    }
    if (*(int *)(DAT_8ced4efe + param_2) == 2) {
      FUN_8ced4eb0(param_2,param_5);
      return;
    }
    if (in_FPSCR_PR == '\0' && SUB84(in_xd2,0) < (float)((ulonglong)in_xd2 >> 0x20)) {
LAB_8ced4e9e:
      *param_5 = 0;
      return;
    }
  }
  *param_5 = 1;
  return;
}


/* Address: 8ced4eb0 */

/* WARNING: Removing unreachable block (ram,0x8ced4ee0) */
/* WARNING: Removing unreachable block (ram,0x8ced4ec0) */
/* WARNING: Removing unreachable block (ram,0x8ced4ee8) */

void FUN_8ced4eb0(float param_1,float param_2,undefined4 param_3,int param_4,undefined4 *param_5)

{
  int iVar1;
  int iVar2;
  float fVar3;
  float fVar4;
  double dVar5;
  double in_xd2;
  undefined8 in_xd4;
  bool bVar7;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  float fVar6;
  
  fVar6 = (float)((ulonglong)in_xd4 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    fVar6 = param_2;
  }
  iVar2 = DAT_8ced4efa + param_4;
  fVar3 = (float)*(int *)((int)DAT_8ced4f02 + DAT_8ced4f00 + param_4);
  if (in_FPSCR_SZ == '\0') {
    in_xd2 = (double)CONCAT44(fVar3,param_3);
  }
  fVar4 = (float)((ulonglong)in_xd2 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    fVar6 = fVar6 + fVar4;
    dVar5 = (double)CONCAT44(fVar6,fVar3);
    bVar7 = fVar6 < param_1;
  }
  else {
    dVar5 = (double)CONCAT44(fVar6,fVar3) + in_xd2;
    fVar6 = (float)((ulonglong)dVar5 >> 0x20);
    bVar7 = dVar5 < (double)CONCAT44(param_1,param_2);
  }
  if (!bVar7) {
    if (in_FPSCR_SZ == '\0') {
      dVar5 = (double)CONCAT44(fVar6,param_2);
    }
    if (in_FPSCR_PR == '\0') {
      fVar4 = SUB84(dVar5,0) - fVar4;
      dVar5 = (double)(ulonglong)(uint)fVar4;
      bVar7 = fVar4 < param_1;
    }
    else {
      dVar5 = dVar5 - in_xd2;
      bVar7 = dVar5 < (double)CONCAT44(param_1,param_2);
    }
    fVar6 = SUB84(dVar5,0);
    if (bVar7) {
      if (((*DAT_8ced501c == DAT_8ced5018) && (*(int *)(param_4 + 0x60) < 3)) &&
         (*(int *)(iVar2 + 0x68) == (int)DAT_8ced5010)) {
        iVar1 = 1;
      }
      else {
        iVar1 = 0;
      }
      if (iVar1 < *(int *)(iVar2 + 0x78)) {
        if (in_FPSCR_PR != '\0' || param_1 <= param_2) {
          *param_5 = 1;
        }
        else {
          *param_5 = 0;
        }
      }
      else {
        *param_5 = *(undefined4 *)(DAT_8ced5012 + iVar2);
      }
      *(undefined4 *)(iVar2 + 0x78) = 0;
      *(undefined4 *)(iVar2 + 0x80) = *param_5;
      if (in_FPSCR_SZ == '\0') {
        *(float *)(iVar2 + 0x84) = param_1;
      }
      else {
        *(ulonglong *)(iVar2 + 0x84) = CONCAT44(param_1,param_2);
      }
    }
    else {
      iVar1 = (int)DAT_8ced4f04;
      *param_5 = 1;
      if (in_FPSCR_SZ == '\0') {
        fVar6 = *(float *)(iVar1 + iVar2);
      }
      if (fVar6 != param_1) {
        if (in_FPSCR_SZ == '\0') {
          fVar6 = *(float *)(iVar2 + 0x7c);
        }
        if (fVar6 != param_1) {
          if (in_FPSCR_SZ == '\0') {
            *(float *)(iVar2 + 0x7c) = param_1;
          }
          else {
            *(ulonglong *)(iVar2 + 0x7c) = CONCAT44(param_1,param_2);
          }
          *(int *)(iVar2 + 0x78) = *(int *)(iVar2 + 0x78) + 1;
          return;
        }
      }
    }
    return;
  }
  *param_5 = 0;
  return;
}


/* Address: 8ced4f66 */

undefined4 callback_candidate_8ced4f66(int param_1,undefined4 *param_2)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 *extraout_r2;
  
  *param_2 = 0xffffffff;
  iVar1 = (*(code *)PTR_FUN_8ced5020)(param_1,param_2);
  if (iVar1 != 0) {
    uVar2 = (*(code *)PTR_FUN_8ced5028)(0,DAT_8ced5024);
    return uVar2;
  }
  if (*(int *)(param_1 + 0x60) != 0) {
    uVar2 = (*DAT_8ced5030)();
    *extraout_r2 = uVar2;
  }
  return 0;
}


/* Address: 8ced4fc0 */

undefined4 callback_candidate_8ced4fc0(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 local_14;
  undefined4 local_10 [2];
  
  iVar3 = *(int *)(DAT_8ced5014 + param_1 + 0x40);
  uVar2 = *(undefined4 *)(DAT_8ced5014 + param_1 + 0x44);
  if (iVar3 != -5) {
    FUN_8ced4898(&local_14,local_10);
    iVar1 = (*DAT_8ced5030)();
    iVar3 = (*(code *)PTR_callback_candidate_8ced65f0_8ced5034)
                      (iVar3 + iVar1,uVar2,local_14,local_10[0]);
    if (iVar3 != 0) {
      return 1;
    }
  }
  return 0;
}


/* Address: 8ced5040 */

undefined4 callback_candidate_8ced5040(undefined4 param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = (*(code *)PTR_FUN_8ced5148)(5);
  if (iVar1 == 0) {
    return 0;
  }
  FUN_8ced5074(param_1);
  uVar2 = FUN_8ced5116(param_1);
  FUN_8ced50d0(param_1);
  return uVar2;
}


/* Address: 8ced5074 */

void FUN_8ced5074(int param_1)

{
  int iVar1;
  
  iVar1 = (*(code *)PTR_FUN_8ced514c)(6);
  if (((iVar1 != 1) &&
      (iVar1 = (*(code *)PTR_FUN_8ced5150)
                         (param_1,*(undefined4 *)
                                   ((int)DAT_8ced5146 + DAT_8ced5144 + param_1 + (int)DAT_8ced5142))
      , iVar1 == 1)) && (iVar1 = FUN_8ced50b6(param_1), iVar1 != 0)) {
    (*(code *)PTR_FUN_8ced5154)(param_1,6,1);
    return;
  }
  return;
}


/* Address: 8ced50b6 */

undefined4 FUN_8ced50b6(void)

{
  int iVar1;
  
  iVar1 = (*(code *)PTR_callback_candidate_8ced4fc0_8ced5158)();
  if (iVar1 == 0) {
    return 0;
  }
  return 1;
}


/* Address: 8ced50d0 */

void FUN_8ced50d0(int param_1)

{
  int iVar1;
  
  iVar1 = (*(code *)PTR_FUN_8ced515c)(6);
  if (((iVar1 != 1) &&
      (iVar1 = (*(code *)PTR_FUN_8ced5160)
                         (param_1,*(undefined4 *)
                                   ((int)DAT_8ced5146 + DAT_8ced5144 + param_1 + (int)DAT_8ced5142))
      , iVar1 == 1)) && (iVar1 = FUN_8ced5112(param_1), iVar1 != 0)) {
    (*(code *)PTR_FUN_8ced5164)(param_1,6,1);
    return;
  }
  return;
}


/* Address: 8ced5112 */

undefined4 FUN_8ced5112(void)

{
  return 1;
}


/* Address: 8ced5116 */

undefined4 FUN_8ced5116(void)

{
  return 0;
}


/* Address: 8ced5170 */

int callback_candidate_8ced5170(int param_1,undefined4 *param_2)

{
  int iVar1;
  
  if ((*(int *)(param_1 + 0x40) == 3) || (*(int *)(param_1 + 0x40) == 4)) {
    iVar1 = (*(code *)PTR_callback_candidate_8ceda152_8ced5234)
                      (param_1,*(undefined4 *)
                                ((int)DAT_8ced522e + DAT_8ced522c + param_1 + (int)DAT_8ced522a),
                       param_2);
    if (iVar1 != 0) {
      return iVar1;
    }
    iVar1 = FUN_8ced51c0(param_1,*param_2);
    if (iVar1 != 0) {
      return 0;
    }
  }
  *param_2 = 0;
  return 0;
}


/* Address: 8ced51c0 */

undefined4 FUN_8ced51c0(int param_1,int param_2)

{
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  
  if (param_2 == 0) {
    return 0;
  }
  if (*(int *)(DAT_8ced5230 + param_1 + 0x38) != 0) {
    return 1;
  }
  local_10 = *(undefined4 *)(param_2 + 0x14);
  local_c = *(undefined4 *)(param_2 + 0x18);
  (*(code *)PTR_callback_candidate_8ced4e08_8ced5238)
            (local_10,local_c,&local_14,*(undefined4 *)((int)DAT_8ced5232 + DAT_8ced5230 + param_1))
  ;
  return local_14;
}


/* Address: 8ced5240 */

/* WARNING: Removing unreachable block (ram,0x8ced5280) */

void callback_candidate_8ced5240(uint param_1,undefined8 *param_2,int param_3)

{
  uint uVar1;
  undefined8 *puVar2;
  undefined8 uVar3;
  undefined8 uVar5;
  undefined8 uVar7;
  undefined8 uVar9;
  char in_FPSCR_SZ;
  undefined4 uVar4;
  undefined4 uVar6;
  undefined4 uVar8;
  undefined4 uVar10;
  
  if (param_3 < 1) {
    return;
  }
  if (((param_1 & 0x1f) == 0) && (((uint)param_2 & 0x1f) == 0)) {
    uVar1 = param_1 >> 0x18 & 0x1c;
    puVar2 = (undefined8 *)(DAT_8ced5314 & param_1 | DAT_8ced5318);
    for (param_3 = param_3 >> 5; 0 < param_3; param_3 = param_3 + -1) {
      if (in_FPSCR_SZ == '\0') {
        uVar4 = *(undefined4 *)param_2;
        uVar6 = *(undefined4 *)((int)param_2 + 4);
        uVar8 = *(undefined4 *)(param_2 + 1);
        uVar10 = *(undefined4 *)((int)param_2 + 0xc);
        param_2 = param_2 + 2;
        *(undefined4 *)puVar2 = uVar4;
        *(undefined4 *)(puVar2 + 1) = uVar6;
        *(undefined4 *)(puVar2 + 2) = uVar8;
        *(undefined4 *)(puVar2 + 3) = uVar10;
      }
      else {
        uVar3 = *param_2;
        uVar5 = param_2[1];
        uVar7 = param_2[2];
        uVar9 = param_2[3];
        param_2 = param_2 + 4;
        *puVar2 = uVar3;
        puVar2[1] = uVar5;
        puVar2[2] = uVar7;
        puVar2[3] = uVar9;
      }
      CacheBlockInvalidate(param_1);
      *DAT_8ced531c = uVar1;
      *DAT_8ced5320 = uVar1;
      param_1 = param_1 + 0x20;
      puVar2 = puVar2 + 4;
    }
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x8ced5260. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*DAT_8ced530c)(param_3);
  return;
}


/* Address: 8ced5324 */

uint FUN_8ced5324(uint *param_1)

{
  uint uVar1;
  byte in_MD;
  byte in_RB;
  byte in_BL;
  byte in_FD;
  byte in_M;
  byte in_Q;
  byte in_IMASK;
  byte in_S;
  byte in_T;
  
  uVar1 = (uint)DAT_8ced5426;
  *param_1 = (((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4) & 0xf0) >> 4;
  return ((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4 | (uint)in_Q << 8 | (uint)in_M << 9 |
          (uint)in_FD << 0xf | (uint)in_BL << 0x1c | (uint)in_RB << 0x1d | (uint)in_MD << 0x1e) &
         uVar1 | 0xf0;
}


/* Address: 8ced533c */

uint FUN_8ced533c(uint *param_1)

{
  byte in_MD;
  byte in_RB;
  byte in_BL;
  byte in_FD;
  byte in_M;
  byte in_Q;
  byte in_IMASK;
  byte in_S;
  byte in_T;
  
  return (*param_1 & 0xf) << 4 |
         ((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4 | (uint)in_Q << 8 | (uint)in_M << 9 |
          (uint)in_FD << 0xf | (uint)in_BL << 0x1c | (uint)in_RB << 0x1d | (uint)in_MD << 0x1e) &
         (int)DAT_8ced5426;
}


/* Address: 8ced5352 */

void callback_candidate_8ced5352(undefined4 param_1,undefined4 param_2)

{
  undefined4 *puVar1;
  undefined1 auStack_10 [4];
  undefined4 local_c;
  undefined4 local_8;
  
  local_c = param_2;
  local_8 = param_1;
  FUN_8ced5324(auStack_10);
  puVar1 = DAT_8ced542c;
  *DAT_8ced542c = local_8;
  puVar1[1] = local_c;
  FUN_8ced533c(auStack_10);
  return;
}


/* Address: 8ced5374 */

/* WARNING: Removing unreachable block (ram,0x8ced5392) */

void callback_candidate_8ced5374(undefined4 param_1)

{
  undefined4 uVar1;
  
  uVar1 = DAT_8ced5430;
  (*DAT_8ced5434)(DAT_8ced5430,0,(int)DAT_8ced5428);
  (*DAT_8ced5438)(uVar1,param_1,&stack0x00000004);
  if (*DAT_8ced542c != 0) {
    (*(code *)*DAT_8ced542c)(DAT_8ced542c[1],uVar1);
  }
  return;
}


/* Address: 8ced53c2 */

void callback_candidate_8ced53c2(int param_1,int param_2)

{
  int iVar1;
  int *piVar2;
  undefined1 auStack_10 [4];
  int local_c;
  int local_8;
  
  local_c = param_2;
  local_8 = param_1;
  FUN_8ced5324(auStack_10);
  iVar1 = 0;
  do {
    if (0xf < iVar1) {
LAB_8ced53f4:
      if (iVar1 == 0x10) {
        callback_candidate_8ced5374(PTR_s_mwMng_too_many_libarary_8ced5440);
      }
      FUN_8ced533c(auStack_10);
      return;
    }
    piVar2 = (int *)(iVar1 * 8 + DAT_8ced543c);
    if (*piVar2 == 0) {
      *piVar2 = local_8;
      piVar2[1] = local_c;
      goto LAB_8ced53f4;
    }
    iVar1 = iVar1 + 1;
  } while( true );
}


/* Address: 8ced5410 */

void callback_candidate_8ced5410(int param_1)

{
  int *piVar1;
  int *piVar2;
  int *piVar3;
  undefined1 auStack_c [4];
  int local_8;
  
  local_8 = param_1;
  FUN_8ced5324(auStack_c);
  piVar3 = (int *)((int)DAT_8ced542a + (int)DAT_8ced543c);
  piVar1 = DAT_8ced543c;
  while (piVar2 = piVar1, piVar2 < piVar3) {
    piVar1 = piVar2 + 2;
    if (*piVar2 == local_8) {
      *piVar2 = 0;
      piVar2[1] = 0;
    }
  }
  FUN_8ced533c(auStack_c);
  return;
}


/* Address: 8ced5464 */

void callback_candidate_8ced5464(void)

{
  int *piVar1;
  int *piVar2;
  undefined4 uVar3;
  
  uVar3 = 0;
  piVar1 = (int *)((int)DAT_8ced552a + (int)DAT_8ced552c);
  for (piVar2 = DAT_8ced552c; piVar2 < piVar1; piVar2 = piVar2 + 2) {
    if (*piVar2 != 0) {
      (*(code *)*piVar2)(piVar2[1],uVar3);
    }
  }
  return;
}


/* Address: 8ced5494 */

void callback_candidate_8ced5494(int param_1,int param_2)

{
  int iVar1;
  int *piVar2;
  undefined1 auStack_10 [4];
  int local_c;
  int local_8;
  
  local_c = param_2;
  local_8 = param_1;
  FUN_8ced5324(auStack_10);
  iVar1 = 0;
  do {
    if (0xf < iVar1) {
LAB_8ced54c6:
      if (iVar1 == 0x10) {
        callback_candidate_8ced5374(PTR_s_mwMng_too_many_libarary_8ced5534);
      }
      FUN_8ced533c(auStack_10);
      return;
    }
    piVar2 = (int *)(iVar1 * 8 + DAT_8ced5530);
    if (*piVar2 == 0) {
      *piVar2 = local_8;
      piVar2[1] = local_c;
      goto LAB_8ced54c6;
    }
    iVar1 = iVar1 + 1;
  } while( true );
}


/* Address: 8ced54e2 */

void callback_candidate_8ced54e2(int param_1)

{
  int *piVar1;
  int *piVar2;
  int *piVar3;
  undefined1 auStack_c [4];
  int local_8;
  
  local_8 = param_1;
  FUN_8ced5324(auStack_c);
  piVar3 = (int *)((int)DAT_8ced552a + (int)DAT_8ced5530);
  piVar1 = DAT_8ced5530;
  while (piVar2 = piVar1, piVar2 < piVar3) {
    piVar1 = piVar2 + 2;
    if (*piVar2 == local_8) {
      *piVar2 = 0;
      piVar2[1] = 0;
    }
  }
  FUN_8ced533c(auStack_c);
  return;
}


/* Address: 8ced5518 */

void callback_candidate_8ced5518(void)

{
  int *piVar1;
  int *piVar2;
  
  piVar1 = (int *)((int)DAT_8ced552a + (int)DAT_8ced5530);
  for (piVar2 = DAT_8ced5530; piVar2 < piVar1; piVar2 = piVar2 + 2) {
    if (*piVar2 != 0) {
      (*(code *)*piVar2)(piVar2[1]);
    }
  }
  return;
}


/* Address: 8ced5556 */

void callback_candidate_8ced5556(int param_1,int param_2)

{
  int iVar1;
  int *piVar2;
  undefined1 auStack_10 [4];
  int local_c;
  int local_8;
  
  local_c = param_2;
  local_8 = param_1;
  FUN_8ced5324(auStack_10);
  iVar1 = 0;
  do {
    if (0xf < iVar1) {
LAB_8ced5588:
      if (iVar1 == 0x10) {
        callback_candidate_8ced5374(PTR_s_mwMng_too_many_libarary_8ced5610);
      }
      FUN_8ced533c(auStack_10);
      return;
    }
    piVar2 = (int *)(iVar1 * 8 + DAT_8ced560c);
    if (*piVar2 == 0) {
      *piVar2 = local_8;
      piVar2[1] = local_c;
      goto LAB_8ced5588;
    }
    iVar1 = iVar1 + 1;
  } while( true );
}


/* Address: 8ced55a4 */

void callback_candidate_8ced55a4(int param_1)

{
  int *piVar1;
  int *piVar2;
  int *piVar3;
  undefined1 auStack_c [4];
  int local_8;
  
  local_8 = param_1;
  FUN_8ced5324(auStack_c);
  piVar3 = (int *)((int)DAT_8ced560a + (int)DAT_8ced560c);
  piVar1 = DAT_8ced560c;
  while (piVar2 = piVar1, piVar2 < piVar3) {
    piVar1 = piVar2 + 2;
    if (*piVar2 == local_8) {
      *piVar2 = 0;
      piVar2[1] = 0;
    }
  }
  FUN_8ced533c(auStack_c);
  return;
}


/* Address: 8ced55da */

void callback_candidate_8ced55da(void)

{
  int *piVar1;
  int *piVar2;
  undefined4 uVar3;
  
  uVar3 = 0;
  piVar1 = (int *)((int)DAT_8ced560a + (int)DAT_8ced560c);
  for (piVar2 = DAT_8ced560c; piVar2 < piVar1; piVar2 = piVar2 + 2) {
    if (*piVar2 != 0) {
      (*(code *)*piVar2)(piVar2[1],uVar3);
    }
  }
  return;
}


/* Address: 8ced5614 */

void callback_candidate_8ced5614(void)

{
  undefined *puVar1;
  code *pcVar2;
  undefined4 uVar3;
  int iVar4;
  
  pcVar2 = DAT_8ced569c;
  puVar1 = PTR_DAT_8ced5698;
  if (*(int *)PTR_DAT_8ced5698 == 0) {
    iVar4 = (int)DAT_8ced5692;
    (*DAT_8ced569c)(DAT_8ced56a0,0,iVar4,0);
    (*pcVar2)(DAT_8ced56a4,0,iVar4);
    (*pcVar2)(DAT_8ced56a8,0,iVar4);
    (*pcVar2)(DAT_8ced56ac,0,8);
    uVar3 = (*DAT_8ced56b4)((int)DAT_8ced5694,PTR_callback_candidate_8ced5518_8ced56b0,0x70,0);
    *(undefined4 *)PTR_DAT_8ced56b8 = uVar3;
  }
  *(int *)puVar1 = *(int *)puVar1 + 1;
  return;
}


/* Address: 8ced5668 */

void FUN_8ced5668(void)

{
  int iVar1;
  
  iVar1 = *(int *)PTR_DAT_8ced5698;
  *(int *)PTR_DAT_8ced5698 = iVar1 + -1;
  if ((iVar1 + -1 == 0) && (*(int *)PTR_DAT_8ced56b8 != 0)) {
    (*DAT_8ced56bc)();
    *(undefined4 *)PTR_DAT_8ced56b8 = 0;
  }
  return;
}


/* Address: 8ced56c0 */

uint FUN_8ced56c0(uint *param_1)

{
  uint uVar1;
  byte in_MD;
  byte in_RB;
  byte in_BL;
  byte in_FD;
  byte in_M;
  byte in_Q;
  byte in_IMASK;
  byte in_S;
  byte in_T;
  
  uVar1 = (uint)DAT_8ced5796;
  *param_1 = (((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4) & 0xf0) >> 4;
  return ((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4 | (uint)in_Q << 8 | (uint)in_M << 9 |
          (uint)in_FD << 0xf | (uint)in_BL << 0x1c | (uint)in_RB << 0x1d | (uint)in_MD << 0x1e) &
         uVar1 | 0xf0;
}


/* Address: 8ced56d8 */

uint FUN_8ced56d8(uint *param_1)

{
  byte in_MD;
  byte in_RB;
  byte in_BL;
  byte in_FD;
  byte in_M;
  byte in_Q;
  byte in_IMASK;
  byte in_S;
  byte in_T;
  
  return (*param_1 & 0xf) << 4 |
         ((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4 | (uint)in_Q << 8 | (uint)in_M << 9 |
          (uint)in_FD << 0xf | (uint)in_BL << 0x1c | (uint)in_RB << 0x1d | (uint)in_MD << 0x1e) &
         (int)DAT_8ced5796;
}


/* Address: 8ced56ee */

void FUN_8ced56ee(int param_1)

{
  undefined *puVar1;
  
  puVar1 = PTR_DAT_8ced57a4;
  if (param_1 == 2) {
    *(undefined4 *)PTR_DAT_8ced57a8 = 1;
    *(undefined4 *)puVar1 = 2;
  }
  else if (param_1 == 3) {
    *(undefined4 *)PTR_DAT_8ced57a8 = 2;
    *(undefined4 *)puVar1 = 3;
  }
  return;
}


/* Address: 8ced5712 */

void callback_candidate_8ced5712(void)

{
  (*DAT_8ced57b0)(DAT_8ced57ac,0,(int)DAT_8ced5798);
  (*(code *)PTR_callback_candidate_8ced5ade_8ced57b4)();
  return;
}


/* Address: 8ced5724 */

void FUN_8ced5724(void)

{
                    /* WARNING: Could not recover jumptable at 0x8ced572a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*DAT_8ced57b0)(DAT_8ced57ac,0,(int)DAT_8ced5798);
  return;
}


/* Address: 8ced572e */

undefined1 * callback_candidate_8ced572e(int *param_1)

{
  undefined *puVar1;
  undefined1 *puVar2;
  code *pcVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  int iVar6;
  char in_FPSCR_SZ;
  undefined1 auStack_20 [20];
  undefined4 local_c;
  
  local_c = 0;
  FUN_8ced56c0(auStack_20);
  puVar2 = DAT_8ced57ac;
  *DAT_8ced57ac = 1;
  pcVar3 = DAT_8ced57b8;
  puVar1 = PTR_DAT_8ced57a4;
  iVar6 = (int)DAT_8ced579a;
  puVar2[8] = 1;
  *(undefined4 *)(puVar2 + 4) = 0;
  uVar5 = (*pcVar3)(puVar2 + iVar6,*(int *)puVar1 << 4,0);
  *(undefined4 *)(puVar2 + DAT_8ced579c) = uVar5;
  if (*(int *)(puVar2 + DAT_8ced579c) == 0) {
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  uVar5 = (*DAT_8ced57b8)(puVar2 + DAT_8ced579e,*(int *)PTR_DAT_8ced57a8 << 3,0);
  *(undefined4 *)(puVar2 + DAT_8ced57a0) = uVar5;
  if (*(int *)(puVar2 + DAT_8ced57a0) != 0) {
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(puVar2 + 0x34) = (undefined4)DAT_8ced58ec;
      *(undefined4 *)(puVar2 + 0x38) = DAT_8ced58ec._4_4_;
    }
    else {
      *(ulonglong *)(puVar2 + 0x34) = CONCAT44(DAT_8ced58ec._4_4_,(undefined4)DAT_8ced58ec);
      *(ulonglong *)(puVar2 + 0x38) = CONCAT44(DAT_8ced58f4,DAT_8ced58ec._4_4_);
    }
    iVar6 = (int)DAT_8ced58e4;
    *(int *)(puVar2 + 0x24) = *param_1 << 4;
    uVar5 = DAT_8ced58f4;
    *(int *)(puVar2 + 0x28) = param_1[1] << 4;
    *(undefined4 *)(puVar2 + iVar6) = 0;
    *(undefined4 *)(puVar2 + 0x68) = 0;
    *(undefined4 *)(puVar2 + 0x6c) = 0;
    *(undefined4 *)(puVar2 + 0x70) = 0;
    *(undefined4 *)(puVar2 + 0x74) = 0;
    uVar4 = DAT_8ced58f8;
    *(undefined4 *)(puVar2 + 0x60) = uVar5;
    *(undefined4 *)(puVar2 + 100) = uVar4;
    (*(code *)PTR_callback_candidate_8ced6428_8ced5900)
              (puVar2,*(undefined4 *)(puVar2 + 0x24),*(undefined4 *)(puVar2 + 0x28),
               *(undefined4 *)PTR_DAT_8ced58fc);
    (*(code *)PTR_FUN_8ced5904)(puVar2,param_1[2]);
    (*DAT_8ced5908)(puVar2 + 0x10,0,8);
    (*(code *)PTR_callback_candidate_8ced5da4_8ced590c)(puVar2);
    FUN_8ced587e(puVar2);
    FUN_8ced56d8(auStack_20);
    return puVar2;
  }
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}


/* Address: 8ced583c */

void callback_candidate_8ced583c(undefined1 *param_1)

{
  int iVar1;
  undefined1 auStack_10 [4];
  int *local_c;
  
  FUN_8ced56c0(auStack_10);
  if (param_1 != (undefined1 *)0x0) {
    iVar1 = (int)DAT_8ced58e6;
    *param_1 = 0;
    local_c = *(int **)(param_1 + iVar1);
    (**(code **)(*local_c + 0xc))(local_c);
    local_c = *(int **)(param_1 + DAT_8ced58e8);
    (**(code **)(*local_c + 0xc))(local_c);
    (*(code *)PTR_callback_candidate_8ced64ba_8ced5910)(param_1);
  }
  FUN_8ced56d8(auStack_10);
  return;
}


/* Address: 8ced587e */

void FUN_8ced587e(int param_1)

{
  undefined *puVar1;
  int iVar2;
  int *piVar3;
  int iVar4;
  int iVar5;
  int *local_20;
  int local_1c;
  
  FUN_8ced5966();
  piVar3 = *(int **)(DAT_8ced58e6 + param_1);
  (**(code **)(*piVar3 + 0x14))(piVar3,piVar3);
  piVar3 = *(int **)(DAT_8ced58e8 + param_1);
  (**(code **)(*piVar3 + 0x14))(piVar3,piVar3);
  while ((**(code **)(**(int **)(DAT_8ced58e6 + param_1) + 0x18))
                   (*(int **)(DAT_8ced58e6 + param_1),0,0x10,&local_20), puVar1 = PTR_DAT_8ced58fc,
        local_1c != 0) {
    (**(code **)(**(int **)(DAT_8ced58e6 + param_1) + 0x20))
              (*(int **)(DAT_8ced58e6 + param_1),1,&local_20);
  }
  iVar4 = 0;
  iVar5 = *(int *)(param_1 + 0xc);
  while( true ) {
    if (*(int *)puVar1 <= iVar4) {
      *(undefined4 *)(param_1 + 4) = 1;
      return;
    }
    (**(code **)(**(int **)(DAT_8ced5a32 + param_1) + 0x18))
              (*(int **)(DAT_8ced5a32 + param_1),1,0x10,&local_20);
    if (local_1c == 0) break;
    iVar2 = (int)DAT_8ced5a32;
    *local_20 = iVar5;
    (**(code **)(**(int **)(iVar2 + param_1) + 0x20))(*(int **)(iVar2 + param_1),0,&local_20);
    iVar5 = iVar5 + 0x28;
    iVar4 = iVar4 + 1;
  }
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}


/* Address: 8ced5966 */

void FUN_8ced5966(int param_1)

{
  int *piVar1;
  
  piVar1 = *(int **)(DAT_8ced5a32 + param_1);
  (**(code **)(*piVar1 + 0x14))(piVar1,piVar1);
  piVar1 = *(int **)(DAT_8ced5a34 + param_1);
  (**(code **)(*piVar1 + 0x14))(piVar1,piVar1);
  *(undefined4 *)(param_1 + 4) = 0;
  return;
}


/* Address: 8ced5996 */

void FUN_8ced5996(int param_1)

{
  uint uVar1;
  int *piVar2;
  
  if (*(int *)(param_1 + 4) == 3) {
    (*(code *)PTR_callback_candidate_8ced61b2_8ced5a38)(param_1);
    return;
  }
  if (*(int *)(param_1 + 4) == 1) {
    piVar2 = *(int **)(DAT_8ced5a32 + param_1);
    uVar1 = (**(code **)(*piVar2 + 0x24))(piVar2,1,piVar2);
    if (*(int *)PTR_DAT_8ced5a3c <= (int)(uVar1 >> 4)) {
      *(undefined4 *)(param_1 + 4) = 3;
    }
  }
  return;
}


/* Address: 8ced59de */

void callback_candidate_8ced59de(void)

{
  undefined1 auStack_8 [8];
  
  FUN_8ced56c0(auStack_8);
  if (*DAT_8ced5a40 == '\x01') {
    FUN_8ced5996(DAT_8ced5a40);
  }
  FUN_8ced56d8(auStack_8);
  return;
}


/* Address: 8ced5a0a */

void callback_candidate_8ced652a(void)

{
  (*DAT_8ced5a44)();
  return;
}


/* Address: 8ced5a10 */

void FUN_8ced5a10(int param_1,undefined4 param_2,undefined4 param_3)

{
  *(undefined4 *)(param_1 + 0x68) = param_2;
  *(undefined4 *)(param_1 + 0x6c) = param_3;
  return;
}


/* Address: 8ced5a1a */

void FUN_8ced5a1a(int param_1,undefined4 param_2,undefined4 param_3)

{
  *(undefined4 *)(param_1 + 0x70) = param_2;
  *(undefined4 *)(param_1 + 0x74) = param_3;
  return;
}


/* Address: 8ced5a4e */

void FUN_8ced5a4e(undefined4 param_1,undefined4 *param_2)

{
  code *UNRECOVERED_JUMPTABLE;
  undefined *puVar1;
  char in_FPSCR_SZ;
  
  *param_2 = DAT_8ced5b54;
  param_2[1] = 0;
  param_2[2] = 0;
  param_2[3] = 0;
  param_2[0x10] = 1;
  param_2[4] = 0;
  param_2[5] = 4;
  param_2[6] = 0;
  param_2[7] = 0;
  param_2[8] = 3;
  param_2[9] = 0;
  param_2[10] = 0;
  param_2[0xb] = 10;
  param_2[0xc] = 0xb;
  param_2[0xd] = 0;
  param_2[0xe] = 0;
  param_2[0xf] = 2;
  param_2[0x10] = 1;
  param_2[0x11] = 1;
  param_2[0x14] = 0;
  puVar1 = PTR_DAT_8ced5b58;
  param_2[0x13] = 0;
  if (*(int *)puVar1 == 0) {
    param_2[0x15] = 0;
  }
  else {
    param_2[0x15] = 2;
  }
  param_2[0x16] = 0;
  param_2[0x17] = 4;
  param_2[0x18] = 1;
  param_2[0x19] = 0;
  param_2[0x1a] = 0;
  if (in_FPSCR_SZ == '\0') {
    param_2[0x1c] = 0x3f800000;
    param_2[0x1d] = 0x3f800000;
    param_2[0x1e] = 0x3f800000;
    param_2[0x1f] = 0x3f800000;
    param_2[0x20] = 0x3f800000;
    param_2[0x21] = 0x3f800000;
    UNRECOVERED_JUMPTABLE = DAT_8ced5b5c;
    param_2[0x22] = 0x3f800000;
                    /* WARNING: Could not recover jumptable at 0x8ced5ada. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    param_2[0x23] = 0x3f800000;
  }
  else {
    *(ulonglong *)(param_2 + 0x1c) = CONCAT44(0x3f800000,param_1);
    *(ulonglong *)(param_2 + 0x1d) = CONCAT44(0x3f800000,param_1);
    *(ulonglong *)(param_2 + 0x1e) = CONCAT44(0x3f800000,param_1);
    *(ulonglong *)(param_2 + 0x1f) = CONCAT44(0x3f800000,param_1);
    *(ulonglong *)(param_2 + 0x20) = CONCAT44(0x3f800000,param_1);
    *(ulonglong *)(param_2 + 0x21) = CONCAT44(0x3f800000,param_1);
    UNRECOVERED_JUMPTABLE = DAT_8ced5b5c;
    *(ulonglong *)(param_2 + 0x22) = CONCAT44(0x3f800000,param_1);
    *(ulonglong *)(param_2 + 0x23) = CONCAT44(0x3f800000,param_1);
  }
  (*UNRECOVERED_JUMPTABLE)();
  return;
}


/* Address: 8ced5ade */

void callback_candidate_8ced5ade(void)

{
  code *UNRECOVERED_JUMPTABLE;
  uint *puVar1;
  uint uVar2;
  
  (*DAT_8ced5b64)(DAT_8ced5b60,0,(int)DAT_8ced5b4c);
  FUN_8ced5a4e(DAT_8ced5b60);
  puVar1 = DAT_8ced5b60;
  uVar2 = (uint)DAT_8ced5b4e;
  *DAT_8ced5b60 = 8;
  puVar1[8] = 3;
  *puVar1 = *puVar1 | uVar2;
  UNRECOVERED_JUMPTABLE = DAT_8ced5b5c;
  puVar1[0x13] = 3;
                    /* WARNING: Could not recover jumptable at 0x8ced5b0a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*UNRECOVERED_JUMPTABLE)();
  return;
}


/* Address: 8ced5b68 */

/* WARNING: Removing unreachable block (ram,0x8ced5cf8) */
/* WARNING: Removing unreachable block (ram,0x8ced5d64) */
/* WARNING: Removing unreachable block (ram,0x8ced5d46) */
/* WARNING: Removing unreachable block (ram,0x8ced5c70) */
/* WARNING: Removing unreachable block (ram,0x8ced5c48) */
/* WARNING: Removing unreachable block (ram,0x8ced5c20) */
/* WARNING: Removing unreachable block (ram,0x8ced5bac) */
/* WARNING: Removing unreachable block (ram,0x8ced5bbc) */
/* WARNING: Removing unreachable block (ram,0x8ced5bfa) */
/* WARNING: Removing unreachable block (ram,0x8ced5bdc) */
/* WARNING: Removing unreachable block (ram,0x8ced5c2e) */
/* WARNING: Removing unreachable block (ram,0x8ced5c54) */
/* WARNING: Removing unreachable block (ram,0x8ced5cb0) */
/* WARNING: Removing unreachable block (ram,0x8ced5d38) */
/* WARNING: Removing unreachable block (ram,0x8ced5cf2) */
/* WARNING: Removing unreachable block (ram,0x8ced5d1e) */
/* WARNING: Removing unreachable block (ram,0x8ced5c8e) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8ced5b68(undefined8 param_1,double param_2,int param_3,int param_4)

{
  int iVar1;
  int iVar2;
  float in_fr3;
  undefined4 uVar3;
  float fVar4;
  float fVar5;
  double dVar6;
  double dVar7;
  float fVar8;
  float fVar9;
  double in_xd2;
  double dVar10;
  double dVar11;
  double dVar12;
  double in_xd6;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  dVar6 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),0x3f800000);
  if (in_FPSCR_SZ == '\0') {
    param_2 = (double)(ulonglong)(uint)DAT_8ced5cc0;
  }
  else {
    in_xd6 = (double)CONCAT44(DAT_8ced5cc0._4_4_,(uint)DAT_8ced5cc0);
  }
  if (in_FPSCR_PR == '\0') {
    fVar8 = 2.0;
  }
  else {
    fVar8 = SUB84(dVar6 + dVar6,0);
  }
  fVar4 = DAT_8ced5cc0._4_4_;
  if (in_FPSCR_SZ != '\0') {
    in_xd2 = (double)CONCAT44(_DAT_8ced5cc8,DAT_8ced5cc0._4_4_);
    fVar4 = in_fr3;
  }
  iVar2 = (int)DAT_8ced5cb8;
  if (in_FPSCR_SZ == '\0') {
    *(float *)(param_3 + 0x5c) = fVar4;
  }
  else {
    *(double *)(param_3 + 0x5c) = in_xd2;
  }
  dVar6 = (double)(ulonglong)(uint)fVar8;
  if (param_4 == 1) {
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_3 + 0x3c) = 0;
      *(undefined4 *)(param_3 + 0x40) = 0;
      *(undefined4 *)(param_3 + 0x44) = *(undefined4 *)(param_3 + 0x34);
      *(undefined4 *)(param_3 + 0x48) = *(undefined4 *)(param_3 + 0x38);
      *(undefined4 *)(param_3 + 0x4c) = 0;
      *(undefined4 *)(param_3 + 0x50) = 0;
    }
    else {
      *(double *)(param_3 + 0x3c) = dVar6;
      *(double *)(param_3 + 0x40) = dVar6;
      *(undefined8 *)(param_3 + 0x44) = *(undefined8 *)(param_3 + 0x34);
      in_xd2 = *(double *)(param_3 + 0x38);
      *(double *)(param_3 + 0x48) = in_xd2;
      *(double *)(param_3 + 0x4c) = dVar6;
      *(double *)(param_3 + 0x50) = dVar6;
    }
    if (in_FPSCR_PR == '\0') {
      fVar8 = (float)*(int *)(param_3 + 0x24) / (float)*(int *)(param_3 + 0x2c);
    }
    else {
      dVar6 = (double)*(int *)(param_3 + 0x2c);
      fVar8 = SUB84(dVar6 / dVar6,0);
    }
    if (in_FPSCR_SZ == '\0') {
      *(float *)(param_3 + 0x54) = fVar8;
    }
    else {
      *(double *)(param_3 + 0x54) = in_xd2;
    }
    if (in_FPSCR_PR == '\0') {
      fVar8 = (float)*(int *)(param_3 + 0x28) / (float)*(int *)(param_3 + 0x30);
    }
    else {
      dVar6 = (double)*(int *)(param_3 + 0x30);
      fVar8 = SUB84(dVar6 / dVar6,0);
    }
    if (in_FPSCR_SZ == '\0') {
      *(float *)(param_3 + 0x58) = fVar8;
      return;
    }
    *(double *)(param_3 + 0x58) = in_xd2;
    return;
  }
  if (iVar2 < *(int *)(param_3 + 0x24)) {
    if (*(int *)(param_3 + 0x24) == (int)DAT_8ced5cba) {
      if (in_FPSCR_SZ == '\0') {
        param_2 = (double)CONCAT44(_DAT_8ced5cc8,SUB84(param_2,0));
        *(undefined4 *)(param_3 + 0x3c) = 0;
        *(undefined4 *)(param_3 + 0x44) = *(undefined4 *)(param_3 + 0x34);
      }
      else {
        param_2 = (double)CONCAT44(_DAT_8ced5ccc,_DAT_8ced5cc8);
        *(double *)(param_3 + 0x3c) = dVar6;
        in_xd2 = *(double *)(param_3 + 0x34);
        *(double *)(param_3 + 0x44) = in_xd2;
      }
      uVar3 = uRam8ced5cd0;
      if (in_FPSCR_SZ == '\0') {
        uVar3 = _DAT_8ced5ccc;
      }
      dVar7 = (double)CONCAT44(uVar3,(float)*(int *)(param_3 + 0x2c));
      goto LAB_8ced5c22;
    }
    if ((int)DAT_8ced5cbc < *(int *)(param_3 + 0x24)) {
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(param_3 + 0x3c) = 0;
        fVar4 = *(float *)(param_3 + 0x34);
      }
      else {
        *(double *)(param_3 + 0x3c) = dVar6;
        in_xd2 = *(double *)(param_3 + 0x34);
      }
    }
    else {
      if (in_FPSCR_SZ == '\0') {
        fVar4 = *(float *)(param_3 + 0x34);
      }
      else {
        fVar4 = (float)((ulonglong)*(undefined8 *)(param_3 + 0x34) >> 0x20);
      }
      fVar5 = (float)*(int *)(param_3 + 0x24);
      if (in_FPSCR_PR == '\0') {
        dVar7 = (double)CONCAT44((fVar4 - fVar5) / fVar8,fVar5);
      }
      else {
        dVar7 = ((double)CONCAT44(fVar4,fVar5) - (double)CONCAT44(fVar4,fVar5)) / dVar6;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_3 + 0x3c) = (int)((ulonglong)dVar7 >> 0x20);
      }
      else {
        *(double *)(param_3 + 0x3c) = dVar7;
      }
      fVar4 = (float)*(int *)(param_3 + 0x24);
    }
    if (in_FPSCR_SZ == '\0') {
      *(float *)(param_3 + 0x44) = fVar4;
      *(undefined4 *)(param_3 + 0x4c) = 0;
    }
    else {
      *(double *)(param_3 + 0x44) = in_xd2;
      *(double *)(param_3 + 0x4c) = dVar6;
    }
    if (in_FPSCR_PR == '\0') {
      fVar4 = (float)*(int *)(param_3 + 0x24) / (float)*(int *)(param_3 + 0x2c);
    }
    else {
      dVar7 = (double)*(int *)(param_3 + 0x2c);
      fVar4 = SUB84(dVar7 / dVar7,0);
    }
    if (in_FPSCR_SZ == '\0') {
      *(float *)(param_3 + 0x54) = fVar4;
    }
    else {
      *(double *)(param_3 + 0x54) = in_xd2;
    }
  }
  else {
    iVar1 = *(int *)(param_3 + 0x24) << 1;
    if (in_FPSCR_PR == '\0') {
      dVar7 = (double)CONCAT44((float)iVar1,(float)(*(int *)(param_3 + 0x24) + -1));
    }
    else {
      dVar7 = (double)iVar1;
    }
    if (in_FPSCR_SZ == '\0') {
      uVar3 = SUB84(dVar7,0);
      dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),*(undefined4 *)(param_3 + 0x34));
      dVar10 = in_xd2;
      param_2 = (double)CONCAT44(uVar3,SUB84(param_2,0));
    }
    else {
      dVar10 = *(double *)(param_3 + 0x34);
      param_2 = in_xd2;
    }
    if (in_FPSCR_PR == '\0') {
      fVar4 = (float)((ulonglong)dVar7 >> 0x20);
      dVar7 = (double)CONCAT44(fVar4,(SUB84(dVar7,0) - fVar4) / fVar8);
    }
    else {
      dVar7 = (dVar7 - dVar7) / dVar6;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_3 + 0x3c) = SUB84(dVar7,0);
      dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),(int)((ulonglong)param_2 >> 0x20));
      in_xd2 = dVar10;
    }
    else {
      *(double *)(param_3 + 0x3c) = dVar10;
      in_xd2 = param_2;
    }
    if (in_FPSCR_PR == '\0') {
      fVar4 = SUB84(dVar7,0) * fVar8;
    }
    else {
      fVar4 = SUB84(dVar7 * dVar6,0);
    }
    if (in_FPSCR_SZ == '\0') {
      *(float *)(param_3 + 0x44) = fVar4;
    }
    else {
      *(double *)(param_3 + 0x44) = in_xd2;
    }
    dVar7 = in_xd6;
    if (in_FPSCR_SZ == '\0') {
      dVar7 = (double)CONCAT44(SUB84(param_2,0),(float)*(int *)(param_3 + 0x2c));
    }
LAB_8ced5c22:
    if (in_FPSCR_PR == '\0') {
      dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) / SUB84(dVar7,0),SUB84(dVar7,0));
    }
    else {
      dVar7 = dVar7 / dVar7;
    }
    uVar3 = (undefined4)((ulonglong)dVar7 >> 0x20);
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_3 + 0x4c) = uVar3;
    }
    else {
      *(double *)(param_3 + 0x4c) = dVar7;
    }
    if (in_FPSCR_PR == '\0') {
      param_2 = (double)CONCAT44((float)((ulonglong)param_2 >> 0x20) /
                                 (float)*(int *)(param_3 + 0x2c),SUB84(param_2,0));
    }
    else {
      param_2 = param_2 / (double)CONCAT44(uVar3,(float)*(int *)(param_3 + 0x2c));
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_3 + 0x54) = (int)((ulonglong)param_2 >> 0x20);
    }
    else {
      *(double *)(param_3 + 0x54) = param_2;
    }
  }
  dVar7 = in_xd2;
  if ((int)DAT_8ced5cbe < *(int *)(param_3 + 0x28)) {
    if (iVar2 < *(int *)(param_3 + 0x28)) {
      if (*(int *)(param_3 + 0x28) <= (int)DAT_8ced5e0a) {
        if (in_FPSCR_SZ == '\0') {
          dVar10 = (double)CONCAT44(DAT_8ced5e10._4_4_,(undefined4)DAT_8ced5e10);
          fVar4 = *(float *)(param_3 + 0x38);
        }
        else {
          dVar10 = (double)CONCAT44(_DAT_8ced5e18,DAT_8ced5e10._4_4_);
          fVar4 = (float)((ulonglong)*(undefined8 *)(param_3 + 0x38) >> 0x20);
        }
        if (in_FPSCR_SZ == '\0') {
          dVar7 = (double)CONCAT44((float)(*(int *)(param_3 + 0x28) + -1),SUB84(param_2,0));
        }
        fVar5 = (float)*(int *)(param_3 + 0x28);
        fVar9 = (float)((ulonglong)dVar10 >> 0x20);
        if (in_FPSCR_PR == '\0') {
          fVar5 = (fVar5 * SUB84(dVar10,0)) / fVar9;
          dVar11 = (double)CONCAT44(fVar4 - fVar5,fVar5);
        }
        else {
          dVar11 = ((double)CONCAT44(fVar4,fVar5) * dVar10) / dVar10;
          dVar11 = dVar11 - dVar11;
        }
        fVar4 = (float)((ulonglong)dVar7 >> 0x20);
        dVar12 = dVar7;
        if (in_FPSCR_SZ == '\0') {
          dVar11 = (double)CONCAT44((int)((ulonglong)dVar11 >> 0x20),fVar4);
          dVar12 = in_xd2;
        }
        if (in_FPSCR_PR == '\0') {
          dVar10 = (double)CONCAT44((float)((ulonglong)dVar11 >> 0x20) / fVar8,
                                    (SUB84(dVar11,0) * SUB84(dVar10,0)) / fVar9);
        }
        else {
          dVar10 = ((dVar11 * dVar10) / dVar6) / dVar10;
        }
        uVar3 = (undefined4)((ulonglong)dVar10 >> 0x20);
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)(param_3 + 0x40) = uVar3;
          *(int *)(param_3 + 0x48) = SUB84(dVar10,0);
          dVar10 = (double)CONCAT44(uVar3,_DAT_8ced5e18);
        }
        else {
          *(double *)(param_3 + 0x40) = dVar10;
          *(double *)(param_3 + 0x48) = dVar12;
          dVar12 = (double)CONCAT44(DAT_8ced5e1c,_DAT_8ced5e18);
        }
        if (in_FPSCR_PR == '\0') {
          dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),SUB84(dVar10,0) / fVar4);
        }
        else {
          dVar10 = dVar10 / dVar7;
        }
        if (in_FPSCR_SZ == '\0') {
          *(int *)(param_3 + 0x50) = SUB84(dVar10,0);
        }
        else {
          *(double *)(param_3 + 0x50) = dVar12;
        }
        goto LAB_8ced5d18;
      }
      if (*(int *)(param_3 + 0x28) <= (int)DAT_8ced5e0c) goto LAB_8ced5d2e;
      if (in_FPSCR_SZ == '\0') {
        *(undefined4 *)(param_3 + 0x40) = 0;
        *(undefined4 *)(param_3 + 0x48) = *(undefined4 *)(param_3 + 0x38);
      }
      else {
        *(double *)(param_3 + 0x40) = dVar6;
        in_xd2 = *(double *)(param_3 + 0x38);
        *(double *)(param_3 + 0x48) = in_xd2;
      }
    }
    else {
LAB_8ced5d2e:
      if (in_FPSCR_SZ == '\0') {
        fVar4 = *(float *)(param_3 + 0x38);
      }
      else {
        fVar4 = (float)((ulonglong)*(undefined8 *)(param_3 + 0x38) >> 0x20);
      }
      fVar5 = (float)*(int *)(param_3 + 0x28);
      if (in_FPSCR_PR == '\0') {
        dVar7 = (double)CONCAT44((fVar4 - fVar5) / fVar8,fVar5);
      }
      else {
        dVar7 = ((double)CONCAT44(fVar4,fVar5) - (double)CONCAT44(fVar4,fVar5)) / dVar6;
      }
      if (in_FPSCR_SZ == '\0') {
        *(int *)(param_3 + 0x40) = (int)((ulonglong)dVar7 >> 0x20);
      }
      else {
        *(double *)(param_3 + 0x40) = dVar7;
      }
      if (in_FPSCR_SZ == '\0') {
        *(float *)(param_3 + 0x48) = (float)*(int *)(param_3 + 0x28);
      }
      else {
        *(double *)(param_3 + 0x48) = in_xd2;
      }
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(param_3 + 0x50) = 0;
    }
    else {
      *(double *)(param_3 + 0x50) = dVar6;
    }
    if (in_FPSCR_PR == '\0') {
      fVar8 = (float)*(int *)(param_3 + 0x28) / (float)*(int *)(param_3 + 0x30);
    }
    else {
      dVar6 = (double)*(int *)(param_3 + 0x30);
      fVar8 = SUB84(dVar6 / dVar6,0);
    }
    if (in_FPSCR_SZ == '\0') {
      *(float *)(param_3 + 0x58) = fVar8;
    }
    else {
      *(double *)(param_3 + 0x58) = in_xd2;
    }
  }
  else {
    iVar2 = *(int *)(param_3 + 0x28) << 1;
    if (in_FPSCR_PR == '\0') {
      dVar10 = (double)CONCAT44((float)iVar2,(float)(*(int *)(param_3 + 0x28) + -1));
    }
    else {
      dVar10 = (double)iVar2;
    }
    if (in_FPSCR_SZ == '\0') {
      uVar3 = SUB84(dVar10,0);
      dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),*(undefined4 *)(param_3 + 0x38));
      dVar11 = in_xd2;
      dVar7 = (double)CONCAT44(uVar3,SUB84(param_2,0));
    }
    else {
      dVar11 = *(double *)(param_3 + 0x38);
    }
    if (in_FPSCR_PR == '\0') {
      fVar4 = SUB84(dVar10,0) - (float)((ulonglong)dVar10 >> 0x20);
    }
    else {
      fVar4 = SUB84(dVar10 - dVar10,0);
    }
    if (in_FPSCR_SZ == '\0') {
      in_xd6 = (double)CONCAT44(SUB84(dVar7,0),fVar4);
    }
    if (in_FPSCR_PR == '\0') {
      dVar10 = (double)CONCAT44((int)((ulonglong)in_xd6 >> 0x20),SUB84(in_xd6,0) / fVar8);
    }
    else {
      dVar10 = in_xd6 / dVar6;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_3 + 0x40) = SUB84(dVar10,0);
      dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),(int)((ulonglong)dVar7 >> 0x20));
    }
    else {
      *(double *)(param_3 + 0x40) = dVar11;
      dVar11 = dVar7;
    }
    if (in_FPSCR_PR == '\0') {
      dVar10 = (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),SUB84(dVar10,0) * fVar8);
    }
    else {
      dVar10 = dVar10 * dVar6;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_3 + 0x48) = SUB84(dVar10,0);
    }
    else {
      *(double *)(param_3 + 0x48) = dVar11;
    }
    fVar4 = (float)((ulonglong)dVar10 >> 0x20);
    fVar8 = (float)*(int *)(param_3 + 0x30);
    dVar10 = (double)CONCAT44(fVar4,fVar8);
    if (in_FPSCR_PR == '\0') {
      dVar10 = (double)CONCAT44(fVar4 / fVar8,fVar8);
    }
    else {
      dVar10 = dVar10 / dVar10;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_3 + 0x50) = (int)((ulonglong)dVar10 >> 0x20);
    }
    else {
      *(double *)(param_3 + 0x50) = dVar10;
    }
LAB_8ced5d18:
    if (in_FPSCR_PR == '\0') {
      dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) / (float)*(int *)(param_3 + 0x30),
                               SUB84(dVar7,0));
    }
    else {
      dVar7 = dVar7 / (double)CONCAT44((int)((ulonglong)dVar10 >> 0x20),
                                       (float)*(int *)(param_3 + 0x30));
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)(param_3 + 0x58) = (int)((ulonglong)dVar7 >> 0x20);
    }
    else {
      *(double *)(param_3 + 0x58) = dVar7;
    }
  }
  return;
}


/* Address: 8ced5d72 */

void FUN_8ced5d72(undefined4 param_1,undefined4 param_2,int param_3)

{
  undefined8 in_xd4;
  char in_FPSCR_SZ;
  
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(param_3 + 0x3c) = param_1;
    *(undefined4 *)(param_3 + 0x40) = param_2;
  }
  else {
    *(ulonglong *)(param_3 + 0x3c) = CONCAT44(param_1,param_2);
    *(undefined8 *)(param_3 + 0x40) = in_xd4;
  }
  callback_candidate_8ced5da4();
  return;
}


/* Address: 8ced5d7c */

void FUN_8ced5d7c(undefined4 param_1,undefined4 param_2,int param_3)

{
  undefined8 in_xd4;
  char in_FPSCR_SZ;
  
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(param_3 + 0x44) = param_1;
    *(undefined4 *)(param_3 + 0x48) = param_2;
  }
  else {
    *(ulonglong *)(param_3 + 0x44) = CONCAT44(param_1,param_2);
    *(undefined8 *)(param_3 + 0x48) = in_xd4;
  }
  callback_candidate_8ced5da4();
  return;
}


/* Address: 8ced5d86 */

/* WARNING: Removing unreachable block (ram,0x8ced5d88) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8ced5d86(double param_1,int param_2)

{
  float fVar1;
  undefined8 in_dr2;
  undefined8 in_xd2;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  if ((float)((ulonglong)param_1 >> 0x20) == 0.0) {
    if (in_FPSCR_SZ == '\0') {
      param_1 = (double)CONCAT44(DAT_8ced5e1c,SUB84(param_1,0));
    }
    else {
      param_1 = (double)CONCAT44(_DAT_8ced5e20,DAT_8ced5e1c);
    }
  }
  if (in_FPSCR_PR == '\0') {
    fVar1 = 1.0 / (float)((ulonglong)param_1 >> 0x20);
  }
  else {
    fVar1 = SUB84((double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),0x3f800000) / param_1,0);
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)(param_2 + 0x5c) = fVar1;
  }
  else {
    *(undefined8 *)(param_2 + 0x5c) = in_xd2;
  }
  callback_candidate_8ced5da4();
  return;
}


/* Address: 8ced5da4 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void callback_candidate_8ced5da4(int param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  int iVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  double in_dr2;
  double dVar7;
  undefined8 in_xd2;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  uVar2 = DAT_8ced5e24;
  uVar1 = _DAT_8ced5e20;
  iVar4 = 0;
  uVar6 = *(undefined4 *)(param_1 + 0x60);
  uVar5 = *(undefined4 *)(param_1 + 100);
  do {
    puVar3 = (undefined4 *)(iVar4 * 0x20 + param_1 + 0x78);
    if (iVar4 == 3) {
      *puVar3 = uVar2;
    }
    else {
      *puVar3 = uVar1;
    }
    if (iVar4 == 0) {
      if (in_FPSCR_SZ == '\0') {
        puVar3[1] = *(undefined4 *)(param_1 + 0x3c);
        dVar7 = (double)CONCAT44(*(undefined4 *)(param_1 + 0x40),*(undefined4 *)(param_1 + 0x48));
      }
      else {
        *(undefined8 *)(puVar3 + 1) = *(undefined8 *)(param_1 + 0x3c);
        in_xd2 = *(undefined8 *)(param_1 + 0x48);
        dVar7 = *(double *)(param_1 + 0x40);
      }
      if (in_FPSCR_PR == '\0') {
        dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) + SUB84(dVar7,0),SUB84(dVar7,0));
      }
      else {
        dVar7 = dVar7 + dVar7;
      }
      if (in_FPSCR_SZ == '\0') {
        puVar3[2] = (int)((ulonglong)dVar7 >> 0x20);
        puVar3[4] = *(undefined4 *)(param_1 + 0x4c);
        dVar7 = (double)CONCAT44(*(undefined4 *)(param_1 + 0x50),*(undefined4 *)(param_1 + 0x58));
      }
      else {
        *(double *)(puVar3 + 2) = dVar7;
        *(undefined8 *)(puVar3 + 4) = *(undefined8 *)(param_1 + 0x4c);
        in_xd2 = *(undefined8 *)(param_1 + 0x58);
        dVar7 = *(double *)(param_1 + 0x50);
      }
      if (in_FPSCR_PR == '\0') {
        in_dr2 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) + SUB84(dVar7,0),SUB84(dVar7,0))
        ;
      }
      else {
        in_dr2 = dVar7 + dVar7;
      }
      if (in_FPSCR_SZ == '\0') {
        puVar3[5] = (int)((ulonglong)in_dr2 >> 0x20);
      }
      else {
        *(double *)(puVar3 + 5) = in_dr2;
      }
LAB_8ced5e90:
      puVar3[6] = uVar6;
      puVar3[7] = uVar5;
    }
    else {
      if (iVar4 == 1) {
        if (in_FPSCR_SZ == '\0') {
          puVar3[1] = *(undefined4 *)(param_1 + 0x3c);
          puVar3[2] = *(undefined4 *)(param_1 + 0x40);
          puVar3[4] = *(undefined4 *)(param_1 + 0x4c);
          in_dr2 = (double)(ulonglong)*(uint *)(param_1 + 0x50);
          puVar3[5] = *(uint *)(param_1 + 0x50);
        }
        else {
          *(undefined8 *)(puVar3 + 1) = *(undefined8 *)(param_1 + 0x3c);
          *(undefined8 *)(puVar3 + 2) = *(undefined8 *)(param_1 + 0x40);
          *(undefined8 *)(puVar3 + 4) = *(undefined8 *)(param_1 + 0x4c);
          in_xd2 = *(undefined8 *)(param_1 + 0x50);
          *(undefined8 *)(puVar3 + 5) = in_xd2;
        }
        goto LAB_8ced5e90;
      }
      if (iVar4 == 2) {
        if (in_FPSCR_SZ == '\0') {
          dVar7 = (double)CONCAT44(*(undefined4 *)(param_1 + 0x3c),*(undefined4 *)(param_1 + 0x44));
        }
        else {
          in_xd2 = *(undefined8 *)(param_1 + 0x44);
          dVar7 = *(double *)(param_1 + 0x3c);
        }
        if (in_FPSCR_PR == '\0') {
          dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) + SUB84(dVar7,0),SUB84(dVar7,0)
                                  );
        }
        else {
          dVar7 = dVar7 + dVar7;
        }
        if (in_FPSCR_SZ == '\0') {
          puVar3[1] = (int)((ulonglong)dVar7 >> 0x20);
          dVar7 = (double)CONCAT44(*(undefined4 *)(param_1 + 0x40),*(undefined4 *)(param_1 + 0x48));
        }
        else {
          *(double *)(puVar3 + 1) = dVar7;
          in_xd2 = *(undefined8 *)(param_1 + 0x48);
          dVar7 = *(double *)(param_1 + 0x40);
        }
        if (in_FPSCR_PR == '\0') {
          dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) + SUB84(dVar7,0),SUB84(dVar7,0)
                                  );
        }
        else {
          dVar7 = dVar7 + dVar7;
        }
        if (in_FPSCR_SZ == '\0') {
          puVar3[2] = (int)((ulonglong)dVar7 >> 0x20);
          dVar7 = (double)CONCAT44(*(undefined4 *)(param_1 + 0x4c),*(undefined4 *)(param_1 + 0x54));
        }
        else {
          *(double *)(puVar3 + 2) = dVar7;
          in_xd2 = *(undefined8 *)(param_1 + 0x54);
          dVar7 = *(double *)(param_1 + 0x4c);
        }
        if (in_FPSCR_PR == '\0') {
          dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) + SUB84(dVar7,0),SUB84(dVar7,0)
                                  );
        }
        else {
          dVar7 = dVar7 + dVar7;
        }
        if (in_FPSCR_SZ == '\0') {
          puVar3[4] = (int)((ulonglong)dVar7 >> 0x20);
          dVar7 = (double)CONCAT44(*(undefined4 *)(param_1 + 0x50),*(undefined4 *)(param_1 + 0x58));
        }
        else {
          *(double *)(puVar3 + 4) = dVar7;
          in_xd2 = *(undefined8 *)(param_1 + 0x58);
          dVar7 = *(double *)(param_1 + 0x50);
        }
        if (in_FPSCR_PR == '\0') {
          in_dr2 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) + SUB84(dVar7,0),
                                    SUB84(dVar7,0));
        }
        else {
          in_dr2 = dVar7 + dVar7;
        }
        if (in_FPSCR_SZ == '\0') {
          puVar3[5] = (int)((ulonglong)in_dr2 >> 0x20);
        }
        else {
          *(double *)(puVar3 + 5) = in_dr2;
        }
        goto LAB_8ced5e90;
      }
      if (iVar4 == 3) {
        if (in_FPSCR_SZ == '\0') {
          dVar7 = (double)CONCAT44(*(undefined4 *)(param_1 + 0x3c),*(undefined4 *)(param_1 + 0x44));
        }
        else {
          in_xd2 = *(undefined8 *)(param_1 + 0x44);
          dVar7 = *(double *)(param_1 + 0x3c);
        }
        if (in_FPSCR_PR == '\0') {
          dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) + SUB84(dVar7,0),SUB84(dVar7,0)
                                  );
        }
        else {
          dVar7 = dVar7 + dVar7;
        }
        if (in_FPSCR_SZ == '\0') {
          puVar3[1] = (int)((ulonglong)dVar7 >> 0x20);
          puVar3[2] = *(undefined4 *)(param_1 + 0x40);
          dVar7 = (double)CONCAT44(*(undefined4 *)(param_1 + 0x4c),*(undefined4 *)(param_1 + 0x54));
        }
        else {
          *(double *)(puVar3 + 1) = dVar7;
          *(undefined8 *)(puVar3 + 2) = *(undefined8 *)(param_1 + 0x40);
          in_xd2 = *(undefined8 *)(param_1 + 0x54);
          dVar7 = *(double *)(param_1 + 0x4c);
        }
        if (in_FPSCR_PR == '\0') {
          in_dr2 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) + SUB84(dVar7,0),
                                    SUB84(dVar7,0));
        }
        else {
          in_dr2 = dVar7 + dVar7;
        }
        if (in_FPSCR_SZ == '\0') {
          puVar3[4] = (int)((ulonglong)in_dr2 >> 0x20);
          in_dr2 = (double)(ulonglong)*(uint *)(param_1 + 0x50);
          puVar3[5] = *(uint *)(param_1 + 0x50);
        }
        else {
          *(double *)(puVar3 + 4) = in_dr2;
          in_xd2 = *(undefined8 *)(param_1 + 0x50);
          *(undefined8 *)(puVar3 + 5) = in_xd2;
        }
        puVar3[6] = uVar6;
        puVar3[7] = uVar5;
      }
    }
    iVar4 = iVar4 + 1;
    if (in_FPSCR_SZ == '\0') {
      in_dr2 = (double)(ulonglong)*(uint *)(param_1 + 0x5c);
    }
    else {
      in_xd2 = *(undefined8 *)(param_1 + 0x5c);
    }
    if (in_FPSCR_SZ == '\0') {
      puVar3[3] = SUB84(in_dr2,0);
    }
    else {
      *(undefined8 *)(puVar3 + 3) = in_xd2;
    }
    if (3 < iVar4) {
      return;
    }
  } while( true );
}


/* Address: 8ced5eea */

void FUN_8ced5eea(int param_1,uint param_2)

{
  param_2 = param_2 & 0xff;
  *(uint *)(param_1 + 0x60) = param_2 << 0x10 | param_2 << 8 | param_2 | DAT_8ced5f30;
  callback_candidate_8ced5da4();
  return;
}


/* Address: 8ced5f04 */

void FUN_8ced5f04(int param_1,uint param_2)

{
  param_2 = param_2 & 0xff;
  *(uint *)(param_1 + 100) = param_2 << 0x10 | param_2 << 8 | param_2 | DAT_8ced5f30;
  callback_candidate_8ced5da4();
  return;
}


/* Address: 8ced5f34 */

void FUN_8ced5f34(int param_1,undefined4 param_2)

{
  code *pcVar1;
  undefined *puVar2;
  int iVar3;
  uint uVar4;
  
  pcVar1 = DAT_8ced6084;
  if (*(char *)(param_1 + 8) != '\0') {
    *DAT_8ced607c = param_2;
    (*pcVar1)(DAT_8ced6080,1);
    (*DAT_8ced6088)(DAT_8ced6080);
    puVar2 = PTR_DAT_8ced608c;
    (*DAT_8ced6090)(*(undefined4 *)PTR_DAT_8ced608c);
    pcVar1 = DAT_8ced6094;
    iVar3 = (int)DAT_8ced6074;
    uVar4 = param_1 + 0x78;
    do {
      (*pcVar1)(*(undefined4 *)puVar2,uVar4,3,0x20);
      uVar4 = uVar4 + 0x20;
    } while (uVar4 < (uint)(param_1 + 0x78 + iVar3));
  }
  return;
}


/* Address: 8ced5f92 */

uint FUN_8ced5f92(int param_1)

{
  undefined4 uVar1;
  byte in_MD;
  byte bVar2;
  byte in_RB;
  byte bVar3;
  byte in_BL;
  byte bVar4;
  byte in_FD;
  byte bVar5;
  byte in_M;
  byte extraout_M;
  byte in_Q;
  byte extraout_Q;
  byte in_IMASK;
  byte bVar6;
  byte in_S;
  byte extraout_S;
  byte in_T;
  byte extraout_T;
  
  bVar4 = (byte)(((uint)in_BL << 0x1c) >> 0x18);
  bVar2 = bVar4 | (byte)(((uint)in_RB << 0x1d) >> 0x18);
  bVar6 = (byte)((((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4) & 0xf0) >> 4);
  bVar5 = (byte)((((uint)in_Q << 8 | (uint)in_M << 9 | (uint)in_FD << 0xf) & 0x8000) >> 0xf);
  bVar4 = (bVar4 & 0x10) >> 4;
  bVar3 = (bVar2 & 0x20) >> 5;
  bVar2 = ((bVar2 | (byte)(((uint)in_MD << 0x1e) >> 0x18)) & 0x40) >> 6;
  (**(code **)(**(int **)(DAT_8ced6078 + param_1) + 0x24))(*(int **)(DAT_8ced6078 + param_1),1,1);
  uVar1 = (*DAT_8ced6098)();
  *(undefined4 *)PTR_DAT_8ced609c = uVar1;
  (**(code **)(**(int **)(DAT_8ced6078 + param_1) + 0x24))(*(int **)(DAT_8ced6078 + param_1),0);
  uVar1 = (*DAT_8ced6098)();
  *(undefined4 *)PTR_DAT_8ced60a0 = uVar1;
  (**(code **)(**(int **)(DAT_8ced607a + param_1) + 0x24))(*(int **)(DAT_8ced607a + param_1),1);
  uVar1 = (*DAT_8ced6098)();
  *(undefined4 *)PTR_DAT_8ced60a4 = uVar1;
  (**(code **)(**(int **)(DAT_8ced607a + param_1) + 0x24))(*(int **)(DAT_8ced607a + param_1),0);
  uVar1 = (*DAT_8ced6098)();
  *(undefined4 *)PTR_DAT_8ced60a8 = uVar1;
  return ((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4) & 0xf0 |
         ((uint)extraout_T | (uint)extraout_S << 1 | (uint)bVar6 << 4 | (uint)extraout_Q << 8 |
          (uint)extraout_M << 9 | (uint)bVar5 << 0xf | (uint)bVar4 << 0x1c | (uint)bVar3 << 0x1d |
         (uint)bVar2 << 0x1e) & (int)DAT_8ced6076;
}


/* Address: 8ced61b2 */

void callback_candidate_8ced61b2(int param_1)

{
  undefined *puVar1;
  undefined *puVar2;
  code *pcVar3;
  int iVar4;
  undefined4 uVar5;
  int *piVar6;
  int iVar7;
  int local_40 [2];
  undefined1 local_38 [4];
  int local_34;
  undefined1 local_30 [4];
  int local_2c;
  undefined4 local_28;
  int local_24;
  undefined4 local_20;
  undefined4 local_1c;
  
  do {
    FUN_8ced5f92(param_1);
    piVar6 = *(int **)(DAT_8ced61ee + param_1);
    (**(code **)(*piVar6 + 0x18))(piVar6,1,0x10,local_30,piVar6);
    if (local_2c == 0) {
      iVar7 = 1;
    }
    else {
      (*DAT_8ced630c)();
      if (*(int *)(param_1 + 0x68) == 0) {
        iVar7 = 0;
      }
      else {
        iVar7 = (**(code **)(param_1 + 0x68))
                          (*(undefined4 *)(param_1 + 0x6c),local_20,local_1c,local_24);
      }
    }
    if (iVar7 == 1) {
      if (0 < local_2c) {
        piVar6 = *(int **)(DAT_8ced6306 + param_1);
        (**(code **)(*piVar6 + 0x1c))(piVar6,1,local_30,piVar6);
      }
      if (*(int *)(param_1 + 0x10) == 0) {
        do {
                    /* WARNING: Do nothing block with infinite loop */
        } while( true );
      }
      (*DAT_8ced630c)();
    }
    else {
      piVar6 = *(int **)(DAT_8ced6308 + param_1);
      iVar4 = (**(code **)(*piVar6 + 0x24))(piVar6,0,piVar6);
      if (iVar4 == 0) {
        piVar6 = *(int **)(DAT_8ced6308 + param_1);
        (**(code **)(*piVar6 + 0x18))(piVar6,1,8,local_38,piVar6);
        (*DAT_8ced630c)();
        piVar6 = *(int **)(DAT_8ced6308 + param_1);
        (**(code **)(*piVar6 + 0x20))(piVar6,0,local_38,piVar6);
        if (*(int *)(param_1 + 0x70) != 0) {
          (**(code **)(param_1 + 0x70))
                    (*(undefined4 *)(param_1 + 0x74),*(undefined4 *)(local_40[0] + 8),
                     *(undefined4 *)(local_40[0] + 0xc),*(undefined4 *)(local_40[0] + 4));
        }
        piVar6 = *(int **)(DAT_8ced6306 + param_1);
        (**(code **)(*piVar6 + 0x20))(piVar6,0,local_40,piVar6);
      }
      piVar6 = *(int **)(DAT_8ced6308 + param_1);
      (**(code **)(*piVar6 + 0x18))(piVar6,0,8,local_38,piVar6);
      if (local_34 == 0) {
        do {
                    /* WARNING: Do nothing block with infinite loop */
        } while( true );
      }
      (*DAT_8ced63b8)();
      piVar6 = *(int **)(DAT_8ced63b4 + param_1);
      (**(code **)(*piVar6 + 0x20))(piVar6,1,local_38,piVar6);
    }
    (*DAT_8ced63b8)();
    if (iVar7 != 2) {
      (*DAT_8ced63b8)();
      uVar5 = (*DAT_8ced63bc)();
      puVar2 = PTR_DAT_8ced63c8;
      puVar1 = PTR_DAT_8ced63c4;
      *(undefined4 *)PTR_DAT_8ced63c0 = uVar5;
      if (local_24 == 0) {
        *(undefined4 *)puVar1 = 0;
        *(undefined4 *)puVar2 = 0;
      }
      else if (local_24 == *(int *)puVar1) {
        *(int *)puVar2 = *(int *)puVar2 + 1;
      }
      *(int *)puVar1 = local_24;
      FUN_8ced5f34(param_1,local_28);
      uVar5 = (*DAT_8ced63bc)();
      pcVar3 = DAT_8ced63d0;
      puVar1 = PTR_DAT_8ced63c0;
      *(undefined4 *)PTR_DAT_8ced63cc = uVar5;
      uVar5 = (*pcVar3)(*(undefined4 *)puVar1,uVar5);
      uVar5 = (*DAT_8ced63d4)(uVar5);
      *(undefined4 *)PTR_DAT_8ced63d8 = uVar5;
      return;
    }
  } while( true );
}


/* Address: 8ced63dc */

void FUN_8ced63dc(int param_1)

{
  if (param_1 == 0) {
    return;
  }
  do {
    *DAT_8ced64f0 = param_1;
  } while( true );
}


/* Address: 8ced63ea */

void FUN_8ced63ea(int param_1,int param_2,int *param_3,int *param_4)

{
  uint uVar1;
  int iVar2;
  
  if (param_1 < 1) {
    *param_3 = 0;
  }
  else {
    uVar1 = 0;
    for (param_1 = param_1 + -1; param_1 != 0; param_1 = param_1 >> 1) {
      uVar1 = uVar1 + 1;
    }
    if ((int)uVar1 < 0) {
      iVar2 = 1 >> (~uVar1 & 0x1f) + 1;
    }
    else {
      iVar2 = 1 << (uVar1 & 0x1f);
    }
    *param_3 = iVar2;
  }
  if (param_2 < 1) {
    *param_4 = 0;
  }
  else {
    uVar1 = 0;
    for (param_2 = param_2 + -1; param_2 != 0; param_2 = param_2 >> 1) {
      uVar1 = uVar1 + 1;
    }
    if ((int)uVar1 < 0) {
      iVar2 = 1 >> (~uVar1 & 0x1f) + 1;
    }
    else {
      iVar2 = 1 << (uVar1 & 0x1f);
    }
    *param_4 = iVar2;
  }
  return;
}


/* Address: 8ced6428 */

void callback_candidate_8ced6428(int param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  code *pcVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  undefined4 local_30;
  undefined4 local_2c [4];
  
  *(int *)(param_1 + 0x18) = param_4;
  *(undefined4 *)(param_1 + 0x24) = param_2;
  *(undefined4 *)(param_1 + 0x28) = param_3;
  uVar2 = (*DAT_8ced64f4)(0x10);
  *(undefined4 *)(param_1 + 0x1c) = uVar2;
  uVar3 = (*DAT_8ced64f4)();
  pcVar1 = DAT_8ced64fc;
  uVar2 = DAT_8ced64f8;
  iVar4 = (int)DAT_8ced64ec;
  *(undefined4 *)(param_1 + 0x20) = uVar3;
  *(undefined4 *)(param_1 + 0xc) = uVar2;
  (*pcVar1)(param_2);
  iVar5 = 0;
  iVar6 = *(int *)(param_1 + 0xc);
  if (0 < param_4) {
    do {
      FUN_8ced63ea(param_2,param_3,&local_30,local_2c);
      *(undefined4 *)(param_1 + 0x2c) = local_30;
      pcVar1 = DAT_8ced6500;
      *(undefined4 *)(param_1 + 0x30) = local_2c[0];
      uVar2 = (*pcVar1)(iVar6,local_30,local_2c[0],iVar4);
      FUN_8ced63dc(uVar2);
      iVar5 = iVar5 + 1;
      iVar6 = iVar6 + 0x28;
    } while (iVar5 < param_4);
  }
  return;
}


/* Address: 8ced64ba */

void callback_candidate_8ced64ba(int param_1)

{
  code *pcVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  pcVar1 = DAT_8ced6504;
  iVar4 = 0;
  iVar3 = *(int *)(param_1 + 0x18);
  iVar5 = *(int *)(param_1 + 0xc);
  if (0 < iVar3) {
    do {
      uVar2 = (*pcVar1)(iVar5);
      FUN_8ced63dc(uVar2);
      iVar4 = iVar4 + 1;
      iVar5 = iVar5 + 0x28;
    } while (iVar4 < iVar3);
  }
  return;
}


/* Address: 8ced6508 */

uint callback_candidate_8ced6508(int param_1)

{
  uint uVar1;
  int *piVar2;
  
  piVar2 = *(int **)(DAT_8ced65d8 + param_1);
  uVar1 = (**(code **)(*piVar2 + 0x24))(piVar2,0,param_1,piVar2);
  return uVar1 >> 4;
}


/* Address: 8ced652a */

void callback_candidate_8ced652a(int param_1,uint param_2,int param_3,int param_4)

{
  code *pcVar1;
  uint uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  int *piVar6;
  undefined4 uVar7;
  int iVar8;
  int *local_18;
  int local_14;
  
  piVar6 = *(int **)(DAT_8ced65d8 + param_1);
  (**(code **)(*piVar6 + 0x18))(piVar6,0,0x10,&local_18,piVar6);
  if (local_14 != 0) {
    iVar8 = *local_18;
    uVar3 = (*DAT_8ced65dc)();
    *DAT_8ced65e0 = uVar3;
    uVar2 = DAT_8ced65e4;
    uVar7 = *(undefined4 *)(iVar8 + 0xc);
    uVar4 = *(undefined4 *)(iVar8 + 0x10);
    *(undefined4 *)(iVar8 + 0xc) = *(undefined4 *)(param_1 + 0x24);
    *(undefined4 *)(iVar8 + 0x10) = *(undefined4 *)(param_1 + 0x28);
    uVar3 = (*DAT_8ced65e8)(iVar8,param_2 | uVar2,1,1,6,1);
    *(undefined4 *)(iVar8 + 0xc) = uVar7;
    pcVar1 = DAT_8ced65dc;
    *(undefined4 *)(iVar8 + 0x10) = uVar4;
    uVar4 = (*pcVar1)();
    *DAT_8ced65ec = uVar4;
    FUN_8ced63dc(uVar3);
    iVar5 = *(int *)(DAT_8ced65da + param_1);
    *(int *)(DAT_8ced65da + param_1) = iVar5 + 1;
    iVar8 = (int)DAT_8ced65d8;
    local_18[1] = iVar5;
    local_18[2] = param_3;
    local_18[3] = param_4;
    piVar6 = *(int **)(iVar8 + param_1);
    (**(code **)(*piVar6 + 0x20))(piVar6,1,&local_18,piVar6);
  }
  return;
}


/* Address: 8ced65f0 */

void callback_candidate_8ced65f0(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined1 auStack_14 [8];
  undefined1 auStack_c [12];
  
  (*(code *)PTR_FUN_8ced6624)(param_3,auStack_c,param_3);
  (*(code *)PTR_FUN_8ced6624)(param_2,param_1,auStack_14);
  (*(code *)PTR_FUN_8ced6628)(auStack_c,auStack_14);
  return;
}


/* Address: 8ced662c */

void FUN_8ced662c(undefined1 *param_1,undefined1 *param_2,uint param_3)

{
  undefined1 uVar1;
  undefined1 uVar2;
  undefined1 uVar3;
  undefined1 *puVar4;
  int iVar5;
  
  iVar5 = (param_3 & 0x1f) + 1;
  while (iVar5 = iVar5 + -1, iVar5 != 0) {
    uVar1 = *param_2;
    param_2 = param_2 + 1;
    *param_1 = uVar1;
    param_1 = param_1 + 1;
  }
  iVar5 = (param_3 >> 5) + 1;
  while (iVar5 = iVar5 + -1, iVar5 != 0) {
    uVar1 = param_2[1];
    uVar2 = param_2[2];
    uVar3 = param_2[3];
    *param_1 = *param_2;
    param_1[1] = uVar1;
    param_1[2] = uVar2;
    param_1[3] = uVar3;
    uVar1 = param_2[5];
    uVar2 = param_2[6];
    uVar3 = param_2[7];
    param_1[4] = param_2[4];
    param_1[5] = uVar1;
    param_1[6] = uVar2;
    param_1[7] = uVar3;
    uVar1 = param_2[9];
    uVar2 = param_2[10];
    uVar3 = param_2[0xb];
    param_1[8] = param_2[8];
    param_1[9] = uVar1;
    param_1[10] = uVar2;
    param_1[0xb] = uVar3;
    uVar1 = param_2[0xd];
    uVar2 = param_2[0xe];
    uVar3 = param_2[0xf];
    param_1[0xc] = param_2[0xc];
    param_1[0xd] = uVar1;
    param_1[0xe] = uVar2;
    param_1[0xf] = uVar3;
    uVar1 = param_2[0x11];
    uVar2 = param_2[0x12];
    uVar3 = param_2[0x13];
    param_1[0x10] = param_2[0x10];
    param_1[0x11] = uVar1;
    param_1[0x12] = uVar2;
    param_1[0x13] = uVar3;
    uVar1 = param_2[0x15];
    uVar2 = param_2[0x16];
    uVar3 = param_2[0x17];
    param_1[0x14] = param_2[0x14];
    param_1[0x15] = uVar1;
    param_1[0x16] = uVar2;
    param_1[0x17] = uVar3;
    uVar1 = param_2[0x19];
    uVar2 = param_2[0x1a];
    uVar3 = param_2[0x1b];
    puVar4 = param_2 + 0x1c;
    param_1[0x18] = param_2[0x18];
    param_1[0x19] = uVar1;
    param_1[0x1a] = uVar2;
    param_1[0x1b] = uVar3;
    uVar1 = param_2[0x1d];
    uVar2 = param_2[0x1e];
    uVar3 = param_2[0x1f];
    param_2 = param_2 + 0x20;
    param_1[0x1c] = *puVar4;
    param_1[0x1d] = uVar1;
    param_1[0x1e] = uVar2;
    param_1[0x1f] = uVar3;
    param_1 = param_1 + 0x20;
  }
  return;
}


/* Address: 8ced6708 */

void FUN_8ced6708(undefined4 *param_1,undefined4 *param_2,uint param_3)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  int iVar4;
  undefined4 uVar5;
  
  iVar4 = (param_3 & 0xf) + 1;
  while (iVar4 = iVar4 + -1, iVar4 != 0) {
    uVar1 = *param_2;
    param_2 = param_2 + 1;
    *param_1 = uVar1;
    param_1 = param_1 + 1;
  }
  iVar4 = (param_3 >> 4) + 1;
  while (iVar4 = iVar4 + -1, iVar4 != 0) {
    uVar5 = param_2[1];
    uVar1 = param_2[2];
    uVar2 = param_2[3];
    *param_1 = *param_2;
    param_1[1] = uVar5;
    param_1[2] = uVar1;
    param_1[3] = uVar2;
    uVar5 = param_2[5];
    uVar1 = param_2[6];
    uVar2 = param_2[7];
    param_1[4] = param_2[4];
    param_1[5] = uVar5;
    param_1[6] = uVar1;
    param_1[7] = uVar2;
    uVar1 = param_2[9];
    uVar2 = param_2[10];
    uVar5 = param_2[0xb];
    puVar3 = param_2 + 0xc;
    param_1[8] = param_2[8];
    param_1[9] = uVar1;
    param_1[10] = uVar2;
    param_1[0xb] = uVar5;
    uVar1 = param_2[0xd];
    uVar5 = param_2[0xe];
    uVar2 = param_2[0xf];
    param_2 = param_2 + 0x10;
    param_1[0xc] = *puVar3;
    param_1[0xd] = uVar1;
    param_1[0xe] = uVar5;
    param_1[0xf] = uVar2;
    param_1 = param_1 + 0x10;
  }
  return;
}


/* Address: 8ced6778 */

void FUN_8ced6778(undefined2 *param_1,undefined2 *param_2,uint param_3)

{
  undefined2 uVar1;
  undefined2 uVar2;
  undefined2 uVar3;
  undefined2 *puVar4;
  int iVar5;
  
  iVar5 = (param_3 & 0xf) + 1;
  while (iVar5 = iVar5 + -1, iVar5 != 0) {
    uVar1 = *param_2;
    param_2 = param_2 + 1;
    *param_1 = uVar1;
    param_1 = param_1 + 1;
  }
  iVar5 = (param_3 >> 4) + 1;
  while (iVar5 = iVar5 + -1, iVar5 != 0) {
    uVar1 = param_2[1];
    uVar2 = param_2[2];
    uVar3 = param_2[3];
    *param_1 = *param_2;
    param_1[1] = uVar1;
    param_1[2] = uVar2;
    param_1[3] = uVar3;
    uVar1 = param_2[5];
    uVar2 = param_2[6];
    uVar3 = param_2[7];
    param_1[4] = param_2[4];
    param_1[5] = uVar1;
    param_1[6] = uVar2;
    param_1[7] = uVar3;
    uVar1 = param_2[9];
    uVar2 = param_2[10];
    uVar3 = param_2[0xb];
    puVar4 = param_2 + 0xc;
    param_1[8] = param_2[8];
    param_1[9] = uVar1;
    param_1[10] = uVar2;
    param_1[0xb] = uVar3;
    uVar1 = param_2[0xd];
    uVar2 = param_2[0xe];
    uVar3 = param_2[0xf];
    param_2 = param_2 + 0x10;
    param_1[0xc] = *puVar4;
    param_1[0xd] = uVar1;
    param_1[0xe] = uVar2;
    param_1[0xf] = uVar3;
    param_1 = param_1 + 0x10;
  }
  return;
}


/* Address: 8ced67f8 */

void FUN_8ced67f8(int param_1,undefined4 param_2,uint param_3)

{
  undefined4 *puVar1;
  int iVar2;
  
  puVar1 = (undefined4 *)(param_1 + param_3 * 4);
  iVar2 = (param_3 & 0xf) + 1;
  while (iVar2 = iVar2 + -1, iVar2 != 0) {
    puVar1 = puVar1 + -1;
    *puVar1 = param_2;
  }
  iVar2 = (param_3 >> 4) + 1;
  while (iVar2 = iVar2 + -1, iVar2 != 0) {
    puVar1[-1] = param_2;
    puVar1[-2] = param_2;
    puVar1[-3] = param_2;
    puVar1[-4] = param_2;
    puVar1[-5] = param_2;
    puVar1[-6] = param_2;
    puVar1[-7] = param_2;
    puVar1[-8] = param_2;
    puVar1[-9] = param_2;
    puVar1[-10] = param_2;
    puVar1[-0xb] = param_2;
    puVar1[-0xc] = param_2;
    puVar1[-0xd] = param_2;
    puVar1[-0xe] = param_2;
    puVar1[-0xf] = param_2;
    puVar1 = puVar1 + -0x10;
    *puVar1 = param_2;
  }
  return;
}


/* Address: 8ced6840 */

void FUN_8ced6840(void)

{
  return;
}


/* Address: 8ced6844 */

void FUN_8ced6844(void)

{
  return;
}


/* Address: 8ced6848 */

undefined4
callback_candidate_8ced6848(int param_1,int param_2,int param_3,int *param_4,uint *param_5)

{
  undefined *puVar1;
  int iVar2;
  undefined4 uVar3;
  int local_28;
  uint local_24 [2];
  
  puVar1 = PTR_FUN_8ced6970;
  *param_4 = 0;
  *param_5 = 0;
  iVar2 = (*(code *)puVar1)(param_1);
  if (iVar2 == 0) {
    do {
      if (param_3 < 4) break;
      iVar2 = FUN_8ced690a(param_1,param_2,param_3,&local_28,local_24);
      *param_5 = *param_5 | local_24[0];
      param_2 = param_2 + local_28;
      param_3 = param_3 - local_28;
      *param_4 = *param_4 + local_28;
    } while (iVar2 != 0);
    if ((*param_5 & DAT_8ced697c) != 0) {
      if (*(int *)(param_1 + 0x24) == 0) {
        if (*(int *)(param_1 + 0x28) == 0) {
          uVar3 = 2;
        }
        else {
          uVar3 = 1;
        }
      }
      else {
        uVar3 = 0;
      }
      (*DAT_8ced6980)(uVar3);
    }
    uVar3 = 0;
  }
  else {
    uVar3 = (*(code *)PTR_FUN_8ced6978)(0,DAT_8ced6974);
  }
  return uVar3;
}


/* Address: 8ced690a */

undefined4
FUN_8ced690a(int param_1,undefined4 param_2,undefined4 param_3,undefined4 *param_4,int *param_5)

{
  undefined *puVar1;
  int iVar2;
  undefined4 uVar3;
  
  puVar1 = PTR_FUN_8ced6984;
  uVar3 = 0;
  *param_4 = 0;
  iVar2 = (*(code *)puVar1)(param_2,4);
  *param_5 = iVar2;
  iVar2 = *param_5;
  if (iVar2 != DAT_8ced6988) {
    if (iVar2 == DAT_8ced698c) {
      FUN_8ced69b2(param_1 + 0x10,param_2,param_3,param_4);
    }
    else {
      if (iVar2 != DAT_8ced697c) {
        if (iVar2 != DAT_8ced6990) {
          return 0;
        }
        FUN_8ced7024(DAT_8ced6ad2 + param_1,param_2,param_3,param_4);
        return 0;
      }
      FUN_8ced6c88(param_1 + 0x1c,param_2,param_3,param_4);
    }
    uVar3 = 1;
  }
  return uVar3;
}


/* Address: 8ced69b2 */

/* WARNING: Removing unreachable block (ram,0x8ced6c5a) */
/* WARNING: Removing unreachable block (ram,0x8ced6b58) */
/* WARNING: Removing unreachable block (ram,0x8ced6ab4) */
/* WARNING: Removing unreachable block (ram,0x8ced6aa8) */
/* WARNING: Removing unreachable block (ram,0x8ced6ade) */
/* WARNING: Removing unreachable block (ram,0x8ced6bd6) */
/* WARNING: Removing unreachable block (ram,0x8ced6c5c) */

void FUN_8ced69b2(uint *param_1,int param_2,undefined4 *param_3)

{
  undefined1 uVar1;
  byte bVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  undefined1 *puVar9;
  undefined1 *puVar10;
  undefined1 *puVar11;
  undefined1 *puVar12;
  uint uVar13;
  uint uVar14;
  
  puVar9 = (undefined1 *)(param_2 + 4U & 0xfffffffc);
  uVar4 = ((param_2 + 4) - (int)puVar9) * 8;
  uVar13 = CONCAT31(CONCAT21(CONCAT11(*puVar9,puVar9[1]),puVar9[2]),puVar9[3]);
  puVar10 = puVar9 + 8;
  uVar14 = CONCAT31(CONCAT21(CONCAT11(puVar9[4],puVar9[5]),puVar9[6]),puVar9[7]);
  if ((int)uVar4 < 0) {
    uVar13 = uVar13 >> (~uVar4 & 0x1f) + 1;
  }
  else {
    uVar13 = uVar13 << (uVar4 & 0x1f);
  }
  if ((int)uVar4 < 0x1e) {
    bVar2 = ((uVar13 & 0x80000000) != 0) << 1 | (uVar13 & 0x40000000) != 0;
    uVar13 = uVar13 << 2;
    iVar5 = uVar4 + 2;
  }
  else {
    iVar5 = uVar4 - 0x1e;
    if (iVar5 == 0) {
      bVar2 = ((uVar13 & 0x80000000) != 0) << 1 | (uVar13 & 0x40000000) != 0;
    }
    else {
      bVar2 = ((uVar13 & 0x80000000) != 0) << 1 | ((uVar13 | uVar14 >> 1) & 0x40000000) != 0;
      uVar14 = uVar14 << 1;
    }
    uVar1 = *puVar10;
    puVar10 = puVar9 + 0xc;
    uVar13 = uVar14;
    uVar14 = CONCAT31(CONCAT21(CONCAT11(uVar1,puVar9[9]),puVar9[10]),puVar9[0xb]);
  }
  uVar4 = iVar5 + 2;
  if ((int)uVar4 < 0x20) {
    uVar13 = uVar13 << 2;
  }
  else {
    uVar1 = *puVar10;
    puVar9 = puVar10 + 1;
    uVar4 = iVar5 - 0x1e;
    puVar11 = puVar10 + 2;
    if ((int)uVar4 < 0) {
      uVar13 = uVar14 >> (~uVar4 & 0x1f) + 1;
    }
    else {
      uVar13 = uVar14 << (uVar4 & 0x1f);
    }
    puVar12 = puVar10 + 3;
    puVar10 = puVar10 + 4;
    uVar14 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar9),*puVar11),*puVar12);
  }
  if ((int)uVar4 < 0x1d) {
    uVar3 = uVar13 << 3;
    uVar4 = uVar4 + 3;
  }
  else {
    uVar4 = uVar4 - 0x1d;
    if (uVar4 != 0) {
      uVar3 = -(3 - uVar4);
      if ((int)uVar3 < 0) {
        uVar3 = uVar14 >> (~uVar3 & 0x1f) + 1;
      }
      else {
        uVar3 = uVar14 << (uVar3 & 0x1f);
      }
      uVar13 = uVar13 | uVar3;
      if ((int)uVar4 < 0) {
        uVar14 = uVar14 >> (~uVar4 & 0x1f) + 1;
      }
      else {
        uVar14 = uVar14 << (uVar4 & 0x1f);
      }
    }
    uVar1 = *puVar10;
    puVar9 = puVar10 + 1;
    puVar11 = puVar10 + 2;
    puVar12 = puVar10 + 3;
    puVar10 = puVar10 + 4;
    uVar3 = uVar14;
    uVar14 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar9),*puVar11),*puVar12);
  }
  uVar6 = uVar4 + 1;
  if ((int)uVar6 < 0x20) {
    uVar3 = uVar3 << 1;
  }
  else {
    uVar1 = *puVar10;
    puVar9 = puVar10 + 1;
    uVar6 = uVar4 - 0x1f;
    puVar11 = puVar10 + 2;
    if ((int)uVar6 < 0) {
      uVar3 = uVar14 >> (~uVar6 & 0x1f) + 1;
    }
    else {
      uVar3 = uVar14 << (uVar6 & 0x1f);
    }
    puVar12 = puVar10 + 3;
    puVar10 = puVar10 + 4;
    uVar14 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar9),*puVar11),*puVar12);
  }
  if ((int)uVar6 < 0x11) {
    uVar4 = uVar3 << 0xf;
    uVar6 = uVar6 + 0xf;
  }
  else {
    uVar6 = uVar6 - 0x11;
    if (uVar6 != 0) {
      uVar4 = -(0xf - uVar6);
      if ((int)uVar4 < 0) {
        uVar4 = uVar14 >> (~uVar4 & 0x1f) + 1;
      }
      else {
        uVar4 = uVar14 << (uVar4 & 0x1f);
      }
      uVar3 = uVar3 | uVar4;
      if ((int)uVar6 < 0) {
        uVar14 = uVar14 >> (~uVar6 & 0x1f) + 1;
      }
      else {
        uVar14 = uVar14 << (uVar6 & 0x1f);
      }
    }
    uVar1 = *puVar10;
    puVar9 = puVar10 + 1;
    puVar11 = puVar10 + 2;
    puVar12 = puVar10 + 3;
    puVar10 = puVar10 + 4;
    uVar4 = uVar14;
    uVar14 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar9),*puVar11),*puVar12);
  }
  uVar7 = uVar6 + 1;
  if ((int)uVar7 < 0x20) {
    uVar4 = uVar4 << 1;
  }
  else {
    uVar1 = *puVar10;
    puVar9 = puVar10 + 1;
    uVar7 = uVar6 - 0x1f;
    puVar11 = puVar10 + 2;
    if ((int)uVar7 < 0) {
      uVar4 = uVar14 >> (~uVar7 & 0x1f) + 1;
    }
    else {
      uVar4 = uVar14 << (uVar7 & 0x1f);
    }
    puVar12 = puVar10 + 3;
    puVar10 = puVar10 + 4;
    uVar14 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar9),*puVar11),*puVar12);
  }
  if ((int)uVar7 < 0x11) {
    uVar6 = uVar4 << 0xf;
    uVar7 = uVar7 + 0xf;
  }
  else {
    uVar7 = uVar7 - 0x11;
    if (uVar7 != 0) {
      uVar6 = -(0xf - uVar7);
      if ((int)uVar6 < 0) {
        uVar6 = uVar14 >> (~uVar6 & 0x1f) + 1;
      }
      else {
        uVar6 = uVar14 << (uVar6 & 0x1f);
      }
      uVar4 = uVar4 | uVar6;
      if ((int)uVar7 < 0) {
        uVar14 = uVar14 >> (~uVar7 & 0x1f) + 1;
      }
      else {
        uVar14 = uVar14 << (uVar7 & 0x1f);
      }
    }
    uVar1 = *puVar10;
    puVar9 = puVar10 + 1;
    puVar11 = puVar10 + 2;
    puVar12 = puVar10 + 3;
    puVar10 = puVar10 + 4;
    uVar6 = uVar14;
    uVar14 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar9),*puVar11),*puVar12);
  }
  uVar8 = uVar7 + 1;
  if ((int)uVar8 < 0x20) {
    uVar6 = uVar6 << 1;
  }
  else {
    uVar1 = *puVar10;
    puVar9 = puVar10 + 1;
    uVar8 = uVar7 - 0x1f;
    puVar11 = puVar10 + 2;
    if ((int)uVar8 < 0) {
      uVar6 = uVar14 >> (~uVar8 & 0x1f) + 1;
    }
    else {
      uVar6 = uVar14 << (uVar8 & 0x1f);
    }
    puVar12 = puVar10 + 3;
    puVar10 = puVar10 + 4;
    uVar14 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar9),*puVar11),*puVar12);
  }
  uVar7 = uVar8 + 1;
  if ((int)uVar7 < 0x20) {
    uVar6 = uVar6 << 1;
  }
  else {
    uVar7 = uVar8 - 0x1f;
    if ((int)uVar7 < 0) {
      uVar6 = uVar14 >> (~uVar7 & 0x1f) + 1;
    }
    else {
      uVar6 = uVar14 << (uVar7 & 0x1f);
    }
    uVar14 = CONCAT31(CONCAT21(CONCAT11(*puVar10,puVar10[1]),puVar10[2]),puVar10[3]);
  }
  if ((9 < (int)uVar7) && (uVar7 - 10 != 0)) {
    uVar7 = -(0x16 - (uVar7 - 10));
    if ((int)uVar7 < 0) {
      uVar14 = uVar14 >> (~uVar7 & 0x1f) + 1;
    }
    else {
      uVar14 = uVar14 << (uVar7 & 0x1f);
    }
    uVar6 = uVar6 | uVar14;
  }
  *param_1 = (uint)(bVar2 == 0);
  param_1[1] = (uVar13 >> 0x1d) << 0x1c | (uVar3 >> 0x11) << 0xd | (int)(uVar4 >> 0x11) >> 2;
  param_1[2] = uVar6 >> 10;
  *param_3 = 0xc;
  return;
}


/* Address: 8ced6c88 */

/* WARNING: Removing unreachable block (ram,0x8ced6f40) */
/* WARNING: Removing unreachable block (ram,0x8ced6da4) */
/* WARNING: Removing unreachable block (ram,0x8ced6f4c) */
/* WARNING: Removing unreachable block (ram,0x8ced6f76) */

void FUN_8ced6c88(uint *param_1,int param_2,int *param_3)

{
  undefined1 uVar1;
  undefined3 uVar2;
  uint uVar3;
  undefined1 *puVar4;
  undefined1 *puVar5;
  undefined1 *puVar6;
  undefined1 *puVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  int iVar11;
  
  puVar4 = (undefined1 *)(param_2 + 4U & 0xfffffffc);
  uVar10 = ((param_2 + 4) - (int)puVar4) * 8;
  uVar9 = CONCAT31(CONCAT21(CONCAT11(*puVar4,puVar4[1]),puVar4[2]),puVar4[3]);
  puVar5 = puVar4 + 8;
  uVar8 = CONCAT31(CONCAT21(CONCAT11(puVar4[4],puVar4[5]),puVar4[6]),puVar4[7]);
  if ((int)uVar10 < 0) {
    uVar9 = uVar9 >> (~uVar10 & 0x1f) + 1;
  }
  else {
    uVar9 = uVar9 << (uVar10 & 0x1f);
  }
  if ((int)uVar10 < 0x10) {
    uVar3 = uVar9 << 0x10;
    *param_1 = uVar9 >> 0x10;
    uVar10 = uVar10 + 0x10;
  }
  else {
    uVar10 = uVar10 - 0x10;
    if (uVar10 == 0) {
      *param_1 = uVar9 >> 0x10;
      uVar3 = uVar8;
    }
    else {
      uVar3 = -(0x10 - uVar10);
      if ((int)uVar3 < 0) {
        uVar3 = uVar8 >> (~uVar3 & 0x1f) + 1;
      }
      else {
        uVar3 = uVar8 << (uVar3 & 0x1f);
      }
      *param_1 = (uVar9 | uVar3) >> 0x10;
      if ((int)uVar10 < 0) {
        uVar3 = uVar8 >> (~uVar10 & 0x1f) + 1;
      }
      else {
        uVar3 = uVar8 << (uVar10 & 0x1f);
      }
    }
    uVar1 = *puVar5;
    puVar5 = puVar4 + 0xc;
    uVar8 = CONCAT31(CONCAT21(CONCAT11(uVar1,puVar4[9]),puVar4[10]),puVar4[0xb]);
  }
  uVar9 = uVar10 + 1;
  if ((int)uVar9 < 0x20) {
    uVar3 = uVar3 << 1;
  }
  else {
    uVar1 = *puVar5;
    puVar4 = puVar5 + 1;
    uVar9 = uVar10 - 0x1f;
    puVar6 = puVar5 + 2;
    if ((int)uVar9 < 0) {
      uVar3 = uVar8 >> (~uVar9 & 0x1f) + 1;
    }
    else {
      uVar3 = uVar8 << (uVar9 & 0x1f);
    }
    puVar7 = puVar5 + 3;
    puVar5 = puVar5 + 4;
    uVar8 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar4),*puVar6),*puVar7);
  }
  if ((int)uVar9 < 10) {
    uVar10 = uVar3 << 0x16;
    param_1[1] = uVar3 >> 10;
    uVar9 = uVar9 + 0x16;
  }
  else {
    uVar9 = uVar9 - 10;
    if (uVar9 == 0) {
      param_1[1] = uVar3 >> 10;
      uVar10 = uVar8;
    }
    else {
      uVar10 = -(0x16 - uVar9);
      if ((int)uVar10 < 0) {
        uVar10 = uVar8 >> (~uVar10 & 0x1f) + 1;
      }
      else {
        uVar10 = uVar8 << (uVar10 & 0x1f);
      }
      param_1[1] = (uVar3 | uVar10) >> 10;
      if ((int)uVar9 < 0) {
        uVar10 = uVar8 >> (~uVar9 & 0x1f) + 1;
      }
      else {
        uVar10 = uVar8 << (uVar9 & 0x1f);
      }
    }
    uVar1 = *puVar5;
    puVar4 = puVar5 + 1;
    puVar6 = puVar5 + 2;
    puVar7 = puVar5 + 3;
    puVar5 = puVar5 + 4;
    uVar8 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar4),*puVar6),*puVar7);
  }
  uVar3 = uVar9 + 1;
  if ((int)uVar3 < 0x20) {
    uVar10 = uVar10 << 1;
  }
  else {
    uVar1 = *puVar5;
    puVar4 = puVar5 + 1;
    uVar3 = uVar9 - 0x1f;
    puVar6 = puVar5 + 2;
    if ((int)uVar3 < 0) {
      uVar10 = uVar8 >> (~uVar3 & 0x1f) + 1;
    }
    else {
      uVar10 = uVar8 << (uVar3 & 0x1f);
    }
    puVar7 = puVar5 + 3;
    puVar5 = puVar5 + 4;
    uVar8 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar4),*puVar6),*puVar7);
  }
  if ((int)uVar3 < 0x1a) {
    uVar9 = uVar10 << 6;
    param_1[2] = uVar10 >> 0x1a;
    uVar3 = uVar3 + 6;
  }
  else {
    uVar3 = uVar3 - 0x1a;
    if (uVar3 == 0) {
      param_1[2] = uVar10 >> 0x1a;
      uVar9 = uVar8;
    }
    else {
      uVar9 = -(6 - uVar3);
      if ((int)uVar9 < 0) {
        uVar9 = uVar8 >> (~uVar9 & 0x1f) + 1;
      }
      else {
        uVar9 = uVar8 << (uVar9 & 0x1f);
      }
      param_1[2] = (uVar10 | uVar9) >> 0x1a;
      if ((int)uVar3 < 0) {
        uVar9 = uVar8 >> (~uVar3 & 0x1f) + 1;
      }
      else {
        uVar9 = uVar8 << (uVar3 & 0x1f);
      }
    }
    uVar1 = *puVar5;
    puVar4 = puVar5 + 1;
    puVar6 = puVar5 + 2;
    puVar7 = puVar5 + 3;
    puVar5 = puVar5 + 4;
    uVar8 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar4),*puVar6),*puVar7);
  }
  param_1[4] = -1 < (int)uVar9 ^ 1;
  if (uVar3 == 0x1f) {
    uVar1 = *puVar5;
    puVar4 = puVar5 + 1;
    iVar11 = 0;
    puVar6 = puVar5 + 2;
    puVar7 = puVar5 + 3;
    puVar5 = puVar5 + 4;
    uVar10 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar4),*puVar6),*puVar7);
    uVar9 = uVar8;
  }
  else {
    uVar9 = uVar9 << 1;
    iVar11 = uVar3 + 1;
    uVar10 = uVar8;
  }
  param_1[5] = -1 < (int)uVar9 ^ 1;
  if (iVar11 == 0x1f) {
    uVar1 = *puVar5;
    puVar4 = puVar5 + 1;
    iVar11 = 0;
    puVar6 = puVar5 + 2;
    puVar7 = puVar5 + 3;
    puVar5 = puVar5 + 4;
    uVar8 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar4),*puVar6),*puVar7);
    uVar9 = uVar10;
  }
  else {
    uVar9 = uVar9 << 1;
    iVar11 = iVar11 + 1;
    uVar8 = uVar10;
  }
  param_1[6] = -1 < (int)uVar9 ^ 1;
  if (iVar11 == 0x1f) {
    uVar1 = *puVar5;
    puVar4 = puVar5 + 1;
    iVar11 = 0;
    puVar6 = puVar5 + 2;
    puVar7 = puVar5 + 3;
    puVar5 = puVar5 + 4;
    uVar10 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar4),*puVar6),*puVar7);
    uVar9 = uVar8;
  }
  else {
    uVar9 = uVar9 << 1;
    iVar11 = iVar11 + 1;
    uVar10 = uVar8;
  }
  param_1[7] = -1 < (int)uVar9 ^ 1;
  if (iVar11 == 0x1f) {
    uVar1 = *puVar5;
    puVar4 = puVar5 + 1;
    iVar11 = 0;
    puVar6 = puVar5 + 2;
    puVar7 = puVar5 + 3;
    puVar5 = puVar5 + 4;
    uVar8 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar4),*puVar6),*puVar7);
    uVar9 = uVar10;
  }
  else {
    uVar9 = uVar9 << 1;
    iVar11 = iVar11 + 1;
    uVar8 = uVar10;
  }
  uVar10 = iVar11 + 1;
  if ((int)uVar10 < 0x20) {
    uVar9 = uVar9 << 1;
  }
  else {
    uVar1 = *puVar5;
    puVar4 = puVar5 + 1;
    uVar10 = iVar11 - 0x1f;
    puVar6 = puVar5 + 2;
    if ((int)uVar10 < 0) {
      uVar9 = uVar8 >> (~uVar10 & 0x1f) + 1;
    }
    else {
      uVar9 = uVar8 << (uVar10 & 0x1f);
    }
    puVar7 = puVar5 + 3;
    puVar5 = puVar5 + 4;
    uVar8 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar4),*puVar6),*puVar7);
  }
  if ((int)uVar10 < 0x1b) {
    uVar3 = uVar9 << 5;
    param_1[3] = uVar9 >> 0x1b;
    uVar10 = uVar10 + 5;
  }
  else {
    uVar10 = uVar10 - 0x1b;
    if (uVar10 == 0) {
      param_1[3] = uVar9 >> 0x1b;
      uVar3 = uVar8;
    }
    else {
      uVar3 = -(5 - uVar10);
      if ((int)uVar3 < 0) {
        uVar3 = uVar8 >> (~uVar3 & 0x1f) + 1;
      }
      else {
        uVar3 = uVar8 << (uVar3 & 0x1f);
      }
      param_1[3] = (uVar9 | uVar3) >> 0x1b;
      if ((int)uVar10 < 0) {
        uVar3 = uVar8 >> (~uVar10 & 0x1f) + 1;
      }
      else {
        uVar3 = uVar8 << (uVar10 & 0x1f);
      }
    }
    uVar1 = *puVar5;
    puVar4 = puVar5 + 1;
    puVar6 = puVar5 + 2;
    puVar7 = puVar5 + 3;
    puVar5 = puVar5 + 4;
    uVar8 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar4),*puVar6),*puVar7);
  }
  uVar9 = uVar10 + 8;
  if (0x1f < (int)uVar9) {
    uVar9 = uVar10 - 0x18;
    uVar2 = CONCAT21(CONCAT11(*puVar5,puVar5[1]),puVar5[2]);
    if ((int)uVar9 < 0) {
      uVar3 = uVar8 >> (~uVar9 & 0x1f) + 1;
    }
    else {
      uVar3 = uVar8 << (uVar9 & 0x1f);
    }
    do {
      puVar4 = puVar5 + 3;
      puVar5 = puVar5 + 4;
      uVar8 = CONCAT31(uVar2,*puVar4);
LAB_8ced6fa0:
      while( true ) {
        if (-1 < (int)uVar3) {
          puVar5 = puVar5 + ((int)(uVar9 + 7) >> 3) + -8;
          *param_3 = (int)puVar5 - param_2;
          iVar11 = (*(code *)PTR_FUN_8ced701c)(puVar5);
          if ((iVar11 == 0) &&
             (iVar11 = (*(code *)PTR_FUN_8ced701c)(puVar5 + 1), iVar11 == DAT_8ced7020)) {
            *param_3 = *param_3 + 1;
          }
          return;
        }
        if (0x1f < (int)(uVar9 + 0x18)) break;
        uVar3 = uVar3 << 0x18;
        uVar9 = uVar9 + 0x18;
      }
      uVar9 = uVar9 - 8;
      if ((int)uVar9 < 0) {
        uVar3 = uVar8 >> (~uVar9 & 0x1f) + 1;
      }
      else {
        uVar3 = uVar8 << (uVar9 & 0x1f);
      }
      uVar2 = CONCAT21(CONCAT11(*puVar5,puVar5[1]),puVar5[2]);
    } while( true );
  }
  uVar3 = uVar3 << 8;
  goto LAB_8ced6fa0;
}


/* Address: 8ced7024 */

/* WARNING: Removing unreachable block (ram,0x8ced7472) */
/* WARNING: Removing unreachable block (ram,0x8ced7388) */
/* WARNING: Removing unreachable block (ram,0x8ced7354) */
/* WARNING: Removing unreachable block (ram,0x8ced77b0) */
/* WARNING: Removing unreachable block (ram,0x8ced76c6) */
/* WARNING: Removing unreachable block (ram,0x8ced7692) */
/* WARNING: Removing unreachable block (ram,0x8ced763c) */
/* WARNING: Removing unreachable block (ram,0x8ced7586) */
/* WARNING: Removing unreachable block (ram,0x8ced74de) */
/* WARNING: Removing unreachable block (ram,0x8ced72e8) */
/* WARNING: Removing unreachable block (ram,0x8ced72ce) */
/* WARNING: Removing unreachable block (ram,0x8ced72da) */
/* WARNING: Removing unreachable block (ram,0x8ced74d2) */
/* WARNING: Removing unreachable block (ram,0x8ced7506) */
/* WARNING: Removing unreachable block (ram,0x8ced7604) */
/* WARNING: Removing unreachable block (ram,0x8ced763e) */
/* WARNING: Removing unreachable block (ram,0x8ced769e) */
/* WARNING: Removing unreachable block (ram,0x8ced7746) */
/* WARNING: Removing unreachable block (ram,0x8ced77b2) */
/* WARNING: Removing unreachable block (ram,0x8ced7360) */
/* WARNING: Removing unreachable block (ram,0x8ced7408) */
/* WARNING: Removing unreachable block (ram,0x8ced7474) */

void FUN_8ced7024(uint *param_1,int param_2,undefined4 *param_3)

{
  undefined1 uVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  undefined1 *puVar5;
  undefined1 *puVar6;
  undefined1 *puVar7;
  undefined1 *puVar8;
  uint uVar9;
  uint uVar10;
  uint uVar11;
  int iVar12;
  uint local_28;
  uint local_24;
  uint local_20;
  
  puVar5 = (undefined1 *)(param_2 + 3U & 0xfffffffc);
  uVar9 = CONCAT31(CONCAT21(CONCAT11(*puVar5,puVar5[1]),puVar5[2]),puVar5[3]);
  uVar11 = ((param_2 + 3) - (int)puVar5) * 8;
  puVar6 = puVar5 + 8;
  uVar10 = CONCAT31(CONCAT21(CONCAT11(puVar5[4],puVar5[5]),puVar5[6]),puVar5[7]);
  if ((int)uVar11 < 0) {
    uVar9 = uVar9 >> (~uVar11 & 0x1f) + 1;
  }
  else {
    uVar9 = uVar9 << (uVar11 & 0x1f);
  }
  if ((int)uVar11 < 0x18) {
    uVar2 = uVar9 << 8;
    uVar11 = uVar11 + 8;
  }
  else {
    uVar11 = uVar11 - 0x18;
    if (uVar11 != 0) {
      uVar2 = -(8 - uVar11);
      if ((int)uVar2 < 0) {
        uVar2 = uVar10 >> (~uVar2 & 0x1f) + 1;
      }
      else {
        uVar2 = uVar10 << (uVar2 & 0x1f);
      }
      uVar9 = uVar9 | uVar2;
      if ((int)uVar11 < 0) {
        uVar10 = uVar10 >> (~uVar11 & 0x1f) + 1;
      }
      else {
        uVar10 = uVar10 << (uVar11 & 0x1f);
      }
    }
    uVar1 = *puVar6;
    puVar6 = puVar5 + 0xc;
    uVar2 = uVar10;
    uVar10 = CONCAT31(CONCAT21(CONCAT11(uVar1,puVar5[9]),puVar5[10]),puVar5[0xb]);
  }
  uVar9 = uVar9 >> 0x18;
  iVar3 = (int)DAT_8ced712c;
  *param_1 = uVar9;
  if (((int)uVar9 < iVar3) || ((int)DAT_8ced712e < (int)uVar9)) {
    if (((int)uVar9 < (int)DAT_8ced7132) || ((int)DAT_8ced7134 < (int)uVar9)) {
      if (uVar9 == (int)DAT_8ced7138) {
        local_24 = 1;
        local_28 = 2;
      }
      else if (uVar9 == (int)DAT_8ced713a) {
        local_24 = 2;
        local_28 = 2;
      }
      else {
        if (uVar9 == (int)DAT_8ced713c) {
          local_28 = 3;
        }
        else {
          local_28 = 4;
        }
        local_24 = 0;
      }
    }
    else {
      local_28 = 0;
      local_24 = (int)DAT_8ced7136 + uVar9;
    }
  }
  else {
    local_28 = 1;
    local_24 = (int)DAT_8ced7130 + uVar9;
  }
  param_1[1] = local_28;
  param_1[2] = local_24;
  if ((int)uVar11 < 0x10) {
    local_24 = uVar2 << 0x10;
    param_1[3] = uVar2 >> 0x10;
    uVar11 = uVar11 + 0x10;
  }
  else {
    uVar11 = uVar11 - 0x10;
    if (uVar11 == 0) {
      param_1[3] = uVar2 >> 0x10;
    }
    else {
      uVar4 = -(0x10 - uVar11);
      if ((int)uVar4 < 0) {
        uVar4 = uVar10 >> (~uVar4 & 0x1f) + 1;
      }
      else {
        uVar4 = uVar10 << (uVar4 & 0x1f);
      }
      param_1[3] = (uVar2 | uVar4) >> 0x10;
      if ((int)uVar11 < 0) {
        uVar10 = uVar10 >> (~uVar11 & 0x1f) + 1;
      }
      else {
        uVar10 = uVar10 << (uVar11 & 0x1f);
      }
    }
    uVar1 = *puVar6;
    puVar5 = puVar6 + 1;
    puVar7 = puVar6 + 2;
    puVar8 = puVar6 + 3;
    puVar6 = puVar6 + 4;
    local_24 = uVar10;
    uVar10 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar5),*puVar7),*puVar8);
  }
  if ((uVar9 == (int)DAT_8ced7240) || (uVar9 == (int)DAT_8ced7242)) {
    *param_3 = 6;
    uVar10 = param_1[3];
    goto LAB_8ced77ea;
  }
  while( true ) {
    uVar9 = local_24 >> 0x18;
    if (0x18 < (int)uVar11) {
      uVar2 = -(0x38 - uVar11);
      if ((int)uVar2 < 0) {
        uVar2 = uVar10 >> (~uVar2 & 0x1f) + 1;
      }
      else {
        uVar2 = uVar10 << (uVar2 & 0x1f);
      }
      uVar9 = uVar9 | uVar2;
    }
    if (uVar9 != (int)DAT_8ced7244) break;
    if ((int)(uVar11 + 8) < 0x20) {
      local_24 = local_24 << 8;
      uVar11 = uVar11 + 8;
    }
    else {
      uVar1 = *puVar6;
      puVar5 = puVar6 + 1;
      uVar11 = uVar11 - 0x18;
      puVar7 = puVar6 + 2;
      if ((int)uVar11 < 0) {
        local_24 = uVar10 >> (~uVar11 & 0x1f) + 1;
      }
      else {
        local_24 = uVar10 << (uVar11 & 0x1f);
      }
      puVar8 = puVar6 + 3;
      puVar6 = puVar6 + 4;
      uVar10 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar5),*puVar7),*puVar8);
    }
  }
  uVar9 = (uint)((local_24 & 0x80000000) != 0) << 1 | (uint)((local_24 & 0x40000000) != 0);
  if (0x1e < (int)uVar11) {
    uVar2 = -(0x3e - uVar11);
    if ((int)uVar2 < 0) {
      uVar2 = uVar10 >> (~uVar2 & 0x1f) + 1;
    }
    else {
      uVar2 = uVar10 << (uVar2 & 0x1f);
    }
    uVar9 = uVar9 | uVar2;
  }
  if (uVar9 == 1) {
    uVar9 = uVar11 + 2;
    if ((int)uVar9 < 0x20) {
      uVar2 = local_24 << 2;
    }
    else {
      uVar1 = *puVar6;
      puVar5 = puVar6 + 1;
      uVar9 = uVar11 - 0x1e;
      puVar7 = puVar6 + 2;
      if ((int)uVar9 < 0) {
        uVar2 = uVar10 >> (~uVar9 & 0x1f) + 1;
      }
      else {
        uVar2 = uVar10 << (uVar9 & 0x1f);
      }
      puVar8 = puVar6 + 3;
      puVar6 = puVar6 + 4;
      uVar10 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar5),*puVar7),*puVar8);
    }
    if (uVar9 == 0x1f) {
      uVar1 = *puVar6;
      puVar5 = puVar6 + 1;
      iVar3 = 0;
      puVar7 = puVar6 + 2;
      puVar8 = puVar6 + 3;
      puVar6 = puVar6 + 4;
      uVar4 = uVar10;
      uVar10 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar5),*puVar7),*puVar8);
    }
    else {
      uVar4 = uVar2 << 1;
      iVar3 = uVar9 + 1;
    }
    if (iVar3 < 0x13) {
      local_24 = uVar4 << 0xd;
      uVar11 = iVar3 + 0xd;
    }
    else {
      uVar11 = iVar3 - 0x13;
      if (uVar11 != 0) {
        uVar9 = -(0xd - uVar11);
        if ((int)uVar9 < 0) {
          uVar9 = uVar10 >> (~uVar9 & 0x1f) + 1;
        }
        else {
          uVar9 = uVar10 << (uVar9 & 0x1f);
        }
        uVar4 = uVar4 | uVar9;
        if ((int)uVar11 < 0) {
          uVar10 = uVar10 >> (~uVar11 & 0x1f) + 1;
        }
        else {
          uVar10 = uVar10 << (uVar11 & 0x1f);
        }
      }
      uVar1 = *puVar6;
      puVar5 = puVar6 + 1;
      puVar7 = puVar6 + 2;
      puVar8 = puVar6 + 3;
      puVar6 = puVar6 + 4;
      local_24 = uVar10;
      uVar10 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar5),*puVar7),*puVar8);
    }
    uVar9 = (uVar4 >> 0x13) << 7;
    if ((int)uVar2 < 0) {
      uVar9 = (uVar4 >> 0x13) << 10;
    }
    param_1[4] = uVar9;
  }
  local_28 = local_24 >> 0x1c;
  if (0x1c < (int)uVar11) {
    uVar9 = -(0x3c - uVar11);
    if ((int)uVar9 < 0) {
      uVar9 = uVar10 >> (~uVar9 & 0x1f) + 1;
    }
    else {
      uVar9 = uVar10 << (uVar9 & 0x1f);
    }
    local_28 = local_28 | uVar9;
  }
  if (local_28 == 2) {
    uVar9 = uVar11 + 4;
    if ((int)uVar9 < 0x20) {
      local_24 = local_24 << 4;
    }
    else {
      uVar1 = *puVar6;
      puVar5 = puVar6 + 1;
      uVar9 = uVar11 - 0x1c;
      puVar7 = puVar6 + 2;
      if ((int)uVar9 < 0) {
        local_24 = uVar10 >> (~uVar9 & 0x1f) + 1;
      }
      else {
        local_24 = uVar10 << (uVar9 & 0x1f);
      }
      puVar8 = puVar6 + 3;
      puVar6 = puVar6 + 4;
      uVar10 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar5),*puVar7),*puVar8);
    }
    if ((int)uVar9 < 0x1d) {
      local_28 = local_24 << 3;
      uVar9 = uVar9 + 3;
    }
    else {
      uVar9 = uVar9 - 0x1d;
      if (uVar9 != 0) {
        uVar11 = -(3 - uVar9);
        if ((int)uVar11 < 0) {
          uVar11 = uVar10 >> (~uVar11 & 0x1f) + 1;
        }
        else {
          uVar11 = uVar10 << (uVar11 & 0x1f);
        }
        local_24 = local_24 | uVar11;
        if ((int)uVar9 < 0) {
          uVar10 = uVar10 >> (~uVar9 & 0x1f) + 1;
        }
        else {
          uVar10 = uVar10 << (uVar9 & 0x1f);
        }
      }
      uVar1 = *puVar6;
      puVar5 = puVar6 + 1;
      puVar7 = puVar6 + 2;
      puVar8 = puVar6 + 3;
      puVar6 = puVar6 + 4;
      local_28 = uVar10;
      uVar10 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar5),*puVar7),*puVar8);
    }
    local_24 = local_24 >> 0x1d;
    uVar11 = uVar9 + 1;
    if ((int)uVar11 < 0x20) {
      local_28 = local_28 << 1;
    }
    else {
      uVar1 = *puVar6;
      puVar5 = puVar6 + 1;
      uVar11 = uVar9 - 0x1f;
      puVar7 = puVar6 + 2;
      if ((int)uVar11 < 0) {
        local_28 = uVar10 >> (~uVar11 & 0x1f) + 1;
      }
      else {
        local_28 = uVar10 << (uVar11 & 0x1f);
      }
      puVar8 = puVar6 + 3;
      puVar6 = puVar6 + 4;
      uVar10 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar5),*puVar7),*puVar8);
    }
    if ((int)uVar11 < 0x11) {
      uVar9 = local_28 << 0xf;
      uVar11 = uVar11 + 0xf;
    }
    else {
      uVar11 = uVar11 - 0x11;
      if (uVar11 != 0) {
        uVar9 = -(0xf - uVar11);
        if ((int)uVar9 < 0) {
          uVar9 = uVar10 >> (~uVar9 & 0x1f) + 1;
        }
        else {
          uVar9 = uVar10 << (uVar9 & 0x1f);
        }
        local_28 = local_28 | uVar9;
        if ((int)uVar11 < 0) {
          uVar10 = uVar10 >> (~uVar11 & 0x1f) + 1;
        }
        else {
          uVar10 = uVar10 << (uVar11 & 0x1f);
        }
      }
      uVar1 = *puVar6;
      puVar5 = puVar6 + 1;
      puVar7 = puVar6 + 2;
      puVar8 = puVar6 + 3;
      puVar6 = puVar6 + 4;
      uVar9 = uVar10;
      uVar10 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar5),*puVar7),*puVar8);
    }
    local_28 = local_28 >> 0x11;
    uVar2 = uVar11 + 1;
    if ((int)uVar2 < 0x20) {
      uVar9 = uVar9 << 1;
    }
    else {
      uVar1 = *puVar6;
      puVar5 = puVar6 + 1;
      uVar2 = uVar11 - 0x1f;
      puVar7 = puVar6 + 2;
      if ((int)uVar2 < 0) {
        uVar9 = uVar10 >> (~uVar2 & 0x1f) + 1;
      }
      else {
        uVar9 = uVar10 << (uVar2 & 0x1f);
      }
      puVar8 = puVar6 + 3;
      puVar6 = puVar6 + 4;
      uVar10 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar5),*puVar7),*puVar8);
    }
    if ((int)uVar2 < 0x11) {
      iVar3 = uVar2 + 0xf;
    }
    else {
      iVar3 = uVar2 - 0x11;
      if (iVar3 != 0) {
        uVar11 = -(0xf - iVar3);
        if ((int)uVar11 < 0) {
          uVar10 = uVar10 >> (~uVar11 & 0x1f) + 1;
        }
        else {
          uVar10 = uVar10 << (uVar11 & 0x1f);
        }
        uVar9 = uVar9 | uVar10;
      }
      puVar6 = puVar6 + 4;
    }
    iVar12 = iVar3 + 1;
    if (0x1f < iVar12) {
      puVar6 = puVar6 + 4;
      iVar12 = iVar3 + -0x1f;
    }
    param_1[5] = local_24 << 0x1c | local_28 << 0xd | (int)(uVar9 >> 0x11) >> 2;
LAB_8ced77ce:
    param_1[6] = 0xffffffff;
  }
  else {
    if (local_28 != 3) {
      iVar12 = uVar11 + 8;
      if (0x1f < iVar12) {
        puVar6 = puVar6 + 4;
        iVar12 = uVar11 - 0x18;
      }
      param_1[5] = 0xffffffff;
      goto LAB_8ced77ce;
    }
    uVar9 = uVar11 + 4;
    if ((int)uVar9 < 0x20) {
      local_24 = local_24 << 4;
    }
    else {
      uVar1 = *puVar6;
      puVar5 = puVar6 + 1;
      uVar9 = uVar11 - 0x1c;
      puVar7 = puVar6 + 2;
      if ((int)uVar9 < 0) {
        local_24 = uVar10 >> (~uVar9 & 0x1f) + 1;
      }
      else {
        local_24 = uVar10 << (uVar9 & 0x1f);
      }
      puVar8 = puVar6 + 3;
      puVar6 = puVar6 + 4;
      uVar10 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar5),*puVar7),*puVar8);
    }
    if ((int)uVar9 < 0x1d) {
      local_28 = local_24 << 3;
      uVar9 = uVar9 + 3;
    }
    else {
      uVar9 = uVar9 - 0x1d;
      if (uVar9 != 0) {
        uVar11 = -(3 - uVar9);
        if ((int)uVar11 < 0) {
          uVar11 = uVar10 >> (~uVar11 & 0x1f) + 1;
        }
        else {
          uVar11 = uVar10 << (uVar11 & 0x1f);
        }
        local_24 = local_24 | uVar11;
        if ((int)uVar9 < 0) {
          uVar10 = uVar10 >> (~uVar9 & 0x1f) + 1;
        }
        else {
          uVar10 = uVar10 << (uVar9 & 0x1f);
        }
      }
      uVar1 = *puVar6;
      puVar5 = puVar6 + 1;
      puVar7 = puVar6 + 2;
      puVar8 = puVar6 + 3;
      puVar6 = puVar6 + 4;
      local_28 = uVar10;
      uVar10 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar5),*puVar7),*puVar8);
    }
    local_20 = local_24 >> 0x1d;
    uVar11 = uVar9 + 1;
    if ((int)uVar11 < 0x20) {
      local_28 = local_28 << 1;
    }
    else {
      uVar1 = *puVar6;
      puVar5 = puVar6 + 1;
      uVar11 = uVar9 - 0x1f;
      puVar7 = puVar6 + 2;
      if ((int)uVar11 < 0) {
        local_28 = uVar10 >> (~uVar11 & 0x1f) + 1;
      }
      else {
        local_28 = uVar10 << (uVar11 & 0x1f);
      }
      puVar8 = puVar6 + 3;
      puVar6 = puVar6 + 4;
      uVar10 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar5),*puVar7),*puVar8);
    }
    if ((int)uVar11 < 0x11) {
      local_24 = local_28 << 0xf;
      uVar11 = uVar11 + 0xf;
    }
    else {
      uVar11 = uVar11 - 0x11;
      if (uVar11 != 0) {
        uVar9 = -(0xf - uVar11);
        if ((int)uVar9 < 0) {
          uVar9 = uVar10 >> (~uVar9 & 0x1f) + 1;
        }
        else {
          uVar9 = uVar10 << (uVar9 & 0x1f);
        }
        local_28 = local_28 | uVar9;
        if ((int)uVar11 < 0) {
          uVar10 = uVar10 >> (~uVar11 & 0x1f) + 1;
        }
        else {
          uVar10 = uVar10 << (uVar11 & 0x1f);
        }
      }
      uVar1 = *puVar6;
      puVar5 = puVar6 + 1;
      puVar7 = puVar6 + 2;
      puVar8 = puVar6 + 3;
      puVar6 = puVar6 + 4;
      local_24 = uVar10;
      uVar10 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar5),*puVar7),*puVar8);
    }
    local_28 = local_28 >> 0x11;
    uVar9 = uVar11 + 1;
    if ((int)uVar9 < 0x20) {
      local_24 = local_24 << 1;
    }
    else {
      uVar1 = *puVar6;
      puVar5 = puVar6 + 1;
      uVar9 = uVar11 - 0x1f;
      puVar7 = puVar6 + 2;
      if ((int)uVar9 < 0) {
        local_24 = uVar10 >> (~uVar9 & 0x1f) + 1;
      }
      else {
        local_24 = uVar10 << (uVar9 & 0x1f);
      }
      puVar8 = puVar6 + 3;
      puVar6 = puVar6 + 4;
      uVar10 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar5),*puVar7),*puVar8);
    }
    if ((int)uVar9 < 0x11) {
      uVar11 = local_24 << 0xf;
      uVar9 = uVar9 + 0xf;
    }
    else {
      uVar9 = uVar9 - 0x11;
      if (uVar9 != 0) {
        uVar11 = -(0xf - uVar9);
        if ((int)uVar11 < 0) {
          uVar11 = uVar10 >> (~uVar11 & 0x1f) + 1;
        }
        else {
          uVar11 = uVar10 << (uVar11 & 0x1f);
        }
        local_24 = local_24 | uVar11;
        if ((int)uVar9 < 0) {
          uVar10 = uVar10 >> (~uVar9 & 0x1f) + 1;
        }
        else {
          uVar10 = uVar10 << (uVar9 & 0x1f);
        }
      }
      uVar1 = *puVar6;
      puVar5 = puVar6 + 1;
      puVar7 = puVar6 + 2;
      puVar8 = puVar6 + 3;
      puVar6 = puVar6 + 4;
      uVar11 = uVar10;
      uVar10 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar5),*puVar7),*puVar8);
    }
    local_24 = local_24 >> 0x11;
    uVar2 = uVar9 + 1;
    if ((int)uVar2 < 0x20) {
      uVar11 = uVar11 << 1;
    }
    else {
      uVar1 = *puVar6;
      puVar5 = puVar6 + 1;
      uVar2 = uVar9 - 0x1f;
      puVar7 = puVar6 + 2;
      if ((int)uVar2 < 0) {
        uVar11 = uVar10 >> (~uVar2 & 0x1f) + 1;
      }
      else {
        uVar11 = uVar10 << (uVar2 & 0x1f);
      }
      puVar8 = puVar6 + 3;
      puVar6 = puVar6 + 4;
      uVar10 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar5),*puVar7),*puVar8);
    }
    uVar9 = uVar2 + 4;
    param_1[5] = local_20 << 0x1c | local_28 << 0xd | (int)local_24 >> 2;
    if ((int)uVar9 < 0x20) {
      uVar11 = uVar11 << 4;
    }
    else {
      uVar1 = *puVar6;
      puVar5 = puVar6 + 1;
      uVar9 = uVar2 - 0x1c;
      puVar7 = puVar6 + 2;
      if ((int)uVar9 < 0) {
        uVar11 = uVar10 >> (~uVar9 & 0x1f) + 1;
      }
      else {
        uVar11 = uVar10 << (uVar9 & 0x1f);
      }
      puVar8 = puVar6 + 3;
      puVar6 = puVar6 + 4;
      uVar10 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar5),*puVar7),*puVar8);
    }
    if ((int)uVar9 < 0x1d) {
      local_24 = uVar11 << 3;
      uVar9 = uVar9 + 3;
    }
    else {
      uVar9 = uVar9 - 0x1d;
      if (uVar9 != 0) {
        uVar2 = -(3 - uVar9);
        if ((int)uVar2 < 0) {
          uVar2 = uVar10 >> (~uVar2 & 0x1f) + 1;
        }
        else {
          uVar2 = uVar10 << (uVar2 & 0x1f);
        }
        uVar11 = uVar11 | uVar2;
        if ((int)uVar9 < 0) {
          uVar10 = uVar10 >> (~uVar9 & 0x1f) + 1;
        }
        else {
          uVar10 = uVar10 << (uVar9 & 0x1f);
        }
      }
      uVar1 = *puVar6;
      puVar5 = puVar6 + 1;
      puVar7 = puVar6 + 2;
      puVar8 = puVar6 + 3;
      puVar6 = puVar6 + 4;
      local_24 = uVar10;
      uVar10 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar5),*puVar7),*puVar8);
    }
    local_28 = uVar11 >> 0x1d;
    uVar11 = uVar9 + 1;
    if ((int)uVar11 < 0x20) {
      local_24 = local_24 << 1;
    }
    else {
      uVar1 = *puVar6;
      puVar5 = puVar6 + 1;
      uVar11 = uVar9 - 0x1f;
      puVar7 = puVar6 + 2;
      if ((int)uVar11 < 0) {
        local_24 = uVar10 >> (~uVar11 & 0x1f) + 1;
      }
      else {
        local_24 = uVar10 << (uVar11 & 0x1f);
      }
      puVar8 = puVar6 + 3;
      puVar6 = puVar6 + 4;
      uVar10 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar5),*puVar7),*puVar8);
    }
    if ((int)uVar11 < 0x11) {
      uVar9 = local_24 << 0xf;
      uVar11 = uVar11 + 0xf;
    }
    else {
      uVar11 = uVar11 - 0x11;
      if (uVar11 != 0) {
        uVar9 = -(0xf - uVar11);
        if ((int)uVar9 < 0) {
          uVar9 = uVar10 >> (~uVar9 & 0x1f) + 1;
        }
        else {
          uVar9 = uVar10 << (uVar9 & 0x1f);
        }
        local_24 = local_24 | uVar9;
        if ((int)uVar11 < 0) {
          uVar10 = uVar10 >> (~uVar11 & 0x1f) + 1;
        }
        else {
          uVar10 = uVar10 << (uVar11 & 0x1f);
        }
      }
      uVar1 = *puVar6;
      puVar5 = puVar6 + 1;
      puVar7 = puVar6 + 2;
      puVar8 = puVar6 + 3;
      puVar6 = puVar6 + 4;
      uVar9 = uVar10;
      uVar10 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar5),*puVar7),*puVar8);
    }
    local_24 = local_24 >> 0x11;
    uVar2 = uVar11 + 1;
    if ((int)uVar2 < 0x20) {
      uVar9 = uVar9 << 1;
    }
    else {
      uVar1 = *puVar6;
      puVar5 = puVar6 + 1;
      uVar2 = uVar11 - 0x1f;
      puVar7 = puVar6 + 2;
      if ((int)uVar2 < 0) {
        uVar9 = uVar10 >> (~uVar2 & 0x1f) + 1;
      }
      else {
        uVar9 = uVar10 << (uVar2 & 0x1f);
      }
      puVar8 = puVar6 + 3;
      puVar6 = puVar6 + 4;
      uVar10 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar5),*puVar7),*puVar8);
    }
    if ((int)uVar2 < 0x11) {
      iVar3 = uVar2 + 0xf;
    }
    else {
      iVar3 = uVar2 - 0x11;
      if (iVar3 != 0) {
        uVar11 = -(0xf - iVar3);
        if ((int)uVar11 < 0) {
          uVar10 = uVar10 >> (~uVar11 & 0x1f) + 1;
        }
        else {
          uVar10 = uVar10 << (uVar11 & 0x1f);
        }
        uVar9 = uVar9 | uVar10;
      }
      puVar6 = puVar6 + 4;
    }
    iVar12 = iVar3 + 1;
    if (0x1f < iVar12) {
      puVar6 = puVar6 + 4;
      iVar12 = iVar3 + -0x1f;
    }
    param_1[6] = local_28 << 0x1c | local_24 << 0xd | (int)(uVar9 >> 0x11) >> 2;
  }
  *param_3 = puVar6 + (((iVar12 + 7 >> 3) + -8) - param_2);
  uVar10 = (param_1[3] - (int)(puVar6 + (((iVar12 + 7 >> 3) + -8) - param_2))) + 6;
LAB_8ced77ea:
  param_1[7] = uVar10;
  return;
}


/* Address: 8ced7800 */

void FUN_8ced7800(void)

{
  return;
}


/* Address: 8ced7808 */

undefined4 callback_candidate_8ced7808(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = (*(code *)PTR_FUN_8ced78b0)(param_1,param_2);
  if (iVar1 != 0) {
    uVar2 = (*(code *)PTR_FUN_8ced78b8)(0,DAT_8ced78b4);
    return uVar2;
  }
  (*DAT_8ced78bc)();
  return 0;
}


/* Address: 8ced783a */

undefined4 callback_candidate_8ced783a(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = (*(code *)PTR_FUN_8ced78b0)(param_1,param_3,param_2);
  if (iVar1 != 0) {
    uVar2 = (*(code *)PTR_FUN_8ced78b8)(0,DAT_8ced78b4);
    return uVar2;
  }
  (*DAT_8ced78c0)();
  return 0;
}


/* Address: 8ced7878 */

undefined4 callback_candidate_8ced7878(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = (*(code *)PTR_FUN_8ced78b0)(param_1,param_2);
  if (iVar1 != 0) {
    uVar2 = (*(code *)PTR_FUN_8ced78b8)(0,DAT_8ced78b4);
    return uVar2;
  }
  (*DAT_8ced78c0)();
  return 0;
}


/* Address: 8ced78c4 */

int callback_candidate_8ced78c4(void)

{
  int iVar1;
  
  iVar1 = FUN_8ced78ea();
  if (iVar1 != 0) {
    return iVar1;
  }
  (*(code *)PTR_FUN_8ced79e8)();
  (*(code *)PTR_FUN_8ced79ec)();
  return 0;
}


/* Address: 8ced78ea */

undefined4 FUN_8ced78ea(int param_1,int param_2)

{
  int *piVar1;
  undefined *puVar2;
  int iVar3;
  undefined4 *puVar4;
  int iVar5;
  
  puVar2 = PTR_FUN_8ced79f4;
  piVar1 = DAT_8ced79f0;
  iVar5 = (int)DAT_8ced79e2;
  iVar3 = (int)DAT_8ced79e4;
  *DAT_8ced79f0 = param_2;
  (*(code *)puVar2)(*piVar1,0,(uint)((param_1 + -1) * iVar5 + iVar3) >> 2);
  (*DAT_8ced79fc)(*piVar1,PTR_s_MPS_Version_1_12_1999_03_08_8ced79f8,0x20);
  (*DAT_8ced79fc)(*piVar1 + 0x20,PTR_s_Copyright__c__1997_1999_CRI_8ced7a00,0x20);
  FUN_8ced79a4(*piVar1 + 0x40);
  iVar3 = 0;
  *(int *)(*piVar1 + 0x4c) = param_1;
  puVar4 = (undefined4 *)(*piVar1 + 0x50);
  if (0 < param_1) {
    do {
      iVar3 = iVar3 + 1;
      *puVar4 = 1;
      puVar4 = (undefined4 *)((int)puVar4 + iVar5);
    } while (iVar3 < param_1);
  }
  return 0;
}


/* Address: 8ced7954 */

void callback_candidate_8ced7954(void)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  int iVar5;
  
  iVar5 = (int)DAT_8ced79e2;
  iVar2 = *(int *)(*DAT_8ced79f0 + 0x4c);
  piVar1 = (int *)(*DAT_8ced79f0 + 0x50);
  piVar4 = piVar1;
  for (iVar3 = 0; iVar3 < iVar2; iVar3 = iVar3 + 1) {
    if (*piVar4 != 1) {
      callback_candidate_8ced7bb4(piVar1);
    }
    piVar4 = (int *)((int)piVar4 + iVar5);
    piVar1 = (int *)((int)piVar1 + iVar5);
  }
  (*(code *)PTR_FUN_8ced7a04)();
                    /* WARNING: Could not recover jumptable at 0x8ced79a0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)PTR_LAB_8ced7a08)();
  return;
}


/* Address: 8ced79a4 */

void FUN_8ced79a4(undefined4 *param_1)

{
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  return;
}


/* Address: 8ced79a8 */

void FUN_8ced79a8(undefined4 *param_1)

{
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  return;
}


/* Address: 8ced79b2 */

undefined4 FUN_8ced79b2(int param_1,undefined4 param_2)

{
  if (param_1 == 0) {
    FUN_8ced7a0c(*DAT_8ced79f0 + 0x40,param_2);
  }
  else {
    FUN_8ced7a0c(param_1 + 4,param_2);
  }
  return param_2;
}


/* Address: 8ced7a0c */

void FUN_8ced7a0c(undefined4 *param_1,int param_2)

{
  param_1[2] = param_2;
  if ((param_2 != 0) && ((code *)*param_1 != (code *)0x0)) {
    (*(code *)*param_1)(param_1[1]);
  }
  return;
}


/* Address: 8ced7a2c */

undefined4 callback_candidate_8ced7a2c(int param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  undefined4 uVar2;
  
  if (param_1 == 0) {
    FUN_8ced7a74(*DAT_8ced7af8 + 0x40,param_2,param_3);
  }
  else {
    iVar1 = FUN_8ced7ac4(param_1);
    if (iVar1 != 0) {
      uVar2 = FUN_8ced79b2(0,DAT_8ced7afc);
      return uVar2;
    }
    FUN_8ced7a74(param_1 + 4,param_2,param_3);
  }
  return 0;
}


/* Address: 8ced7a74 */

void FUN_8ced7a74(undefined4 *param_1,undefined4 param_2,undefined4 param_3)

{
  *param_1 = param_2;
  param_1[1] = param_3;
  return;
}


/* Address: 8ced7ac4 */

undefined4 FUN_8ced7ac4(int *param_1)

{
  *DAT_8ced7b08 = param_1;
  if ((param_1 != (int *)0x0) && (*param_1 != 1)) {
    return 0;
  }
  return 0xffffffff;
}


/* Address: 8ced7adc */

undefined4 callback_candidate_8ced7adc(void)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = FUN_8ced7b0c();
  if (iVar1 == 0) {
    return 0;
  }
  uVar2 = FUN_8ced7b3e(iVar1);
  return uVar2;
}


/* Address: 8ced7b0c */

int * FUN_8ced7b0c(void)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  
  iVar2 = 0;
  iVar3 = *(int *)(*DAT_8ced7bdc + 0x4c);
  piVar1 = (int *)(*DAT_8ced7bdc + 0x50);
  if (0 < iVar3) {
    do {
      if (*piVar1 == 1) {
        return piVar1;
      }
      iVar2 = iVar2 + 1;
      piVar1 = (int *)((int)piVar1 + (int)DAT_8ced7bd8);
    } while (iVar2 < iVar3);
  }
  return (int *)0x0;
}


/* Address: 8ced7b3e */

undefined4 * FUN_8ced7b3e(undefined4 *param_1)

{
  (*(code *)PTR_FUN_8ced7be0)(0,0x2f);
  *param_1 = 2;
  FUN_8ced79a8(param_1 + 1);
  FUN_8ced7b82(param_1 + 4);
  FUN_8ced7b8c(param_1 + 7);
  FUN_8ced7b8c(param_1 + 0xf);
  FUN_8ced7b8c(param_1 + 0x17);
  FUN_8ced7b8c(param_1 + 0x1f);
  FUN_8ced7ba0((int)DAT_8ced7bda + (int)param_1);
  return param_1;
}


/* Address: 8ced7b82 */

void FUN_8ced7b82(undefined4 *param_1)

{
  *param_1 = 0xffffffff;
  param_1[1] = 0xffffffff;
  param_1[2] = 0xffffffff;
  return;
}


/* Address: 8ced7b8c */

void FUN_8ced7b8c(undefined4 *param_1)

{
  *param_1 = 0xffffffff;
  param_1[1] = 0xffffffff;
  param_1[2] = 0xffffffff;
  param_1[3] = 0xffffffff;
  param_1[4] = 0xffffffff;
  param_1[5] = 0xffffffff;
  param_1[6] = 0xffffffff;
  param_1[7] = 0xffffffff;
  return;
}


/* Address: 8ced7ba0 */

void FUN_8ced7ba0(undefined4 *param_1)

{
  *param_1 = 0xffffffff;
  param_1[1] = 0xffffffff;
  param_1[2] = 0xffffffff;
  param_1[3] = 0xffffffff;
  param_1[4] = 0xffffffff;
  param_1[5] = 0xffffffff;
  param_1[6] = 0xffffffff;
  param_1[7] = 0xffffffff;
  return;
}


/* Address: 8ced7bb4 */

undefined4 callback_candidate_8ced7bb4(undefined4 *param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = FUN_8ced7ac4();
  if (iVar1 != 0) {
    uVar2 = FUN_8ced79b2(0,DAT_8ced7be4);
    return uVar2;
  }
  *param_1 = 1;
  return 0;
}


/* Address: 8ced7be8 */

int FUN_8ced7be8(undefined1 *param_1)

{
  int iVar1;
  
  iVar1 = CONCAT31(CONCAT21(CONCAT11(*param_1,param_1[1]),param_1[2]),param_1[3]);
  if (iVar1 == DAT_8ced7cac) {
    iVar1 = 4;
  }
  else if (iVar1 == DAT_8ced7cae) {
    iVar1 = 3;
  }
  else if ((DAT_8ced7cae < iVar1) && (iVar1 <= DAT_8ced7cb0)) {
    iVar1 = 1;
  }
  else if (iVar1 == DAT_8ced7cb2) {
    iVar1 = 0x20;
  }
  else if (iVar1 == DAT_8ced7cb4) {
    iVar1 = 0x40;
  }
  else if (iVar1 == DAT_8ced7cb6) {
    iVar1 = 0x10;
  }
  else if (iVar1 == DAT_8ced7cb8) {
    iVar1 = (int)DAT_8ced7cba;
  }
  else if (iVar1 == DAT_8ced7cbc) {
    iVar1 = 8;
  }
  else {
    iVar1 = 0;
  }
  return iVar1;
}


/* Address: 8ced7c62 */

char * callback_candidate_8ced7c62(char *param_1,int param_2,uint param_3)

{
  char cVar1;
  uint uVar2;
  int iVar3;
  char *pcVar4;
  
  pcVar4 = param_1 + -param_2;
  iVar3 = 0;
joined_r0x8ced7c84:
  do {
    while( true ) {
      if (param_1 <= pcVar4) {
        return (char *)0x0;
      }
      param_1 = param_1 + -1;
      cVar1 = *param_1;
      if (iVar3 != 0) break;
LAB_8ced7cf0:
      iVar3 = 1;
    }
    if (iVar3 != 1) {
      if (iVar3 == 2) {
        if (cVar1 == '\0') {
          iVar3 = 3;
        }
        else if (cVar1 != '\x01') {
          iVar3 = 1;
        }
      }
      else if (iVar3 == 3) {
        if (cVar1 == '\0') {
          uVar2 = FUN_8ced7be8(param_1);
          if ((uVar2 & param_3) != 0) {
            return param_1;
          }
          iVar3 = 0;
        }
        else {
          if (cVar1 != '\x01') goto LAB_8ced7cf0;
          iVar3 = 2;
        }
      }
      goto joined_r0x8ced7c84;
    }
    if (cVar1 == '\x01') {
      iVar3 = 2;
    }
  } while( true );
}


/* Address: 8ced7d0e */

char * callback_candidate_8ced7d0e(char *param_1,int param_2,uint param_3)

{
  char cVar1;
  char *pcVar2;
  uint uVar3;
  int iVar4;
  char *pcVar5;
  
  iVar4 = 0;
  pcVar5 = param_1;
  while( true ) {
    do {
      while( true ) {
        while( true ) {
          while( true ) {
            pcVar2 = pcVar5;
            if (param_1 + param_2 <= pcVar2) {
              return (char *)0x0;
            }
            cVar1 = *pcVar2;
            pcVar5 = pcVar2 + 1;
            if (iVar4 != 0) break;
            if (cVar1 == '\0') {
              iVar4 = 1;
            }
          }
          if (iVar4 != 1) break;
          if (cVar1 == '\0') {
            iVar4 = 2;
          }
          else {
            iVar4 = 0;
          }
        }
        if (iVar4 != 2) break;
        if (cVar1 == '\x01') {
          iVar4 = 3;
        }
        else if (cVar1 != '\0') {
          iVar4 = 0;
        }
      }
    } while (iVar4 != 3);
    uVar3 = FUN_8ced7be8(pcVar2 + -3);
    if ((uVar3 & param_3) != 0) break;
    iVar4 = 0;
  }
  return pcVar2 + -3;
}


/* Address: 8ced7da8 */

char * callback_candidate_8ced7da8(char *param_1,char *param_2,int param_3,uint param_4)

{
  char cVar1;
  char *pcVar2;
  uint uVar3;
  char *pcVar4;
  int iVar5;
  char *pcVar6;
  
  iVar5 = 0;
  pcVar6 = param_2;
  while( true ) {
    do {
      while( true ) {
        while( true ) {
          while( true ) {
            pcVar2 = pcVar6;
            if (param_2 + param_3 <= pcVar2) {
              return pcVar2;
            }
            cVar1 = *pcVar2;
            pcVar6 = pcVar2 + 1;
            pcVar4 = param_1 + 1;
            *param_1 = cVar1;
            param_1 = pcVar4;
            if (iVar5 != 0) break;
            if (cVar1 == '\0') {
              iVar5 = 1;
            }
          }
          if (iVar5 != 1) break;
          if (cVar1 == '\0') {
            iVar5 = 2;
          }
          else {
            iVar5 = 0;
          }
        }
        if (iVar5 != 2) break;
        if (cVar1 == '\x01') {
          iVar5 = 3;
        }
        else if (cVar1 != '\0') {
          iVar5 = 0;
        }
      }
    } while (iVar5 != 3);
    uVar3 = FUN_8ced7be8(pcVar2 + -3);
    if ((uVar3 & param_4) != 0) break;
    iVar5 = 0;
  }
  return pcVar6;
}


/* Address: 8ced7e44 */

void FUN_8ced7e44(void)

{
  undefined4 *puVar1;
  
  puVar1 = DAT_8ced7f1c;
  *DAT_8ced7f1c = 0;
  puVar1[1] = 0;
  puVar1[2] = 0;
  puVar1[3] = 0;
  puVar1[4] = 0;
  return;
}


/* Address: 8ced7e46 */

void FUN_8ced7e46(undefined4 *param_1)

{
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  param_1[3] = 0;
  param_1[4] = 0;
  return;
}


/* Address: 8ced7e54 */

undefined4 callback_candidate_8ced7e54(int param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = (*(code *)PTR_FUN_8ced7f20)();
  if (iVar1 != 0) {
    uVar2 = callback_candidate_8ced7ecc(0,DAT_8ced7f24);
    return uVar2;
  }
  iVar1 = (int)DAT_8ced7f1a;
  *(undefined4 *)(iVar1 + param_1) = param_2;
  ((undefined4 *)(iVar1 + param_1))[1] = param_3;
  return 0;
}


/* Address: 8ced7ecc */

undefined4 callback_candidate_8ced7ecc(int param_1,undefined4 param_2)

{
  if (param_1 == 0) {
    FUN_8ced7efa(DAT_8ced7f1c,param_2);
  }
  else {
    FUN_8ced7efa(param_1 + DAT_8ced7f1a,param_2);
  }
  return param_2;
}


/* Address: 8ced7efa */

void FUN_8ced7efa(undefined4 *param_1,int param_2)

{
  param_1[2] = param_2;
  if ((param_2 != 0) && ((code *)*param_1 != (code *)0x0)) {
    (*(code *)*param_1)(param_1[1]);
  }
  return;
}


/* Address: 8ced7f2c */

undefined4
callback_candidate_8ced7f2c(int param_1,undefined4 *param_2,undefined4 param_3,undefined4 *param_4)

{
  undefined *puVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  int iVar6;
  undefined4 *puVar7;
  undefined4 *puVar8;
  int iVar9;
  undefined1 auStack_64 [88];
  
  iVar2 = (*(code *)PTR_FUN_8ced8080)();
  if (iVar2 == 0) {
    (*(code *)PTR_FUN_8ced808c)(auStack_64);
    uVar4 = *param_2;
    iVar9 = DAT_8ced8078 + param_1;
    uVar3 = param_2[1];
    iVar6 = *(int *)(iVar9 + 0xc);
    iVar2 = *(int *)(iVar9 + 0x10);
    uVar5 = param_4[1];
    puVar7 = (undefined4 *)(DAT_8ced807a + param_1);
    *puVar7 = *param_4;
    puVar7[1] = uVar5;
    uVar5 = param_4[3];
    puVar7[2] = param_4[2];
    puVar7[3] = uVar5;
    uVar5 = param_4[5];
    puVar7[4] = param_4[4];
    puVar7[5] = uVar5;
    uVar5 = param_4[7];
    puVar7[6] = param_4[6];
    puVar7[7] = uVar5;
    uVar5 = param_4[9];
    puVar7[8] = param_4[8];
    puVar7[9] = uVar5;
    uVar5 = param_4[0xb];
    puVar7[10] = param_4[10];
    puVar7[0xb] = uVar5;
    (*(code *)PTR_FUN_8ced8090)(param_1);
    (*(code *)PTR_FUN_8ced8094)(param_1);
    uVar3 = (*(code *)PTR_callback_candidate_8ced93e2_8ced8098)(param_1,uVar4,uVar3,param_3);
    puVar7 = (undefined4 *)(DAT_8ced807c + param_1);
    puVar8 = (undefined4 *)param_4[9];
    uVar4 = puVar7[1];
    *puVar8 = *puVar7;
    puVar8[1] = uVar4;
    uVar4 = puVar7[3];
    puVar8[2] = puVar7[2];
    puVar8[3] = uVar4;
    uVar4 = puVar7[5];
    puVar8[4] = puVar7[4];
    puVar8[5] = uVar4;
    uVar4 = puVar7[7];
    puVar8[6] = puVar7[6];
    puVar8[7] = uVar4;
    uVar4 = puVar7[9];
    puVar8[8] = puVar7[8];
    puVar8[9] = uVar4;
    uVar4 = puVar7[0xb];
    puVar8[10] = puVar7[10];
    puVar8[0xb] = uVar4;
    param_4[10] = *(int *)(iVar9 + 0xc) - iVar6;
    puVar1 = PTR_FUN_8ced809c;
    param_4[0xb] = *(int *)(iVar9 + 0x10) - iVar2;
    (*(code *)puVar1)(auStack_64);
  }
  else {
    uVar3 = (*(code *)PTR_callback_candidate_8ced7ecc_8ced8088)(0,DAT_8ced8084);
  }
  return uVar3;
}


/* Address: 8ced801c */

void callback_candidate_8ced801c(undefined4 param_1,int *param_2,int *param_3)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  
  iVar1 = (*(code *)PTR_FUN_8ced8080)();
  if (iVar1 == 0) {
    iVar4 = param_2[1];
    iVar3 = *param_2;
    iVar1 = (*(code *)PTR_callback_candidate_8ced7d0e_8ced80a4)(iVar3,iVar4,(int)DAT_8ced807e);
    uVar2 = DAT_8ced80a8;
    if (iVar1 == 0) {
      *param_3 = iVar4;
    }
    else {
      uVar2 = 0;
      *param_3 = iVar1 - iVar3;
    }
  }
  else {
    param_1 = 0;
    uVar2 = DAT_8ced80a0;
  }
  (*(code *)PTR_callback_candidate_8ced7ecc_8ced8088)(param_1,uVar2);
  return;
}


/* Address: 8ced8074 */

void FUN_8ced8074(void)

{
  return;
}


/* Address: 8ced80ac */

int callback_candidate_8ced80ac(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = FUN_8ced811a();
  if (iVar1 == 0) {
    FUN_8ced81cc();
    uVar2 = FUN_8ced8206(param_2);
    FUN_8ced8348(param_1,uVar2);
    iVar1 = *DAT_8ced8188;
    (*(code *)PTR_FUN_8ced818c)();
    (*(code *)PTR_FUN_8ced8190)();
    (*(code *)PTR_callback_candidate_8cedab14_8ced8194)(DAT_8ced817c + iVar1);
    (*(code *)PTR_callback_candidate_8ceda920_8ced8198)(iVar1);
    (*(code *)PTR_callback_candidate_8ceda9a4_8ced819c)(DAT_8ced817e + iVar1);
    FUN_8ced8216();
    (*(code *)PTR_callback_candidate_8ceda7c4_8ced81a0)(iVar1 + 0x60);
    return 0;
  }
  if (iVar1 == DAT_8ced8184) {
    return iVar1;
  }
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}


/* Address: 8ced811a */

undefined4 FUN_8ced811a(void)

{
  undefined *puVar1;
  int iVar2;
  undefined4 uVar3;
  
  puVar1 = PTR_callback_candidate_8ced7ecc_8ced81a4;
  uVar3 = DAT_8ced81a8;
  if (((((DAT_8ced8180 & 0x1f) == 0) &&
       (uVar3 = DAT_8ced81b4, *(int *)PTR_s_ZZZZx_8ced81ac == DAT_8ced81b0)) &&
      (iVar2 = (*(code *)PTR_FUN_8ced81b8)(), uVar3 = DAT_8ced81bc, iVar2 == 0)) &&
     ((iVar2 = (*(code *)PTR_callback_candidate_8cedc35c_8ced81c0)(), uVar3 = DAT_8ced81c4,
      iVar2 == 0 &&
      (iVar2 = (*(code *)PTR_callback_candidate_8cedc394_8ced81c8)(), uVar3 = DAT_8ced8184,
      iVar2 == 0)))) {
    return 0;
  }
  uVar3 = (*(code *)puVar1)(0,uVar3);
  return uVar3;
}


/* Address: 8ced81cc */

void FUN_8ced81cc(void)

{
  int iVar1;
  uint *puVar2;
  uint uVar3;
  
  puVar2 = DAT_8ced830c;
  iVar1 = DAT_8ced8308;
  *(undefined4 *)(DAT_8ced8308 + 0x60) = 0;
  *(undefined4 *)(iVar1 + 100) = 0;
  uVar3 = *puVar2;
  if ((DAT_8ced8310 & uVar3) != 0) {
    *(undefined4 *)(iVar1 + 0x60) = 1;
  }
  if ((uVar3 & (int)DAT_8ced82f0) != 0) {
    *(undefined4 *)(iVar1 + 100) = 1;
  }
  return;
}


/* Address: 8ced8206 */

uint FUN_8ced8206(uint param_1)

{
  if (*DAT_8ced831c != 0) {
    param_1 = param_1 | DAT_8ced8318;
  }
  return param_1;
}


/* Address: 8ced8216 */

void FUN_8ced8216(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar3 = *(int *)(DAT_8ced8308 + 0x70);
  iVar4 = 0;
  iVar2 = *(int *)(DAT_8ced8308 + 0x74);
  if (0 < iVar3) {
    do {
      iVar1 = (int)DAT_8ced82f2;
      FUN_8ced8246(iVar2);
      iVar4 = iVar4 + 1;
      iVar2 = iVar2 + iVar1;
    } while (iVar4 < iVar3);
  }
  return;
}


/* Address: 8ced8246 */

void FUN_8ced8246(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined4 *puVar5;
  int iVar6;
  
  puVar5 = DAT_8ced8324;
  *(undefined4 *)(DAT_8ced82f4 + param_1) = 1;
  iVar4 = *DAT_8ced8320;
  *(undefined4 *)(param_1 + 0x10) = *puVar5;
  *(int *)(param_1 + 0x14) = *DAT_8ced8328 + -0x10;
  *(int *)(param_1 + 0x18) = *DAT_8ced832c + -0x20;
  *(int *)(param_1 + 0x1c) = *DAT_8ced8330 + -0x20;
  *(undefined4 *)(param_1 + 0x20) = *DAT_8ced8334;
  *(undefined4 *)(param_1 + 0x24) = *DAT_8ced8338;
  *(undefined4 *)(param_1 + 0x28) = *DAT_8ced833c;
  *(int *)(param_1 + 0x2c) = iVar4 + 0x20;
  *(int *)(param_1 + 0x30) = iVar4;
  *(int *)(param_1 + 0x34) = iVar4 + 0x60;
  uVar1 = *DAT_8ced8340;
  puVar5 = (undefined4 *)(DAT_8ced82f6 + param_1);
  iVar6 = (int)DAT_8ced82fc;
  *(undefined4 *)(param_1 + 0x38) = uVar1;
  *puVar5 = uVar1;
  iVar2 = (int)DAT_8ced82fa;
  iVar4 = (int)DAT_8ced8300;
  puVar5[1] = DAT_8ced82f8 + param_1;
  puVar5[2] = iVar2 + param_1;
  iVar3 = (int)DAT_8ced82fe;
  puVar5[3] = iVar2 + param_1 + iVar6;
  iVar2 = (int)DAT_8ced82f8;
  *(int *)(iVar4 + param_1) = iVar3 + param_1;
  iVar3 = (int)DAT_8ced8302;
  *(int *)(iVar4 + 4 + param_1) = iVar6 + iVar2 + param_1;
  iVar2 = (int)DAT_8ced8304;
  *(int *)(iVar4 + 8 + param_1) = iVar3 + param_1;
  iVar3 = (int)DAT_8ced82f8;
  *(int *)(iVar4 + 0xc + param_1) = iVar2 + param_1;
  iVar2 = (int)DAT_8ced8306;
  *(int *)(iVar4 + 0x10 + param_1) = iVar3 + param_1;
  *(int *)(iVar4 + 0x14 + param_1) = iVar2 + param_1;
  return;
}


/* Address: 8ced8348 */

/* WARNING: Removing unreachable block (ram,0x8ced837c) */
/* WARNING: Removing unreachable block (ram,0x8ced8384) */

void FUN_8ced8348(int param_1,int param_2)

{
  undefined1 *puVar1;
  undefined *puVar2;
  undefined *puVar3;
  int iVar4;
  undefined1 *puVar5;
  uint uVar6;
  int iVar7;
  
  (*(code *)PTR_FUN_8ced8504)(param_2,0,(uint)(param_1 * 0x2000 + (int)DAT_8ced84fc) >> 2);
  puVar2 = PTR_s_MPV_Version_1_12_1999_03_08_8ced850c;
  puVar1 = DAT_8ced8508;
  iVar4 = (int)DAT_8ced8500;
  uVar6 = param_2 + DAT_8ced84fe & 0xffffe000;
  iVar7 = param_1 * 0x2000 + uVar6;
  puVar5 = PTR_s_MPV_Version_1_12_1999_03_08_8ced850c + 1;
  *DAT_8ced8508 = *PTR_s_MPV_Version_1_12_1999_03_08_8ced850c;
  puVar1[1] = *puVar5;
  puVar1[2] = puVar2[2];
  puVar1[3] = puVar2[3];
  puVar1[4] = puVar2[4];
  puVar1[5] = puVar2[5];
  puVar1[6] = puVar2[6];
  puVar1[7] = puVar2[7];
  puVar1[8] = puVar2[8];
  puVar1[9] = puVar2[9];
  puVar1[10] = puVar2[10];
  puVar1[0xb] = puVar2[0xb];
  puVar1[0xc] = puVar2[0xc];
  puVar1[0xd] = puVar2[0xd];
  puVar1[0xe] = puVar2[0xe];
  puVar1[0xf] = puVar2[0xf];
  puVar1[0x10] = puVar2[0x10];
  puVar1[0x11] = puVar2[0x11];
  puVar1[0x12] = puVar2[0x12];
  puVar1[0x13] = puVar2[0x13];
  puVar1[0x14] = puVar2[0x14];
  puVar1[0x15] = puVar2[0x15];
  puVar1[0x16] = puVar2[0x16];
  puVar1[0x17] = puVar2[0x17];
  puVar1[0x18] = puVar2[0x18];
  puVar1[0x19] = puVar2[0x19];
  puVar1[0x1a] = puVar2[0x1a];
  puVar1[0x1b] = puVar2[0x1b];
  puVar2 = PTR_s_Copyright__c__1997_1999_CRI_8ced8510;
  puVar5 = PTR_s_Copyright__c__1997_1999_CRI_8ced8510 + 1;
  puVar1[0x20] = *PTR_s_Copyright__c__1997_1999_CRI_8ced8510;
  puVar1[0x21] = *puVar5;
  puVar1[0x22] = puVar2[2];
  puVar1[0x23] = puVar2[3];
  puVar1[0x24] = puVar2[4];
  puVar1[0x25] = puVar2[5];
  puVar1[0x26] = puVar2[6];
  puVar1[0x27] = puVar2[7];
  puVar1[0x28] = puVar2[8];
  puVar1[0x29] = puVar2[9];
  puVar1[0x2a] = puVar2[10];
  puVar1[0x2b] = puVar2[0xb];
  puVar1[0x2c] = puVar2[0xc];
  puVar1[0x2d] = puVar2[0xd];
  puVar1[0x2e] = puVar2[0xe];
  puVar1[0x2f] = puVar2[0xf];
  puVar1[0x30] = puVar2[0x10];
  puVar1[0x31] = puVar2[0x11];
  puVar1[0x32] = puVar2[0x12];
  puVar1[0x33] = puVar2[0x13];
  puVar3 = PTR_DAT_8ced8514;
  puVar1[0x34] = puVar2[0x14];
  puVar1[0x35] = puVar2[0x15];
  puVar1[0x36] = puVar2[0x16];
  puVar1[0x37] = puVar2[0x17];
  puVar1[0x38] = puVar2[0x18];
  puVar1[0x39] = puVar2[0x19];
  puVar1[0x3a] = puVar2[0x1a];
  puVar1[0x3b] = puVar2[0x1b];
  (*(code *)PTR_FUN_8ced8518)(puVar1 + 0x40,puVar3,8);
  *(int *)(puVar1 + 0x68) = iVar7;
  *(int *)(puVar1 + 0x6c) = iVar4 + iVar7;
  *(int *)(puVar1 + 0x70) = param_1;
  *(uint *)(puVar1 + 0x74) = uVar6;
  return;
}


/* Address: 8ced8532 */

void FUN_8ced8532(void)

{
  return;
}


/* Address: 8ced8536 */

int callback_candidate_8ced8536(void)

{
  int iVar1;
  undefined *puVar2;
  undefined *puVar3;
  undefined *puVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  
  puVar4 = PTR_FUN_8ced8650;
  puVar3 = PTR_FUN_8ced864c;
  puVar2 = PTR_callback_candidate_8ceda9fc_8ced8648;
  iVar1 = DAT_8ced8644;
  iVar5 = 0;
  iVar7 = *(int *)(DAT_8ced8644 + 0x74);
  if (0 < *(int *)(DAT_8ced8644 + 0x70)) {
    do {
      if (*(int *)(DAT_8ced8636 + iVar7) == 1) {
        iVar5 = (int)DAT_8ced8636;
        iVar6 = (int)DAT_8ced8638;
        *(undefined4 *)(iVar5 + iVar7) = 2;
        *(undefined4 *)(iVar5 + 4 + iVar7) = 0;
        (*(code *)puVar4)(iVar6 + iVar7,iVar1 + 0x40,8);
        (*(code *)puVar3)(DAT_8ced863a + iVar7);
        (*(code *)puVar2)(iVar7);
        FUN_8ced85a4(iVar7);
        return iVar7;
      }
      iVar5 = iVar5 + 1;
      iVar7 = iVar7 + DAT_8ced863c;
    } while (iVar5 < *(int *)(DAT_8ced8644 + 0x70));
  }
  return 0;
}


/* Address: 8ced85a4 */

void FUN_8ced85a4(int param_1)

{
  int iVar1;
  int iVar2;
  
  (*(code *)PTR_FUN_8ced8654)(param_1 + 0x70);
  iVar2 = (int)DAT_8ced8640;
  *(int *)(param_1 + 0xb8) = DAT_8ced863e + param_1;
  iVar1 = (int)DAT_8ced8642;
  *(int *)(param_1 + 0x9c) = iVar2 + param_1;
  *(int *)(param_1 + 0xa0) = iVar1 + param_1;
  return;
}


/* Address: 8ced85d2 */

void FUN_8ced85d2(int param_1,undefined4 *param_2,undefined4 *param_3)

{
  *param_2 = *(undefined4 *)(param_1 + 0x88);
  *param_3 = *(undefined4 *)(param_1 + 0x8c);
  return;
}


/* Address: 8ced85de */

undefined4 callback_candidate_8ced85de(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = FUN_8ced86ae();
  if (iVar1 != 0) {
    uVar2 = (*(code *)PTR_callback_candidate_8ced7ecc_8ced865c)(0,DAT_8ced8658);
    return uVar2;
  }
  *(undefined4 *)(DAT_8ced8636 + param_1) = 1;
  return 0;
}


/* Address: 8ced8604 */

undefined4 callback_candidate_8ced8604(int param_1,int param_2,undefined4 param_3)

{
  int iVar1;
  undefined4 uVar2;
  
  if (param_1 == 0) {
    FUN_8ced867e(param_2,param_3);
    param_1 = DAT_8ced8660;
  }
  else {
    iVar1 = FUN_8ced86ae(param_1);
    if (iVar1 != 0) {
      uVar2 = (*(code *)PTR_callback_candidate_8ced7ecc_8ced865c)(0,DAT_8ced8664);
      return uVar2;
    }
    param_1 = DAT_8ced86c8 + param_1;
  }
  *(undefined4 *)(param_2 * 4 + param_1) = param_3;
  return 0;
}


/* Address: 8ced867e */

void FUN_8ced867e(int param_1,undefined4 param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar1 = *(int *)(DAT_8ced86d0 + 0x70);
  iVar2 = *(int *)(DAT_8ced86d0 + 0x74);
  iVar3 = 0;
  if (0 < iVar1) {
    do {
      iVar3 = iVar3 + 1;
      if (*(int *)(DAT_8ced86ca + iVar2) == 2) {
        *(undefined4 *)(DAT_8ced86c8 + iVar2 + param_1 * 4) = param_2;
      }
      iVar2 = iVar2 + DAT_8ced86cc;
    } while (iVar3 < iVar1);
  }
  return;
}


/* Address: 8ced86ae */

undefined4 FUN_8ced86ae(int param_1)

{
  *DAT_8ced86d4 = param_1;
  if ((param_1 != 0) && (*(int *)(DAT_8ced86ca + param_1) == 2)) {
    return 0;
  }
  return 0xffffffff;
}


/* Address: 8ced86d8 */

int callback_candidate_8ced86d8(undefined4 param_1,undefined4 *param_2,undefined4 *param_3)

{
  undefined *puVar1;
  undefined *puVar2;
  int iVar3;
  int iVar4;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 *local_1c;
  
  puVar1 = PTR_callback_candidate_8ced7ecc_8ced8794;
  local_1c = param_2;
  iVar3 = (*(code *)PTR_FUN_8ced8798)();
  puVar2 = PTR_FUN_8ced87a0;
  if (iVar3 == 0) {
    local_24 = *local_1c;
    local_20 = local_1c[1];
    *param_3 = 0;
LAB_8ced870e:
    do {
      iVar3 = FUN_8ced944e(param_1,0x4c,&local_24,&local_20,param_3);
      if (iVar3 != 0) goto LAB_8ced872a;
      iVar3 = (*(code *)puVar2)(local_24);
      if (iVar3 == 0x40) {
        iVar3 = FUN_8ced87a4(param_1,&local_24,&local_20,param_3);
        if (iVar3 == -1) goto LAB_8ced870e;
        if (iVar3 != 0) {
          return iVar3;
        }
LAB_8ced875e:
        iVar3 = FUN_8ced8ce0(param_1,&local_24,&local_20,param_3);
        if (iVar3 != 0) goto LAB_8ced870e;
      }
      else {
        if (iVar3 == 8) goto LAB_8ced875e;
        if (iVar3 != 4) goto LAB_8ced870e;
      }
      iVar4 = FUN_8ced8e3e(param_1,&local_24,&local_20,param_3);
      iVar3 = 0;
    } while (iVar4 != 0);
  }
  else {
    param_1 = 0;
    iVar3 = DAT_8ced879c;
LAB_8ced872a:
    iVar3 = (*(code *)puVar1)(param_1,iVar3);
  }
  return iVar3;
}


/* Address: 8ced87a4 */

/* WARNING: Removing unreachable block (ram,0x8ced8c0a) */
/* WARNING: Removing unreachable block (ram,0x8ced8a1c) */
/* WARNING: Removing unreachable block (ram,0x8ced89cc) */
/* WARNING: Removing unreachable block (ram,0x8ced89a2) */
/* WARNING: Removing unreachable block (ram,0x8ced893c) */
/* WARNING: Removing unreachable block (ram,0x8ced891e) */
/* WARNING: Removing unreachable block (ram,0x8ced88e2) */
/* WARNING: Removing unreachable block (ram,0x8ced88f4) */
/* WARNING: Removing unreachable block (ram,0x8ced8948) */
/* WARNING: Removing unreachable block (ram,0x8ced8970) */
/* WARNING: Removing unreachable block (ram,0x8ced8994) */
/* WARNING: Removing unreachable block (ram,0x8ced8a30) */
/* WARNING: Removing unreachable block (ram,0x8ced8a5a) */
/* WARNING: Removing unreachable block (ram,0x8ced8c12) */

void FUN_8ced87a4(int param_1,int *param_2,int *param_3,int *param_4)

{
  undefined1 uVar1;
  uint uVar2;
  undefined1 uVar3;
  char *pcVar4;
  char *pcVar5;
  uint *puVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  int iVar10;
  undefined1 *puVar11;
  undefined1 *puVar12;
  undefined1 *puVar13;
  undefined1 *puVar14;
  
  puVar6 = (uint *)(DAT_8ced88ec + param_1);
  puVar11 = (undefined1 *)(*param_2 + 4U & 0xfffffffc);
  uVar9 = ((*param_2 + 4) - (int)puVar11) * 8;
  uVar7 = CONCAT31(CONCAT21(CONCAT11(*puVar11,puVar11[1]),puVar11[2]),puVar11[3]);
  if ((int)uVar9 < 0) {
    uVar7 = uVar7 >> (~uVar9 & 0x1f) + 1;
  }
  else {
    uVar7 = uVar7 << (uVar9 & 0x1f);
  }
  puVar12 = puVar11 + 8;
  uVar8 = CONCAT31(CONCAT21(CONCAT11(puVar11[4],puVar11[5]),puVar11[6]),puVar11[7]);
  if ((int)uVar9 < 0x14) {
    uVar2 = uVar7 << 0xc;
    *puVar6 = uVar7 >> 0x14;
    uVar9 = uVar9 + 0xc;
  }
  else {
    uVar9 = uVar9 - 0x14;
    if (uVar9 == 0) {
      *puVar6 = uVar7 >> 0x14;
    }
    else {
      uVar2 = -(0xc - uVar9);
      if ((int)uVar2 < 0) {
        uVar2 = uVar8 >> (~uVar2 & 0x1f) + 1;
      }
      else {
        uVar2 = uVar8 << (uVar2 & 0x1f);
      }
      *puVar6 = (uVar7 | uVar2) >> 0x14;
      if ((int)uVar9 < 0) {
        uVar8 = uVar8 >> (~uVar9 & 0x1f) + 1;
      }
      else {
        uVar8 = uVar8 << (uVar9 & 0x1f);
      }
    }
    uVar3 = *puVar12;
    puVar12 = puVar11 + 0xc;
    uVar2 = uVar8;
    uVar8 = CONCAT31(CONCAT21(CONCAT11(uVar3,puVar11[9]),puVar11[10]),puVar11[0xb]);
  }
  if ((int)uVar9 < 0x14) {
    uVar7 = uVar2 << 0xc;
    puVar6[1] = uVar2 >> 0x14;
    uVar9 = uVar9 + 0xc;
  }
  else {
    uVar9 = uVar9 - 0x14;
    if (uVar9 == 0) {
      puVar6[1] = uVar2 >> 0x14;
    }
    else {
      uVar7 = -(0xc - uVar9);
      if ((int)uVar7 < 0) {
        uVar7 = uVar8 >> (~uVar7 & 0x1f) + 1;
      }
      else {
        uVar7 = uVar8 << (uVar7 & 0x1f);
      }
      puVar6[1] = (uVar2 | uVar7) >> 0x14;
      if ((int)uVar9 < 0) {
        uVar8 = uVar8 >> (~uVar9 & 0x1f) + 1;
      }
      else {
        uVar8 = uVar8 << (uVar9 & 0x1f);
      }
    }
    uVar3 = *puVar12;
    puVar11 = puVar12 + 1;
    puVar13 = puVar12 + 2;
    puVar14 = puVar12 + 3;
    puVar12 = puVar12 + 4;
    uVar7 = uVar8;
    uVar8 = CONCAT31(CONCAT21(CONCAT11(uVar3,*puVar11),*puVar13),*puVar14);
  }
  if ((int)uVar9 < 0x1c) {
    uVar2 = uVar7 << 4;
    *(uint *)(DAT_8ced8a26 + param_1) = uVar7 >> 0x1c;
    uVar9 = uVar9 + 4;
  }
  else {
    uVar9 = uVar9 - 0x1c;
    if (uVar9 == 0) {
      *(uint *)(DAT_8ced8a26 + param_1) = uVar7 >> 0x1c;
    }
    else {
      uVar2 = -(4 - uVar9);
      if ((int)uVar2 < 0) {
        uVar2 = uVar8 >> (~uVar2 & 0x1f) + 1;
      }
      else {
        uVar2 = uVar8 << (uVar2 & 0x1f);
      }
      *(uint *)(DAT_8ced88ee + param_1) = (uVar7 | uVar2) >> 0x1c;
      if ((int)uVar9 < 0) {
        uVar8 = uVar8 >> (~uVar9 & 0x1f) + 1;
      }
      else {
        uVar8 = uVar8 << (uVar9 & 0x1f);
      }
    }
    uVar3 = *puVar12;
    puVar11 = puVar12 + 1;
    puVar13 = puVar12 + 2;
    puVar14 = puVar12 + 3;
    puVar12 = puVar12 + 4;
    uVar2 = uVar8;
    uVar8 = CONCAT31(CONCAT21(CONCAT11(uVar3,*puVar11),*puVar13),*puVar14);
  }
  if ((int)uVar9 < 0x1c) {
    uVar7 = uVar2 << 4;
    puVar6[4] = uVar2 >> 0x1c;
    uVar9 = uVar9 + 4;
  }
  else {
    uVar9 = uVar9 - 0x1c;
    if (uVar9 == 0) {
      puVar6[4] = uVar2 >> 0x1c;
    }
    else {
      uVar7 = -(4 - uVar9);
      if ((int)uVar7 < 0) {
        uVar7 = uVar8 >> (~uVar7 & 0x1f) + 1;
      }
      else {
        uVar7 = uVar8 << (uVar7 & 0x1f);
      }
      puVar6[4] = (uVar2 | uVar7) >> 0x1c;
      if ((int)uVar9 < 0) {
        uVar8 = uVar8 >> (~uVar9 & 0x1f) + 1;
      }
      else {
        uVar8 = uVar8 << (uVar9 & 0x1f);
      }
    }
    uVar3 = *puVar12;
    puVar11 = puVar12 + 1;
    puVar13 = puVar12 + 2;
    puVar14 = puVar12 + 3;
    puVar12 = puVar12 + 4;
    uVar7 = uVar8;
    uVar8 = CONCAT31(CONCAT21(CONCAT11(uVar3,*puVar11),*puVar13),*puVar14);
  }
  if ((int)uVar9 < 0xe) {
    uVar2 = uVar7 << 0x12;
    *(uint *)(DAT_8ced8a28 + param_1) = uVar7 >> 0xe;
    uVar9 = uVar9 + 0x12;
  }
  else {
    uVar9 = uVar9 - 0xe;
    if (uVar9 == 0) {
      *(uint *)(DAT_8ced8a28 + param_1) = uVar7 >> 0xe;
    }
    else {
      uVar2 = -(0x12 - uVar9);
      if ((int)uVar2 < 0) {
        uVar2 = uVar8 >> (~uVar2 & 0x1f) + 1;
      }
      else {
        uVar2 = uVar8 << (uVar2 & 0x1f);
      }
      *(uint *)(DAT_8ced8a28 + param_1) = (uVar7 | uVar2) >> 0xe;
      if ((int)uVar9 < 0) {
        uVar8 = uVar8 >> (~uVar9 & 0x1f) + 1;
      }
      else {
        uVar8 = uVar8 << (uVar9 & 0x1f);
      }
    }
    uVar3 = *puVar12;
    puVar11 = puVar12 + 1;
    puVar13 = puVar12 + 2;
    puVar14 = puVar12 + 3;
    puVar12 = puVar12 + 4;
    uVar2 = uVar8;
    uVar8 = CONCAT31(CONCAT21(CONCAT11(uVar3,*puVar11),*puVar13),*puVar14);
  }
  uVar7 = uVar9 + 1;
  if ((int)uVar7 < 0x20) {
    uVar2 = uVar2 << 1;
  }
  else {
    uVar3 = *puVar12;
    puVar11 = puVar12 + 1;
    uVar7 = uVar9 - 0x1f;
    puVar13 = puVar12 + 2;
    if ((int)uVar7 < 0) {
      uVar2 = uVar8 >> (~uVar7 & 0x1f) + 1;
    }
    else {
      uVar2 = uVar8 << (uVar7 & 0x1f);
    }
    puVar14 = puVar12 + 3;
    puVar12 = puVar12 + 4;
    uVar8 = CONCAT31(CONCAT21(CONCAT11(uVar3,*puVar11),*puVar13),*puVar14);
  }
  if ((int)uVar7 < 0x16) {
    uVar9 = uVar2 << 10;
    *(uint *)(DAT_8ced8b3a + param_1) = uVar2 >> 0x16;
    uVar7 = uVar7 + 10;
  }
  else {
    uVar7 = uVar7 - 0x16;
    if (uVar7 == 0) {
      *(uint *)(DAT_8ced8b3a + param_1) = uVar2 >> 0x16;
    }
    else {
      uVar9 = -(10 - uVar7);
      if ((int)uVar9 < 0) {
        uVar9 = uVar8 >> (~uVar9 & 0x1f) + 1;
      }
      else {
        uVar9 = uVar8 << (uVar9 & 0x1f);
      }
      *(uint *)(DAT_8ced8a2a + param_1) = (uVar2 | uVar9) >> 0x16;
      if ((int)uVar7 < 0) {
        uVar8 = uVar8 >> (~uVar7 & 0x1f) + 1;
      }
      else {
        uVar8 = uVar8 << (uVar7 & 0x1f);
      }
    }
    uVar3 = *puVar12;
    puVar11 = puVar12 + 1;
    puVar13 = puVar12 + 2;
    puVar14 = puVar12 + 3;
    puVar12 = puVar12 + 4;
    uVar9 = uVar8;
    uVar8 = CONCAT31(CONCAT21(CONCAT11(uVar3,*puVar11),*puVar13),*puVar14);
  }
  *(uint *)(DAT_8ced8b3c + param_1) = -1 < (int)uVar9 ^ 1;
  if (uVar7 == 0x1f) {
    uVar3 = *puVar12;
    puVar11 = puVar12 + 1;
    iVar10 = 0;
    puVar13 = puVar12 + 2;
    puVar14 = puVar12 + 3;
    puVar12 = puVar12 + 4;
    uVar9 = uVar8;
    uVar8 = CONCAT31(CONCAT21(CONCAT11(uVar3,*puVar11),*puVar13),*puVar14);
  }
  else {
    uVar9 = uVar9 << 1;
    iVar10 = uVar7 + 1;
  }
  if (iVar10 == 0x1f) {
    uVar3 = *puVar12;
    puVar11 = puVar12 + 1;
    uVar2 = 0;
    puVar13 = puVar12 + 2;
    puVar14 = puVar12 + 3;
    puVar12 = puVar12 + 4;
    uVar7 = uVar8;
    uVar8 = CONCAT31(CONCAT21(CONCAT11(uVar3,*puVar11),*puVar13),*puVar14);
  }
  else {
    uVar7 = uVar9 << 1;
    uVar2 = iVar10 + 1;
  }
  if ((int)uVar9 < 0) {
    pcVar5 = DAT_8ced8b40 + 0x40;
    for (pcVar4 = DAT_8ced8b40; pcVar4 < pcVar5; pcVar4 = pcVar4 + 1) {
      if ((int)uVar2 < 0x18) {
        uVar9 = uVar7 << 8;
        uVar2 = uVar2 + 8;
      }
      else {
        uVar2 = uVar2 - 0x18;
        if (uVar2 != 0) {
          uVar9 = -(8 - uVar2);
          if ((int)uVar9 < 0) {
            uVar9 = uVar8 >> (~uVar9 & 0x1f) + 1;
          }
          else {
            uVar9 = uVar8 << (uVar9 & 0x1f);
          }
          uVar7 = uVar7 | uVar9;
          if ((int)uVar2 < 0) {
            uVar8 = uVar8 >> (~uVar2 & 0x1f) + 1;
          }
          else {
            uVar8 = uVar8 << (uVar2 & 0x1f);
          }
        }
        uVar3 = *puVar12;
        puVar11 = puVar12 + 1;
        puVar13 = puVar12 + 2;
        puVar14 = puVar12 + 3;
        puVar12 = puVar12 + 4;
        uVar9 = uVar8;
        uVar8 = CONCAT31(CONCAT21(CONCAT11(uVar3,*puVar11),*puVar13),*puVar14);
      }
      *(char *)((int)*pcVar4 + DAT_8ced8b3e + param_1) = (char)(uVar7 >> 0x18);
      uVar7 = uVar9;
    }
  }
  else {
    (*(code *)PTR_FUN_8ced8c8c)(param_1 + DAT_8ced8c80,DAT_8ced8c88,0x10,param_1);
  }
  if (uVar2 == 0x1f) {
    uVar3 = *puVar12;
    puVar11 = puVar12 + 1;
    uVar2 = 0;
    puVar13 = puVar12 + 2;
    puVar14 = puVar12 + 3;
    puVar12 = puVar12 + 4;
    uVar9 = uVar8;
    uVar8 = CONCAT31(CONCAT21(CONCAT11(uVar3,*puVar11),*puVar13),*puVar14);
  }
  else {
    uVar9 = uVar7 << 1;
    uVar2 = uVar2 + 1;
  }
  if ((int)uVar7 < 0) {
    pcVar5 = DAT_8ced8c90 + 0x40;
    for (pcVar4 = DAT_8ced8c90; pcVar4 < pcVar5; pcVar4 = pcVar4 + 1) {
      uVar3 = (undefined1)(uVar9 >> 0x18);
      if ((int)uVar2 < 0x18) {
        uVar9 = uVar9 << 8;
        uVar2 = uVar2 + 8;
      }
      else {
        uVar2 = uVar2 - 0x18;
        if (uVar2 != 0) {
          uVar7 = -(8 - uVar2);
          if ((int)uVar7 < 0) {
            uVar7 = uVar8 >> (~uVar7 & 0x1f) + 1;
          }
          else {
            uVar7 = uVar8 << (uVar7 & 0x1f);
          }
          uVar9 = uVar9 | uVar7;
          if ((int)uVar2 < 0) {
            uVar8 = uVar8 >> (~uVar2 & 0x1f) + 1;
          }
          else {
            uVar8 = uVar8 << (uVar2 & 0x1f);
          }
        }
        uVar3 = (undefined1)(uVar9 >> 0x18);
        uVar1 = *puVar12;
        puVar11 = puVar12 + 1;
        puVar13 = puVar12 + 2;
        puVar14 = puVar12 + 3;
        puVar12 = puVar12 + 4;
        uVar9 = uVar8;
        uVar8 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar11),*puVar13),*puVar14);
      }
      *(undefined1 *)((int)*pcVar4 + DAT_8ced8c82 + param_1) = uVar3;
    }
  }
  else {
    (*(code *)PTR_FUN_8ced8c98)(param_1 + DAT_8ced8c82,DAT_8ced8c94,0x10,param_1);
  }
  iVar10 = (int)DAT_8ced8c84;
  puVar6[2] = (int)(*puVar6 + 0xf) >> 4;
  uVar7 = (int)(puVar6[1] + 0xf) >> 4;
  puVar6[3] = uVar7;
  *(uint *)(iVar10 + param_1) = uVar7 * puVar6[2] + -1;
  FUN_8ced8cae(param_1);
  iVar10 = (int)(puVar12 + ((int)(uVar2 + 7) >> 3) + -8) - *param_2;
  *param_4 = *param_4 + iVar10;
  *param_3 = *param_3 - iVar10;
  *param_2 = (int)(puVar12 + ((int)(uVar2 + 7) >> 3) + -8);
  FUN_8ced9264(param_1,param_2,param_3,param_4,8);
  return;
}


/* Address: 8ced8cae */

void FUN_8ced8cae(int param_1)

{
  char *pcVar1;
  char *pcVar2;
  int iVar3;
  char cVar4;
  int iVar5;
  
  cVar4 = '\0';
  iVar5 = *(int *)(DAT_8ced8dd6 + param_1);
  pcVar2 = (char *)(param_1 + DAT_8ced8dd8);
  pcVar1 = pcVar2 + DAT_8ced8dda;
  iVar3 = 0;
  for (; pcVar2 < pcVar1; pcVar2 = pcVar2 + 2) {
    if (iVar5 <= iVar3) {
      iVar3 = 0;
      cVar4 = cVar4 + '\x01';
    }
    *pcVar2 = cVar4;
    pcVar2[1] = (char)iVar3;
    iVar3 = iVar3 + 1;
  }
  return;
}


/* Address: 8ced8ce0 */

/* WARNING: Removing unreachable block (ram,0x8ced8d5e) */
/* WARNING: Removing unreachable block (ram,0x8ced8d40) */
/* WARNING: Removing unreachable block (ram,0x8ced8da2) */

void FUN_8ced8ce0(int param_1,int *param_2,int *param_3,int *param_4)

{
  undefined1 uVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  undefined1 *puVar9;
  undefined1 *puVar10;
  
  puVar9 = (undefined1 *)(*param_2 + 4U & 0xfffffffc);
  uVar5 = ((*param_2 + 4) - (int)puVar9) * 8;
  uVar7 = CONCAT31(CONCAT21(CONCAT11(*puVar9,puVar9[1]),puVar9[2]),puVar9[3]);
  if ((int)uVar5 < 0) {
    uVar7 = uVar7 >> (~uVar5 & 0x1f) + 1;
  }
  else {
    uVar7 = uVar7 << (uVar5 & 0x1f);
  }
  puVar10 = puVar9 + 8;
  uVar6 = CONCAT31(CONCAT21(CONCAT11(puVar9[4],puVar9[5]),puVar9[6]),puVar9[7]);
  uVar4 = uVar7 >> 7;
  iVar3 = DAT_8ced8ddc + param_1;
  if ((int)uVar5 < 7) {
    uVar8 = uVar7 << 0x19;
    uVar5 = uVar5 + 0x19;
  }
  else {
    uVar5 = uVar5 - 7;
    uVar8 = uVar6;
    if (uVar5 != 0) {
      uVar4 = -(0x19 - uVar5);
      if ((int)uVar4 < 0) {
        uVar4 = uVar6 >> (~uVar4 & 0x1f) + 1;
      }
      else {
        uVar4 = uVar6 << (uVar4 & 0x1f);
      }
      uVar4 = (uVar7 | uVar4) >> 7;
      if ((int)uVar5 < 0) {
        uVar8 = uVar6 >> (~uVar5 & 0x1f) + 1;
      }
      else {
        uVar8 = uVar6 << (uVar5 & 0x1f);
      }
    }
    uVar1 = *puVar10;
    puVar10 = puVar9 + 0xc;
    uVar6 = CONCAT31(CONCAT21(CONCAT11(uVar1,puVar9[9]),puVar9[10]),puVar9[0xb]);
  }
  iVar2 = (int)DAT_8ced8dde;
  *(uint *)(iVar3 + 0x2c) = uVar4 & 0x3f;
  *(uint *)(iVar3 + 0x28) = uVar4 >> 6 & 0x3f;
  *(uint *)(iVar3 + 0x24) = uVar4 >> 0xd & 0x3f;
  *(uint *)(iVar3 + 0x20) = uVar4 >> 0x13 & 0x1f;
  *(uint *)(iVar3 + 0x1c) = uVar4 >> 0x18;
  *(uint *)(iVar2 + param_1) = -1 < (int)uVar8 ^ 1;
  if (uVar5 == 0x1f) {
    iVar3 = 0;
    puVar10 = puVar10 + 4;
  }
  else {
    uVar6 = uVar8 << 1;
    iVar3 = uVar5 + 1;
  }
  *(uint *)(DAT_8ced8f3c + param_1) = -1 < (int)uVar6 ^ 1;
  if (iVar3 == 0x1f) {
    iVar3 = 0;
    puVar10 = puVar10 + 4;
  }
  else {
    iVar3 = iVar3 + 1;
  }
  iVar2 = (int)(puVar10 + (iVar3 + 7 >> 3) + -8) - *param_2;
  *param_4 = *param_4 + iVar2;
  *param_3 = *param_3 - iVar2;
  *param_2 = (int)(puVar10 + (iVar3 + 7 >> 3) + -8);
  FUN_8ced9264(4);
  return;
}


/* Address: 8ced8e3e */

/* WARNING: Removing unreachable block (ram,0x8ced90a2) */
/* WARNING: Removing unreachable block (ram,0x8ced900c) */
/* WARNING: Removing unreachable block (ram,0x8ced8f46) */
/* WARNING: Removing unreachable block (ram,0x8ced8f1a) */
/* WARNING: Removing unreachable block (ram,0x8ced8ec2) */
/* WARNING: Removing unreachable block (ram,0x8ced8eb6) */
/* WARNING: Removing unreachable block (ram,0x8ced8eea) */
/* WARNING: Removing unreachable block (ram,0x8ced8f0e) */
/* WARNING: Removing unreachable block (ram,0x8ced9018) */
/* WARNING: Removing unreachable block (ram,0x8ced9044) */
/* WARNING: Removing unreachable block (ram,0x8ced90bc) */

undefined4 FUN_8ced8e3e(int param_1,int *param_2,int *param_3,int *param_4)

{
  undefined1 uVar1;
  undefined *puVar2;
  undefined *puVar3;
  int iVar4;
  undefined4 uVar5;
  uint uVar6;
  uint *puVar7;
  uint uVar8;
  int iVar9;
  int iVar10;
  uint uVar11;
  uint uVar12;
  uint uVar13;
  undefined1 *puVar14;
  undefined1 *puVar15;
  undefined1 *puVar16;
  undefined1 *puVar17;
  
  puVar14 = (undefined1 *)(*param_2 + 4U & 0xfffffffc);
  iVar10 = DAT_8ced8f3e + param_1;
  uVar11 = CONCAT31(CONCAT21(CONCAT11(*puVar14,puVar14[1]),puVar14[2]),puVar14[3]);
  uVar12 = ((*param_2 + 4) - (int)puVar14) * 8;
  puVar15 = puVar14 + 8;
  uVar13 = CONCAT31(CONCAT21(CONCAT11(puVar14[4],puVar14[5]),puVar14[6]),puVar14[7]);
  if ((int)uVar12 < 0) {
    uVar11 = uVar11 >> (~uVar12 & 0x1f) + 1;
  }
  else {
    uVar11 = uVar11 << (uVar12 & 0x1f);
  }
  if ((int)uVar12 < 0x16) {
    uVar6 = uVar11 << 10;
    *(uint *)(iVar10 + 0x14) = uVar11 >> 0x16;
    uVar12 = uVar12 + 10;
  }
  else {
    uVar12 = uVar12 - 0x16;
    if (uVar12 == 0) {
      *(uint *)(iVar10 + 0x14) = uVar11 >> 0x16;
    }
    else {
      uVar6 = -(10 - uVar12);
      if ((int)uVar6 < 0) {
        uVar6 = uVar13 >> (~uVar6 & 0x1f) + 1;
      }
      else {
        uVar6 = uVar13 << (uVar6 & 0x1f);
      }
      *(uint *)(iVar10 + 0x14) = (uVar11 | uVar6) >> 0x16;
      if ((int)uVar12 < 0) {
        uVar13 = uVar13 >> (~uVar12 & 0x1f) + 1;
      }
      else {
        uVar13 = uVar13 << (uVar12 & 0x1f);
      }
    }
    uVar1 = *puVar15;
    puVar15 = puVar14 + 0xc;
    uVar6 = uVar13;
    uVar13 = CONCAT31(CONCAT21(CONCAT11(uVar1,puVar14[9]),puVar14[10]),puVar14[0xb]);
  }
  if ((int)uVar12 < 0x1d) {
    uVar11 = uVar6 << 3;
    *(uint *)(iVar10 + 0x18) = uVar6 >> 0x1d;
    uVar12 = uVar12 + 3;
  }
  else {
    uVar12 = uVar12 - 0x1d;
    if (uVar12 == 0) {
      *(uint *)(iVar10 + 0x18) = uVar6 >> 0x1d;
    }
    else {
      uVar11 = -(3 - uVar12);
      if ((int)uVar11 < 0) {
        uVar11 = uVar13 >> (~uVar11 & 0x1f) + 1;
      }
      else {
        uVar11 = uVar13 << (uVar11 & 0x1f);
      }
      *(uint *)(iVar10 + 0x18) = (uVar6 | uVar11) >> 0x1d;
      if ((int)uVar12 < 0) {
        uVar13 = uVar13 >> (~uVar12 & 0x1f) + 1;
      }
      else {
        uVar13 = uVar13 << (uVar12 & 0x1f);
      }
    }
    uVar1 = *puVar15;
    puVar14 = puVar15 + 1;
    puVar16 = puVar15 + 2;
    puVar17 = puVar15 + 3;
    puVar15 = puVar15 + 4;
    uVar11 = uVar13;
    uVar13 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar14),*puVar16),*puVar17);
  }
  if ((int)uVar12 < 0x10) {
    uVar6 = uVar11 << 0x10;
    *(uint *)(DAT_8ced9036 + param_1) = uVar11 >> 0x10;
    uVar12 = uVar12 + 0x10;
  }
  else {
    uVar12 = uVar12 - 0x10;
    if (uVar12 == 0) {
      *(uint *)(DAT_8ced9036 + param_1) = uVar11 >> 0x10;
    }
    else {
      uVar6 = -(0x10 - uVar12);
      if ((int)uVar6 < 0) {
        uVar6 = uVar13 >> (~uVar6 & 0x1f) + 1;
      }
      else {
        uVar6 = uVar13 << (uVar6 & 0x1f);
      }
      *(uint *)(DAT_8ced9036 + param_1) = (uVar11 | uVar6) >> 0x10;
      if ((int)uVar12 < 0) {
        uVar13 = uVar13 >> (~uVar12 & 0x1f) + 1;
      }
      else {
        uVar13 = uVar13 << (uVar12 & 0x1f);
      }
    }
    uVar1 = *puVar15;
    puVar14 = puVar15 + 1;
    puVar16 = puVar15 + 2;
    puVar17 = puVar15 + 3;
    puVar15 = puVar15 + 4;
    uVar6 = uVar13;
    uVar13 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar14),*puVar16),*puVar17);
  }
  iVar10 = *(int *)(iVar10 + 0x18);
  if ((iVar10 == 2) || (iVar10 == 3)) {
    puVar7 = (uint *)(DAT_8ced9038 + param_1);
    *puVar7 = -1 < (int)uVar6 ^ 1;
    if (uVar12 == 0x1f) {
      uVar1 = *puVar15;
      puVar14 = puVar15 + 1;
      iVar4 = 0;
      puVar16 = puVar15 + 2;
      puVar17 = puVar15 + 3;
      puVar15 = puVar15 + 4;
      uVar11 = uVar13;
      uVar13 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar14),*puVar16),*puVar17);
    }
    else {
      uVar11 = uVar6 << 1;
      iVar4 = uVar12 + 1;
    }
    if (iVar4 < 0x1d) {
      uVar6 = uVar11 << 3;
      uVar12 = iVar4 + 3;
    }
    else {
      uVar12 = iVar4 - 0x1d;
      if (uVar12 != 0) {
        uVar6 = -(3 - uVar12);
        if ((int)uVar6 < 0) {
          uVar6 = uVar13 >> (~uVar6 & 0x1f) + 1;
        }
        else {
          uVar6 = uVar13 << (uVar6 & 0x1f);
        }
        uVar11 = uVar11 | uVar6;
        if ((int)uVar12 < 0) {
          uVar13 = uVar13 >> (~uVar12 & 0x1f) + 1;
        }
        else {
          uVar13 = uVar13 << (uVar12 & 0x1f);
        }
      }
      uVar1 = *puVar15;
      puVar14 = puVar15 + 1;
      puVar16 = puVar15 + 2;
      puVar17 = puVar15 + 3;
      puVar15 = puVar15 + 4;
      uVar6 = uVar13;
      uVar13 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar14),*puVar16),*puVar17);
    }
    uVar11 = (uVar11 >> 0x1d) - 1;
    puVar7[1] = uVar11;
    puVar7[2] = 0x1b - uVar11;
    if ((int)uVar11 < 0) {
      uVar11 = 1 >> (~uVar11 & 0x1f) + 1;
    }
    else {
      uVar11 = 1 << (uVar11 & 0x1f);
    }
    puVar7[3] = uVar11;
    if (iVar10 == 3) {
      puVar7 = (uint *)(DAT_8ced90e0 + param_1);
      *puVar7 = -1 < (int)uVar6 ^ 1;
      if (uVar12 == 0x1f) {
        uVar1 = *puVar15;
        puVar14 = puVar15 + 1;
        iVar4 = 0;
        puVar16 = puVar15 + 2;
        puVar17 = puVar15 + 3;
        puVar15 = puVar15 + 4;
        uVar6 = uVar13;
        uVar13 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar14),*puVar16),*puVar17);
      }
      else {
        uVar6 = uVar6 << 1;
        iVar4 = uVar12 + 1;
      }
      uVar11 = uVar6 >> 0x1d;
      if (iVar4 < 0x1d) {
        uVar6 = uVar6 << 3;
        uVar12 = iVar4 + 3;
      }
      else {
        uVar12 = iVar4 - 0x1d;
        if (uVar12 != 0) {
          uVar11 = -(3 - uVar12);
          if ((int)uVar11 < 0) {
            uVar11 = uVar13 >> (~uVar11 & 0x1f) + 1;
          }
          else {
            uVar11 = uVar13 << (uVar11 & 0x1f);
          }
          uVar11 = (uVar6 | uVar11) >> 0x1d;
          if ((int)uVar12 < 0) {
            uVar13 = uVar13 >> (~uVar12 & 0x1f) + 1;
          }
          else {
            uVar13 = uVar13 << (uVar12 & 0x1f);
          }
        }
        uVar1 = *puVar15;
        puVar14 = puVar15 + 1;
        puVar16 = puVar15 + 2;
        puVar17 = puVar15 + 3;
        puVar15 = puVar15 + 4;
        uVar6 = uVar13;
        uVar13 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar14),*puVar16),*puVar17);
      }
      uVar11 = uVar11 - 1;
      if ((int)uVar11 < 0) {
        uVar8 = 1 >> (~uVar11 & 0x1f) + 1;
      }
      else {
        uVar8 = 1 << (uVar11 & 0x1f);
      }
      puVar7[1] = uVar11;
      puVar7[2] = 0x1b - uVar11;
      puVar7[3] = uVar8;
    }
  }
  iVar9 = *(int *)(DAT_8ced922e + param_1);
  iVar10 = iVar10 * 4;
  *(undefined4 *)(DAT_8ced9230 + param_1) = *(undefined4 *)(PTR_DAT_8ced9244 + iVar10);
  iVar4 = iVar9 * 0x14;
  iVar9 = iVar9 * 4;
  *(undefined4 *)(DAT_8ced9232 + param_1) = *(undefined4 *)(PTR_DAT_8ced9248 + iVar10 + iVar4);
  *(undefined4 *)(DAT_8ced9234 + param_1) = *(undefined4 *)(PTR_PTR_8ced924c + iVar9);
  *(undefined4 *)(DAT_8ced9236 + param_1) = *(undefined4 *)(PTR_PTR_8ced9250 + iVar9);
  puVar2 = PTR_DAT_8ced9258;
  *(undefined4 *)(DAT_8ced923a + param_1) =
       *(undefined4 *)(PTR_DAT_8ced9254 + iVar10 + iVar4 + *(int *)(DAT_8ced9238 + param_1) * 0x28);
  puVar3 = PTR_DAT_8ced9260;
  *(undefined4 *)(DAT_8ced923c + param_1) = *(undefined4 *)(puVar2 + iVar10 + iVar4);
  *(undefined4 *)(DAT_8ced923e + param_1) = *(undefined4 *)(PTR_DAT_8ced925c + iVar10 + iVar4);
  *(undefined4 *)(DAT_8ced9240 + param_1) = *(undefined4 *)(puVar3 + iVar10 + iVar4);
  do {
    if (-1 < (int)uVar6) {
      iVar10 = uVar12 + 1;
      if (0x1f < iVar10) {
        iVar10 = uVar12 - 0x1f;
        puVar15 = puVar15 + 4;
      }
      iVar4 = (int)(puVar15 + (iVar10 + 7 >> 3) + -8) - *param_2;
      *param_4 = *param_4 + iVar4;
      *param_3 = *param_3 - iVar4;
      *param_2 = (int)(puVar15 + (iVar10 + 7 >> 3) + -8);
      uVar5 = FUN_8ced9264(2);
      return uVar5;
    }
    uVar11 = uVar12 + 9;
    if ((int)uVar11 < 0x20) {
      uVar6 = uVar6 << 9;
    }
    else {
      uVar1 = *puVar15;
      puVar14 = puVar15 + 1;
      uVar11 = uVar12 - 0x17;
      puVar16 = puVar15 + 2;
      if ((int)uVar11 < 0) {
        uVar6 = uVar13 >> (~uVar11 & 0x1f) + 1;
      }
      else {
        uVar6 = uVar13 << (uVar11 & 0x1f);
      }
      puVar17 = puVar15 + 3;
      puVar15 = puVar15 + 4;
      uVar13 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar14),*puVar16),*puVar17);
    }
    uVar12 = uVar11;
  } while ((int)(puVar15 + ((((int)(uVar11 + 7) >> 3) + -8) - *param_2)) < *param_3);
  *param_4 = *param_4 + *param_3;
  *param_2 = *param_2 + *param_3;
  *param_3 = 0;
  return 0xfffffffd;
}


/* Address: 8ced9264 */

undefined4 FUN_8ced9264(int *param_1,int *param_2,int *param_3,uint param_4)

{
  uint uVar1;
  
  uVar1 = callback_candidate_8ced92e2(param_1,param_2,param_3);
  if (uVar1 != 0) {
    if (uVar1 == 0x10) {
      *param_1 = *param_1 + 4;
      *param_2 = *param_2 + -4;
      *param_3 = *param_3 + 4;
      uVar1 = callback_candidate_8ced92e2(param_1,param_2,param_3);
      if (uVar1 == 0) {
        return 0xffffffff;
      }
    }
    if (uVar1 == 0x20) {
      *param_1 = *param_1 + 4;
      *param_2 = *param_2 + -4;
      *param_3 = *param_3 + 4;
      uVar1 = callback_candidate_8ced92e2(param_1,param_2,param_3);
      if (uVar1 == 0) {
        return 0xffffffff;
      }
    }
    if ((uVar1 & param_4) != 0) {
      return 0;
    }
  }
  return 0xffffffff;
}


/* Address: 8ced92e2 */

undefined4 callback_candidate_8ced92e2(int *param_1,int *param_2,int *param_3)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = (*(code *)PTR_callback_candidate_8ced7d0e_8ced939c)(*param_1,*param_2,0xffffffff);
  if (iVar1 == 0) {
    uVar2 = 0;
    iVar1 = *param_2;
  }
  else {
    uVar2 = (*(code *)PTR_FUN_8ced93a0)(iVar1);
    iVar1 = iVar1 - *param_1;
  }
  *param_1 = *param_1 + iVar1;
  *param_2 = *param_2 - iVar1;
  *param_3 = *param_3 + iVar1;
  return uVar2;
}


/* Address: 8ced9338 */

undefined4 callback_candidate_8ced9338(int param_1,undefined4 *param_2)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  
  iVar1 = (*(code *)PTR_FUN_8ced93a4)();
  if (iVar1 != 0) {
    uVar2 = (*(code *)PTR_callback_candidate_8ced7ecc_8ced93ac)(0,DAT_8ced93a8);
    return uVar2;
  }
  puVar3 = (undefined4 *)(DAT_8ced9398 + param_1);
  uVar2 = puVar3[1];
  *param_2 = *puVar3;
  param_2[1] = uVar2;
  uVar2 = puVar3[3];
  param_2[2] = puVar3[2];
  param_2[3] = uVar2;
  uVar2 = puVar3[5];
  param_2[4] = puVar3[4];
  param_2[5] = uVar2;
  uVar2 = puVar3[7];
  param_2[6] = puVar3[6];
  param_2[7] = uVar2;
  uVar2 = puVar3[9];
  param_2[8] = puVar3[8];
  param_2[9] = uVar2;
  uVar2 = puVar3[0xb];
  param_2[10] = puVar3[10];
  param_2[0xb] = uVar2;
  return 0;
}


/* Address: 8ced93b0 */

undefined4 callback_candidate_8ced93b0(int param_1,undefined4 *param_2)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = (*(code *)PTR_FUN_8ced94e8)();
  if (iVar1 != 0) {
    uVar2 = (*(code *)PTR_callback_candidate_8ced7ecc_8ced94f0)(0,DAT_8ced94ec);
    return uVar2;
  }
  *param_2 = *(undefined4 *)(DAT_8ced94e0 + param_1);
  return 0;
}


/* Address: 8ced93e2 */

undefined4
callback_candidate_8ced93e2
          (undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 *param_4)

{
  undefined *puVar1;
  undefined *puVar2;
  int iVar3;
  undefined4 uVar4;
  uint uVar5;
  undefined4 local_20;
  undefined4 local_1c [2];
  
  puVar2 = PTR_FUN_8ced94f4;
  puVar1 = PTR_callback_candidate_8ced7ecc_8ced94f0;
  *param_4 = 0;
  local_20 = param_3;
  local_1c[0] = param_2;
  while( true ) {
    iVar3 = FUN_8ced944e(param_1,0xffffffff,local_1c,&local_20,param_4);
    if (iVar3 != 0) {
      uVar4 = (*(code *)puVar1)(param_1);
      return uVar4;
    }
    uVar5 = (*(code *)puVar2)(local_1c[0]);
    if ((uVar5 & 1) == 0) break;
    FUN_8ced94fc(param_1,local_1c,&local_20,param_4);
  }
  return 0;
}


/* Address: 8ced944e */

undefined4 FUN_8ced944e(int param_1,uint param_2,int *param_3,int *param_4,int *param_5)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  
  uVar1 = (*(code *)PTR_FUN_8ced94f4)(*param_3,1);
  if ((uVar1 & param_2) == 0) {
    iVar2 = (int)DAT_8ced94e4;
    iVar3 = param_1 + DAT_8ced94e2;
    *(int *)(iVar3 + 0xc) = *(int *)(iVar3 + 0xc) + 1;
    if (*(int *)(iVar2 + param_1) == 0) {
      return 0xfffffffe;
    }
    iVar2 = (*(code *)PTR_callback_candidate_8ced7d0e_8ced94f8)(*param_3,*param_4,param_2);
    if (iVar2 == 0) {
      *param_5 = *param_5 + *param_4;
      *param_3 = *param_3 + *param_4;
      *param_4 = 0;
      return 0xfffffffd;
    }
    *param_5 = *param_5 + (iVar2 - *param_3);
    *param_4 = *param_4 - (iVar2 - *param_3);
    *param_3 = iVar2;
    *(int *)(iVar3 + 0x10) = *(int *)(iVar3 + 0x10) + 1;
  }
  return 0;
}


/* Address: 8ced94fc */

/* WARNING: Removing unreachable block (ram,0x8ced95ce) */
/* WARNING: Removing unreachable block (ram,0x8ced95c0) */
/* WARNING: Removing unreachable block (ram,0x8ced95fa) */

undefined4 FUN_8ced94fc(uint *param_1,uint *param_2,int *param_3,int *param_4)

{
  undefined1 uVar1;
  int iVar2;
  undefined4 uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  undefined1 *puVar8;
  undefined1 *puVar9;
  undefined1 *puVar10;
  undefined1 *puVar11;
  
  puVar8 = (undefined1 *)(*param_2 & 0xfffffffc);
  uVar7 = (*param_2 - (int)puVar8) * 8;
  uVar5 = CONCAT31(CONCAT21(CONCAT11(*puVar8,puVar8[1]),puVar8[2]),puVar8[3]);
  if ((int)uVar7 < 0) {
    uVar5 = uVar5 >> (~uVar7 & 0x1f) + 1;
  }
  else {
    uVar5 = uVar5 << (uVar7 & 0x1f);
  }
  uVar6 = CONCAT31(CONCAT21(CONCAT11(puVar8[4],puVar8[5]),puVar8[6]),puVar8[7]);
  if (uVar7 != 0) {
    uVar4 = -((*param_2 - (int)puVar8) * -8 + 0x20);
    if ((int)uVar4 < 0) {
      uVar4 = uVar6 >> (~uVar4 & 0x1f) + 1;
    }
    else {
      uVar4 = uVar6 << (uVar4 & 0x1f);
    }
    uVar5 = uVar5 | uVar4;
    if ((int)uVar7 < 0) {
      uVar6 = uVar6 >> (~uVar7 & 0x1f) + 1;
    }
    else {
      uVar6 = uVar6 << (uVar7 & 0x1f);
    }
  }
  puVar9 = puVar8 + 0xc;
  uVar4 = CONCAT31(CONCAT21(CONCAT11(puVar8[8],puVar8[9]),puVar8[10]),puVar8[0xb]);
  *(uint *)((int)DAT_8ced9654 + (int)param_1) =
       ((uVar5 & 0xff) - 1) * *(int *)((int)DAT_8ced9652 + (int)param_1) + -1;
  if ((int)uVar7 < 0x1b) {
    uVar5 = uVar6 << 5;
    *(uint *)((int)DAT_8ced9656 + (int)param_1) = uVar6 >> 0x1b;
    uVar7 = uVar7 + 5;
  }
  else {
    uVar7 = uVar7 - 0x1b;
    if (uVar7 == 0) {
      *(uint *)((int)DAT_8ced9656 + (int)param_1) = uVar6 >> 0x1b;
    }
    else {
      uVar5 = -(5 - uVar7);
      if ((int)uVar5 < 0) {
        uVar5 = uVar4 >> (~uVar5 & 0x1f) + 1;
      }
      else {
        uVar5 = uVar4 << (uVar5 & 0x1f);
      }
      *(uint *)((int)DAT_8ced9656 + (int)param_1) = (uVar6 | uVar5) >> 0x1b;
      if ((int)uVar7 < 0) {
        uVar4 = uVar4 >> (~uVar7 & 0x1f) + 1;
      }
      else {
        uVar4 = uVar4 << (uVar7 & 0x1f);
      }
    }
    uVar1 = *puVar9;
    puVar9 = puVar8 + 0x10;
    uVar5 = uVar4;
    uVar4 = CONCAT31(CONCAT21(CONCAT11(uVar1,puVar8[0xd]),puVar8[0xe]),puVar8[0xf]);
  }
  (*(code *)PTR_FUN_8ced965c)((int)DAT_8ced9658 + (int)param_1,1);
  (*(code *)PTR_FUN_8ced965c)((int)DAT_8ced965a + (int)param_1);
  (*(code *)PTR_FUN_8ced9660)(param_1);
  do {
    if (-1 < (int)uVar5) {
      uVar6 = uVar7 + 1;
      if ((int)uVar6 < 0x20) {
        uVar5 = uVar5 << 1;
      }
      else {
        uVar1 = *puVar9;
        puVar8 = puVar9 + 1;
        uVar6 = uVar7 - 0x1f;
        puVar10 = puVar9 + 2;
        if ((int)uVar6 < 0) {
          uVar5 = uVar4 >> (~uVar6 & 0x1f) + 1;
        }
        else {
          uVar5 = uVar4 << (uVar6 & 0x1f);
        }
        puVar11 = puVar9 + 3;
        puVar9 = puVar9 + 4;
        uVar4 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar8),*puVar10),*puVar11);
      }
      iVar2 = (int)DAT_8ced9702;
      *param_1 = uVar5;
      param_1[1] = uVar4;
      param_1[2] = uVar6;
      param_1[3] = (uint)puVar9;
                    /* WARNING: Could not recover jumptable at 0x8ced96ea. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      uVar3 = (**(code **)(iVar2 + (int)param_1))(param_1,param_2,param_3,param_4);
      return uVar3;
    }
    uVar6 = uVar7 + 9;
    if ((int)uVar6 < 0x20) {
      uVar5 = uVar5 << 9;
    }
    else {
      uVar1 = *puVar9;
      puVar8 = puVar9 + 1;
      uVar6 = uVar7 - 0x17;
      puVar10 = puVar9 + 2;
      if ((int)uVar6 < 0) {
        uVar5 = uVar4 >> (~uVar6 & 0x1f) + 1;
      }
      else {
        uVar5 = uVar4 << (uVar6 & 0x1f);
      }
      puVar11 = puVar9 + 3;
      puVar9 = puVar9 + 4;
      uVar4 = CONCAT31(CONCAT21(CONCAT11(uVar1,*puVar8),*puVar10),*puVar11);
    }
    uVar7 = uVar6;
  } while ((int)(puVar9 + ((((int)(uVar6 + 7) >> 3) + -8) - *param_2)) < *param_3);
  *param_4 = *param_4 + *param_3;
  *param_2 = *param_2 + *param_3;
  *param_3 = 0;
  return 0xfffffffd;
}


/* Address: 8ced9708 */

uint callback_candidate_8ced9708(int param_1)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  uint local_18;
  int local_14;
  
  (*(code *)PTR_FUN_8ced978c)(&local_18);
  iVar1 = (*(code *)PTR_FUN_8ced9790)(&local_18);
  uVar2 = DAT_8ced979c;
  if (iVar1 == 0) {
    uVar3 = DAT_8ced9798;
    if (param_1 < 0) goto LAB_8ced9740;
  }
  else {
    uVar3 = DAT_8ced9794;
    if (-1 < param_1) goto LAB_8ced9740;
  }
  uVar2 = DAT_8ced9798;
LAB_8ced9740:
  iVar1 = 0;
  do {
    if ((local_18 & DAT_8ced97a0) == uVar3) break;
    local_18 = local_18 << 1;
    if (local_14 < 0) {
      local_18 = local_18 | 1;
    }
    iVar1 = iVar1 + 1;
    local_14 = local_14 << 1;
  } while (iVar1 < 0x20);
  if (iVar1 == 0) {
    param_1 = 0;
  }
  else {
    uVar3 = -(0x20 - iVar1);
    if ((int)uVar3 < 0) {
      param_1 = param_1 >> (~uVar3 & 0x1f) + 1;
    }
    else {
      param_1 = param_1 << (uVar3 & 0x1f);
    }
  }
  if (param_1 != 0) {
    uVar2 = (*DAT_8ced97a4)();
  }
  return uVar2;
}


/* Address: 8ced97a8 */

void FUN_8ced97a8(void)

{
  return;
}


/* Address: 8ced97ac */

void FUN_8ced97ac(void)

{
  return;
}


/* Address: 8ced97b0 */

void callback_candidate_8ced97b0(undefined4 param_1,int param_2)

{
  undefined4 uVar1;
  int iVar2;
  undefined1 auStack_34 [36];
  
  iVar2 = param_2 + 8;
  FUN_8ced984e(auStack_34,iVar2,*(undefined4 *)(param_2 + 4));
  uVar1 = (*DAT_8ced9910)();
  FUN_8ced9864(param_1,auStack_34,iVar2,0,uVar1,0);
  FUN_8ced9864(param_1,auStack_34,iVar2,1,DAT_8ced9914,0);
  FUN_8ced9864(param_1,auStack_34,iVar2,2,(int)DAT_8ced9906,0);
  FUN_8ced9918(param_1,auStack_34,iVar2,3);
  FUN_8ced9978(param_1,auStack_34,iVar2,4);
  FUN_8ced9918(param_1,auStack_34,iVar2,5);
  FUN_8ced9978(param_1,auStack_34,iVar2,6);
  FUN_8ced9864(param_1,auStack_34,iVar2,7,(int)DAT_8ced9906,0);
  return;
}


/* Address: 8ced984e */

void FUN_8ced984e(int param_1,int param_2,int param_3)

{
  int *piVar1;
  uint uVar2;
  
  uVar2 = 0;
  do {
    *(int *)(uVar2 + param_1) = param_3;
    piVar1 = (int *)(uVar2 + param_2);
    uVar2 = uVar2 + 4;
    param_3 = param_3 + *piVar1;
  } while (uVar2 < 0x20);
  return;
}


/* Address: 8ced9864 */

void FUN_8ced9864(int param_1,int param_2,int param_3,int param_4,int param_5,int param_6)

{
  int iVar1;
  undefined4 *puVar2;
  int *piVar3;
  int *piVar4;
  int iVar5;
  bool bVar6;
  
  puVar2 = (undefined4 *)(param_1 + param_4 * DAT_8ced9908);
  piVar3 = (int *)(param_3 + param_4 * 4);
  piVar4 = puVar2 + 4;
  bVar6 = *piVar3 != 0;
  if (!bVar6) {
    param_6 = 0;
    param_5 = 0;
  }
  if (param_6 == 0) {
    iVar5 = 0;
  }
  else {
    iVar5 = (int)DAT_8ced990a;
  }
  iVar1 = (int)DAT_8ced990c;
  *puVar2 = 0;
  puVar2[1] = (uint)bVar6;
  puVar2[2] = 0;
  puVar2[3] = 0;
  *(undefined4 *)(iVar1 + (int)puVar2) = 9;
  *(undefined4 *)(iVar1 + 4 + (int)puVar2) = 9;
  *piVar4 = *(int *)(param_2 + param_4 * 4);
  iVar1 = (*piVar3 - param_5) + iVar5 * -4;
  puVar2[5] = iVar1;
  puVar2[6] = *piVar4 + iVar1;
  puVar2[7] = param_5;
  puVar2[8] = *piVar4;
  puVar2[9] = *piVar4;
  puVar2[10] = 0;
  puVar2[0xb] = 0;
  puVar2[0xc] = 0;
  puVar2[0xd] = 0;
  puVar2[0xe] = 0;
  puVar2[0xf] = 0;
  puVar2[0x10] = 0;
  iVar1 = (int)DAT_8ced990e;
  puVar2[0x11] = puVar2[6] + puVar2[7];
  puVar2[0x12] = iVar5;
  puVar2[0x13] = 0;
  puVar2[0x14] = iVar1;
  return;
}


/* Address: 8ced9918 */

void FUN_8ced9918(int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  int *piVar4;
  
  piVar4 = (int *)(param_3 + param_4 * 4);
  puVar2 = (undefined4 *)(param_1 + param_4 * DAT_8ced9a58);
  iVar1 = *piVar4;
  *puVar2 = 1;
  puVar2[1] = (uint)(iVar1 != 0);
  iVar1 = (int)DAT_8ced9a5a;
  puVar2[2] = 0;
  puVar2[3] = 0;
  *(undefined4 *)(iVar1 + (int)puVar2) = 9;
  *(undefined4 *)(iVar1 + 4 + (int)puVar2) = 9;
  puVar3 = puVar2 + 8;
  iVar1 = (int)DAT_8ced9a5c;
  puVar2[4] = *(undefined4 *)(param_2 + param_4 * 4);
  puVar2[5] = *piVar4;
  puVar2[6] = 0;
  puVar2[7] = 0;
  do {
    *puVar3 = 0;
    puVar3 = puVar3 + 0xc;
  } while (puVar3 < (undefined4 *)((int)puVar2 + iVar1 + 0x20));
  return;
}


/* Address: 8ced9978 */

void FUN_8ced9978(int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  undefined4 *puVar2;
  int *piVar3;
  
  puVar2 = (undefined4 *)(param_1 + param_4 * DAT_8ced9a58);
  piVar3 = (int *)(param_3 + param_4 * 4);
  iVar1 = *piVar3;
  *puVar2 = 2;
  puVar2[1] = (uint)(iVar1 != 0);
  iVar1 = (int)DAT_8ced9a5a;
  puVar2[2] = 0;
  puVar2[3] = 0;
  *(undefined4 *)(iVar1 + (int)puVar2) = 9;
  *(undefined4 *)(iVar1 + 4 + (int)puVar2) = 9;
  puVar2[4] = *(undefined4 *)(param_2 + param_4 * 4);
  puVar2[5] = *piVar3;
  puVar2[6] = 0;
  puVar2[7] = 0;
  puVar2[8] = 0;
  puVar2[9] = 0;
  puVar2[10] = 0;
  puVar2[0xb] = 0;
  puVar2[0xc] = 0;
  puVar2[0xd] = 0;
  puVar2[0xe] = 0;
  puVar2[0xf] = 0;
  return;
}


/* Address: 8ced9a2e */

undefined4 callback_candidate_8ced9a2e(int param_1,int param_2,uint *param_3)

{
  undefined4 uVar1;
  int iVar2;
  uint *puVar3;
  uint local_1c;
  uint local_18;
  uint local_14;
  uint local_10 [2];
  
  iVar2 = param_2 * DAT_8ced9a58 + DAT_8ced9a5e + param_1;
  if (*(int *)(iVar2 + 4) == 0) {
    uVar1 = (*(code *)PTR_FUN_8ced9a68)(DAT_8ced9a64);
    return uVar1;
  }
  puVar3 = (uint *)(iVar2 + 0x10);
  FUN_8ced9cf4(puVar3,&local_18,&local_14,&local_1c,local_10);
  if ((int)local_10[0] < (int)(local_14 + *(int *)(iVar2 + 0x14))) {
    if (local_18 <= local_1c) {
      *param_3 = local_1c;
      param_3[1] = *(int *)(iVar2 + 0x18) - local_1c;
      param_3[2] = *puVar3;
      param_3[3] = local_18 - *puVar3;
      goto LAB_8ced9ad2;
    }
    *param_3 = local_1c;
    param_3[1] = local_18 - local_1c;
  }
  else {
    *param_3 = local_1c;
    param_3[1] = 0;
  }
  param_3[2] = local_1c;
  param_3[3] = 0;
LAB_8ced9ad2:
  param_3[4] = *(uint *)(iVar2 + 0x40);
  param_3[5] = local_10[0];
  param_3[6] = local_14;
  return 0;
}


/* Address: 8ced9aea */

undefined4 callback_candidate_8ced9aea(int param_1,int param_2,int param_3,int param_4)

{
  undefined *puVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined1 auStack_1c [4];
  int local_18;
  
  puVar1 = PTR_FUN_8ced9b88;
  iVar4 = DAT_8ced9b84 + param_1 + param_2 * DAT_8ced9b86;
  uVar6 = 0;
  uVar5 = DAT_8ced9b8c;
  local_18 = param_3;
  if (((*(int *)(iVar4 + 4) == 0) || (uVar5 = DAT_8ced9b90, param_4 != *(int *)(iVar4 + 0x38))) ||
     (uVar5 = DAT_8ced9b94, *(int *)(iVar4 + 0x40) != 0)) {
    uVar6 = (*(code *)PTR_FUN_8ced9b88)(param_1,uVar5);
  }
  else {
    (*(code *)PTR_FUN_8ced9b98)(auStack_1c);
    uVar3 = *(int *)(iVar4 + 0x20) + local_18;
    *(uint *)(iVar4 + 0x20) = uVar3;
    if (*(uint *)(iVar4 + 0x18) <= uVar3) {
      *(int *)(iVar4 + 0x20) = *(int *)(iVar4 + 0x20) - *(int *)(iVar4 + 0x14);
    }
    iVar2 = *(int *)(iVar4 + 0x38) + local_18;
    *(int *)(iVar4 + 0x38) = iVar2;
    if (*(int *)(iVar4 + 0x3c) + *(int *)(iVar4 + 0x14) < iVar2) {
      uVar6 = (*(code *)puVar1)(param_1,DAT_8ced9b9c);
    }
    (*(code *)PTR_FUN_8ced9ba0)(auStack_1c);
  }
  return uVar6;
}


/* Address: 8ced9ba4 */

undefined4 callback_candidate_8ced9ba4(int param_1,int param_2,uint *param_3)

{
  undefined4 uVar1;
  int iVar2;
  uint *puVar3;
  uint local_1c;
  uint local_18;
  uint local_14;
  uint local_10 [2];
  
  iVar2 = param_2 * DAT_8ced9cde + DAT_8ced9cdc + param_1;
  if (*(int *)(iVar2 + 4) == 0) {
    uVar1 = (*(code *)PTR_FUN_8ced9ce4)(DAT_8ced9ce0);
    return uVar1;
  }
  puVar3 = (uint *)(iVar2 + 0x10);
  FUN_8ced9cf4(puVar3,&local_1c,&local_14,&local_18,local_10);
  if ((int)local_14 < (int)local_10[0]) {
    if (local_18 <= local_1c) {
      *param_3 = local_1c;
      param_3[1] = *(int *)(iVar2 + 0x18) - local_1c;
      param_3[2] = *puVar3;
      param_3[3] = local_18 - *puVar3;
      goto LAB_8ced9c30;
    }
    *param_3 = local_1c;
    param_3[1] = local_18 - local_1c;
  }
  else {
    *param_3 = local_1c;
    param_3[1] = 0;
  }
  param_3[2] = local_1c;
  param_3[3] = 0;
LAB_8ced9c30:
  param_3[4] = *(uint *)(iVar2 + 0x40);
  param_3[5] = local_10[0];
  param_3[6] = local_14;
  return 0;
}


/* Address: 8ced9c48 */

undefined4 callback_candidate_8ced9c48(int param_1,int param_2,int param_3)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  undefined1 auStack_1c [4];
  int local_18;
  
  iVar3 = DAT_8ced9cdc + param_1 + param_2 * DAT_8ced9cde;
  uVar4 = 0;
  local_18 = param_3;
  if (*(int *)(iVar3 + 4) == 0) {
    uVar4 = (*(code *)PTR_FUN_8ced9ce4)(param_1,DAT_8ced9ce0);
  }
  else {
    (*(code *)PTR_FUN_8ced9ce8)(auStack_1c);
    uVar1 = *(int *)(iVar3 + 0x24) + local_18;
    *(uint *)(iVar3 + 0x24) = uVar1;
    if (*(uint *)(iVar3 + 0x28) <= uVar1) {
      *(undefined4 *)(iVar3 + 0x28) = 0;
      *(undefined4 *)(iVar3 + 0x2c) = 0;
    }
    if (*(uint *)(iVar3 + 0x18) <= *(uint *)(iVar3 + 0x24)) {
      *(int *)(iVar3 + 0x24) = *(int *)(iVar3 + 0x24) - *(int *)(iVar3 + 0x14);
      *(undefined4 *)(iVar3 + 0x30) = 0;
      *(undefined4 *)(iVar3 + 0x34) = 0;
    }
    iVar2 = *(int *)(iVar3 + 0x3c) + local_18;
    *(int *)(iVar3 + 0x3c) = iVar2;
    if (*(int *)(iVar3 + 0x38) < iVar2) {
      uVar4 = (*(code *)PTR_FUN_8ced9ce4)(param_1,DAT_8ced9cec);
    }
    (*(code *)PTR_FUN_8ced9cf0)(auStack_1c);
  }
  return uVar4;
}


/* Address: 8ced9cf4 */

void FUN_8ced9cf4(int param_1,undefined4 *param_2,undefined4 *param_3,undefined4 *param_4,
                 undefined4 *param_5)

{
  undefined1 auStack_18 [4];
  undefined4 *local_14;
  undefined4 *local_10;
  undefined4 *local_c;
  
  local_14 = param_3;
  local_10 = param_4;
  local_c = param_2;
  (*(code *)PTR_FUN_8ced9e54)(auStack_18);
  *local_c = *(undefined4 *)(param_1 + 0x14);
  *local_14 = *(undefined4 *)(param_1 + 0x2c);
  *local_10 = *(undefined4 *)(param_1 + 0x10);
  *param_5 = *(undefined4 *)(param_1 + 0x28);
  (*(code *)PTR_FUN_8ced9e58)(auStack_18);
  return;
}


/* Address: 8ced9d2e */

void callback_candidate_8ced9d2e
               (int param_1,int param_2,undefined4 *param_3,undefined4 *param_4,undefined4 *param_5,
               undefined4 *param_6,undefined4 *param_7)

{
  int iVar1;
  undefined1 auStack_18 [4];
  undefined4 *local_14;
  undefined4 *local_10;
  int local_c;
  int local_8;
  
  local_14 = param_4;
  local_10 = param_3;
  local_c = param_2;
  local_8 = param_1;
  (*(code *)PTR_FUN_8ced9e54)(auStack_18);
  iVar1 = local_8 + DAT_8ced9e4e + local_c * DAT_8ced9e50;
  *local_10 = *(undefined4 *)(iVar1 + 0x28);
  *local_14 = *(undefined4 *)(iVar1 + 0x2c);
  *param_6 = *(undefined4 *)(iVar1 + 0x30);
  *param_5 = *(undefined4 *)(iVar1 + 0x34);
  *param_7 = *(undefined4 *)(iVar1 + 0x1c);
  (*(code *)PTR_FUN_8ced9e58)(auStack_18);
  return;
}


/* Address: 8ced9d7e */

void callback_candidate_8ced9d7e
               (int param_1,int param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5,
               undefined4 param_6)

{
  undefined *puVar1;
  int iVar2;
  undefined1 auStack_18 [4];
  undefined4 local_14;
  undefined4 local_10;
  int local_c;
  int local_8;
  
  local_14 = param_4;
  local_10 = param_3;
  local_c = param_2;
  local_8 = param_1;
  (*(code *)PTR_FUN_8ced9e54)(auStack_18);
  iVar2 = local_8 + DAT_8ced9e4e + local_c * DAT_8ced9e50;
  *(undefined4 *)(iVar2 + 0x28) = local_10;
  *(undefined4 *)(iVar2 + 0x2c) = local_14;
  *(undefined4 *)(iVar2 + 0x30) = param_6;
  puVar1 = PTR_FUN_8ced9e58;
  *(undefined4 *)(iVar2 + 0x34) = param_5;
  (*(code *)puVar1)(auStack_18);
  return;
}


/* Address: 8ceda152 */

undefined4 callback_candidate_8ceda152(int param_1,int param_2,undefined4 param_3)

{
  undefined4 uVar1;
  int iVar2;
  
  iVar2 = param_2 * DAT_8ceda1d4 + DAT_8ceda1d2 + param_1;
  if (*(int *)(iVar2 + 4) == 0) {
    uVar1 = (*(code *)PTR_callback_candidate_8ceda5fc_8ceda1ec)
                      (*(undefined4 *)(DAT_8ceda1d6 + iVar2),0xb,param_3,0);
    return uVar1;
  }
  return 0;
}


/* Address: 8ceda192 */

undefined4 callback_candidate_8ceda192(int param_1,int param_2,undefined4 param_3)

{
  undefined4 uVar1;
  int iVar2;
  
  iVar2 = param_2 * DAT_8ceda1d4 + DAT_8ceda1d2 + param_1;
  if (*(int *)(iVar2 + 4) == 0) {
    uVar1 = (*(code *)PTR_callback_candidate_8ceda5fc_8ceda1ec)
                      (*(undefined4 *)(DAT_8ceda1d6 + iVar2),0xc,param_3,0);
    return uVar1;
  }
  return 0;
}


/* Address: 8ceda244 */

void FUN_8ceda244(int param_1,int param_2,undefined4 param_3)

{
  *(undefined4 *)(param_1 + DAT_8ceda284 + param_2 * DAT_8ceda286 + 8) = param_3;
  return;
}


/* Address: 8ceda254 */

undefined4 FUN_8ceda254(int param_1,int param_2)

{
  return *(undefined4 *)(param_1 + DAT_8ceda284 + param_2 * DAT_8ceda286 + 8);
}


/* Address: 8ceda264 */

void FUN_8ceda264(int param_1,int param_2,undefined4 param_3)

{
  *(undefined4 *)(param_1 + DAT_8ceda284 + param_2 * DAT_8ceda286 + 0xc) = param_3;
  return;
}


/* Address: 8ceda274 */

undefined4 FUN_8ceda274(int param_1,int param_2)

{
  return *(undefined4 *)(param_1 + DAT_8ceda284 + param_2 * DAT_8ceda286 + 0xc);
}


/* Address: 8ceda288 */

void callback_candidate_8ceda288(undefined4 param_1)

{
  (*DAT_8ceda3b4)();
  FUN_8ceda2b2(param_1,0);
  return;
}


/* Address: 8ceda2a6 */

void callback_candidate_8ceda2a6(void)

{
  FUN_8ceda2b2(1);
  return;
}


/* Address: 8ceda2b2 */

int FUN_8ceda2b2(int *param_1,int param_2)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  
  iVar1 = 0;
  piVar2 = param_1 + 0xf;
  piVar3 = param_1;
  while (((param_1 < piVar2 && (*param_1 != 0)) &&
         (iVar1 = (**(code **)(param_2 * 4 + *piVar3))(0,0,0), iVar1 == 0))) {
    param_1 = param_1 + 1;
    piVar3 = piVar3 + 1;
  }
  return iVar1;
}


/* Address: 8ceda304 */

undefined4 callback_candidate_8ceda304(undefined4 param_1,int param_2,int *param_3)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 *puVar4;
  
  puVar1 = (undefined4 *)*param_3;
  iVar3 = (int)DAT_8ceda3ac;
  puVar4 = puVar1;
  do {
    uVar2 = *puVar4;
    puVar4 = puVar4 + 1;
    FUN_8ceda362(param_2,uVar2);
    param_2 = param_2 + iVar3;
  } while (puVar4 < puVar1 + 9);
  iVar3 = FUN_8ceda388(param_1,puVar1);
  if (iVar3 != 0) {
    uVar2 = (*(code *)PTR_FUN_8ceda3bc)(param_1,DAT_8ceda3b8);
    return uVar2;
  }
  return 0;
}


/* Address: 8ceda362 */

void FUN_8ceda362(undefined4 *param_1,undefined4 param_2)

{
  int iVar1;
  int iVar2;
  undefined4 *puVar3;
  
  iVar1 = (int)DAT_8ceda3ae;
  param_1[1] = 0;
  *param_1 = 0;
  *(undefined4 *)(iVar1 + (int)param_1) = param_2;
  iVar2 = (int)DAT_8ceda3b0;
  *(undefined4 *)(iVar1 + 4 + (int)param_1) = 8;
  puVar3 = (undefined4 *)(iVar2 + (int)param_1);
  do {
    iVar1 = (int)DAT_8ceda3b0;
    *puVar3 = 8;
    puVar3 = puVar3 + 1;
  } while (puVar3 < (undefined4 *)((int)param_1 + iVar1 + 0xc));
  return;
}


/* Address: 8ceda388 */

undefined4 FUN_8ceda388(undefined4 param_1,int param_2)

{
  undefined *puVar1;
  undefined4 uVar2;
  
  puVar1 = PTR_callback_candidate_8ced43f8_8ceda548;
  if (*(int *)(param_2 + 4) == 0) {
    if (*(int *)(param_2 + 8) == 0) {
      if (*(int *)(param_2 + 0xc) == 0) {
        if (*(int *)(param_2 + 0x20) == 0) {
          return 0xffffffff;
        }
        FUN_8ceda518(param_1,0,7);
        FUN_8ceda512(param_1,param_2);
        (*(code *)puVar1)(param_1,6,0);
      }
      else {
        FUN_8ceda518(param_1,0,2);
        FUN_8ceda4cc(param_1,param_2);
      }
      uVar2 = 5;
    }
    else {
      FUN_8ceda518(param_1,0,1);
      FUN_8ceda486(param_1,param_2);
      uVar2 = 6;
    }
    (*(code *)puVar1)(param_1,uVar2,0);
  }
  else {
    FUN_8ceda518(param_1,0);
    FUN_8ceda42c(param_1,param_2);
  }
  return 0;
}


/* Address: 8ceda42c */

void FUN_8ceda42c(undefined4 param_1,int param_2)

{
  FUN_8ceda57c(0,1);
  if (*(int *)(param_2 + 8) != 0) {
    FUN_8ceda524(param_1,1);
    FUN_8ceda486(param_1,param_2);
  }
  if (*(int *)(param_2 + 0xc) != 0) {
    FUN_8ceda530(param_1,1,2);
    FUN_8ceda4cc(param_1,param_2);
  }
  if (*(int *)(param_2 + 0x20) != 0) {
    FUN_8ceda53c(param_1,1,7);
    FUN_8ceda512(param_1,param_2);
    return;
  }
  return;
}


/* Address: 8ceda486 */

void FUN_8ceda486(undefined4 param_1,int param_2)

{
  undefined4 uVar1;
  
  FUN_8ceda57c(param_1,1,2);
  FUN_8ceda518(param_1,2,3);
  if (*(int *)(param_2 + 0x10) == 0) {
    uVar1 = 3;
  }
  else {
    FUN_8ceda57c(param_1,3,4);
    FUN_8ceda518(param_1,4,5);
    uVar1 = 5;
  }
  FUN_8ceda57c(param_1,uVar1,6);
  return;
}


/* Address: 8ceda4cc */

void FUN_8ceda4cc(undefined4 param_1,int param_2)

{
  undefined4 uVar1;
  
  FUN_8ceda57c(param_1,2,3);
  FUN_8ceda518(param_1,3,4);
  if (*(int *)(param_2 + 0x14) == 0) {
    uVar1 = 4;
  }
  else {
    FUN_8ceda57c(param_1,4,5);
    FUN_8ceda518(param_1,5,6);
    uVar1 = 6;
  }
  FUN_8ceda57c(param_1,uVar1,7);
  return;
}


/* Address: 8ceda512 */

void FUN_8ceda512(void)

{
  FUN_8ceda57c(7,8);
  return;
}


/* Address: 8ceda518 */

void FUN_8ceda518(undefined4 param_1)

{
  FUN_8ceda54c(0,param_1);
  return;
}


/* Address: 8ceda524 */

void FUN_8ceda524(undefined4 param_1)

{
  FUN_8ceda54c(0,param_1);
  return;
}


/* Address: 8ceda530 */

void FUN_8ceda530(undefined4 param_1)

{
  FUN_8ceda54c(1,param_1);
  return;
}


/* Address: 8ceda53c */

void FUN_8ceda53c(undefined4 param_1)

{
  FUN_8ceda54c(2,param_1);
  return;
}


/* Address: 8ceda54c */

void FUN_8ceda54c(int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  int iVar2;
  
  iVar1 = (int)DAT_8ceda6a2;
  iVar2 = (int)DAT_8ceda6a4;
  *(int *)(param_3 * 4 + (int)DAT_8ceda6a0 + param_2 * DAT_8ceda69e + DAT_8ceda69c + param_1) =
       param_4;
  *(int *)((int)DAT_8ceda6a6 + param_1 + iVar1 + param_4 * iVar2) = param_2;
  return;
}


/* Address: 8ceda57c */

void FUN_8ceda57c(int param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  
  iVar1 = (int)DAT_8ceda69e;
  iVar2 = (int)DAT_8ceda69c;
  *(int *)((int)DAT_8ceda6a8 + DAT_8ceda6a2 + param_1 + param_2 * DAT_8ceda6a4) = param_3;
  *(int *)((int)DAT_8ceda6aa + param_1 + iVar2 + param_3 * iVar1) = param_2;
  return;
}


/* Address: 8ceda59e */

int callback_candidate_8ceda59e(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = 0;
  iVar1 = 0;
  iVar2 = DAT_8ceda69c + param_1;
  while ((iVar3 < 9 &&
         ((*(int *)(DAT_8ceda6ac + iVar2) == 0 ||
          (iVar1 = (**(code **)(param_2 * 4 + *(int *)(DAT_8ceda6ac + iVar2)))(param_1,0,0),
          iVar1 == 0))))) {
    iVar3 = iVar3 + 1;
    iVar2 = iVar2 + DAT_8ceda69e;
  }
  return iVar1;
}


/* Address: 8ceda5fc */

undefined4
callback_candidate_8ceda5fc
          (int param_1,int param_2,int param_3,undefined4 param_4,undefined4 param_5)

{
  undefined4 uVar1;
  int iVar2;
  
  iVar2 = *(int *)((int)DAT_8ceda6ac + param_2 * DAT_8ceda69e + DAT_8ceda69c + param_1);
  if (iVar2 == 0) {
    return 0;
  }
  uVar1 = (**(code **)(iVar2 + param_3 * 4))(param_4,param_5,0,iVar2);
  return uVar1;
}


/* Address: 8ceda63e */

void FUN_8ceda63e(int param_1,int param_2,undefined4 param_3)

{
  *(undefined4 *)(param_1 + DAT_8ceda69c + param_2 * DAT_8ceda69e) = param_3;
  return;
}


/* Address: 8ceda64e */

undefined4 FUN_8ceda64e(int param_1,int param_2)

{
  return *(undefined4 *)(param_1 + DAT_8ceda69c + param_2 * DAT_8ceda69e);
}


/* Address: 8ceda65e */

void FUN_8ceda65e(int param_1,int param_2,undefined4 param_3)

{
  *(undefined4 *)(param_1 + DAT_8ceda69c + param_2 * DAT_8ceda69e + 4) = param_3;
  return;
}


/* Address: 8ceda66e */

undefined4 FUN_8ceda66e(int param_1,int param_2)

{
  return *(undefined4 *)(param_1 + DAT_8ceda69c + param_2 * DAT_8ceda69e + 4);
}


/* Address: 8ceda67e */

undefined4 FUN_8ceda67e(int param_1,int param_2)

{
  if (*(int *)((int)DAT_8ceda6ac + param_1 + DAT_8ceda69c + param_2 * DAT_8ceda69e) == 0) {
    return 0;
  }
  return 1;
}


/* Address: 8ceda70a */

undefined4 FUN_8ceda70a(int *param_1,int *param_2)

{
  if (*param_1 == *param_2) {
    if (*param_1 < 0) {
      if ((uint)param_2[1] <= (uint)param_1[1]) {
        return 1;
      }
    }
    else if ((uint)param_1[1] <= (uint)param_2[1]) {
      return 1;
    }
  }
  else if (*param_1 <= *param_2) {
    return 1;
  }
  return 0;
}


/* Address: 8ceda7ba */

bool FUN_8ceda7ba(int *param_1)

{
  return -1 < *param_1;
}


/* Address: 8ceda7c4 */

void callback_candidate_8ceda7c4(uint param_1)

{
  int *piVar1;
  uint *puVar2;
  undefined *puVar3;
  undefined *puVar4;
  
  puVar2 = DAT_8ceda864;
  piVar1 = DAT_8ceda860;
  if (param_1 == 0) {
    param_1 = DAT_8ceda868 & 0xffffffe0;
  }
  *DAT_8ceda864 = param_1;
  puVar4 = PTR_FUN_8ceda870;
  puVar3 = PTR_DAT_8ceda86c;
  *piVar1 = param_1 + 0x40;
  (*(code *)puVar4)(*puVar2,puVar3,0x10);
  (*(code *)PTR_FUN_8ceda870)(*piVar1,PTR_DAT_8ceda874,0x10);
  return;
}


/* Address: 8ceda7f8 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8ceda7f8(undefined1 *param_1)

{
  undefined4 *puVar1;
  undefined *puVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  char in_FPSCR_SZ;
  
  uVar3 = DAT_8ceda87c;
  puVar2 = PTR_LAB_8ceda878;
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  param_1[3] = 0;
  param_1[4] = 0;
  param_1[5] = 0;
  param_1[6] = 0;
  param_1[7] = 0;
  *(undefined **)(param_1 + 8) = puVar2;
  *(undefined4 *)(param_1 + 0xc) = uVar3;
  uVar3 = DAT_8ceda884;
  *(undefined4 *)(param_1 + 0x10) = DAT_8ceda880;
  *(undefined4 *)(param_1 + 0x14) = uVar3;
  *(undefined4 *)(param_1 + 0x18) = 0;
  *(undefined4 *)(param_1 + 0x1c) = 0;
  iVar5 = (int)DAT_8ceda85a;
  iVar4 = (int)DAT_8ceda85c;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(param_1 + 0x20) = DAT_8ceda888;
  }
  else {
    *(ulonglong *)(param_1 + 0x20) = CONCAT44(_FUN_8ceda88c,DAT_8ceda888);
  }
  *(int *)(param_1 + 0x24) = iVar5;
  *(undefined4 *)(param_1 + 0x28) = 0;
  *(undefined4 *)(param_1 + 0x2c) = 0;
  *(undefined4 *)(param_1 + 0x30) = 0;
  *(undefined4 *)(param_1 + 0x34) = 0;
  *(int *)(param_1 + 0x38) = iVar4;
  puVar1 = DAT_8ceda864;
  *(int *)(param_1 + 0x3c) = iVar5 + -0x80;
  *(undefined4 *)(param_1 + 0x40) = *puVar1;
  *(undefined4 *)(param_1 + 0x44) = *DAT_8ceda860;
  *(undefined4 *)(param_1 + 0x48) = 0;
  *(undefined4 *)(param_1 + 0x4c) = 0;
  *(undefined4 *)(param_1 + 0x50) = 0;
  return;
}


/* Address: 8ceda88c */

undefined4 FUN_8ceda88c(undefined1 *param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = CONCAT31(CONCAT21(CONCAT11(*param_1,param_1[1]),param_1[2]),param_1[3]);
  uVar2 = DAT_8ceda914;
  if (((((iVar1 != DAT_8ceda8fe) && (uVar2 = DAT_8ceda918, iVar1 != DAT_8ceda900)) &&
       (uVar2 = DAT_8ceda91c, iVar1 != DAT_8ceda902)) &&
      ((uVar2 = DAT_8ceda910, iVar1 != DAT_8ceda904 && (iVar1 != DAT_8ceda906)))) &&
     ((iVar1 != DAT_8ceda908 && ((iVar1 < DAT_8ceda90a || (DAT_8ceda90c < iVar1)))))) {
    uVar2 = 0;
  }
  return uVar2;
}


/* Address: 8ceda920 */

void callback_candidate_8ceda920(int param_1)

{
  char cVar1;
  undefined1 uVar2;
  undefined *puVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  char *pcVar8;
  char *pcVar9;
  undefined *puVar10;
  
  iVar5 = DAT_8ceda994;
  iVar4 = DAT_8ceda990;
  puVar3 = PTR_DAT_8ceda98c;
  iVar6 = 0;
  pcVar8 = PTR_DAT_8ceda998;
  pcVar9 = PTR_DAT_8ceda98c;
  puVar10 = PTR_DAT_8ceda988;
  do {
    cVar1 = *pcVar8;
    pcVar8 = pcVar8 + 1;
    iVar7 = iVar6 + 1;
    *(undefined *)(iVar6 + iVar5) = puVar3[cVar1];
    uVar2 = *puVar10;
    puVar10 = puVar10 + 1;
    cVar1 = *pcVar9;
    pcVar9 = pcVar9 + 1;
    *(undefined1 *)(cVar1 + iVar4) = uVar2;
    iVar6 = iVar7;
  } while (iVar7 < 0x40);
  if (param_1 != -0x20) {
    (*(code *)PTR_FUN_8ceda99c)(DAT_8ceda994,0x10);
  }
  if (param_1 != 0) {
    (*(code *)PTR_FUN_8ceda99c)(PTR_DAT_8ceda9a0,8);
    return;
  }
  return;
}


/* Address: 8ceda9a4 */

void callback_candidate_8ceda9a4(int param_1)

{
  FUN_8ceda9c8();
  if (param_1 != 0) {
    (*(code *)PTR_FUN_8cedaad8)(param_1,DAT_8cedaad4,(int)DAT_8cedaac2);
    *DAT_8cedaadc = param_1 + DAT_8cedaac4;
  }
  return;
}


/* Address: 8ceda9c8 */

void FUN_8ceda9c8(void)

{
  undefined2 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined1 *puVar5;
  int iVar6;
  
  iVar2 = (int)DAT_8cedaac4;
  iVar3 = iVar2;
  puVar5 = DAT_8cedaad4;
  do {
    iVar3 = iVar3 + -1;
    *puVar5 = 0;
    puVar5 = puVar5 + 1;
  } while (iVar3 != 0);
  iVar6 = (int)DAT_8cedaac2;
  iVar3 = 0;
  do {
    *puVar5 = (char)iVar3;
    uVar1 = DAT_8cedaac6;
    iVar3 = iVar3 + 1;
    puVar5 = puVar5 + 1;
    iVar4 = iVar2;
  } while (iVar3 < iVar6);
  do {
    iVar4 = iVar4 + -1;
    *puVar5 = (char)uVar1;
    puVar5 = puVar5 + 1;
  } while (iVar4 != 0);
  *DAT_8cedaadc = DAT_8cedaad4 + iVar2;
  return;
}


/* Address: 8ceda9fc */

void callback_candidate_8ceda9fc(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  
  (*(code *)PTR_FUN_8cedaae0)(param_1 + DAT_8cedaac8);
  puVar3 = (undefined4 *)(DAT_8cedaaca + param_1);
  iVar2 = DAT_8cedaacc + param_1;
  if (*(int *)(DAT_8cedaace + param_1) == 0) {
    uVar1 = 0xffffffff;
  }
  else {
    uVar1 = 4;
  }
  *puVar3 = uVar1;
  puVar3[1] = iVar2;
  puVar3[3] = iVar2;
  puVar3[5] = iVar2;
  puVar3[7] = iVar2;
  puVar3[9] = iVar2;
  puVar3[0xb] = iVar2;
  puVar3[2] = 8;
  puVar3[4] = 8;
  puVar3[6] = 8;
  puVar3[8] = 8;
  puVar3[10] = 8;
  puVar3[0xc] = 8;
  return;
}


/* Address: 8cedaa72 */

void FUN_8cedaa72(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  int iVar4;
  
  iVar4 = (int)DAT_8cedaad2;
  puVar3 = (undefined4 *)(DAT_8cedaad0 + param_1);
  if (*(int *)(DAT_8cedaace + param_1) == 0) {
    uVar2 = 0xffffffff;
  }
  else {
    uVar2 = 4;
  }
  *puVar3 = uVar2;
  iVar1 = (int)*(short *)(iVar4 + param_1 + 0x1c);
  puVar3[2] = iVar1;
  puVar3[4] = iVar1;
  iVar4 = (int)*(short *)(iVar4 + param_1 + 0x1e);
  puVar3[6] = iVar4;
  puVar3[8] = iVar4;
  puVar3[10] = iVar4;
  puVar3[0xc] = iVar4;
  return;
}


/* Address: 8cedaaa6 */

void FUN_8cedaaa6(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  
  iVar2 = (int)DAT_8cedaad0;
  if (*(int *)(DAT_8cedaace + param_1) == 0) {
    uVar1 = 0xffffffff;
  }
  else {
    uVar1 = 4;
  }
  *(undefined4 *)(DAT_8cedaaca + param_1) = uVar1;
  *(undefined4 *)(iVar2 + param_1) = uVar1;
  return;
}


/* Address: 8cedab10 */

undefined4 FUN_8cedab10(void)

{
  return 0;
}


/* Address: 8cedab14 */

void callback_candidate_8cedab14(int param_1)

{
  FUN_8cedab72();
  FUN_8cedadac();
  FUN_8cedafb6();
  FUN_8cedb1fc();
  FUN_8cedb26c();
  FUN_8cedb312();
  FUN_8cedb486();
  (*(code *)PTR_FUN_8cedabd0)();
  (*(code *)PTR_FUN_8cedabd4)();
  (*(code *)PTR_FUN_8cedabd8)();
  (*(code *)PTR_FUN_8cedabdc)();
  if (param_1 != 0) {
    (*(code *)PTR_callback_candidate_8cedc0ba_8cedabe0)(param_1);
    return;
  }
  return;
}


/* Address: 8cedab64 */

void FUN_8cedab64(void)

{
  FUN_8cedab72();
  FUN_8cedadac();
  FUN_8cedafb6();
  return;
}


/* Address: 8cedab72 */

void FUN_8cedab72(void)

{
  ushort uVar1;
  ushort uVar2;
  ushort uVar3;
  ushort uVar4;
  ushort uVar5;
  int iVar6;
  int iVar7;
  ushort *puVar8;
  int iVar9;
  int iVar10;
  
  uVar5 = DAT_8cedabca;
  puVar8 = DAT_8cedabe4;
  for (iVar9 = 0x10; uVar1 = DAT_8cedabcc, iVar9 != 0; iVar9 = iVar9 + -1) {
    *puVar8 = uVar5;
    puVar8 = puVar8 + 1;
  }
  for (iVar9 = 2; iVar9 != 0; iVar9 = iVar9 + -1) {
    *puVar8 = uVar1;
    puVar8 = puVar8 + 1;
  }
  for (iVar9 = 0xc; uVar1 = DAT_8cedabce, iVar9 != 0; iVar9 = iVar9 + -1) {
    *puVar8 = uVar5;
    puVar8 = puVar8 + 1;
  }
  for (iVar9 = 2; iVar9 != 0; iVar9 = iVar9 + -1) {
    *puVar8 = uVar1;
    puVar8 = puVar8 + 1;
  }
  for (iVar9 = 0x10; uVar2 = DAT_8cedacc6, uVar1 = DAT_8cedacc4, iVar9 != 0; iVar9 = iVar9 + -1) {
    *puVar8 = uVar5;
    puVar8 = puVar8 + 1;
  }
  iVar10 = (int)DAT_8cedacc2;
  iVar6 = (int)DAT_8cedacc8;
  iVar9 = iVar10;
  do {
    *puVar8 = (ushort)iVar10 | uVar1;
    iVar10 = iVar10 + -0x10;
    puVar8[1] = (ushort)iVar9 | uVar2;
    uVar4 = DAT_8cedacd0;
    uVar3 = DAT_8cedacce;
    puVar8 = puVar8 + 2;
    iVar9 = iVar9 + -0x10;
  } while (iVar6 <= iVar10);
  iVar10 = (int)DAT_8cedacca;
  iVar6 = (int)DAT_8cedaccc;
  iVar9 = iVar10;
  do {
    for (iVar7 = 2; iVar7 != 0; iVar7 = iVar7 + -1) {
      *puVar8 = (ushort)iVar10 | uVar3;
      puVar8 = puVar8 + 1;
    }
    for (iVar7 = 2; uVar2 = DAT_8cedacd6, uVar1 = DAT_8cedacd4, iVar7 != 0; iVar7 = iVar7 + -1) {
      *puVar8 = (ushort)iVar9 | uVar4;
      puVar8 = puVar8 + 1;
    }
    iVar10 = iVar10 + -0x10;
    iVar9 = iVar9 + -0x10;
  } while (iVar6 <= iVar10);
  iVar6 = (int)DAT_8cedacd2;
  iVar9 = iVar6;
  do {
    for (iVar10 = 8; iVar10 != 0; iVar10 = iVar10 + -1) {
      *puVar8 = (ushort)iVar6 | uVar1;
      puVar8 = puVar8 + 1;
    }
    for (iVar10 = 8; uVar4 = DAT_8cedace0, uVar3 = DAT_8cedacde, iVar10 != 0; iVar10 = iVar10 + -1)
    {
      *puVar8 = (ushort)iVar9 | uVar2;
      puVar8 = puVar8 + 1;
    }
    iVar6 = iVar6 + -0x10;
    iVar9 = iVar9 + -0x10;
  } while (DAT_8cedacd8 <= iVar6);
  iVar10 = (int)DAT_8cedacda;
  iVar6 = (int)DAT_8cedacdc;
  iVar9 = iVar10;
  do {
    for (iVar7 = 0x10; iVar7 != 0; iVar7 = iVar7 + -1) {
      *puVar8 = (ushort)iVar10 | uVar3;
      puVar8 = puVar8 + 1;
    }
    for (iVar7 = 0x10; iVar7 != 0; iVar7 = iVar7 + -1) {
      *puVar8 = (ushort)iVar9 | uVar4;
      puVar8 = puVar8 + 1;
    }
    iVar10 = iVar10 + -0x10;
    iVar9 = iVar9 + -0x10;
  } while (iVar6 <= iVar10);
  puVar8 = DAT_8cedace4;
  for (iVar9 = 4; uVar2 = DAT_8cedadd4, uVar1 = DAT_8cedadd2, iVar9 != 0; iVar9 = iVar9 + -1) {
    *puVar8 = uVar5;
    puVar8 = puVar8 + 1;
  }
  uVar5 = 0x70;
  iVar9 = 0x70;
  do {
    *puVar8 = (ushort)iVar9 | uVar2;
    iVar9 = iVar9 + -0x10;
    puVar8[1] = uVar5 | uVar1;
    uVar4 = DAT_8cedadd8;
    uVar3 = DAT_8cedadd6;
    puVar8 = puVar8 + 2;
    uVar5 = uVar5 - 0x10;
  } while (0x5f < iVar9);
  uVar5 = 0x50;
  iVar9 = 0x50;
  do {
    for (iVar6 = 2; iVar6 != 0; iVar6 = iVar6 + -1) {
      *puVar8 = (ushort)iVar9 | uVar4;
      puVar8 = puVar8 + 1;
    }
    for (iVar6 = 2; uVar2 = DAT_8cedaddc, uVar1 = DAT_8cedadda, iVar6 != 0; iVar6 = iVar6 + -1) {
      *puVar8 = uVar5 | uVar3;
      puVar8 = puVar8 + 1;
    }
    iVar9 = iVar9 + -0x10;
    uVar5 = uVar5 - 0x10;
  } while (0x3f < iVar9);
  uVar5 = 0x30;
  iVar9 = 0x30;
  do {
    for (iVar6 = 4; iVar6 != 0; iVar6 = iVar6 + -1) {
      *puVar8 = (ushort)iVar9 | uVar1;
      puVar8 = puVar8 + 1;
    }
    for (iVar6 = 4; uVar3 = DAT_8cedadde, iVar6 != 0; iVar6 = iVar6 + -1) {
      *puVar8 = uVar5 | uVar2;
      puVar8 = puVar8 + 1;
    }
    iVar9 = iVar9 + -0x10;
    uVar5 = uVar5 - 0x10;
  } while (0x1f < iVar9);
  for (iVar9 = 0x10; uVar5 = DAT_8cedade0, iVar9 != 0; iVar9 = iVar9 + -1) {
    *puVar8 = uVar3;
    puVar8 = puVar8 + 1;
  }
  for (iVar9 = 0x10; iVar9 != 0; iVar9 = iVar9 + -1) {
    *puVar8 = uVar5;
    puVar8 = puVar8 + 1;
  }
  return;
}


/* Address: 8cedadac */

void FUN_8cedadac(void)

{
  ushort uVar1;
  ushort uVar2;
  ushort uVar3;
  ushort uVar4;
  ushort *puVar5;
  ushort *puVar6;
  ushort uVar7;
  ushort uVar8;
  ushort uVar9;
  int iVar10;
  int iVar11;
  
  uVar1 = DAT_8cedade2;
  puVar5 = DAT_8cedade8;
  for (iVar10 = 8; iVar10 != 0; iVar10 = iVar10 + -1) {
    *puVar5 = uVar1;
    puVar5 = puVar5 + 1;
  }
  *puVar5 = DAT_8cedade4;
  for (iVar10 = 6; puVar5 = puVar5 + 1, iVar10 != 0; iVar10 = iVar10 + -1) {
    *puVar5 = uVar1;
  }
  *puVar5 = DAT_8cedaee2;
  for (iVar10 = 8; puVar5 = puVar5 + 1, iVar10 != 0; iVar10 = iVar10 + -1) {
    *puVar5 = uVar1;
  }
  iVar11 = (int)DAT_8cedaee6;
  iVar10 = (int)DAT_8cedaee4;
  do {
    uVar7 = (ushort)iVar10;
    iVar10 = iVar10 + -0x10;
    *puVar5 = uVar7 | 0xb;
    uVar7 = DAT_8cedaeea;
    puVar5 = puVar5 + 1;
  } while (iVar11 <= iVar10);
  iVar11 = (int)DAT_8cedaeec;
  iVar10 = (int)DAT_8cedaee8;
  do {
    uVar9 = (ushort)iVar10;
    *puVar5 = uVar9 | 10;
    iVar10 = iVar10 + -0x10;
    puVar5[1] = uVar9 | uVar7;
    uVar2 = DAT_8cedaef4;
    uVar8 = DAT_8cedaef2;
    uVar9 = DAT_8cedaef0;
    puVar5 = puVar5 + 2;
  } while (iVar11 <= iVar10);
  iVar11 = (int)DAT_8cedaef6;
  iVar10 = (int)DAT_8cedaeee;
  do {
    uVar7 = (ushort)iVar10;
    *puVar5 = uVar7 | 8;
    puVar5[1] = uVar7 | uVar2;
    puVar5[2] = uVar7 | uVar9;
    puVar5[3] = uVar7 | uVar9;
    uVar7 = uVar7 | uVar8;
    puVar5[4] = uVar7;
    puVar5[5] = uVar7;
    puVar5[6] = uVar7;
    iVar10 = iVar10 + -0x10;
    puVar5[7] = uVar7;
    uVar4 = DAT_8cedaefe;
    uVar3 = DAT_8cedaefc;
    uVar7 = DAT_8cedaefa;
    puVar5 = puVar5 + 8;
  } while (iVar11 <= iVar10);
  iVar11 = (int)DAT_8cedaf00;
  iVar10 = (int)DAT_8cedaef8;
  do {
    uVar9 = (ushort)iVar10;
    *puVar5 = uVar9 | 7;
    puVar5[1] = uVar9 | 7;
    puVar5[2] = uVar9 | uVar4;
    puVar5[3] = uVar9 | uVar4;
    uVar8 = uVar9 | uVar7;
    puVar5[4] = uVar8;
    puVar5[5] = uVar8;
    puVar5[6] = uVar8;
    puVar5[7] = uVar8;
    uVar9 = uVar9 | uVar3;
    puVar5[8] = uVar9;
    puVar5[9] = uVar9;
    puVar5[10] = uVar9;
    puVar5[0xb] = uVar9;
    puVar5[0xc] = uVar9;
    puVar5[0xd] = uVar9;
    puVar5[0xe] = uVar9;
    iVar10 = iVar10 + -0x10;
    puVar5[0xf] = uVar9;
    puVar5 = puVar5 + 0x10;
  } while (iVar11 <= iVar10);
  puVar5 = DAT_8cedaf04;
  for (iVar10 = 2; iVar10 != 0; iVar10 = iVar10 + -1) {
    *puVar5 = uVar1;
    puVar5 = puVar5 + 1;
  }
  iVar10 = 0x70;
  do {
    uVar1 = (ushort)iVar10;
    iVar10 = iVar10 + -0x10;
    *puVar5 = uVar1 | 5;
    uVar1 = DAT_8cedaff8;
    puVar5 = puVar5 + 1;
  } while (0x5f < iVar10);
  iVar10 = 0x50;
  do {
    uVar7 = (ushort)iVar10;
    *puVar5 = uVar7 | 4;
    iVar10 = iVar10 + -0x10;
    puVar5[1] = uVar7 | uVar1;
    uVar9 = DAT_8cedaffc;
    uVar7 = DAT_8cedaffa;
    puVar5 = puVar5 + 2;
  } while (0x3f < iVar10);
  iVar10 = 0x30;
  do {
    puVar6 = puVar5;
    uVar1 = (ushort)iVar10;
    *puVar6 = uVar1 | 3;
    puVar6[1] = uVar1 | uVar9;
    puVar6[2] = uVar1 | uVar7;
    iVar10 = iVar10 + -0x10;
    puVar6[3] = uVar1 | uVar7;
    puVar5 = puVar6 + 4;
  } while (0x1f < iVar10);
  puVar6[4] = 0x11;
  puVar6[5] = 0x11;
  uVar1 = DAT_8cedaffe;
  puVar6[6] = DAT_8cedaffe;
  puVar6[7] = uVar1;
  uVar1 = DAT_8cedb000;
  puVar6[8] = DAT_8cedb000;
  puVar6[9] = uVar1;
  puVar6[10] = uVar1;
  puVar6[0xb] = uVar1;
  uVar1 = DAT_8cedb002;
  puVar6[0xc] = DAT_8cedb002;
  puVar6[0xd] = uVar1;
  puVar6[0xe] = uVar1;
  puVar6[0xf] = uVar1;
  puVar6[0x10] = uVar1;
  puVar6[0x11] = uVar1;
  puVar6[0x12] = uVar1;
  puVar6[0x13] = uVar1;
  return;
}


/* Address: 8cedafb6 */

void FUN_8cedafb6(void)

{
  ushort uVar1;
  ushort uVar2;
  ushort uVar3;
  ushort uVar4;
  ushort uVar5;
  ushort uVar6;
  ushort uVar7;
  ushort uVar8;
  ushort *puVar9;
  ushort *puVar10;
  ushort uVar11;
  ushort uVar12;
  ushort uVar13;
  int iVar14;
  int iVar15;
  int iVar16;
  
  uVar13 = DAT_8cedb004;
  puVar9 = DAT_8cedb00c;
  for (iVar14 = 8; iVar14 != 0; iVar14 = iVar14 + -1) {
    *puVar9 = uVar13;
    puVar9 = puVar9 + 1;
  }
  *puVar9 = DAT_8cedb006;
  for (iVar14 = 6; puVar9 = puVar9 + 1, iVar14 != 0; iVar14 = iVar14 + -1) {
    *puVar9 = uVar13;
  }
  *puVar9 = DAT_8cedb008;
  for (iVar14 = 8; puVar9 = puVar9 + 1, iVar14 != 0; iVar14 = iVar14 + -1) {
    *puVar9 = uVar13;
  }
  iVar15 = (int)DAT_8cedb10e;
  iVar14 = (int)DAT_8cedb10c;
  do {
    uVar12 = (ushort)iVar14;
    iVar14 = iVar14 + -0x10;
    *puVar9 = uVar12 | 0xb;
    puVar9 = puVar9 + 1;
  } while (iVar15 <= iVar14);
  iVar15 = (int)DAT_8cedb110;
  iVar16 = (int)DAT_8cedb112;
  iVar14 = iVar15;
  do {
    *puVar9 = (ushort)iVar15 | 10;
    iVar15 = iVar15 + -0x10;
    puVar9[1] = (ushort)iVar14 | 10;
    uVar3 = DAT_8cedb11e;
    uVar2 = DAT_8cedb11a;
    uVar11 = DAT_8cedb118;
    uVar12 = DAT_8cedb116;
    iVar14 = iVar14 + -0x10;
    puVar9 = puVar9 + 2;
  } while (iVar16 <= iVar15);
  iVar15 = (int)DAT_8cedb11c;
  iVar14 = (int)DAT_8cedb114;
  do {
    uVar1 = (ushort)iVar14;
    *puVar9 = uVar1 | 8;
    puVar9[1] = uVar1 | 8;
    puVar9[2] = uVar1 | uVar11;
    puVar9[3] = uVar1 | uVar2;
    puVar9[4] = uVar1 | uVar12;
    puVar9[5] = uVar1 | uVar12;
    puVar9[6] = uVar1 | uVar3;
    iVar14 = iVar14 + -0x10;
    puVar9[7] = uVar1 | uVar3;
    uVar8 = DAT_8cedb12c;
    uVar7 = DAT_8cedb12a;
    uVar6 = DAT_8cedb128;
    uVar5 = DAT_8cedb126;
    uVar4 = DAT_8cedb124;
    uVar1 = DAT_8cedb122;
    puVar9 = puVar9 + 8;
  } while (iVar15 <= iVar14);
  iVar15 = (int)DAT_8cedb120;
  iVar14 = iVar15 + -0x10;
  do {
    uVar12 = (ushort)iVar15;
    *puVar9 = uVar12 | 7;
    puVar9[1] = uVar12 | 7;
    puVar9[2] = uVar12 | uVar1;
    puVar9[3] = uVar12 | uVar8;
    puVar9[4] = uVar12 | uVar4;
    puVar9[5] = uVar12 | uVar4;
    puVar9[6] = uVar12 | uVar7;
    puVar9[7] = uVar12 | uVar7;
    uVar11 = uVar12 | uVar5;
    puVar9[8] = uVar11;
    puVar9[9] = uVar11;
    puVar9[10] = uVar11;
    puVar9[0xb] = uVar11;
    uVar12 = uVar12 | uVar6;
    puVar9[0xc] = uVar12;
    puVar9[0xd] = uVar12;
    puVar9[0xe] = uVar12;
    iVar15 = iVar15 + -0x10;
    puVar9[0xf] = uVar12;
    puVar9 = puVar9 + 0x10;
  } while (iVar14 <= iVar15);
  puVar9 = DAT_8cedb130;
  for (iVar14 = 2; iVar14 != 0; iVar14 = iVar14 + -1) {
    *puVar9 = uVar13;
    puVar9 = puVar9 + 1;
  }
  iVar14 = 0x70;
  do {
    uVar13 = (ushort)iVar14;
    iVar14 = iVar14 + -0x10;
    *puVar9 = uVar13 | 5;
    puVar9 = puVar9 + 1;
  } while (0x5f < iVar14);
  iVar14 = 0x50;
  uVar13 = 0x50;
  do {
    *puVar9 = (ushort)iVar14 | 4;
    iVar14 = iVar14 + -0x10;
    puVar9[1] = uVar13 | 4;
    uVar11 = DAT_8cedb24c;
    uVar12 = DAT_8cedb24a;
    uVar13 = uVar13 - 0x10;
    puVar9 = puVar9 + 2;
  } while (0x3f < iVar14);
  iVar14 = 0x30;
  do {
    puVar10 = puVar9;
    uVar13 = (ushort)iVar14;
    *puVar10 = uVar13 | 3;
    puVar10[1] = uVar13 | 3;
    puVar10[2] = uVar13 | uVar12;
    iVar14 = iVar14 + -0x10;
    puVar10[3] = uVar13 | uVar11;
    uVar13 = DAT_8cedb24e;
    puVar9 = puVar10 + 4;
  } while (0x1f < iVar14);
  puVar10[4] = 0x11;
  puVar10[5] = 0x11;
  puVar10[6] = uVar13;
  uVar13 = DAT_8cedb252;
  puVar10[7] = DAT_8cedb250;
  puVar10[8] = uVar13;
  puVar10[9] = uVar13;
  uVar13 = DAT_8cedb254;
  puVar10[10] = DAT_8cedb254;
  puVar10[0xb] = uVar13;
  uVar13 = DAT_8cedb256;
  puVar10[0xc] = DAT_8cedb256;
  puVar10[0xd] = uVar13;
  puVar10[0xe] = uVar13;
  puVar10[0xf] = uVar13;
  uVar13 = DAT_8cedb258;
  puVar10[0x10] = DAT_8cedb258;
  puVar10[0x11] = uVar13;
  puVar10[0x12] = uVar13;
  puVar10[0x13] = uVar13;
  return;
}


/* Address: 8cedb1f2 */

void FUN_8cedb1f2(void)

{
  FUN_8cedb1fc();
  FUN_8cedb26c();
  return;
}


/* Address: 8cedb1fc */

void FUN_8cedb1fc(void)

{
  undefined2 uVar1;
  undefined2 uVar2;
  undefined2 *puVar3;
  int iVar4;
  
  puVar3 = DAT_8cedb268;
  uVar1 = DAT_8cedb262;
  *DAT_8cedb268 = DAT_8cedb25a;
  puVar3[1] = DAT_8cedb25c;
  puVar3[2] = DAT_8cedb25e;
  puVar3 = puVar3 + 3;
  *puVar3 = DAT_8cedb260;
  for (iVar4 = 4; uVar2 = DAT_8cedb264, puVar3 = puVar3 + 1, iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar3 = uVar1;
  }
  for (iVar4 = 8; uVar1 = DAT_8cedb266, iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar3 = uVar2;
    puVar3 = puVar3 + 1;
  }
  for (iVar4 = 0x10; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar3 = uVar1;
    puVar3 = puVar3 + 1;
  }
  return;
}


/* Address: 8cedb26c */

void FUN_8cedb26c(void)

{
  undefined2 uVar1;
  undefined2 uVar2;
  undefined2 *puVar3;
  int iVar4;
  
  puVar3 = DAT_8cedb378;
  uVar1 = DAT_8cedb362;
  *DAT_8cedb378 = DAT_8cedb35a;
  puVar3[1] = DAT_8cedb35c;
  puVar3[2] = DAT_8cedb35e;
  puVar3 = puVar3 + 3;
  *puVar3 = DAT_8cedb360;
  for (iVar4 = 2; uVar2 = DAT_8cedb364, puVar3 = puVar3 + 1, iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar3 = uVar1;
  }
  for (iVar4 = 2; uVar1 = DAT_8cedb366, iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar3 = uVar2;
    puVar3 = puVar3 + 1;
  }
  for (iVar4 = 4; uVar2 = DAT_8cedb368, iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar3 = uVar1;
    puVar3 = puVar3 + 1;
  }
  for (iVar4 = 4; uVar1 = DAT_8cedb36a, iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar3 = uVar2;
    puVar3 = puVar3 + 1;
  }
  for (iVar4 = 8; uVar2 = DAT_8cedb36c, iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar3 = uVar1;
    puVar3 = puVar3 + 1;
  }
  for (iVar4 = 8; uVar1 = DAT_8cedb36e, iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar3 = uVar2;
    puVar3 = puVar3 + 1;
  }
  for (iVar4 = 0x10; uVar2 = DAT_8cedb370, iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar3 = uVar1;
    puVar3 = puVar3 + 1;
  }
  for (iVar4 = 0x10; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar3 = uVar2;
    puVar3 = puVar3 + 1;
  }
  return;
}


/* Address: 8cedb312 */

void FUN_8cedb312(void)

{
  ushort uVar1;
  ushort uVar2;
  undefined2 uVar3;
  undefined2 uVar4;
  int iVar5;
  ushort *puVar6;
  undefined2 *puVar7;
  int iVar8;
  
  puVar6 = DAT_8cedb37c;
  for (iVar8 = 0x18; uVar2 = DAT_8cedb372, iVar8 != 0; iVar8 = iVar8 + -1) {
    *puVar6 = 0x7f;
    puVar6 = puVar6 + 1;
  }
  iVar8 = 0x10;
  do {
    uVar1 = (ushort)iVar8;
    *puVar6 = uVar1 & 0xff | uVar2;
    iVar8 = iVar8 + -1;
    puVar6[1] = -uVar1 & 0xff | uVar2;
    uVar1 = DAT_8cedb374;
    puVar6 = puVar6 + 2;
  } while (10 < iVar8);
  iVar8 = 10;
  do {
    for (iVar5 = 2; iVar5 != 0; iVar5 = iVar5 + -1) {
      *puVar6 = (ushort)iVar8 & 0xff | uVar1;
      puVar6 = puVar6 + 1;
    }
    for (iVar5 = 2; uVar2 = DAT_8cedb44c, iVar5 != 0; iVar5 = iVar5 + -1) {
      *puVar6 = -(ushort)iVar8 & 0xff | uVar1;
      puVar6 = puVar6 + 1;
    }
    iVar8 = iVar8 + -1;
  } while (7 < iVar8);
  iVar8 = 7;
  do {
    for (iVar5 = 8; iVar5 != 0; iVar5 = iVar5 + -1) {
      *puVar6 = (ushort)iVar8 & 0xff | uVar2;
      puVar6 = puVar6 + 1;
    }
    for (iVar5 = 8; uVar1 = DAT_8cedb44e, iVar5 != 0; iVar5 = iVar5 + -1) {
      *puVar6 = -(ushort)iVar8 & 0xff | uVar2;
      puVar6 = puVar6 + 1;
    }
    iVar8 = iVar8 + -1;
  } while (4 < iVar8);
  for (iVar8 = 0x10; uVar2 = DAT_8cedb450, iVar8 != 0; iVar8 = iVar8 + -1) {
    *puVar6 = uVar1;
    puVar6 = puVar6 + 1;
  }
  for (iVar8 = 0x10; iVar8 != 0; iVar8 = iVar8 + -1) {
    *puVar6 = uVar2;
    puVar6 = puVar6 + 1;
  }
  puVar7 = DAT_8cedb460;
  for (iVar8 = 2; uVar3 = DAT_8cedb456, iVar8 != 0; iVar8 = iVar8 + -1) {
    *puVar7 = 0x7f;
    puVar7 = puVar7 + 1;
  }
  *puVar7 = DAT_8cedb452;
  puVar7 = puVar7 + 1;
  *puVar7 = DAT_8cedb454;
  for (iVar8 = 2; uVar4 = DAT_8cedb458, puVar7 = puVar7 + 1, iVar8 != 0; iVar8 = iVar8 + -1) {
    *puVar7 = uVar3;
  }
  for (iVar8 = 2; uVar3 = DAT_8cedb45a, iVar8 != 0; iVar8 = iVar8 + -1) {
    *puVar7 = uVar4;
    puVar7 = puVar7 + 1;
  }
  for (iVar8 = 4; uVar4 = DAT_8cedb45c, iVar8 != 0; iVar8 = iVar8 + -1) {
    *puVar7 = uVar3;
    puVar7 = puVar7 + 1;
  }
  for (iVar8 = 4; uVar3 = DAT_8cedb560, iVar8 != 0; iVar8 = iVar8 + -1) {
    *puVar7 = uVar4;
    puVar7 = puVar7 + 1;
  }
  for (iVar8 = 0x10; iVar8 != 0; iVar8 = iVar8 + -1) {
    *puVar7 = uVar3;
    puVar7 = puVar7 + 1;
  }
  return;
}


/* Address: 8cedb486 */

void FUN_8cedb486(void)

{
  undefined2 uVar1;
  undefined2 uVar2;
  int iVar3;
  int local_8;
  undefined2 *local_4;
  
  local_4 = DAT_8cedb570;
  for (iVar3 = 0; uVar1 = DAT_8cedb562, iVar3 < 2; iVar3 = iVar3 + 1) {
    *local_4 = 0;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar2 = DAT_8cedb564, local_8 < 1; local_8 = local_8 + 1) {
    *local_4 = uVar1;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar1 = DAT_8cedb566, local_8 < 1; local_8 = local_8 + 1) {
    *local_4 = uVar2;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar2 = DAT_8cedb568, local_8 < 1; local_8 = local_8 + 1) {
    *local_4 = uVar1;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar1 = DAT_8cedb56a, local_8 < 1; local_8 = local_8 + 1) {
    *local_4 = uVar2;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar2 = DAT_8cedb56c, local_8 < 1; local_8 = local_8 + 1) {
    *local_4 = uVar1;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar1 = DAT_8cedb662, local_8 < 1; local_8 = local_8 + 1) {
    *local_4 = uVar2;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar2 = DAT_8cedb664, local_8 < 2; local_8 = local_8 + 1) {
    *local_4 = uVar1;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar1 = DAT_8cedb666, local_8 < 2; local_8 = local_8 + 1) {
    *local_4 = uVar2;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar2 = DAT_8cedb668, local_8 < 2; local_8 = local_8 + 1) {
    *local_4 = uVar1;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar1 = DAT_8cedb66a, local_8 < 2; local_8 = local_8 + 1) {
    *local_4 = uVar2;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar2 = DAT_8cedb66c, local_8 < 2; local_8 = local_8 + 1) {
    *local_4 = uVar1;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar1 = DAT_8cedb66e, local_8 < 2; local_8 = local_8 + 1) {
    *local_4 = uVar2;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar2 = DAT_8cedb75e, local_8 < 2; local_8 = local_8 + 1) {
    *local_4 = uVar1;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar1 = DAT_8cedb760, local_8 < 2; local_8 = local_8 + 1) {
    *local_4 = uVar2;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar2 = DAT_8cedb762, local_8 < 2; local_8 = local_8 + 1) {
    *local_4 = uVar1;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar1 = DAT_8cedb764, local_8 < 2; local_8 = local_8 + 1) {
    *local_4 = uVar2;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar2 = DAT_8cedb766, local_8 < 2; local_8 = local_8 + 1) {
    *local_4 = uVar1;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar1 = DAT_8cedb768, local_8 < 2; local_8 = local_8 + 1) {
    *local_4 = uVar2;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar2 = DAT_8cedb76a, local_8 < 2; local_8 = local_8 + 1) {
    *local_4 = uVar1;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar1 = DAT_8cedb85a, local_8 < 2; local_8 = local_8 + 1) {
    *local_4 = uVar2;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar2 = DAT_8cedb85c, local_8 < 2; local_8 = local_8 + 1) {
    *local_4 = uVar1;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar1 = DAT_8cedb85e, local_8 < 2; local_8 = local_8 + 1) {
    *local_4 = uVar2;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar2 = DAT_8cedb860, local_8 < 2; local_8 = local_8 + 1) {
    *local_4 = uVar1;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar1 = DAT_8cedb862, local_8 < 2; local_8 = local_8 + 1) {
    *local_4 = uVar2;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar2 = DAT_8cedb864, local_8 < 2; local_8 = local_8 + 1) {
    *local_4 = uVar1;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar1 = DAT_8cedb866, local_8 < 2; local_8 = local_8 + 1) {
    *local_4 = uVar2;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar2 = DAT_8cedb956, local_8 < 2; local_8 = local_8 + 1) {
    *local_4 = uVar1;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar1 = DAT_8cedb958, local_8 < 2; local_8 = local_8 + 1) {
    *local_4 = uVar2;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar2 = DAT_8cedb95a, local_8 < 2; local_8 = local_8 + 1) {
    *local_4 = uVar1;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar1 = DAT_8cedb95c, local_8 < 2; local_8 = local_8 + 1) {
    *local_4 = uVar2;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar2 = DAT_8cedb95e, local_8 < 2; local_8 = local_8 + 1) {
    *local_4 = uVar1;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar1 = DAT_8cedb960, local_8 < 2; local_8 = local_8 + 1) {
    *local_4 = uVar2;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar2 = DAT_8cedb962, local_8 < 2; local_8 = local_8 + 1) {
    *local_4 = uVar1;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar1 = DAT_8cedba52, local_8 < 2; local_8 = local_8 + 1) {
    *local_4 = uVar2;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar2 = DAT_8cedba54, local_8 < 4; local_8 = local_8 + 1) {
    *local_4 = uVar1;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar1 = DAT_8cedba56, local_8 < 4; local_8 = local_8 + 1) {
    *local_4 = uVar2;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar2 = DAT_8cedba58, local_8 < 4; local_8 = local_8 + 1) {
    *local_4 = uVar1;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar1 = DAT_8cedba5a, local_8 < 4; local_8 = local_8 + 1) {
    *local_4 = uVar2;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar2 = DAT_8cedba5c, local_8 < 4; local_8 = local_8 + 1) {
    *local_4 = uVar1;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar1 = DAT_8cedba5e, local_8 < 4; local_8 = local_8 + 1) {
    *local_4 = uVar2;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar2 = DAT_8cedbb4e, local_8 < 4; local_8 = local_8 + 1) {
    *local_4 = uVar1;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar1 = DAT_8cedbb50, local_8 < 4; local_8 = local_8 + 1) {
    *local_4 = uVar2;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar2 = DAT_8cedbb52, local_8 < 8; local_8 = local_8 + 1) {
    *local_4 = uVar1;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar1 = DAT_8cedbb54, local_8 < 8; local_8 = local_8 + 1) {
    *local_4 = uVar2;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar2 = DAT_8cedbb56, local_8 < 8; local_8 = local_8 + 1) {
    *local_4 = uVar1;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar1 = DAT_8cedbb58, local_8 < 8; local_8 = local_8 + 1) {
    *local_4 = uVar2;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar2 = DAT_8cedbb5a, local_8 < 0x10; local_8 = local_8 + 1) {
    *local_4 = uVar1;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar1 = DAT_8cedbc4a, local_8 < 0x10; local_8 = local_8 + 1) {
    *local_4 = uVar2;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar2 = DAT_8cedbc4c, local_8 < 0x10; local_8 = local_8 + 1) {
    *local_4 = uVar1;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar1 = DAT_8cedbc4e, local_8 < 0x10; local_8 = local_8 + 1) {
    *local_4 = uVar2;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar2 = DAT_8cedbc50, local_8 < 0x10; local_8 = local_8 + 1) {
    *local_4 = uVar1;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar1 = DAT_8cedbc52, local_8 < 0x10; local_8 = local_8 + 1) {
    *local_4 = uVar2;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar2 = DAT_8cedbc54, local_8 < 0x10; local_8 = local_8 + 1) {
    *local_4 = uVar1;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar1 = DAT_8cedbc56, local_8 < 0x10; local_8 = local_8 + 1) {
    *local_4 = uVar2;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar2 = DAT_8cedbd46, local_8 < 0x10; local_8 = local_8 + 1) {
    *local_4 = uVar1;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar1 = DAT_8cedbd48, local_8 < 0x10; local_8 = local_8 + 1) {
    *local_4 = uVar2;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar2 = DAT_8cedbd4a, local_8 < 0x10; local_8 = local_8 + 1) {
    *local_4 = uVar1;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar1 = DAT_8cedbd4c, local_8 < 0x10; local_8 = local_8 + 1) {
    *local_4 = uVar2;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar2 = DAT_8cedbd4e, local_8 < 0x20; local_8 = local_8 + 1) {
    *local_4 = uVar1;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar1 = DAT_8cedbd50, local_8 < 0x20; local_8 = local_8 + 1) {
    *local_4 = uVar2;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar2 = DAT_8cedbd52, local_8 < 0x20; local_8 = local_8 + 1) {
    *local_4 = uVar1;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; uVar1 = DAT_8cedbe50, local_8 < 0x20; local_8 = local_8 + 1) {
    *local_4 = uVar2;
    local_4 = local_4 + 1;
  }
  for (local_8 = 0; local_8 < 0x40; local_8 = local_8 + 1) {
    *local_4 = uVar1;
    local_4 = local_4 + 1;
  }
  return;
}


/* Address: 8cedbd92 */

void FUN_8cedbd92(void)

{
  FUN_8cedbd9c();
  FUN_8cedbe14();
  return;
}


/* Address: 8cedbd9c */

void FUN_8cedbd9c(void)

{
  undefined1 *puVar1;
  int iVar2;
  
  iVar2 = 0x20;
  puVar1 = DAT_8cedbe54;
  do {
    iVar2 = iVar2 + -1;
    *puVar1 = 0x12;
    puVar1 = puVar1 + 1;
  } while (iVar2 != 0);
  iVar2 = 0x20;
  do {
    iVar2 = iVar2 + -1;
    *puVar1 = 0x22;
    puVar1 = puVar1 + 1;
  } while (iVar2 != 0);
  iVar2 = 0x10;
  do {
    iVar2 = iVar2 + -1;
    *puVar1 = 3;
    puVar1 = puVar1 + 1;
  } while (iVar2 != 0);
  iVar2 = 0x10;
  do {
    iVar2 = iVar2 + -1;
    *puVar1 = 0x33;
    puVar1 = puVar1 + 1;
  } while (iVar2 != 0);
  iVar2 = 0x10;
  do {
    iVar2 = iVar2 + -1;
    *puVar1 = 0x43;
    puVar1 = puVar1 + 1;
  } while (iVar2 != 0);
  iVar2 = 8;
  do {
    iVar2 = iVar2 + -1;
    *puVar1 = 0x54;
    puVar1 = puVar1 + 1;
  } while (iVar2 != 0);
  iVar2 = 4;
  do {
    iVar2 = iVar2 + -1;
    *puVar1 = 0x65;
    puVar1 = puVar1 + 1;
  } while (iVar2 != 0);
  iVar2 = 2;
  do {
    iVar2 = iVar2 + -1;
    *puVar1 = 0x76;
    puVar1 = puVar1 + 1;
  } while (iVar2 != 0);
  iVar2 = 2;
  do {
    iVar2 = iVar2 + -1;
    *puVar1 = 0x87;
    puVar1 = puVar1 + 1;
  } while (iVar2 != 0);
  return;
}


/* Address: 8cedbe14 */

void FUN_8cedbe14(void)

{
  undefined1 *puVar1;
  int iVar2;
  
  puVar1 = DAT_8cedbe58;
  for (iVar2 = 0x20; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar1 = 2;
    puVar1 = puVar1 + 1;
  }
  for (iVar2 = 0x20; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar1 = 0x12;
    puVar1 = puVar1 + 1;
  }
  for (iVar2 = 0x20; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar1 = 0x22;
    puVar1 = puVar1 + 1;
  }
  for (iVar2 = 0x10; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar1 = 0x33;
    puVar1 = puVar1 + 1;
  }
  for (iVar2 = 8; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar1 = 0x44;
    puVar1 = puVar1 + 1;
  }
  for (iVar2 = 4; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar1 = 0x55;
    puVar1 = puVar1 + 1;
  }
  for (iVar2 = 2; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar1 = 0x66;
    puVar1 = puVar1 + 1;
  }
  *puVar1 = 0x77;
  puVar1[1] = 0x88;
  return;
}


/* Address: 8cedbea2 */

void FUN_8cedbea2(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  int iVar4;
  
  puVar3 = DAT_8cedbfe4;
  for (iVar4 = 4; uVar1 = DAT_8cedbfe8, iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar3 = 0;
    puVar3 = puVar3 + 1;
  }
  for (iVar4 = 4; uVar2 = DAT_8cedbfec, iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar3 = uVar1;
    puVar3 = puVar3 + 1;
  }
  for (iVar4 = 2; uVar1 = DAT_8cedbff0, iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar3 = uVar2;
    puVar3 = puVar3 + 1;
  }
  for (iVar4 = 2; uVar2 = DAT_8cedbff4, iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar3 = uVar1;
    puVar3 = puVar3 + 1;
  }
  for (iVar4 = 2; uVar1 = DAT_8cedbff8, iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar3 = uVar2;
    puVar3 = puVar3 + 1;
  }
  for (iVar4 = 2; uVar2 = DAT_8cedbffc, iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar3 = uVar1;
    puVar3 = puVar3 + 1;
  }
  for (iVar4 = 4; uVar1 = DAT_8cedc000, iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar3 = uVar2;
    puVar3 = puVar3 + 1;
  }
  for (iVar4 = 4; uVar2 = DAT_8cedc004, iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar3 = uVar1;
    puVar3 = puVar3 + 1;
  }
  for (iVar4 = 4; uVar1 = DAT_8cedc008, iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar3 = uVar2;
    puVar3 = puVar3 + 1;
  }
  for (iVar4 = 4; uVar2 = DAT_8cedc028, iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar3 = uVar1;
    puVar3 = puVar3 + 1;
  }
  *puVar3 = DAT_8cedc00c;
  puVar3[1] = DAT_8cedc010;
  iVar4 = DAT_8cedc014;
  puVar3[2] = DAT_8cedc014;
  puVar3[3] = iVar4 + -1;
  puVar3[4] = DAT_8cedc018;
  puVar3[5] = DAT_8cedc01c;
  puVar3[6] = DAT_8cedc020;
  puVar3 = puVar3 + 7;
  *puVar3 = DAT_8cedc024;
  for (iVar4 = 8; uVar1 = DAT_8cedc02c, puVar3 = puVar3 + 1, iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar3 = uVar2;
  }
  for (iVar4 = 8; uVar2 = DAT_8cedc030, iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar3 = uVar1;
    puVar3 = puVar3 + 1;
  }
  for (iVar4 = 8; uVar1 = DAT_8cedc034, iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar3 = uVar2;
    puVar3 = puVar3 + 1;
  }
  for (iVar4 = 0x10; uVar2 = DAT_8cedc038, iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar3 = uVar1;
    puVar3 = puVar3 + 1;
  }
  for (iVar4 = 0x10; uVar1 = DAT_8cedc03c, iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar3 = uVar2;
    puVar3 = puVar3 + 1;
  }
  for (iVar4 = 0x20; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar3 = uVar1;
    puVar3 = puVar3 + 1;
  }
  return;
}


/* Address: 8cedc040 */

void FUN_8cedc040(void)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  undefined *puVar5;
  undefined *puVar6;
  
  puVar2 = DAT_8cedc0e4;
  uVar1 = DAT_8cedc0e0;
  *DAT_8cedc0dc = DAT_8cedc0d8;
  puVar4 = DAT_8cedc0ec;
  uVar3 = DAT_8cedc0e8;
  *puVar2 = uVar1;
  puVar2 = DAT_8cedc0f4;
  uVar1 = DAT_8cedc0f0;
  *puVar4 = uVar3;
  puVar4 = DAT_8cedc0fc;
  uVar3 = DAT_8cedc0f8;
  *puVar2 = uVar1;
  puVar2 = DAT_8cedc104;
  uVar1 = DAT_8cedc100;
  *puVar4 = uVar3;
  puVar4 = DAT_8cedc10c;
  uVar3 = DAT_8cedc108;
  *puVar2 = uVar1;
  puVar2 = DAT_8cedc114;
  uVar1 = DAT_8cedc110;
  *puVar4 = uVar3;
  puVar4 = DAT_8cedc11c;
  uVar3 = DAT_8cedc118;
  *puVar2 = uVar1;
  puVar2 = DAT_8cedc124;
  uVar1 = DAT_8cedc120;
  *puVar4 = uVar3;
  puVar4 = DAT_8cedc12c;
  uVar3 = DAT_8cedc128;
  *puVar2 = uVar1;
  puVar2 = DAT_8cedc134;
  uVar1 = DAT_8cedc130;
  *puVar4 = uVar3;
  puVar4 = DAT_8cedc13c;
  uVar3 = DAT_8cedc138;
  *puVar2 = uVar1;
  puVar2 = DAT_8cedc144;
  puVar5 = PTR_DAT_8cedc140;
  *puVar4 = uVar3;
  puVar4 = DAT_8cedc14c;
  puVar6 = PTR_DAT_8cedc148;
  *puVar2 = puVar5;
  puVar2 = DAT_8cedc154;
  puVar5 = PTR_DAT_8cedc150;
  *puVar4 = puVar6;
  puVar4 = DAT_8cedc15c;
  puVar6 = PTR_DAT_8cedc158;
  *puVar2 = puVar5;
  puVar2 = DAT_8cedc164;
  puVar5 = PTR_DAT_8cedc160;
  *puVar4 = puVar6;
  puVar4 = DAT_8cedc16c;
  puVar6 = PTR_DAT_8cedc168;
  *puVar2 = puVar5;
  puVar2 = DAT_8cedc174;
  uVar1 = DAT_8cedc170;
  *puVar4 = puVar6;
  *puVar2 = uVar1;
  return;
}


/* Address: 8cedc0ba */

void callback_candidate_8cedc0ba(int param_1)

{
  undefined4 uVar1;
  
  uVar1 = FUN_8cedc178(param_1 + DAT_8cedc0d4);
  uVar1 = FUN_8cedc218(uVar1);
  uVar1 = FUN_8cedc256(uVar1);
  FUN_8cedc2f4(uVar1);
  return;
}


/* Address: 8cedc178 */

int FUN_8cedc178(int param_1)

{
  int *piVar1;
  undefined *puVar2;
  undefined4 uVar3;
  undefined *puVar4;
  int iVar5;
  
  uVar3 = DAT_8cedc2a0;
  puVar2 = PTR_FUN_8cedc29c;
  piVar1 = DAT_8cedc298;
  param_1 = param_1 - DAT_8cedc292;
  iVar5 = (int)DAT_8cedc294;
  *DAT_8cedc298 = param_1;
  (*(code *)puVar2)(*piVar1,uVar3,iVar5);
  puVar4 = PTR_DAT_8cedc2a8;
  piVar1 = DAT_8cedc2a4;
  *DAT_8cedc2a4 = param_1 + -0x10;
  (*(code *)puVar2)(*piVar1,puVar4,4);
  puVar4 = PTR_DAT_8cedc2b0;
  piVar1 = DAT_8cedc2ac;
  *DAT_8cedc2ac = param_1 + -0x30;
  (*(code *)puVar2)(*piVar1,puVar4,8);
  puVar4 = PTR_DAT_8cedc2b8;
  piVar1 = DAT_8cedc2b4;
  *DAT_8cedc2b4 = param_1 + -0x50;
  (*(code *)puVar2)(*piVar1,puVar4,8);
  puVar4 = PTR_DAT_8cedc2c0;
  piVar1 = DAT_8cedc2bc;
  *DAT_8cedc2bc = param_1 + -0x70;
  (*(code *)puVar2)(*piVar1,puVar4,8);
  puVar4 = PTR_DAT_8cedc2c8;
  piVar1 = DAT_8cedc2c4;
  *DAT_8cedc2c4 = param_1 + -0x90;
  (*(code *)puVar2)(*piVar1,puVar4,8);
  puVar4 = PTR_DAT_8cedc2d0;
  piVar1 = DAT_8cedc2cc;
  *DAT_8cedc2cc = param_1 + -0xb0;
  (*(code *)puVar2)(*piVar1,puVar4,8);
  return param_1 + -0xb0;
}


/* Address: 8cedc218 */

int FUN_8cedc218(int param_1)

{
  int *piVar1;
  undefined4 uVar2;
  int iVar3;
  
  uVar2 = DAT_8cedc2d8;
  piVar1 = DAT_8cedc2d4;
  iVar3 = (int)DAT_8cedc294;
  *DAT_8cedc2d4 = param_1 - iVar3;
  (*(code *)PTR_FUN_8cedc29c)(*piVar1,uVar2,0x20);
  uVar2 = DAT_8cedc2e0;
  piVar1 = DAT_8cedc2dc;
  iVar3 = (param_1 - iVar3) - (int)DAT_8cedc294;
  *DAT_8cedc2dc = iVar3;
  (*(code *)PTR_FUN_8cedc29c)(*piVar1,uVar2,0x20);
  return iVar3;
}


/* Address: 8cedc256 */

int FUN_8cedc256(int param_1)

{
  int *piVar1;
  undefined4 uVar2;
  int iVar3;
  
  uVar2 = DAT_8cedc2e8;
  piVar1 = DAT_8cedc2e4;
  iVar3 = (int)DAT_8cedc296;
  *DAT_8cedc2e4 = param_1 - iVar3;
  (*(code *)PTR_FUN_8cedc29c)(*piVar1,uVar2,0x40);
  uVar2 = DAT_8cedc2f0;
  piVar1 = DAT_8cedc2ec;
  iVar3 = (param_1 - iVar3) + -0x40;
  *DAT_8cedc2ec = iVar3;
  (*(code *)PTR_FUN_8cedc29c)(*piVar1,uVar2,0x10);
  return iVar3;
}


/* Address: 8cedc2f4 */

int FUN_8cedc2f4(int param_1)

{
  int *piVar1;
  undefined4 uVar2;
  int iVar3;
  
  uVar2 = DAT_8cedc338;
  piVar1 = DAT_8cedc334;
  *DAT_8cedc334 = param_1 + -0x40;
  (*(code *)PTR_FUN_8cedc33c)(*piVar1,uVar2,0x10);
  uVar2 = DAT_8cedc344;
  piVar1 = DAT_8cedc340;
  iVar3 = (param_1 + -0x40) - (int)DAT_8cedc330;
  *DAT_8cedc340 = iVar3;
  (*(code *)PTR_FUN_8cedc33c)(*piVar1,uVar2,0x20);
  return iVar3;
}


/* Address: 8cedc348 */

bool FUN_8cedc348(int param_1,int param_2)

{
  uint uVar1;
  
  uVar1 = param_2 - param_1;
  if ((int)uVar1 < 0) {
    uVar1 = -uVar1;
  }
  return (uVar1 & 0xfffffff4) == 0;
}


/* Address: 8cedc35c */

undefined4 callback_candidate_8cedc35c(void)

{
  char extraout_T;
  char extraout_T_00;
  char extraout_T_01;
  char extraout_T_02;
  
  FUN_8cedc348(DAT_8cedc3e0,DAT_8cedc3f0);
  if ((((extraout_T != '\0') && (FUN_8cedc348(DAT_8cedc3e4,DAT_8cedc3f4), extraout_T_00 != '\0')) &&
      (FUN_8cedc348(DAT_8cedc3e8,DAT_8cedc3f8), extraout_T_01 != '\0')) &&
     (FUN_8cedc348(DAT_8cedc3ec,DAT_8cedc3fc), extraout_T_02 != '\0')) {
    return 0;
  }
  return 0xffffffff;
}


/* Address: 8cedc394 */

undefined4 callback_candidate_8cedc394(void)

{
  char extraout_T;
  char extraout_T_00;
  char extraout_T_01;
  
  FUN_8cedc348(PTR_DAT_8cedc3d0,PTR_DAT_8cedc3c4);
  if (((extraout_T != '\0') &&
      (FUN_8cedc348(PTR_DAT_8cedc3d4,PTR_DAT_8cedc3c8), extraout_T_00 != '\0')) &&
     (FUN_8cedc348(PTR_s_SLH448_1_SFD_8cedc3d8,PTR_s_SLH448_1_SFD_8cedc3cc), extraout_T_01 != '\0'))
  {
    return 0;
  }
  return 0xffffffff;
}


/* Address: 8cedc40c */

void FUN_8cedc40c(int param_1,int param_2,undefined4 *param_3)

{
  param_3[1] = (int)((longlong)param_1 * (longlong)param_2);
  *param_3 = (int)((ulonglong)((longlong)param_1 * (longlong)param_2) >> 0x20);
  return;
}


/* Address: 8cedc418 */

void FUN_8cedc418(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                 int param_5)

{
  uint uVar1;
  undefined4 in_fr1;
  undefined4 in_fr3;
  undefined4 in_fr13;
  undefined4 in_fr12;
  undefined4 in_fr15;
  undefined4 in_fr14;
  undefined8 in_xd0;
  undefined8 in_xd2;
  undefined8 in_xd4;
  undefined8 in_xd6;
  undefined8 in_xd8;
  undefined8 in_xd10;
  undefined8 in_xd12;
  undefined8 in_xd14;
  char in_FPSCR_SZ;
  
  uVar1 = param_5 + 0x57U & 0xfffffff8;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(uVar1 - 4) = in_fr12;
    *(undefined4 *)(uVar1 - 8) = in_fr14;
    *(undefined4 *)(uVar1 - 0xc) = in_fr1;
    *(undefined4 *)(uVar1 - 0x10) = in_fr3;
    *(undefined4 *)(uVar1 - 0x14) = param_1;
    *(undefined4 *)(uVar1 - 0x18) = param_2;
    *(undefined4 *)(uVar1 - 0x1c) = param_3;
    *(undefined4 *)(uVar1 - 0x20) = param_4;
    *(undefined4 *)(uVar1 - 0x24) = in_fr13;
    *(undefined4 *)(uVar1 - 0x28) = in_fr15;
  }
  else {
    *(ulonglong *)(uVar1 - 8) = CONCAT44(in_fr12,in_fr13);
    *(ulonglong *)(uVar1 - 0x10) = CONCAT44(in_fr14,in_fr15);
    *(undefined8 *)(uVar1 - 0x18) = in_xd0;
    *(undefined8 *)(uVar1 - 0x20) = in_xd2;
    *(undefined8 *)(uVar1 - 0x28) = in_xd4;
    *(undefined8 *)(uVar1 - 0x30) = in_xd6;
    *(undefined8 *)(uVar1 - 0x38) = in_xd8;
    *(undefined8 *)(uVar1 - 0x40) = in_xd10;
    *(undefined8 *)(uVar1 - 0x48) = in_xd12;
    *(undefined8 *)(uVar1 - 0x50) = in_xd14;
  }
  return;
}


/* Address: 8cedc438 */

void FUN_8cedc438(void)

{
  return;
}


/* Address: 8cedc458 */

void FUN_8cedc458(undefined4 *param_1)

{
  undefined4 *puVar1;
  
  puVar1 = DAT_8cedc46c;
  *param_1 = *DAT_8cedc46c;
  param_1[1] = puVar1[1];
  param_1[2] = puVar1[2];
  param_1[3] = puVar1[3];
  return;
}


