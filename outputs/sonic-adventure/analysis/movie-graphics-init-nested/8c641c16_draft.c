/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c641c16(int param_1,int param_2,int param_3,undefined4 param_4)

{
  int iVar1;
  int *piVar2;
  undefined *puVar3;
  undefined4 *puVar4;
  int iVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  
  puVar3 = PTR_FUN_8c641ca8;
  iVar1 = DAT_8c641ca0;
  uVar7 = 0;
  if ((param_1 == 0) || (param_2 == 0)) {
    uVar7 = 0xe;
  }
  else {
    *(int *)(DAT_8c641ca0 + 0x3c) = param_1;
    *(int *)(iVar1 + 0x40) = param_2;
    *(undefined4 *)(iVar1 + 0x44) = param_4;
    piVar2 = DAT_8c641ca4;
    *(int *)(iVar1 + 0x18) = param_3;
    *piVar2 = param_3;
    iVar5 = (*(code *)puVar3)(0);
    if (iVar5 == DAT_8c641cac) {
      if (*(int *)(param_1 + 0x18) == *(int *)(param_2 + 0x18)) {
        if (*(int *)(param_1 + 0x14) == *(int *)(param_2 + 0x14)) {
          *DAT_8c641d60 = *(undefined4 *)(param_1 + 0x1c);
          puVar4 = DAT_8c641d64;
          *DAT_8c641d64 = 0;
          *DAT_8c641d68 = *(undefined4 *)(param_2 + 0x1c);
          *DAT_8c641d6c = 0;
          puVar3 = PTR_FUN_8c641d70;
          (*(code *)PTR_FUN_8c641d70)(0x50,puVar4[-0x1b]);
          (*(code *)puVar3)(0x54,*DAT_8c641d74);
          (*(code *)puVar3)(0x60,*DAT_8c641d60);
          (*(code *)puVar3)(100,*DAT_8c641d68);
          puVar4 = DAT_8c641d60;
          *DAT_8c641d74 = *DAT_8c641d60;
          puVar4[-0x18] = *DAT_8c641d68;
          if (param_3 == 1) {
            *DAT_8c641d78 = 0;
            *DAT_8c641d7c = 0;
            (*(code *)puVar3)(0x50,*DAT_8c641d74);
            uVar6 = *DAT_8c641d80;
          }
          else {
            (*(code *)puVar3)(0x50,*DAT_8c641d74);
            uVar6 = *DAT_8c641d74;
          }
          (*(code *)puVar3)(0x54,uVar6);
          *DAT_8c641d84 = 1;
          *DAT_8c641d88 = param_4;
        }
        else {
          uVar7 = 0x10;
        }
      }
      else {
        uVar7 = 0xf;
      }
    }
    else {
      iVar5 = (*(code *)PTR_FUN_8c641de4)((int)DAT_8c641de0);
      if (iVar5 == DAT_8c641de2) {
        (*(code *)PTR_FUN_8c641dec)(DAT_8c641de8);
        uVar7 = (*(code *)PTR_FUN_8c641df0)(*(undefined4 *)(iVar1 + 0x54));
      }
    }
  }
  return uVar7;
}

