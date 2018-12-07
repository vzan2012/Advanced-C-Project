/*** France.c source code. By Sitharaman Deepak Guptha. November 2018 ***/

#include "France.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

static Node* available = 0;

/*** This function is used to create a list ***/
List* newList(compFun comp,prFun pr) {
	List* list = malloc(sizeof(List));
	if (list == NULL) {
		return 0;
    }
    else {
	list->nelts = 0;
	list->head = NULL;
	list->comp = comp;
	list->pr = pr;
	return list;
    }
}

/*** This function is used to build a City ***/
City * BuildCity(char* name, int lat, int lgt){
    City * ct = malloc(sizeof(City));
    copy_string(ct->name,name);

    ct->lati = lat;
    ct->loni = lgt;
    ct->Startdistance = -HUGE_VAL;
    ct->neighbours = newList(Neighbcmp, CitDisp);
    ct->neighbours->comp=Neighbcmp;
    //c->link = 0;
    return ct;
}

/*** This function is used to delete a list ***/
void delList(List* l) {
	Node* n1 = l->head;
	while (n1 != NULL) {
		Node* n2 = n1->next;
		free(n1);
		n1 = n2;
	}
	free(l);
}

/*** To find the city in the list ***/
City * lookupCity(List * list,char name[100]){
    if(!list->head) return NULL;
    Node * nd = list->head;
    while(nd){

        if(strcmp(((City *)(nd->val))->name, name) == 0){
            return nd->val;
        }
        nd=nd->next;
    }
    return 0;
}

/*** Retrieve the Nth element from the List ***/
status nthInList(List* l,int n,void** e) {
	Node* nl = l->head;
	while (n--)	{
		if (nl == NULL) {
			break;
        }
		nl = nl->next;
	}
	if (nl == NULL) {
		return ERRINDEX;
	}
	*e = nl->val;
	return OK;
}

/*** Used to display the City with Latitude and Longitude ***/
void CitDisp(void * disp){
    City * dp = disp;
    printf("\n%s  %d  %d  %d  \t", dp->name,dp->lati,dp->loni,dp->neighbours->nelts);
    forEach(dp->neighbours, NeigDisp);
}

/*** To insert element at a given position in the list (O(N)). ***/
status addListAt (List* l, int pos, void* elt) {
    if (pos <= 0 || pos > l->nelts+1)
        return ERRINDEX;
    else {

        /* get a new Node and increment length */
        Node * toAdd = available;
        if (!toAdd) toAdd = (Node*) malloc(sizeof(Node));
        else available = available->next;
        if (!toAdd) return ERRALLOC;
        l->nelts++;
        toAdd->val = elt;

        /* if pos is 1, must change list head */
        if (pos == 1) {
            toAdd->next = l->head;
            l->head = toAdd;
        }

        /* otherwise we need a temporary pointer */
        else {
            Node * tmp = l->head;

            /* tmp points to the predecessor */
            while (pos-- > 2) tmp = tmp->next;

            /* actual insertion */
            toAdd->next = tmp->next;
            tmp->next = toAdd;
        }
        return OK;
    }
}

/*** Comparing the distance between the Neighboring cities ***/
int Neighbcmp(void * nbA, void * nbB) {
    int cp = ((Neighbours*)nbA)->distance + (~((Neighbours*)nbB)->distance + 1);
    return cp;
}

/*** To remove the element located at a given position in list (O(N)). ***/
status remFromListAt	(List* l,int pos, void** res) {
    Node *toRem = l->head;

    if (pos <= 0 || pos > l->nelts) return ERRINDEX;

    /* particular case: must remove list head */

    if (pos == 1)
        l->head = toRem->next;

    /* otherwise point to predecessor to change links */
    else {
        Node * prec = toRem;
        while (pos-- > 2) prec = prec->next;
        toRem = prec->next;
        prec->next = toRem->next;
    }

    *res = toRem->val;
    toRem->next = available;
    available = toRem;
    l->nelts--;
    return OK;
}

/*** Compare the two elements ***/
int Eltcmp (void * stA, void * stB) {

    if (!stA && !stB) return 0;
    if (stA && !stB) return 1;
    if (!stA && stB) return -1;

    while (stA && stB && stA == stB) stA++, stB++;

    if (!stA && !stB) return 0;
    else if (stA > stB) return 1;
    else return -1;
}

/*** Remove given element from given list (O(N)). ***/
status remFromList(List* l,void* e) {
	Node* nl = l->head;
	if (nl == NULL) {
		return ERRABSENT;
    }
	if (nl->val == e) {
		l->head = nl->next;
	} else {
		Node* n2 = nl;
		nl = nl->next;
		while (nl != NULL) {
			if (nl->val == e) {
				n2->next = nl->next;
				break;
			}
			n2 = nl;
			nl = nl->next;
		}
		if (nl == NULL) {
			return ERRABSENT;
		}
	}
	free(nl);
	l->nelts--;
	return OK;
}

/*** Display the Neighbor city and distance ***/
void NeigDisp(void * disp){
    Neighbours * nbs = disp;
    printf("%s %d\n",nbs->city->name,nbs->distance);
}

/*** Display list elements as "[ e1 ... eN ]" (O(N)). ***/
status displayList(List* l) {
	printf("[ ");
	Node* nl = l->head;
	while (nl != NULL) {
		(*l->pr)(nl->val);
		putchar(' ');
		nl = nl->next;
	}
	printf("]");
}

/*** This method is used to find the distance between two cities ***/
int DisttwoCity(void * citA, void * citB) {
    int cp = ((City *)citA)->Startdistance + (~((City *)citA)->Startdistance + 1);
    return cp;
}

/*** To Sequencially call given function with each element of given list ***/
void forEach(List* l,void(*f)(void*)) {
	Node* nl = l->head;
	while (nl != NULL) {
		(*f)(nl->val);
		nl = nl->next;
	}
}

/*** Compute and return the number of elements in given list (O(1)) ***/
int lengthList(List* l) {
	return l->nelts;
}

/*** Calculating the distance between Rennes to Lyon ***/
int rennesTolyon(City * ct,City * finish){
    float distance;
    distance = sqrt((ct->lati - finish->lati) * (ct->lati - finish->lati) + (ct->loni - finish->loni) * (ct->loni - finish->loni));
    return distance;
}

/**
 * Private function to get the node preceding the one we're looking for
 * @param e the searched element
 * @return 0 if element is not found (typically, list is empty)
 * @return 1 if element is head of list
 * @return a pointer to the predecessor otherwise
 */
static Node* previous(List* l, void* e) {
    Node * prec = l->head;
    if (l->nelts == 0) return 0;
    if (!(l->comp)(e,prec->val)) return (Node*)1;

    /* prec must address element prior to the one to remove */
    while (prec->next && (l->comp)(e,prec->next->val))
        prec = prec->next;

    /* here,
     *  - either we address nothing (no such element)
     *  - or we address the element prior to the one we're looking for
     */
    return prec;
}

/*** Go to the current city ***/
int curr_to_go(City * ct){

    while(ct->Startdistance) {
        int t = (ct->Startdistance & ct->Finishdistance) <<1;
        ct->Finishdistance ^= ct->Startdistance;
        ct->Startdistance = t;
    }
    return ct->Finishdistance;
}

/*** Insert element at a given position in the list (O(N)). ***/
status addList(List* l,void* e) {
	Node* n1 = malloc(sizeof(Node));
	if (n1 == NULL)
		return ERRALLOC;
	n1->val = e;
	Node* n2 = l->head;
	if (n2 == NULL) {
		l->head = n1;
		n1->next = NULL;
	} else if ((*l->comp)(e, n2->val) <= 0) {
		n1->next = l->head;
		l->head = n1;
	} else {
		Node* prev = n2;
		n2 = n2->next;
		while (n2 != NULL) {
			if ((*l->comp)(e, n2->val) <= 0) {
				break;
			}
			prev = n2;
			n2 = n2->next;
		}
		prev->next = n1;
		n1->next = n2;
	}
	l->nelts++;
	return OK;
}

/*** To Test whether the list contains the given element ***/
Node* isInList(List* l,void* e) {
	Node* nl = l->head;
	while (nl != NULL) {
		if (e == nl->val) {
			return (Node*)1;
        }
		nl = nl->next;
	}
	return 0;
}

/*** Comparing the distance between two cities ***/
int citycmp(void * ct1, void * ct2){
    City * c1 = ct1;
    City * c2 = ct2;
    return (curr_to_go(c1)+curr_to_go(c2))/2;
}

/*** Used to Copy string ***/
void copy_string(char d[], char s[]) {
   int c = 0;

   while (s[c] != '\0') {
      d[c] = s[c];
      c++;
   }
   d[c] = '\0';
}

/*** To create a Neighbor city ***/
Neighbours * CreateNeighbour(City * city, int distance){
    Neighbours * nb = malloc(sizeof(Neighbours));
    nb->city = city;
    nb->distance = distance;
    return nb;
}
