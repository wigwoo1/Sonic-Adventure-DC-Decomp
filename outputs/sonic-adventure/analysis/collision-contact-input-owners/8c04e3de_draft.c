/* Ghidra draft only; not buildable/verified source. */

void FUN_8c04e3de(void)

{
  undefined *puVar1;
  undefined1 *puVar2;
  undefined *puVar3;
  undefined4 uVar4;
  undefined4 *puVar5;
  
  if (*(int *)PTR_DAT_8c04e440 == 1) {
    (*(code *)PTR_FUN_8c04e448)(DAT_8c04e444);
  }
  puVar3 = PTR_DAT_8c04e458;
  puVar5 = (undefined4 *)PTR_DAT_8c04e454;
  puVar2 = DAT_8c04e450;
  *(undefined2 *)PTR_DAT_8c04e44c = 0;
  *puVar2 = 0xff;
  puVar1 = PTR_FUN_8c04e448;
  if (*(int *)puVar3 != 0) {
    *puVar5 = 1;
    (*(code *)puVar1)(0);
    puVar5 = (undefined4 *)PTR_DAT_8c04e45c;
    if (*(int *)PTR_DAT_8c04e458 != 1) {
      *(undefined4 *)PTR_DAT_8c04e45c = 1;
      goto LAB_8c04e464;
    }
  }
  *puVar5 = 0;
LAB_8c04e464:
  puVar5 = DAT_8c04e5b0;
  *(undefined4 *)PTR_DAT_8c04e5a4 = DAT_8c04e5a0;
  uVar4 = DAT_8c04e5ac;
  *DAT_8c04e5a8 = 0;
  *puVar5 = uVar4;
  (*(code *)PTR_FUN_8c04e5b4)(3);
  (*(code *)PTR_FUN_8c04e5b8)();
  (*(code *)PTR_FUN_8c04e5bc)();
  *PTR_DAT_8c04e5c0 = 0;
  return;
}

