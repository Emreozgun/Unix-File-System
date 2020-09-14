# TCP-IP-BASIC-DROPBOX-MODEL

> In this project, There is client-server architecture model. The server is expected to backup according to the directory provided by the client. Clients can be online after connecting to the server via the same port and IP address, and all changes made on the client side must be reflected on the server side (Add / remove / update). The multi-thread server side should be able to serve more than one client at the same time. All processes are hold in the log file.

> This project is an application that runs through the console.

![Badges](https://img.shields.io/badge/linux-shell-green) 
![Badges](https://img.shields.io/badge/love-coding-black.svg)
![Badges](https://img.shields.io/badge/core-dumped-red)
![Badges](https://img.shields.io/badge/build-passing-succes.svg)
![Badges](https://img.shields.io/badge/test-success-success.svg)
![Badges](https://img.shields.io/badge/computer-science-critical.svg)
![Badges](https://img.shields.io/badge/love-linux-yellow.svg)
![Badges](https://img.shields.io/badge/coding-life-red.svg)
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
![Badges](https://img.shields.io/badge/open-source-blueviolet.svg)

---

## Table of Contents

- [Installation](#installation)
- [Run with Project](#run)
- [Commands](#commands)
- [Support](#support)

## Installation

- All the `code` required to get started

### Clone

- Clone this repo to your local machine using `https://github.com/Emreozgun/TCP-IP-BASIC-DROPBOX-MODEL.git`
- Then open the terminal and go to the directory where the file is located.

### Run with Project
- Compile and run with **g++ -Werror -o makeFileSystem makeFileSystem.cpp && ./makeFileSystem [BlockSize] [inodeCount] [fileSystem-Data file]** to create the file system.
- Compile **g++ -Werror -o ./fileSystemOper  fileSystemOper.cpp** to perform operations in file system.
- Then usage that the following operations -> - [Commands](#commands)

## Commands
  ### Server Side 
 > list:	./fileSystemOper fileSystem.data list [path-Folder] -> lists the contents of the root directory.

 > mkdir:	./fileSystemOper fileSystem.data mkdir [path-Folder] ->makes a new folder under the parent directory if given path is correct and not already exists.

 > rmdir:	./fileSystemOper fileSystem.data rmdir [path-Folder] -> remove folder if given folder is empty and path is correct.
		
 > dumpe2fs:./fileSystemOper fileSystem.data dumpe2fs -> list block count, inode count,free block and inodes, block size, number of files and directories and all 
occupies inodes some informations(blocks, filename)

 > write:	./fileSystemOper fileSystem.data write [path-File] [linuxFile] -> Creates a file named file under parent folder in file system, then copies the contents of the given file.
into the new file.

 > read:	./fileSystemOper fileSystem.data read [path-File] [linuxFile]

 > del:	./fileSystemOper fileSystem.data del [path-File]

**I like linux penguins :)**

![ttystudio GIF](https://media.giphy.com/media/4Zgy9QqzWU8C3ugvCa/giphy.gif)

## Support

Reach out to me at one of the following places!

- Linkedin at <a href="https://www.linkedin.com/in/emre-ozgun" target="_blank">`Emre_Ozgun_linkedin`</a>
- Stackoverflow at <a href="https://stackoverflow.com/users/12690037/emre-ozgun" target="_blank">`Emre_Ozgun_stackoverflow`</a>

---

