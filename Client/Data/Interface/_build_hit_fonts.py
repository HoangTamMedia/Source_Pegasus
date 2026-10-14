"""Redraw the floating-damage glyphs, and build the hit-word sheet beside them.

FontTest.tga is addressed by hardcoded UVs in RenderNumber, so its geometry is
fixed and must be reproduced exactly:

    a 16 x 2 grid of square cells
      top row     cells 0-9  digits, cell 10 comma
      bottom row  cells 0-1  the "Miss" word, as one block

The shipped FontTest.OZT is 256x32, so a cell is 16px. Every UV in RenderNumber
is a fraction though, so the sheet can be built at any multiple of that and the
same UVs still land on the same glyphs - this writes 1024x128, four times the
detail, which is what stops the digits looking chewed at close range.

Nothing else in the sheet is referenced, so the rest is free.

The glyphs stay white. RenderNumber multiplies them by a colour chosen from the
damage type, so any colour baked in here would be multiplied a second time and
come out muddy - the design has to live in the shape, the outline and the glow
rather than in the hue.

pg_hitword.tga is new and entirely ours, so it is laid out plainly: 512 wide,
one word per 64px row, indexed top to bottom by PG_HITWORD_* in ZzzEffect.h.
"""

import os
from PIL import Image, ImageDraw, ImageFont, ImageFilter

HERE = os.path.dirname(os.path.abspath(__file__))

FONT_BLACK = r"C:\Windows\Fonts\ariblk.ttf"
FONT_IMPACT = r"C:\Windows\Fonts\impact.ttf"

CELL = 64


def write_tga(img, base):
    """Write both .tga and .OZT.

    The .OZT is the one that matters. OpenTga swaps whatever extension it is
    handed for "OZT" and opens that, so a lone .tga beside it is never read -
    which is exactly how a missing .OZT turns into a blank tinted quad on
    screen. The .tga goes out too, purely so the folder can be inspected with
    ordinary tools.
    """
    w, h = img.size
    header = bytearray(18)
    header[2] = 2
    header[12] = w & 0xFF
    header[13] = (w >> 8) & 0xFF
    header[14] = h & 0xFF
    header[15] = (h >> 8) & 0xFF
    header[16] = 32
    header[17] = 8

    px = img.load()
    body = bytearray()
    for y in range(h - 1, -1, -1):
        for x in range(w):
            r, g, b, a = px[x, y]
            body += bytes((b, g, r, a))

    tga = bytes(header) + bytes(body)

    with open(base + ".tga", "wb") as f:
        f.write(tga)
    # OZT is the same file with a 4-byte marker spliced in at offset 6.
    with open(base + ".OZT", "wb") as f:
        f.write(tga[:6] + b"\x02\x00\x00\x00" + tga[6:])

    print("  %-20s %dx%d  (.tga + .OZT)" % (os.path.basename(base), w, h))


def draw_glyph(size, text, font, skew=0.22, fill=0.78, stroke=0.055, glow_amt=0.42):
    """One glyph on a transparent tile: dark outline, white body, soft glow.

    Drawn at 4x and brought down, which is what keeps the outline smooth at the
    small sizes these end up on screen. The skew leans the text the way the
    original sheet did.
    """
    S = 4
    w, h = size[0] * S, size[1] * S
    tile = Image.new("RGBA", (w, h), (0, 0, 0, 0))
    d = ImageDraw.Draw(tile)

    f = ImageFont.truetype(font, int(h * fill))
    bb = d.textbbox((0, 0), text, font=f, stroke_width=int(h * stroke))
    tx = (w - (bb[2] - bb[0])) // 2 - bb[0]
    ty = (h - (bb[3] - bb[1])) // 2 - bb[1]

    # Outline first, then the body over it.
    d.text((tx, ty), text, font=f, fill=(10, 10, 14, 255),
           stroke_width=int(h * stroke), stroke_fill=(10, 10, 14, 255))

    body = Image.new("RGBA", (w, h), (0, 0, 0, 0))
    ImageDraw.Draw(body).text((tx, ty), text, font=f, fill=(255, 255, 255, 255))

    # A vertical fall-off from white to light grey gives the glyph some form
    # without introducing a hue that the runtime tint would fight with.
    shade = Image.new("L", (w, h))
    sp = shade.load()
    for y in range(h):
        v = int(255 - 42 * (y / float(h)))
        for x in range(w):
            sp[x, y] = v
    body.putalpha(body.split()[3])
    body = Image.merge("RGBA", (shade, shade, shade, body.split()[3]))

    tile = Image.alpha_composite(tile, body)

    # Glow lifted from the body alpha, laid underneath so it reads as light
    # coming off the glyph rather than a second outline.
    glow = body.split()[3].filter(ImageFilter.GaussianBlur(h * 0.06))
    glow_img = Image.new("RGBA", (w, h), (255, 255, 255, 0))
    glow_img.putalpha(glow.point(lambda v: int(v * glow_amt)))
    tile = Image.alpha_composite(glow_img, tile)

    # Lean.
    if skew:
        tile = tile.transform((w, h), Image.AFFINE, (1, skew, -skew * h * 0.5, 0, 1, 0),
                              resample=Image.BICUBIC)

    return tile.resize(size, Image.LANCZOS)


print("building floating-damage glyphs")

# --- FontTest.tga : digits, comma, Miss -------------------------------------
sheet = Image.new("RGBA", (1024, 128), (0, 0, 0, 0))

for i, ch in enumerate("0123456789,"):
    tile = draw_glyph((CELL, CELL), ch, FONT_BLACK,
                      fill=0.94, stroke=0.075, glow_amt=0.6)
    sheet.paste(tile, (i * CELL, 0), tile)

# "Miss" is read as one 128x60 block from the bottom row.
miss = draw_glyph((128, 60), "Miss", FONT_BLACK, skew=0.18,
                  fill=0.88, stroke=0.07, glow_amt=0.6)
sheet.paste(miss, (0, 68), miss)

write_tga(sheet, os.path.join(HERE, "FontTest"))


# --- pg_hitword.tga : the shout words ---------------------------------------
# Order here IS the index order in ZzzEffect.h. Append only at the end.
WORDS = [
    "CRITICAL",
    "EXCELLENT",
    "DOUBLE",
    "COMBO",
    "BLOCKED",
    "PERFECT",
    "DEADLY",
    "SMASH",
]

WORD_W, WORD_H = 512, 64
words = Image.new("RGBA", (WORD_W, WORD_H * len(WORDS)), (0, 0, 0, 0))

for i, w in enumerate(WORDS):
    tile = draw_glyph((WORD_W, WORD_H), w, FONT_IMPACT, skew=0.16,
                      fill=0.80, stroke=0.05, glow_amt=0.5)
    words.paste(tile, (0, i * WORD_H), tile)

write_tga(words, os.path.join(HERE, "Pegasus", "Pegasus_Main", "pg_hitword"))

print("done - %d words" % len(WORDS))
