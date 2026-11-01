/* Ghidra draft only; not buildable/verified source. */

undefined4 callback_8c64c374(undefined4 param_1,undefined4 param_2)

{
  undefined *puVar1;
  undefined4 *puVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  undefined4 uVar6;
  int iStack_8;
  uint uStack_4;
  
  iVar3 = (int)DAT_8c64c472;
  *(undefined4 *)((int)&uStack_4 + DAT_8c64c474 + iVar3) = param_1;
  *(undefined4 *)((int)&uStack_4 + DAT_8c64c476 + iVar3) = param_2;
  *(undefined4 *)(&stack0x00000014 + iVar3) = 0;
  *(undefined4 *)(&stack0x0000000c + iVar3) = 0xffffffff;
  iVar5 = *(int *)((int)&uStack_4 + DAT_8c64c476 + iVar3) * 0xc +
          **(int **)((int)&uStack_4 + DAT_8c64c474 + iVar3);
  *(int *)((int)&uStack_4 + DAT_8c64c478 + iVar3) = iVar5;
  *(int *)((int)&uStack_4 + DAT_8c64c47a + iVar3) = iVar5 + 8;
  if (((*(uint *)(*(int *)((int)&uStack_4 + DAT_8c64c478 + iVar3) + 4) & DAT_8c64c48c) != 0) &&
     (*(int *)((int)&uStack_4 + DAT_8c64c476 + iVar3) != 0)) {
    iVar5 = (*(int *)((int)&uStack_4 + DAT_8c64c476 + iVar3) + -1) * 0xc +
            **(int **)((int)&uStack_4 + DAT_8c64c474 + iVar3);
    *(int *)((int)&uStack_4 + DAT_8c64c47c + iVar3) = iVar5;
    *(int *)((int)&uStack_4 + DAT_8c64c47e + iVar3) = iVar5 + 8;
    *(uint *)(&stack0x0000000c + iVar3) =
         (*(uint *)(*(int *)((int)&uStack_4 + DAT_8c64c478 + iVar3) + 8) & DAT_8c64c490) >> 0x1a;
    iVar5 = (*(code *)PTR_FUN_8c64c49c)
                      (*(undefined4 *)PTR_DAT_8c64c498,*(undefined4 *)PTR_DAT_8c64c494,
                       *(undefined4 *)(&stack0x00000010 + iVar3));
    *(int *)(&stack0x0000001c + iVar3) = iVar5;
    if ((*(uint *)(iVar5 * 0x44 + *(int *)PTR_DAT_8c64c498 + 0x10) & 1) == 0) {
      uVar6 = 2;
      *(undefined4 *)((int)&iStack_8 + iVar3) = *(undefined4 *)(&stack0x0000000c + iVar3);
    }
    else {
      *(undefined4 *)((int)&iStack_8 + iVar3) = *(undefined4 *)(&stack0x0000000c + iVar3);
      uVar6 = 3;
    }
    (*(code *)PTR_FUN_8c64c4a0)
              (*(undefined4 *)((int)&iStack_8 + DAT_8c64c476 + iVar3),
               **(int **)((int)&iStack_8 + DAT_8c64c47a + iVar3) + 0x14,
               *(undefined4 *)(&stack0x00000010 + iVar3),uVar6);
    *(undefined2 *)(**(int **)((int)&uStack_4 + DAT_8c64c47a + iVar3) + 0x42) = 0;
    (*(code *)PTR_FUN_8c64c4a4)(**(undefined4 **)((int)&uStack_4 + DAT_8c64c47a + iVar3));
    (*(code *)PTR_FUN_8c64c4a8)
              (**(undefined4 **)((int)&uStack_4 + DAT_8c64c47a + iVar3),
               *(undefined4 *)(&stack0x0000000c + iVar3));
    *(undefined4 *)(&stack0x00000014 + iVar3) = 1;
    return 1;
  }
  iVar5 = (*(code *)PTR_FUN_8c64c5e0)
                    (&stack0x00000020 + iVar3,
                     **(undefined4 **)((int)&uStack_4 + DAT_8c64c5d6 + iVar3));
  *(int *)((int)&uStack_4 + DAT_8c64c5d8 + iVar3) = iVar5;
  if (iVar5 < 1) {
    (*(code *)PTR_FUN_8c64d250)(DAT_8c64d24c,DAT_8c64d248,0x28);
    *(undefined4 *)((int)&iStack_8 + iVar3) = *(undefined4 *)(&stack0x0000000c + iVar3);
    (*(code *)PTR_FUN_8c64d238)
              (*(undefined4 *)((int)&iStack_8 + DAT_8c64d22e + iVar3),DAT_8c64d254,0xffffffff,0);
    *(undefined2 *)(**(int **)((int)&uStack_4 + DAT_8c64d230 + iVar3) + 0x42) = 3;
    *(undefined2 *)(**(int **)((int)&uStack_4 + DAT_8c64d230 + iVar3) + 0x40) = 0;
    *DAT_8c64d258 = *(undefined4 *)((int)&uStack_4 + DAT_8c64d22e + iVar3);
    *DAT_8c64d25c = *(undefined4 *)(&stack0x00000010 + iVar3);
    return 0xffffffff;
  }
  if ((*(uint *)(*(int *)((int)&uStack_4 + DAT_8c64c5dc + iVar3) * 0xc +
                 **(int **)((int)&uStack_4 + DAT_8c64c5da + iVar3) + 4) & DAT_8c64c5e4) != 0) {
    uVar6 = *(undefined4 *)
             (*(int *)((int)&uStack_4 + DAT_8c64c5dc + iVar3) * 0xc +
              **(int **)((int)&uStack_4 + DAT_8c64c5da + iVar3) + 8);
    *(undefined4 *)(&stack0x00000010 + iVar3) = uVar6;
    iVar5 = (*(code *)PTR_FUN_8c64c5f0)
                      (*(undefined4 *)PTR_DAT_8c64c5ec,*(undefined4 *)PTR_DAT_8c64c5e8,uVar6);
    *(int *)(&stack0x0000001c + iVar3) = iVar5;
    if (iVar5 < 0) {
      *(undefined4 *)(&stack0x00000008 + iVar3) = 2;
    }
    else {
      *(undefined4 *)(&stack0x00000008 + iVar3) = 1;
    }
    iVar5 = (*(code *)PTR_FUN_8c64c5f4)(&stack0x00000020 + iVar3);
    *(int *)((int)&uStack_4 + DAT_8c64c5d8 + iVar3) = iVar5;
    if (iVar5 < 0) {
      (*(code *)PTR_FUN_8c64c600)(DAT_8c64c5fc,DAT_8c64c5f8,0x28);
      *(undefined4 *)((int)&iStack_8 + iVar3) = 0xffffffff;
      (*(code *)PTR_FUN_8c64c608)
                (*(undefined4 *)((int)&iStack_8 + DAT_8c64c5dc + iVar3),DAT_8c64c604,
                 *(undefined4 *)(&stack0x00000010 + iVar3),0);
      *(short *)(**(int **)((int)&uStack_4 + DAT_8c64c5de + iVar3) + 0x42) =
           -*(short *)((int)&uStack_4 + DAT_8c64c5d8 + iVar3);
      *(undefined2 *)(**(int **)((int)&uStack_4 + DAT_8c64c5de + iVar3) + 0x40) = 0;
      return 0xffffffff;
    }
    goto LAB_8c64c814;
  }
  if ((*(uint *)(*(int *)((int)&uStack_4 + DAT_8c64c5dc + iVar3) * 0xc +
                 **(int **)((int)&uStack_4 + DAT_8c64c5da + iVar3) + 4) & DAT_8c64c60c) == 0) {
    iVar5 = (*(code *)PTR_FUN_8c64c734)(&stack0x00000020 + iVar3);
    *(int *)((int)&uStack_4 + DAT_8c64c720 + iVar3) = iVar5;
    if (iVar5 < 0) {
      (*(code *)PTR_FUN_8c64c740)(DAT_8c64c73c,DAT_8c64c738,0x28);
      *(undefined4 *)((int)&iStack_8 + iVar3) = 0xffffffff;
      (*(code *)PTR_FUN_8c64c748)
                (*(undefined4 *)((int)&iStack_8 + DAT_8c64c722 + iVar3),DAT_8c64c744,
                 *(undefined4 *)(&stack0x00000010 + iVar3),0);
      *(short *)(**(int **)((int)&uStack_4 + DAT_8c64c724 + iVar3) + 0x42) =
           -*(short *)((int)&uStack_4 + DAT_8c64c720 + iVar3);
      *(undefined2 *)(**(int **)((int)&uStack_4 + DAT_8c64c724 + iVar3) + 0x40) = 0;
      return 0xffffffff;
    }
    uVar6 = (*(code *)PTR_FUN_8c64c74c)(*(undefined4 *)((int)&uStack_4 + DAT_8c64c726 + iVar3));
    *(undefined4 *)(&stack0x00000010 + iVar3) = uVar6;
    iVar5 = (*(code *)PTR_FUN_8c64c750)();
    *(int *)(&stack0x00000004 + iVar3) = iVar5;
    if (iVar5 == -1) {
      (*(code *)PTR_FUN_8c64c740)(DAT_8c64c73c,DAT_8c64c738,0x28);
      *(undefined4 *)((int)&iStack_8 + iVar3) = 0xffffffff;
      (*(code *)PTR_FUN_8c64c748)
                (*(undefined4 *)((int)&iStack_8 + DAT_8c64c722 + iVar3),DAT_8c64c744,
                 *(undefined4 *)(&stack0x00000010 + iVar3),0);
      puVar1 = PTR_FUN_8c64c758;
      *DAT_8c64c754 = 1;
      (*(code *)puVar1)();
      return 0xffffffff;
    }
    iVar5 = (*(code *)PTR_FUN_8c64c838)(*(uint *)(&stack0x00000004 + iVar3) & DAT_8c64c834);
    *(int *)(&stack0x00000014 + iVar3) = iVar5;
    if (iVar5 == 1) {
      *(uint *)(&stack0x0000000c + iVar3) =
           (*(uint *)(*(int *)((int)&uStack_4 + DAT_8c64c82e + iVar3) + 8) & DAT_8c64c83c) >> 0x1a;
      iVar5 = (*(code *)PTR_FUN_8c64c848)
                        (*(undefined4 *)PTR_DAT_8c64c844,*(undefined4 *)PTR_DAT_8c64c840,
                         *(undefined4 *)(&stack0x00000010 + iVar3));
      *(int *)(&stack0x0000001c + iVar3) = iVar5;
      if (iVar5 < 0) {
        uVar6 = 2;
      }
      else {
        iVar5 = (*(code *)PTR_FUN_8c64c84c)
                          (*(undefined4 *)PTR_DAT_8c64c844,*(undefined4 *)PTR_DAT_8c64c840,
                           *(undefined4 *)(&stack0x00000010 + iVar3),
                           *(undefined4 *)(&stack0x0000000c + iVar3));
        *(int *)(&stack0x00000018 + iVar3) = iVar5;
        if (-1 < iVar5) {
          *(undefined4 *)(&stack0x00000008 + iVar3) = 1;
          goto LAB_8c64c814;
        }
        uVar6 = 0;
      }
    }
    else {
      if (*(int *)(&stack0x00000010 + iVar3) == -1) {
        *(undefined4 *)((int)&iStack_8 + iVar3) = 0xffffffff;
        (*(code *)PTR_FUN_8c64c854)
                  (*(undefined4 *)((int)&iStack_8 + DAT_8c64c830 + iVar3),DAT_8c64c850,
                   *(undefined4 *)(&stack0x00000010 + iVar3),0);
        *(undefined2 *)(**(int **)((int)&uStack_4 + DAT_8c64c832 + iVar3) + 0x42) = 9;
        puVar1 = PTR_FUN_8c64c858;
        *(undefined2 *)(**(int **)((int)&uStack_4 + DAT_8c64c832 + iVar3) + 0x40) = 0;
        (*(code *)puVar1)();
        *DAT_8c64c85c = 9;
        return 0xffffffff;
      }
      iVar5 = (*(code *)PTR_FUN_8c64c848)
                        (*(undefined4 *)PTR_DAT_8c64c844,*(undefined4 *)PTR_DAT_8c64c840,
                         *(undefined4 *)(&stack0x00000010 + iVar3));
      *(int *)(&stack0x0000001c + iVar3) = iVar5;
      if (-1 < iVar5) {
        *(undefined4 *)(&stack0x00000008 + iVar3) = 1;
        goto LAB_8c64c814;
      }
      uVar6 = 2;
    }
    *(undefined4 *)(&stack0x00000008 + iVar3) = uVar6;
    goto LAB_8c64c814;
  }
  *(uint *)(&stack0x00000010 + iVar3) =
       *(uint *)(*(int *)((int)&uStack_4 + DAT_8c64c5dc + iVar3) * 0xc +
                 **(int **)((int)&uStack_4 + DAT_8c64c5da + iVar3) + 8) & DAT_8c64c610;
  *(uint *)(&stack0x0000000c + iVar3) =
       (*(uint *)(*(int *)((int)&uStack_4 + DAT_8c64c5d6 + iVar3) + 8) & DAT_8c64c614) >> 0x1a;
  iVar5 = (*(code *)PTR_FUN_8c64c5f0)
                    (*(undefined4 *)PTR_DAT_8c64c5ec,*(undefined4 *)PTR_DAT_8c64c5e8,
                     *(undefined4 *)(&stack0x00000010 + iVar3));
  *(int *)(&stack0x0000001c + iVar3) = iVar5;
  if (iVar5 < 0) {
    uVar6 = 2;
LAB_8c64c630:
    *(undefined4 *)(&stack0x00000008 + iVar3) = uVar6;
  }
  else {
    iVar5 = (*(code *)PTR_FUN_8c64c730)
                      (*(undefined4 *)PTR_DAT_8c64c72c,*(undefined4 *)PTR_DAT_8c64c728,
                       *(undefined4 *)(&stack0x00000010 + iVar3),
                       *(undefined4 *)(&stack0x0000000c + iVar3));
    *(int *)(&stack0x00000018 + iVar3) = iVar5;
    if (iVar5 < 0) {
      uVar6 = 0;
      goto LAB_8c64c630;
    }
    *(undefined4 *)(&stack0x00000008 + iVar3) = 1;
  }
  *(undefined4 *)(&stack0x00000014 + iVar3) = 1;
  iVar5 = (*(code *)PTR_FUN_8c64c734)(&stack0x00000020 + iVar3);
  *(int *)((int)&uStack_4 + DAT_8c64c720 + iVar3) = iVar5;
  if (iVar5 < 0) {
    (*(code *)PTR_FUN_8c64c740)(DAT_8c64c73c,DAT_8c64c738,0x28);
    *(undefined4 *)((int)&iStack_8 + iVar3) = *(undefined4 *)(&stack0x0000000c + iVar3);
    (*(code *)PTR_FUN_8c64c748)
              (*(undefined4 *)((int)&iStack_8 + DAT_8c64c722 + iVar3),DAT_8c64c744,
               *(undefined4 *)(&stack0x00000010 + iVar3),0);
    *(short *)(**(int **)((int)&uStack_4 + DAT_8c64c724 + iVar3) + 0x42) =
         -*(short *)((int)&uStack_4 + DAT_8c64c720 + iVar3);
    *(undefined2 *)(**(int **)((int)&uStack_4 + DAT_8c64c724 + iVar3) + 0x40) = 0;
    return 0xffffffff;
  }
LAB_8c64c814:
  iVar5 = *(int *)(&stack0x00000008 + iVar3);
  if (iVar5 == 0) {
    iVar5 = *(int *)(&stack0x0000001c + iVar3) * 0x44 + *(int *)PTR_DAT_8c64c994;
    *(int *)((int)&uStack_4 + DAT_8c64c988 + iVar3) = iVar5;
    uVar4 = *(uint *)(iVar5 + 0x10);
    *(uint *)(&stack0x00000000 + iVar3) = uVar4;
    if ((uVar4 & 2) == 0) {
      uVar4 = *(uint *)(*(int *)((int)&uStack_4 + DAT_8c64cacc + iVar3) + 4) & DAT_8c64cae8;
      if (uVar4 == 0) {
        iVar5 = (*(code *)PTR_FUN_8c64caf4)
                          (*(undefined4 *)((int)&uStack_4 + DAT_8c64cacc + iVar3),
                           *(int *)((int)&uStack_4 + DAT_8c64cace + iVar3) + 0x14);
        puVar2 = DAT_8c64caf8;
        *(int *)((int)&uStack_4 + DAT_8c64cad0 + iVar3) = iVar5;
        if (iVar5 < 0) {
          *(short *)(**(int **)((int)&uStack_4 + DAT_8c64cac6 + iVar3) + 0x42) =
               -*(short *)((int)&uStack_4 + DAT_8c64cad0 + iVar3);
          *puVar2 = *(undefined4 *)((int)&uStack_4 + DAT_8c64caca + iVar3);
          *DAT_8c64cafc = *(undefined4 *)(&stack0x00000010 + iVar3);
          (*(code *)PTR_FUN_8c64cb00)();
          return 0xffffffff;
        }
        *(undefined4 *)((int)&iStack_8 + iVar3) = *(undefined4 *)(&stack0x0000000c + iVar3);
        (*(code *)PTR_FUN_8c64cc34)
                  (*(undefined4 *)((int)&iStack_8 + DAT_8c64cc28 + iVar3),
                   *(int *)((int)&iStack_8 + DAT_8c64cc26 + iVar3) + 0x14,
                   *(undefined4 *)(&stack0x00000010 + iVar3),1);
        *(undefined2 *)(**(int **)((int)&uStack_4 + DAT_8c64cc2a + iVar3) + 0x42) = 0;
        (*(code *)PTR_FUN_8c64cc38)
                  (*(undefined4 *)(&stack0x00000010 + iVar3),2,
                   *(undefined4 *)(**(int **)((int)&uStack_4 + DAT_8c64cc2a + iVar3) + 0x14));
      }
      else if (uVar4 == DAT_8c64caec) {
        *(undefined4 *)((int)&iStack_8 + iVar3) = *(undefined4 *)(&stack0x0000000c + iVar3);
        (*(code *)PTR_FUN_8c64cc34)
                  (*(undefined4 *)((int)&iStack_8 + DAT_8c64cc28 + iVar3),
                   *(int *)((int)&iStack_8 + DAT_8c64cc26 + iVar3) + 0x14,
                   *(undefined4 *)(&stack0x00000010 + iVar3),1);
      }
      else if (uVar4 == DAT_8c64caf0) {
        iVar5 = (*(code *)PTR_FUN_8c64cc3c)
                          (*(undefined4 *)((int)&uStack_4 + DAT_8c64cc2e + iVar3),
                           *(int *)((int)&uStack_4 + DAT_8c64cc2c + iVar3) + 0x14);
        puVar2 = DAT_8c64cc40;
        *(int *)((int)&uStack_4 + DAT_8c64cc30 + iVar3) = iVar5;
        if (iVar5 < 0) {
          *(short *)(**(int **)((int)&uStack_4 + DAT_8c64cc2a + iVar3) + 0x42) =
               -*(short *)((int)&uStack_4 + DAT_8c64cc30 + iVar3);
          *puVar2 = *(undefined4 *)((int)&uStack_4 + DAT_8c64cc28 + iVar3);
          *DAT_8c64cc44 = *(undefined4 *)(&stack0x00000010 + iVar3);
          (*(code *)PTR_FUN_8c64cc48)();
          return 0xffffffff;
        }
        *(undefined4 *)((int)&iStack_8 + iVar3) = *(undefined4 *)(&stack0x0000000c + iVar3);
        (*(code *)PTR_FUN_8c64cc34)
                  (*(undefined4 *)((int)&iStack_8 + DAT_8c64cc28 + iVar3),
                   *(int *)((int)&iStack_8 + DAT_8c64cc26 + iVar3) + 0x14,
                   *(undefined4 *)(&stack0x00000010 + iVar3),1);
        *(undefined2 *)(**(int **)((int)&uStack_4 + DAT_8c64cc2a + iVar3) + 0x42) = 0;
        (*(code *)PTR_FUN_8c64cc38)
                  (*(undefined4 *)(&stack0x00000010 + iVar3),2,
                   *(undefined4 *)(**(int **)((int)&uStack_4 + DAT_8c64cc2a + iVar3) + 0x14));
      }
    }
    else {
      uVar4 = *(uint *)(*(int *)((int)&uStack_4 + DAT_8c64c98a + iVar3) + 4) & DAT_8c64c998;
      if (uVar4 == 0) {
        *(undefined4 *)((int)&iStack_8 + iVar3) = *(undefined4 *)(&stack0x0000000c + iVar3);
        (*(code *)PTR_FUN_8c64c9a4)
                  (*(undefined4 *)((int)&iStack_8 + DAT_8c64c98e + iVar3),
                   *(int *)((int)&iStack_8 + DAT_8c64c98c + iVar3) + 0x14,
                   *(undefined4 *)(&stack0x00000010 + iVar3),2);
        *(undefined2 *)(**(int **)((int)&uStack_4 + DAT_8c64c990 + iVar3) + 0x42) = 0;
        puVar1 = PTR_FUN_8c64c9a8;
        *(undefined2 *)(**(int **)((int)&uStack_4 + DAT_8c64c990 + iVar3) + 0x40) = 1;
        (*(code *)puVar1)(**(undefined4 **)((int)&uStack_4 + DAT_8c64c990 + iVar3));
        (*(code *)PTR_FUN_8c64c9ac)
                  (**(undefined4 **)((int)&uStack_4 + DAT_8c64c990 + iVar3),
                   *(undefined4 *)(&stack0x0000000c + iVar3));
      }
      else if ((uVar4 == DAT_8c64c99c) || (uVar4 == DAT_8c64c9a0)) {
        if ((*(uint *)(*(int *)((int)&uStack_4 + DAT_8c64c988 + iVar3) + 0x10) & 1) == 0) {
          iVar5 = (*(code *)PTR_FUN_8c64c9b0)
                            (*(undefined4 *)((int)&uStack_4 + DAT_8c64c98a + iVar3),
                             **(int **)((int)&uStack_4 + DAT_8c64c990 + iVar3) + 0x14);
          puVar2 = DAT_8c64c9b4;
          *(int *)((int)&uStack_4 + DAT_8c64c992 + iVar3) = iVar5;
          if (iVar5 < 0) {
            *(short *)(**(int **)((int)&uStack_4 + DAT_8c64c990 + iVar3) + 0x42) =
                 -*(short *)((int)&uStack_4 + DAT_8c64c992 + iVar3);
            *puVar2 = *(undefined4 *)((int)&uStack_4 + DAT_8c64c98e + iVar3);
            *DAT_8c64c9b8 = *(undefined4 *)(&stack0x00000010 + iVar3);
            (*(code *)PTR_FUN_8c64c9bc)();
            return 0xffffffff;
          }
          *(undefined4 *)((int)&iStack_8 + iVar3) = *(undefined4 *)(&stack0x0000000c + iVar3);
          (*(code *)PTR_FUN_8c64c9a4)
                    (*(undefined4 *)((int)&iStack_8 + DAT_8c64c98e + iVar3),
                     *(int *)((int)&iStack_8 + DAT_8c64c98c + iVar3) + 0x14,
                     *(undefined4 *)(&stack0x00000010 + iVar3),2);
          *(undefined2 *)(**(int **)((int)&uStack_4 + DAT_8c64c990 + iVar3) + 0x42) = 0;
          puVar1 = PTR_FUN_8c64c9a8;
          *(undefined2 *)(**(int **)((int)&uStack_4 + DAT_8c64c990 + iVar3) + 0x40) = 1;
          (*(code *)puVar1)(**(undefined4 **)((int)&uStack_4 + DAT_8c64c990 + iVar3));
          (*(code *)PTR_FUN_8c64c9ac)
                    (**(undefined4 **)((int)&uStack_4 + DAT_8c64c990 + iVar3),
                     *(undefined4 *)(&stack0x0000000c + iVar3));
          *(undefined4 *)((int)&uStack_4 + iVar3) = 0;
          while (puVar1 = PTR_DAT_8c64cad4,
                *(uint *)((int)&uStack_4 + iVar3) < *(uint *)PTR_DAT_8c64cad8) {
            if (*(int *)(*(int *)((int)&uStack_4 + iVar3) * 0x44 + *(int *)PTR_DAT_8c64cad4) ==
                *(int *)(&stack0x00000010 + iVar3)) {
              iVar5 = *(int *)((int)&uStack_4 + iVar3) * 0x44 + *(int *)PTR_DAT_8c64cad4;
              *(uint *)(iVar5 + 0x10) = *(uint *)(iVar5 + 0x10) | 1;
              *(undefined4 *)(*(int *)((int)&uStack_4 + iVar3) * 0x44 + *(int *)puVar1 + 0x14) =
                   *(undefined4 *)(**(int **)((int)&uStack_4 + DAT_8c64cac6 + iVar3) + 0x14);
            }
            *(int *)((int)&uStack_4 + iVar3) = *(int *)((int)&uStack_4 + iVar3) + 1;
          }
        }
        else {
          *(undefined4 *)((int)&iStack_8 + iVar3) = *(undefined4 *)(&stack0x0000000c + iVar3);
          (*(code *)PTR_FUN_8c64cadc)
                    (*(undefined4 *)((int)&iStack_8 + DAT_8c64caca + iVar3),
                     *(int *)((int)&iStack_8 + DAT_8c64cac8 + iVar3) + 0x14,
                     *(undefined4 *)(&stack0x00000010 + iVar3),3);
          *(undefined2 *)(**(int **)((int)&uStack_4 + DAT_8c64cac6 + iVar3) + 0x42) = 0;
          puVar1 = PTR_FUN_8c64cae0;
          *(undefined2 *)(**(int **)((int)&uStack_4 + DAT_8c64cac6 + iVar3) + 0x40) = 1;
          (*(code *)puVar1)(**(undefined4 **)((int)&uStack_4 + DAT_8c64cac6 + iVar3));
          (*(code *)PTR_FUN_8c64cae4)
                    (**(undefined4 **)((int)&uStack_4 + DAT_8c64cac6 + iVar3),
                     *(undefined4 *)(&stack0x0000000c + iVar3));
        }
      }
    }
  }
  else if (iVar5 == 1) {
    iVar5 = (int)DAT_8c64cc2e;
    **(int **)((int)&uStack_4 + DAT_8c64cc2a + iVar3) =
         *(int *)(&stack0x0000001c + iVar3) * 0x44 + *(int *)PTR_DAT_8c64cc4c;
    uVar4 = *(uint *)(*(int *)((int)&uStack_4 + iVar5 + iVar3) + 4) & DAT_8c64cc50;
    if (uVar4 == 0) {
      if ((*(uint *)(**(int **)((int)&uStack_4 + DAT_8c64cc2a + iVar3) + 0x10) & 2) == 0) {
        iVar5 = (*(code *)PTR_FUN_8c64cd68)
                          (*(undefined4 *)((int)&uStack_4 + DAT_8c64cd5a + iVar3),
                           *(int *)(&stack0x0000001c + iVar3) * 0x44 + *(int *)PTR_DAT_8c64cd64 +
                           0x14);
        puVar2 = DAT_8c64cd6c;
        *(int *)((int)&uStack_4 + DAT_8c64cd5c + iVar3) = iVar5;
        if (iVar5 < 0) {
          *(short *)(**(int **)((int)&uStack_4 + DAT_8c64cd5e + iVar3) + 0x42) =
               -*(short *)((int)&uStack_4 + DAT_8c64cd5c + iVar3);
          *puVar2 = *(undefined4 *)((int)&uStack_4 + DAT_8c64cd60 + iVar3);
          *DAT_8c64cd70 = *(undefined4 *)(&stack0x00000010 + iVar3);
          (*(code *)PTR_FUN_8c64cd74)();
          return 0xffffffff;
        }
        *(undefined2 *)(**(int **)((int)&uStack_4 + DAT_8c64cd5e + iVar3) + 0x42) = 0;
        iVar5 = **(int **)((int)&uStack_4 + DAT_8c64cd5e + iVar3);
        *(uint *)(iVar5 + 0x10) = *(uint *)(iVar5 + 0x10) | 2;
        iVar5 = **(int **)((int)&uStack_4 + DAT_8c64cd5e + iVar3);
        *(short *)(iVar5 + 0x40) = *(short *)(iVar5 + 0x40) + 1;
        if (*(int *)(&stack0x00000014 + iVar3) != 0) {
          (*(code *)PTR_FUN_8c64cd78)
                    (*(undefined4 *)(&stack0x00000010 + iVar3),2,
                     *(undefined4 *)(**(int **)((int)&uStack_4 + DAT_8c64cd5e + iVar3) + 0x14));
        }
      }
      else {
        iVar3 = **(int **)((int)&uStack_4 + DAT_8c64cc2a + iVar3);
        *(short *)(iVar3 + 0x40) = *(short *)(iVar3 + 0x40) + 1;
      }
    }
    else if (uVar4 == DAT_8c64cc54) {
      if ((*(uint *)(**(int **)((int)&uStack_4 + DAT_8c64cd5e + iVar3) + 0x10) & 1) == 0) {
        iVar5 = (*(code *)PTR_FUN_8c64cd7c)
                          (*(undefined4 *)((int)&uStack_4 + DAT_8c64cd5a + iVar3),
                           **(int **)((int)&uStack_4 + DAT_8c64cd5e + iVar3) + 0x14);
        puVar2 = DAT_8c64cd6c;
        *(int *)((int)&uStack_4 + DAT_8c64cd5c + iVar3) = iVar5;
        if (iVar5 < 0) {
          *(short *)(**(int **)((int)&uStack_4 + DAT_8c64cd5e + iVar3) + 0x42) =
               -*(short *)((int)&uStack_4 + DAT_8c64cd5c + iVar3);
          *puVar2 = *(undefined4 *)((int)&uStack_4 + DAT_8c64cd60 + iVar3);
          *DAT_8c64cd70 = *(undefined4 *)(&stack0x00000010 + iVar3);
          (*(code *)PTR_FUN_8c64cd74)();
          return 0xffffffff;
        }
        *(undefined2 *)(**(int **)((int)&uStack_4 + DAT_8c64cd5e + iVar3) + 0x42) = 0;
        iVar3 = **(int **)((int)&uStack_4 + DAT_8c64cd5e + iVar3);
        *(uint *)(iVar3 + 0x10) = *(uint *)(iVar3 + 0x10) | 1;
      }
    }
    else if (uVar4 == DAT_8c64cc58) {
      if ((*(uint *)(**(int **)((int)&uStack_4 + DAT_8c64ce8a + iVar3) + 0x10) & 2) == 0) {
        iVar5 = (*(code *)PTR_FUN_8c64cea4)
                          (*(undefined4 *)((int)&uStack_4 + DAT_8c64ce8c + iVar3),
                           **(int **)((int)&uStack_4 + DAT_8c64ce8a + iVar3) + 0x14);
        puVar2 = DAT_8c64ce98;
        *(int *)((int)&uStack_4 + DAT_8c64ce8e + iVar3) = iVar5;
        if (iVar5 < 0) {
          *(short *)(**(int **)((int)&uStack_4 + DAT_8c64ce8a + iVar3) + 0x42) =
               -*(short *)((int)&uStack_4 + DAT_8c64ce8e + iVar3);
          *puVar2 = *(undefined4 *)((int)&uStack_4 + DAT_8c64ce90 + iVar3);
          *DAT_8c64ce9c = *(undefined4 *)(&stack0x00000010 + iVar3);
          (*(code *)PTR_FUN_8c64cea0)();
          return 0xffffffff;
        }
        *(undefined2 *)(**(int **)((int)&uStack_4 + DAT_8c64ce8a + iVar3) + 0x42) = 0;
        iVar5 = **(int **)((int)&uStack_4 + DAT_8c64ce8a + iVar3);
        *(uint *)(iVar5 + 0x10) = *(uint *)(iVar5 + 0x10) | 2;
        iVar5 = **(int **)((int)&uStack_4 + DAT_8c64ce8a + iVar3);
        *(short *)(iVar5 + 0x40) = *(short *)(iVar5 + 0x40) + 1;
        if (*(int *)(&stack0x00000014 + iVar3) != 0) {
          (*(code *)PTR_FUN_8c64cea8)
                    (*(undefined4 *)(&stack0x00000010 + iVar3),2,
                     *(undefined4 *)(**(int **)((int)&uStack_4 + DAT_8c64ce8a + iVar3) + 0x14));
        }
      }
      else {
        iVar5 = **(int **)((int)&uStack_4 + DAT_8c64ce8a + iVar3);
        *(short *)(iVar5 + 0x40) = *(short *)(iVar5 + 0x40) + 1;
        if ((*(uint *)(**(int **)((int)&uStack_4 + DAT_8c64ce8a + iVar3) + 0x10) & 1) == 0) {
          iVar5 = (*(code *)PTR_FUN_8c64ce94)
                            (*(undefined4 *)((int)&uStack_4 + DAT_8c64ce8c + iVar3),
                             **(int **)((int)&uStack_4 + DAT_8c64ce8a + iVar3) + 0x14);
          puVar2 = DAT_8c64ce98;
          *(int *)((int)&uStack_4 + DAT_8c64ce8e + iVar3) = iVar5;
          if (iVar5 < 0) {
            *(short *)(**(int **)((int)&uStack_4 + DAT_8c64ce8a + iVar3) + 0x42) =
                 -*(short *)((int)&uStack_4 + DAT_8c64ce8e + iVar3);
            *puVar2 = *(undefined4 *)((int)&uStack_4 + DAT_8c64ce90 + iVar3);
            *DAT_8c64ce9c = *(undefined4 *)(&stack0x00000010 + iVar3);
            (*(code *)PTR_FUN_8c64cea0)();
            return 0xffffffff;
          }
          *(undefined2 *)(**(int **)((int)&uStack_4 + DAT_8c64ce8a + iVar3) + 0x42) = 0;
          iVar3 = **(int **)((int)&uStack_4 + DAT_8c64ce8a + iVar3);
          *(uint *)(iVar3 + 0x10) = *(uint *)(iVar3 + 0x10) | 1;
        }
      }
    }
    (*(code *)PTR_FUN_8c64cea0)();
  }
  else if (iVar5 == 2) {
    uVar4 = *(uint *)(*(int *)((int)&uStack_4 + DAT_8c64cf7c + iVar3) + 4) & DAT_8c64cf84;
    if (uVar4 == 0) {
      uVar6 = (*(code *)PTR_FUN_8c64cf90)(*(undefined4 *)(&stack0x00000010 + iVar3));
      puVar1 = PTR_FUN_8c64cf98;
      *DAT_8c64cf94 = uVar6;
      iVar5 = (*(code *)puVar1)(*(undefined4 *)((int)&uStack_4 + DAT_8c64cf7c + iVar3),uVar6);
      *(int *)((int)&uStack_4 + DAT_8c64cf7e + iVar3) = iVar5;
      if (iVar5 < 0) {
        *(undefined4 *)((int)&iStack_8 + iVar3) = *(undefined4 *)(&stack0x0000000c + iVar3);
        (*(code *)PTR_FUN_8c64cf9c)
                  (*(undefined4 *)((int)&iStack_8 + DAT_8c64cf80 + iVar3),*DAT_8c64cf94,
                   *(undefined4 *)(&stack0x00000010 + iVar3),0);
        *(short *)(**(int **)((int)&uStack_4 + DAT_8c64cf82 + iVar3) + 0x42) =
             -*(short *)((int)&uStack_4 + DAT_8c64cf7e + iVar3);
        *(undefined2 *)(**(int **)((int)&uStack_4 + DAT_8c64cf82 + iVar3) + 0x40) = 0;
        *DAT_8c64cfa0 = *(undefined4 *)((int)&uStack_4 + DAT_8c64cf80 + iVar3);
        *DAT_8c64cfa4 = *(undefined4 *)(&stack0x00000010 + iVar3);
        (*(code *)PTR_FUN_8c64cfa8)();
        return 0xffffffff;
      }
      *(undefined4 *)((int)&iStack_8 + iVar3) = *(undefined4 *)(&stack0x0000000c + iVar3);
      (*(code *)PTR_FUN_8c64cf9c)
                (*(undefined4 *)((int)&iStack_8 + DAT_8c64cf80 + iVar3),*DAT_8c64cf94,
                 *(undefined4 *)(&stack0x00000010 + iVar3),2);
      puVar1 = PTR_FUN_8c64cfac;
      *(undefined2 *)(**(int **)((int)&uStack_4 + DAT_8c64cf82 + iVar3) + 0x42) = 0;
      (*(code *)puVar1)(**(undefined4 **)((int)&uStack_4 + DAT_8c64cf82 + iVar3));
      if (*(int *)(&stack0x00000014 + iVar3) != 0) {
        (*(code *)PTR_FUN_8c64cfb0)
                  (**(undefined4 **)((int)&uStack_4 + DAT_8c64cf82 + iVar3),
                   *(undefined4 *)(&stack0x0000000c + iVar3));
      }
    }
    else if (uVar4 == DAT_8c64cf88) {
      uVar6 = (*(code *)PTR_FUN_8c64d0e0)(*(undefined4 *)(&stack0x00000010 + iVar3));
      puVar1 = PTR_FUN_8c64d0e8;
      *DAT_8c64d0e4 = uVar6;
      iVar5 = (*(code *)puVar1)(*(undefined4 *)((int)&uStack_4 + DAT_8c64d0d8 + iVar3),uVar6);
      *(int *)((int)&uStack_4 + DAT_8c64d0da + iVar3) = iVar5;
      if (iVar5 < 0) {
        *(undefined4 *)((int)&iStack_8 + iVar3) = *(undefined4 *)(&stack0x0000000c + iVar3);
        (*(code *)PTR_FUN_8c64d0ec)
                  (*(undefined4 *)((int)&iStack_8 + DAT_8c64d0dc + iVar3),*DAT_8c64d0e4,
                   *(undefined4 *)(&stack0x00000010 + iVar3),0);
        *(short *)(**(int **)((int)&uStack_4 + DAT_8c64d0de + iVar3) + 0x42) =
             -*(short *)((int)&uStack_4 + DAT_8c64d0da + iVar3);
        *DAT_8c64d0f0 = *(undefined4 *)((int)&uStack_4 + DAT_8c64d0dc + iVar3);
        *DAT_8c64d0f4 = *(undefined4 *)(&stack0x00000010 + iVar3);
        (*(code *)PTR_FUN_8c64d0f8)();
        return 0xffffffff;
      }
      *(undefined4 *)((int)&iStack_8 + iVar3) = *(undefined4 *)(&stack0x0000000c + iVar3);
      (*(code *)PTR_FUN_8c64d0ec)
                (*(undefined4 *)((int)&iStack_8 + DAT_8c64d0dc + iVar3),*DAT_8c64d0e4,
                 *(undefined4 *)(&stack0x00000010 + iVar3),1);
      *(undefined2 *)(**(int **)((int)&uStack_4 + DAT_8c64d0de + iVar3) + 0x42) = 0;
    }
    else if (uVar4 == DAT_8c64cf8c) {
      uVar6 = (*(code *)PTR_FUN_8c64d0e0)(*(undefined4 *)(&stack0x00000010 + iVar3));
      puVar1 = PTR_FUN_8c64d0fc;
      *DAT_8c64d0e4 = uVar6;
      iVar5 = (*(code *)puVar1)(*(undefined4 *)((int)&uStack_4 + DAT_8c64d0d8 + iVar3),uVar6);
      *(int *)((int)&uStack_4 + DAT_8c64d0da + iVar3) = iVar5;
      if (iVar5 < 0) {
        *(undefined4 *)((int)&iStack_8 + iVar3) = *(undefined4 *)(&stack0x0000000c + iVar3);
        (*(code *)PTR_FUN_8c64d0ec)
                  (*(undefined4 *)((int)&iStack_8 + DAT_8c64d0dc + iVar3),*DAT_8c64d0e4,
                   *(undefined4 *)(&stack0x00000010 + iVar3),0);
        *(short *)(**(int **)((int)&uStack_4 + DAT_8c64d0de + iVar3) + 0x42) =
             -*(short *)((int)&uStack_4 + DAT_8c64d0da + iVar3);
        *(undefined2 *)(**(int **)((int)&uStack_4 + DAT_8c64d0de + iVar3) + 0x40) = 0;
        *DAT_8c64d0f0 = *(undefined4 *)((int)&uStack_4 + DAT_8c64d0dc + iVar3);
        *DAT_8c64d0f4 = *(undefined4 *)(&stack0x00000010 + iVar3);
        (*(code *)PTR_FUN_8c64d0f8)();
        return 0xffffffff;
      }
      iVar5 = (*(code *)PTR_FUN_8c64d100)(0,*DAT_8c64d0e4);
      *(int *)((int)&uStack_4 + DAT_8c64d0da + iVar3) = iVar5;
      if (iVar5 == 0) {
        *(undefined4 *)((int)&iStack_8 + iVar3) = *(undefined4 *)(&stack0x0000000c + iVar3);
        (*(code *)PTR_FUN_8c64d0ec)
                  (*(undefined4 *)((int)&iStack_8 + DAT_8c64d0dc + iVar3),*DAT_8c64d0e4,
                   *(undefined4 *)(&stack0x00000010 + iVar3),2);
        *DAT_8c64d0f0 = *(undefined4 *)((int)&uStack_4 + DAT_8c64d0dc + iVar3);
        *DAT_8c64d0f4 = *(undefined4 *)(&stack0x00000010 + iVar3);
        return 0xffffffff;
      }
      *(undefined4 *)((int)&iStack_8 + iVar3) = *(undefined4 *)(&stack0x0000000c + iVar3);
      (*(code *)PTR_FUN_8c64d238)
                (*(undefined4 *)((int)&iStack_8 + DAT_8c64d22e + iVar3),*DAT_8c64d234,
                 *(undefined4 *)(&stack0x00000010 + iVar3),3);
      puVar1 = PTR_FUN_8c64d23c;
      *(undefined2 *)(**(int **)((int)&uStack_4 + DAT_8c64d230 + iVar3) + 0x42) = 0;
      (*(code *)puVar1)(**(undefined4 **)((int)&uStack_4 + DAT_8c64d230 + iVar3));
      if (*(int *)(&stack0x00000014 + iVar3) != 0) {
        (*(code *)PTR_FUN_8c64d240)
                  (**(undefined4 **)((int)&uStack_4 + DAT_8c64d230 + iVar3),
                   *(undefined4 *)(&stack0x0000000c + iVar3));
      }
    }
    (*(code *)PTR_FUN_8c64d244)();
  }
  return 1;
}

