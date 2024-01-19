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
    // queue_contex_t * t = malloc(sizeof(queue_contex_t));


    // alloc mem for list_head
    struct list_head *ht = malloc(sizeof(struct list_head));

    ht->next = ht;
    ht->prev = ht;

    return ht;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    // use container of
    if (l == NULL)
        return;

    struct list_head *t = l->prev;

    /**
     * container_of() - Calculae address of structure that contains address ptr
     * @ptr: pointer to member variable
     * @type: type of the structure containing ptr
     * @member: name of the member variable in struct @type
     *
     * Return: @type pointer of structure containing ptr
     */

    if (t == l) {
        free(t);
        return;
    }

    do {
        element_t *et = container_of(t, element_t, list);
        t = t->prev;
        free(et->value);
        free(et);

    } while (t != l);

    free(l);


    return;
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    // false for allocation failed or queue is NULL
    if (head == NULL)
        return false;


    element_t *t = malloc(sizeof(element_t));

    // mem alloc for string new string
    char *st = (char *) malloc(sizeof(s));

    size_t i = 0;

    for (i = 0; s[i] != '\0'; i++) {
        st[i] = s[i];
    }

    st[i] = '\0';


    t->value = st;

    t->list.next = head->next;
    t->list.prev = head;


    // change the second element_t
    head->next->prev = &(t->list);
    head->next = &(t->list);


    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (head == NULL)
        return false;


    // set the new node
    element_t *t = malloc(sizeof(element_t));

    // avoid to use malloc(sizeof(*s)) !!!
    char *st = (char *) malloc(sizeof(s));

    size_t i = 0;

    for (i = 0; s[i] != '\0'; i++) {
        st[i] = s[i];
    }

    st[i] = '\0';

    t->value = st;



    t->list.next = head;
    t->list.prev = head->prev;



    // change the second node
    head->prev->next = &(t->list);
    head->prev = &(t->list);

    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (head == NULL)
        return NULL;
    else if (head->next == head)
        return NULL;
    struct list_head *t = head;


    // node needed to be removed
    t = t->next;
    element_t *et = container_of(t, element_t, list);
    // change the linked-list
    head->next = t->next;
    t->next->prev = head;


    // Set the st


    if (sp != NULL) {
        size_t i = 0;
        for (i = 0; et->value[i] != '\0'; i++) {
            sp[i] = et->value[i];
        }
        sp[i] = '\0';
    }



    return et;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (head == NULL)
        return NULL;
    else if (head->next == head)
        return NULL;
    struct list_head *t = head;
    // node needed to be removed
    t = t->prev;

    element_t *et = container_of(t, element_t, list);
    // change the linked-list
    head->prev = t->prev;
    t->prev->next = head;


    // Set the st


    if (sp != NULL) {
        size_t i = 0;
        for (i = 0; et->value[i] != '\0'; i++) {
            sp[i] = et->value[i];
        }
        sp[i] = '\0';
    }


    return et;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (head == NULL || list_empty(head))
        return 0;


    struct list_head *t = head->next;

    int len = 0;

    while (t != head) {
        len++;
        t = t->next;
    }


    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/

    if (head == NULL || list_empty(head))
        return false;



    int size = q_size(head);

    struct list_head *node = head->next;

    size /= 2;

    for (int i = 0; i < size; i++) {
        node = node->next;
    }

    element_t *et = container_of(node, element_t, list);

    list_del(node);

    free(et->value);
    free(et);



    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (head == NULL || list_empty(head))
        return false;



    // struct list_head *node = head->next;


    /*
    struct list_head *ni = head;
    struct list_head *nj = head;
    struct list_head *nt;


    list_for_each (ni, head) {
        element_t *eti = container_of(ni, element_t, list);

        list_for_each (nj, ni) {
            element_t *etj = container_of(nj, element_t, list);

            if (strcmp(eti->value, etj->value) == 0) {
                nt = nj->prev;
                list_del(nj);
                nj = nt;
                free(etj->value);
                free(etj);
            }
        }
    }*/
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
