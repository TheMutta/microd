# Microd
An init system for linux that aims to be a functional replacement for a SysVInit-style init, but in a more modern and more flexible way.  
This is a personal project, don't use it in a production enviroment. Use it as a case study instead.
## How does it work?
The microd is composed by two programs:
 - init: the init system
 - initctl: a userspace utilty that communicates with PID 1.

## How does PID 1 work?
The init has three phases of operation:
 - **initrd phase**:
   It happens when an initramfs is used. It should load necessary kernel modules, mount the root device and pivot root.
 - **root phase**:
   It happens when the rootfs is mounted. Secondary disks are mounted, startup scripts are executed and daemons are launched.
 - **post-init phase**:
   It happens once the programs and daemons are launched. The init remains quiet and performs housekeeping jobs, such as controlling the status of children and restarting them if necessary. Also, it listens for any requests coming from initctl.

## Is it complete?
No, and probably it will never be, as I'll be continuously adding more features
