import os
import sys
import re

# Add project root to the Python path
sys.path.insert(0, os.path.abspath('../../screamer'))  # Adjust the relative path as necessary


# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'screamer'
copyright = '2024, Thijs van den Berg, Mohammadjavad Vakili'
author = 'Thijs van den Berg, Mohammadjavad Vakili'
release = '0.1.31'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.napoleon',
    'sphinx.ext.mathjax',
    'sphinx.ext.viewcode',
    'sphinx_autodoc_typehints',  # To capture type hints
    'myst_parser', # markdown    
]

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

myst_enable_extensions = [
    "colon_fence",  # Optional extensions, you can enable more if needed
    "amsmath",      # For parsing LaTeX-style math
]

source_suffix = {
    '.rst': 'restructuredtext',
    '.md': 'markdown',  # Ensure Markdown files are recognized
}


# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output
html_theme = 'sphinx_rtd_theme'

html_static_path = ['_static']

# Display the type hints but hide the 'self' argument
autodoc_typehints = "description"  # Shows type hints in the description, not in the signature
autodoc_typehints_format = "short"  # Simplifies the displayed types (e.g., 'numpy.ndarray' instead of the full path)
autodoc_default_options = {
    'special-members': '__init__, __call__',  # Include both __call__ and __init__ in the documentation    
}
#autodoc_member_order = 'bysource'

def remove_self_and_simplify_signature(app, what, name, obj, options, signature, return_annotation):
    if signature:
        # Remove 'self: <type>, ' pattern
        signature = re.sub(r'\(self: [^,]+, ', '(', signature)
        signature = re.sub(r'\(self: [^,]+\)', '()', signature)
        
        # Simplify numpy type hints in the signature
        signature = signature.replace('numpy.ndarray[numpy.float64]', 'numpy.ndarray')
    
    if return_annotation:
        # Simplify numpy type hints in the signature
        return_annotation = return_annotation.replace('numpy.ndarray[numpy.float64]', 'numpy.ndarray')    

    return signature, return_annotation

def simplify_type_hints(app, what, name, obj, options, lines):
    """
    This function modifies the docstring, specifically the type annotations
    inside the docstring, and simplifies them for better readability.
    """
    # Go through each line of the docstring and replace verbose types
    for i, line in enumerate(lines):
        # Replace verbose numpy types with simpler ones
        lines[i] = line.replace('numpy.ndarray[numpy.float64]', 'numpy.ndarray')
        # You can add more replacements here as needed


def setup(app):
    app.connect('autodoc-process-signature', remove_self_and_simplify_signature)
    app.connect('autodoc-process-docstring', simplify_type_hints)
    pass