"""SQLite repository for LEGO feature storage."""

import sqlite3
import csv
from pathlib import Path
from datetime import datetime
from typing import List, Tuple, Optional


class FeatureRepository:
    """Repository for storing and retrieving LEGO feature data."""

    def __init__(self, db_path: Optional[str] = None):
        if db_path is None:
            db_path = str(Path(__file__).parent.parent / "lego_features.db")
        self._db_path = db_path
        self._init_database()

    def _init_database(self):
        """Initialize the database schema."""
        with sqlite3.connect(self._db_path) as conn:
            conn.execute("""
                CREATE TABLE IF NOT EXISTS features (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    image_path TEXT NOT NULL,
                    image_name TEXT NOT NULL,
                    area REAL NOT NULL,
                    perimeter REAL NOT NULL,
                    aspect_ratio REAL NOT NULL,
                    circles INTEGER NOT NULL,
                    classification TEXT,
                    created_at TEXT NOT NULL
                )
            """)
            conn.commit()

    def save_features(
        self,
        image_path: str,
        image_name: str,
        area: float,
        perimeter: float,
        aspect_ratio: float,
        circles: int,
        classification: Optional[str] = None
    ) -> int:
        """Save extracted features to the database."""
        if classification is None:
            classification = self._classify(circles)

        created_at = datetime.now().isoformat()

        with sqlite3.connect(self._db_path) as conn:
            cursor = conn.execute(
                """
                INSERT INTO features (image_path, image_name, area, perimeter, aspect_ratio, circles, classification, created_at)
                VALUES (?, ?, ?, ?, ?, ?, ?, ?)
                """,
                (image_path, image_name, area, perimeter, aspect_ratio, circles, classification, created_at)
            )
            conn.commit()
            return cursor.lastrowid

    def get_all(self) -> List[Tuple]:
        """Retrieve all feature records."""
        with sqlite3.connect(self._db_path) as conn:
            cursor = conn.execute(
                """
                SELECT id, image_name, area, perimeter, aspect_ratio, circles, created_at
                FROM features
                ORDER BY created_at DESC
                """
            )
            return cursor.fetchall()

    def get_by_id(self, feature_id: int) -> Optional[Tuple]:
        """Retrieve a specific feature record by ID."""
        with sqlite3.connect(self._db_path) as conn:
            cursor = conn.execute(
                """
                SELECT id, image_path, image_name, area, perimeter, aspect_ratio, circles, classification, created_at
                FROM features
                WHERE id = ?
                """,
                (feature_id,)
            )
            return cursor.fetchone()

    def clear_all(self):
        """Delete all feature records."""
        with sqlite3.connect(self._db_path) as conn:
            conn.execute("DELETE FROM features")
            conn.commit()

    def export_to_csv(self, output_path: str):
        """Export all features to a CSV file."""
        with sqlite3.connect(self._db_path) as conn:
            cursor = conn.execute(
                """
                SELECT id, image_path, image_name, area, perimeter, aspect_ratio, circles, classification, created_at
                FROM features
                ORDER BY created_at DESC
                """
            )
            rows = cursor.fetchall()

        with open(output_path, "w", newline="", encoding="utf-8") as f:
            writer = csv.writer(f)
            writer.writerow([
                "id", "image_path", "image_name", "area", "perimeter",
                "aspect_ratio", "circles", "classification", "created_at"
            ])
            writer.writerows(rows)

    def _classify(self, circles: int) -> str:
        """Generate classification based on stud count."""
        classifications = {
            0: "Unknown",
            1: "1x1 Brick",
            2: "1x2 Brick",
            3: "1x3 Brick",
            4: "2x2 or 1x4 Brick",
            6: "2x3 or 1x6 Brick",
            8: "2x4 or 1x8 Brick",
        }

        if circles in classifications:
            return classifications[circles]
        elif circles > 8:
            return f"Large Brick ({circles} studs)"
        else:
            return f"{circles}-Stud Brick"
