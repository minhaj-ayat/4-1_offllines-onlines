import binascii
import timeit
from BitVector import *

Sbox = (
    0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
    0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
    0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
    0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
    0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
    0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
    0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
    0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
    0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
    0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
    0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
    0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
    0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
    0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
    0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16,
)

InvSbox = (
    0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
    0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
    0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
    0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
    0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
    0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
    0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
    0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
    0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
    0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
    0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
    0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
    0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
    0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
    0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D,
)

Mixer = [
    [BitVector(hexstring="02"), BitVector(hexstring="03"), BitVector(hexstring="01"), BitVector(hexstring="01")],
    [BitVector(hexstring="01"), BitVector(hexstring="02"), BitVector(hexstring="03"), BitVector(hexstring="01")],
    [BitVector(hexstring="01"), BitVector(hexstring="01"), BitVector(hexstring="02"), BitVector(hexstring="03")],
    [BitVector(hexstring="03"), BitVector(hexstring="01"), BitVector(hexstring="01"), BitVector(hexstring="02")]
]

InvMixer = [
    [BitVector(hexstring="0E"), BitVector(hexstring="0B"), BitVector(hexstring="0D"), BitVector(hexstring="09")],
    [BitVector(hexstring="09"), BitVector(hexstring="0E"), BitVector(hexstring="0B"), BitVector(hexstring="0D")],
    [BitVector(hexstring="0D"), BitVector(hexstring="09"), BitVector(hexstring="0E"), BitVector(hexstring="0B")],
    [BitVector(hexstring="0B"), BitVector(hexstring="0D"), BitVector(hexstring="09"), BitVector(hexstring="0E")]
]

rc = BitVector(hexstring="01")
AES_modulus = BitVector(bitstring='100011011')


def make_matrix(string):
    m = [
        [string[0:2], string[8:10], string[16:18], string[24:26]],
        [string[2:4], string[10:12], string[18:20], string[26:28]],
        [string[4:6], string[12:14], string[20:22], string[28:30]],
        [string[6:8], string[14:16], string[22:24], string[30:32]]
    ]
    return m


def make_matrix_2(arr):
    m = [
        [arr[0], arr[4], arr[8], arr[12]],
        [arr[1], arr[5], arr[9], arr[13]],
        [arr[2], arr[6], arr[10], arr[14]],
        [arr[3], arr[7], arr[11], arr[15]]
    ]
    return m


def input_text(txt, t):
    txt = txt.ljust(16)
    print("Text---")

    if t:
        print(txt + "  [In ASCII]")
        hk = ""
        for ch in txt:
            hk += hex(ord(ch)).split('x')[1]
        print(hk + "   [In HEX]")
        km = make_matrix(hk)
        return km

    else:
        print(txt.decode(errors="replace") + (" [In ASCII]"))

        hk = [""] * 16
        ai = 0
        for ch in txt:
            sa = hex(ch).split("x")[1]
            sa = sa if len(sa) > 1 else "0" + sa
            hk[ai] = sa
            ai += 1
        print(hk)
        km = make_matrix_2(hk)
        return km


def input_key():
    key = input("Enter key : ").zfill(16)
    key = key[:-(len(key) - 16)] if (len(key) - 16) > 0 else key
    print("Key---")
    print(key + "  [In ASCII]")

    hk = ""
    for ch in key:
        hk += hex(ord(ch)).split('x')[1]
    print(hk + "   [In HEX]")
    km = make_matrix(hk)
    return km


def make_four_words(km):
    w = [""] * 44
    ct = 0
    while ct < 4:
        w[ct] = km[0][ct] + km[1][ct] + km[2][ct] + km[3][ct]
        ct = ct + 1
    return w


def generating_function(root):
    # circular left shift
    w = BitVector(hexstring=root).intValue()
    w = ((w << 8) | (w >> (32 - 8))) & 0xFFFFFFFF
    w = BitVector(intVal=w, size=32).get_bitvector_in_hex()

    # byte substitution
    ct = 0
    i = 0
    o = ""
    while ct < 4:
        ck = BitVector(hexstring=w[i] + w[i + 1])
        int_val = ck.intValue()
        ck = Sbox[int_val]
        ck = BitVector(intVal=ck, size=8).get_bitvector_in_hex()
        i = i + 2
        o += ck
        ct = ct + 1

    # adding round constant
    global rc

    bv1 = BitVector(hexstring="02")
    bv2 = BitVector(hexstring=rc.get_bitvector_in_hex())
    rc = bv1.gf_multiply_modular(bv2, AES_modulus, 8) if words[4] != "" else BitVector(hexstring="01")
    x1 = BitVector(hexstring=o[0:2])
    x = rc.intValue() ^ x1.intValue()
    fg = BitVector(intVal=x, size=8).get_bitvector_in_hex() + o[2:]
    return fg


def generate_round_key(rnd):
    global words
    j = 0
    while j < 4:
        ind = 4 * rnd + j
        w0 = BitVector(hexstring=words[ind]).intValue()
        w3 = BitVector(hexstring=words[ind + 3]).intValue() if (ind + 4) % 4 != 0 else BitVector(
            hexstring=generating_function(words[ind + 3])).intValue()
        w = w0 ^ w3
        words[ind + 4] = BitVector(intVal=w, size=32).get_bitvector_in_hex()
        j = j + 1


def print_keys():
    rn = 0
    while rn < 41:
        print(words[rn:rn + 4])
        rn = rn + 4


def add_round_key(rnd, inv):
    global state_matrix
    ind = 4 * rnd if not inv else 40 - 4 * rnd
    km = make_matrix(words[ind] + words[ind + 1] + words[ind + 2] + words[ind + 3])
    for i in range(4):
        for j in range(4):
            x = BitVector(hexstring=state_matrix[i][j]).intValue() ^ BitVector(hexstring=km[i][j]).intValue()
            x = BitVector(intVal=x, size=8).get_bitvector_in_hex()
            state_matrix[i][j] = x


def byte_substitution(inv):
    for i in range(4):
        for j in range(4):
            b = BitVector(hexstring=state_matrix[i][j])
            int_val = b.intValue()
            if not inv:
                s = Sbox[int_val]
            else:
                s = InvSbox[int_val]
            s = BitVector(intVal=s, size=8)
            state_matrix[i][j] = s.get_bitvector_in_hex()


def shift_rows(inv):
    global state_matrix
    string = state_matrix[0][0] + state_matrix[0][1] + state_matrix[0][2] + state_matrix[0][3]
    for i in range(3):
        st = state_matrix[i + 1][0] + state_matrix[i + 1][1] + state_matrix[i + 1][2] + state_matrix[i + 1][3]
        w = BitVector(hexstring=st).intValue()
        if not inv:
            w = ((w << (8 + 8 * i)) | (w >> (32 - (8 + 8 * i)))) & 0xFFFFFFFF
        else:
            w = ((w >> (8 + 8 * i)) | (w << (32 - (8 + 8 * i)))) & 0xFFFFFFFF
        w = BitVector(intVal=w, size=32).get_bitvector_in_hex()
        string = string + w

    state_matrix = [
        [string[0:2], string[2:4], string[4:6], string[6:8]],
        [string[8:10], string[10:12], string[12:14], string[14:16]],
        [string[16:18], string[18:20], string[20:22], string[22:24]],
        [string[24:26], string[26:28], string[28:30], string[30:32]]
    ]


def mix_columns(inv):
    res = [[0 for x in range(4)] for y in range(4)]
    for i in range(4):
        for j in range(4):
            for k in range(4):
                if not inv:
                    bv1 = Mixer[i][k]
                else:
                    bv1 = InvMixer[i][k]
                bv2 = BitVector(hexstring=state_matrix[k][j])
                bv3 = bv1.gf_multiply_modular(bv2, AES_modulus, 8)
                bv3 = bv3.intValue()
                res[i][j] = res[i][j] ^ bv3

    for i in range(4):
        for j in range(4):
            state_matrix[i][j] = BitVector(intVal=res[i][j], size=8).get_bitvector_in_hex()


def encrypt():
    curr_round = 0
    add_round_key(curr_round, False)
    curr_round = curr_round + 1

    while curr_round < 11:
        byte_substitution(False)
        shift_rows(False)
        if curr_round != 10:
            mix_columns(False)
        add_round_key(curr_round, False)
        curr_round = curr_round + 1


def decrypt():
    curr_round = 0
    add_round_key(curr_round, True)
    curr_round = curr_round + 1
    print(state_matrix)

    while curr_round < 11:
        shift_rows(True)
        byte_substitution(True)
        add_round_key(curr_round, True)
        if curr_round != 10:
            mix_columns(True)
        curr_round = curr_round + 1
        print(state_matrix)


def matrix_to_text(mat, inv, t):
    if not inv:
        print("Ciphered Text--- ")
    else:
        print("Deciphered Text--- ")

    if t:
        st = ""
        for i in range(4):
            for j in range(4):
                st = st + mat[j][i]

        print(st + "  [In HEX]")
        ft = bytearray.fromhex(st).decode(errors="replace")
        print(ft + " [In ASCII]")
        return ft

    else:
        st = ""
        for i in range(4):
            for j in range(4):
                st = st + (mat[j][i])

        print(st + ("  [In HEX]"))
        ft = binascii.unhexlify(st)
        print(ft)
        return ft


def print_words():
    print(words)


def process_text():
    global state_matrix
    global words
    text = input("Enter plain text: ")
    key_matrix = input_key()

    cnt = 0
    lc = len(text) / 16
    index = 0
    final_val = ""
    while cnt <= lc:
        state_matrix = input_text(text[index: index + 16], True)
        if cnt == lc and len(text) % 16 != 0:
            state_matrix = input_text(text[index:], True)
        elif cnt == lc and len(text) % 16 == 0:
            break
        index += 16
        words = make_four_words(key_matrix)
        cnt += 1

        start = timeit.default_timer()
        for r in range(10):
            generate_round_key(r)
        stop = timeit.default_timer()

        start1 = timeit.default_timer()
        encrypt()
        stop1 = timeit.default_timer()
        matrix_to_text(state_matrix, False, True)

        start2 = timeit.default_timer()
        decrypt()
        stop2 = timeit.default_timer()
        final_val += matrix_to_text(state_matrix, True, True)

        print("\nEncryption Time:")
        print("Key Scheduling time : " + str(stop - start))
        print("Encryption time : " + str(stop1 - start1))
        print("Decryption time : " + str(stop2 - start2))
        print("\n")

    print("Merged Output: " + final_val + "\n")


def process_file2():
    global state_matrix
    global words
    fn = input("Enter file name: ")
    key_matrix = input_key()

    final_val = b""
    file = open(fn, "rb")
    while True:
        text = file.read(16)
        print(text)
        if not text:
            break
        state_matrix = input_text(text, False)
        words = make_four_words(key_matrix)

        start = timeit.default_timer()
        for r in range(10):
            generate_round_key(r)
        stop = timeit.default_timer()

        start1 = timeit.default_timer()
        encrypt()
        stop1 = timeit.default_timer()
        matrix_to_text(state_matrix, False, False)

        start2 = timeit.default_timer()
        decrypt()
        stop2 = timeit.default_timer()
        final_val += matrix_to_text(state_matrix, True, False)

        print("\nEncryption Time:")
        print("Key Scheduling time : " + str(stop - start))
        print("Encryption time : " + str(stop1 - start1))
        print("Decryption time : " + str(stop2 - start2))
        print("\n")

    file.close()
    print(final_val)
    file2 = open(fn.split(".")[0] + "_decrypt." + fn.split(".")[1], "wb")
    file2.write(final_val)
    file2.close()


def generate_s_box():
    sbox = [["" for i in range(16)] for j in range(16)]
    inv_sbox = [["" for i in range(16)] for j in range(16)]
    for i in range(1, 256):
        hs = hex(i).split("x")[1]
        m = BitVector(hexstring=hs).gf_MI(BitVector(bitstring="100011011"), 8).intValue()
        sv = hex(m ^ (((m << 1) | (m >> 7)) & 0xFF) ^ (((m << 2) | (m >> 6)) & 0xFF) ^ (
                    ((m << 3) | (m >> 5)) & 0xFF) ^ (((m << 4) | (m >> 4)) & 0xFF) ^ 0x63)
        sv = sv if len(sv)>3 else sv[0:2]+"0"+sv[-1]
        sbox[(int(i / 16))][i % 16] = sv

    sbox[0][0] = "0x63"
    for e in sbox:
        print(e)

    for i in range(16):
        for j in range(16):
            el = sbox[i][j]
            r = el[2:3]
            c = el[3:4]
            print(r+"->"+c)
            r = int(r, 16)
            c = int(c, 16)
            inv_sbox[r][c] = "0x"+hex(i).split("x")[1]+hex(j).split("x")[1]

    for e in inv_sbox:
        print(e)


state_matrix = [["" * 4] * 4]
words = ["" * 44]

print("1. Enter Plain Text")
print("2. Enter File")
print("3. Generate S-box and Inv S-box")
inp = input("Choice: ")

if int(inp) == 1:
    process_text()
elif int(inp) == 3:
    generate_s_box()
else:
    process_file2()
