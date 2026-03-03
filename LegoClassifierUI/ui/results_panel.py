"""Results display panel for extracted features."""

import tkinter as tk
from tkinter import ttk
from typing import Optional


class ResultsPanel(ttk.Frame):
    """Panel displaying extracted LEGO features and classification."""

    def __init__(self, parent, **kwargs):
        super().__init__(parent, **kwargs)
        self._setup_ui()

    def _setup_ui(self):
        style = ttk.Style()
        style.configure("Header.TLabel", font=("Segoe UI", 12, "bold"))
        style.configure("Value.TLabel", font=("Segoe UI", 11))
        style.configure("Classification.TLabel", font=("Segoe UI", 14, "bold"))

        header = ttk.Label(self, text="Extracted Features", style="Header.TLabel")
        header.pack(pady=(0, 10), anchor=tk.W)

        ttk.Separator(self, orient=tk.HORIZONTAL).pack(fill=tk.X, pady=5)

        features_frame = ttk.Frame(self)
        features_frame.pack(fill=tk.X, pady=5)

        self._area_var = tk.StringVar(value="--")
        self._perimeter_var = tk.StringVar(value="--")
        self._aspect_ratio_var = tk.StringVar(value="--")
        self._circles_var = tk.StringVar(value="--")

        self._create_feature_row(features_frame, "Area:", self._area_var, 0)
        self._create_feature_row(features_frame, "Perimeter:", self._perimeter_var, 1)
        self._create_feature_row(features_frame, "Aspect Ratio:", self._aspect_ratio_var, 2)
        self._create_feature_row(features_frame, "Circles (Studs):", self._circles_var, 3)

        ttk.Separator(self, orient=tk.HORIZONTAL).pack(fill=tk.X, pady=10)

        class_header = ttk.Label(self, text="Classification", style="Header.TLabel")
        class_header.pack(pady=(0, 5), anchor=tk.W)

        self._classification_var = tk.StringVar(value="No classification")
        self._classification_label = ttk.Label(
            self,
            textvariable=self._classification_var,
            style="Classification.TLabel"
        )
        self._classification_label.pack(anchor=tk.W)

        self._description_var = tk.StringVar(value="")
        self._description_label = ttk.Label(
            self,
            textvariable=self._description_var,
            wraplength=250
        )
        self._description_label.pack(anchor=tk.W, pady=(5, 0))

    def _create_feature_row(self, parent, label: str, var: tk.StringVar, row: int):
        """Create a labeled feature display row."""
        ttk.Label(parent, text=label).grid(row=row, column=0, sticky=tk.W, pady=2)
        ttk.Label(parent, textvariable=var, style="Value.TLabel").grid(
            row=row, column=1, sticky=tk.E, pady=2, padx=(20, 0)
        )
        parent.columnconfigure(1, weight=1)

    def update_features(
        self,
        area: Optional[float] = None,
        perimeter: Optional[float] = None,
        aspect_ratio: Optional[float] = None,
        circles: Optional[int] = None
    ):
        """Update the displayed feature values."""
        self._area_var.set(f"{area:.2f}" if area is not None else "--")
        self._perimeter_var.set(f"{perimeter:.2f}" if perimeter is not None else "--")
        self._aspect_ratio_var.set(f"{aspect_ratio:.4f}" if aspect_ratio is not None else "--")
        self._circles_var.set(str(circles) if circles is not None else "--")

        if circles is not None:
            self._update_classification(circles)
        else:
            self._classification_var.set("No classification")
            self._description_var.set("")

    def _update_classification(self, circles: int):
        """Update classification based on stud count."""
        classifications = {
            0: ("Unknown", "No studs detected - may not be a standard LEGO brick"),
            1: ("1x1 Brick", "Single stud - smallest standard brick"),
            2: ("1x2 Brick", "Two studs - common building brick"),
            3: ("1x3 Brick", "Three studs in a row"),
            4: ("2x2 or 1x4 Brick", "Four studs - could be square or linear"),
            6: ("2x3 or 1x6 Brick", "Six studs"),
            8: ("2x4 or 1x8 Brick", "Eight studs - classic LEGO brick size"),
        }

        if circles in classifications:
            name, desc = classifications[circles]
        elif circles > 8:
            name = f"Large Brick ({circles} studs)"
            desc = "Large brick or plate element"
        else:
            name = f"{circles}-Stud Brick"
            desc = "Non-standard stud configuration"

        self._classification_var.set(name)
        self._description_var.set(desc)

    def clear(self):
        """Clear all displayed values."""
        self._area_var.set("--")
        self._perimeter_var.set("--")
        self._aspect_ratio_var.set("--")
        self._circles_var.set("--")
        self._classification_var.set("No classification")
        self._description_var.set("")
