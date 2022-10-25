# Copyright 2022 The Foundry Visionmongers Ltd
# SPDX-License-Identifier: Apache-2.0

import sys

try:
    import openassetio
    from openassetio.hostApi import ManagerFactory
except ImportError as exc:
    print("Error importing openassetio package: {}".format(exc))
    print("Using python executable: {}".format(sys.executable))
    print("With PYTHONPATH:")
    for i in sys.path:
        print("\t{}".format(i))
    sys.exit(1)

print("openassetio found at {}".format(openassetio.__file__))
sys.exit(0)
