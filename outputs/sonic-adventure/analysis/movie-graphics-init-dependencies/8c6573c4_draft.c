/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c6573f2) */
/* WARNING: Removing unreachable block (ram,0x8c657402) */

void FUN_8c6573c4(uint param_1)

{
  int iVar1;
  int local_10 [4];
  
  local_10[0] = *(int *)PTR_DAT_8c65746c;
  local_10[1] = *(undefined4 *)(PTR_DAT_8c65746c + 4);
  local_10[2] = *(undefined4 *)(PTR_DAT_8c65746c + 8);
  local_10[3] = *(undefined4 *)(PTR_DAT_8c65746c + 0xc);
  iVar1 = *DAT_8c657474 * *DAT_8c657470;
  if ((*DAT_8c657478 & 0x20) != 0) {
    if (*DAT_8c65747c == 0) {
      iVar1 = iVar1 * 2;
    }
    else {
      iVar1 = iVar1 * 4;
    }
  }
  *DAT_8c657600 =
       ((local_10[param_1 >> 0x11 & 3] + local_10[param_1 >> 0x13 & 3] +
         local_10[param_1 >> 0x15 & 3] + local_10[param_1 >> 0x17 & 3] +
        local_10[param_1 >> 0x19 & 3]) * 4 + 0x18) * iVar1;
  return;
}

