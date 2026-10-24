/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c666228) */

void FUN_8c665fa8(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  undefined *puVar5;
  int iVar6;
  int iVar7;
  undefined4 uVar8;
  uint uVar9;
  undefined1 *puVar10;
  int iVar11;
  uint uVar12;
  int iVar13;
  uint local_20;
  undefined4 local_c;
  
  local_c = 1;
  iVar6 = (int)DAT_8c665fe4;
  *(undefined4 *)((int)&local_20 + iVar6) = param_1;
  *(undefined4 *)(&stack0xffffffe4 + iVar6) = param_2;
  *(undefined4 *)(&stack0xffffffe8 + iVar6) = param_3;
  *(undefined4 *)(&stack0x00000030 + iVar6) = 0;
  iVar7 = *DAT_8c66601c;
  if (*DAT_8c6660c4 == 0) {
    FUN_8c665e3c(*(undefined4 *)((int)&local_20 + iVar6),*(undefined4 *)(&stack0xffffffe4 + iVar6),
                 *(undefined4 *)((int)&local_20 + DAT_8c6660c0 + iVar6));
  }
  iVar13 = *(int *)((int)&local_20 + iVar6) * *(int *)(&stack0xffffffe4 + iVar6);
  *(int *)(&stack0xffffffe4 + iVar6) = iVar13;
  iVar11 = *(int *)((int)&local_20 + DAT_8c6660c0 + iVar6) * 0x20;
  *(int *)(&stack0xffffffec + iVar6) =
       *(int *)(&stack0xffffffe8 + iVar6) * iVar13 * 0x18 + *(int *)(DAT_8c6660c8 + iVar11);
  if (((*DAT_8c6660cc & 0x20) != 0) && (*DAT_8c6660d0 != 0)) {
    uVar8 = FUN_8c665f42(*(int *)(DAT_8c6660d4 + iVar11 + 4) + iVar13 * 0x60,
                         *(undefined4 *)(DAT_8c6660d4 + iVar11));
    *(undefined4 *)((int)&local_c + iVar6) = uVar8;
    if ((*DAT_8c6660d8 == 0) && ((*DAT_8c6660dc != 0 && ((iVar7 == 1) != 0)))) {
      uVar8 = 1;
    }
    else {
      uVar8 = 0;
    }
    *(undefined4 *)(&stack0x00000030 + iVar6) = uVar8;
  }
  puVar5 = PTR_FUN_8c6662a4;
  uVar4 = DAT_8c6662a0;
  uVar3 = DAT_8c66629c;
  uVar2 = DAT_8c666298;
  uVar1 = DAT_8c6660e0;
  if (*DAT_8c6660d8 == 0) {
    *(uint *)(&stack0x0000002c + iVar6) = DAT_8c6660e0;
  }
  else {
    *(undefined4 *)(&stack0x0000002c + iVar6) = 0;
  }
  if (*DAT_8c666160 == 0) {
    *(uint *)(&stack0x0000003c + iVar6) = DAT_8c6660e0;
  }
  else {
    *(undefined4 *)(&stack0x0000003c + iVar6) = 0;
  }
  if (*DAT_8c666164 == 0) {
    *(uint *)(&stack0x00000028 + iVar6) = DAT_8c6660e0;
  }
  else {
    *(undefined4 *)(&stack0x00000028 + iVar6) = 0;
  }
  if (*DAT_8c666168 == 0) {
    *(uint *)(&stack0x00000034 + iVar6) = DAT_8c6660e0;
  }
  else {
    *(undefined4 *)(&stack0x00000034 + iVar6) = 0;
  }
  if (*DAT_8c66616c == 0) {
    *(uint *)(&stack0x00000038 + iVar6) = DAT_8c6660e0;
  }
  else {
    *(undefined4 *)(&stack0x00000038 + iVar6) = 0;
  }
  *(int *)(&stack0x00000018 + iVar6) = *(int *)(PTR_DAT_8c666284 + *DAT_8c666288 * 4) << 2;
  *(int *)(&stack0x00000020 + iVar6) = *(int *)(PTR_DAT_8c666284 + *DAT_8c66628c * 4) << 2;
  *(int *)(&stack0x0000001c + iVar6) = *(int *)(PTR_DAT_8c666284 + *DAT_8c666290 * 4) << 2;
  *(int *)(&stack0x00000010 + iVar6) = *(int *)(PTR_DAT_8c666284 + DAT_8c66628c[2] * 4) << 2;
  *(int *)(&stack0x00000024 + iVar6) = *(int *)(PTR_DAT_8c666284 + DAT_8c666290[2] * 4) << 2;
  *(undefined4 *)(&stack0xfffffffc + iVar6) = *(undefined4 *)(DAT_8c666294 + iVar11);
  iVar13 = *(int *)(&stack0xffffffe4 + iVar6);
  iVar11 = *(int *)(PTR_DAT_8c666284 + *DAT_8c666288 * 4) * iVar13 * 4 +
           *(int *)(&stack0xfffffffc + iVar6);
  *(int *)(&stack0xfffffff8 + iVar6) = iVar11;
  *(int *)(&stack0x00000004 + iVar6) =
       iVar11 + *(int *)(PTR_DAT_8c666284 + DAT_8c666290[-1] * 4) * iVar13 * 4;
  iVar11 = *(int *)(PTR_DAT_8c666284 + *DAT_8c666290 * 4) * iVar13 * 4 +
           *(int *)(&stack0x00000004 + iVar6);
  *(int *)(&stack0x00000000 + iVar6) = iVar11;
  uVar12 = 0;
  *(int *)(&stack0x00000008 + iVar6) =
       iVar11 + *(int *)(PTR_DAT_8c666284 + DAT_8c666288[3] * 4) * iVar13 * 4;
  *(int *)(&stack0x0000000c + iVar6) = iVar13 + -1;
  *(uint *)(&stack0x00000014 + iVar6) = (uint)(iVar7 == 1) << 0x1d;
  if (iVar13 != 0) {
    do {
      iVar7 = *(int *)((int)&local_20 + DAT_8c666280 + iVar6);
      uVar9 = 0;
      if (iVar7 == 0) {
        uVar8 = (*(code *)PTR_FUN_8c6662ac)();
        *(undefined4 *)(&stack0xfffffff0 + iVar6) = uVar8;
        uVar8 = (*(code *)PTR_FUN_8c6662b0)();
LAB_8c6662d6:
        *(undefined4 *)(&stack0xffffffe8 + iVar6) = uVar8;
      }
      else if (iVar7 == 1) {
        uVar8 = (*(code *)PTR_FUN_8c6663b0)();
        *(undefined4 *)(&stack0xfffffff0 + iVar6) = uVar8;
        uVar8 = (*(code *)PTR_FUN_8c6663b4)();
        goto LAB_8c6662d6;
      }
      if ((*DAT_8c6663b8 == 1) &&
         ((*DAT_8c6663bc < *(uint *)(&stack0xfffffff0 + iVar6) ||
          (*DAT_8c6663c0 < *(uint *)(&stack0xffffffe8 + iVar6))))) {
        uVar9 = uVar1;
      }
      iVar7 = *(uint *)(&stack0xffffffe8 + iVar6) * *DAT_8c6663ac +
              *(uint *)(&stack0xfffffff0 + iVar6);
      *(uint *)((int)&local_20 + DAT_8c6663a0 + iVar6) =
           (uint)(uVar12 == *(uint *)(&stack0x0000000c + iVar6)) << 0x1f |
           (*(uint *)(&stack0xffffffe8 + iVar6) & 0x3f) << 8 |
           (*(uint *)(&stack0xfffffff0 + iVar6) & 0x3f) << 2 | *(uint *)(&stack0x00000014 + iVar6);
      *(uint *)((int)&local_20 + DAT_8c6663a2 + iVar6) =
           *(int *)(&stack0x00000018 + iVar6) * iVar7 + *(int *)(&stack0xfffffffc + iVar6) |
           *(uint *)(&stack0x0000002c + iVar6) | uVar9;
      *(uint *)((int)&local_20 + DAT_8c6663a4 + iVar6) =
           *(int *)(&stack0x00000020 + iVar6) * iVar7 + *(int *)(&stack0xfffffff8 + iVar6) |
           *(uint *)(&stack0x0000003c + iVar6) | uVar9;
      *(uint *)((int)&local_20 + DAT_8c6663a6 + iVar6) =
           *(int *)(&stack0x0000001c + iVar6) * iVar7 + *(int *)(&stack0x00000004 + iVar6) |
           *(uint *)(&stack0x00000028 + iVar6) | uVar9;
      *(uint *)((int)&local_20 + DAT_8c6663a8 + iVar6) =
           *(int *)(&stack0x00000010 + iVar6) * iVar7 + *(int *)(&stack0x00000000 + iVar6) |
           *(uint *)(&stack0x00000034 + iVar6) | uVar9;
      if (*(int *)PTR_DAT_8c6663c4 == 0) {
        *(uint *)((int)&local_20 + DAT_8c666420 + iVar6) =
             *(int *)(&stack0x00000024 + iVar6) * iVar7 + *(int *)(&stack0x00000008 + iVar6) |
             *(uint *)(&stack0x00000038 + iVar6) | uVar9;
      }
      else {
        *(undefined4 *)((int)&local_20 + DAT_8c6663aa + iVar6) =
             *(undefined4 *)((int)&local_20 + DAT_8c6663a6 + iVar6);
      }
      if (*DAT_8c666428 != 0) {
        if (*DAT_8c66642c == 1) {
          *(undefined4 *)((int)&local_20 + DAT_8c666424 + iVar6) =
               *(undefined4 *)((int)&local_20 + DAT_8c666422 + iVar6);
        }
        else {
          *(undefined4 *)((int)&local_20 + DAT_8c6665a2 + iVar6) =
               *(undefined4 *)((int)&local_20 + DAT_8c6665a0 + iVar6);
        }
      }
      if ((*DAT_8c6665c0 & 0x20) == 0) {
        (*(code *)puVar5)(uVar12 * 0x18 + *(int *)(&stack0xffffffec + iVar6),
                          (int)&local_20 + DAT_8c6666a2 + iVar6,0x18);
      }
      else {
        uVar8 = *(undefined4 *)((int)&local_20 + DAT_8c6665a4 + iVar6);
        if (*DAT_8c6665c4 == 0) {
          *(undefined4 *)((int)&local_20 + DAT_8c6666a0 + iVar6) = uVar8;
          *(uint *)((int)&local_20 + DAT_8c6666a2 + iVar6) =
               *(uint *)((int)&local_20 + DAT_8c6666a2 + iVar6) & uVar4;
          *(uint *)((int)&local_20 + DAT_8c6666a2 + iVar6) =
               *(uint *)((int)&local_20 + DAT_8c6666a2 + iVar6) & DAT_8c6666b0 | uVar2;
          *(uint *)((int)&local_20 + DAT_8c6666a0 + iVar6) =
               *(uint *)((int)&local_20 + DAT_8c6666a0 + iVar6) | uVar3;
          *(uint *)((int)&local_20 + DAT_8c6666a0 + iVar6) =
               *(uint *)((int)&local_20 + DAT_8c6666a0 + iVar6) & DAT_8c6666b4;
          *(uint *)((int)&local_20 + DAT_8c6666a4 + iVar6) = uVar1;
          *(uint *)((int)&local_20 + DAT_8c6666a6 + iVar6) = uVar1;
          *(uint *)((int)&local_20 + DAT_8c6666a8 + iVar6) = uVar1;
          *(uint *)((int)&local_20 + DAT_8c6666aa + iVar6) = uVar1;
          *(uint *)((int)&local_20 + DAT_8c6666ac + iVar6) = uVar1;
          iVar7 = uVar12 * 0x30 + *(int *)(&stack0xffffffec + iVar6);
          *(int *)((int)&local_20 + iVar6) = iVar7;
          (*(code *)puVar5)(iVar7,(int)&local_20 + DAT_8c6666a2 + iVar6,0x18);
          iVar7 = *(int *)((int)&local_20 + iVar6) + 0x18;
          puVar10 = (undefined1 *)((int)&local_20 + DAT_8c6666a0 + iVar6);
        }
        else {
          *(undefined4 *)((int)&local_20 + DAT_8c6665a6 + iVar6) = uVar8;
          *(undefined4 *)(&stack0x00000058 + iVar6) = uVar8;
          *(undefined4 *)(&stack0x00000040 + iVar6) = uVar8;
          *(uint *)((int)&local_20 + DAT_8c6665a4 + iVar6) =
               *(uint *)((int)&local_20 + DAT_8c6665a4 + iVar6) & uVar4;
          *(uint *)((int)&local_20 + DAT_8c6665a6 + iVar6) =
               *(uint *)((int)&local_20 + DAT_8c6665a6 + iVar6) & uVar4;
          *(uint *)(&stack0x00000058 + iVar6) = *(uint *)(&stack0x00000058 + iVar6) & uVar4;
          *(uint *)((int)&local_20 + DAT_8c6665a4 + iVar6) =
               *(uint *)((int)&local_20 + DAT_8c6665a4 + iVar6) & DAT_8c6665c8;
          *(uint *)((int)&local_20 + DAT_8c6665a6 + iVar6) =
               *(uint *)((int)&local_20 + DAT_8c6665a6 + iVar6) | uVar3;
          *(uint *)(&stack0x00000058 + iVar6) = *(uint *)(&stack0x00000058 + iVar6) | uVar3;
          *(uint *)(&stack0x00000040 + iVar6) = *(uint *)(&stack0x00000040 + iVar6) | uVar3;
          *(uint *)((int)&local_20 + DAT_8c6665a4 + iVar6) =
               *(uint *)((int)&local_20 + DAT_8c6665a4 + iVar6) | uVar2;
          *(uint *)((int)&local_20 + DAT_8c6665a6 + iVar6) =
               *(uint *)((int)&local_20 + DAT_8c6665a6 + iVar6) | uVar2;
          *(uint *)(&stack0x00000058 + iVar6) = *(uint *)(&stack0x00000058 + iVar6) | uVar2;
          *(uint *)(&stack0x00000040 + iVar6) = *(uint *)(&stack0x00000040 + iVar6) & DAT_8c6665cc;
          *(uint *)((int)&local_20 + DAT_8c6665a6 + iVar6) =
               *(uint *)((int)&local_20 + DAT_8c6665a6 + iVar6) & DAT_8c6665d0;
          if (*(int *)(&stack0x00000030 + iVar6) != 0) {
            *(undefined4 *)((int)&local_20 + DAT_8c6665a8 + iVar6) =
                 *(undefined4 *)((int)&local_c + iVar6);
          }
          *(uint *)((int)&local_20 + DAT_8c6665aa + iVar6) = uVar1;
          *(uint *)((int)&local_20 + DAT_8c6665ac + iVar6) = uVar1;
          *(undefined4 *)((int)&local_20 + DAT_8c6665ae + iVar6) =
               *(undefined4 *)((int)&local_c + iVar6);
          *(uint *)((int)&local_20 + DAT_8c6665b0 + iVar6) = uVar1;
          *(uint *)((int)&local_20 + DAT_8c6665b2 + iVar6) = uVar1;
          *(uint *)(&stack0x0000005c + iVar6) = uVar1;
          *(uint *)((int)&local_20 + DAT_8c6665b4 + iVar6) = uVar1;
          *(undefined4 *)((int)&local_20 + DAT_8c6665b8 + iVar6) =
               *(undefined4 *)((int)&local_20 + DAT_8c6665b6 + iVar6);
          *(undefined4 *)((int)&local_20 + DAT_8c6665ba + iVar6) =
               *(undefined4 *)((int)&local_20 + DAT_8c6665a0 + iVar6);
          *(uint *)((int)&local_20 + DAT_8c6665bc + iVar6) = uVar1;
          *(uint *)((int)&local_20 + DAT_8c6665b6 + iVar6) = uVar1;
          *(uint *)((int)&local_20 + DAT_8c6665a0 + iVar6) = uVar1;
          *(uint *)(&stack0x00000044 + iVar6) = uVar1;
          *(uint *)(&stack0x00000048 + iVar6) = uVar1;
          *(uint *)(&stack0x0000004c + iVar6) = uVar1;
          *(uint *)(&stack0x00000050 + iVar6) = uVar1;
          *(uint *)(&stack0x00000054 + iVar6) = uVar1;
          iVar7 = uVar12 * 0x60 + *(int *)(&stack0xffffffec + iVar6);
          *(int *)((int)&local_20 + iVar6) = iVar7;
          (*(code *)puVar5)(iVar7,(int)&local_20 + DAT_8c6665a4 + iVar6,0x18);
          (*(code *)puVar5)(*(int *)((int)&local_20 + iVar6) + 0x18,
                            (int)&local_20 + DAT_8c6665a6 + iVar6,0x18);
          (*(code *)puVar5)(*(int *)((int)&local_20 + iVar6) + 0x30,&stack0x00000058 + iVar6,0x18);
          puVar10 = &stack0x00000040 + iVar6;
          iVar7 = *(int *)((int)&local_20 + iVar6) + 0x48;
        }
        (*(code *)puVar5)(iVar7,puVar10,0x18);
      }
      uVar12 = uVar12 + 1;
    } while (uVar12 < *(uint *)(&stack0xffffffe4 + iVar6));
  }
  return;
}

