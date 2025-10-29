
#ifndef MY_BLOCKCHAIN_H
#define MY_BLOCKCHAIN_H

#define MAX_NODES 100
#define MAX_BLOCKS 100
#define BACKUP_FILE "blockchain.bak"

/* Struct Node (different name from first project) */
typedef struct {
    int nid;                    /* Node ID */
    int blocks[MAX_BLOCKS];      /* Block IDs */
    int block_cnt;               /* Number of blocks */
} Node;

/* Global variables */
extern Node nodes[MAX_NODES];
extern int node_cnt;
extern int sync_state;

/* Backup functions */
void load_backup(void);
void save_backup(void);

/* Node commands */
void cmd_add_node(int nid);
void cmd_rm_node(char *arg);
void cmd_add_block(int bid, char *arg);
void cmd_rm_block(int bid, char *arg);
void cmd_ls(int detail);
void cmd_sync(void);

/* Helper functions */
Node* find_node(int nid);
int find_block(Node *n, int bid);

/* Utility */
int str_to_int_safe(const char *s);

#endif
