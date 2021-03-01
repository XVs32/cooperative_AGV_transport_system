#include <stdio.h>
#include <stdlib.h>
#include "linklist.h"

void int32_add_to_ll(int32_node *head, int32_node *target, int mode){
    
    int32_node *cur = head;
    int32_node *prev = head;
    
    switch(mode){
        case ASCENDING:
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
        case TO_TAIL:
            target->next = NULL;
            if(cur==NULL){
                head = target;
                break;
            }
            while(cur->next!=NULL){
                cur = cur->next;
            }
            cur->next = target;
            break;
        default:
            printf("Error: undefined int32_add_to_ll mode %d, exit.\n", mode);
            exit(1);
    }
    return;
}

void u_int16_add_to_ll(uint16_node *head, uint16_node *target, int mode){
    
    uint16_node *cur = head;
    uint16_node *prev = head;
    
    switch(mode){
        case ASCENDING:
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
        case TO_TAIL:
            target->next = NULL;
            if(cur==NULL){
                head = target;
                break;
            }
            while(cur->next!=NULL){
                cur = cur->next;
            }
            cur->next = target;
            break;
        default:
            printf("Error: undefined u_int16_add_to_ll mode %d, exit.\n", mode);
            exit(1);
    }
    return;
}

