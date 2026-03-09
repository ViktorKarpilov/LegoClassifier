#!/usr/bin/env python3
"""LEGO Classifier UI - Entry point."""
import struct
import sys
import tkinter as tk
import numpy as np
from PIL import Image, ImageTk

from connection.packet_type import PacketType
from connection.usb_connection import UsbConnection
from ui.main_window import MainWindow

def rgb565_to_image(byte_data, width, height):
    # Unpack bytes into 16-bit big-endian unsigned shorts
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

def main():
    # usb = UsbConnection()
    #
    # while 1:
    #     test = usb.read_packet()
    #     if test.type is PacketType.Image and len(test.byte_data) >= 160*120*2:
    #         image = rgb565_to_image(test.byte_data, 160, 120)
    #         image.save("output_picture.png")
    #         image.show("TestTitle")


    root = tk.Tk()
    root.title("LEGO Classifier")
    root.geometry("1200x800")
    root.minsize(800, 600)

    app = MainWindow(root)
    app.pack(fill=tk.BOTH, expand=True)

    root.mainloop()


if __name__ == "__main__":
    sys.exit(main() or 0)
