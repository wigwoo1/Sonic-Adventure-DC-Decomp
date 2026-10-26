/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c65eb54) */
/* WARNING: Removing unreachable block (ram,0x8c65eb50) */
/* WARNING: Removing unreachable block (ram,0x8c65eb4c) */
/* WARNING: Removing unreachable block (ram,0x8c65eb48) */
/* WARNING: Removing unreachable block (ram,0x8c65eb44) */
/* WARNING: Removing unreachable block (ram,0x8c65eb40) */
/* WARNING: Removing unreachable block (ram,0x8c65eb3c) */
/* WARNING: Removing unreachable block (ram,0x8c65eb38) */
/* WARNING: Removing unreachable block (ram,0x8c65eb30) */
/* WARNING: Removing unreachable block (ram,0x8c65eb2c) */
/* WARNING: Removing unreachable block (ram,0x8c65eb28) */
/* WARNING: Removing unreachable block (ram,0x8c65eb24) */
/* WARNING: Removing unreachable block (ram,0x8c65eb20) */
/* WARNING: Removing unreachable block (ram,0x8c65eb1c) */
/* WARNING: Removing unreachable block (ram,0x8c65eb18) */
/* WARNING: Removing unreachable block (ram,0x8c65eb16) */
/* WARNING: Removing unreachable block (ram,0x8c65eb1a) */
/* WARNING: Removing unreachable block (ram,0x8c65eb1e) */
/* WARNING: Removing unreachable block (ram,0x8c65eb22) */
/* WARNING: Removing unreachable block (ram,0x8c65eb26) */
/* WARNING: Removing unreachable block (ram,0x8c65eb2a) */
/* WARNING: Removing unreachable block (ram,0x8c65eb2e) */
/* WARNING: Removing unreachable block (ram,0x8c65eb32) */
/* WARNING: Removing unreachable block (ram,0x8c65eb34) */
/* WARNING: Removing unreachable block (ram,0x8c65eb3a) */
/* WARNING: Removing unreachable block (ram,0x8c65eb3e) */
/* WARNING: Removing unreachable block (ram,0x8c65eb42) */
/* WARNING: Removing unreachable block (ram,0x8c65eb46) */
/* WARNING: Removing unreachable block (ram,0x8c65eb4a) */
/* WARNING: Removing unreachable block (ram,0x8c65eb4e) */
/* WARNING: Removing unreachable block (ram,0x8c65eb52) */
/* WARNING: Removing unreachable block (ram,0x8c65eb56) */

void callback_8c65eaec(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                      undefined4 param_5,undefined4 param_6,undefined4 param_7,undefined4 param_8,
                      undefined4 param_9,undefined4 param_10,undefined4 param_11,undefined4 param_12
                      ,undefined4 param_13)

{
  int iVar1;
  undefined1 *puVar2;
  bool bVar3;
  
  bVar3 = false;
  if (*(int *)PTR_DAT_8c65ebac != 0) {
    iVar1 = *(int *)PTR_DAT_8c65eba4;
    *(int *)PTR_DAT_8c65eba4 = iVar1 + -1;
    bVar3 = iVar1 + -1 == 0;
  }
  puVar2 = &stack0x000000e4;
  if (bVar3) {
    puVar2 = *(undefined1 **)PTR_DAT_8c65eba8;
  }
  *(undefined4 *)(puVar2 + -4) = param_9;
  (*(code *)&LAB_ac000010)
            (CONCAT44(param_1,param_2),CONCAT44(param_3,param_4),CONCAT44(param_5,param_6),
             CONCAT44(param_7,param_8),param_10,param_11,param_12,param_13);
  return;
}

