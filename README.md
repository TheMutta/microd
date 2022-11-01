# Microd
An init system for linux that aims to be a functional replacement for a SysVInit-style init, but in a more modern and more flexible way.  
This is a personal project, don't use it in a production enviroment. Use it as a case study instead.
## How does it work?
The init has three phases of operation:
 - initrd phase: this phase is happens when an initramfs is used. It should load necessary kernel modules, mount the root device and pivot root.
 - root phase: this phase is happens when the rootfs is mounted. Secondary disks are mounted, startup scripts are executed and daemons are launched.
 - post-init phase: this fase happens once the programs and daemons are launched. The init remains quiet and performs housekeeping jobs, such as controlling the status of children and restarting them if necessary.
## Is it complete?
No, and probably it will never be, as I'll be continuously adding more features
