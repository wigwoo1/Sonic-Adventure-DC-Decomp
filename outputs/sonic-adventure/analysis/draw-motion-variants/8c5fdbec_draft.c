/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void callback_8c5fdbec(ulonglong param_1,uint *param_2)

{
  undefined2 *puVar1;
  undefined2 *puVar2;
  undefined4 *puVar3;
  undefined8 uVar4;
  uint uVar5;
  undefined8 uVar6;
  char in_FPSCR_SZ;
  
  while( true ) {
    (*_DAT_8c5fddd0)(0);
    puVar2 = _DAT_8c5fdddc;
    puVar1 = _DAT_8c5fddd8;
    if ((*param_2 & 0x40) == 0) {
      *_DAT_8c5fddd4 = 0;
      *puVar1 = 0;
      puVar3 = _DAT_8c5fdde4;
      puVar1 = _DAT_8c5fdde0;
      *puVar2 = 0;
      *puVar1 = 0;
      (*(code *)*puVar3)(param_2);
      puVar3 = _DAT_8c5fdde8;
      *_DAT_8c5fdde0 = 1;
      (*(code *)*puVar3)(param_2);
      (*(code *)PTR_LAB_8c5fddec)();
    }
    else {
      if (in_FPSCR_SZ == '\0') {
        uVar6 = CONCAT44(param_2[4],(int)param_1);
        uVar4 = CONCAT44(param_2[2],param_2[3]);
      }
      else {
        uVar6 = *(undefined8 *)(param_2 + 4);
        uVar4 = *(undefined8 *)(param_2 + 2);
      }
      (*(code *)PTR_FUN_8c5fddf0)(uVar4,uVar6,0);
      uVar5 = (uint)uVar6;
      if ((*param_2 & 0x20) == 0) {
        (*(code *)PTR_FUN_8c5fddf8)(0,param_2[5],param_2[6],param_2[7]);
      }
      else {
        (*(code *)PTR_FUN_8c5fddf4)(0,param_2[5],param_2[6],param_2[7]);
      }
      if (in_FPSCR_SZ == '\0') {
        param_1 = (ulonglong)uVar5;
      }
      else {
        param_1 = *(ulonglong *)(param_2 + 10);
      }
      (*(code *)PTR_FUN_8c5fddfc)(0);
    }
    if ((code *)*_DAT_8c5fde00 != (code *)0x0) {
      (*(code *)*_DAT_8c5fde00)(param_2);
    }
    if ((param_2[1] != 0) && ((*param_2 & 8) == 0)) {
      (*_DAT_8c5fde04)(param_2[1]);
    }
    if (param_2[0xb] != 0) {
      callback_8c5fdbec(param_2[0xb]);
    }
    (*_DAT_8c5fde08)(1);
    if (param_2[0xc] == 0) break;
    param_2 = (uint *)param_2[0xc];
  }
  return;
}

