/* Ghidra draft only; not buildable/verified source. */

void FUN_0c904d22(undefined4 param_1,int param_2)

{
  undefined4 uVar1;
  char cVar2;
  undefined4 *puVar3;
  undefined8 in_xd2;
  char in_FPSCR_SZ;
  
  *(undefined **)(param_2 + 0x7c4) = PTR_PTR_0c904e68;
  cVar2 = **(char **)(param_2 + 0xec);
  if (cVar2 == '\a') {
    cVar2 = '\0';
  }
  puVar3 = (undefined4 *)(PTR_PTR_0c904e6c + cVar2 * 0x18);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(param_2 + 0xd4) = 0;
  }
  else {
    *(undefined8 *)(param_2 + 0xd4) = in_xd2;
  }
  uVar1 = FUN_0c90451c(*puVar3,0);
  *(undefined4 *)(param_2 + 0x248) = uVar1;
  FUN_0c9044e2(uVar1,param_2 + 0x24c);
  FUN_0c9044e2(*puVar3,param_2 + 0x314);
  FUN_0c9044e2(puVar3[1],param_2 + 0x3dc);
  FUN_0c9044e2(puVar3[2],param_2 + 0x6fc);
  FUN_0c9044e2(puVar3[3],param_2 + 0x4a4);
  FUN_0c9044e2(puVar3[4],param_2 + 0x56c);
  FUN_0c9044e2(puVar3[5],param_2 + 0x634);
  if ((*(ushort *)(*(int *)(param_2 + 0xec) + 0x36) & 0x40) != 0) {
    FUN_0c904aea(param_1,param_2);
  }
  if ((*(ushort *)(*(int *)(param_2 + 0xec) + 0x36) & 4) != 0) {
    FUN_0c904bbc(param_1,param_2);
    return;
  }
  if ((*(ushort *)(*(int *)(param_2 + 0xec) + 0x36) & 8) != 0) {
    FUN_0c904c80(param_1,param_2);
    return;
  }
  return;
}

