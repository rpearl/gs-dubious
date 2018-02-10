mapping=[c for c in u'\x00\x01\x02\x03\x04\x05\x06\x07\x08\t\n\x0b\x0c\r\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f \uff01"#$%&\'\uff08\uff09*+,-./0123456789:;<=>\uff1f@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~       \u3092\u3049\u3043\u3045\u3047\u3049\u3083\u3085\u3087\u3063\u30fc\u3042\u3044\u3046\u3048\u304a\u304b\u304d\u304f\u3051\u3053\u3055\u3057\u3059\u305b\u305d\u3000\u3002\u300c\u300d\u3001\u30fb\u30f2\u30a1\u30a3\u30a5\u30a7\u30a9\u30e3\u30e5\u30e7\u3063\u30fc\u30a2\u30a4\u30a6\u30a8\u30aa\u30ab\u30ad\u30af\u30b1\u30b3\u30b5\u30b7\u30b9\u30bb\u30bd\u30bf\u30c1\u30c4\u30c6\u30c8\u30ca\u30cb\u30cc\u30cd\u30ce\u30cf\u30d2\u30d5\u30d8\u30db\u30de\u30df\u30e0\u30e1\u30e2\u30e4\u30e6\u30e8\u30e9\u30ea\u30eb\u30ec\u30ed\u30ef\u30f3\u3099\u309A\u305f\u3061\u3064\u3066\u3068\u306a\u306b\u306c\u306d\u306e\u306f\u3072\u3075\u3078\u307b\u307e\u307f\u3080\u3081\u3082\u3084\u3086\u3088\u3089\u308a\u308b\u308c\u308d\u308f\u3093\u3099\u309A']

for c in xrange(1,32):
    if c not in [ord('\n')]:
        mapping[c] = '[%02x]' % (c)
f = open('jpn-bytes.txt')

out = open('mapping.txt', 'w')

outstr = u''

for line in f:
    mod = u''.join(mapping[ord(c)] for c in line)
    outstr += mod

out.write(outstr.encode('utf-8'))
