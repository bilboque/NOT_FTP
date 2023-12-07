# C asignement 2023

## Project structure
Source files are in ./src folder.
Binary objects are in the ./out folder (including executables).
The server storage is in the ./myBigDataCenter folder, if you rename this folder:
- you have to change the STORAGE_PATH macro accordingly in the server source code.
- STORAGE_PATH can't be ./ because the put command will be broken.

## How to run this project
Use the makefile then run server and client from the same wd as the makefile.
(with ./out/server and ./out/client [ip] [port] or just ./out/client)

## Note on the client server implementation
The c code i wrote incude posix syscall for file handling so you have to run this code on a posix OS.
The fts_send utilitary function used by both client and server use the sendfile syscall wich isn't posix.(cf linux man 2 sendfile)
If you run this code on macos or openbsd system it might not work so you have to rewrite fts_send code to
follow posix norm.
Only send regular files from client to server (no pipes, symlink, blockdevice ...). (it might work, didn't checked)

