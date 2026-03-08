"""Application configuration loaded from .env."""

from pathlib import Path
from dotenv import load_dotenv
import os

load_dotenv(Path(__file__).parent.parent / ".env")

OPENCV_DLL_PATH: str = os.getenv("OPENCV_DLL_PATH", "")
MSYS64_DLL_PATH: str = os.getenv("MSYS64_DLL_PATH", "")

USB_VID: str = os.getenv("USB_VID", "")
USB_BAUD_RATE: int = int(os.getenv("USB_BAUD_RATE", "115200"))
USB_TIMEOUT: float = float(os.getenv("USB_TIMEOUT", "1"))

DATABASE_PATH: str = os.getenv("DATABASE_PATH", "")
