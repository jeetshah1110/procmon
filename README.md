# ProcMon
__Process Monitoring Tool__
- - - -
Process Monitoring Tool emphasizes on providing a detailed information of all the running processes(.exe files) on your Windows NT machine.

The tool provides the following features: 
1. Display all running processes.
2. Display thread information of processes.
3. Search a precific running process.
4. Kill/Terminate a specific running process.
5. Create Log File which contains all the information of every running process.
6. Display hardware information of current machine.
7. Display memory usage of each process.

Command | Description
------- | ------------------------------------------
ps      | display all information of process
ps      | display all memory information of process
ps -t   | display all information about threads
ps -d   | Display all information about DLL
cls     | Clear all contents of console
log     | Creates log of current running process on c drive
readlog | display the information from specified log file
sysinfo | Display the current hardware configuration
search  | Serach and display information of specific running process
exit    | Terminate Marvellous Procmon

_Hardware Requirements: _
* 32 bit Windows NT OS(XP onwards) with built-in Win32 APIs
* Min 512Mb RAM

Detailed information regarding Win32 APIs can be found on
> https://docs.microsoft.com/en-us/windows/desktop/api/tlhelp32/
