/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c66018e(uint *param_1,int param_2)

{
  undefined1 uVar1;
  byte bVar2;
  undefined1 *puVar3;
  byte *pbVar4;
  uint *puVar5;
  
  puVar3 = (undefined1 *)(param_2 + 8);
  *param_1 = (uint)*(byte *)(param_2 + 5) << 0x10 & DAT_8c660234 |
             (uint)*(byte *)(param_2 + 6) << 8 & DAT_8c660238 |
             (uint)*(byte *)(param_2 + 4) << 0x18 & DAT_8c660234 << 8 | (uint)*(byte *)(param_2 + 7)
  ;
  for (puVar5 = param_1 + 1; puVar5 < param_1 + 2; puVar5 = (uint *)((int)puVar5 + 1)) {
    uVar1 = *puVar3;
    puVar3 = puVar3 + 1;
    *(undefined1 *)puVar5 = uVar1;
  }
  for (puVar5 = param_1 + 2; puVar5 < param_1 + 3; puVar5 = (uint *)((int)puVar5 + 1)) {
    uVar1 = *puVar3;
    puVar3 = puVar3 + 1;
    *(undefined1 *)puVar5 = uVar1;
  }
  for (puVar5 = param_1 + 3; puVar5 < param_1 + 4; puVar5 = (uint *)((int)puVar5 + 1)) {
    uVar1 = *puVar3;
    puVar3 = puVar3 + 1;
    *(undefined1 *)puVar5 = uVar1;
  }
  pbVar4 = puVar3 + 1;
  *(undefined1 *)((int)param_1 + 0x11) = *puVar3;
  if ((char)param_1[4] == '\0') {
    *(undefined *)((int)param_1 + 0x12) = PTR_DAT_8c660370[*pbVar4 & 0xf];
    *(undefined1 *)((int)param_1 + 0x13) = 0;
  }
  else {
    *(byte *)((int)param_1 + 0x12) = *pbVar4 & 3;
    *(byte *)((int)param_1 + 0x13) = *pbVar4 >> 2 & 3;
  }
  if (*(int *)(*DAT_8c660374 + (int)DAT_8c66036c) == 1) {
    for (puVar5 = param_1 + 5; puVar5 < (uint *)((int)param_1 + 0x33);
        puVar5 = (uint *)((int)puVar5 + 1)) {
      bVar2 = *pbVar4;
      pbVar4 = pbVar4 + 1;
      *(byte *)puVar5 = bVar2;
    }
  }
  else {
    for (puVar5 = param_1 + 5; pbVar4 = pbVar4 + 1, puVar5 < (uint *)((int)param_1 + 0x32);
        puVar5 = (uint *)((int)puVar5 + 1)) {
      *(byte *)puVar5 = *pbVar4;
    }
    *(undefined1 *)((int)param_1 + 0x32) = 0;
  }
  *(undefined1 *)((int)param_1 + 0x33) = 0;
  for (puVar5 = param_1 + 0xd; puVar5 < param_1 + 0x1c; puVar5 = (uint *)((int)puVar5 + 1)) {
    bVar2 = *pbVar4;
    pbVar4 = pbVar4 + 1;
    *(byte *)puVar5 = bVar2;
  }
  *(undefined1 *)(param_1 + 0x1c) = 0;
  *(undefined2 *)(param_1 + 0x1d) = *(undefined2 *)pbVar4;
  *(undefined2 *)((int)param_1 + 0x76) = *(undefined2 *)(pbVar4 + 2);
  return 0;
}

