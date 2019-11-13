#include "link_list_cell_t.h"

void link_list_cell_t_init(link_list_cell_t* link_list, int(*comp)(cell_t*, cell_t*)) {
	link_list->head = NULL;
	link_list->tail = NULL;
	link_list->comp = comp;
}

void link_list_cell_t_destroy(link_list_cell_t* link_list ) {
   node_cell_t* cur = link_list->head;
   node_cell_t* next = cur;
   while(cur) {
       next = cur->next;
       free(cur);
       cur = next;
   }
   link_list->head = NULL;
   link_list->tail = NULL;
}

int link_list_cell_t_empty(link_list_cell_t* link_list) {
	return (link_list->head == NULL);
}

node_cell_t* link_list_cell_t_new_node(cell_t* data) {
	node_cell_t* node = (node_cell_t*)malloc(sizeof(node_cell_t));
	node->prev = NULL;
	node->next = NULL;
	node->data = *data;
	return node;
}

node_cell_t* link_list_cell_t_find_ith(link_list_cell_t* link_list, size_t i) {
	node_cell_t* node = link_list->head;
	int j = 0;
	for(; (j < i) & (node != NULL);  ++j) {
		node = node->next;
	}
	return node;
}

cell_t* link_list_cell_t_get_ith(link_list_cell_t* link_list, size_t i) {
	node_cell_t* node = link_list_cell_t_find_ith(link_list, i);
	assert(node);
	return &(node->data);
}

cell_t link_list_cell_t_insert_after(link_list_cell_t* link_list, node_cell_t* target, cell_t* data) {
	node_cell_t* node = link_list_cell_t_new_node(data);
	if(link_list_cell_t_empty(link_list)) {
		link_list->head = node;
		link_list->tail = node;
		node->prev = NULL;
		node->next = NULL;
	}
	else if(!target || (target == link_list->tail)) {
		node->prev = link_list->tail;
		node->next = NULL;
		link_list->tail->next = node;
		link_list->tail = node;
	}
	else {
		node->next = target->next;
		node->prev = target;
		target->next->prev = node;
		target->next = node;
	}
}

void link_list_cell_t_insert_after_ith(link_list_cell_t* link_list, size_t i, cell_t* data) {
	node_cell_t* target = link_list_cell_t_find_ith(link_list, i);
	//assert(target);
	link_list_cell_t_insert_after(link_list, target, data);
}

void link_list_cell_t_push(link_list_cell_t* link_list, cell_t* data) {
	node_cell_t* target = link_list->tail;
	if(link_list->comp != NULL) {
		target = link_list->head;
		while(target && link_list->comp(data, &(target->data))) {
			target = target->next;
		}
	}
	link_list_cell_t_insert_after(link_list, target, data);
}

void link_list_cell_t_remove(link_list_cell_t* link_list, node_cell_t* node) {
	if(node == link_list->head && node == link_list->tail) {
		link_list->head = NULL;
		link_list->tail = NULL;
	}
	else if(node == link_list->head) {
		link_list->head = node->next;
		link_list->head->prev = NULL;
	}
	else if(node == link_list->tail) {
		link_list->tail = node->prev;
		link_list->tail->next = NULL;
	}
	else {
		node->prev->next = node->next;
		node->next->prev = node->prev;
	}
	free(node);
}

cell_t link_list_cell_t_remove_front(link_list_cell_t* link_list) {
	cell_t data = link_list->head->data;
	link_list_cell_t_remove(link_list, link_list->head);
	return data;
}

void link_list_cell_t_remove_ith(link_list_cell_t* link_list, size_t i) {
	node_cell_t* node = link_list_cell_t_find_ith(link_list, i);
	if(node) {
		link_list_cell_t_remove(link_list, node);
	}
}

cell_t link_list_cell_t_remove_back(link_list_cell_t* link_list) {
	cell_t data = link_list->tail->data;
	link_list_cell_t_remove(link_list, link_list->head);
	return data;
}

void link_list_cell_t_update(link_list_cell_t* link_list, void(*update)(node_cell_t*)) {
	node_cell_t* cur = link_list->head;
	while(cur) {
		update(cur);
		cur = cur->next;
	}
}

void link_list_cell_t_swap(node_cell_t* a, node_cell_t* b) {
	node_cell_t tmp = *b;
	b->data = a->data;
	a->data = tmp.data;
}

void link_list_cell_t_sort(link_list_cell_t* link_list) {
	if(link_list->head == link_list->tail) {
		return;
	}
	node_cell_t* start = link_list->head;
	node_cell_t* cur = start->next;
	node_cell_t* min = start;
	while(1) {
		while(cur) {
			if(link_list->comp(&(cur->data), &(min->data))) {
				min = cur;
			}
			cur = cur->next;
		}
		link_list_cell_t_swap(start, min);
		if(!start->next) {
			break;
		}
		start = start->next;
		cur = start->next;
		min = start;
	}
}
