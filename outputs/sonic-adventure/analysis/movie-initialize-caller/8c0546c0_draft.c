/* Ghidra draft only; not buildable/verified source. */

void FUN_8c0546c0(void)

{
  undefined *puVar1;
  undefined4 *puVar2;
  short sVar3;
  int iVar4;
  undefined4 uVar5;
  int local_10 [2];
  
  uVar5 = DAT_8c0547e8;
  *DAT_8c0547e0 = 0;
  puVar1 = PTR_FUN_8c0547ec;
  *DAT_8c0547e4 = 0;
  (*(code *)puVar1)(uVar5);
  iVar4 = (*(code *)PTR_FUN_8c0547f0)();
  puVar1 = PTR_FUN_8c0547fc;
  puVar2 = DAT_8c0547f8;
  *DAT_8c0547f4 = iVar4;
  if (iVar4 == 1) {
    (*(code *)puVar1)(1,0);
    *puVar2 = 2;
  }
  else {
    sVar3 = (*(code *)PTR_FUN_8c054800)();
    if (((short)(sVar3 + -1) == 1) || (((short)(sVar3 + -1) < 0 && (*DAT_8c0547f4 == 2)))) {
      (*(code *)puVar1)(0x3a,0,1);
      uVar5 = 1;
    }
    else {
      (*(code *)puVar1)(0x38,0,1);
      uVar5 = 0;
    }
    *puVar2 = uVar5;
  }
  (*(code *)PTR_FUN_8c054804)(0);
  (*(code *)PTR_FUN_8c054808)(2);
  (*(code *)PTR_FUN_8c05480c)();
  (*(code *)PTR_FUN_8c054818)(DAT_8c054814,DAT_8c054810);
  (*(code *)PTR_FUN_8c054824)(0xffffffff,0,DAT_8c054820,DAT_8c05481c);
  (*(code *)PTR_FUN_8c054828)(0,0);
  (*(code *)PTR_FUN_8c05482c)(0,0);
  (*(code *)PTR_FUN_8c054830)(DAT_8c054814,0x1000);
  (*(code *)PTR_FUN_8c054838)(DAT_8c054834);
  iVar4 = (*(code *)PTR_FUN_8c054840)(DAT_8c05483c);
  puVar1 = PTR_FUN_8c054848;
  if (iVar4 != 0) goto LAB_8c05486e;
  *DAT_8c054844 = 1;
  iVar4 = (*(code *)puVar1)(local_10);
  puVar2 = DAT_8c05484c;
  if (iVar4 == 0) {
    if (local_10[0] == 0) {
      *DAT_8c054844 = 0;
      *puVar2 = 0;
    }
    else if (local_10[0] == 1) {
LAB_8c054866:
      *DAT_8c05484c = 1;
    }
    else {
      if (local_10[0] == 2) {
        uVar5 = 4;
      }
      else {
        if (local_10[0] == 3) {
          *DAT_8c05484c = 2;
          goto LAB_8c054868;
        }
        if (local_10[0] != 4) goto LAB_8c054866;
        uVar5 = 3;
      }
      *DAT_8c05484c = uVar5;
    }
  }
LAB_8c054868:
  (*(code *)PTR_FUN_8c0548c0)();
LAB_8c05486e:
  FUN_8c0544e2();
  (*(code *)PTR_FUN_8c0548c4)();
  (*(code *)PTR_FUN_8c0548c8)();
  puVar1 = PTR_thunk_FUN_8c601f5a_8c0548d0;
  puVar2 = DAT_8c0548cc;
  uVar5 = (*(code *)PTR_thunk_FUN_8c601f5a_8c0548d0)(0);
  *puVar2 = uVar5;
  uVar5 = (*(code *)puVar1)(6);
  puVar2[1] = uVar5;
  uVar5 = (*(code *)puVar1)(0xc);
  puVar2[2] = uVar5;
  uVar5 = (*(code *)puVar1)(0x12);
  puVar2[3] = uVar5;
  return;
}

