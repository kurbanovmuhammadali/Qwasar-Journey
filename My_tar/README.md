# Welcome to My Tar
***

## Task
my_tar is a command to manipulate tape archive. The first option to tar is a mode indicator from the following list:

-c Create a new archive containing the specified items.
-r Like -c, but new entries are appended to the archive. The -f option is required.
-t List archive contents to stdout.
-u Like -r, but new entries are added only if they have a modification date newer than the corresponding entry in the archive. The -f option is required.
-x Extract to disk from the archive. If a file with the same name appears more than once in the archive, each copy will be extracted, with later copies overwriting (replacing) earlier copies.

## Description
The following creates a new archive called file.tar that contains two files source.c and source.h:

> ./my_tar -cf file.tar source.c source.h
To view a detailed table of contents for this archive:

> ./my_tar -tf file.tar
source.c
source.h
Errors handling Errors will be written on STDERR.
File not found (provided file is: i_don_t_exist): my_tar: i_don_t_exist: Cannot stat: No such file or directory

Error with the tarball file (provided file is: tarball.tar): my_tar: Cannot open tarball.tar

## Installation
you can install this project by this link: git@git.us.qwasar.io:my_tar_184911_jukeyf/my_tar.git

## Usage
How to test the code:
1. Open Terminal
2. make (write it in the terminal) and make file1 and file2 for testing the code 
3. ./my_tar -c -f archive.tar file1 file2 (you will something in the archive.tar)
4. ./my_tar -xf archive.tar 
5. ./my_tar -tf archive.tar
6. and another commands you can see on the internet
7. to clean the created file from make command: make fclean
```
./my_project argument1 argument2
```

### The Core Team


<span><i>Made at <a href='https://qwasar.io'>Qwasar SV -- Software Engineering School</a></i></span>
<span><img alt='Qwasar SV -- Software Engineering School's Logo' src='https://storage.googleapis.com/qwasar-public/qwasar-logo_50x50.png' width='20px' /></span>
