/* Ghidra draft only; not buildable/verified source. */

void FUN_8c63d414(int *param_1,int param_2,uint param_3)

{
  undefined *puVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  int *piVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  uint uVar10;
  int local_20;
  
  puVar1 = PTR_FUN_8c63d510;
  iVar2 = (int)DAT_8c63d4f0;
  *(int *)((int)&local_20 + DAT_8c63d4f4 + iVar2) = (int)&local_20 + DAT_8c63d4f2 + iVar2;
  *(int *)((int)&local_20 + DAT_8c63d4f8 + iVar2) = (int)&local_20 + DAT_8c63d4f6 + iVar2;
  *(int *)((int)&local_20 + DAT_8c63d4fc + iVar2) = (int)&local_20 + DAT_8c63d4fa + iVar2;
  *(int *)((int)&local_20 + DAT_8c63d500 + iVar2) = (int)&local_20 + DAT_8c63d4fe + iVar2;
  *(int *)((int)&local_20 + DAT_8c63d504 + iVar2) = (int)&local_20 + DAT_8c63d502 + iVar2;
  piVar5 = (int *)((int)&local_20 + DAT_8c63d50a + iVar2);
  *(int *)((int)&local_20 + DAT_8c63d508 + iVar2) = (int)&local_20 + DAT_8c63d506 + iVar2;
  *piVar5 = (int)&local_20 + DAT_8c63d50c + iVar2;
  iVar8 = 0;
  piVar5[1] = (int)&local_20 + DAT_8c63d50e + iVar2;
  piVar5[2] = (int)(&stack0xffffffe4 + iVar2);
  iVar9 = 0;
  if (0 < param_2) {
    do {
      (*(code *)puVar1)(0,*param_1 + iVar9,*(int *)((int)&local_20 + DAT_8c63d4f4 + iVar2) + iVar9);
      iVar8 = iVar8 + 1;
      iVar9 = iVar9 + 0xc;
    } while (iVar8 < param_2);
  }
  iVar8 = (*(code *)PTR_FUN_8c63d514)((int)&local_20 + DAT_8c63d4f4 + iVar2,param_2);
  if (iVar8 == 0) {
    return;
  }
  iVar8 = 0;
  iVar7 = 0;
  iVar6 = 0;
  iVar9 = 0;
  uVar10 = DAT_8c63d518 & param_3;
  if (0 < param_2) {
    do {
      if (uVar10 == 0) {
        *(undefined4 *)(*(int *)((int)&local_20 + DAT_8c63d5ec + iVar2) + iVar6) =
             *(undefined4 *)(param_1[1] + iVar6);
      }
      else {
        iVar3 = (int)DAT_8c63d4fc;
        *(undefined2 *)(*(int *)((int)&local_20 + DAT_8c63d4fc + iVar2) + iVar8) =
             *(undefined2 *)(param_1[2] + iVar8);
        *(undefined2 *)(*(int *)((int)&local_20 + iVar3 + iVar2) + iVar8 + 2) =
             *(undefined2 *)(param_1[2] + iVar8 + 2);
        if (param_1[1] != 0) {
          *(undefined4 *)(*(int *)((int)&local_20 + DAT_8c63d4f8 + iVar2) + iVar9) =
               *(undefined4 *)(param_1[1] + iVar9);
        }
      }
      iVar7 = iVar7 + 1;
      iVar9 = iVar9 + 4;
      iVar6 = iVar6 + 4;
      iVar8 = iVar8 + 4;
    } while (iVar7 < param_2);
  }
  iVar8 = (*(code *)PTR_FUN_8c63d5f4)
                    ((int)&local_20 + DAT_8c63d5f0 + iVar2,param_2,
                     (int)&local_20 + DAT_8c63d5ee + iVar2,param_3);
  if (iVar8 < 3) {
    return;
  }
  (*(code *)PTR_FUN_8c63d5f8)();
  if (uVar10 == 0) {
LAB_8c63d584:
    if ((param_3 & 0x40) == 0) {
      (*(code *)PTR_FUN_8c63d5fc)(DAT_8c63d618);
      uVar4 = (*(code *)PTR_FUN_8c63d61c)();
    }
    else {
      (*(code *)PTR_FUN_8c63d5fc)(DAT_8c63d610);
      uVar4 = (*(code *)PTR_FUN_8c63d614)();
    }
    *(undefined4 *)((int)&local_20 + iVar2) = uVar4;
  }
  else {
    if ((param_3 & 0x40) == 0) {
      if (uVar10 == 0) goto LAB_8c63d584;
      (*(code *)PTR_FUN_8c63d5fc)(DAT_8c63d608);
      uVar4 = (*(code *)PTR_FUN_8c63d60c)();
    }
    else {
      (*(code *)PTR_FUN_8c63d5fc)(DAT_8c63d600);
      uVar4 = (*(code *)PTR_FUN_8c63d604)();
    }
    *(undefined4 *)((int)&local_20 + iVar2) = uVar4;
  }
  if ((param_3 & 0x20) != 0) {
    if (uVar10 != 0) {
      (*(code *)PTR_FUN_8c63d620)
                (iVar8,(int)&local_20 + DAT_8c63d5ee + iVar2,(int)&local_20 + iVar2);
      goto LAB_8c63d638;
    }
    if ((param_3 & 0x20) != 0) {
      (*(code *)PTR_FUN_8c63d624)
                (iVar8,(int)&local_20 + DAT_8c63d5ee + iVar2,(int)&local_20 + iVar2);
      goto LAB_8c63d638;
    }
  }
  if (uVar10 == 0) {
    (*(code *)PTR_FUN_8c63d660)(iVar8,(int)&local_20 + DAT_8c63d65a + iVar2,(int)&local_20 + iVar2);
  }
  else {
    (*(code *)PTR_FUN_8c63d628)(iVar8,(int)&local_20 + DAT_8c63d5ee + iVar2,(int)&local_20 + iVar2);
  }
LAB_8c63d638:
  (*(code *)PTR_FUN_8c63d664)(*(undefined4 *)((int)&local_20 + iVar2));
  (*(code *)PTR_FUN_8c63d668)();
  return;
}

