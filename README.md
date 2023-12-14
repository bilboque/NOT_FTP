# C asignement 2023 - TCP/IP CLIENT SERVER

## Project structure
Source files are in ./src folder.
Binary objects are in the ./out folder (including executables).
The server storage is in the ./myBigDataCenter folder, if you rename this folder:
- you have to change the STORAGE_PATH macro accordingly in the server source code.
- STORAGE_PATH can't be ./ because get and put will not function properly.

## How to run this project
This project requires the gnu readline library, you will have to download it to compile the project.
You maybe also have to change the LIB keyword in the Makefile to properly link the readline library.
Use the makefile to build the light server and run it in the same wd as the makefile.
(you can run this project with ./out/lightserver and ./out/client [ip] [port] or just ./out/client)

## Note on the implementation
The code i wrote include posix syscall for file handling (open, read, write, ...) so you have to run this code on a posix OS.
The fts_send utilitary function used by both client and server use the sendfile syscall wich isn't posix.(cf linux man 2 sendfile)
If you run this code on macos or openbsd system it might not work so you have to rewrite fts_send code to
follow posix norm.
Only send regular files from client to server (no pipes, symlink, blockdevice ...).
(it might work but i haven't run tests for those specific types of files)

