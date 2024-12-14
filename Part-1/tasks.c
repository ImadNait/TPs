#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
typedef struct task
{
    int id;
    int priority;
    char status[20];
    char description[50];
    struct task *next;
} task;

task *creertache(int id, int priority, const char *status, const char *description)
{
    task *new = (task *)malloc(sizeof(task));
    new->id = id;
    new->priority = priority;
    strcpy(new->status, status);
    strcpy(new->description, description);
    new->next = NULL;
    return new;
}

void taskinsertion(task **list, int id, int priority, const char *status, const char *description)
{
    task *new = creertache(id, priority, status, description);
    if (*list == NULL || (*list)->priority > new->priority)
    {
        new->next = *list;
        *list = new;
        return;
    }
    task *temp = *list;
    while (temp->next != NULL && temp->next->priority <= new->priority)
    {
        temp = temp->next;
    }
    new->next = temp->next;
    temp->next = new;
    return;
}

void taskdeletion(task **list, int id)
{
    task *temp = *list;
    task *temp2 = NULL;
    if (temp->id == id)
    {
        *list = temp->next;
        free(temp);
        printf("la tache avec l identifiant ID %d a ete suprimee.\n", id);
        return;
    }
    while (temp != NULL && temp->id != id)
    {
        temp2 = temp;
        temp = temp->next;
    }
    if (temp == NULL)
    {
        printf("la tache avec l identifiant ID %d n existe pas.\n", id);
        return;
    }
    temp2->next = temp->next;
    free(temp);
    printf("la tache avec l identifiant ID %d a ete suprimee.\n", id);
    return;
}

void updatetaskstatus(task **list, int id, const char *nstatus)
{
    task *temp = *list;
    while (temp != NULL)
    {
        if (temp->id == id)
        {
            strcpy(temp->status, nstatus);
            printf("status de la tache avec id:'%d' a ete mis a jour en  '%s'.\n", id, nstatus);
            return;
        }
        temp = temp->next;
    }
    printf("la tache avec id:'%d' n existe pas, \n", id);
}

void displaytasks(task **list)
{
    if (*list == NULL)
    {
        printf("la liste est vide");
        return;
    }
    task *temp = *list;
    while (temp != NULL)
    {
        if (strcmp(temp->status, "pending") == 0)
        {
            printf("ID: %d,priority:%d Description: %s, Status: %s\n",
                   temp->id, temp->priority, temp->description, temp->status);
        }
        temp = temp->next;
    }

    temp = *list;
    while (temp != NULL)
    {
        if (strcmp(temp->status, "inprogress") == 0)
        {
            printf("ID: %d,priority:%d Description: %s, Status: %s\n",
                   temp->id, temp->priority, temp->description, temp->status);
        }
        temp = temp->next;
    }

    temp = *list;
    while (temp != NULL)
    {
        if (strcmp(temp->status, "completed") == 0)
        {
            printf("ID: %d,priority:%d Description: %s, Status: %s\n",
                   temp->id, temp->priority, temp->description, temp->status);
        }
        temp = temp->next;
    }
}

void searchbypriority(task **list, int priority)
{
    task *temp = *list;
    printf("\nTasks with priority %d:\n", priority);
    while (temp != NULL)
    {
        if (temp->priority == priority)
        {
            printf("ID: %d,priority:%d Description: %s, Status: %s\n",
                   temp->id, temp->priority, temp->description, temp->status);
        }
        temp = temp->next;
    }
}

int main()
{
    int n;
    int i;
    int id;
    int priority;
    char status[20];
    char description[50];
    int priority2;
    char status2[20];
    int id2;
    int id3;
    int choix;

    task *list = NULL;
    do
    {
        printf("saisir le nombre de tache: ");
        scanf("%d", &n);
    } while (n < 1);
    for (i = 0; i < n; i++)
    {
        printf("inserez l identifiant: ");
        scanf("%d", &id);
        printf("inserez l'ordre de priorite: ");
        do
        {
            scanf("%d", &priority);
        } while (priority < 1 || priority > 5);
        do
        {
            printf("inserez le status de la tache (pending, completed, inprogress): ");
            scanf("%s", &status);
            if (strcmp(status, "pending") == 0 || strcmp(status, "completed") == 0 || strcmp(status, "inprogress") == 0)
            {
                break;
            }
            else
            {
                printf("Mot invalide. Essayez à nouveau.\n");
            }
        } while (1);
        printf("inserez la description de la tache :");
        scanf("%s", &description);
        taskinsertion(&list, id, priority, status, description);
    }
    bool check = true;
    while(check){
    printf("veuillez selectioner l'une des operations suivantes:\n1.inserer une nouvelle tache.\n2.recherche par numero de priorite.\n3.changement de status d'une tache.\n4.supression de tache.\n5.afficher la liste de taches.\n6.Quitter\n select:");
    do
    {
        scanf("%d", &choix);
    } while (choix < 1 || choix > 6);
    if (choix == 1)
    {

        printf("inserez l identifiant: ");
        scanf("%d", &id);
        printf("inserez l'ordre de priorite: ");
        do
        {
            scanf("%d", &priority);
        } while (priority < 1 || priority > 5);
        do
        {
            printf("inserez le status de la tache (pending, completed, inprogress): ");
            scanf("%s", &status);
            if (strcmp(status, "pending") == 0 || strcmp(status, "completed") == 0 || strcmp(status, "inprogress") == 0)
            {
                break;
            }
            else
            {
                printf("Mot invalide. Essayez à nouveau.\n");
            }
        } while (1);
        printf("inserez la description de la tache: ");
        scanf("%s", &description);
        taskinsertion(&list, id, priority, status, description);
        printf("tache ajouter a la liste \n");
        sleep(1);
        printf("Affichage des taches de la liste");
        sleep(1);
        printf(".");
        sleep(1);
        printf(".");
        sleep(1);
        printf(".\n");
        sleep(1);
        displaytasks(&list);
        sleep(5);
    }
    else if (choix == 2)
    {
        printf("inserez le numero de priorite des taches recherches: ");
        do
        {
            scanf("%d", &priority2);
        } while (priority2 < 1 || priority2 > 5);
        sleep(1);
        printf("\n.");
        sleep(1);
        printf(".");
        sleep(1);
        printf(".");
        sleep(1);
        searchbypriority(&list, priority2);
        sleep(5);
    }
    else if (choix == 3)
    {
        printf("inserez l identifiant de la tache que vous voudrez changer le status: ");
        scanf("%d", &id2);

        do
        {
            printf("inserez le nouveau status de la tache (pending, completed, inprogress): ");
            scanf("%s", &status2);
            if (strcmp(status2, "pending") == 0 || strcmp(status2, "completed") == 0 || strcmp(status2, "inprogress") == 0)
            {
                break;
            }
            else
            {
                printf("Mot invalide. Essayez à nouveau.\n");
            }
        } while (1);

        updatetaskstatus(&list, id2, status2);
        sleep(5);
    }
    else if (choix == 4)
    {

        printf("inserez l identifiant de la tache que vous voudrez supprimer: ");
        scanf("%d", &id3);
        taskdeletion(&list, id3);
        sleep(1);
        printf("\n.");
        sleep(1);
        printf(".");
        sleep(1);
        printf(".");
        sleep(1);
        printf("\nvoici les taches de la liste \n");
        displaytasks(&list);
        sleep(5);
    }
    else if (choix == 5)
    {

        printf("voici les taches de la liste \n");
        displaytasks(&list);
        sleep(5);
    }
    else if (choix == 6)
    {
    sleep(1);
    printf("Merci de votre visite revenez au plus vite :)");
    sleep(2);
    check = false;

    }}
    return 0;
}