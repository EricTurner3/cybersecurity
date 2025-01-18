# https://signify.readthedocs.io/en/stable/authenticode.html
import sys
import signify # pip install signify
# pass the file name as an cli arg
with open(sys.argv[1], "rb") as f:
    pefile = SignedPEFile(f)
    for signed_data in pefile.signed_datas:
        print(signed_data.signer_info.program_name)
        if signed_data.signer_info.countersigner is not None:
            print(signed_data.signer_info.countersigner.signing_time)