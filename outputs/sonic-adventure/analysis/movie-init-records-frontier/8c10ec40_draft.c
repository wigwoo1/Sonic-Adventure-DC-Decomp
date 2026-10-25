/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c10ec40(undefined4 *param_1,int param_2)

{
  undefined *puVar1;
  int *piVar2;
  undefined4 uVar3;
  
  puVar1 = PTR_DAT_8c10ed4c;
  piVar2 = (int *)(param_2 * 0x34 + *(int *)PTR_DAT_8c10ed4c + 4);
  uVar3 = DAT_8c10ed50;
  if (piVar2[10] != 1) {
    *(int *)(*piVar2 * 0x34 + *(int *)PTR_DAT_8c10ed4c + 4) = *piVar2;
    *(undefined4 *)(*piVar2 * 0x34 + *(int *)puVar1 + 0x2c) = 0;
    *(undefined4 *)(*piVar2 * 0x34 + *(int *)puVar1 + 0x30) = 0;
    *(undefined4 *)(*piVar2 * 0x34 + *(int *)puVar1 + 0x34) = 0;
    *(undefined4 *)(*piVar2 * 0x34 + *(int *)puVar1 + 8) = 0;
    *(undefined4 *)(*piVar2 * 0x34 + *(int *)puVar1 + 0xc) = 0;
    *(undefined4 *)(*piVar2 * 0x34 + *(int *)puVar1 + 0x10) = 0;
    *(undefined4 *)(*piVar2 * 0x34 + *(int *)puVar1 + 0x14) = 0;
    *(undefined4 *)(*piVar2 * 0x34 + *(int *)puVar1 + 0x18) = 0;
    *(undefined4 *)(*piVar2 * 0x34 + *(int *)puVar1 + 0x1c) = 3;
    *(undefined4 *)(*piVar2 * 0x34 + *(int *)puVar1 + 0x20) = 0;
    *(undefined4 *)(*piVar2 * 0x34 + *(int *)puVar1 + 0x24) = 0;
    piVar2[10] = 1;
    *param_1 = piVar2;
    uVar3 = 0;
  }
  return uVar3;
}

