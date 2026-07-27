#!/bin/bash
PROJ_PATH="/home/aashrit/ChessAIV2"
VENV_PATH="${PROJ_PATH}/env"
PY_PACKAGES_PATH="${VENV_PATH}/lib/python3.14/site-packages"
BOARD_PATH="${PY_PACKAGES_PATH}/board"
source "${VENV_PATH}/bin/activate"
pip uninstall -y board
rm -rf "${BOARD_PATH}/*"
pip install "${PROJ_PATH}" --config-settings=cmake.build-type="Debug"
pybind11-stubgen board -o "${PY_PACKAGES_PATH}" --root-suffix=""
touch "${BOARD_PATH}/py.typed"
# Manually stub board module and add method signatures for Piece Enum, which pybind11-stubgen appears to be unable to handle
sed -i '/class Piece(enum.IntEnum):/a\
    def color(self) -> Color:\
        ...\
    def type(self) -> PieceType:\
        ...\
' ${BOARD_PATH}/board_interface.pyi
    

