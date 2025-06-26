<!--
SPDX-FileCopyrightText: 2025 Thomas Mathys
SPDX-License-Identifier: MIT
-->

# TODO
* Fix reuse compliancy after adding Shrinkler sources
* Start writing a wrapper around the data packer
  * Expose all options
    * Add new ones, such as the parity thing?
  * For starters, running in memory is fine, respectively same interface as agbpack?
  * Need to figure out relatively early whether our old depacker code can cope with
    shrinkler 4.7 output
    * Can we not simply use our standard test file (lostmarbles?)
      * A first quick check shows
        * The size is about right, except that the file is not padded anymore
        * Byte by byte comparison not done because remember, shrinkler-gba outputs the bitstream
          little endian, whereas the original has it in big endian. Remember and implement this!
          * Note that if the amiga packer in the bitstream omits trailing zero and thus does not
            pad the file this will have no effect on *our* output size: in order to swap the
            endianness we have to add these zeroes again, since in our case they will *not*
            be at the end of the file
  * Obviously write tests. Once we have a real test, remove testthetest
* Choose license
  * Shrinkler-license for shrinkler code
  * MIT for our code? Or shrinkler for everything? Is probably simpler since we probably need to modify shrinkler code
