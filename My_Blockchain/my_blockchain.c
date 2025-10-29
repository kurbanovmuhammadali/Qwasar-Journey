
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "my_blockchain.h"

Node nodes[MAX_NODES];
int node_cnt = 0;
int sync_state = 1;

/* Convert string to int safely */
int str_to_int_safe(const char *s) {
    int res = 0, sign = 1, i = 0;
    if (s[0] == '-') { sign = -1; i++; }
    for (; s[i]; i++) {
        if (s[i] >= '0' && s[i] <= '9') res = res * 10 + (s[i] - '0');
        else break;
    }
    return res * sign;
}

void load_backup() {
    int fd = open(BACKUP_FILE, O_RDONLY);
    if (fd < 0) return;

    ssize_t ignored;
    ignored = read(fd, &node_cnt, sizeof(int));
    ignored = read(fd, &sync_state, sizeof(int));
    for (int i = 0; i < node_cnt; i++)
        ignored = read(fd, &nodes[i], sizeof(Node));
    close(fd);
}

void save_backup() {
    int fd = open(BACKUP_FILE, O_CREAT | O_TRUNC | O_WRONLY, 0666);
    ssize_t ignored;
    ignored = write(fd, &node_cnt, sizeof(int));
    ignored = write(fd, &sync_state, sizeof(int));
    for (int i = 0; i < node_cnt; i++)
        ignored = write(fd, &nodes[i], sizeof(Node));
    close(fd);
}

Node* find_node(int nid) {
    for (int i = 0; i < node_cnt; i++)
        if (nodes[i].nid == nid) return &nodes[i];
    return NULL;
}

int find_block(Node *n, int bid) {
    for (int i = 0; i < n->block_cnt; i++)
        if (n->blocks[i] == bid) return 1;
    return 0;
}

void cmd_add_node(int nid) {
    if (node_cnt >= MAX_NODES) { printf("nok: 1\n"); return; }
    if (find_node(nid)) { printf("nok: 2\n"); return; }
    nodes[node_cnt].nid = nid;
    nodes[node_cnt].block_cnt = 0;
    node_cnt++;
    printf("OK\n");
}

void cmd_rm_node(char *arg) {
    if (strcmp(arg, "*") == 0) {
        node_cnt = 0;
        sync_state = 1;
        printf("OK\n");
        return;
    }
    int nid = str_to_int_safe(arg);
    Node *n = find_node(nid);
    if (!n) { printf("nok: 4\n"); return; }
    int idx = n - nodes;
    for (int i = idx; i < node_cnt - 1; i++)
        nodes[i] = nodes[i + 1];
    node_cnt--;

    sync_state = (node_cnt <= 1) ? 1 : 0;
    printf("OK\n");
}

void cmd_add_block(int bid, char* nid_str) {
    int nid = str_to_int_safe(nid_str);
    int applied = 0;

    for (int i = 0; i < node_cnt; i++) {
        if (strcmp(nid_str, "*") != 0 && nodes[i].nid != nid) continue;
        if (find_block(&nodes[i], bid)) continue;
        if (nodes[i].block_cnt >= MAX_BLOCKS) { printf("nok: 1\n"); return; }
        nodes[i].blocks[nodes[i].block_cnt++] = bid;
        applied = 1;
    }

    if (!applied && !find_node(nid) && strcmp(nid_str, "*") != 0) {
        printf("nok: 4\n"); return;
    }

    if (applied && node_cnt > 1) sync_state = 0;
    printf("OK\n");
}

void cmd_rm_block(int bid, char* nid_str) {
    int nid = str_to_int_safe(nid_str);
    int removed = 0;
    for (int i = 0; i < node_cnt; i++) {
        if (strcmp(nid_str, "*") != 0 && nodes[i].nid != nid) continue;
        Node *n = &nodes[i];
        for (int j = 0; j < n->block_cnt; j++) {
            if (n->blocks[j] == bid) {
                for (int k = j; k < n->block_cnt - 1; k++)
                    n->blocks[k] = n->blocks[k + 1];
                n->block_cnt--;
                removed = 1;
                j--;
            }
        }
    }
    if (!removed) { printf("nok: 5\n"); return; }
    if (node_cnt > 1) sync_state = 0;
    printf("OK\n");
}

void cmd_ls(int detail) {
    for (int i = 0; i < node_cnt; i++) {
        printf("%d", nodes[i].nid);
        if (detail) {
            printf(":");
            for (int j = 0; j < nodes[i].block_cnt; j++) {
                printf(" %d", nodes[i].blocks[j]);
                if (j < nodes[i].block_cnt - 1) printf(",");
            }
        }
        printf("\n");
    }
}

void cmd_sync() {
    if (node_cnt <= 1) { sync_state = 1; printf("OK\n"); return; }

    int master_list[MAX_NODES * MAX_BLOCKS];
    int master_count = 0;
    for (int i = node_cnt - 1; i >= 0; i--) {
        for (int j = 0; j < nodes[i].block_cnt; j++) {
            int found = 0;
            for (int k = 0; k < master_count; k++)
                if (master_list[k] == nodes[i].blocks[j]) { found = 1; break; }
            if (!found) master_list[master_count++] = nodes[i].blocks[j];
        }
    }

    for (int i = 0; i < node_cnt; i++)
        for (int j = 0; j < master_count; j++)
            if (!find_block(&nodes[i], master_list[j]))
                nodes[i].blocks[nodes[i].block_cnt++] = master_list[j];

    sync_state = 1;
    printf("OK\n");
}

int main() {
    load_backup();
    char line[256];
    while (1) {
        printf("[%c%d]>", sync_state ? 's' : '-', node_cnt);
        if (!fgets(line, sizeof(line), stdin)) break;

        char cmd[16], arg1[32], arg2[32], arg3[32];
        cmd[0] = arg1[0] = arg2[0] = arg3[0] = '\0';
        int cnt = sscanf(line, "%15s %31s %31s %31s", cmd, arg1, arg2, arg3);
        if (cnt <= 0) continue;

        if (strcmp(cmd, "add") == 0) {
            if (cnt == 3 && strcmp(arg1, "node") == 0) cmd_add_node(str_to_int_safe(arg2));
            else if (cnt == 4 && strcmp(arg1, "block") == 0) cmd_add_block(str_to_int_safe(arg2), arg3);
            else printf("nok: 6\n");
        } else if (strcmp(cmd, "rm") == 0) {
            if (cnt == 3 && strcmp(arg1, "node") == 0) cmd_rm_node(arg2);
            else if (cnt == 4 && strcmp(arg1, "block") == 0) cmd_rm_block(str_to_int_safe(arg2), arg3);
            else printf("nok: 6\n");
        } else if (strcmp(cmd, "ls") == 0 && (cnt == 1 || cnt == 2)) {
            cmd_ls(cnt == 2 && strcmp(arg1, "-l") == 0);
        } else if (strcmp(cmd, "sync") == 0 && cnt == 1) {
            cmd_sync();
        } else if (strcmp(cmd, "quit") == 0 && cnt == 1) {
            save_backup();
            break;
        } else {
            printf("nok: 6\n");
        }
    }
    return 0;
}
