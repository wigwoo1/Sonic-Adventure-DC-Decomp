/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c64e08e) */

undefined4 FUN_8c64e054(void)

{
  undefined *puVar1;
  undefined *puVar2;
  undefined *puVar3;
  int iVar4;
  
  iVar4 = (*(code *)PTR_FUN_8c64e130)(0);
  *DAT_8c64e134 = iVar4;
  if ((iVar4 != 0) &&
     (iVar4 = (*(code *)PTR_FUN_8c64e13c)(*DAT_8c64e134,PTR_DAT_8c64e138), puVar3 = PTR_DAT_8c64e150
     , puVar1 = PTR_DAT_8c64e148, iVar4 != 0)) {
    *(int *)PTR_DAT_8c64e144 = *(int *)PTR_DAT_8c64e138 + (int)DAT_8c64e12e >> 0xb;
    puVar2 = PTR_DAT_8c64e14c;
    *(undefined4 *)puVar1 = 0;
    *(undefined4 *)puVar2 = 0;
    puVar2 = PTR_DAT_8c64e158;
    puVar1 = PTR_DAT_8c64e154;
    *(undefined4 *)puVar3 = 0;
    *(undefined4 *)puVar1 = 0;
    *(undefined4 *)puVar2 = 0;
    return 0;
  }
  *DAT_8c64e140 = 2;
  return 0xfffffffe;
}

