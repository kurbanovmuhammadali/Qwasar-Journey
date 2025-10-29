# Welcome to My Ls
***

## Task
The ls program lists information about files (of any type, including directories). Options and file arguments can be intermixed arbitrarily, as usual. Later options override earlier options that are incompatible.

For non-option command-line arguments that are directories, by default ls lists the contents of directories, not recursively, and omitting files with names beginning with ‘.’. For other non-option arguments, by default ls lists just the file name. If no non-option argument is specified, ls operates on the current directory, acting as if it had been invoked with a single argument of ‘.’.

## Description
These options determine which files ls lists information for. By default, ls lists files and the contents of any directories on the command line, except that in directories it ignores files whose names start with ‘.’.
‘-a’
‘--all’
In directories, do not ignore file names that start with ‘.’.

‘-A’
‘--almost-all’
In directories, do not ignore all file names that start with ‘.’; ignore only . and ... The --all (-a) option overrides this option.

## Installation
you can install my project by this link: git@git.us.qwasar.io:my_ls_184723_c5mvky/my_ls.git

## Usage
to run my program follow this steps:
1. open terminal
2. gcc -o my_ls my_ls.c
3. ./my_ls (for showing all current files)
4. ./my_ls -a ( for showing also hidden files)
5. ./my_ls -t ( for sorting files with time )
6. and there are other combination but if you want to look at them text me kurbanov_m discord
```
./my_project argument1 argument2
```

### The Core Team


<span><i>Made at <a href='https://qwasar.io'>Qwasar SV -- Software Engineering School</a></i></span>
<span><img alt='Qwasar SV -- Software Engineering School's Logo' src='https://storage.googleapis.com/qwasar-public/qwasar-logo_50x50.png' width='20px' /></span>
