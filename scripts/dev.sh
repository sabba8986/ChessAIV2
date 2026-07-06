#!/bin/bash
PROJ_PATH="/home/aashrit/ChessAIV2"
VENV_PATH="${PROJ_PATH}/env"
PY_PACKAGES_PATH="${VENV_PATH}/lib/python3.14/site-packages"
BOARD_PATH="${PY_PACKAGES_PATH}/board"
source "${VENV_PATH}/bin/activate"
pip uninstall board
rm -f "${BOARD_PATH}/*"
pip install "${PROJ_PATH}"
pybind11-stubgen board -o "${PY_PACKAGES_PATH}" --root-suffix=""
touch "${BOARD_PATH}/py.typed"

