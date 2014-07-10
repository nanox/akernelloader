/*  Akernelloader TEAM
    akaloaderadmin@gmail.com

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. 
 */

#ifndef __FAT_LIST_H__
#define __FAT_LIST_H__

#ifndef FAT_ASSERT
    #define FAT_ASSERT(x)
#endif



//-----------------------------------------------------------------
// Types
//-----------------------------------------------------------------
struct fat_list;

struct fat_node
{
    struct fat_node    *previous;
    struct fat_node    *next;
};

struct fat_list
{
    struct fat_node    *head;
    struct fat_node    *tail;
};

//-----------------------------------------------------------------
// Macros
//-----------------------------------------------------------------
#define fat_list_entry(p, t, m)     p ? ((t *)((char *)(p)-(char*)(&((t *)0)->m))) : 0
#define fat_list_next(l, p)         (p)->next
#define fat_list_prev(l, p)         (p)->previous
#define fat_list_first(l)           (l)->head
#define fat_list_last(l)            (l)->tail
#define fat_list_for_each(l, p)     for ((p) = (l)->head; (p); (p) = (p)->next)

void fat_list_init(struct fat_list *list);
void fat_list_remove(struct fat_list *list, struct fat_node *node) ;
void fat_list_insert_after(struct fat_list *list, struct fat_node *node, struct fat_node *new_node) ;
void fat_list_insert_before(struct fat_list *list, struct fat_node *node, struct fat_node *new_node);
void fat_list_insert_first(struct fat_list *list, struct fat_node *node) ;
void fat_list_insert_last(struct fat_list *list, struct fat_node *node);
int fat_list_is_empty(struct fat_list *list);
struct fat_node * fat_list_pop_head(struct fat_list *list);

#endif

