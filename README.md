# Disk Image Solution Pack [![Build Status](https://travis-ci.org/ruebot/islandora_solution_pack_disk_image.png?branch=7.x)](https://travis-ci.org/ruebot/islandora_solution_pack_disk_image)

## Introduction

Adds all required Fedora objects to allow users to ingest and retrieve disk images through the Islandora interface.

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [sleuthkit](https://github.com/sleuthkit/sleuthkit)
* afflib-tools
* libafflib-dev
* libewf-dev
* ewf-tools

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

Sleuthkit install:

* `sudo apt-get install libafflib-dev afflib-tools libewf-dev ewf-tools`
* `git clone https://github.com/sleuthkit/sleuthkit.git`
* `cd sleuthkit`
* `./bootstrap`
* `./configure` (no need to declare `--with-afflib=dir` if you installed `libafflib-dev` and `afflib-tools` as described above)
* `make && sudo make install && sudo ldconfig`

**Note**: If you're on Ubuntu 12.04, you will need to [build](https://github.com/libyal/libewf/wiki/Building#using-debian-package-tools-deb) `libewf` from [source](https://github.com/libyal/libewf).

## Configuration

Set the path for `fiwalk` in Administration » Islandora » Solution pack configuation » Disk image (admin/islandora/solution_pack_config/disk_image).

## Troubleshooting

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers

[Nick Ruest](https://github.com/ruebot)

## Development

If you would like to contribute to this module, please check out our helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the Islandora.ca site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
