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
- [Features](#features)
- [Support](#support)

## Installation

- All the `code` required to get started

### Clone

- Clone this repo to your local machine using `https://github.com/Emreozgun/TCP-IP-BASIC-DROPBOX-MODEL.git`
- Then open the terminal and go to the directory where the file is located.

### Run
- Run with **make all && ./server [directory] [threadPoolSize] [portnumber] && ./client [dirName] [ip address] [portnumber]** 
- If you want to clean executable files,you can use **make clean** 

## Features
  ### Server Side 
 > server [directory] [threadPoolSize] [portnumber]
 
 > where the directory is the servers specific area for file operations (there shouldn’t be multiple servers running on the same directory in the same computer), threadPoolSize is the maximum number of threads active at a time (meaning maximum number of active connected clients), portnumber is the port server will wait for connection.
 
 > The server side is  backup the files of the client by mirroring. While the client is connected to the server,   modifications done to client’s directory (add, delete or modify file) also be done in the server side. Therefore, the two directories are consistent while the connection is active. The server is able to handle multiple clients at the same time (a multi-threaded internet server). The server is also log the create, delete and update operations of all files in a log file under the corresponding directory reserved for the client. 
  
  ### Client Side 
 > client [dirName] [ip address] [portnumber]
 
 > where dirName is the name of the directory in the computer where client is called, and ip address and portnumber is the connection address and port of the server (ex. 10.1.18.44 18232). The path of the
directory identifies each client. When a client that was connected before connects again, it will receive missing files from the server if there is any. The files that were created or modified while the client is offline is detected and copied to the server side after connecting.
> 


**I like linux penguins :)**

![ttystudio GIF](https://media.giphy.com/media/4Zgy9QqzWU8C3ugvCa/giphy.gif)


  
## Support

Reach out to me at one of the following places!

- Linkedin at <a href="https://www.linkedin.com/in/emre-ozgun" target="_blank">`Emre_Ozgun_linkedin`</a>
- Stackoverflow at <a href="https://stackoverflow.com/users/12690037/emre-ozgun" target="_blank">`Emre_Ozgun_stackoverflow`</a>

---

