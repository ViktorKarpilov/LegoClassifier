"""Main application window for LEGO Classifier."""

import tkinter as tk
from tkinter import ttk, filedialog, messagebox
from pathlib import Path
from typing import Optional
import sys
import os

from .image_panel import ImagePanel
from .results_panel import ResultsPanel
import config

if config.OPENCV_DLL_PATH:
    os.add_dll_directory(config.OPENCV_DLL_PATH)
if config.MSYS64_DLL_PATH:
    os.add_dll_directory(config.MSYS64_DLL_PATH)
if config.BINDINGS_PATH and config.BINDINGS_PATH not in sys.path:
    sys.path.insert(0, config.BINDINGS_PATH)

try:
    import lego_classifier
    BINDINGS_AVAILABLE = True
except ImportError:
    lego_classifier = None
    BINDINGS_AVAILABLE = False


class MainWindow(ttk.Frame):
    """Main application window with image display and controls."""

    def __init__(self, parent, **kwargs):
        super().__init__(parent, **kwargs)

        self._current_image_path: Optional[Path] = None
        self._current_contour: Optional[object] = None
        self._current_features: Optional[object] = None

        self._setup_menu(parent)
        self._setup_ui()
        self._setup_bindings(parent)

        if not BINDINGS_AVAILABLE:
            self.after(100, self._show_bindings_warning)

    def _show_bindings_warning(self):
        messagebox.showwarning(
            "Bindings Not Found",
            "C++ bindings (lego_classifier) not found.\n\n"
            "Build the project first:\n"
            "cd LegoClassifierUtils\n"
            "cmake -B build -DCMAKE_BUILD_TYPE=Release\n"
            "cmake --build build"
        )

    def _setup_menu(self, parent):
        """Setup the menu bar."""
        menubar = tk.Menu(parent)
        parent.config(menu=menubar)

        file_menu = tk.Menu(menubar, tearoff=0)
        menubar.add_cascade(label="File", menu=file_menu)
        file_menu.add_command(label="Open Image...", command=self._open_image, accelerator="Ctrl+O")
        file_menu.add_command(label="Export to CSV...", command=self._export_csv)
        file_menu.add_separator()
        file_menu.add_command(label="Exit", command=parent.quit)

        view_menu = tk.Menu(menubar, tearoff=0)
        menubar.add_cascade(label="View", menu=view_menu)
        self._view_mode = tk.StringVar(value="original")
        view_menu.add_radiobutton(label="Original", variable=self._view_mode, value="original", command=self._update_view)
        view_menu.add_radiobutton(label="Contours", variable=self._view_mode, value="contours", command=self._update_view)
        view_menu.add_radiobutton(label="Overlay", variable=self._view_mode, value="overlay", command=self._update_view)

        db_menu = tk.Menu(menubar, tearoff=0)
        menubar.add_cascade(label="Database", menu=db_menu)
        db_menu.add_command(label="View History...", command=self._show_history)
        db_menu.add_command(label="Clear All...", command=self._clear_database)

    def _setup_ui(self):
        """Setup the main UI layout."""
        main_paned = ttk.PanedWindow(self, orient=tk.HORIZONTAL)
        main_paned.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)

        self._image_panel = ImagePanel(main_paned)
        main_paned.add(self._image_panel, weight=3)

        right_panel = ttk.Frame(main_paned, width=300)
        main_paned.add(right_panel, weight=1)

        controls_frame = ttk.LabelFrame(right_panel, text="Controls", padding=10)
        controls_frame.pack(fill=tk.X, pady=(0, 10))

        ttk.Button(controls_frame, text="Open Image", command=self._open_image).pack(fill=tk.X, pady=2)
        ttk.Button(controls_frame, text="Extract Features", command=self._extract_features).pack(fill=tk.X, pady=2)
        ttk.Button(controls_frame, text="Save to Database", command=self._save_to_database).pack(fill=tk.X, pady=2)

        view_frame = ttk.LabelFrame(right_panel, text="View Mode", padding=10)
        view_frame.pack(fill=tk.X, pady=(0, 10))

        ttk.Radiobutton(view_frame, text="Original", variable=self._view_mode, value="original", command=self._update_view).pack(anchor=tk.W)
        ttk.Radiobutton(view_frame, text="Contours", variable=self._view_mode, value="contours", command=self._update_view).pack(anchor=tk.W)
        ttk.Radiobutton(view_frame, text="Overlay", variable=self._view_mode, value="overlay", command=self._update_view).pack(anchor=tk.W)

        epsilon_frame = ttk.LabelFrame(right_panel, text="Epsilon (Contour Approximation)", padding=10)
        epsilon_frame.pack(fill=tk.X, pady=(0, 10))

        self._epsilon_var = tk.DoubleVar(value=3.0)
        self._epsilon_slider = ttk.Scale(
            epsilon_frame,
            from_=1.0,
            to=20.0,
            variable=self._epsilon_var,
            orient=tk.HORIZONTAL,
            command=self._on_epsilon_change
        )
        self._epsilon_slider.pack(fill=tk.X)

        self._epsilon_label = ttk.Label(epsilon_frame, text="Value: 3.0")
        self._epsilon_label.pack()

        results_frame = ttk.LabelFrame(right_panel, text="Results", padding=10)
        results_frame.pack(fill=tk.BOTH, expand=True)

        self._results_panel = ResultsPanel(results_frame)
        self._results_panel.pack(fill=tk.BOTH, expand=True)

    def _setup_bindings(self, parent):
        """Setup keyboard shortcuts."""
        parent.bind("<Control-o>", lambda e: self._open_image())
        parent.bind("<Control-s>", lambda e: self._save_to_database())

    def _open_image(self):
        """Open an image file."""
        filetypes = [
            ("Image files", "*.jpg *.jpeg *.png *.bmp *.tiff *.tif"),
            ("All files", "*.*")
        ]
        path = filedialog.askopenfilename(filetypes=filetypes)
        if path:
            self._current_image_path = Path(path)
            self._current_contour = None
            self._current_features = None
            self._results_panel.clear()

            if self._image_panel.load_from_path(path):
                self._view_mode.set("original")

                if BINDINGS_AVAILABLE:
                    try:
                        self._current_contour = lego_classifier.contours.lego_contour(path)
                    except Exception as e:
                        print(f"Error creating contour: {e}")

    def _extract_features(self):
        """Extract features from the current image."""
        if not self._current_image_path:
            messagebox.showinfo("No Image", "Please open an image first.")
            return

        if not BINDINGS_AVAILABLE:
            messagebox.showerror("Error", "C++ bindings not available.")
            return

        try:
            features = lego_classifier.features.extract_features(str(self._current_image_path))
            self._current_features = features
            self._results_panel.update_features(
                area=features.area,
                perimeter=features.perimeter,
                aspect_ratio=features.aspect_ratio,
                circles=features.circles
            )
        except Exception as e:
            messagebox.showerror("Error", f"Failed to extract features:\n{e}")

    def _update_view(self):
        """Update the displayed image based on view mode."""
        if not self._current_image_path:
            return

        mode = self._view_mode.get()

        if mode == "original":
            self._image_panel.load_from_path(self._current_image_path)
        elif mode == "contours" and self._current_contour and BINDINGS_AVAILABLE:
            try:
                epsilon = self._epsilon_var.get()
                img_array = self._current_contour.build_contours_image(epsilon)
                self._image_panel.load_from_array(img_array)
            except Exception as e:
                messagebox.showerror("Error", f"Failed to build contours image:\n{e}")
        elif mode == "overlay" and self._current_contour and BINDINGS_AVAILABLE:
            try:
                img_array = self._current_contour.build_overlay_image()
                self._image_panel.load_from_array(img_array)
            except Exception as e:
                messagebox.showerror("Error", f"Failed to build overlay image:\n{e}")

    def _on_epsilon_change(self, value):
        """Handle epsilon slider change."""
        epsilon = float(value)
        self._epsilon_label.config(text=f"Value: {epsilon:.1f}")

        if self._view_mode.get() == "contours":
            self._update_view()

    def _save_to_database(self):
        """Save current features to database."""
        if not self._current_features or not self._current_image_path:
            messagebox.showinfo("No Features", "Please extract features first.")
            return

        try:
            from database.repository import FeatureRepository
            repo = FeatureRepository()
            repo.save_features(
                image_path=str(self._current_image_path),
                image_name=self._current_image_path.name,
                area=self._current_features.area,
                perimeter=self._current_features.perimeter,
                aspect_ratio=self._current_features.aspect_ratio,
                circles=self._current_features.circles
            )
            messagebox.showinfo("Success", "Features saved to database.")
        except Exception as e:
            messagebox.showerror("Error", f"Failed to save to database:\n{e}")

    def _show_history(self):
        """Show database history in a dialog."""
        try:
            from database.repository import FeatureRepository
            repo = FeatureRepository()
            records = repo.get_all()

            if not records:
                messagebox.showinfo("History", "No records in database.")
                return

            history_window = tk.Toplevel(self)
            history_window.title("Feature History")
            history_window.geometry("800x400")

            columns = ("id", "image_name", "area", "perimeter", "aspect_ratio", "circles", "created_at")
            tree = ttk.Treeview(history_window, columns=columns, show="headings")

            tree.heading("id", text="ID")
            tree.heading("image_name", text="Image")
            tree.heading("area", text="Area")
            tree.heading("perimeter", text="Perimeter")
            tree.heading("aspect_ratio", text="Aspect Ratio")
            tree.heading("circles", text="Circles")
            tree.heading("created_at", text="Created At")

            tree.column("id", width=40)
            tree.column("image_name", width=150)
            tree.column("area", width=80)
            tree.column("perimeter", width=80)
            tree.column("aspect_ratio", width=100)
            tree.column("circles", width=60)
            tree.column("created_at", width=150)

            for record in records:
                tree.insert("", tk.END, values=record)

            scrollbar = ttk.Scrollbar(history_window, orient=tk.VERTICAL, command=tree.yview)
            tree.configure(yscrollcommand=scrollbar.set)

            tree.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
            scrollbar.pack(side=tk.RIGHT, fill=tk.Y)

        except Exception as e:
            messagebox.showerror("Error", f"Failed to load history:\n{e}")

    def _clear_database(self):
        """Clear all database records."""
        if messagebox.askyesno("Confirm", "Clear all database records?"):
            try:
                from database.repository import FeatureRepository
                repo = FeatureRepository()
                repo.clear_all()
                messagebox.showinfo("Success", "Database cleared.")
            except Exception as e:
                messagebox.showerror("Error", f"Failed to clear database:\n{e}")

    def _export_csv(self):
        """Export database to CSV."""
        path = filedialog.asksaveasfilename(
            defaultextension=".csv",
            filetypes=[("CSV files", "*.csv"), ("All files", "*.*")]
        )
        if path:
            try:
                from database.repository import FeatureRepository
                repo = FeatureRepository()
                repo.export_to_csv(path)
                messagebox.showinfo("Success", f"Exported to {path}")
            except Exception as e:
                messagebox.showerror("Error", f"Failed to export:\n{e}")
