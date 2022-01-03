# ChecksumInator
A checksum calculator for 8-bit ROMs. Created for homebrew purposes.

New in version 0.6.1
  - Reworked the region & size validation code for Mark III/Master System/Game Gear ROMs
  - Started using structs with bit fields
    - Saves having to use AND to trim checksum to an 8-bit or 16-bit value
    - Allows for makeshift booleans

New in version 0.6.0
  - Rewrote the entire code in C for ... reasons
  - Removed header data display
  - ChecksumInator only writes the output file if data is changed in the source file
  - Currently supported systems
    - Game Boy/Game Boy Color/Super Game Boy
    - Game Boy Advance
    - Master System (Mark III and Game Gear ROMs do not require checksums)
