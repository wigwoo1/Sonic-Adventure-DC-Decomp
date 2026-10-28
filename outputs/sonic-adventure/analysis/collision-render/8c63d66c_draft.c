/* Ghidra draft only; not buildable/verified source. */

void FUN_8c63d66c(undefined4 param_1,int *param_2,undefined4 param_3,uint param_4)

{
  undefined *puVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  int iVar9;
  undefined8 uVar10;
  char in_FPSCR_SZ;
  int local_20;
  int local_c;
  
  local_c = 0xa0;
  iVar2 = (int)DAT_8c63d762;
  *(undefined4 *)(&stack0xfffffffc + iVar2) = param_3;
  if (in_FPSCR_SZ == '\0') {
    uVar10 = CONCAT44(*(undefined4 *)DAT_8c63d774,param_1);
  }
  else {
    uVar10 = *DAT_8c63d774;
  }
  *(int *)((int)&local_20 + DAT_8c63d766 + iVar2) = (int)&local_20 + DAT_8c63d764 + iVar2;
  *(undefined1 **)((int)&local_20 + DAT_8c63d768 + iVar2) = &stack0x0000002c + iVar2;
  *(undefined1 **)((int)&local_20 + DAT_8c63d76a + iVar2) = &stack0x00000020 + iVar2;
  *(undefined1 **)((int)&local_20 + DAT_8c63d76c + iVar2) = &stack0x00000038 + iVar2;
  *(undefined1 **)((int)&local_20 + DAT_8c63d76e + iVar2) = &stack0x00000010 + iVar2;
  *(undefined1 **)((int)&local_20 + DAT_8c63d770 + iVar2) = &stack0x00000000 + iVar2;
  (*(code *)PTR_FUN_8c63d778)(uVar10);
  uVar8 = DAT_8c63d780 & param_4;
  if (uVar8 == 0) {
LAB_8c63d6f2:
    if ((param_4 & 0x40) == 0) {
      (*(code *)PTR_FUN_8c63d77c)(DAT_8c63d79c);
      uVar3 = (*(code *)PTR_FUN_8c63d7a0)();
    }
    else {
      (*(code *)PTR_FUN_8c63d77c)(DAT_8c63d794);
      uVar3 = (*(code *)PTR_FUN_8c63d798)();
    }
    *(undefined4 *)((int)&local_20 + iVar2) = uVar3;
  }
  else {
    if ((param_4 & 0x40) == 0) {
      if (uVar8 == 0) goto LAB_8c63d6f2;
      (*(code *)PTR_FUN_8c63d77c)(DAT_8c63d78c);
      uVar3 = (*(code *)PTR_FUN_8c63d790)();
    }
    else {
      (*(code *)PTR_FUN_8c63d77c)(DAT_8c63d784);
      uVar3 = (*(code *)PTR_FUN_8c63d788)();
    }
    *(undefined4 *)((int)&local_20 + iVar2) = uVar3;
  }
  puVar1 = PTR_FUN_8c63d7a4;
  *(uint *)(&stack0xffffffe8 + iVar2) = param_4 & 1;
  if ((param_4 & 2) == 0) {
    if ((param_4 & 1) == 0) {
      iVar9 = 3;
    }
    else {
      iVar9 = 2;
    }
  }
  else {
    iVar9 = 1;
  }
  *(undefined4 *)(&stack0xffffffe4 + iVar2) = 0;
  *(undefined4 *)(&stack0xfffffff0 + iVar2) = 1;
  *(undefined4 *)((int)&local_c + iVar2) = 1;
  *(undefined4 *)(&stack0xfffffff8 + iVar2) = 1;
  *(uint *)(&stack0xffffffec + iVar2) = param_4 & 0x20;
  if (0 < *(int *)(&stack0xfffffffc + iVar2)) {
    do {
      if (*(int *)(&stack0xffffffe8 + iVar2) == 0) {
        iVar7 = *(int *)(&stack0xffffffe4 + iVar2) * iVar9;
      }
      else {
        iVar7 = 0;
      }
      (*(code *)puVar1)(0,iVar7 * 0xc + *param_2,
                        *(undefined4 *)((int)&local_20 + DAT_8c63d8fa + iVar2));
      iVar6 = *(int *)(&stack0xfffffff8 + iVar2);
      (*(code *)puVar1)(0,iVar6 * 0xc + *param_2,
                        *(int *)((int)&local_20 + DAT_8c63d8fa + iVar2) + 0xc);
      (*(code *)puVar1)(0,(iVar6 + 1) * 0xc + *param_2,
                        *(int *)((int)&local_20 + DAT_8c63d8fa + iVar2) + 0x18);
      iVar6 = (*(code *)PTR_FUN_8c63d904)((int)&local_20 + DAT_8c63d8fa + iVar2,3);
      if (iVar6 != 0) {
        if (uVar8 != 0) {
          iVar6 = (int)DAT_8c63d8fc;
          **(undefined2 **)((int)&local_20 + DAT_8c63d8fc + iVar2) =
               *(undefined2 *)(param_2[2] + iVar7 * 4);
          *(undefined2 *)(*(int *)((int)&local_20 + iVar6 + iVar2) + 2) =
               *(undefined2 *)(param_2[2] + iVar7 * 4 + 2);
          iVar4 = *(int *)((int)&local_c + iVar2);
          iVar5 = iVar4 * 4;
          iVar6 = (int)DAT_8c63d8fc;
          *(undefined2 *)(*(int *)((int)&local_20 + DAT_8c63d8fc + iVar2) + 4) =
               *(undefined2 *)(param_2[2] + iVar5);
          *(undefined2 *)(*(int *)((int)&local_20 + iVar6 + iVar2) + 6) =
               *(undefined2 *)(param_2[2] + iVar5 + 2);
          iVar4 = (iVar4 + 1) * 4;
          iVar6 = (int)DAT_8c63d8fc;
          *(undefined2 *)(*(int *)((int)&local_20 + DAT_8c63d8fc + iVar2) + 8) =
               *(undefined2 *)(param_2[2] + iVar4);
          *(undefined2 *)(*(int *)((int)&local_20 + iVar6 + iVar2) + 10) =
               *(undefined2 *)(param_2[2] + iVar4 + 2);
        }
        iVar4 = (int)DAT_8c63d900;
        **(undefined4 **)((int)&local_20 + DAT_8c63d8fe + iVar2) =
             *(undefined4 *)(iVar7 * 4 + param_2[1]);
        iVar7 = *(int *)(&stack0xfffffff0 + iVar2);
        *(undefined4 *)(*(int *)((int)&local_20 + DAT_8c63d8fe + iVar2) + 4) =
             *(undefined4 *)(iVar7 * 4 + param_2[1]);
        iVar6 = (int)DAT_8c63d8fa;
        *(undefined4 *)(*(int *)((int)&local_20 + DAT_8c63d8fe + iVar2) + 8) =
             *(undefined4 *)((iVar7 + 1) * 4 + param_2[1]);
        iVar7 = (*(code *)PTR_FUN_8c63d908)
                          ((int)&local_20 + iVar6 + iVar2,3,(int)&local_20 + iVar4 + iVar2,param_4);
        if (2 < iVar7) {
          if ((*(int *)(&stack0xffffffec + iVar2) == 0) || (uVar8 == 0)) {
            if (*(int *)(&stack0xffffffec + iVar2) == 0) {
              if (uVar8 == 0) {
                (*(code *)PTR_FUN_8c63d96c)
                          ((int)&local_20 + DAT_8c63d968 + iVar2,(int)&local_20 + iVar2);
              }
              else {
                (*(code *)PTR_FUN_8c63d914)
                          ((int)&local_20 + DAT_8c63d900 + iVar2,(int)&local_20 + iVar2);
              }
            }
            else {
              (*(code *)PTR_FUN_8c63d910)
                        ((int)&local_20 + DAT_8c63d900 + iVar2,(int)&local_20 + iVar2);
            }
          }
          else {
            (*(code *)PTR_FUN_8c63d90c)
                      ((int)&local_20 + DAT_8c63d900 + iVar2,(int)&local_20 + iVar2);
          }
        }
      }
      *(int *)(&stack0xffffffe4 + iVar2) = *(int *)(&stack0xffffffe4 + iVar2) + 1;
      *(int *)(&stack0xfffffff0 + iVar2) = *(int *)(&stack0xfffffff0 + iVar2) + iVar9;
      *(int *)((int)&local_c + iVar2) = *(int *)((int)&local_c + iVar2) + iVar9;
      *(int *)(&stack0xfffffff8 + iVar2) = *(int *)(&stack0xfffffff8 + iVar2) + iVar9;
    } while (*(int *)(&stack0xffffffe4 + iVar2) < *(int *)(&stack0xfffffffc + iVar2));
  }
  (*(code *)PTR_FUN_8c63d970)(*(undefined4 *)((int)&local_20 + iVar2));
  (*(code *)PTR_FUN_8c63d974)();
  return;
}

