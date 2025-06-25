<!--
SPDX-FileCopyrightText: 2025 Thomas Mathys
SPDX-License-Identifier: MIT
-->

# TODO
* Set up github build action
* Obtain shrinkler code (4.7?)
* Start writing a wrapper around the data packer
  * Expose all options
  * For starters, running in memory is fine, respectively same interface as agbpack?
  * Need to figure out relatively early whether our old depacker code can cope with
    shrinkler 4.7 output
    * Can we not simply use our standard test file (lostmarbles?)
  * Obviously write tests. Once we have a real test, remove testthetest
* Choose license
  * Shrinkler-license for shrinkler code
  * MIT for our code? Or shrinkler for everything? Is probably simpler since we probably need to modify shrinkler code
