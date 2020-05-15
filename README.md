#### Control-System which helps file tracking and storing different versions of files.

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
