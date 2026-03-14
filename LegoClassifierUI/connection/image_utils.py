import struct
from PIL import Image


def rgb565_to_image(byte_data, width, height):
    pixels = struct.unpack(f'>{width * height}H', byte_data)

    image = Image.new("RGB", (width, height))
    for y in range(height):
        for x in range(width):
            px = pixels[y * width + x]
            r = (px & 0xF800) >> 8
            g = (px & 0x07E0) >> 3
            b = (px & 0x001F) << 3
            image.putpixel((x, y), (r, g, b))
    return image
