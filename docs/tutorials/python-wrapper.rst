Python Wrapper Workflow
=======================

This tutorial runs the Cython wrapper and verifies that the extension can
import, configure a model, run the C core, and clean up.

1. Build and install the wrapper:

   .. prompt:: bash

      make PYTHON=python3 all

2. Run the documented example:

   .. prompt:: bash

      python3 docs/examples/python_wrapper.py

3. Inspect the used-values file:

   .. prompt:: bash

      cat Output_python_example/wlcovpy_param.txt-usedvalues

Run the example from the repository root because it uses short relative paths
for ``clsfile`` and ``rootDir``.  The current C output-directory helper expects
relative output paths without embedded ``/`` characters.

The wrapper returns CPU time from ``Run``.  The final radial integral is
available through ``model.getIntegral()`` before ``clean_all`` releases the C
state.
