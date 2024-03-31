#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head =
        (struct list_head *) malloc(sizeof(struct list_head));
    if (head == NULL) {
        printf("malloc faild\n");
        return NULL;
    }
    INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    if (l == NULL) {
        return;
    }
    element_t *cur_entry = NULL, *nxt_entry = NULL;
    list_for_each_entry_safe (cur_entry, nxt_entry, l, list)
        q_release_element(cur_entry);
    free(l);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head) {
        return false;
    }
    element_t *addEntry = (element_t *) malloc(sizeof(element_t));
    if (!addEntry) {
        printf("entry malloc failed\n");
        return false;
    }
    // Entry value store string
    addEntry->value = strdup(s);
    if (!addEntry->value) {
        printf("string malloc failed");
        free(addEntry);
        return false;
    }
    list_add(&addEntry->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head) {
        return false;
    }
    element_t *addEntry = (element_t *) malloc(sizeof(element_t));
    if (!addEntry) {
        printf("Entry malloc failed");
        return false;
    }
    // Entry value store string
    addEntry->value = strdup(s);
    if (!addEntry->value) {
        printf("string malloc failed");
        free(addEntry);
        return false;
    }
    list_add_tail(&addEntry->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (head == NULL || list_empty(head)) {
        return NULL;
    }
    element_t *del_item = list_first_entry(head, element_t, list);
    list_del(&del_item->list);
    size_t value_len = strlen(del_item->value);
    if (!sp || bufsize <= value_len) {
        return NULL;
    }
    strncpy(sp, del_item->value, strlen(del_item->value));
    sp[value_len + 1] = '\0';
    return del_item;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (head == NULL || list_empty(head)) {
        return NULL;
    }
    element_t *del_item = list_last_entry(head, element_t, list);
    list_del(&del_item->list);
    size_t value_len = strlen(del_item->value);
    if (!sp || bufsize <= value_len) {
        return NULL;
    }
    strncpy(sp, del_item->value, strlen(del_item->value));
    sp[value_len + 1] = '\0';
    return del_item;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (head == NULL) {
        return 0;
    }
    int q_len = 0;
    struct list_head *cur;
    list_for_each (cur, head)
        q_len++;
    return q_len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (head == NULL || list_empty(head)) {
        return false;
    }
    struct list_head *first = head->next;
    struct list_head *last = head->prev;
    while (!(first == last || first->next == last)) {
        first = first->next;
        last = last->prev;
    }
    list_del(first);
    q_release_element(list_entry(first, element_t, list));
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (head == NULL || list_empty(head) || list_is_singular(head)) {
        return true;
    }
    struct list_head *prev_list = head->next;
    while (prev_list != head) {
        struct list_head *cur_list = prev_list->next;
        element_t *dup = list_entry(prev_list, element_t, list);
        if (cur_list == head ||
            strcmp(dup->value, list_entry(cur_list, element_t, list)->value)) {
            prev_list = cur_list;
            continue;
        }
        while (cur_list != head) {
            element_t *cur_element = list_entry(cur_list, element_t, list);
            if (!strcmp(dup->value, cur_element->value)) {
                list_del(cur_list);
                q_release_element(cur_element);
            } else
                break;
            cur_list = prev_list->next;
        }
        list_del(prev_list);
        q_release_element(dup);
        prev_list = cur_list;
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
