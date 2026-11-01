/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c660f44(uint *param_1,uint *param_2,undefined4 *param_3,undefined4 *param_4)

{
  undefined *puVar1;
  int iVar2;
  uint uVar3;
  int local_2c;
  undefined4 local_28;
  uint *local_24;
  uint *local_20;
  undefined4 *local_1c;
  undefined4 *local_18;
  undefined4 local_14;
  undefined4 local_10;
  ushort uStack_c;
  ushort uStack_a;
  
  local_24 = param_1;
  local_20 = param_2;
  local_1c = param_3;
  local_18 = param_4;
  do {
    puVar1 = PTR_FUN_8c661038;
    iVar2 = (*(code *)PTR_FUN_8c661038)(&local_2c,4);
    if (iVar2 < 0) {
      return 0xffffffff;
    }
    if (local_2c == DAT_8c66103c) {
      iVar2 = (*(code *)puVar1)(&local_14,0xc);
      if (iVar2 < 0) {
        return 0xffffffff;
      }
    }
    else {
      iVar2 = (*(code *)puVar1)(&local_28,4);
      if (iVar2 < 0) {
        return 0xffffffff;
      }
      (*(code *)PTR_FUN_8c661040)(local_28,1);
    }
    iVar2 = DAT_8c661044;
  } while (local_2c != DAT_8c66103c);
  *local_18 = local_14;
  *local_24 = (uint)uStack_c;
  uVar3 = *local_24;
  if ((((uVar3 == 8) || (uVar3 == 0x10)) || (uVar3 == 0x20)) ||
     (((uVar3 == 0x40 || (uVar3 == (int)DAT_8c66102e)) ||
      ((uVar3 == (int)DAT_8c661030 || ((uVar3 == (int)DAT_8c661032 || (uVar3 == (int)DAT_8c661034)))
       ))))) {
    *local_20 = (uint)uStack_a;
    uVar3 = *local_20;
    if ((uVar3 == 8) ||
       (((((uVar3 == 0x10 || (uVar3 == 0x20)) || (uVar3 == 0x40)) ||
         ((uVar3 == (int)DAT_8c66102e || (uVar3 == (int)DAT_8c661030)))) ||
        ((uVar3 == (int)DAT_8c661032 || (uVar3 == (int)DAT_8c661034)))))) {
      *local_1c = local_10;
      return 1;
    }
  }
  *(undefined4 *)(iVar2 + 8) = 4;
  return 0xfffffffc;
}

