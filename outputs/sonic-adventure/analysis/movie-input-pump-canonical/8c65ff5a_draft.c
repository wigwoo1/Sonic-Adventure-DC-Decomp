/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c65ff5a(int param_1,undefined4 *param_2)

{
  undefined4 uVar1;
  undefined1 local_4;
  undefined1 uStack_3;
  undefined1 uStack_2;
  undefined1 uStack_1;
  
  uVar1 = *param_2;
  local_4 = (undefined1)uVar1;
  uStack_3 = (undefined1)((uint)uVar1 >> 8);
  uStack_2 = (undefined1)((uint)uVar1 >> 0x10);
  uStack_1 = (undefined1)((uint)uVar1 >> 0x18);
  if (*(int *)(*DAT_8c660054 + (int)DAT_8c66004c) == 1) {
    *(undefined1 *)(param_1 + 0xb) = local_4;
    *(undefined1 *)(param_1 + 10) = uStack_3;
    *(undefined1 *)(param_1 + 9) = uStack_2;
    *(undefined1 *)(param_1 + 8) = uStack_1;
  }
  else {
    *(undefined1 *)(param_1 + 8) = local_4;
    *(undefined1 *)(param_1 + 9) = uStack_3;
    *(undefined1 *)(param_1 + 10) = uStack_2;
    *(undefined1 *)(param_1 + 0xb) = uStack_1;
  }
  if (*(byte *)(param_1 + 8) == DAT_8c66004e) {
    *(undefined4 *)(param_1 + 0x14) = 0xffffffff;
  }
  else if ((short)(ushort)*(byte *)(param_1 + 8) < DAT_8c660050) {
    *(undefined4 *)(param_1 + 0x14) = 0;
  }
  else {
    *(undefined4 *)(param_1 + 0x14) = param_2[1];
  }
  return 0;
}

