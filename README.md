![example workflow](https://github.com/kakalakola/ChecksumInator/actions/workflows/ChecksumInator.yml/badge.svg)

# ChecksumInator
A checksum calculator for 8-bit & 16-bit ROMs. Created for homebrew purposes.

New in version 1.0.2
  - As precaution, ChecksumInator now adds "_fixed" suffix to file name if no output file name is provided
    - "filename.ext" -> "filename_fixed.ext"
    - "filename" -> "filename_fixed"
    - ".filename" -> "_fixed.filename"

New in version 1.0.1
  - Added support for SEGA Genesis/Mega Drive/32X ROMs
    - Admittedly, some officially released Genesis/MD games seem to have bad checksums

New in version 0.8.0
  - Added basic support for SNES ROMs
  - Moved data array & file size to a struct

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

== We're Using GitHub Under Protest ==

This project is currently hosted on GitHub.  This is not ideal; GitHub is a proprietary, trade-secret system that is not Free and Open Souce Software (FOSS).  We 
are deeply concerned about using a proprietary system like GitHub to develop our FOSS project. We urge you to read about the [Give up GitHub](https://GiveUpGitHub.org) campaign from [the Software Freedom Conservancy](https://sfconservancy.org) to understand some of the reasons why GitHub is not
a good place to host FOSS projects.

![Logo of the GiveUpGitHub campaign](https://sfconservancy.org/img/GiveUpGitHub.png)
