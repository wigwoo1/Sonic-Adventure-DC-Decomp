/* Ghidra draft only; not buildable/verified source. */

void FUN_8c64acda(byte *param_1,int param_2)

{
  byte bVar1;
  int *piVar2;
  undefined2 uVar3;
  uint uVar4;
  byte *pbVar5;
  byte *pbVar6;
  
  pbVar5 = (byte *)(param_2 + 0x10);
  *(uint *)(param_2 + 4) = (uint)*param_1;
  pbVar6 = param_1 + 4;
  do {
    bVar1 = *pbVar6;
    pbVar6 = pbVar6 + 1;
    *pbVar5 = bVar1;
    pbVar5 = pbVar5 + 1;
  } while (pbVar5 < (byte *)(param_2 + 0x1c));
  pbVar5 = (byte *)(param_2 + 0x20);
  *(undefined1 *)(param_2 + 0x1c) = 0;
  pbVar6 = param_1 + 0x10;
  do {
    bVar1 = *pbVar6;
    pbVar6 = pbVar6 + 1;
    *pbVar5 = bVar1;
    piVar2 = DAT_8c64ada4;
    pbVar5 = pbVar5 + 1;
  } while (pbVar5 < (byte *)(param_2 + 0x28));
  uVar4 = (**(code **)(*DAT_8c64ada4 + 0xc))(param_1,2,1);
  *(uint *)(param_2 + 0xc) = uVar4 & 0xffff;
  uVar4 = (**(code **)(*piVar2 + 0xc))(param_1,0x18);
  *(uint *)(param_2 + 0x28) = uVar4 & 0xffff;
  *(byte *)(param_2 + 10) = param_1[1];
  uVar3 = (**(code **)(*piVar2 + 0xc))(param_1,0x1a);
  *(undefined2 *)(param_2 + 8) = uVar3;
  return;
}

