import warnings
import logging

# Suppress Python deprecation warnings about testing element truth values
warnings.filterwarnings("ignore", category=DeprecationWarning, message=".*Testing an element's truth value.*")

# Suppress warnings about external images not found in XML output
warnings.filterwarnings("ignore", message=".*was not found in XML_OUTPUT", category=RuntimeWarning)
warnings.filterwarnings("ignore", message=".*was not found in XML_OUTPUT", category=UserWarning)

# Suppress the specific logging messages about external images in m.css
class ImageNotFoundFilter(logging.Filter):
    def filter(self, record):
        # Suppress specific image not found messages
        if "was not found in XML_OUTPUT" in record.getMessage():
            return False
        return True

# Apply the filter to the root logger
logging.getLogger().addFilter(ImageNotFoundFilter())

DOXYFILE = 'Doxyfile'

LINKS_NAVBAR1 = [
    (None, 'pages', [(None, 'about')]),
    (None, 'namespaces', []),
]

# Add your own navbar links using the code below.
# To find the valid link names, you can inspect the URL of a generated documentation site.

# LINKS_NAVBAR1 = [
#     (None, 'pages', [(None, 'about')]),
#     (None, 'namespaces', [(None, 'namespacexnetwork')]),
# ]
#
# LINKS_NAVBAR2 = [
#     (None, 'annotated', [(None, 'classxnetwork_1_1_xnetwork')]),
#     (None, 'files', [(None, 'xnetwork_8h')]),
# ]
