"""Build the bar the map/position line sits on, top-left of the screen.

This is drawn instead of the old three-part Minimap_position art so the bar
matches the "Menu Game [F5]" bar: bronze body, gold bevel, and the three-line
glyph on the left that opens the game menu.

It is generated rather than cropped out of an existing file because nothing in
the folder is this shape - TournamentUI_I9 is the right silhouette but it is
grey, and TournamentMenu_I17's title strip is blue and 548 wide. Generating it
also means the caps are exact multiples of what the three-slice draw expects,
so no seam shows where the middle stretches.

Output: pg_mapbar.tga + pg_mapbar.OZT, 256x24. The .OZT is the one the client
actually opens - OpenTga swaps the extension it is handed for "OZT".

    python _build_mapbar.py
"""

import os

W, H = 256, 24
CAP_L = 32      # glyph end - must match PG_MAPBAR_CAP_L in NewUIHeroPositionInfo.cpp
CAP_R = 10      # right bevel  - must match PG_MAPBAR_CAP_R

OUT = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                   "Pegasus", "Pegasus_UI", "pg_mapbar")


def lerp(a, b, t):
    return tuple(int(round(a[i] + (b[i] - a[i]) * t)) for i in range(len(a)))


# Body reads darkest at the edges and warmest a third of the way down, which is
# what gives the bar its rounded, lit-from-above look on a flat quad.
TOP    = (58, 40, 22)
MID    = (92, 66, 34)
BOTTOM = (34, 23, 13)
GOLD   = (196, 158, 84)
GOLD_HI = (238, 208, 140)
EDGE   = (18, 13, 8)


def body_colour(y):
    t = y / float(H - 1)
    if t < 0.35:
        return lerp(TOP, MID, t / 0.35)
    return lerp(MID, BOTTOM, (t - 0.35) / 0.65)


def build():
    from PIL import Image
    img = Image.new("RGBA", (W, H), (0, 0, 0, 0))
    px = img.load()

    r = 4   # corner radius

    def inside(x, y, inset):
        """Rounded-rect test, inset from the full quad by `inset` pixels."""
        x0, y0 = inset, inset
        x1, y1 = W - 1 - inset, H - 1 - inset
        if x < x0 or x > x1 or y < y0 or y > y1:
            return False
        rr = max(0, r - inset)
        for cx, cy in ((x0 + rr, y0 + rr), (x1 - rr, y0 + rr),
                       (x0 + rr, y1 - rr), (x1 - rr, y1 - rr)):
            if ((x < x0 + rr and cx == x0 + rr) or (x > x1 - rr and cx == x1 - rr)) and \
               ((y < y0 + rr and cy == y0 + rr) or (y > y1 - rr and cy == y1 - rr)):
                if (x - cx) ** 2 + (y - cy) ** 2 > rr * rr:
                    return False
        return True

    for y in range(H):
        for x in range(W):
            if not inside(x, y, 0):
                continue
            if not inside(x, y, 1):
                px[x, y] = EDGE + (255,)          # outer dark keyline
            elif not inside(x, y, 2):
                # Gold bevel, brighter along the top edge than the bottom.
                px[x, y] = (GOLD_HI if y < H // 2 else GOLD) + (255,)
            else:
                c = body_colour(y)
                # A one-pixel inner highlight under the bevel lifts the top.
                if inside(x, y, 2) and not inside(x, y, 3) and y < H // 2:
                    c = lerp(c, GOLD, 0.30)
                px[x, y] = c + (255,)

    # The three-line glyph, gold, vertically centred in the cap.
    gx0, gx1 = 9, 25
    for i, gy in enumerate((7, 11, 15)):
        for x in range(gx0, gx1 + 1):
            # Round the ends so the bars read as bars, not as slots.
            edge = x == gx0 or x == gx1
            for yy in (gy, gy + 1):
                if edge and yy == gy + 1:
                    continue
                px[x, yy] = (GOLD_HI if yy == gy else GOLD) + (255,)

    # Divider between the glyph and the text, so the button reads as separate.
    for y in range(5, H - 5):
        px[CAP_L - 3, y] = lerp(body_colour(y), EDGE, 0.65) + (255,)
        px[CAP_L - 2, y] = lerp(body_colour(y), GOLD, 0.22) + (255,)

    return img


def write_tga(img, base):
    """Write both .tga and .OZT - the .OZT is the one the client reads."""
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
    with open(base + ".OZT", "wb") as f:
        f.write(tga[:6] + b"\x02\x00\x00\x00" + tga[6:])
    print("  %-20s %dx%d  (.tga + .OZT)" % (os.path.basename(base), w, h))


if __name__ == "__main__":
    write_tga(build(), OUT)
    print("caps: left %d, right %d" % (CAP_L, CAP_R))
