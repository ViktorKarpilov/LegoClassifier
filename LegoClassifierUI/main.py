#!/usr/bin/env python3
"""LEGO Classifier UI - Entry point."""

import sys
import tkinter as tk

from connection.usb_connection import UsbConnection
from ui.main_window import MainWindow


def main():
    usb = UsbConnection()
    while 1:
        usb.read()

    # root = tk.Tk()
    # root.title("LEGO Classifier")
    # root.geometry("1200x800")
    # root.minsize(800, 600)
    #
    # app = MainWindow(root)
    # app.pack(fill=tk.BOTH, expand=True)
    #
    # root.mainloop()


if __name__ == "__main__":
    sys.exit(main() or 0)
