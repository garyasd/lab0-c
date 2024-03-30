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
        printf("malloc faild");
        return NULL;
    }
    head->prev = head;
    head->next = head;
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    if (l == NULL) {
        return;
    }
    struct list_head *cur = l->next;
    while (cur != l) {
        element_t *del = container_of(cur, element_t, list);
        cur = cur->next;
        free(del->value);
        free(del);
    }
    free(l);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    element_t *listNode = (element_t *) malloc(sizeof(element_t));
    if (listNode == NULL) {
        printf("listNode malloc failed");
        return false;
    }
    // listNode store string
    int slen = strlen(s);
    listNode->value = (char *) malloc(sizeof(char) * (slen + 1));
    if (listNode->value == NULL) {
        printf("string malloc failed");
        free(listNode);
        return false;
    }
    strncpy(listNode->value, s, slen + 1);
    listNode->list.prev = head;
    listNode->list.next = head->next;
    head->next->prev = &listNode->list;
    head->next = &listNode->list;
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    element_t *listNode = (element_t *) malloc(sizeof(element_t));
    if (listNode == NULL) {
        printf("listNode malloc failed");
        return false;
    }
    // listNode store string
    int slen = strlen(s);
    listNode->value = (char *) malloc(sizeof(char) * (slen + 1));
    if (listNode->value == NULL) {
        printf("string malloc failed");
        free(listNode);
        return false;
    }
    strncpy(listNode->value, s, slen + 1);
    listNode->list.prev = head->prev;
    listNode->list.next = head;
    head->prev->next = &listNode->list;
    head->prev = &listNode->list;
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
    if (head == NULL) {
        printf("queue is empty.");
        return false;
    }
    struct list_head **slow = &head;
    struct list_head *fast = head->next;
    while (fast && fast->next) {
        fast = fast->next->next;
        slow = &(*slow)->next;
    }
    struct list_head *temp = *slow;
    (*slow)->next->prev = (*slow)->prev;
    *slow = (*slow)->next;
    free(temp);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (head == NULL || list_empty(head) || list_is_singular(head)) {
        return true;
    }
    struct list_head *first_list = head->next;
    bool is_dup = false;
    while (first_list != head) {
        element_t *first_element = list_entry(first_list, element_t, list);
        struct list_head *cur_list = first_list->next;
        while (cur_list != head) {
            element_t *cur_element = list_entry(cur_list, element_t, list);
            if (!strcmp(first_element->value, cur_element->value)) {
                list_del(cur_list);
                q_release_element(cur_element);
                is_dup = true;
            } else
                break;
            cur_list = first_list->next;
        }
        if (is_dup) {
            list_del(first_list);
            q_release_element(first_element);
            is_dup = false;
        }
        first_list = cur_list;
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
