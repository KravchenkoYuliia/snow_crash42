# Security Vulnerability for Level03

## Finding the Password for the `flag03` User

### Inspect file `level03`

```bash
ls -l
```
shows file permissions:

```bash
-rwsr-sr-x 1 flag03 level03 8627 Mar  5  2016 level03
```

#### What `s` means in permissions:

![s in permissions](images/s_permissions.png)

The `s` permission allows a file to run with the permissions of its owner or group instead of the permissions of the user who executes it.

It's useful if user need to use a program with root privileges, but we don't want to give this user the full access.

SUID - which stands for Set User ID - is a permission that applies only to files, not directories.
When you execute a script or file, you perform the task using the privileges of the current UID (User ID) - the owner of the file.

Unlike SUID, SGID applies to both files and directories.
SGID stands for Set Group ID.

It is important to note, however, that if a user runs a script they own that has the SGID bit set, the script executes with the privileges of the group to which it belongs.

For example, a script belonging to the `root` group could read from or write to files or directories that would normally be inaccessible to the user.

It gives the idea that the file owned by `flag03` user can let `level03` user execute smth that normally `level03` user doesn't have permission for.

