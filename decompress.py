#        static public int getInt32(byte[] buffer, int pos) { //(int addr)
#            return buffer[pos++] | (buffer[pos++] << 8) | buffer[pos++] << 16 | (buffer[pos] << 24);
def read_int32(buf, pos):
    return buf[pos] | (buf[pos+1] << 8) | (buf[pos+2] << 16) | buf[pos+3] << 24

def read_int16(buf, pos):
    return (buf[pos] | buf[pos+1] << 8)

def read_byte(buf, pos):
    return buf[pos]

def decompress_one(src, i, info):
    asmpchar, asmptext, char_data, char_ptrs = info

    offset = (i >> 8) << 3
    text_tree   = read_int32(src,   asmptext+offset) - 0x08000000
    text_offset = read_int32(src, asmptext+offset+4) - 0x08000000
    srcInd = i & 0xff

    while srcInd >= 0:
        srcInd -= 1

        c_len = 0xff
        while c_len == 0xff:
            c_len = read_byte(src, text_offset)
            text_offset += 1
            text_tree += c_len

    text_tree <<= 3

    init_char = 0

    done = False
    out = bytearray()

    while not done:
        char_tree = (char_data + read_int16(src, char_ptrs + (init_char << 1))) << 3
        char_slot = char_tree - 12

        def char_flag():
            nonlocal char_tree
            b = read_byte(src, char_tree >> 3)
            ret = (b >> (char_tree & 7)) & 1
            char_tree += 1
            return ret

        def text_flag():
            nonlocal text_tree
            b = read_byte(src, text_tree >> 3)
            ret = (b >> (text_tree & 7)) & 1
            text_tree += 1
            return ret

        while char_flag() == 0:
            if text_flag() == 1:
                depth = 0
                while depth >= 0:
                    while char_flag() == 0:
                        depth += 1
                    char_slot -= 12
                    depth -= 1
        init_char = (read_int16(src, char_slot >> 3) >> (char_slot & 7)) & 0xfff
        out.append(init_char)
        done = (init_char == 0)
    return out

def decompress(src):
    try:

        asmpchar = read_int32(src, 0x38578) - 0x08000000
        asmptext = read_int32(src, 0x385DC) - 0x08000000

        char_data = read_int32(src,   asmpchar) - 0x08000000
        char_ptrs = read_int32(src, asmpchar+4) - 0x08000000

        info = asmpchar, asmptext, char_data, char_ptrs

        strings = []

        for i in range(12461):
            strings.append(decompress_one(src, i, info))
        return strings
    except:
        import pdb; pdb.post_mortem()

with open('english.gba', 'rb') as f:
    from datetime import datetime
    s = datetime.now()
    b = f.read()
    e = datetime.now() - s
    print(e)
    strings = decompress(b)
    print(strings[:20])
