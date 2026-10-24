/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c6006c0(void)

{
  undefined *puVar1;
  int *piVar2;
  int iVar3;
  undefined4 uVar4;
  byte in_IMASK;
  byte extraout_S;
  
  puVar1 = PTR_DAT_8c600788;
  piVar2 = (int *)(*(code *)PTR_FUN_8c60078c)(*(undefined4 *)(*(int *)PTR_DAT_8c600788 + 0x14),0,0);
  if ((piVar2 == (int *)0x0) ||
     (iVar3 = (**(code **)(*(int *)(*(int *)puVar1 + 0xc) + 0x3c))
                        (1,0xe,*(int *)puVar1 + 0x40,
                         (((uint)extraout_S << 1 | (uint)in_IMASK << 4) & 0xf0) >> 4), iVar3 == 0))
  {
    uVar4 = 0xfffffff3;
  }
  else {
    piVar2[0xf] = iVar3;
    *(undefined2 *)(piVar2 + 0x14) = 4;
    *(undefined2 *)(piVar2 + 0x12) = 10;
    *(int **)(*piVar2 + 0x1c) = piVar2;
    (**(code **)(*(int *)(*(int *)puVar1 + 0xc) + 0x40))();
    uVar4 = 0;
  }
  return uVar4;
}

