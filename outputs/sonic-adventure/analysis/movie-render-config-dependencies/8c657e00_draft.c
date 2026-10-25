/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c658106) */
/* WARNING: Removing unreachable block (ram,0x8c657f2c) */
/* WARNING: Removing unreachable block (ram,0x8c658222) */

undefined4 FUN_8c657e00(int param_1,int param_2,int param_3,int param_4)

{
  uint *puVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int local_20;
  undefined4 local_c;
  
  local_c = 0x1e0;
  iVar2 = (int)DAT_8c657ea0;
  uVar4 = *(undefined4 *)(PTR_DAT_8c657ea8 + 4);
  *(undefined4 *)(&stack0xffffffe8 + iVar2) = *(undefined4 *)PTR_DAT_8c657ea8;
  *(undefined4 *)(&stack0xffffffec + iVar2) = uVar4;
  uVar4 = *(undefined4 *)(PTR_DAT_8c657ea8 + 0xc);
  *(undefined4 *)(&stack0xfffffff0 + iVar2) = *(undefined4 *)(PTR_DAT_8c657ea8 + 8);
  *(undefined4 *)((int)&local_c + iVar2) = uVar4;
  uVar4 = *(undefined4 *)(PTR_DAT_8c657ea8 + 0x14);
  *(undefined4 *)(&stack0xfffffff8 + iVar2) = *(undefined4 *)(PTR_DAT_8c657ea8 + 0x10);
  *(undefined4 *)(&stack0xfffffffc + iVar2) = uVar4;
  iVar7 = *(int *)(&stack0xffffffe8 + param_3 * 4 + iVar2);
  if ((*(int *)((int)&local_20 + DAT_8c657ea2 + iVar2) != 0) && (param_2 == DAT_8c657ea4)) {
    param_2 = param_2 << 1;
  }
  *DAT_8c657eac = param_1;
  *DAT_8c657eb0 = param_2;
  *(undefined4 *)(&stack0x00000020 + iVar2) = 1;
  *(undefined4 *)(&stack0x00000038 + iVar2) = 0;
  *(undefined4 *)(&stack0x00000034 + iVar2) = 0;
  *(undefined4 *)(&stack0x00000040 + iVar2) = 0;
  *(undefined4 *)(&stack0x00000070 + iVar2) = 0;
  *(undefined4 *)(&stack0x00000068 + iVar2) = 0;
  *(undefined4 *)(&stack0x00000064 + iVar2) = 0;
  if ((*DAT_8c657eb4 & 1) == 0) {
    *(undefined4 *)(&stack0x0000006c + iVar2) = 0;
    if (param_4 == 1) {
      *(undefined4 *)(&stack0xffffffe4 + iVar2) = 2;
      goto LAB_8c657e92;
    }
    iVar6 = 1;
    *(undefined4 *)(&stack0xffffffe4 + iVar2) = 1;
  }
  else {
    *(undefined4 *)(&stack0x0000006c + iVar2) = 1;
    *(undefined4 *)(&stack0xffffffe4 + iVar2) = 1;
LAB_8c657e92:
    iVar6 = 0;
  }
  if (iVar7 != 0x10) {
    if (iVar7 == 0x18) {
      *(undefined4 *)(&stack0x00000030 + iVar2) = 4;
      *(undefined4 *)(&stack0x0000003c + iVar2) = 2;
    }
    else {
      if (iVar7 != 0x20) goto LAB_8c657f18;
      *(undefined4 *)(&stack0x00000030 + iVar2) = 6;
      *(undefined4 *)(&stack0x0000003c + iVar2) = 3;
    }
    *(undefined4 *)(&stack0x0000002c + iVar2) = 0;
    goto LAB_8c657f18;
  }
  if (param_3 == 3) {
    *(undefined4 *)(&stack0x00000030 + iVar2) = 3;
LAB_8c657ef4:
    *(undefined4 *)(&stack0x0000003c + iVar2) = 0;
  }
  else if (param_3 == 0) {
    *(undefined4 *)(&stack0x00000030 + iVar2) = 1;
    *(undefined4 *)(&stack0x0000003c + iVar2) = 1;
  }
  else {
    if (param_3 == 1) {
      *(undefined4 *)(&stack0x00000030 + iVar2) = 0;
      goto LAB_8c657ef4;
    }
    *(undefined4 *)(&stack0x00000030 + iVar2) = 1;
    *(undefined4 *)(&stack0x0000003c + iVar2) = 1;
  }
  *(undefined4 *)(&stack0x0000002c + iVar2) = *(undefined4 *)((int)&local_20 + DAT_8c657fa0 + iVar2)
  ;
LAB_8c657f18:
  iVar8 = param_1 * iVar7;
  *(undefined4 *)(&stack0x00000028 + iVar2) = 0;
  *(int *)(&stack0x00000024 + iVar2) = param_4;
  *(undefined4 *)(&stack0x0000001c + iVar2) = 0;
  *(int *)((int)&local_20 + iVar2) = iVar8;
  if (iVar8 < 0) {
    iVar8 = iVar8 + 0x3f;
  }
  *(int *)(&stack0x00000044 + iVar2) = iVar8 >> 6;
  uVar5 = 0x20;
  iVar8 = (*(code *)PTR_FUN_8c657fa8)();
  *(int *)(&stack0x00000000 + iVar2) = iVar8 + -1;
  iVar8 = iVar7;
  if (iVar7 < 0) {
    iVar8 = iVar7 + 7;
  }
  iVar9 = (iVar8 >> 3) * param_1;
  iVar8 = param_2 + -1;
  if (param_4 == 1) {
    if (param_2 == DAT_8c657fa2) {
      *(int *)(&stack0x00000004 + iVar2) = (int)DAT_8c657fa4;
      iVar3 = (*(code *)PTR_FUN_8c657fa8)();
      *(int *)(&stack0x00000008 + iVar2) = iVar3 + 1;
      *DAT_8c657fb0 = 0;
      *DAT_8c657fb4 = iVar9;
    }
    else {
      *(int *)(&stack0x00000004 + iVar2) = iVar8;
      *DAT_8c658024 = 0;
      *DAT_8c658028 = iVar9;
      *(undefined4 *)(&stack0x00000008 + iVar2) = 1;
    }
  }
  else if ((*DAT_8c65802c & 1) == 0) {
    if (param_2 == DAT_8c658020) {
      *(int *)(&stack0x00000004 + iVar2) = (int)DAT_8c658022;
      *DAT_8c658024 = 0;
      *DAT_8c658028 = iVar9;
      iVar9 = (*(code *)PTR_FUN_8c658030)();
      *(int *)(&stack0x00000008 + iVar2) = iVar9 + 1;
    }
    else {
      *(int *)(&stack0x00000004 + iVar2) = iVar8;
      *DAT_8c6580c4 = 0;
      *DAT_8c6580c8 = iVar9;
      *(undefined4 *)(&stack0x00000008 + iVar2) = 1;
    }
  }
  else {
    *(int *)(&stack0x00000004 + iVar2) = iVar8;
    *DAT_8c658024 = 0;
    *DAT_8c658028 = iVar9;
    *(undefined4 *)(&stack0x00000008 + iVar2) = 1;
  }
  iVar9 = (int)DAT_8c6580c0;
  if (*(int *)((int)&local_20 + DAT_8c6580c2 + iVar2) == 0) {
    *(undefined4 *)(&stack0x0000000c + iVar2) = 0;
    if (((*DAT_8c65814c & 1) == 1) || ((*DAT_8c65814c & 0x2c) != 0x20)) {
      if ((*DAT_8c6581e4 & 1) == 0) {
        if ((*DAT_8c6581e4 & uVar5) == 0) {
          *(int *)(&stack0x00000010 + iVar2) = (int)DAT_8c6581e0;
          *(int *)(&stack0x00000004 + iVar2) = *(int *)(&stack0x00000004 + iVar2) + -1;
        }
        else {
          *(int *)(&stack0x00000010 + iVar2) = iVar9;
          *(int *)(&stack0x00000004 + iVar2) = *(int *)(&stack0x00000004 + iVar2) + -2;
        }
      }
      else {
        *(int *)(&stack0x00000010 + iVar2) = (int)DAT_8c6581e0;
      }
    }
    else {
      *(int *)(&stack0x00000010 + iVar2) = (int)DAT_8c658140;
      *(int *)(&stack0x00000004 + iVar2) = (int)DAT_8c658142;
      *DAT_8c658144 = 0;
      *DAT_8c658148 = 0;
      *(undefined4 *)(&stack0x00000008 + iVar2) = 1;
    }
    iVar3 = 1;
  }
  else {
    *DAT_8c6580cc = *DAT_8c6580cc | 1;
    *(undefined4 *)(&stack0x0000000c + iVar2) = 1;
    iVar3 = 2;
    if ((*DAT_8c6580d0 & 1) == 0) {
      uVar5 = *DAT_8c6580d0 & 0xffffffec;
      if ((uVar5 == 0x24) || (uVar5 == 0x28)) {
        *(int *)(&stack0x00000010 + iVar2) = iVar9;
        iVar9 = (*(code *)PTR_FUN_8c6580d4)();
        *(int *)(&stack0x00000008 + iVar2) = iVar9 + 1;
        *(int *)(&stack0x00000004 + iVar2) = *(int *)(&stack0x00000004 + iVar2) + -2;
      }
      else if (((uVar5 == 0x20) || (uVar5 == 0)) || (uVar5 == 4)) {
        *DAT_8c658144 = 0;
        *DAT_8c658148 = 0;
        *(int *)(&stack0x00000010 + iVar2) = (int)DAT_8c658140;
        *(undefined4 *)(&stack0x00000008 + iVar2) = 1;
        *(int *)(&stack0x00000004 + iVar2) = *(int *)(&stack0x00000004 + iVar2) + -2;
      }
    }
    else {
      *(int *)(&stack0x00000010 + iVar2) = iVar9;
      *(int *)(&stack0x00000004 + iVar2) = *(int *)(&stack0x00000004 + iVar2) + -2;
    }
  }
  *(undefined4 *)(&stack0x00000014 + iVar2) = 0x40;
  *(int *)(&stack0x00000018 + iVar2) = (int)DAT_8c6581e2;
  iVar9 = (*(code *)PTR_FUN_8c6581e8)();
  *DAT_8c6581ec = iVar9 * iVar3;
  iVar9 = (*(code *)PTR_FUN_8c6581e8)();
  puVar1 = DAT_8c6581f0;
  *DAT_8c6581f0 = iVar9 + iVar6;
  if (0xf < *puVar1) {
    *DAT_8c6581f0 = 0xf;
  }
  *(undefined4 *)(&stack0x0000004c + iVar2) = 9;
  *(undefined4 *)(&stack0x00000048 + iVar2) = 6;
  if (iVar7 == 0x10) {
    *(undefined4 *)(&stack0x00000050 + iVar2) = 8;
  }
  else {
    *(undefined4 *)(&stack0x00000050 + iVar2) = 0xb;
  }
  *(undefined4 *)(&stack0x00000058 + iVar2) = 0;
  *(int *)(&stack0x00000054 + iVar2) = param_1 + -1;
  *(undefined4 *)(&stack0x00000060 + iVar2) = 0;
  if (((*DAT_8c658260 & 1) == 1) || ((*DAT_8c658260 & 0x2c) != 0x20)) {
    *(int *)(&stack0x0000005c + iVar2) = iVar8;
  }
  else {
    *(int *)(&stack0x0000005c + iVar2) = ((int)(param_2 + (uint)(param_2 < 0)) >> 1) + -1;
  }
  (*(code *)PTR_FUN_8c6582c4)();
  FUN_8c657c60();
  *DAT_8c6582c8 = 0;
  *DAT_8c6582cc = 0;
  return 0;
}

