## Table of Contents
- [Unix File System](#unix-file-system)
- [Architecture of File system](#architecture-of-file-system)
- [Installation](#installation)
- [Compile](#compile)
- [Commands](#commands)
- [Support](#support)

# Unix File System 

- In this project, It has been designed and implemented a simplifies UNIX like file system in C++. File system that uses i-nodes blocks and data blocks to keep your files and use, a i-node structure as single link i-nodes(not supports double link and triple link i-nodes).File attributes will include size, last
modification date and time, and name of the file. No permissions or owner attributes will be kept.
**Add Fig 4.33-Fig 4.34**
- Program that creates an empty file system as a 1 MB Linux file. This file includes all the information about file system including the i-nodes, data blocks, free blocks and i-nodes, directories, data, etc.
- The sample run of the program will be like  -> **makeFileSystem 4 400 mySystem.dat**

- where 4 is the block size of the file system in KB for both data blocks and i-node blocks, and 400 is the number of free i-nodes for an empty file system. mySystem.dat is the Linux file that contains all the file system. When you work on the file system, this file contains all the information for the file system. The size of mySystem.dat will be exactly 1 MB all the time whether it contains any information or not.

- And program that performs file system operation on the file system. 
You can observe all system operation -> [Commands usage](#commands-usage)


![Badges](https://img.shields.io/badge/linux-shell-green) 
![Badges](https://img.shields.io/badge/love-coding-black.svg)
![Badges](https://img.shields.io/badge/core-dumped-red)
![Badges](https://img.shields.io/badge/build-passing-succes.svg)
![Badges](https://img.shields.io/badge/test-success-success.svg)
![Badges](https://img.shields.io/badge/computer-science-critical.svg)
![Badges](https://img.shields.io/badge/love-linux-yellow.svg)
![Badges](https://img.shields.io/badge/coding-life-red.svg)
![Badges](https://img.shields.io/badge/open-source-blueviolet.svg)

---

## Architecture of File system
![arch_file_system](https://user-images.githubusercontent.com/30092986/93148066-94afe100-f6fb-11ea-8934-8326aaad16d5.png)

There are 5 main structures in the above file system. It is placed in 1mb file system in order from left to right. Superblock holds metadata about all file system. Bitmaps connects inodes and blocks to apply operations.It holds emptiness or fullness information specific inodes or blocks.(1 or 0).

The Inode describes a file system object such as file,directory,links etc. Each inode stores the **metadata**(last modification,name,size etc.) about objects,block locations. 

It has been placed unused part between Inode and blocks. Because the start of the superblock and the end of the file system should be at a modularly easy computable value. It has been calculated size that cannot be created as block.

Unused part = 1 mb - sizeof(Superblock + Bitmaps + Inodes + blocks)

To Superblock, It has been calculated number of blocks as block size and remaining from other informations(superblock,bitmaps,inode).

## Installation

- All the `code` required to get started

### Clone

- Clone this repo to your local machine using **https://github.com/Emreozgun/Unix-File-System.git**
- Then open the terminal and go to the directory where the file is located.

## Compile
- Compile and run with **g++ -Werror -o makeFileSystem makeFileSystem.cpp && ./makeFileSystem [BlockSize] [inodeCount] [fileSystem-Data file]** to create the file system.
- g++ -Werror -o p1 makeFileSystem.cpp  && g++ -Werror -o p2  fileSystemOper.cpp 
- Compile **g++ -Werror -o ./fileSystemOper  fileSystemOper.cpp** to perform operations in file system.
- Then usage that the following operations -> - [Commands usage](#commands-usage)
## Compile with debug mode 
- You can add **-DDEBUG** keyword end of the compile command  **g++ -Werror -o makeFileSystem makeFileSystem.cpp -DDEBUG**

## Commands
  - **list** : Lists the contents of the directory shown by path on the screen.
  - **mkdir** : Create the directory, if they do not already exist.
  - **rmdir** :  Remove directory if given directory is empty and path is correct.
  - **dumpe2fs** : List block count, inode count,free block and inodes, block size, number of files and directories and all occupies inodes some informations(blocks, filename)
  - **write** : Creates and writes data to the file.
  - **read** : Reads data from the file.
  - **del** : Deletes file from the path.

  ### Commands usage
  - **list**: ./fileSystemOper fileSystem.data list [path-Folder]

  - **mkdir**:	./fileSystemOper fileSystem.data mkdir [path-Folder] 

  - **rmdir**:	./fileSystemOper fileSystem.data rmdir [path-Folder]
		
  - **dumpe2fs**:./fileSystemOper fileSystem.data dumpe2fs  

  - **write**:	./fileSystemOper fileSystem.data write [path-File] [linuxFile] 

  - **read**:	./fileSystemOper fileSystem.data read [path-File] [linuxFile]

  - **del**:	./fileSystemOper fileSystem.data del [path-File]


## Support

Reach out to me at one of the following places!

- Linkedin at <a href="https://www.linkedin.com/in/emre-ozgun" target="_blank">`Emre_Ozgun_linkedin`</a>
- Stackoverflow at <a href="https://stackoverflow.com/users/12690037/emre-ozgun" target="_blank">`Emre_Ozgun_stackoverflow`</a>

---

