#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */

typedef bool (*list_cmp_func_t)(void *,
                                const struct list_head *,
                                const struct list_head *);



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



    struct list_head *ni;
    struct list_head *nj;
    struct list_head *nt;


    for (ni = head->next; ni != head; ni = ni->next) {
        bool has_d = false;
        element_t *eti = container_of(ni, element_t, list);


        for (nj = ni->next; nj != head; nj = nj->next) {
            element_t *etj = container_of(nj, element_t, list);

            if (strcmp(eti->value, etj->value) == 0) {
                has_d = true;
                nt = nj->prev;
                list_del(nj);
                nj = nt;
                free(etj->value);
                free(etj);
            }
        }
        if (has_d) {
            nt = ni->prev;
            list_del(ni);
            ni = nt;
            free(eti->value);
            free(eti);
        }
    }
    return true;
}



/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    // https://leetcode.com/problems/reverse-linked-list/
    if (head == NULL || list_empty(head) || list_is_singular(head))
        return;

    struct list_head *node = head->next;

    struct list_head *nt;

    for (; node != head;) {
        nt = node->next;
        node->next = node->prev;
        node->prev = nt;
        node = nt;
    }

    nt = head->next;
    head->next = head->prev;
    head->prev = nt;

    return;
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    if (head == NULL || list_empty(head) || list_is_singular(head))
        return;
    if (k == 1)
        return;

    struct list_head *node = head->next;
    struct list_head *headn = NULL;
    struct list_head *tailn = NULL;
    struct list_head *lastntail = head;  // record the last group tail
    // struct list_head *lasthead = NULL;
    struct list_head *tn;  // temp node
    // set head and tail of size k list
    // struct list_head *lh = head;
    // struct list_head *lt = NULL;


    for (; node != head;) {
        for (int i = 0; i < k && node != head; i++) {
            if (i == 0) {
                tailn = node;  // record the first node to tail node
                tn = node->next;
                node->prev = tn;
                node = tn;

            } else if (i == k - 1 || node->next == head) {
                headn = node;  // record the last node to head node
                tn = node->next;
                node->next = node->prev;
                node = tn;

            } else {
                // exchange the link of prev and next of node
                tn = node->next;
                node->next = node->prev;
                node->prev = tn;
                node = tn;
            }
        }


        lastntail->next = headn;  // let last k node link the first node
        headn->prev = lastntail;  // let first node link the last tail node
        lastntail = tailn;        // record the last group tail
    }

    // link the head prev
    head->prev = tailn;
    tailn->next = head;


    return;
}



bool cmp(void *priv, const struct list_head *a, const struct list_head *b)
{
    if (*(bool *) priv == true) {
        if (strcmp(container_of(a, element_t, list)->value,
                   container_of(b, element_t, list)->value) < 0)
            return false;
        else if (strcmp(container_of(a, element_t, list)->value,
                        container_of(b, element_t, list)->value) > 0)
            return true;
    } else {
        if (strcmp(container_of(a, element_t, list)->value,
                   container_of(b, element_t, list)->value) < 0)
            return true;
        else if (strcmp(container_of(a, element_t, list)->value,
                        container_of(b, element_t, list)->value) > 0)
            return false;
    }

    return true;
}

/*
 * Returns a list organized in an intermediate format suited
 * to chaining of merge() calls: null-terminated, no reserved or
 * sentinel head node, "prev" links not maintained.
 */

struct list_head *merge(void *priv,
                        list_cmp_func_t cmp,
                        struct list_head *a,
                        struct list_head *b)
{
    struct list_head *head = NULL, **tail = &head;

    for (;;) {
        /* if equal, take 'a' -- important for sort stability */
        if (cmp(priv, a, b) == true) {
            *tail = a;
            tail = &a->next;
            a = a->next;
            if (!a) {
                *tail = b;
                break;
            }
        } else {
            *tail = b;
            tail = &b->next;
            b = b->next;
            if (!b) {
                *tail = a;
                break;
            }
        }
    }
    return head;
}

void merge_final(void *priv,
                 list_cmp_func_t cmp,
                 struct list_head *head,
                 struct list_head *a,
                 struct list_head *b)
{
    struct list_head *tail = head;
    int count = 0;

    for (;;) {
        /* if equal, take 'a' -- important for sort stability */
        if (cmp(priv, a, b) == true) {
            tail->next = a;
            a->prev = tail;
            tail = a;
            a = a->next;
            if (!a)
                break;
        } else {
            tail->next = b;
            b->prev = tail;
            tail = b;
            b = b->next;
            if (!b) {
                b = a;
                break;
            }
        }
    }

    /* Finish linking remainder of list b on to tail */
    tail->next = b;
    do {
        /*
         * If the merge is highly unbalanced (e.g. the input is
         * already sorted), this loop may run many iterations.
         * Continue callbacks to the client even though no
         * element comparison is needed, so the client's cmp()
         * routine can invoke cond_resched() periodically.
         */
        if (unlikely(!++count))
            cmp(priv, b, b);
        b->prev = tail;
        tail = b;
        b = b->next;
    } while (b);

    /* And the final links to make a circular doubly-linked list */
    tail->next = head;
    head->prev = tail;
}

void list_sort(void *priv, struct list_head *head, list_cmp_func_t cmp)
{
    struct list_head *list = head->next, *pending = NULL;
    size_t count = 0; /* Count of pending */

    if (list == head->prev) /* Zero or one elements */
        return;

    /* Convert to a null-terminated singly-linked list. */
    head->prev->next = NULL;

    /*
     * Data structure invariants:
     * - All lists are singly linked and null-terminated; prev
     *   pointers are not maintained.
     * - pending is a prev-linked "list of lists" of sorted
     *   sublists awaiting further merging.
     * - Each of the sorted sublists is power-of-two in size.
     * - Sublists are sorted by size and age, smallest & newest at front.
     * - There are zero to two sublists of each size.
     * - A pair of pending sublists are merged as soon as the number
     *   of following pending elements equals their size (i.e.
     *   each time count reaches an odd multiple of that size).
     *   That ensures each later final merge will be at worst 2:1.
     * - Each round consists of:
     *   - Merging the two sublists selected by the highest bit
     *     which flips when count is incremented, and
     *   - Adding an element from the input as a size-1 sublist.
     */
    do {
        size_t bits;
        struct list_head **tail = &pending;

        /* Find the least-significant clear bit in count */
        for (bits = count; bits & 1; bits >>= 1)
            tail = &(*tail)->prev;
        /* Do the indicated merge */
        if (likely(bits)) {
            struct list_head *a = *tail, *b = a->prev;

            a = merge(priv, cmp, b, a);
            /* Install the merged result in place of the inputs */
            a->prev = b->prev;
            *tail = a;
        }

        /* Move one element from input list to pending */
        list->prev = pending;
        pending = list;
        list = list->next;
        pending->next = NULL;
        count++;
    } while (list);

    /* End of input; merge together all the pending lists. */
    list = pending;
    pending = pending->prev;
    for (;;) {
        struct list_head *next = pending->prev;

        if (!next)
            break;
        list = merge(priv, cmp, pending, list);
        pending = next;
    }
    /* The final merge, rebuilding prev links */
    merge_final(priv, cmp, head, pending, list);
}
/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (head == NULL || list_empty(head) || list_is_singular(head))
        return;

    /**
     * list_cut_position() - Move beginning of a list to another list
     * @head_to: pointer to the head of the list which receives nodes
     * @head_from: pointer to the head of the list
     * @node: pointer to the node in which defines the cutting point
     *
     * All entries from the beginning of the list @head_from to (including) the
     * @node is moved to @head_to.
     *
     * @head_to is replaced when @head_from is not empty. @node must be a real
     * list node from @head_from or the behavior is undefined.
     */
    // void *priv = &descend;
    list_sort(&descend, head, cmp);
}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (head == NULL || list_empty(head))
        return false;

    // struct list_head *node = head->next;

    struct list_head *ni;
    struct list_head *nj;
    struct list_head *nt;

    for (ni = head->next; ni != head; ni = ni->next) {
        element_t *eti = container_of(ni, element_t, list);


        for (nj = ni->next; nj != head; nj = nj->next) {
            element_t *etj = container_of(nj, element_t, list);

            if (strcmp(eti->value, etj->value) >= 0) {
                nt = nj->prev;
                list_del(nj);
                nj = nt;
                free(etj->value);
                free(etj);
            }
        }
    }
    return true;



    return q_size(head);
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (head == NULL || list_empty(head))
        return false;

    // struct list_head *node = head->next;

    struct list_head *ni;
    struct list_head *nj;
    struct list_head *nt;

    for (ni = head->next; ni != head; ni = ni->next) {
        element_t *eti = container_of(ni, element_t, list);


        for (nj = ni->next; nj != head; nj = nj->next) {
            element_t *etj = container_of(nj, element_t, list);

            if (strcmp(eti->value, etj->value) <= 0) {
                nt = nj->prev;
                list_del(nj);
                nj = nt;
                free(etj->value);
                free(etj);
            }
        }
    }


    return q_size(head);


    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    // queue_contex_t *contex = container_of(head, queue_contex_t, chain);
    struct list_head *h = head->next;


    for (; h != head; h = h->next) {
        list_splice_tail(head, h);
    }



    return 0;
}
