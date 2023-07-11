#ifndef __MLIST_H__
#define __MLIST_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"

#define MTRUE      (1)
#define MFALSE     (0)

/**
 * @brief 获取某个结构体在成员中的地址偏移量
 * 
 * @param T 结构体类型
 * @param member 结构体成员名
 */
#define M_OFFSET_OF(T, member)          (long)((char *)&(((T *)(0))->member))

/**
 * @brief 通过指定成员的地址获取结构体地址
 * 
 * @param ptr 当前成员地址
 * @param T 结构体类型
 * @param member 结构体成员名
 */
#define M_GET_OBJECT(ptr, T, member)    (T *)(((char *)(ptr)) - M_OFFSET_OF(T, member))

typedef struct mlist {
    struct mlist *prev;
    struct mlist *next;
} MLIST;

#define MLIST_POISON        ((struct mlist *)0)

static inline void __mlist_add(MLIST *p_list, MLIST *p_node)
{
    p_node->prev = p_list;
    p_node->next = p_list->next;
    p_list->next->prev = p_node;
    p_list->next = p_node;
}

static inline void __mlist_del(MLIST *p_node)
{
    p_node->prev->next = p_node->next;
    p_node->next->prev = p_node->prev;
}


/**
 * @brief 静态声明并初始化MLIST节点
 * 
 * @param list 要声明的节点名
 */
#define MLIST_INITIALIZE(list) \
            MLIST list = { &(list) , &(list) }


/**
 * @brief 初始化MLIST节点
 * 
 * @param p_list 待初始化的节点
 */
static inline void MLIST_INIT(MLIST *p_list)
{
    p_list->next = p_list;
    p_list->prev = p_list;
}


/**
 * @brief 反初始化某个节点
 * 
 * @param p_list 待重初始化的节点
 */
static inline void MLIST_DEINIT(MLIST *p_list)
{
    __mlist_del(p_list);
    p_list->next = MLIST_POISON;
    p_list->prev = MLIST_POISON;
}


/**
 * @brief 判断以链表是否为空
 * 
 * @param p_list MLIST节点
 * @return int 
 *              MTRUE  : 链表为空
 *              MFALSE : 链表不为空
 */
static inline int MLIST_EMPTY(MLIST *p_list)
{
    return (p_list->next == p_list)?(MTRUE):(MFALSE);
}


/**
 * @brief 将某个节点插入到链表中
 * 
 * @param p_list 待插入的链表
 * @param p_node 待插入的节点
 */
static inline void MLIST_ADD(MLIST *p_list, MLIST *p_node)
{
    __mlist_add(p_list, p_node);
}


/**
 * @brief 将某个节点插入到链表中(尾插)
 * 
 * @param p_list 待插入的链表
 * @param p_node 待插入的节点
 */
static inline void MLIST_ADD_TAIL(MLIST *p_list, MLIST *p_node)
{
    __mlist_add(p_list->prev, p_node);
}


/**
 * @brief 删除某个节点
 * 
 * @param p_node 待删除的节点
 */
static inline void MLIST_DEL(MLIST *p_node)
{
    __mlist_del(p_node);
    p_node->prev = MLIST_POISON;
    p_node->next = MLIST_POISON;
}


/**
 * @brief 获取链表中的第一个成员
 * 
 * @param p_list 链表头
 */
#define MLIST_FIRST(p_list) ((p_list)->next)


/**
 * @brief 遍历链表
 * 
 * @param p_item 指针, 用于获取每个节点
 * @param p_list 链表头
 */
#define MLIST_FOR_EACH(p_item, p_list) \
    for ((p_item) = (p_list)->next; (p_item) != (p_list); (p_item) = (p_item)->next)


/**
 * @brief 遍历链表
 * 
 * @param p_item 指针, 用于获取每个节点
 * @param p_n 指针, 用于暂存下一个节点
 * @param p_list 链表头
 */
#define MLIST_FOR_EACH_SAFE(p_item, p_n, p_list) \
    for ((p_item) = (p_list)->next, (p_n) = (p_item)->next; (p_item) != (p_list); \
            (p_item) = (p_n), (p_n) = (p_item)->next)


/**
 * @brief 获取链表中第一个节点所属的对象
 * 
 * @param T 对象类型
 * @param p_list 链表头
 * @param member 节点成员名
 */
#define MLIST_FIRST_ENTRY(T, p_list, member) M_GET_OBJECT((p_list)->next, T, member)


/**
 * @brief 遍历链表, 获取每个对象
 * 
 * @param p_obj 指针, 用于获取每一个对象
 * @param T 对象类型
 * @param p_list 链表头
 * @param member 节点成员名
 */
#define MLIST_FOR_EACH_ENTRY(p_obj, T, p_list, member)       \
    for((p_obj) = M_GET_OBJECT((p_list)->next, T, member);      \
        &((p_obj)->member) != (p_list);                           \
        (p_obj) = M_GET_OBJECT((p_obj)->member.next, T, member))


/**
 * @brief 遍历链表, 获取每个对象
 * 
 * @param p_obj 指针, 用于获取每一个对象
 * @param p_n 指针, 用于暂存下一个对象
 * @param T 对象类型
 * @param p_list 链表头
 * @param member 节点成员名
 */
#define MLIST_FOR_EACH_ENTRY_SAFE(p_obj, p_n, T, p_list, member)   \
    for((p_obj) = M_GET_OBJECT((p_list)->next, T, member),          \
        (p_n) = M_GET_OBJECT((p_obj)->member.next, T, member);        \
        &((p_obj)->member) != (p_list);                               \
        (p_obj) = (p_n), (p_n) = M_GET_OBJECT((p_obj)->member.next, T, member))

#ifdef __cplusplus
}
#endif

#endif /* __MLIST_H__ */
