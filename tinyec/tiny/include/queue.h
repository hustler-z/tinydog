/**
 * Hustler's Project
 *
 * File:  queue.h
 * Date:  2024/05/22
 * Usage:
 */

#ifndef _TINY_QUEUE_H_
#define _TINY_QUEUE_H_

// ####################################################

/*
 * Tail queue definitions.
 */
#define _TAILQ_HEAD(name, type, qual)                           \
struct name {                                                   \
    qual type *tqh_first;       /* first element */             \
    qual type *qual *tqh_last;  /* addr of last next element */ \
}
#define TAILQ_HEAD(name, type) _TAILQ_HEAD(name, struct type,)

#define TAILQ_HEAD_INITIALIZER(head)                            \
    { NULL, &(head).tqh_first }

#define _TAILQ_ENTRY(type, qual)                                \
struct {                                                        \
    qual type *tqe_next;        /* next element */              \
    qual type *qual *tqe_prev;  /* addr of prev next element */ \
}
#define TAILQ_ENTRY(type) _TAILQ_ENTRY(struct type,)

/*
 * Tail queue functions.
 */
#define QUEUEDEBUG_TAILQ_INSERT_HEAD(head, elm, field)
#define QUEUEDEBUG_TAILQ_INSERT_TAIL(head, elm, field)
#define QUEUEDEBUG_TAILQ_OP(elm, field)
#define QUEUEDEBUG_TAILQ_PREREMOVE(head, elm, field)
#define QUEUEDEBUG_TAILQ_POSTREMOVE(elm, field)

#define TAILQ_INIT(head) do {                                   \
    (head)->tqh_first = NULL;                                   \
    (head)->tqh_last = &(head)->tqh_first;                      \
} while (/* CONSTCOND */0)

#define TAILQ_INSERT_HEAD(head, elm, field) do {                \
    QUEUEDEBUG_TAILQ_INSERT_HEAD((head), (elm), field)          \
    if (((elm)->field.tqe_next = (head)->tqh_first) != NULL)    \
        (head)->tqh_first->field.tqe_prev =                     \
            &(elm)->field.tqe_next;                             \
    else                                                        \
        (head)->tqh_last = &(elm)->field.tqe_next;              \
    (head)->tqh_first = (elm);                                  \
    (elm)->field.tqe_prev = &(head)->tqh_first;                 \
} while (/* CONSTCOND */0)

#define TAILQ_INSERT_TAIL(head, elm, field) do {                \
QUEUEDEBUG_TAILQ_INSERT_TAIL((head), (elm), field)              \
    (elm)->field.tqe_next = NULL;                               \
    (elm)->field.tqe_prev = (head)->tqh_last;                   \
    *(head)->tqh_last = (elm);                                  \
    (head)->tqh_last = &(elm)->field.tqe_next;                  \
} while (/* CONSTCOND */0)

#define TAILQ_INSERT_AFTER(head, listelm, elm, field) do {      \
    QUEUEDEBUG_TAILQ_OP((listelm), field)                       \
    if (((elm)->field.tqe_next = (listelm)->field.tqe_next)     \
        != NULL)                                                \
        (elm)->field.tqe_next->field.tqe_prev =                 \
            &(elm)->field.tqe_next;                             \
    else                                                        \
        (head)->tqh_last = &(elm)->field.tqe_next;              \
    (listelm)->field.tqe_next = (elm);                          \
    (elm)->field.tqe_prev = &(listelm)->field.tqe_next;         \
} while (/* CONSTCOND */0)

#define TAILQ_INSERT_BEFORE(listelm, elm, field) do {           \
    QUEUEDEBUG_TAILQ_OP((listelm), field)                       \
    (elm)->field.tqe_prev = (listelm)->field.tqe_prev;          \
    (elm)->field.tqe_next = (listelm);                          \
    *(listelm)->field.tqe_prev = (elm);                         \
    (listelm)->field.tqe_prev = &(elm)->field.tqe_next;         \
} while (/* CONSTCOND */0)

#define TAILQ_REMOVE(head, elm, field) do {                     \
    QUEUEDEBUG_TAILQ_PREREMOVE((head), (elm), field)            \
    QUEUEDEBUG_TAILQ_OP((elm), field)                           \
    if (((elm)->field.tqe_next) != NULL)                        \
        (elm)->field.tqe_next->field.tqe_prev =                 \
            (elm)->field.tqe_prev;                              \
    else                                                        \
        (head)->tqh_last = (elm)->field.tqe_prev;               \
    *(elm)->field.tqe_prev = (elm)->field.tqe_next;             \
    QUEUEDEBUG_TAILQ_POSTREMOVE((elm), field);                  \
} while (/* CONSTCOND */0)

#define TAILQ_FOREACH(var, head, field)                         \
    for ((var) = ((head)->tqh_first);                           \
        (var);                                                  \
        (var) = ((var)->field.tqe_next))

#define TAILQ_FOREACH_SAFE(var, head, field, next)              \
    for ((var) = ((head)->tqh_first);                           \
        (var) != NULL && ((next) = TAILQ_NEXT(var, field), 1);  \
        (var) = (next))

#define TAILQ_FOREACH_REVERSE(var, head, headname, field)                \
    for ((var) = (*(((struct headname *)((head)->tqh_last))->tqh_last)); \
        (var);                                                           \
        (var) = (*(((struct headname *)((var)->field.tqe_prev))->tqh_last)))

#define TAILQ_FOREACH_REVERSE_SAFE(var, head, headname, field, prev)     \
    for ((var) = TAILQ_LAST((head), headname);                           \
        (var) && ((prev) = TAILQ_PREV((var), headname, field), 1);       \
        (var) = (prev))

#define TAILQ_CONCAT(head1, head2, field) do {                  \
    if (!TAILQ_EMPTY(head2)) {                                  \
        *(head1)->tqh_last = (head2)->tqh_first;                \
        (head2)->tqh_first->field.tqe_prev = (head1)->tqh_last; \
        (head1)->tqh_last = (head2)->tqh_last;                  \
        TAILQ_INIT((head2));                                    \
    }                                                           \
} while (/* CONSTCOND */0)

#define TAILQ_EMPTY(head)           ((head)->tqh_first == NULL)
#define TAILQ_FIRST(head)           ((head)->tqh_first)
#define TAILQ_NEXT(elm, field)      ((elm)->field.tqe_next)

#define TAILQ_LAST(head, headname)                              \
    (*(((struct headname *)((head)->tqh_last))->tqh_last))
#define TAILQ_PREV(elm, headname, field)                        \
    (*(((struct headname *)((elm)->field.tqe_prev))->tqh_last))

// ####################################################

/*
 * Circular queue definitions.
 */
#define QUEUEDEBUG_CIRCLEQ_HEAD(head, field)
#define QUEUEDEBUG_CIRCLEQ_ELM(head, elm, field)
#define QUEUEDEBUG_CIRCLEQ_POSTREMOVE(elm, field)

#define CIRCLEQ_HEAD(name, type)                                \
struct name {                                                   \
    struct type *cqh_first;/* first element */                  \
    struct type *cqh_last; /* last element */                   \
}

#define CIRCLEQ_HEAD_INITIALIZER(head)                          \
    { (void *)&head, (void *)&head }

#define CIRCLEQ_ENTRY(type)                                     \
struct {                                                        \
    struct type *cqe_next;      /* next element */              \
    struct type *cqe_prev;      /* previous element */          \
    (head)->cqh_first = (void *)(head);                         \
    (head)->cqh_last = (void *)(head);                          \
} while (/* CONSTCOND */0)

#define CIRCLEQ_INSERT_AFTER(head, listelm, elm, field) do {    \
    QUEUEDEBUG_CIRCLEQ_HEAD((head), field)                      \
    QUEUEDEBUG_CIRCLEQ_ELM((head), (listelm), field)            \
    (elm)->field.cqe_next = (listelm)->field.cqe_next;          \
    (elm)->field.cqe_prev = (listelm);                          \
    if ((listelm)->field.cqe_next == (void *)(head))            \
        (head)->cqh_last = (elm);                               \
    else                                                        \
        (listelm)->field.cqe_next->field.cqe_prev = (elm);      \
    (listelm)->field.cqe_next = (elm);                          \
} while (/* CONSTCOND */0)

#define CIRCLEQ_INSERT_BEFORE(head, listelm, elm, field) do {   \
    QUEUEDEBUG_CIRCLEQ_HEAD((head), field)                      \
    QUEUEDEBUG_CIRCLEQ_ELM((head), (listelm), field)            \
    (elm)->field.cqe_next = (listelm);                          \
    (elm)->field.cqe_prev = (listelm)->field.cqe_prev;          \
    if ((listelm)->field.cqe_prev == (void *)(head))            \
        (head)->cqh_first = (elm);                              \
    else                                                        \
        (listelm)->field.cqe_prev->field.cqe_next = (elm);      \
    (listelm)->field.cqe_prev = (elm);                          \
} while (/* CONSTCOND */0)

#define CIRCLEQ_INSERT_HEAD(head, elm, field) do {              \
    QUEUEDEBUG_CIRCLEQ_HEAD((head), field)                      \
    (elm)->field.cqe_next = (head)->cqh_first;                  \
    (elm)->field.cqe_prev = (void *)(head);                     \
    if ((head)->cqh_last == (void *)(head))                     \
        (head)->cqh_last = (elm);                               \
    else                                                        \
        (head)->cqh_first->field.cqe_prev = (elm);              \
    (head)->cqh_first = (elm);                                  \
} while (/* CONSTCOND */0)

#define CIRCLEQ_INSERT_TAIL(head, elm, field) do {              \
    QUEUEDEBUG_CIRCLEQ_HEAD((head), field)                      \
    (elm)->field.cqe_next = (void *)(head);                     \
    (elm)->field.cqe_prev = (head)->cqh_last;                   \
    if ((head)->cqh_first == (void *)(head))                    \
        (head)->cqh_first = (elm);                              \
    else                                                        \
        (head)->cqh_last->field.cqe_next = (elm);               \
    (head)->cqh_last = (elm);                                   \
} while (/* CONSTCOND */0)

#define CIRCLEQ_REMOVE(head, elm, field) do {                   \
    QUEUEDEBUG_CIRCLEQ_HEAD((head), field)                      \
    QUEUEDEBUG_CIRCLEQ_ELM((head), (elm), field)                \
    if ((elm)->field.cqe_next == (void *)(head))                \
        (head)->cqh_last = (elm)->field.cqe_prev;               \
    else                                                        \
        (elm)->field.cqe_next->field.cqe_prev =                 \
            (elm)->field.cqe_prev;                              \
    if ((elm)->field.cqe_prev == (void *)(head))                \
        (head)->cqh_first = (elm)->field.cqe_next;              \
    else                                                        \
        (elm)->field.cqe_prev->field.cqe_next =                 \
            (elm)->field.cqe_next;                              \
    QUEUEDEBUG_CIRCLEQ_POSTREMOVE((elm), field)                 \
} while (/* CONSTCOND */0)

#define	CIRCLEQ_FOREACH(var, head, field)                       \
    for ((var) = ((head)->cqh_first);                           \
        (var) != (const void *)(head);                          \
        (var) = ((var)->field.cqe_next))

#define	CIRCLEQ_FOREACH_REVERSE(var, head, field)               \
    for ((var) = ((head)->cqh_last);                            \
        (var) != (const void *)(head);                          \
        (var) = ((var)->field.cqe_prev))

/*
 * Circular queue access methods.
 */
#define CIRCLEQ_EMPTY(head)      ((head)->cqh_first == (void *)(head))
#define CIRCLEQ_FIRST(head)      ((head)->cqh_first)
#define CIRCLEQ_LAST(head)       ((head)->cqh_last)
#define CIRCLEQ_NEXT(elm, field) ((elm)->field.cqe_next)
#define CIRCLEQ_PREV(elm, field) ((elm)->field.cqe_prev)

#define CIRCLEQ_LOOP_NEXT(head, elm, field)                     \
    (((elm)->field.cqe_next == (void *)(head))                  \
        ? ((head)->cqh_first)                                   \
        : (elm->field.cqe_next))
#define CIRCLEQ_LOOP_PREV(head, elm, field)                     \
    (((elm)->field.cqe_prev == (void *)(head))                  \
        ? ((head)->cqh_last)                                    \
        : (elm->field.cqe_prev))

// ####################################################
#endif /* _TINY_QUEUE_H_ */
