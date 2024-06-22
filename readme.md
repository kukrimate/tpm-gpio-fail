# TPM GPIO fail vulnerability

NOTE: this PoC isn't fully complete yet and only support some example platforms.

## Detection utility

See `detect` directory.

## Reset PoC

See `reset` directory, currently SPT-S/H, KBP-S/H, ADL-S/H, and RPL-S/H are supported.

1. Compile with `make`
2. Then run `tpm2_shutdown -c && ./inteltool && tpm2_startup -c` (needs tpm2-tools)
3. Check PCRs via sysfs.

Errors/warnings shown by tpm2-tools are likely normal and can be ignored as long as PCRs are clear in step 3.
