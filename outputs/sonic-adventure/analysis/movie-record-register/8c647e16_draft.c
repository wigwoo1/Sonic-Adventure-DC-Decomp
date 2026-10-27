/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c647e16(undefined4 param_1,uint param_2,int param_3)

{
  int *piVar1;
  undefined *puVar2;
  int *piVar3;
  undefined *puVar4;
  undefined4 *puVar5;
  undefined4 *puVar6;
  int iVar7;
  uint uVar8;
  
  puVar5 = DAT_8c647eb4;
  piVar1 = DAT_8c647e9c;
  puVar6 = DAT_8c647eb4 + 8;
  *DAT_8c647e9c = DAT_8c647eb0;
  do {
    *puVar5 = 0;
    puVar2 = PTR_FUN_8c647eb8;
    puVar5 = puVar5 + 1;
  } while (puVar5 < puVar6);
  (*(code *)PTR_FUN_8c647eb8)(DAT_8c647eb0,0,0x14);
  (*(code *)puVar2)(DAT_8c647ebc,0,(int)DAT_8c647e8c);
  FUN_8c647de8(0);
  piVar3 = DAT_8c647ec0;
  iVar7 = 0;
  uVar8 = 1;
  *DAT_8c647ec0 = 0;
  do {
    iVar7 = iVar7 + 1;
    if ((param_2 & uVar8) != 0) {
      *piVar3 = *piVar3 + 1;
    }
    uVar8 = uVar8 << 1;
  } while (iVar7 < 8);
  if (param_3 == 0) {
    FUN_8c647f10(param_1,param_2,0);
    *(undefined4 *)(*piVar1 + 8) = 1;
  }
  else {
    FUN_8c647f4c(param_1,param_2,param_3);
    *(undefined4 *)(*piVar1 + 8) = 0;
  }
  puVar4 = PTR_FUN_8c647fbc;
  puVar5 = DAT_8c647fb8;
  *DAT_8c647fb4 = 8;
  *puVar5 = 0;
  (*(code *)puVar4)(10);
  (*(code *)puVar2)(DAT_8c647fc0,0,*DAT_8c647fb4 * 0x48);
  (*(code *)PTR_FUN_8c647fc4)();
  return 0;
}

