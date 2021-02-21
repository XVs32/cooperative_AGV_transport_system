#include <stdio.h>


void add_to_ll(int_node *head, int_node *target, int mode){
    
    int_node *cur = head;
    int_node *prev = head;
    
    switch(mode){
        case ASCENDING;
            if(cur->val > target->val){
                target->next = head;
                head = target;
                break;
            }
            if(cur != NULL){
                cur = cur->next;
            }
            while(cur != NULL){
                if(cur->val > target->val){
                    target->next = cur;
                    prev->next = target;
                    break;
                }
                prev = cur;
                cur = cur->next;
            }
            if(cur == NULL){
                target->next = NULL;
                prev->next = target;
            }
            break;
        case DESCENDING:
            if(cur->val < target->val){
                target->next = head;
                head = target;
                break;
            }
            if(cur != NULL){
                cur = cur->next;
            }
            while(cur != NULL){
                if(cur->val < target->val){
                    target->next = cur;
                    prev->next = target;
                    break;
                }
            }
            if(cur == NULL){
                prev = cur;
                cur = cur->next;
            }
            
            break;
        case TO_HEAD:
            target->next = head;
            head = target;
            break;
    }
    return;
}

