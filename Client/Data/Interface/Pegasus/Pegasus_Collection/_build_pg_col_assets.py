"""Cut the Collection window's textures out of Pegasus_Collections.png.

The sheet is 2399x1507, well past OpenTga's MAX_WIDTH of 1024, so the client
never loaded it at all - every piece has to be cut out and shipped separately.

Two rules govern the cuts:

  * every output is power-of-two. OpenTga rounds a non-power-of-two image up to
    the next power of two and leaves the extra rows and columns UNINITIALISED,
    so a UV that reaches into the padding samples heap garbage and draws as a
    white translucent block.

  * the art is anchored top-left, and the source rect the code asks for has to
    be the art's real extent, never the texture's. The constants in
    PegasusCollection.cpp are those extents - keep the two in step.

The pedestal frames were previously cut on a 194px pitch from a guessed origin.
The frames are actually 172x197 on a 185px pitch starting at x=1131, which is
why slot1 caught a sliver of its neighbour's edge and slot3 caught the blue
navigation arrows and the X button sitting to the right of the row.
"""

import os
from PIL import Image

SRC = os.path.dirname(os.path.abspath(__file__))
SHEET = os.path.join(SRC, "Pegasus_Collections.png")


def write_tga_and_ozt(img, out_base):
    w, h = img.size
    assert w & (w - 1) == 0 and h & (h - 1) == 0, f"{out_base} not POT: {w}x{h}"

    header = bytearray(18)
    header[2] = 2                       # uncompressed true-colour
    header[12] = w & 0xFF
    header[13] = (w >> 8) & 0xFF
    header[14] = h & 0xFF
    header[15] = (h >> 8) & 0xFF
    header[16] = 32                     # bpp
    header[17] = 8                      # 8 alpha bits, origin lower-left

    px = img.load()
    body = bytearray()
    for y in range(h - 1, -1, -1):      # bottom-up, like every other asset here
        for x in range(w):
            r, g, b, a = px[x, y]
            body += bytes((b, g, r, a))

    tga = bytes(header) + bytes(body)
    with open(out_base + ".tga", "wb") as f:
        f.write(tga)

    # OZT is the same file with a 4-byte marker spliced in at offset 6.
    with open(out_base + ".OZT", "wb") as f:
        f.write(tga[:6] + b"\x02\x00\x00\x00" + tga[6:])

    print(f"  {os.path.basename(out_base):16s} {w}x{h}   art {ART[out_base]}")


ART = {}


def cut(sheet, box, canvas, out_name):
    """box is (left, top, right, bottom) on the sheet; canvas is the POT size."""
    piece = sheet.crop(box)
    out = Image.new("RGBA", canvas, (0, 0, 0, 0))
    out.paste(piece, (0, 0))            # top-left anchored == UV origin
    base = os.path.join(SRC, out_name)
    ART[base] = f"{piece.width}x{piece.height}"
    write_tga_and_ozt(out, base)


print("building Pegasus Collection textures")
sheet = Image.open(SHEET).convert("RGBA")

# The four pedestal frames, one per rarity. 172x170 each, on a 185px pitch,
# with a clean 13px gutter between them and 30px of empty sheet above the row.
SLOT_X = (1131, 1316, 1501, 1686)
SLOT_Y0, SLOT_Y1 = 780, 950
for i, x in enumerate(SLOT_X):
    cut(sheet, (x, SLOT_Y0, x + 172, SLOT_Y1), (256, 256), f"pg_col_slot{i}")
