/* Ghidra draft only; not buildable/verified source. */

void callback_8c60b4a8(undefined4 param_1)

{
  ushort *puVar1;
  
  puVar1 = DAT_8c60b4fc;
  (**(code **)(DAT_8c60b50c + (uint)*DAT_8c60b4fc * 4))();
  (**(code **)(DAT_8c60b510 + (uint)*puVar1 * 4))(param_1);
                    /* WARNING: Could not recover jumptable at 0x8c60b4dc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(DAT_8c60b514 + (uint)*puVar1 * 4))(param_1);
  return;
}

