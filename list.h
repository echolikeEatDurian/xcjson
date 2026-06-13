//
// Created by echo on 2026/6/13.
//

#ifndef CJSON_FORK_LIST_H
#define CJSON_FORK_LIST_H
#include <stddef.h>

struct list_head {
    struct list_head *next;
    struct list_head *prev;
};

#define LIST_HEAD_INIT(name) { &(name),&(name)}
#define LIST_HEAD(name) \
        struct list_head name = LIST_HEAD_INIT(name)

static inline void init_list_head(struct list_head *list) {
    list->next = list;
    list->prev = list;
}

static inline void __list_add(struct list_head *new, struct list_head *prev, struct list_head *next) {
    new->prev = prev;
    new->next = next;
    prev->next = new;
    next->prev = new;
}

static inline void list_add(struct list_head *new, struct list_head *head) {
    __list_add(new, head, head->next);
}

static inline void list_add_tail(struct list_head *new, struct list_head *head) {
    __list_add(new, head->prev, head);
}


static inline void __list_del(struct list_head *prev, struct list_head *next) {
    prev->next = next;
    next->prev = prev;
}

static inline void list_del(struct list_head *entity) {
    __list_del(entity->prev, entity->next);
    entity->next = NULL;
    entity->prev = NULL;
}

static inline int list_empty(const struct list_head *head) {
    return head->next == head;
}

#define container_of(ptr, type, member) \
((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))

#define list_entry(ptr, type, member) \
container_of(ptr, type, member)

#define list_for_each(pos,head) \
        for(pos = (head)->next ; pos != (head) ; pos = pos->next )

#define  list_for_each_safe(pos,n,head) \
        for(pos=(head)->next,n = pos->next; \
            pos != (head); \
            pos = n, n = pos->next)

#endif //CJSON_FORK_LIST_H
