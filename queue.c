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
    if (!head) {
        return head;
    }
    INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    if (!l) {
        return;
    }
    element_t *cur_entry, *nxt_entry;
    list_for_each_entry_safe (cur_entry, nxt_entry, l, list) {
        q_release_element(cur_entry);
    }
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
        return false;
    }
    size_t len = strlen(s);
    addEntry->value = malloc(sizeof(char) * (len + 1));
    if (!addEntry->value) {
        q_release_element(addEntry);
        return false;
    }
    list_add(&addEntry->list, head);
    if (!addEntry->list.prev || !addEntry->list.next) {
        q_release_element(addEntry);
        return false;
    }
    strncpy(addEntry->value, s, len);
    *(addEntry->value + len) = '\0';
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head) {
        return false;
    }
    q_insert_head(head->prev, s);
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
    if (sp) {
        strncpy(sp, del_item->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    return del_item;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (head == NULL || list_empty(head)) {
        return NULL;
    }
    return q_remove_head(head->prev->prev, sp, bufsize);
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
    element_t *cur, *nxt;
    bool dup = false;
    list_for_each_entry_safe (cur, nxt, head, list) {
        if (&nxt->list != head && !strcmp(cur->value, nxt->value)) {
            list_del(&cur->list);
            q_release_element(cur);
            dup = true;
        } else {
            if (dup) {
                list_del(&cur->list);
                q_release_element(cur);
                dup = false;
            }
        }
    }

    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head || list_empty(head)) {
        return;
    }
    struct list_head *cur, *nxt;
    list_for_each_safe (cur, nxt, head) {
        if (nxt == head)
            return;
        list_move(cur, nxt);
        nxt = cur->next;
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head)) {
        return;
    }
    struct list_head *cur, *safe;
    list_for_each_safe (cur, safe, head) {
        list_move(cur, head);
    }
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    if (head == NULL || list_empty(head)) {
        return;
    }
    int count = 0;
    struct list_head *cur, *safe, *pre = head;
    list_for_each_safe (cur, safe, head) {
        list_move(cur, pre);
        count++;
        if (count == k) {
            count = 0;
            pre = safe->prev;
        }
    }
}
/* Merge two sorted list */
void q_merge_sorted(struct list_head *first,
                    struct list_head *second,
                    bool descend)
{
    if (!first || !second)
        return;
    // nodes of the second list insert into the first list and sorted.
    struct list_head *cur = first;
    for (; cur->next != first && !list_empty(second); cur = cur->next) {
        int val = strcmp(list_first_entry(cur, element_t, list)->value,
                         list_first_entry(second, element_t, list)->value);
        struct list_head *node = ((val > 0 && descend) || (val < 0 && !descend))
                                     ? cur->next
                                     : second->next;
        list_move(node, cur);
    }
    if (!list_empty(second))
        list_splice_init(second, cur);
}
struct list_head *q_middle(struct list_head *head)
{
    if (!head || list_empty(head))
        return NULL;
    struct list_head *first = head->next;
    struct list_head *last = head->prev;
    while (first != last && first->next != last) {
        first = first->next;
        last = last->prev;
    }
    return first;
}
/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;
    LIST_HEAD(right);
    struct list_head *mid = q_middle(head);
    list_cut_position(&right, mid, head->prev);
    q_sort(head, descend);
    q_sort(&right, descend);
    q_merge_sorted(head, &right, descend);
}
/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head) || list_is_singular(head))
        return 0;
    int cnt = 1;
    struct list_head *cur = head->prev;
    while (cur->prev != head) {
        element_t *pre_ele = list_entry(cur->prev, element_t, list);
        element_t *cur_ele = list_entry(cur, element_t, list);
        if (strcmp(pre_ele->value, cur_ele->value) >= 0) {
            list_del(&pre_ele->list);
            q_release_element(pre_ele);
        } else {
            cur = &pre_ele->list;
            cnt++;
        }
    }
    return cnt;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head) || list_is_singular(head))
        return 0;
    int cnt = 1;
    struct list_head *cur = head->prev;
    while (cur->prev != head) {
        element_t *pre_ele = list_entry(cur->prev, element_t, list);
        element_t *cur_ele = list_entry(cur, element_t, list);
        if (strcmp(pre_ele->value, cur_ele->value) <= 0) {
            list_del(&pre_ele->list);
            q_release_element(pre_ele);
        } else {
            cur = &pre_ele->list;
            cnt++;
        }
    }
    return cnt;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    if (!head || list_empty(head)) {
        return 0;
    }
    if (list_is_singular(head)) {
        return list_first_entry(head, queue_contex_t, chain)->size;
    }


    return 0;
}
