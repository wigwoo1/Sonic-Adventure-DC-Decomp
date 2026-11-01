/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c64f074(int param_1)

{
  undefined *puVar1;
  undefined *puVar2;
  undefined *puVar3;
  undefined *puVar4;
  undefined *puVar5;
  undefined *puVar6;
  int iVar7;
  undefined4 uVar8;
  int local_30;
  undefined4 local_2c;
  undefined1 auStack_28 [8];
  undefined4 local_20;
  
  puVar5 = PTR_DAT_8c64f164;
  puVar4 = PTR_DAT_8c64f160;
  puVar3 = PTR_DAT_8c64f15c;
  puVar2 = PTR_FUN_8c64f158;
  puVar1 = PTR_FUN_8c64f150;
  if ((*(uint *)(param_1 + 4) & DAT_8c64f154) == 0) {
    if (*DAT_8c64f144 == 0) {
      do {
        puVar6 = PTR_FUN_8c64f168;
        iVar7 = (*(code *)PTR_FUN_8c64f168)(&local_30,4);
        if (iVar7 < 0) {
          return 0xffffffff;
        }
        if (local_30 == DAT_8c64f16c) {
          (*(code *)puVar1)(0xfffffffc,1);
          iVar7 = (*(code *)puVar6)(auStack_28,0xc);
          if (iVar7 < 0) {
            return 0xffffffff;
          }
        }
        else {
          iVar7 = (*(code *)puVar6)(&local_2c,4);
          if (iVar7 < 0) {
            return 0xffffffff;
          }
          (*(code *)puVar1)(local_2c,1);
        }
      } while ((local_30 != DAT_8c64f170) && (local_30 != DAT_8c64f16c));
      uVar8 = local_20;
      if (local_30 == DAT_8c64f16c) goto LAB_8c64f188;
      while (iVar7 = (*(code *)puVar2)(*(undefined4 *)puVar3,*(undefined4 *)puVar4,
                                       *(undefined4 *)puVar5), iVar7 != -1) {
        *(int *)puVar5 = *(int *)puVar5 + -1;
      }
    }
    else {
      while (iVar7 = (*(code *)puVar2)(*(undefined4 *)puVar3,*(undefined4 *)puVar4,
                                       *(undefined4 *)puVar5), iVar7 != -1) {
        *(int *)puVar5 = *(int *)puVar5 + -1;
      }
    }
    uVar8 = *(undefined4 *)puVar5;
  }
  else {
    uVar8 = *(undefined4 *)(param_1 + 8);
  }
LAB_8c64f188:
  (*(code *)puVar1)(0);
  return uVar8;
}

