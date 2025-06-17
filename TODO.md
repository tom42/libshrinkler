<!--
SPDX-FileCopyrightText: 2025 Thomas Mathys
SPDX-License-Identifier: MIT
-->

# TODO
* Set up source tree
  * Add src subdirectory
	* In there, do what? Well, another subdir libshrinkler, and in there have the production source code
	* That would be the place to set up our module
  * Set up test subdir, in there probably another subdir, with the unit tests
  * Honor all the options added to CMakeLists.txt
* Choose license
  * Shrinkler-license for shrinkler code
  * MIT for our code? Or shrinkler for everything? Is probably simpler since we probably need to modify shrinkler code
* Set up basic source tree structure, including main target and a test
* Automate the build
* Turn on all error messages, treat warnings as errors
* Set up reuse to check licensing stuff => reuse lint target in CMakeLists.txt
* Set up github build action
