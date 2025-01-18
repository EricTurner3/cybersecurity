import base64



def base64_to_original_key(base64_string, key):
    # Step 1: Decode the Base64 string
    binary_data = base64.b64decode(base64_string)
    
    # Step 2: XOR each byte with 0xbb to retrieve the original key
    original_key = bytes(byte ^ key for byte in binary_data)
    
    return original_key

base64_key = "<b64_key_here>"
xor_key = 0xFF
original_key = base64_to_original_key(base64_key, xor_key)


# Print the original key in hexadecimal format
print("Original Key (b64):", base64.b64encode(original_key))
print("Original Key (Hex):", original_key.hex())
print("# of bytes {}".format(len(original_key.hex())/2))
