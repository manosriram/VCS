#### Control-System which helps file tracking and storing different versions of files.

```
Useful Way to deal with files. (Tested on UNIX)

View previous versions by -log command. (./vcs -log)
Copy the version's commit_id you need.
View the tree of that commit using -tree command. (./vcs -tree <commit_id>)
It shows all the files of that version.
Copy any file's hash and view content by -file command. (./vcs -file <file_hash>)
```

```
Commit Structure:
<tree_hash_of_commit> <commit_created_on>

<commit_message>
Commit <commit_id>
```

```
Tree Structure:
<file_hash> <path_to_file>
```


```
Usage: 
./vcs <options>
  
-log [Commit logs]
-view <commit_id> or <tree_id> [View Commit or Tree]
-commit <commit_message> [Take a snapshot of current repository status]
-tree <commit_id> [Shows the tree of that commit]
-file <file_hash> [View contents of file]
```

**Flow**
```
make
./vcs -commit "Your Commit Message"
./vcs -log
./vcs -tree <tree_hash_from_log>
./vcs -file <file_hash_from_tree> (Content of file of during that commit's stage)
```
