"""Sphinx configuration for the wlcov documentation."""

from __future__ import annotations

import os
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT))

project = "wlcov"
author = "Sofia Samario-Nava, Alejandro Aviles, and contributors"
copyright = "2026, wlcov contributors"
release = "1.0.0"
version = release
root_doc = "index"

extensions = [
    "sphinx.ext.autosectionlabel",
    "sphinx.ext.intersphinx",
    "sphinx.ext.mathjax",
    "sphinx.ext.napoleon",
    "sphinx.ext.todo",
    "sphinx-prompt",
]

autosectionlabel_prefix_document = True
templates_path = ["_templates"]
exclude_patterns = ["_build", "Thumbs.db", ".DS_Store", "man/wlcov.html"]

html_theme = "sphinx_rtd_theme"
html_title = "wlcov documentation"
html_show_sourcelink = True
html_context = {
    "display_github": True,
    "github_user": "rodriguezmeza",
    "github_repo": "wlcov",
    "github_version": "main",
    "conf_py_path": "/docs/",
}

pygments_style = "sphinx"
todo_include_todos = False

man_pages = [
    (
        "index",
        "wlcov",
        "wlcov documentation",
        [author],
        1,
    )
]

latex_documents = [
    (
        "index",
        "wlcov.tex",
        "wlcov Documentation",
        author,
        "manual",
    )
]

latex_elements = {
    "papersize": "letterpaper",
    "pointsize": "10pt",
}

intersphinx_mapping = {
    "python": ("https://docs.python.org/3", None),
    "numpy": ("https://numpy.org/doc/stable/", None),
}

rst_epilog = """
.. _DESC Coding Guidelines: https://lsstdesc.org
.. _emcee documentation: https://emcee.readthedocs.io/en/stable/
.. _LSST DM Developer Guide: https://developer.lsst.io/
.. _GNU Scientific Library: https://www.gnu.org/software/gsl/
.. _Read the Docs: https://readthedocs.org/
"""
