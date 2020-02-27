#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q)
        return NULL;
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;
    list_ele_t *tmp;
    while (q->head) {
        tmp = q->head->next;
        free(q->head->value);
        free(q->head);
        q->head = tmp;
    }
    free(q);
}
/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh;
    if (!q)
        return false;
    newh = (list_ele_t *) malloc(sizeof(list_ele_t));
    if (newh == NULL)
        return false;
    newh->value = (char *) malloc(sizeof(char) * (strlen(s) + 1));
    if (newh->value == NULL) {
        free(newh);
        return false;
    }
    strncpy(newh->value, s, strlen(s) + 1);

    newh->next = q->head;
    q->head = newh;

    q->size++;
    if (q->size == 1)
        q->tail = newh;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    list_ele_t *newt;
    if (!q)
        return false;
    newt = (list_ele_t *) malloc(sizeof(list_ele_t));
    if (newt == NULL)
        return false;
    newt->value = (char *) malloc(sizeof(char) * (strlen(s) + 1));
    if (newt->value == NULL) {
        free(newt);
        return false;
    }

    strncpy(newt->value, s, strlen(s) + 1);
    newt->next = NULL;
    q->tail->next = newt;
    q->tail = newt;

    q->size++;
    if (q->size == 1)
        q->head = newt;
    return true;
}
/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q)
        return false;
    if (q->size == 0)
        return false;
    if (sp)
        strncpy(sp, q->head->value, bufsize);

    list_ele_t *tmp = q->head;
    q->head = q->head->next;
    free(tmp->value);
    free(tmp);

    q->size--;
    if (q->size == 0)
        q->tail = NULL;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    return (q == NULL) ? 0 : q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (q == NULL)
        return;
    if (q->size <= 1)
        return;
    list_ele_t *tmp;
    q->tail->next = q->head;
    while (q->head->next != q->tail) {
        tmp = q->head->next;
        q->head->next = tmp->next;
        tmp->next = q->tail->next;
        q->tail->next = tmp;
    }
    q->tail = q->head;
    q->head = q->head->next;
    q->tail->next = NULL;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
list_ele_t *merge(list_ele_t *l1, list_ele_t *l2)
{
    if (l2 == NULL)
        return l1;
    if (l1 == NULL)
        return l2;
    list_ele_t *tmp;
    if (strcmp(l1->value, l2->value) < 0) {
        tmp = l1;
        l1 = l1->next;
    } else {
        tmp = l2;
        l2 = l2->next;
    }
    list_ele_t *re = tmp;
    while (l1 != NULL && l2 != NULL) {
        if (strcmp(l1->value, l2->value) < 0) {
            tmp->next = l1;
            l1 = l1->next;
        } else {
            tmp->next = l2;
            l2 = l2->next;
        }
        tmp = tmp->next;
    }
    if (l1)
        tmp->next = l1;
    if (l2)
        tmp->next = l2;
    return re;
}
list_ele_t *mergesort(list_ele_t *h)
{
    if (h == NULL || h->next == NULL)
        return h;
    list_ele_t *fast = h->next;
    list_ele_t *slow = h;
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }
    fast = slow->next;
    slow->next = NULL;
    list_ele_t *l1 = mergesort(h);
    list_ele_t *l2 = mergesort(fast);

    return merge(l1, l2);
}
void q_sort(queue_t *q)
{
    if (!q)
        return;
    if (q->size <= 1)
        return;
    mergesort(q->head);
}
