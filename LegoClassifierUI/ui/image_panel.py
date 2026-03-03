"""Image display panel with auto-scaling support."""

import tkinter as tk
from tkinter import ttk
from PIL import Image, ImageTk
import numpy as np
from pathlib import Path
from typing import Optional, Union


class ImagePanel(ttk.Frame):
    """Canvas-based image display with auto-scaling."""

    def __init__(self, parent, **kwargs):
        super().__init__(parent, **kwargs)

        self._image: Optional[Image.Image] = None
        self._photo: Optional[ImageTk.PhotoImage] = None
        self._image_id: Optional[int] = None

        self._setup_ui()
        self.bind("<Configure>", self._on_resize)

    def _setup_ui(self):
        self.canvas = tk.Canvas(self, bg="#2b2b2b", highlightthickness=0)
        self.canvas.pack(fill=tk.BOTH, expand=True)

        self._placeholder_text = self.canvas.create_text(
            0, 0,
            text="No image loaded\n\nCtrl+O to open an image",
            fill="#666666",
            font=("Segoe UI", 14),
            justify=tk.CENTER
        )

    def load_from_path(self, path: Union[str, Path]) -> bool:
        """Load an image from a file path."""
        try:
            self._image = Image.open(path)
            self._display_image()
            return True
        except Exception as e:
            print(f"Error loading image: {e}")
            return False

    def load_from_array(self, array: np.ndarray) -> bool:
        """Load an image from a NumPy array (from C++ bindings)."""
        try:
            if array.size == 0:
                return False

            if len(array.shape) == 2:
                self._image = Image.fromarray(array, mode="L")
            elif array.shape[2] == 3:
                array_rgb = array[:, :, ::-1].copy()
                self._image = Image.fromarray(array_rgb, mode="RGB")
            elif array.shape[2] == 4:
                array_rgba = np.concatenate([
                    array[:, :, 2:3],
                    array[:, :, 1:2],
                    array[:, :, 0:1],
                    array[:, :, 3:4]
                ], axis=2)
                self._image = Image.fromarray(array_rgba, mode="RGBA")
            else:
                return False

            self._display_image()
            return True
        except Exception as e:
            print(f"Error loading array: {e}")
            return False

    def clear(self):
        """Clear the displayed image."""
        self._image = None
        self._photo = None
        if self._image_id:
            self.canvas.delete(self._image_id)
            self._image_id = None
        self.canvas.itemconfigure(self._placeholder_text, state=tk.NORMAL)
        self._center_placeholder()

    def _display_image(self):
        """Display the current image with auto-scaling."""
        if self._image is None:
            return

        self.canvas.itemconfigure(self._placeholder_text, state=tk.HIDDEN)

        canvas_width = self.canvas.winfo_width()
        canvas_height = self.canvas.winfo_height()

        if canvas_width <= 1 or canvas_height <= 1:
            self.after(10, self._display_image)
            return

        img_width, img_height = self._image.size
        scale = min(canvas_width / img_width, canvas_height / img_height, 1.0)

        new_width = max(1, int(img_width * scale))
        new_height = max(1, int(img_height * scale))

        resized = self._image.resize((new_width, new_height), Image.Resampling.LANCZOS)
        self._photo = ImageTk.PhotoImage(resized)

        x = canvas_width // 2
        y = canvas_height // 2

        if self._image_id:
            self.canvas.delete(self._image_id)

        self._image_id = self.canvas.create_image(x, y, image=self._photo, anchor=tk.CENTER)

    def _on_resize(self, event):
        """Handle window resize."""
        self._center_placeholder()
        if self._image:
            self._display_image()

    def _center_placeholder(self):
        """Center the placeholder text."""
        width = self.canvas.winfo_width()
        height = self.canvas.winfo_height()
        self.canvas.coords(self._placeholder_text, width // 2, height // 2)
