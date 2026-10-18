/* Ghidra draft only; not buildable/verified source. */

void FUN_0c9044e2(int param_1,int *param_2)

{
  undefined4 uVar1;
  undefined **ppuVar2;
  
  ppuVar2 = &PTR_DAT_0c91c3d0;
  *param_2 = param_1;
  uVar1 = 1;
  if (*(int *)(param_1 + 0x2c) != 0) {
    uVar1 = FUN_0c9044a0(*(undefined4 *)(param_1 + 0x2c),param_2,1,&PTR_DAT_0c91c3d0);
  }
  if (*(int *)(param_1 + 0x30) != 0) {
    FUN_0c9044a0(*(undefined4 *)(param_1 + 0x30),param_2,uVar1,ppuVar2);
  }
  return;
}

