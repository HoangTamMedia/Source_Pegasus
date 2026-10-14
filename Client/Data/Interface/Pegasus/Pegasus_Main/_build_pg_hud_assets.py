import os
from PIL import Image

SRC = os.path.dirname(os.path.abspath(__file__))


def write_tga_and_ozt(img, out_base):
    """img: RGBA PIL image whose size is already power-of-two.

    The client's OpenTga() rounds nx/ny up to the next power of two and leaves
    the padding rows/columns UNINITIALISED. Any UV that reaches into that
    padding samples heap garbage, which is what renders as a white translucent
    block. Shipping textures that are already power-of-two means there is no
    padding at all, so that whole failure mode disappears.
    """
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

    print(f"  {os.path.basename(out_base)}  {w}x{h}")


def pad(src_name, size, resize=None):
    im = Image.open(os.path.join(SRC, src_name)).convert("RGBA")
    if resize:
        im = im.resize(resize, Image.LANCZOS)
    canvas = Image.new("RGBA", size, (0, 0, 0, 0))
    canvas.paste(im, (0, 0))            # art anchored top-left == UV origin
    return canvas


print("building Pegasus HUD textures")

# The full HUD plate. I7 ships at 1224x180, past the loader's MAX_WIDTH of 1024,
# so it has to come down to 1024 wide; 151 keeps the aspect. The canvas is then
# 1024x256 so nothing samples past the art.
write_tga_and_ozt(pad("TournamentMain_I7.png", (1024, 256), (1024, 151)),
                  os.path.join(SRC, "pg_hud_bar"))

# Neutral diamond, tinted red / blue per side.
write_tga_and_ozt(pad("TournamentMain_I4.png", (256, 256)),
                  os.path.join(SRC, "pg_hud_gem"))

# Gauge fills and the empty track behind them.
write_tga_and_ozt(pad("TournamentMain_I5.png", (512, 16)),
                  os.path.join(SRC, "pg_hud_fill_sd"))
write_tga_and_ozt(pad("TournamentMain_I6.png", (512, 16)),
                  os.path.join(SRC, "pg_hud_fill_ag"))
write_tga_and_ozt(pad("TournamentMain_I8.png", (512, 16)),
                  os.path.join(SRC, "pg_hud_track"))

# PegaShop / Start Auto plates.
write_tga_and_ozt(pad("TournamentMain_I101.png", (256, 64)),
                  os.path.join(SRC, "pg_hud_btn"))


def repack_gem(src_name, out_name):
    """Re-lay a 60-frame gem loop so the client can load it.

    The three gem animations ship as 912x1520 sheets - a 6x10 grid of 152px
    cells - which is past OpenTga's MAX_HEIGHT, so none of them ever loaded.
    Re-laid as an 8x8 grid of 128px cells the sheet is exactly 1024x1024: under
    the ceiling, already power-of-two, and with no padding for a UV to fall
    into. Frame order is preserved, because PgGem indexes it as
    col = frame % 8, row = frame / 8.
    """
    SRC_CELL, SRC_COLS = 152, 6
    DST_CELL, DST_COLS = 128, 8
    FRAMES = 60

    src = Image.open(os.path.join(SRC, src_name)).convert("RGBA")
    dst = Image.new("RGBA", (1024, 1024), (0, 0, 0, 0))

    for f in range(FRAMES):
        sx, sy = (f % SRC_COLS) * SRC_CELL, (f // SRC_COLS) * SRC_CELL
        cell = src.crop((sx, sy, sx + SRC_CELL, sy + SRC_CELL))
        cell = cell.resize((DST_CELL, DST_CELL), Image.LANCZOS)
        dst.paste(cell, ((f % DST_COLS) * DST_CELL, (f // DST_COLS) * DST_CELL))

    write_tga_and_ozt(dst, os.path.join(SRC, out_name))


# The animated gems. I3 is fire for life, I2 is drifting light for mana, and I1
# is the green crystal that shipped with them and was never used - it carries
# shield on top of the life gem.
repack_gem("TournamentMain_I3.png", "pg_gem_hp")
repack_gem("TournamentMain_I2.png", "pg_gem_mp")
repack_gem("TournamentMain_I1.png", "pg_gem_sd")

print("done")
