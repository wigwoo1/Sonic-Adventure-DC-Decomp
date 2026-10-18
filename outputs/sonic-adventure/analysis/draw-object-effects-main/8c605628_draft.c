/* Ghidra draft only; not buildable/verified source. */

void FUN_8c605628(int param_1,undefined4 param_2)

{
  int *piVar1;
  
  piVar1 = DAT_8c605730;
  switch(param_2) {
  case 0:
    param_2 = 0;
    break;
  case 1:
    param_2 = 1;
    break;
  case 2:
    param_2 = 2;
    break;
  case 3:
    param_2 = 3;
    break;
  case 4:
    param_2 = 4;
    break;
  case 5:
    param_2 = 5;
    break;
  case 6:
    param_2 = 6;
    break;
  case 7:
    param_2 = 7;
    break;
  case 8:
    param_2 = 8;
    break;
  case 9:
    param_2 = 9;
    break;
  case 10:
    param_2 = 10;
    break;
  case 0xb:
    param_2 = 0xb;
  }
  if (param_1 == 0) {
    *(undefined4 *)*DAT_8c605730 = 0x40;
    *(undefined4 *)(*piVar1 + 0x2c) = param_2;
  }
  if (param_1 == 1) {
    *(int *)*piVar1 = (int)DAT_8c60572a;
    *(undefined4 *)(*piVar1 + 0x30) = param_2;
  }
  (*(code *)PTR_FUN_8c605734)(*piVar1);
  (*(code *)PTR_FUN_8c605738)(*piVar1);
  return;
}

