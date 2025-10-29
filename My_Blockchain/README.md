# Welcome to My Blockchain
***

## Task
  Blockchain is a command that allows for the creation and management of a blockchain. When the program starts (it loads a backup if there is one) then a prompt appears. This prompt allows to execute commands. When the commands are successful they display "ok" and if not, "nok: info" or info is an error message - see below:

add node nid add a nid identifier to the blockchain node.
rm node nid... remove nodes from the blockchain with a nid identifier. If nid is '*', then all nodes are impacted.
add block bid nid... add a bid identifier block to nodes identified by nid. If nid is '*', then all nodes are impacted.
rm block bid nid... remove the bid identified blocks from nodes identified by nid..
ls list all nodes by their identifiers. The option -l attaches the blocks bid's associated with each node.
sync synchronize all of the nodes with each other. Upon issuing this command, all of the nodes are composed of the same blocks.
quit save and leave the blockchain.

## Description
Error messages
1: no more resources available on the computer
2: this node already exists
3: this block already exists
4: node doesn't exist
5: block doesn't exist
6: command not found

++Great job on the project +I like how detailed the README is +Code is organized, clean, and works as expected

--Unfortunately you would need to create your own versions of sscanf and strcmp. Any function not listed under the list of approved functions needs to be self-implemented -I was told global variables are not allowed for this project. Please check in with a project manager to confirm

you must create a Makefile, and the output is the command itself
NID is an integer, BID is a string
You can use:
• malloc(3)

• free(3)

• printf(3)

• write(2)

• open(2)

• read(2)

• close(2)
Multiline macros are forbidden

Include another .c is forbidden

Macros with logic (while/if/variables/...) are forbidden
## Installation
you can install this project by this link: git@git.us.qwasar.io:my_blockchain_185018_yb6px1/my_blockchain.git

## Usage
How to use it:
1. make( in the terminal )
2. if you want to start from a scratch write: make fclean then: make
3. ./my_blockchain [you will see someting like this: [s0]>]
4. Commands you can use: Commands       Explanation      Usage
Command	Description	Example
add node <nid> ||	Add a new node with ID <nid>  ||  add node 1
rm node <nid>  ||	Remove node by ID   ||  rm node 1
rm node *  ||	Remove all nodes  ||  	rm node *
add block <bid> <nid>  ||	Add block <bid> to node <nid>  ||  	add block 101 1
add block <bid> *  ||	Add block <bid> to all nodes  ||  	add block 101 *
rm block <bid> <nid>  ||	Remove block <bid> from node <nid>  ||  	rm block 101 1
rm block <bid> *  ||	Remove block <bid> from all nodes  ||  	rm block 101 *
ls  ||	List all nodes (IDs only)  ||  	ls
ls -l  ||	List nodes with their blocks  ||  	ls -l
sync  ||	Synchronize all nodes’ blocks  ||  	sync
quit  ||	Save backup and exit  ||  	quit
5. then when you tested all this things the write: make fclean
```
./my_project argument1 argument2
```

### The Core Team


<span><i>Made at <a href='https://qwasar.io'>Qwasar SV -- Software Engineering School</a></i></span>
<span><img alt='Qwasar SV -- Software Engineering School's Logo' src='https://storage.googleapis.com/qwasar-public/qwasar-logo_50x50.png' width='20px' /></span>
