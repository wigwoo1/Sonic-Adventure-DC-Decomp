/* Ghidra draft only; not buildable/verified source. */

void callback_8c041516(undefined4 param_1)

{
  ushort *puVar1;
  
  puVar1 = DAT_8c0415c4;
  (**(code **)(DAT_8c0415e8 + (uint)*DAT_8c0415c4 * 4))();
  (**(code **)(DAT_8c0415ec + (uint)*puVar1 * 4))(param_1);
                    /* WARNING: Could not recover jumptable at 0x8c04154a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(DAT_8c0415f0 + (uint)*puVar1 * 4))(param_1);
  return;
}

