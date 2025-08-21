<!--
SPDX-FileCopyrightText: 2025 Thomas Mathys
SPDX-License-Identifier: MIT
-->

# TODO
* Start writing a wrapper around the data packer
  * When integrating into shrinkler-gba: remember that default endianness is big. For GBA we need to set it to little!
  * When removing shrinker and shrinklerwrapper from shrinkler-gba:
    * Carefully review what we're deleting. For instance there is some documentation scraped off of ada.untergrund.net which explains
      the decompressor a bit and which I'd like to keep.
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
* Choose license
  * Shrinkler-license for shrinkler code
  * MIT for our code? Or shrinkler for everything? Is probably simpler since we probably need to modify shrinkler code
