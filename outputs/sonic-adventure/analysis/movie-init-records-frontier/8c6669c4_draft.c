/* Ghidra draft only; not buildable/verified source. */

int FUN_8c6669c4(undefined4 param_1,uint param_2)

{
  undefined *puVar1;
  undefined *puVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  
  puVar1 = PTR_DAT_8c666a48;
  iVar3 = DAT_8c666a4c;
  if (*(int *)PTR_DAT_8c666a48 != 0) {
    iVar3 = (int)DAT_8c666a46;
    *(undefined4 *)(iVar3 + *(int *)PTR_DAT_8c666a48) = param_1;
    puVar2 = PTR_FUN_8c666a54;
    *(uint *)(iVar3 + 4 + *(int *)puVar1) = param_2;
    *(undefined4 *)(iVar3 + 0x10 + *(int *)puVar1) = 0;
    *(undefined4 *)(iVar3 + 0xc + *(int *)puVar1) = 0;
    *DAT_8c666a50 = param_1;
    (*(code *)puVar2)(0,param_2 * 0x1c);
    uVar5 = 0;
    iVar4 = 0;
    iVar3 = 0;
    if (param_2 != 0) {
      do {
        (*(code *)PTR_FUN_8c666a5c)(iVar4);
        uVar5 = uVar5 + 1;
        iVar4 = iVar4 + 0x1c;
      } while (uVar5 < param_2);
    }
  }
  if (iVar3 != 0) {
    (*(code *)PTR_FUN_8c666a64)(PTR_FUN_8c666a60,iVar3);
  }
  return iVar3;
}

