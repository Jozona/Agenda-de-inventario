#include "Ordenamiento.h"


//No tengo ni la menor idea de porque el hepify jala pero JALAAAAAAAAAA
void Ordenar::ejecutar() {
    Carnet* origen;
    Carnet* aux;
    origen = aux = NULL;

    agregarNodo(origen, aux, 4);
    agregarNodo(origen, aux, 3);
    agregarNodo(origen, aux, 2);
    agregarNodo(origen, aux, 1);

    encuentraNodo(origen, 2);

    node_swap(origen->next, aux->next->next->next);
    while (origen->prev != NULL) {
        origen = origen->prev;
    }

    int totalNodos = 0;
    while (aux != NULL) {
        aux = aux->next;
        totalNodos++;
    }
    heapSort(origen, totalNodos);
    while (origen->prev != NULL) {
        origen = origen->prev;
    }
    int p = 2;
}

void Ordenar::agregarNodo(Carnet*& org, Carnet*& aux ,int numero) {
    if (org == NULL) {
        org = new Carnet;
        org->ID = numero;
        org->next = NULL;
        org->prev = NULL;
        aux = org;
    }
    else {
        while (aux->next != NULL) {
            aux = aux->next;
        }
        aux->next = new Carnet;
        aux->next->prev = aux;
        aux = aux->next;
        aux->ID = numero;
        aux->next = NULL;
        aux = org;
    }
}

void Ordenar::estanJuntos(struct Carnet* left, struct Carnet* right) {
    struct Carnet* tmp;

    tmp = left->prev;
    if (tmp)
        tmp->next = right;
    right->prev = tmp;

    left->prev = right;
    left->next = right->next;
    right->next = left;
    if (left->next != NULL)
        left->next->prev = left;

}

void Ordenar::node_swap(struct Carnet* i, struct Carnet* j) {
    struct Carnet* tmp;
    struct Carnet* tmp2;
    struct Carnet* tmp3;

    //Esto es en caso de que esten juntos!
    //Apunta hacia atras de i y si no es nulo, redirije hacia el nuevo nodo
    //Despues conecta el nuevo nodo con el prev anterior de i!
    tmp2 = j->prev;
    tmp3 = j->next;
    tmp = i->prev;

    if (i->next == j) {
        estanJuntos(i, j);
        return;
    }

    if (tmp)
        tmp->next = j;
    j->prev = tmp;

    //Ahora tenemos que hacerlo desde la derecha!!!
    tmp = i->next;
    if (tmp)
        tmp->prev = j;
    j->next = tmp;


    //Repetimos el mismo paso pero en el otro nodo!
    if (tmp2)
        tmp2->next = i;
    i->prev = tmp2;

    if (tmp3)
        tmp3->prev = i;
    i->next = tmp3;

}

void Ordenar::heapify(Carnet* arr, int iteration) {
    int par, node;
    for (int i = 0; i <= iteration; i++) {
        node = i;
        par = (node == 0) ? 0 : (int)node / 2;
        while (par >= 0 && node != 0) {
            while (arr->prev != NULL) {
                arr = arr->prev;
            }
            Carnet* tmpPar = encuentraNodo(arr, par);
            while (arr->prev != NULL) {
                arr = arr->prev;
            }
            Carnet* tmpNode = encuentraNodo(arr, node);
            if (tmpPar->id < tmpNode->id)
                node_swap(tmpPar, tmpNode);
            node = par;
            par = (node == 0) ? 0 : (int)node / 2;
        }
    }
}

void Ordenar::heapSort(Carnet* arr, int size) {
    for (int i = size - 1; i >= 0; i--) {
        while (arr->prev != NULL) {
            arr = arr->prev;
        }
        heapify(arr, i);
        while (arr->prev != NULL) {
            arr = arr->prev;
        }
        Carnet* temp = encuentraNodo(arr, i);
        while (arr->prev != NULL) {
            arr = arr->prev;
        }
        node_swap(arr, temp);
    }
}

Carnet* Ordenar::encuentraNodo(Carnet* org, int posicion) {
    Carnet* temp = org;
    for (int i = 0; i < posicion; i++) {
        temp = temp->next;
    }
    return temp;
}