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
    if (q) {
        q->head = NULL;
        q->tail = NULL;
        q->size = 0;
    }
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;
    while (q->head) {
        list_ele_t *prev = q->head;
        q->head = q->head->next;
        free(prev->value);
        free(prev);
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

    newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;

    newh->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (!newh->value) {
        free(newh);
        return false;
    }
    strncpy(newh->value, s, strlen(s) + 1);
    newh->next = q->head;
    q->head = newh;
    if (!q->tail)
        q->tail = newh;
    q->size++;
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
    if (!q)
        return false;
    list_ele_t *newt = malloc(sizeof(list_ele_t));
    if (!newt)
        return false;
    newt->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (!newt->value) {
        free(newt);
        return false;
    }
    strncpy(newt->value, s, strlen(s) + 1);
    newt->next = NULL;
    if (q->tail)
        q->tail->next = newt;
    q->tail = newt;
    if (!q->head)
        q->head = q->tail;
    q->size++;
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
    if (!q || !(q->head)) {
        return false;
    }
    list_ele_t *tmp = q->head;
    q->head = q->head->next;
    tmp->next = NULL;
    // sp is non-NULL, check real bufsize
    if (sp) {
        size_t realbufsize =
            strlen(tmp->value) < bufsize ? strlen(tmp->value) : bufsize;
        strncpy(sp, tmp->value, realbufsize);
        sp[bufsize - 1] = '\0';
    }
    free(tmp->value);
    free(tmp);
    q->size--;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q)
        return 0;
    return q->size;
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
    if (!q || !(q->head))
        return;
    // set tail position before do sth with q->head
    q->tail = q->head;
    list_ele_t *cursor = NULL;
    while (q->head) {
        // next just use in this block, declare here is safety
        list_ele_t *next = q->head->next;
        q->head->next = cursor;
        cursor = q->head;
        q->head = next;
    }
    q->head = cursor;
}


void merge_sort(list_ele_t **head)
{
    // queue element size is less then 2
    if (!(*head) || !((*head)->next))
        return;
    list_ele_t *left = *head;
    list_ele_t *right = (*head)->next;
    while (right && right->next) {
        left = left->next;
        right = right->next->next;
    }
    right = left->next;
    left->next = NULL;
    left = (*head);
    merge_sort(&left);
    merge_sort(&right);


    list_ele_t *newlist = NULL;


    while (left && right) {
        if (strcmp(left->value, right->value) < 0) {
            if (newlist) {
                newlist->next = left;
                newlist = newlist->next;
            } else {
                newlist = left;
                *head = newlist;
            }
            left = left->next;
        } else {
            if (newlist) {
                newlist->next = right;
                newlist = newlist->next;
            } else {
                newlist = right;
                *head = newlist;
            }
            right = right->next;
        }
    }
    newlist->next = (left) ? left : right;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    /* TODO: Remove the above comment when you are about to implement. */
    if (!q || q->size < 2)
        return;
    merge_sort(&q->head);
    // bubble_sort(&q->head,q->size);

    // update q->tail
    while (q->tail->next)
        q->tail = q->tail->next;
}
