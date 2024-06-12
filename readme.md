# TPM GPIO fail vulnerability

WIP

## How to use PoC

Currently Skylake/Kabylake, and Alderlake/Raptorlake have PoCs. (See `poc` directory.)

1. Compile with `make`
2. Then run `tpm2_shutdown -c && ./inteltool && tpm2_startup -c` (needs tpm2-tools)
